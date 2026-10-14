[CmdletBinding()]
param([string]$Stage='Observed')
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
function Get-Hash([byte[]]$bytes) {
    [bigint]$hash=[bigint]::Parse('14695981039346656037')
    foreach($byte in $bytes){$hash=(($hash -bxor $byte)*1099511628211) -band [bigint]::Parse('18446744073709551615')}
    return [uint64]$hash
}
function Get-StringHash([string]$value){return Get-Hash ([Text.Encoding]::ASCII.GetBytes($value))}
function Get-SequenceSignature($native,[string]$map,[bool]$canonical) {
    $keys=@(foreach($instance in $native.instances|Where-Object playerChannel -ge 0) {
        if(!$instance.asset -or !$instance.subscene -or $instance.state -notin @(3,5)){throw 'Native sequence is not stably playing.'}
        $subscene=$instance.subscene
        if($canonical -and $map -eq 'campaign/dunwall/escape/tower/dunwall_escape_tower_p' -and
           $instance.asset -eq 'midnight/campaign/dunwall/escape/tower/coup_main_post_fight.midnightscene' -and
           $instance.sceneDurationMs -eq 28000 -and $subscene -eq 'player_is_emily'){$subscene='player_is_corvo'}
        [uint32]$mask=0
        if($native.channels[0] -eq $instance.id){$mask=$mask -bor 1}
        if($native.channels[1] -eq $instance.id){$mask=$mask -bor 2}
        [pscustomobject]@{Asset=(Get-StringHash $instance.asset);Subscene=(Get-StringHash $subscene);Duration=[uint32]$instance.sceneDurationMs;Channels=$mask}
    })
    if(!$keys.Count){if($native.channels[0] -or $native.channels[1]){throw 'Native player sequence handle unresolved.'};return '0'}
    if($keys.Count -gt 2){throw 'Unexpected native sequence count.'}
    $bytes=[Collections.Generic.List[byte]]::new()
    $bytes.AddRange([BitConverter]::GetBytes([uint32]$keys.Count))
    foreach($key in $keys|Sort-Object Asset,Subscene,Duration,Channels) {
        $bytes.AddRange([BitConverter]::GetBytes([uint64]$key.Asset));$bytes.AddRange([BitConverter]::GetBytes([uint64]$key.Subscene))
        $bytes.AddRange([BitConverter]::GetBytes([uint32]$key.Duration));$bytes.AddRange([BitConverter]::GetBytes([uint32]$key.Channels))
    }
    return (Get-Hash $bytes.ToArray()).ToString('x')
}
$states=@{}
foreach($role in 'host','client') {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected game process.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale status.'}
    $status=Get-Content $file.FullName -Raw|ConvertFrom-Json
    $native=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch.pid --midnight
    if($LASTEXITCODE){throw 'Independent native sequence inspection failed.'}
    $native=$native|ConvertFrom-Json
    $signature=Get-SequenceSignature $native $status.localMap $false
    $boundary=Get-SequenceSignature $native $status.localMap $true
    $count=@($native.instances|Where-Object playerChannel -ge 0).Count
    if($status.poseProtocol -notin @(6,7,8,9,10) -or !$status.remoteFresh -or $status.connectionState -ne 3 -or
       $status.localSequenceMetadataValid -ne 1 -or $status.localSequenceCount -ne $count -or
       $status.localSequenceSignature -ne $signature -or $status.localSequenceBoundaryId -ne $boundary){throw "Independent native sequence metadata differs from the $role packet."}
    $states[$role]=[pscustomobject]@{Status=$status;Native=$native;ExpectedSignature=$signature;ExpectedBoundary=$boundary}
}
foreach($role in 'host','client') {
    $other=if($role -eq 'host'){'client'}else{'host'}
    if($states[$other].Status.remoteSequenceMetadataValid -ne 1 -or
       $states[$other].Status.remoteSequenceSignature -ne $states[$role].ExpectedSignature -or
       $states[$other].Status.remoteSequenceBoundaryId -ne $states[$role].ExpectedBoundary){throw 'Native Steam peer metadata differs from independent sequence observations.'}
}
$entry=[pscustomobject]@{Stage=$Stage;Passed=$true;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;States=$states}
$record=Join-Path $PSScriptRoot 'research/native-sequence-identity-verification.json'
$entries=if(Test-Path $record){@((Get-Content $record -Raw|ConvertFrom-Json).Tests)}else{@()}
[ordered]@{Tests=@($entries)+$entry;Scope='Independent native player Midnight asset/subscene/channel observations and their Steam replication. Boundary equivalence covers only the exact observed opening Corvo/Emily pair.'}|ConvertTo-Json -Depth 12|Set-Content $record
[pscustomobject]@{Stage=$Stage;Passed=$true;HostBoundary=$states.host.ExpectedBoundary;ClientBoundary=$states.client.ExpectedBoundary}



