[CmdletBinding()]
param([ValidateSet('host','client')][string]$SourceRole='client',[switch]$SharedRecovery)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$receiver=if($SourceRole-eq'client'){'host'}else{'client'};$pids=@{};$handles=@{}
$rows=[Collections.Generic.List[object]]::new();$polls=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role,$suffix=''){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $args=@($pids[$role],$option);if($handle){$args+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Unavailable $role $option"
}
function Capture($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){$r[$role]=[pscustomobject]@{Bridge=(State $role);Melee=(State $role '.enemy-melee.json');Guard=(State $role '.guard.json');Damage=(State $role '.damage.json');
  Health=(Native $role '--health' $handles[$role]);Player=(Native $role '--health' 1);Decision=(Native $role '--ai-decision' $handles[$role]);Reflex=(Native $role '--ai-reflex' $handles[$role])}}
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
function ResolveGuards {
 foreach($role in 'host','client'){$cat=Native $role '--world-npcs';$n=@($cat.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Ambiguous native guard'};$handles[$role]=$n[0].handle}
}
try{
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  foreach($command in 'coop_enemy_melee off','coop_enemy_outcome off','coop_guard_link off','coop_sword_link off','coop_partner_enemy off','coop_guard_weapon off','coop_probe_follow exact','coop_damage_observe off'){Command $role $command}
  Command $role 'setviewpos -39.4 -161.4 68.1 270 20'; & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button X -DurationMs 400|Out-Null
 }
 $saw=@{host=$false;client=$false};$watch=[Diagnostics.Stopwatch]::StartNew()
 do{$ready=$true;foreach($role in 'host','client'){$s=State $role;if($s.localGameLogicState-eq5){$saw[$role]=$true};if(!$saw[$role]-or$s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 150}}while(!$ready-and$watch.ElapsedMilliseconds-lt45000)
 if(!$ready){throw 'Native Mayhew interaction did not finish on both characters'}
 Start-Sleep -Milliseconds 250
 foreach($role in 'host','client'){Command $role 'setviewpos -28.0921 -153.962 68.1 270 0';Command $role 'coop_probe_follow exact';Command $role 'coop_damage_observe 60000'}
 foreach($role in 'host','client'){$s=State $role;if(!$s.probeFollowEnabled-or!$s.probeFollowExact){throw "Exact native counterpart following unavailable: $role"}}
 ResolveGuards
 Command client 'coop_observer_isolation 30000';Start-Sleep -Milliseconds 200
 Command host 'coop_guard_link escape 60000'
 if($SourceRole-eq'client'){
  Command client 'setviewpos -42.9563 -122 62.7 90 0';$watch.Restart()
  do{
   $target=Native client '--ai-targets' $handles.client;$attack=Native client '--ai-attack' $handles.client
   if($target.combat.resolved-and$target.combat.handle-eq1){break};Start-Sleep -Milliseconds 30
  }while($watch.ElapsedMilliseconds-lt1500)
  if(!$target.combat.resolved-or$target.combat.handle-ne1){throw 'Native client guard awareness unavailable'}
  # Preserve native knowledge of the player, then leave attack range before
  # transferring control. This avoids racing a queued melee attack against
  # the reliable guard handshake.
  Command client 'setviewpos -28.0921 -153.962 68.1 270 0';$watch.Restart()
  do{Start-Sleep -Milliseconds 80;$attack=Native client '--ai-attack' $handles.client}while($attack.schedulerActive-and$watch.ElapsedMilliseconds-lt4000)
  if($attack.schedulerActive){throw 'Native client guard did not leave attack scheduling after retreat'}
 }
 Command client 'coop_guard_link escape 60000'
 $watch.Restart();do{Start-Sleep -Milliseconds 50;$h=State host '.guard.json';$c=State client '.guard.json'}while(($h.phase-ne7-or$c.phase-ne7)-and$watch.ElapsedMilliseconds-lt2500)
 if($h.phase-ne7-or$c.phase-ne7){Capture BindingFailed|Out-Null;throw "Native guard handshake unavailable: $($h.result), $($c.result)"}
 $before=Capture BeforeArm
 Command $SourceRole 'coop_enemy_melee 15000';Start-Sleep -Milliseconds 350;$one=Capture OneArm
 if(!$one.$SourceRole.Melee.armed){throw "Source melee arm unavailable: $($one.$SourceRole.Melee.result)"}
 if($one.$SourceRole.Melee.ready-or$one.$receiver.Melee.armed-or$one.$receiver.Melee.applied-ne$before.$receiver.Melee.applied){throw 'One-sided arm created damage authority'}
 $checks.Add('One local opt-in cannot arm the other instance or apply damage')
 Command $receiver 'coop_enemy_melee 15000';$watch.Restart()
 do{Start-Sleep -Milliseconds 80;$a=State $SourceRole '.enemy-melee.json';$b=State $receiver '.enemy-melee.json'}while((!$a.ready-or!$b.ready)-and$watch.ElapsedMilliseconds-lt2500)
 if(!$a.ready-or!$b.ready){throw "Melee handshake unavailable: $($a.result), $($b.result)"}
 Command $SourceRole 'coop_enemy_outcome 60000'
 if((State $SourceRole '.enemy-outcome.json').result-ne'owner_conflict'){throw 'Assassination and ordinary melee acquired the same health authority'}
 $checks.Add('Assassination outcome cannot acquire health authority during the ordinary melee lease')
 if($SourceRole-eq'host'){
  Command host 'setviewpos -42.9563 -122 62.7 90 0';$watch.Restart()
  do{Start-Sleep -Milliseconds 100;$attack=Native host '--ai-attack' $handles.host}while(!$attack.schedulerActive-and$watch.ElapsedMilliseconds-lt3500)
  if(!$attack.schedulerActive){throw 'Native host guard did not become alert for an ordinary strike'}
 }else{Command client 'setviewpos -42.9563 -120.0 62.7 90 0';Start-Sleep -Milliseconds 250}
 $armed=Capture Armed
 & "$PSScriptRoot/Send-D2Input.ps1" -Role $SourceRole -RightTrigger 255 -DurationMs 220|Out-Null
 $watch.Restart()
 do{
  Start-Sleep -Milliseconds 80;$a=State $SourceRole '.enemy-melee.json';$b=State $receiver '.enemy-melee.json'
  $polls.Add([pscustomobject]@{Elapsed=$watch.ElapsedMilliseconds;Source=$a;Receiver=$b})
 }while((!$a.acknowledged-or$b.applied-ne$before.$receiver.Melee.applied+1)-and$watch.ElapsedMilliseconds-lt6000)
 $after=Capture Hit
 Command $SourceRole 'setviewpos -28.0921 -153.962 68.1 270 0'
 if(!$after.$SourceRole.Melee.acknowledged-or$after.$SourceRole.Melee.captured-ne$before.$SourceRole.Melee.captured+1-or
    $after.$receiver.Melee.applied-ne$before.$receiver.Melee.applied+1-or$after.host.Health.current-ne30-or$after.client.Health.current-ne30){throw "Ordinary hit did not converge: $($a.result), $($b.result)"}
 $hits=@{}
 foreach($role in 'host','client'){
  $hits[$role]=@($after.$role.Damage.events|Where-Object handle -eq ([Convert]::ToUInt32($handles[$role].Substring(2),16)))
  $hit=$hits[$role];$attacker=if($role-eq$SourceRole){1}else{$armed.$role.Bridge.probeActorHandle}
  if($hit.Count-ne1-or$hit[0].attackerHandle-ne$attacker-or$hit[0].damageAmount-ne10-or$hit[0].damageType-ne'80'-or
     $hit[0].before-ne40-or!$hit[0].afterValid-or$hit[0].after-ne30-or$hit[0].inflictedDamage-ne10){throw "Native ordinary hit attribution differs: $role"}
  if($after.$role.Guard.phase-ne7-or$after.$role.Guard.releaseLost-ne$armed.$role.Guard.releaseLost){throw 'Ordinary hit lost native guard ownership'}
 }
 foreach($field in @(@(0x20,4),@(0x30,12),@(0x3c,12),@(0x4c,4))){
  if($hits[$SourceRole][0].rawInfo.Substring($field[0]*2,$field[1]*2)-ne$hits[$receiver][0].rawInfo.Substring($field[0]*2,$field[1]*2)){throw 'Native region, normal or direction changed during relay'}
 }
 if($after.client.Guard.healthApplied-ne$armed.client.Guard.healthApplied-or$after.client.Melee.healthBypassed-le$armed.client.Melee.healthBypassed){throw 'Passive guard health competed with the native damage event'}
 if($after.client.Guard.heldReferences-ne2-or$after.client.Decision.schedulerActive-or$after.client.Reflex.schedulerActive){throw 'Client guard resumed independent AI during melee'}
 $checks.Add('One real controller slash produces one attributed native 40-to-30 guard hit in each instance with matching region, normal and direction')
 $checks.Add('Native damage owns health without passive-setter duplication while the client guard retains its two AI references and root binding')
 Command $SourceRole 'coop_enemy_melee_duplicate';Start-Sleep -Milliseconds 350;$duplicate=Capture Duplicate
 if($duplicate.$receiver.Melee.applied-ne$after.$receiver.Melee.applied-or$duplicate.$receiver.Damage.events.Count-ne$after.$receiver.Damage.events.Count-or
    $duplicate.$receiver.Melee.rejected-le$after.$receiver.Melee.rejected-or$duplicate.$receiver.Melee.captured-ne$before.$receiver.Melee.captured-or
    $duplicate.host.Health.current-ne30-or$duplicate.client.Health.current-ne30){throw 'Duplicate hit applied damage or echoed'}
 $checks.Add('Duplicate delivery cannot apply a second native hit, reduce health again or echo as a local player attack')
 foreach($role in 'host','client'){Command $role 'coop_enemy_melee off';Command $role 'coop_guard_link off'}
 if($SharedRecovery){
  & "$PSScriptRoot/Test-D2SharedCheckpoint.ps1" -RequestRole $SourceRole -DelayRole $receiver|Out-Null
  ResolveGuards;$restored=Capture SharedRecovery
  foreach($role in 'host','client'){
   if($restored.$role.Health.current-ne40-or$restored.$role.Player.current-ne56-or$restored.$role.Melee.armed-or$restored.$role.Guard.phase-or
      $restored.$role.Bridge.localEpoch-eq$after.$role.Bridge.localEpoch){throw 'Shared recovery retained old damage or authority'}
  }
  $checks.Add('One delayed coordinated checkpoint reload restores both guards to 40 and both players to 56 with new world identities')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  foreach($command in 'coop_enemy_melee off','coop_guard_link off','coop_damage_observe off'){Command $role $command}
  if((State $role).localGameLogicState-eq1){Command $role 'setviewpos -28.0921 -153.962 68.1 270 0'}
 }
 Command client 'coop_observer_isolation off'
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 SourceRole=$SourceRole;SharedRecovery=[bool]$SharedRecovery;Scope='One actual ordinary player-sword event on the exact bound escape guard. No simultaneous attacks, general damage, parry/block, hit-reaction animation or campaign authority claim.';
 Checks=$checks;Samples=$rows;Polls=$polls}|ConvertTo-Json -Depth 24|Set-Content "$PSScriptRoot/research/enemy-melee-$SourceRole$(if($SharedRecovery){'-recovery'})-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;SourceRole=$SourceRole;Checks=$checks.Count}
