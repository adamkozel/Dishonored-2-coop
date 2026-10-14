[CmdletBinding()]
param([ValidateSet('host','client')][string]$SourceRole='client',[switch]$SharedRecovery,[switch]$EquippedGuard)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$receiver=if($SourceRole-eq'client'){'host'}else{'client'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$polls=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role,$suffix=''){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $args=@($pids[$role],$option);if($handle){$args+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Unavailable $role $option"
}
function Capture($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){
  $r[$role]=[pscustomobject]@{Bridge=(State $role);Outcome=(State $role '.enemy-outcome.json');Guard=(State $role '.guard.json');Equipment=(State $role '.guard-equipment.json');Damage=(State $role '.damage.json');
   Health=(Native $role '--health' $handles[$role]);Player=(Native $role '--health' 1);Decision=(Native $role '--ai-decision' $handles[$role]);Reflex=(Native $role '--ai-reflex' $handles[$role])}
 }
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
try{
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  Command $role 'coop_enemy_outcome off'
  Command $role 'coop_guard_weapon off';Command $role 'coop_guard_equipment off'
  # The original checkpoint precedes the native story action that enables
  # weapons. Complete it normally on both characters; no speed changes.
  Command $role 'setviewpos -39.4 -161.4 68.1 270 20'
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button X -DurationMs 400|Out-Null
 }
 $sawScene=@{host=$false;client=$false};$watch=[Diagnostics.Stopwatch]::StartNew()
 do{
  $ready=$true
  foreach($role in 'host','client'){$s=State $role;if($s.localGameLogicState-eq5){$sawScene[$role]=$true};if(!$sawScene[$role]-or$s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}}
  if(!$ready){Start-Sleep -Milliseconds 200}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt45000)
 if(!$ready){throw 'Native Mayhew interaction did not complete on both characters'}
 $checks.Add('Both native Mayhew interactions finish at normal speed before player-weapon testing')
 foreach($role in 'host','client'){
  Command $role 'setviewpos -28.0921 -153.962 68.1 270 0'
  Command $role 'coop_probe_follow exact';Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'
  $cat=Native $role '--world-npcs';$n=@($cat.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Ambiguous guard'};$handles[$role]=$n[0].handle
  Command $role 'coop_guard_link escape 60000'
 }
 Start-Sleep -Milliseconds 600
 Command client 'coop_observer_isolation 30000'
 $before=Capture BeforeArm
 Command $SourceRole 'coop_enemy_outcome 60000';Start-Sleep -Milliseconds 500;$one=Capture OneArm
 if($one.$SourceRole.Outcome.ready-or$one.$receiver.Outcome.armed-or$one.$receiver.Outcome.applied-ne$before.$receiver.Outcome.applied){throw 'One-sided opt-in acquired outcome authority'}
 $checks.Add('One local opt-in cannot arm the other instance')
 Command $receiver 'coop_enemy_outcome 60000';$watch.Restart()
 do{Start-Sleep -Milliseconds 100;$a=State $SourceRole '.enemy-outcome.json';$b=State $receiver '.enemy-outcome.json'}while((!$a.ready-or!$b.ready)-and$watch.ElapsedMilliseconds-lt2500)
 if(!$a.ready-or!$b.ready){throw "Outcome handshake unavailable: $($a.result), $($b.result)"}
 Command $SourceRole 'setviewpos -42.9563 -117 62.7 270 0';Start-Sleep -Milliseconds 500
 if($EquippedGuard){Command client 'coop_guard_equipment 2000';Start-Sleep -Milliseconds 250}
 $armed=Capture Armed
 if($EquippedGuard){
  $sword=Native client '--sword-equipment' $handles.client
  $armed|Add-Member -NotePropertyName ClientSwordBeforeStrike -NotePropertyValue $sword
  if(!$armed.client.Equipment.active-or!$sword.equipped-or!$sword.started-or$sword.attachedTag-ne'right_hand_wpn'){throw 'Native equipped guard fixture unavailable'}
 }
 & "$PSScriptRoot/Send-D2Input.ps1" -Role $SourceRole -RightTrigger 255 -DurationMs 220|Out-Null
 $watch.Restart()
 do{
  Start-Sleep -Milliseconds 100;$a=State $SourceRole '.enemy-outcome.json';$b=State $receiver '.enemy-outcome.json'
  $polls.Add([pscustomobject]@{Elapsed=$watch.ElapsedMilliseconds;Source=$a;Receiver=$b})
 }while((!$a.acknowledged-or$b.applied-ne$before.$receiver.Outcome.applied+1)-and$watch.ElapsedMilliseconds-lt6000)
 $after=Capture Killed
 if(!$after.$SourceRole.Outcome.acknowledged-or$after.$SourceRole.Outcome.captured-ne$before.$SourceRole.Outcome.captured+1-or
    $after.$receiver.Outcome.applied-ne$before.$receiver.Outcome.applied+1-or$after.host.Health.current-ne0-or$after.client.Health.current-ne0){throw "Native enemy outcome did not complete: $($a.result), $($b.result)"}
 foreach($role in 'host','client'){
  $targetEvents=@($after.$role.Damage.events|Where-Object handle -eq ([Convert]::ToUInt32($handles[$role].Substring(2),16)))
  $hit=@($targetEvents|Where-Object damageType -eq '8c')
  $expectedAttacker=if($role-eq$SourceRole){1}else{$armed.$role.Bridge.probeActorHandle}
  if($hit.Count-ne1-or$hit[0].before-ne40-or!$hit[0].afterValid-or$hit[0].after-ne0-or$hit[0].inflictedDamage-ne40-or$hit[0].damageType-ne'8c'-or$hit[0].attackerHandle-ne$expectedAttacker){throw "Native kill attribution differs: $role"}
  # Other native NPCs can strike a corpse after the single lethal transition.
  # Keep those callbacks in the report; accept only independently identified
  # NPC sword contacts with zero health change and zero inflicted damage.
  $contacts=@($targetEvents|Where-Object damageType -ne '8c')
  if($contacts.Count){
   $catalog=Native $role '--world-npcs'
   foreach($contact in $contacts){
    $actor=@($catalog.npcs|Where-Object{[Convert]::ToUInt32($_.handle.Substring(2),16)-eq$contact.attackerHandle})
    if($contact.before-ne0-or!$contact.afterValid-or$contact.after-ne0-or$contact.inflictedDamage-ne0-or$contact.damageAmount-ne20-or$contact.damageType-ne'80'-or
       $actor.Count-ne1-or!$actor[0].unique-or!$actor[0].reciprocal-or$actor[0].spawnerName-notin'ld_spawner_ramsey_escape','ld_spawner_guard_escape_2'){throw 'Unexpected additional native victim damage'}
   }
   $after.$role|Add-Member -NotePropertyName NativeCorpseContacts -NotePropertyValue $contacts
  }
  foreach($ai in $after.$role.Decision,$after.$role.Reflex){if($ai.schedulerActive-or$ai.pauseReferences-or$ai.started){throw "Dead guard still scheduled: $role"}}
  if($after.$role.Guard.phase-or$after.$role.Guard.heldReferences-or$after.$role.Guard.releaseLost-ne$armed.$role.Guard.releaseLost){throw 'Death retained guard control'}
  $ended=($after.$role.Guard.released-$armed.$role.Guard.released)+($after.$role.Guard.retiredByDeath-$armed.$role.Guard.retiredByDeath)
  if($ended-ne$armed.$role.Guard.heldReferences){throw 'Death cleanup does not account for every owned AI reference'}
 }
 $checks.Add('One real controller assassination produces one native guard death per instance, attributed to the local player or its remote counterpart')
 $checks.Add('Native decision/reflex components stop and network guard ownership releases on both sides')
 # Retain the actual kill attempt before moving away from unrelated enemies.
 Command $SourceRole 'setviewpos -28.0921 -153.962 68.1 270 0'
 if($EquippedGuard){
  Start-Sleep -Milliseconds 1200;$settled=Capture EquipmentSettled
  if($after.client.Equipment.result-notin'enemy_died','retired_by_death'){throw 'Equipment lease did not remain owned until native enemy death'}
  if($settled.client.Equipment.active-or$settled.client.Equipment.restoring-or$settled.client.Equipment.lost-ne$armed.client.Equipment.lost){throw 'Native enemy death left unresolved equipment ownership'}
  if($settled.client.Equipment.retiredByDeath-gt$armed.client.Equipment.retiredByDeath){
   $entry=Native client '--object' $sword.entry;$inventory=Native client '--object' $sword.inventory;$components=Native client '--components' $sword.weaponHandle
   $settled|Add-Member -NotePropertyName RetiredEquipmentNative -NotePropertyValue ([pscustomobject]@{Entry=$entry;Inventory=$inventory;SwordComponents=$components})
   $entryWords=@{};foreach($word in $entry.words){$entryWords[$word.offset]=$word.word}
   $slot=($inventory.words|Where-Object offset -eq '0x68').word
   if($entryWords['0x10']-ne'0x0'-or$entryWords['0x18']-ne'0x0'-or([Convert]::ToUInt64($entryWords['0x20'].Substring(2),16)-band[uint64]4294967295)-ne0-or$slot-eq$sword.entry-or
      @($components.components|Where-Object {$_.pointer-eq$sword.component-or$_.vtableRva-eq'0x1b9cf48'}).Count){throw 'Independent native equipment removal differs from reported retirement'}
  }
  $checks.Add('A client guard with a verified native drawn sword loses equipment ownership cleanly through shared death')
 }
 Command $SourceRole 'coop_enemy_duplicate';Start-Sleep -Milliseconds 500;$duplicate=Capture Duplicate
 $beforeCanonical=@($after.$receiver.Damage.events|Where-Object damageType -eq '8c')
 $afterCanonical=@($duplicate.$receiver.Damage.events|Where-Object damageType -eq '8c')
 if($duplicate.$receiver.Outcome.applied-ne$after.$receiver.Outcome.applied-or$afterCanonical.Count-ne$beforeCanonical.Count-or
    $duplicate.$receiver.Outcome.rejected-le$after.$receiver.Outcome.rejected){throw 'Duplicate outcome was not refused'}
 if($duplicate.$receiver.Outcome.captured-ne$before.$receiver.Outcome.captured){throw 'Received kill echoed as a local source event'}
 $checks.Add('Duplicate kill delivery cannot apply a second native death and the received event does not echo')
 if($SharedRecovery){
  & "$PSScriptRoot/Test-D2SharedCheckpoint.ps1" -RequestRole $SourceRole -DelayRole $receiver|Out-Null
  foreach($role in 'host','client'){
   $cat=Native $role '--world-npcs';$n=@($cat.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
   if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Replacement guard identity ambiguous'};$handles[$role]=$n[0].handle
  }
  $restored=Capture SharedRecovery
  foreach($role in 'host','client'){
   if($restored.$role.Health.current-ne40-or$restored.$role.Player.current-ne56-or$restored.$role.Bridge.localEpoch-eq$after.$role.Bridge.localEpoch-or
      $restored.$role.Outcome.armed-or$restored.$role.Guard.phase-or$restored.$role.Guard.heldReferences){throw 'Checkpoint retained the dead guard or its old kill binding'}
   foreach($ai in $restored.$role.Decision,$restored.$role.Reflex){if(!$ai.schedulerActive-or$ai.pauseReferences-or!$ai.started){throw 'Checkpoint guard AI was not restored'}}
  }
  $checks.Add('One coordinated delayed checkpoint reload restores both guards to native health 40 and both players to 56 while invalidating the old outcome binding')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  Command $role 'coop_enemy_outcome off';Command $role 'coop_guard_link off';Command $role 'coop_guard_equipment off';Command $role 'coop_damage_observe off'
  if((State $role).localGameLogicState-eq1){Command $role 'setviewpos -28.0921 -153.962 68.1 270 0'}
 }
 Command client 'coop_observer_isolation off'
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 SourceRole=$SourceRole;SharedRecovery=[bool]$SharedRecovery;EquippedGuard=[bool]$EquippedGuard;Scope='One actual player assassination and a canonical native death outcome over Steam for the exact escape guard. Does not synchronize the assassination animation, ragdoll transforms, loot, quest/chaos counters or general combat damage.';
 Checks=$checks;Samples=$rows;Polls=$polls}|ConvertTo-Json -Depth 24|Set-Content "$PSScriptRoot/research/enemy-outcome-$SourceRole$(if($EquippedGuard){'-equipped'})$(if($SharedRecovery){'-recovery'})-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;SourceRole=$SourceRole;Checks=$checks.Count}
