[CmdletBinding()]
param([switch]$DisconnectClient)
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$samples=[Collections.Generic.List[object]]::new();$failure=$null
function State([string]$role) {
    $path=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $path 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected lab process.'}
    $file=Get-Item (Join-Path $path 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale bridge status.'}
    $status=Get-Content $file.FullName -Raw|ConvertFrom-Json
    $logic=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch.pid --logic
    if($LASTEXITCODE){throw 'Native logic read failed.'}
    $native=$logic|ConvertFrom-Json
    if($native.stack[-1].id -ne $status.localGameLogicState){throw 'Native logic and bridge disagree.'}
    [pscustomobject]@{Status=$status;Logic=$native;Launch=$launch}
}
function Pair([string]$stage) {
    $entry=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=(State host);Client=(State client)}
    $samples.Add($entry);$entry
}
try {
    $before=Pair Ready
    foreach($s in $before.Host.Status,$before.Client.Status){if(!$s.probeFollowEnabled -or !$s.locomotionEnabled -or !$s.remoteFresh -or $s.localGameLogicState -ne 1){throw 'Connected, enabled gameplay avatars required.'}}
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -Button Start -DurationMs 200|Out-Null
    Start-Sleep -Milliseconds 1200
    $paused=Pair Paused
    if($paused.Client.Status.localGameLogicState -ne 2 -or $paused.Host.Status.localGameLogicState -ne 1){throw 'Native client pause not reached.'}
    foreach($s in $paused.Host.Status,$paused.Client.Status){if($s.probeFollowEnabled){throw 'Follow survived a peer pause.'}}
    & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role host -Command 'coop_probe_follow on'|Out-Null
    for($i=0;$i -lt 5;$i++){
        Start-Sleep -Milliseconds 200
        $still=Pair PausedStable
        foreach($role in 'Host','Client'){
            if($still.$role.Status.locomotionApplied -ne $paused.$role.Status.locomotionApplied -or $still.$role.Status.probeFollowEnabled){throw 'Animation adapter or follow ran during pause.'}
        }
    }
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -Button A -DurationMs 200|Out-Null
    Start-Sleep -Milliseconds 1000
    $resume=Pair ResumedBeforeFollow
    foreach($s in $resume.Host.Status,$resume.Client.Status){if($s.localGameLogicState -ne 1){throw 'Native gameplay did not resume.'}}
    foreach($role in 'host','client'){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_follow on'|Out-Null}
    Start-Sleep -Milliseconds 1000
    $after=Pair FollowingAgain
    foreach($role in 'Host','Client'){
        if(!$after.$role.Status.probeFollowEnabled -or $after.$role.Status.probeActorHandle -ne $before.$role.Status.probeActorHandle -or
           $after.$role.Status.locomotionApplied -le $paused.$role.Status.locomotionApplied){throw 'Owned avatar did not resume after an explicit follow request.'}
    }
    if($DisconnectClient){
        # The switch explicitly ends only the process validated by State above.
        $clientProcess=Get-Process -Id $after.Client.Launch.pid
        if($clientProcess.Path -ne $after.Client.Launch.executable){throw 'Client process changed before disconnect test.'}
        & taskkill.exe /PID $clientProcess.Id /F|Out-Null
        if(!$clientProcess.WaitForExit(10000)){throw 'Client did not exit.'}
        Start-Sleep -Milliseconds 1800
        $lost=State host
        if($lost.Status.remoteFresh -or $lost.Status.probeFollowEnabled){throw 'Stale peer did not stop owned follow.'}
        for($i=0;$i -lt 5;$i++){
            Start-Sleep -Milliseconds 200;$check=State host
            $samples.Add([pscustomobject]@{Stage='PeerDisconnected';At=[DateTimeOffset]::Now.ToString('o');Host=$check})
            if($check.Status.locomotionApplied -ne $lost.Status.locomotionApplied){throw 'Animation adapter continued after peer loss.'}
        }
    }
}catch{$failure=$_.Exception.Message}finally{
    foreach($role in 'host','client'){
        $launch=Get-Content (Join-Path $workspaceRoot "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
        if(Get-Process -Id $launch.pid -ErrorAction SilentlyContinue){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null}
    }
}
[ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;
    DisconnectClient=[bool]$DisconnectClient;Samples=$samples.ToArray();Scope='Native client pause, explicit follow refusal, resume with preserved owned identities, and optional actual client process exit. Verifies adapter application stops; does not establish shared pause policy or reconnection.'}|
    ConvertTo-Json -Depth 14|Set-Content (Join-Path $PSScriptRoot 'research/locomotion-guards-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;PauseSamples=5;DisconnectTest=[bool]$DisconnectClient}
