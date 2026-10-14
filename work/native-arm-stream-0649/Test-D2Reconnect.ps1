[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$samples=[Collections.Generic.List[object]]::new();$failure=$null
function Snapshot([string]$stage) {
    $row=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
    foreach($role in 'host','client') {
        $path=Join-Path $root "work/lab-instances/$role/coop-status.json"
        $row[$role]=Get-Content $path -Raw|ConvertFrom-Json
    }
    $item=[pscustomobject]$row;$samples.Add($item);$item
}
try {
    $before=Snapshot Before
    if(!$before.host.remoteFresh -or !$before.client.remoteFresh){throw 'Fresh connected pair required.'}
    $hostLaunch=Get-Content (Join-Path $root 'work/lab-instances/host/launch.json') -Raw|ConvertFrom-Json
    $clientLaunch=Get-Content (Join-Path $root 'work/lab-instances/client/launch.json') -Raw|ConvertFrom-Json
    $expected=(Resolve-Path (Join-Path $root 'work/lab-game/Dishonored2.exe')).Path
    $proc=Get-Process -Id $clientLaunch.pid
    if($proc.Path -ne $expected -or $proc.StartTime.ToUniversalTime() -lt ([DateTimeOffset]$clientLaunch.startedAt).UtcDateTime.AddSeconds(-5)){throw 'Client process identity mismatch.'}
    Stop-Process -Id $proc.Id -Force
    if(!$proc.WaitForExit(10000)){throw 'Client did not stop.'}
    Start-Sleep -Milliseconds 5500
    $silent=Snapshot ClientStopped
    if($silent.host.remoteFresh -or $silent.host.probeFollowEnabled -or $silent.host.aiHoldActive){throw 'Host retained controls after peer loss.'}
    & (Join-Path $PSScriptRoot 'Start-D2Lab.ps1') -Role client|Out-Null
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do {
        Start-Sleep -Milliseconds 500
        $current=Snapshot Reconnecting
        if($current.host.remoteFresh -and $current.client.remoteFresh -and $current.host.connectionState -eq 3 -and $current.client.connectionState -eq 3 -and $current.client.localGameLogicState -eq 6){break}
    }while($watch.ElapsedMilliseconds -lt 45000)
    if(!$current.host.remoteFresh -or !$current.client.remoteFresh -or $current.client.localGameLogicState -ne 6){throw 'Restarted client did not reconnect.'}
    if($current.host.connection -eq $before.host.connection){throw 'Host reused the old Steam connection handle.'}
    if($current.host.localEpoch -ne $before.host.localEpoch -or $current.host.localGameLogicState -ne $before.host.localGameLogicState){throw 'Host changed worlds during reconnection.'}
    if((Get-Content (Join-Path $root 'work/lab-instances/host/launch.json') -Raw|ConvertFrom-Json).pid -ne $hostLaunch.pid){throw 'Host was restarted.'}
    $received=$current.host.received;Start-Sleep -Milliseconds 750
    $after=Snapshot After
    if($after.host.received -le $received -or !$after.host.remoteFresh -or !$after.client.remoteFresh){throw 'Reconnected transport did not continue receiving.'}
}catch{$failure=$_.Exception.Message}
$report=[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=($null -eq $failure);Failure=$failure;DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;ReconnectMs=$watch.ElapsedMilliseconds;Samples=$samples;Scope='Actual client process termination and relaunch while host stays in its current world. Native Steam loopback reconnection and stale-peer control release only; no world or checkpoint synchronization.'}
$report|ConvertTo-Json -Depth 12|Set-Content (Join-Path $PSScriptRoot 'research/native-reconnect-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;ReconnectMs=$watch.ElapsedMilliseconds;OldHostConnection=$before.host.connection;NewHostConnection=$after.host.connection;HostWorldUnchanged=$true}
