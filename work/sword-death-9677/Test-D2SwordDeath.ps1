[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$pids=@{};$rows=[Collections.Generic.List[object]]::new();$before=@{};$screens=@{};$after=@{};$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Reset($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.reset.json" -Raw|ConvertFrom-Json}
function Link($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.sword-link.json" -Raw|ConvertFrom-Json}
function Damage($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.damage.json" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $args=@($pids[$role],$option);if($handle){$args+=$handle}
 for($n=0;$n-lt6;$n++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw "Native read unavailable: $role $option"
}
try {
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'};$pids[$role]=$launch.pid
  Command $role 'coop_sword_link off';Command $role 'coop_partner_enemy off';Command $role 'coop_probe_follow on'
 }
 Command client 'setviewpos -42.9563 -117.27 62.7 270 0'
 Command host 'coop_probe_follow exact';Start-Sleep -Milliseconds 150;Command host 'coop_probe_follow off'
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0';Start-Sleep -Milliseconds 400
 Command host 'coop_probe_follow on';Start-Sleep -Milliseconds 300
 foreach($role in 'host','client'){
  if((Native $role '--health' '1').current-ne56){throw 'Setup received local damage'}
  Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'
  if(!(Reset $role).enabled){Command $role 'coop_checkpoint_sync on'}
  Command $role $(if($role-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})
 }
 Start-Sleep -Milliseconds 700
 foreach($role in 'host','client'){$before[$role]=@{Bridge=(Bridge $role);Reset=(Reset $role);Link=(Link $role);Health=(Native $role '--health' '1')}}
 foreach($role in 'host','client'){Command $role 'coop_sword_link 60000'}
 $watch=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 100;$h=Link host;$c=Link client}while((!$h.ready-or!$c.ready)-and$watch.ElapsedMilliseconds-lt2000)
 if(!$h.ready-or!$c.ready){throw 'Cumulative hit handshake failed'}
 Command host 'coop_partner_enemy escape 60000';$watch.Restart()
 do{
  Start-Sleep -Milliseconds 200;$both=$true;$row=[ordered]@{At=$watch.ElapsedMilliseconds}
  foreach($role in 'host','client'){
   $r=Reset $role;$b=Bridge $role;$link=Link $role;$row[$role]=@{Reset=$r;Bridge=$b;Link=$link}
   if($r.phase-eq7){throw 'Shared recovery failed during combat'}
   if($b.localGameLogicState-ne13-or!$r.deathReady-or!$r.peerDeathReady){$both=$false}
  }
  $rows.Add([pscustomobject]$row)
 }while(!$both-and$watch.ElapsedMilliseconds-lt62000)
 if(!$both){throw 'Repeated remote sword damage did not reach both native death screens'}
 foreach($role in 'host','client'){
  $screens[$role]=@{GameOver=(Native $role '--gameover');Health=(Native $role '--health' '1');Reset=(Reset $role);Damage=(Damage $role);Link=(Link $role);Player=(Native $role '--player')}
  $g=$screens[$role].GameOver
  if($g.logic-ne13-or$g.pending-or$g.flash-eq'0x0'-or$g.showFunction[0]-eq'0x0'-or$g.showFunction[1]-eq'0x0'){throw 'Native death UI binding missing'}
  if($screens[$role].Reset.deathCount-ne$before[$role].Reset.deathCount+1-or$screens[$role].Reset.loads-ne$before[$role].Reset.loads){throw 'Repeated or premature shared reset'}
 }
 if($screens.client.Health.current-ne0-or$screens.host.Health.current-ne56){throw 'Fatal remote damage or survivor isolation failed'}
 $hits=@($screens.client.Damage.events|Where-Object handle -eq 1)
 $source=@($screens.host.Damage.events|Where-Object {$_.handle-eq$before.host.Bridge.probeActorHandle-and$_.damageType-eq'80'-and$_.damageAmount-eq20})
 if($hits.Count-lt4-or$hits.Count-gt12-or$source.Count-lt$hits.Count-or@($screens.host.Damage.events|Where-Object handle -eq 1).Count){throw 'Unexpected source/receiver combat events'}
 $health=56.0
 foreach($hit in $hits){if($hit.before-ne$health-or$hit.damageAmount-ne20-or$hit.damageType-ne'80'-or$hit.inflictedDamage-le0){throw 'Native damage sequence is inconsistent'};$health-=$hit.inflictedDamage;if($hit.afterValid-and$hit.after-ne$health){throw 'Native damage after-value differs'}}
 if($health-ne0){throw 'Native inflicted damage did not account for death'}
 Command host 'coop_gameover_retry';Command client 'RestartFromCheckpoint';$watch.Restart();$lastPress=@{host=0;client=0}
 do{
  Start-Sleep -Milliseconds 100;$done=$true
  foreach($role in 'host','client'){
   $r=Reset $role;$b=Bridge $role;if($r.phase-eq7){throw 'Shared retry failed'}
   if($r.completed-ne$before[$role].Reset.completed+1-or$b.localGameLogicState-ne1-or$b.localPendingGameLogicState){$done=$false}
   if(!$r.held-and$watch.ElapsedMilliseconds-gt6000-and$b.localGameLogicState-in8,15-and$watch.ElapsedMilliseconds-$lastPress[$role]-gt700){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 100|Out-Null;$lastPress[$role]=$watch.ElapsedMilliseconds}
  }
 }while(!$done-and$watch.ElapsedMilliseconds-lt35000)
 if(!$done){throw 'Shared combat retry did not finish'};Start-Sleep -Milliseconds 2200
 foreach($role in 'host','client'){
  $after[$role]=@{Reset=(Reset $role);Bridge=(Bridge $role);Character=(Native $role '--character');Health=(Native $role '--health' '1');Link=(Link $role);Avatars=@((Native $role '--entities').entities|Where-Object name -match '^d2coop_probe_')}
  $a=$after[$role]
  if($a.Reset.loads-ne$before[$role].Reset.loads+1-or$a.Reset.completed-ne$before[$role].Reset.completed+1-or$a.Reset.deathActive-or$a.Link.armed-or$a.Health.current-ne56-or$a.Character.character-ne$(if($role-eq'host'){1}else{2})-or$a.Character.queued-or!$a.Bridge.probeFollowEnabled-or$a.Avatars.Count-ne1){throw 'Combat retry did not restore one role-preserving attempt'}
 }
 if($after.host.Reset.transaction-ne$after.client.Reset.transaction-or$after.host.Reset.releasedAt-lt$after.client.Reset.readyAt-or$after.client.Reset.releasedAt-lt$after.host.Reset.readyAt){throw 'Combat retry bypassed both-ready gate'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){Command $role 'coop_sword_link off';Command $role 'coop_partner_enemy off';Command $role 'coop_damage_observe off'}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Up to twelve actual native guard sword hits on host remote Emily, native client health handling over Steam through fatal damage, both original death screens and one shared retry with role restoration and both-ready gate. Explicit 60-second lab scope; offset placement isolates client from local guards. No full world authority or weapon/block/power replication.';Before=$before;Screens=$screens;After=$after;Samples=$rows}|ConvertTo-Json -Depth 20|Set-Content "$PSScriptRoot/research/sword-death-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;NativeSwordHits=$hits.Count;SharedCombatRetry=$true}
