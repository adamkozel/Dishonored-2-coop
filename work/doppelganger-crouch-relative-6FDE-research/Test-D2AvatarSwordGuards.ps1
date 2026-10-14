[CmdletBinding()]
param([switch]$DisconnectHost)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function Input($role,$button){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button $button -DurationMs 200|Out-Null}
function State{Get-Content "$root/work/lab-instances/client/coop-status.json.avatar-sword.json" -Raw|ConvertFrom-Json}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Reset($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.reset.json" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'}
 $arguments=@($launch.pid,$option);if($handle){$arguments+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @arguments;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native equipment guard read failed: $role $option"
}
function Capture($stage){
 $b=Bridge client;$r=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');State=(State);Bridge=$b;Sword=$(if($b.localGameLogicState-in1,2){Native client '--avatar-sword' $b.probeActorHandle}else{$null})}
 $samples.Add($r);return $r
}
function Arm{
 foreach($role in 'host','client'){Command $role 'coop_probe_follow exact'}
 Start-Sleep -Milliseconds 300;$before=Capture BeforeArm;Command client 'coop_avatar_sword 2000'
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 50;$r=Capture Arming}while((!$r.State.active-or$r.State.observed-le$before.State.observed)-and$w.ElapsedMilliseconds-lt1000)
 if(!$r.State.active-or$r.State.started-ne$before.State.started+1-or!$r.Sword.equipped-or$r.Sword.attachedTag-ne'right_hand_wpn'){throw 'Native Corvo sword failed to arm'}
 return $r
}
function Restored($before,$stage){
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 80;$s=State}while(($s.active-or$s.restoring)-and$w.ElapsedMilliseconds-lt3000)
 $r=Capture $stage
 if($r.State.active-or$r.State.restoring-or$r.State.lost-ne$before.State.lost-or$r.State.restored-ne$before.State.restored+1-or$r.Sword.equipped-or$r.Sword.started-or$r.Sword.attachedTag-ne'holster_wpn'-or$r.Sword.weaponHandle-ne$before.Sword.weaponHandle){throw "Sword cleanup failed: $stage"}
 return $r
}
try{
 foreach($role in 'host','client'){Command $role 'coop_avatar_sword off';Command $role 'coop_probe_arm off'}
 foreach($mode in 'off','on'){
  $a=Arm;Command client "coop_probe_follow $mode";$r=Restored $a "Follow $mode"
  Command client 'coop_avatar_sword 2000';Start-Sleep -Milliseconds 250;$rejected=Capture "Refuse $mode"
  if($rejected.State.active-or$rejected.State.started-ne$r.State.started-or$rejected.State.refused-le$r.State.refused){throw 'Non-exact or disabled follow accepted sword ownership'}
  $checks.Add("Follow $mode restores the same sword and refuses a new equipment lease")
 }
 foreach($role in 'host','client'){
  $a=Arm;Input $role Start;Start-Sleep -Milliseconds 400;$p=Capture "$role paused"
  if((Bridge $role).localGameLogicState-ne2-or$p.State.active){throw 'Native pause retained the equipment lease'}
  Command client 'coop_avatar_sword 2000';Start-Sleep -Milliseconds 200;$refused=State
  if($refused.active-or$refused.started-ne$a.State.started){throw 'Equipment rearmed during native pause'}
  Input $role A;$r=Restored $a "$role resumed";Start-Sleep -Milliseconds 300;$later=Capture NoResurrection
  if($later.State.active-or$later.State.started-ne$r.State.started){throw 'Resume resurrected equipment ownership'}
  $checks.Add("$role native pause cancels equipment, refuses rearming and restores the original holster without resurrection")
 }
 foreach($role in 'host','client'){Command $role 'coop_probe_follow exact';Command $role 'coop_checkpoint_sync on';Command $role 'coop_checkpoint_delay 0'}
 Start-Sleep -Milliseconds 600;$oldHost=Reset host;$oldClient=Reset client
 if(!$oldHost.peerArm-or!$oldClient.peerArm-or$oldHost.phase-or$oldClient.phase){throw 'Shared checkpoint pair not ready'}
 $a=Arm;Command host 'RestartFromCheckpoint';$w=[Diagnostics.Stopwatch]::StartNew();$lastPress=@{host=0;client=0}
 do{
  $done=$true
  foreach($role in 'host','client'){
   $r=Reset $role;$b=Bridge $role;$old=if($role-eq'host'){$oldHost}else{$oldClient}
   if($r.phase-eq7){throw "Shared reset failed: $($r.result)"}
   if($r.completed-ne$old.completed+1-or$b.localGameLogicState-ne1-or!$r.avatarReady){$done=$false}
   if(!$r.held-and$w.ElapsedMilliseconds-gt6000-and$b.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-$lastPress[$role]-gt700){Input $role A;$lastPress[$role]=$w.ElapsedMilliseconds}
  }
  if(!$done){Start-Sleep -Milliseconds 100}
 }while(!$done-and$w.ElapsedMilliseconds-lt40000)
 if(!$done){throw 'Shared checkpoint did not restore both characters'}
 Start-Sleep -Milliseconds 500;$r=Capture SharedReload
 if($r.State.active-or$r.State.restoring-or$r.State.lost-ne$a.State.lost-or$r.Bridge.localEpoch-eq$a.Bridge.localEpoch-or$r.Sword.equipped-or$r.Sword.started-or$r.Sword.attachedTag-ne'holster_wpn'){throw 'Old equipment state survived world replacement'}
 if(($r.State.restored-$a.State.restored)+($r.State.retired-$a.State.retired)-ne1){throw 'Old equipment lease was not retired or restored exactly once'}
 foreach($role in 'host','client'){
  $c=Native $role '--character';$expected=if($role-eq'host'){1}else{2};$old=if($role-eq'host'){$oldHost}else{$oldClient};$reset=Reset $role
  if($c.character-ne$expected-or$c.queued-or$reset.loads-ne$old.loads+1-or(Native $role '--health' 1).current-ne56){throw 'Shared reload changed role, health or load count'}
 }
 $checks.Add('A shared checkpoint requested during native equip retires/restores the old lease exactly once, restores a holstered new-world sword, preserves both roles and health, and loads once per peer')
 if($DisconnectHost){
  $a=Arm;$launch=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json;$game=Get-Process -Id $launch.pid
  if($game.Path-ne$launch.executable){throw 'Unexpected host process'}
  & taskkill.exe /PID $game.Id /F|Out-Null;if(!$game.WaitForExit(10000)){throw 'Host did not exit'}
  $r=Restored $a ActualHostExit
  & "$PSScriptRoot/Start-D2Lab.ps1" -Role host|Out-Null;& "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
  $new=Capture Reconnected
  if($new.State.active-or$new.State.restoring-or$new.Sword.equipped){throw 'Reconnection resurrected old equipment'}
  $checks.Add('Actual host termination restores the client native holster; reconnection and fixture restoration leave the old lease inactive')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(Get-Process -Id $launch.pid -ErrorAction SilentlyContinue){Input $role Neutral;Command $role 'coop_avatar_sword off'}
 }
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;DisconnectHost=[bool]$DisconnectHost;
 Scope='Corvo-only local equipment cleanup across follow modes, either native pause, shared checkpoint and optional real host termination. No network equipment replication.';Checks=$checks;Samples=$samples}|ConvertTo-Json -Depth 22|Set-Content "$PSScriptRoot/research/avatar-sword-guards-verification.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$samples.Count}
