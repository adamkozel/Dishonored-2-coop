[CmdletBinding()]
param([switch]$Doppelganger)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
function ReadLiveJson([string]$path){
    # Native status writers can briefly hold/truncate their own file. Retry
    # the actual read, never substitute an older cached snapshot or relax
    # the native startup/epoch checks below.
    for($attempt=0;$attempt-lt8;$attempt++){
        try{
            $parsed=Get-Content -LiteralPath $path -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop
            if($null-eq$parsed){throw 'Native status file is temporarily empty'}
            return $parsed
        }
        catch{if($attempt-eq7){throw};Start-Sleep -Milliseconds 15}
    }
}
function State([string]$role){ReadLiveJson (Join-Path $root "work/lab-instances/$role/coop-status.json")}
function Command([string]$role,[string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null}
# A restarted process can leave an older process's status file on disk until
# its first engine update. Wait for a fresh, usable native state before input.
foreach($role in 'host','client') {
    $launch=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    $process=Get-Process -Id $launch.pid -ErrorAction Stop
    if($process.Path -ne $launch.executable){throw 'Unexpected lab process.'}
    $statusPath=Join-Path $root "work/lab-instances/$role/coop-status.json"
    $startup=[Diagnostics.Stopwatch]::StartNew()
    do {
        $fresh=(Test-Path $statusPath) -and (Get-Item $statusPath).LastWriteTimeUtc -ge $process.StartTime.ToUniversalTime()
        if($fresh){$state=State $role;$fresh=$state.localGameLogicState -in 1,2,6,13 -and !$state.localPendingGameLogicState}
        if(!$fresh){Start-Sleep -Milliseconds 250}
    # 2026-10-22: a relaunched game can rebuild its GPU shader cache for minutes at logic 0 (Not Responding) before its
    # first menu (deploy 1214hw: about 4 minutes); wait up to 6 minutes instead of 45 s.
    }while(!$fresh -and $startup.ElapsedMilliseconds -lt 360000)
    if(!$fresh){throw "$role did not reach a fresh native menu/gameplay state."}
    $resetPath=Join-Path $root "work/lab-instances/$role/coop-status.json.reset.json"
    if((Test-Path $resetPath) -and (ReadLiveJson $resetPath).enabled) {
        Command $role 'coop_checkpoint_sync off';Start-Sleep -Milliseconds 350
        if((ReadLiveJson $resetPath).enabled){throw 'Cannot restore local fixtures during an active shared reset.'}
    }
}
$epochs=@{}
foreach($role in 'host','client') {
    if(!(Test-Path (Join-Path $root "work/lab-instances/$role/base/savegame/SLOT81/checkpoint.dat"))){throw 'Preserved role-specific lab checkpoints are required.'}
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null
    if((State $role).localGameLogicState -eq 6){
        # State 6 appears before a cold menu has finished its native setup.
        # Use the established menu-input timing; issuing LoadGame immediately
        # after that first state sample can leave MapTransition waiting forever.
        Start-Sleep -Milliseconds 2000
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button A -DurationMs 400|Out-Null
        Start-Sleep -Milliseconds 1200
    }
    $epochs[$role]=(State $role).localEpoch
    Command $role 'LoadGame 81'
}
$watch=[Diagnostics.Stopwatch]::StartNew();$lastContinue=@{host=0;client=0}
do {
    $ready=$true
    foreach($role in 'host','client') {
        $s=State $role
        if($s.localEpoch -eq $epochs[$role] -or $s.localGameLogicState -ne 1 -or $s.localPendingGameLogicState){$ready=$false}
        if($watch.ElapsedMilliseconds -gt 6000 -and $s.localGameLogicState -in 8,15 -and $watch.ElapsedMilliseconds-$lastContinue[$role] -gt 1000){
            & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button A -DurationMs 400|Out-Null
            $lastContinue[$role]=$watch.ElapsedMilliseconds
        }
    }
    if(!$ready){Start-Sleep -Milliseconds 500}
}while(!$ready -and $watch.ElapsedMilliseconds -lt 25000)
if(!$ready){throw 'Both isolated checkpoints did not return to gameplay.'}
foreach($role in 'host','client'){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null}
Start-Sleep -Milliseconds 1800
foreach($role in 'host','client') {
    $s=State $role
    if(!$s.probeActorHandle){Command $role $(if($role -eq 'host'){$(if($Doppelganger){'coop_spawn_probe shadow_emily'}else{'coop_spawn_probe emily'})}else{$(if($Doppelganger){'coop_spawn_probe shadow_corvo'}else{'coop_spawn_probe corvo'})});Start-Sleep -Milliseconds 800}
    foreach($text in 'coop_probe_follow on','coop_probe_locomotion on','coop_world_observe on','coop_cinematic_sync on','coop_cinematic_barrier on') {
        Command $role $text;Start-Sleep -Milliseconds 200
    }
}
Start-Sleep -Milliseconds 1500
$rows=@(foreach($role in 'host','client') {
    $s=State $role;$launch=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch.pid --character
    if($LASTEXITCODE){throw 'Final native character identity unavailable.'}
    $character=$raw|ConvertFrom-Json
    $expected=if($role -eq 'host'){1}else{2}
    $guard=ReadLiveJson (Join-Path $root "work/lab-instances/$role/coop-status.json.guard.json")
    $world=ReadLiveJson (Join-Path $root "work/lab-instances/$role/coop-status.json.world.json")
    if($s.localGameLogicState -ne 1 -or !$s.remoteFresh -or !$s.probeFollowEnabled -or !$s.locomotionEnabled -or
       !$s.cinematicSyncEnabled -or !$s.barrierEnabled -or $guard.phase -or $guard.heldReferences -or $s.aiHoldActive -or
       $character.character -ne $expected){throw "Final $role state does not match the lab fixture."}
    [pscustomobject]@{Role=$role;Pid=$launch.pid;Bridge=$s;Character=$character;Guard=$guard;World=$world}
})
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=$true;DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;Instances=$rows;
    Scope='Independent preserved Corvo and Emily Dunwall fixtures restored by the external developer harness. This is not a shared reset implementation.'}|
    ConvertTo-Json -Depth 14|Set-Content (Join-Path $PSScriptRoot 'research/lab-restoration-verification.json')
$rows|Select-Object Role,Pid,@{n='Character';e={$_.Character.character}},@{n='Follow';e={$_.Bridge.probeFollowEnabled}},@{n='Fresh';e={$_.Bridge.remoteFresh}}
