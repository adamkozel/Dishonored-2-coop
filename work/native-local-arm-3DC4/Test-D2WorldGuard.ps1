[CmdletBinding()]
param([ValidateSet('Ready','ClientMenu')][string]$Stage='Ready')
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$record=Join-Path $PSScriptRoot 'research\world-transition-verification.json'
$states=@{}
foreach($role in 'host','client') {
    $root=Join-Path $workspaceRoot "work\lab-instances\$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    $gameProcess=Get-Process -Id $launch.pid -ErrorAction Stop
    if($gameProcess.Path -ne $launch.executable){throw 'Unexpected process path.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale bridge status.'}
    $status=Get-Content $file.FullName -Raw|ConvertFrom-Json
    if(!$status.remoteFresh -or $status.connectionState -ne 3){throw 'Native Steam peer unavailable.'}
    $world=& (Join-Path $PSScriptRoot 'bin\d2_runtime_probe.exe') $launch.pid --world
    if($LASTEXITCODE){throw 'Independent world inspection failed.'}
    $states[$role]=[pscustomobject]@{Status=$status;World=($world|ConvertFrom-Json)}
}
$hostStatus=$states.host.Status;$clientStatus=$states.client.Status
if($Stage -eq 'Ready') {
    if(!$hostStatus.localPoseAvailable -or !$clientStatus.localPoseAvailable -or
       $hostStatus.localMapId -eq '0' -or $hostStatus.localMapId -ne $clientStatus.localMapId -or
       !$hostStatus.probeFollowEnabled){throw 'Load matching campaign maps and enable host follow first.'}
    [ordered]@{ReadyAt=[DateTimeOffset]::Now.ToString('o');Ready=$states;Passed=$false}|
        ConvertTo-Json -Depth 10|Set-Content -LiteralPath $record
}else {
    $prior=Get-Content -LiteralPath $record -Raw|ConvertFrom-Json
    $before=$prior.Ready.client.Status
    if($clientStatus.localPoseAvailable -or $clientStatus.localMapId -ne '0' -or
       $clientStatus.localMap -ne 'campaign/menu/menu_p' -or $clientStatus.probeActorHandle -ne 0 -or
       $clientStatus.localEpoch -le $before.localEpoch -or
       $hostStatus.remotePoseAvailable -or $hostStatus.probeFollowEnabled -or
       $states.client.World.mapName -ne 'campaign/menu/menu_p') {throw 'World transition invalidation failed.'}
    $prior|Add-Member ClientMenuAt ([DateTimeOffset]::Now.ToString('o'))
    $prior|Add-Member ClientMenu $states
    $prior.Passed=$true
    $prior|ConvertTo-Json -Depth 10|Set-Content -LiteralPath $record
}
[pscustomobject]@{Stage=$Stage;HostMap=$hostStatus.localMap;ClientMap=$clientStatus.localMap;HostFollow=$hostStatus.probeFollowEnabled;ClientEpoch=$clientStatus.localEpoch;Passed=$true}
