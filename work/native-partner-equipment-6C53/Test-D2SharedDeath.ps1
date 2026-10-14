[CmdletBinding()]
param([ValidateSet('host','client')][string]$DeathRole='client',
      [ValidateSet('host','client')][string]$RetryRole='host',
      [switch]$Simultaneous,[switch]$PauseSurvivor,[switch]$HealthTelemetry,[switch]$DamageTelemetry,[switch]$AvatarWeaponTelemetry)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
if($DamageTelemetry){$HealthTelemetry=$true}
$pids=@{};$before=@{};$screens=@{};$after=@{};$failure=$null
$samples=[Collections.Generic.List[object]]::new()
function State([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json}
function Reset([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json.reset.json") -Raw|ConvertFrom-Json}
function Damage([string]$role){if($DamageTelemetry){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json.damage.json") -Raw|ConvertFrom-Json}}
function Equipment([string]$role){if($AvatarWeaponTelemetry){[pscustomobject]@{Stream=(Get-Content "$root/work/lab-instances/$role/coop-status.json.avatar-weapon.json" -Raw|ConvertFrom-Json);Sword=(Get-Content "$root/work/lab-instances/$role/coop-status.json.avatar-sword.json" -Raw|ConvertFrom-Json)}}}
function Command([string]$role,[string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null}
function Native([string]$role,[string]$option) {
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $pids[$role] $option
    if($LASTEXITCODE){throw "Native read failed: $role $option"};$raw|ConvertFrom-Json
}
function Health([string]$role){
    if(!$HealthTelemetry){return $null}
    foreach($attempt in 1..6){
        $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $pids[$role] --health 1
        if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)}
        if($LASTEXITCODE -ne 54){throw 'Native health probe failed'}
        Start-Sleep -Milliseconds 25
    }
    throw 'Stable health observation unavailable'
}
try {
    foreach($role in 'host','client') {
        $pids[$role]=(Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json).pid
        if((State $role).localGameLogicState -ne 1){throw 'Both roles must start in gameplay.'}
        $character=Native $role '--character';if($character.character -ne $(if($role -eq 'host'){1}else{2})){throw 'Unexpected starting role.'}
        if(!(Reset $role).enabled){Command $role 'coop_checkpoint_sync on';Start-Sleep -Milliseconds 300}
        Command $role $(if($role -eq 'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null
        if($DamageTelemetry){Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'}
    }
    Start-Sleep -Milliseconds 700
    foreach($role in 'host','client'){$before[$role]=[pscustomobject]@{Reset=(Reset $role);Bridge=(State $role);Player=(Native $role '--player');Health=(Health $role);Damage=(Damage $role);Equipment=(Equipment $role)}}
    if($DamageTelemetry){foreach($role in 'host','client'){if(!$before[$role].Damage.active -or !$before[$role].Damage.observationOnly){throw 'Native damage observer did not arm.'}}}
    if($PauseSurvivor) {
        if($Simultaneous){throw 'Paused-survivor scenario requires one natural death.'}
        $survivor=if($DeathRole -eq 'host'){'client'}else{'host'}
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $survivor -Button Start -DurationMs 150|Out-Null
        Start-Sleep -Milliseconds 600
        if((State $survivor).localGameLogicState -ne 2){throw 'Survivor did not enter native pause.'}
    }
    if($Simultaneous) {
        Command host 'coop_gameover_probe';Command client 'coop_gameover_probe'
    }else {
        Command $DeathRole 'setviewpos -28.0921 -153.962 120 0 0 0'
    }
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do {
        Start-Sleep -Milliseconds 100;$both=$true
        foreach($role in 'host','client') {
            $r=Reset $role;$s=State $role
            $samples.Add([pscustomobject]@{At=$watch.ElapsedMilliseconds;Role=$role;Logic=$s.localGameLogicState;Epoch=$s.localEpoch;Reset=$r})
            if($r.phase -eq 7){throw "$role failed during death: $($r.result)"}
            if($s.localGameLogicState -ne 13 -or !$r.deathReady -or !$r.peerDeathReady){$both=$false}
        }
    }while(!$both -and $watch.ElapsedMilliseconds -lt 20000)
    if(!$both){throw 'Both native death screens did not become ready.'}
    foreach($role in 'host','client') {
        $screens[$role]=[pscustomobject]@{GameOver=(Native $role '--gameover');Logic=(Native $role '--logic');Character=(Native $role '--character');Player=(Native $role '--player');Reset=(Reset $role);Health=(Health $role);Damage=(Damage $role);Equipment=(Equipment $role)}
        $g=$screens[$role].GameOver
        if($g.logic -ne 13 -or $g.pending -or $g.flash -eq '0x0' -or $g.showFunction[0] -eq '0x0' -or $g.showFunction[1] -eq '0x0'){throw 'Native game-over Flash binding unavailable.'}
        if($screens[$role].Reset.deathCount -ne $before[$role].Reset.deathCount+1 -or $screens[$role].Reset.loads -ne $before[$role].Reset.loads){throw 'Failure duplicated or retried before a native retry request.'}
    }
    if(!$Simultaneous) {
        $survivor=if($DeathRole -eq 'host'){'client'}else{'host'}
        if($DamageTelemetry){
            $events=@($screens[$DeathRole].Damage.events|Where-Object {$_.handle -eq 1 -and $_.afterValid -and $_.before -gt 0 -and $_.after -eq 0})
            if($events.Count -ne 1 -or @($screens[$survivor].Damage.events|Where-Object handle -eq 1).Count){throw 'Native damage capture did not distinguish one actual player death from the shared death state.'}
            # Damage can execute on a native worker. The observer copies its
            # result under a lock and never calls back into the engine here.
            if($events[0].before -ne $before[$DeathRole].Health.current -or !$events[0].thread -or
               $events[0].inflictedDamage -ne $events[0].before){throw 'Fatal native damage disagreed with independent health evidence.'}
        }
        if($HealthTelemetry -and ($before[$DeathRole].Health.current -le 0 -or $screens[$DeathRole].Health.current -ne 0 -or
           $screens[$survivor].Health.current -ne $before[$survivor].Health.current -or
           $screens[$DeathRole].Health.maximum -ne $before[$DeathRole].Health.maximum)){throw 'Native health did not distinguish actual damage from the shared death screen'}
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $survivor -LeftY 16000 -DurationMs 450|Out-Null
        Start-Sleep -Milliseconds 650;$stationary=Native $survivor '--player';$distance=0.0
        for($i=0;$i -lt 3;$i++){$d=$stationary.candidatePosition[$i]-$screens[$survivor].Player.candidatePosition[$i];$distance+=$d*$d}
        if([Math]::Sqrt($distance) -gt 0.001 -or (State $survivor).localGameLogicState -ne 13){throw 'The survivor continued moving after the shared death.'}
    }
    Command $RetryRole 'coop_gameover_retry'
    Command $(if($RetryRole -eq 'host'){'client'}else{'host'}) 'RestartFromCheckpoint'
    $watch.Restart();$lastPress=@{host=0;client=0}
    do {
        Start-Sleep -Milliseconds 100;$done=$true
        foreach($role in 'host','client') {
            $r=Reset $role;$s=State $role
            if($r.phase -eq 7){throw "$role retry failed: $($r.result)"}
            if($r.completed -ne $before[$role].Reset.completed+1 -or $s.localGameLogicState -ne 1 -or $s.localPendingGameLogicState){$done=$false}
            if(!$r.held -and $watch.ElapsedMilliseconds -gt 6000 -and $s.localGameLogicState -in 8,15 -and $watch.ElapsedMilliseconds-$lastPress[$role] -gt 700) {
                & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button A -DurationMs 100|Out-Null;$lastPress[$role]=$watch.ElapsedMilliseconds
            }
        }
    }while(!$done -and $watch.ElapsedMilliseconds -lt 35000)
    if(!$done){throw 'Shared death retry did not finish.'}
    Start-Sleep -Milliseconds 2200
    foreach($role in 'host','client') {
        $after[$role]=[pscustomobject]@{Reset=(Reset $role);Character=(Native $role '--character');Player=(Native $role '--player');Bridge=(State $role);Health=(Health $role);Equipment=(Equipment $role);
            Avatars=@((Native $role '--entities').entities|Where-Object name -match '^d2coop_probe_')}
        $r=$after[$role].Reset
        if($HealthTelemetry -and ($after[$role].Health.current -le 0 -or $after[$role].Health.maximum -ne $before[$role].Health.maximum)){throw 'Restored checkpoint health was not valid'}
        if($r.deathActive -or $r.loads -ne $before[$role].Reset.loads+1 -or $r.completed -ne $before[$role].Reset.completed+1 -or
           $after[$role].Character.character -ne $(if($role -eq 'host'){1}else{2}) -or $after[$role].Character.queued){throw "$role did not restore one canonical attempt with its assigned role."}
        if($r.avatarRequested -and (!$r.avatarReady -or $r.avatarPending -or !$after[$role].Bridge.probeFollowEnabled -or
           $r.avatarRestored -ne $before[$role].Reset.avatarRestored+1 -or $after[$role].Avatars.Count -ne 1 -or
           $after[$role].Avatars[0].name -ne $(if($role -eq 'host'){'d2coop_probe_emily'}else{'d2coop_probe_corvo'}))){throw "$role lost or duplicated its counterpart presentation after retry."}
    }
    if($after.host.Reset.transaction -ne $after.client.Reset.transaction -or
       $after.host.Reset.releasedAt -lt $after.client.Reset.readyAt -or $after.client.Reset.releasedAt -lt $after.host.Reset.readyAt){throw 'Death retry bypassed the both-ready gate.'}
}catch{$failure=$_.Exception.Message}finally{
    if($DamageTelemetry){foreach($role in 'host','client'){Command $role 'coop_damage_observe off'}}
}
$suffix=if($Simultaneous){'concurrent'}else{"$DeathRole$(if($PauseSurvivor){'-from-pause'})"}
if($HealthTelemetry){$suffix+='-health'}
if($DamageTelemetry){$suffix+='-damage'}
if($AvatarWeaponTelemetry){$suffix+='-equipment'}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;
    DeathRole=$DeathRole;RetryRole=$RetryRole;SimultaneousRequests=[bool]$Simultaneous;SurvivorInitiallyPaused=[bool]$PauseSurvivor;HealthTelemetry=[bool]$HealthTelemetry;DamageTelemetry=[bool]$DamageTelemetry;ProbeSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe')).Hash;
    Scope='Dunwall natural fall death (or concurrent native GameOver probes), both original game-over states and Flash bindings, survivor movement stopped, native retry callback plus overlapping retry, exactly one shared fixed-fixture reload with restored Corvo/Emily roles.';
    VisualVerification=$false;Before=$before;Screens=$screens;After=$after;Samples=$samples}|ConvertTo-Json -Depth 15|
    Set-Content (Join-Path $PSScriptRoot "research/shared-death-$suffix-verification.json")
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;DeathRole=$DeathRole;RetryRole=$RetryRole;Concurrent=[bool]$Simultaneous;Transaction=$after.host.Reset.transaction}
