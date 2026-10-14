param([switch]$AlertBeforeBind)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;$lab="$root/outputs/d2-coop-lab";$probe="$lab/bin/d2_runtime_probe.exe"
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$pids=@{};$handles=@{}
function Command($role,$command){& "$lab/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role,$suffix=''){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $args=@($pids[$role],$option);if($handle){$args+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};return $null
}
function Capture($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){$r[$role]=[pscustomobject]@{Bridge=(State $role);Health=(Native $role '--health' $handles[$role]);Player=(Native $role '--health' 1);Guard=(State $role '.guard.json');Damage=(State $role '.damage.json')}}
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
try{
 & "$lab/Restore-D2Lab.ps1"|Out-Null
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  foreach($c in 'coop_enemy_outcome off','coop_guard_link off','coop_sword_link off','coop_partner_enemy off','coop_guard_weapon off'){Command $role $c}
 }
 Command client 'setviewpos -39.4 -161.4 68.1 270 20'; & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 400|Out-Null
 $watch=[Diagnostics.Stopwatch]::StartNew();$saw=$false
 do{$s=State client;if($s.localGameLogicState-eq5){$saw=$true};$ready=$saw-and$s.localGameLogicState-eq1-and!$s.localPendingGameLogicState;if(!$ready){Start-Sleep -Milliseconds 150}}while(!$ready-and$watch.ElapsedMilliseconds-lt45000)
 if(!$ready){throw 'Native Mayhew scene did not finish'}
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0'
 foreach($role in 'host','client'){
  $cat=Native $role '--world-npcs';$n=@($cat.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Ambiguous guard'};$handles[$role]=$n[0].handle
  Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'
 }
 if($AlertBeforeBind){
  Command client 'coop_observer_isolation 30000';Start-Sleep -Milliseconds 200
  Command client 'setviewpos -42.9563 -122 62.7 90 0';$watch.Restart()
  do{
   $target=Native client '--ai-targets' $handles.client;$attack=Native client '--ai-attack' $handles.client
   if($attack.schedulerActive){throw 'Native guard began attacking before the observation binding'}
   if($target.combat.resolved-and$target.combat.handle-eq1){break};Start-Sleep -Milliseconds 30
  }while($watch.ElapsedMilliseconds-lt1200)
  if(!$target.combat.resolved-or$target.combat.handle-ne1){throw 'Guard did not identify the native player before binding'}
 }
 foreach($role in 'host','client'){Command $role 'coop_guard_link escape 60000'}
 if($AlertBeforeBind){
  $watch.Restart();do{Start-Sleep -Milliseconds 50;$g=State client '.guard.json'}while($g.phase-ne7-and$watch.ElapsedMilliseconds-lt1500)
  if($g.phase-ne7){throw 'Alerted guard binding unavailable'}
 }
 Start-Sleep -Milliseconds 500;Command client 'coop_observer_isolation 30000';Start-Sleep -Milliseconds 250
 Capture Before|Out-Null
 $positions=if($AlertBeforeBind){@('-120.0')}else{@('-121.0','-120.7','-120.4')}
 foreach($y in $positions){
  Command client "setviewpos -42.9563 $y 62.7 90 0";Start-Sleep -Milliseconds 250
  & "$lab/Send-D2Input.ps1" -Role client -RightTrigger 255 -DurationMs 220|Out-Null
  Start-Sleep -Milliseconds 1000;$r=Capture "Strike-Y$y"
  $hits=@($r.client.Damage.events|Where-Object handle -eq ([Convert]::ToUInt32($handles.client.Substring(2),16)))
  if($hits.Count){break}
 }
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0';Start-Sleep -Milliseconds 500;Capture Settled|Out-Null
}catch{$failure=$_.Exception.Message}finally{
 [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Failure=$failure;DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;AlertBeforeBind=[bool]$AlertBeforeBind;InitialTarget=$target;InitialAttack=$attack;Samples=$rows;
 Scope='Native local player sword contacts against the bound guard at front-facing distances. No enemy damage outcome adapter is armed; existing host health snapshots remain active.'}|ConvertTo-Json -Depth 20|Set-Content "$root/work/bound-player-strike$(if($AlertBeforeBind){'-aware'})-observations.json"
 foreach($role in 'host','client'){Command $role 'coop_guard_link off';Command $role 'coop_damage_observe off'}
 Command client 'coop_observer_isolation off'; & "$lab/Restore-D2Lab.ps1"|Out-Null
}
if($failure){throw $failure}
$rows|ForEach-Object{[pscustomobject]@{Stage=$_.Stage;Host=$_.host.Health.current;Client=$_.client.Health.current;Hits=$_.client.Damage.events.Count}}
