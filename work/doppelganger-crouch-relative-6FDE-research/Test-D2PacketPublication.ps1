$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'tooling/Publish-D2Packet.ps1')
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$directory=Join-Path $workspaceRoot ('work/packet-publication-'+[guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $directory|Out-Null
$target=Join-Path $directory 'input.bin'
Add-Type -TypeDefinition @'
using System.IO;
using System.Threading.Tasks;
public static class D2PacketSharingTest {
    public static Task Hold(string path, int milliseconds) {
        var handle = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);
        return Task.Run(async () => { try { await Task.Delay(milliseconds); } finally { handle.Dispose(); } });
    }
}
'@
$failure=$null;$checks=[Collections.Generic.List[string]]::new()
try {
    $initial=[byte[]](1,2,3);[IO.File]::WriteAllBytes($target,$initial)
    $packet=[byte[]](4,5,6,7)
    # Warm the function before opening the lock so the test exercises retries,
    # rather than spending the entire short lock interval in initial JIT work.
    Publish-D2Packet -Path $target -Bytes $initial -ExpiresUtc ([DateTime]::UtcNow.AddSeconds(5))
    $task=[D2PacketSharingTest]::Hold($target,250)
    $watch=[Diagnostics.Stopwatch]::StartNew()
    Publish-D2Packet -Path $target -Bytes $packet -ExpiresUtc ([DateTime]::UtcNow.AddSeconds(5)) -RetryWindowMs 500
    if($watch.ElapsedMilliseconds -lt 100 -or ([IO.File]::ReadAllBytes($target) -join ',') -ne ($packet -join ',')){throw 'Sharing retry did not publish intact bytes after release.'}
    $task.GetAwaiter().GetResult()|Out-Null;$checks.Add('Transient sharing failure retries and publishes complete packet')
    $handle=[IO.File]::Open($target,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
    try {
        $failed=$false;$watch.Restart()
        try {Publish-D2Packet -Path $target -Bytes $initial -ExpiresUtc ([DateTime]::UtcNow.AddSeconds(5)) -RetryWindowMs 40}catch{$failed=$true}
        if(!$failed -or $watch.ElapsedMilliseconds -gt 1000){throw 'Permanent lock did not fail within a bound.'}
        $failed=$false
        try {Publish-D2Packet -Path $target -Bytes $initial -ExpiresUtc ([DateTime]::UtcNow.AddMilliseconds(40)) -RetryWindowMs 500}catch{$failed=$_.Exception.Message -match 'expired'}
        if(!$failed){throw 'Publication did not stop at the original lease expiry.'}
    }finally{$handle.Dispose()}
    if(([IO.File]::ReadAllBytes($target) -join ',') -ne ($packet -join ',')){throw 'Failed publication changed the existing packet.'}
    $checks.Add('Permanent lock fails; existing packet preserved')
    $checks.Add('Original expiry is not extended during retry')
    if(@(Get-ChildItem -LiteralPath $directory -Filter '*.tmp').Count){throw 'Temporary packet leaked.'}
    $checks.Add('Temporary files cleaned after success and failure')
}catch{$failure=$_.Exception.Message}
[ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');Checks=$checks.ToArray()}|
    ConvertTo-Json -Depth 4|Set-Content (Join-Path $PSScriptRoot 'research/packet-publication-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count}
