[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Status($role,$suffix=''){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $args=@($pids[$role],$option);if($handle){$args+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15}
 throw "Native read unavailable: $role $option"
}
function Sample($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){
  $r[$role]=@{Guard=(Status $role '.guard.json');Link=(Status $role '.sword-link.json');Bridge=(Status $role);Enemy=(Status $role '.partner-ai.json');
   Pose=(Native $role '--npc-pose' $handles[$role]);Attack=(Native $role '--ai-attack' $handles[$role]);Health=(Native $role '--health' '1')}
 }
 $r.client.Decision=Native client '--ai-decision' $handles.client;$r.client.Reflex=Native client '--ai-reflex' $handles.client
 $r.host.Target=Native host '--ai-targets' $handles.host
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
function Held($s){
 if($s.host.Guard.phase-ne7-or$s.client.Guard.phase-ne7-or$s.client.Guard.heldReferences-ne2-or$s.host.Guard.binding-ne$s.client.Guard.binding){throw 'Combat guard binding was lost'}
 foreach($ai in $s.client.Decision,$s.client.Reflex){if(!$ai.started-or$ai.schedulerActive-or$ai.pauseReferences-ne1-or$ai.pauseState-ne2){throw 'Client guard AI did not remain natively paused'}}
 if($s.client.Attack.started-or$s.client.Attack.schedulerActive){throw 'Client guard started an independent attack'}
}
function Distance($a,$b){$sum=0.0;for($i=0;$i-lt$a.Count;$i++){$sum+=([double]$a[$i]-[double]$b[$i])*([double]$a[$i]-[double]$b[$i])};[Math]::Sqrt($sum)}
function SegmentDistance($point,$a,$b){
 $dot=0.0;$length=0.0
 for($i=0;$i-lt3;$i++){$d=[double]$b[$i]-$a[$i];$dot+=$d*([double]$point[$i]-$a[$i]);$length+=$d*$d}
 $t=if($length){[Math]::Clamp($dot/$length,0.0,1.0)}else{0.0}
 $nearest=@(for($i=0;$i-lt3;$i++){[double]$a[$i]+$t*([double]$b[$i]-$a[$i])})
 Distance $point $nearest
}
try{
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'};$pids[$role]=$launch.pid
  Command $role 'coop_guard_link off';Command $role 'coop_sword_link off';Command $role 'coop_partner_enemy off'
  Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000';Command $role 'coop_probe_follow on'
  $n=@((Native $role '--world-npcs').npcs|Where-Object spawnerName -eq 'ld_spawner_guard_escape_1')
  if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Authored escape guard is ambiguous'};$handles[$role]=$n[0].handle
 }
 $before=Sample Before
 foreach($role in 'host','client'){Command $role 'coop_guard_link escape 60000'}
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$h=Status host '.guard.json';$c=Status client '.guard.json'}while(($h.phase-ne7-or$c.phase-ne7)-and$watch.ElapsedMilliseconds-lt2500)
 if($h.phase-ne7-or$c.phase-ne7){throw "Combat guard handshake failed: $($h.result)/$($c.result)"}
 Start-Sleep -Milliseconds 300;$active=Sample Active;Held $active
 # Existing isolated fixture: only the counterpart stands near the host guard.
 # The actual client remains in the starting room, separating remote damage
 # from unreplicated nearby guards. This is not a normal gameplay arrangement.
 Command client 'setviewpos -42.9563 -117.27 62.7 270 0'
 Command host 'coop_probe_follow exact';Start-Sleep -Milliseconds 150;Command host 'coop_probe_follow off'
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0';Start-Sleep -Milliseconds 400;Command host 'coop_probe_follow on';Start-Sleep -Milliseconds 300
 $initial=Sample Isolated;Held $initial
 if($initial.host.Health.current-ne56-or$initial.client.Health.current-ne56){throw 'Fixture received local damage'}
 foreach($role in 'host','client'){Command $role 'coop_sword_link 15000'}
 $watch.Restart();do{Start-Sleep -Milliseconds 100;$h=Status host '.sword-link.json';$c=Status client '.sword-link.json'}while((!$h.ready-or!$c.ready)-and$watch.ElapsedMilliseconds-lt2000)
 if(!$h.ready-or!$c.ready){throw 'Combined guard/damage handshake failed'}
 Command host 'coop_partner_enemy escape 60000';$watch.Restart();$retreated=$false
 do{
  Start-Sleep -Milliseconds 100;$hit=Sample Combat;Held $hit
  if(!$retreated-and$hit.host.Target.combat.handle-eq$hit.host.Bridge.probeActorHandle-and$hit.client.Link.applied-eq$initial.client.Link.applied){
   # First establish perception at the proven close position, then move the
   # actual player so its followed counterpart retreats and elicits pursuit.
   Command client 'setviewpos -28.0921 -152.462 68.1 270 0';$retreated=$true
  }
 }while($hit.client.Link.applied-eq$initial.client.Link.applied-and$watch.ElapsedMilliseconds-lt11000)
 if($hit.client.Link.applied-ne$initial.client.Link.applied+1-or$hit.host.Link.captured-ne$initial.host.Link.captured+1){throw 'Host combat guard did not relay one native hit'}
 if($hit.host.Health.current-ne56-or$hit.client.Health.current-notin41,51){throw 'Unexpected player health'}
 $combat=@($rows|Where-Object Stage -eq Combat)
 $attacking=@($combat|Where-Object {$_.host.Attack.schedulerActive-and$_.host.Target.combat.handle-eq$_.host.Bridge.probeActorHandle})
 if($attacking.Count-lt2){throw 'No sustained host attack on remote Emily was observed'}
 $maxError=0.0;$maxSampleError=0.0;$poseSamples=0;$hostMovement=0.0;$clientMovement=0.0
 foreach($s in $combat){
  if(!$s.client.Pose.stable){continue}
  $candidates=@($combat|Where-Object {[Math]::Abs(([DateTimeOffset]$_.At-[DateTimeOffset]$s.At).TotalMilliseconds)-le600-and$_.host.Pose.stable})
  if(!$candidates.Count){continue}
  $error=($candidates|ForEach-Object {Distance $_.host.Pose.root[0..2] $s.client.Pose.root[0..2]}|Measure-Object -Minimum).Minimum
  $maxSampleError=[Math]::Max($maxSampleError,$error)
  # External probes are sequential. Compare with the short observed host
  # trajectory, not a claim of simultaneous transforms or a latency bound.
  for($i=1;$i-lt$candidates.Count;$i++){$error=[Math]::Min($error,(SegmentDistance $s.client.Pose.root[0..2] $candidates[$i-1].host.Pose.root[0..2] $candidates[$i].host.Pose.root[0..2]))}
  if($error-gt$maxError){$maxError=$error};$poseSamples++
  $hostMovement=[Math]::Max($hostMovement,(Distance $active.host.Pose.root[0..2] $s.host.Pose.root[0..2]))
  $clientMovement=[Math]::Max($clientMovement,(Distance $active.client.Pose.root[0..2] $s.client.Pose.root[0..2]))
  if((Distance $s.client.Pose.root $s.client.Pose.render)-gt.02){throw 'Client cached and rendered guard transform diverged'}
 }
 if($poseSamples-lt3-or$maxError-gt.25-or$hostMovement-lt.3-or$clientMovement-lt.3){throw 'Natural host guard movement did not replicate within the observation bound'}
 $damage=Status client '.damage.json';$hits=@($damage.events|Where-Object handle -eq 1)
 if($hits.Count-ne1-or$hits[0].attackerHandle-ne[Convert]::ToUInt32($handles.client.Substring(2),16)-or$hits[0].inflictedDamage-ne56-$hit.client.Health.current){throw 'Receiving native damage was not mapped to the local authored guard'}
 Command host 'coop_sword_duplicate';Start-Sleep -Milliseconds 300;$duplicate=Sample Duplicate;Held $duplicate
 if($duplicate.client.Health.current-ne$hit.client.Health.current-or$duplicate.client.Link.applied-ne$hit.client.Link.applied-or$duplicate.client.Link.rejected-le$hit.client.Link.rejected){throw 'Duplicate damage was not rejected'}
 # Releasing AI ownership must cancel its dependent perception/damage leases.
 Command client 'coop_guard_link off';Start-Sleep -Milliseconds 600;$off=Sample Cancelled
 foreach($role in 'host','client'){if($off.$role.Guard.phase-or$off.$role.Link.armed){throw 'Combined leases survived guard release'}}
 if((Status host '.partner-ai.json').active-or$off.client.Guard.releaseLost-ne$before.client.Guard.releaseLost-or$off.client.Guard.released-ne$before.client.Guard.released+2){throw 'Native guard ownership was not released cleanly'}
 foreach($ai in $off.client.Decision,$off.client.Reflex){if(!$ai.schedulerActive-or$ai.pauseReferences-or$ai.pauseState){throw 'Client AI did not resume after owned references were released'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){Command $role 'coop_sword_link off';Command $role 'coop_partner_enemy off';Command $role 'coop_guard_link off';Command $role 'coop_damage_observe off'}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='One authored host escape guard: natural navigation, counterpart targeting and sword hit, client native decision/reflex pause, root transform over Steam, one native client health event, duplicate rejection and dependent lease cleanup. Offset fixture isolates actual client from other guards. Attack animation, other NPCs, parry and general combat authority remain unimplemented.';
 HostMovement=$hostMovement;ClientMovement=$clientMovement;MaxObservedTrajectoryError=$maxError;MaxDiscreteSampleError=$maxSampleError;TrajectoryWindowMs=600;PoseSamples=$poseSamples;HostAttackSamples=$attacking.Count;Damage=$damage;Samples=$rows}|ConvertTo-Json -Depth 20|Set-Content "$PSScriptRoot/research/combat-guard-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;HostMovement=$hostMovement;ClientMovement=$clientMovement;HostAttackSamples=$attacking.Count;NativeDamage=$hit.client.Link.inflicted}
