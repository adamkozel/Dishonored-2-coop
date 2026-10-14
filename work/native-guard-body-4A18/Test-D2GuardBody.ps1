[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$handles=@{};$otherHandles=@{};$catalogs=@{};$samples=[Collections.Generic.List[object]]::new();$failure=$null;$damage=@{};$health=@{}
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Status($role,$suffix=''){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $args=@($pids[$role],$option);if($handle){$args+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15}
 throw "Native read unavailable: $role $option"
}
function Core($role){
 $s=Native $role '--skeleton' $handles[$role]
 [pscustomobject]@{Model=$s.model;Skeleton=$s.skeleton;Stable=$s.stable;JointCount=$s.joints.Count;Root=$s.root;
  Joints=@($s.joints|Select-Object -First 27);Phases=@($s.phases|ForEach-Object{[pscustomobject]@{Phase=$_.phase;Matrices=@($_.matrices|Where-Object joint -lt 27)}})}
}
function Capture($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=(Core host);Client=(Core client);
  HostAttack=(Native host '--ai-attack' $handles.host);ClientAttack=(Native client '--ai-attack' $handles.client);
  Target=(Native host '--ai-targets' $handles.host);ClientDecision=(Native client '--ai-decision' $handles.client);ClientReflex=(Native client '--ai-reflex' $handles.client)}
 # The next status publication includes a bounded history of the poses that
 # bracket the independent native read. No frame is assumed simultaneous.
 Start-Sleep -Milliseconds 280
 $r.HostBody=Status host '.guard-body.json';$r.ClientBody=Status client '.guard-body.json'
 $r.HostGuard=Status host '.guard.json';$r.ClientGuard=Status client '.guard.json';$r.HostBridge=Status host
 $v=[pscustomobject]$r;$samples.Add($v);return $v
}
try{
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'};$pids[$role]=$launch.pid
  foreach($command in 'coop_guard_body off','coop_guard_link off','coop_sword_link off','coop_partner_enemy off','coop_probe_follow on'){Command $role $command}
  Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'
  $catalog=Native $role '--world-npcs';$catalogs[$role]=$catalog;$n=@($catalog.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Escape guard is ambiguous'};$handles[$role]=$n[0].handle
  $other=@($catalog.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_2)
  if($other.Count-ne1-or!$other[0].unique-or!$other[0].reciprocal){throw 'Second escape guard is ambiguous'};$otherHandles[$role]=$other[0].handle
 }
 foreach($role in 'host','client'){Command $role 'coop_guard_link escape 60000'}
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$h=Status host '.guard.json';$c=Status client '.guard.json'}while(($h.phase-ne7-or$c.phase-ne7)-and$watch.ElapsedMilliseconds-lt2500)
 if($h.phase-ne7-or$c.phase-ne7){throw 'Combat guard binding unavailable'}
 foreach($role in 'host','client'){Command $role 'coop_guard_body on'}
 Start-Sleep -Milliseconds 500
 Command client 'setviewpos -42.9563 -117.27 62.7 270 0';Command host 'coop_probe_follow exact';Start-Sleep -Milliseconds 150;Command host 'coop_probe_follow off'
 # Keep the replicated guard visible. Native animation evaluation is culled
 # from the distant starting room; offscreen packet arrival is not pose proof.
 Command client 'setviewpos -42.9563 -112 62.7 270 0';Start-Sleep -Milliseconds 400;Command host 'coop_probe_follow on';Start-Sleep -Milliseconds 300
 for($i=0;$i-lt4;$i++){Capture Standing|Out-Null}
 # Other NPC hits can stagger the observer and therefore move its offset
 # counterpart. Restore the known close target before enabling perception.
 Command client 'setviewpos -42.9563 -112 62.7 270 0';Start-Sleep -Milliseconds 150
 Command host 'coop_partner_enemy escape 60000';$retreated=$false;$attackSamples=0
 for($i=0;$i-lt14;$i++){
  $s=Capture Combat
  if($s.HostGuard.phase-ne7-or$s.ClientGuard.phase-ne7-or!$s.ClientBody.active-or!$s.ClientBody.applied){throw 'Guard animation stream did not remain active'}
  foreach($ai in $s.ClientDecision,$s.ClientReflex){if($ai.schedulerActive-or$ai.pauseReferences-ne1){throw 'Client AI ownership was lost'}}
  if($s.ClientAttack.schedulerActive-or$s.ClientAttack.started){throw 'Client started an independent attack'}
  if(!$retreated-and$s.Target.combat.handle-eq$s.HostBridge.probeActorHandle){Command client 'setviewpos -42.9563 -110.5 62.7 270 0';$retreated=$true}
  if($s.HostAttack.schedulerActive-and$s.Target.combat.handle-eq$s.HostBridge.probeActorHandle){$attackSamples++}
  if($i-ge7-and$attackSamples-ge6){break}
 }
 $active=Status client '.guard-body.json';Command host 'coop_guard_body off';Start-Sleep -Milliseconds 600
 $expired=Status client '.guard-body.json';if($expired.active){throw 'Body stream survived source shutdown'}
 Start-Sleep -Milliseconds 350;$stopped=Status client '.guard-body.json';if($stopped.applied-ne$expired.applied){throw 'Expired body stream continued applying'}
 Command host 'coop_guard_body on';Start-Sleep -Milliseconds 500;$resumed=Status client '.guard-body.json'
 if(!$resumed.active-or$resumed.applied-le$stopped.applied){throw 'Body stream did not resume after local opt-in'}
 Command client 'coop_guard_link off';Start-Sleep -Milliseconds 600;$released=Status client '.guard-body.json'
 if($released.active-or(Status host '.guard.json').phase-or(Status client '.guard.json').phase){throw 'Guard release left animation control alive'}
 foreach($role in 'host','client'){$health[$role]=Native $role '--health' '1';$damage[$role]=Status $role '.damage.json'}
 if($health.host.current-ne56-or$health.client.current-lt0){throw 'Unexpected player health during presentation test'}
 # Other uncontrolled native NPCs can approach this viewpoint. Their local
 # attacks are outside the replicated guard's scope and recorded by identity.
 # The controlled replica must never cause an independent native player hit.
 # Other NPC damage, including lethal damage after the pose observations, is
 # recorded as an explicit world-authority limitation, not an animation failure.
 $accounted=56.0;$unrelatedHits=@($damage.client.events|Where-Object handle -eq 1)
 foreach($hit in $unrelatedHits){
  $attacker=@($catalogs.client.npcs|Where-Object {[Convert]::ToUInt32($_.handle.Substring(2),16)-eq$hit.attackerHandle})
  if($attacker.Count-ne1-or!$attacker[0].unique-or!$attacker[0].reciprocal-or$attacker[0].handle-eq$handles.client-or$hit.before-ne$accounted-or!$hit.afterValid){throw 'Player damage did not belong exclusively to another authored NPC'}
  $hit|Add-Member NoteProperty AttackerSpawner $attacker[0].spawnerName
  $accounted-=$hit.inflictedDamage;if($hit.after-ne$accounted){throw 'Unrelated native damage readback differs'}
 }
 if($health.client.current-ne$accounted){throw 'Unaccounted player health change during native animation test'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  if(!$damage[$role]){$damage[$role]=Status $role '.damage.json'}
  foreach($command in 'coop_guard_body off','coop_sword_link off','coop_partner_enemy off','coop_guard_link off','coop_damage_observe off'){Command $role $command}
 }
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Samples=$samples;Active=$active;Expired=$expired;Stopped=$stopped;Resumed=$resumed;Released=$released;Health=$health;Damage=$damage;Catalogs=$catalogs;OtherGuardHandles=$otherHandles;OtherGuardHits=$unrelatedHits.Count}|ConvertTo-Json -Depth 20|Set-Content "$root/work/guard-body-observations.json"
if($failure){throw $failure}
& node "$PSScriptRoot/tooling/verify-guard-body.cjs"
if($LASTEXITCODE){throw 'Independent guard body comparison failed'}
