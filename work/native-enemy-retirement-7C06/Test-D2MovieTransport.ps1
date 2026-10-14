[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$samples=@()
for($i=0;$i -lt 4;$i++) {
    if($i){Start-Sleep -Milliseconds 2000}
    $states=@{}
    foreach($role in 'host','client') {
        $root=Join-Path $workspaceRoot "work\lab-instances\$role"
        $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
        $gameProcess=Get-Process -Id $launch.pid -ErrorAction Stop
        if($gameProcess.Path -ne $launch.executable){throw 'Unexpected process path.'}
        $file=Get-Item (Join-Path $root 'coop-status.json')
        if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale transport status during movie.'}
        $status=Get-Content $file.FullName -Raw|ConvertFrom-Json
        if(!$status.remoteFresh -or $status.connectionState -ne 3){throw 'Native Steam peer unavailable during movie.'}
        $logic=& (Join-Path $PSScriptRoot 'bin\d2_runtime_probe.exe') $launch.pid --logic
        if($LASTEXITCODE){throw 'Native logic inspection failed.'}
        $logic=$logic|ConvertFrom-Json
        if($logic.stack[-1].id -ne $status.localGameLogicState -or $logic.pending -ne $status.localPendingGameLogicState){throw 'Native and transmitted local state differ.'}
        $states[$role]=[pscustomobject]@{Status=$status;Logic=$logic}
    }
    if($states.host.Status.localGameLogicState -ne 1 -or $states.client.Status.localGameLogicState -ne 15 -or $states.client.Status.localPendingGameLogicState -ne 5){throw 'Expected the host in gameplay and the client in its native intro movie loop.'}
    if($states.host.Status.remoteGameLogicState -ne 15 -or $states.host.Status.remotePendingGameLogicState -ne 5){throw 'Host did not receive movie transition state.'}
    foreach($role in 'host','client') {
        if($states[$role].Status.probeFollowEnabled){throw 'Movement follow active during movie.'}
        if($i) {
            $before=$samples[-1].States[$role].Status
            if($states[$role].Status.sent -le $before.sent -or $states[$role].Status.received -le $before.received){throw 'Native Steam traffic stopped during movie.'}
        }
    }
    if($i -and $states.client.Status.screenTransportPumps -le $samples[-1].States.client.Status.screenTransportPumps){throw 'Native screen fallback stopped during movie.'}
    $samples+=[pscustomobject]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');States=$states}
}
[ordered]@{Passed=$true;DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin\dinput8.dll')).Hash;Samples=$samples;Scope='Native Steam traffic remains fresh in both directions for six seconds of native pre-rendered intro movie playback. Read-only phase sampling runs in the native screen callback on the established engine thread. This does not synchronize movie playback or skipping.'}|
    ConvertTo-Json -Depth 12|Set-Content (Join-Path $PSScriptRoot 'research\native-movie-transport-verification.json')
[pscustomobject]@{Passed=$true;Samples=$samples.Count;ClientSentIncrease=$samples[-1].States.client.Status.sent-$samples[0].States.client.Status.sent;ClientReceivedIncrease=$samples[-1].States.client.Status.received-$samples[0].States.client.Status.received}
