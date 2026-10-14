[CmdletBinding()]
param([switch]$DisconnectHost)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$handles=@{};$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Status($role,$suffix='.observer-isolation.json'){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($role,$kind,$handle=''){
 $args=@($pids[$role],$kind);if($handle){$args+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20}
 throw "Native read failed: $role $kind"
}
function Resolve {
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  $catalog=Native $role '--world-npcs';$handles[$role]=@{}
  foreach($name in 'ld_spawner_ramsey_escape','ld_spawner_guard_escape_2','ld_spawner_guard_escape_1'){
   $n=@($catalog.npcs|Where-Object spawnerName -eq $name)
   if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Ambiguous isolation target'};$handles[$role][$name]=$n[0].handle
  }
 }
}
function Sample($stage,$hostPresent=$true){
 $actors=@{}
 foreach($role in $(if($hostPresent){@('host','client')}else{@('client')})){
  $actors[$role]=@{}
  foreach($name in $handles[$role].Keys){$handle=$handles[$role][$name];$actors[$role][$name]=@{Decision=(Native $role '--ai-decision' $handle);Reflex=(Native $role '--ai-reflex' $handle);Attack=(Native $role '--ai-attack' $handle)}}
 }
 $s=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=$(if($hostPresent){Status host}else{$null});Client=(Status client);Actors=$actors;Bridge=(Status client '')}
 $samples.Add($s);return $s
}
function Held($s){
 if(!$s.Client.ready-or$s.Client.heldReferences-ne4-or$s.Host.active){throw 'Isolation did not own exactly four local references'}
 foreach($role in 'host','client'){foreach($name in $handles[$role].Keys){$a=$s.Actors[$role][$name];$held=$role-eq'client'-and$name-ne'ld_spawner_guard_escape_1'
  foreach($v in $a.Decision,$a.Reflex){if($v.pauseReferences-ne[int]$held-or$v.schedulerActive-eq$held){throw 'Native AI isolation differs from the expected targets'}}
  if($a.Attack.schedulerActive-or$a.Attack.started){throw 'Unexpected native attack activation'}
 }}
}
function Released($before,$after,$paused=$false){
 if($after.Client.active-or$after.Client.heldReferences-or$after.Client.released-ne$before.Client.released+4-or$after.Client.lost-ne$before.Client.lost){throw 'Owned isolation references not released exactly once'}
 foreach($name in 'ld_spawner_ramsey_escape','ld_spawner_guard_escape_2'){
  $a=$after.Actors.client[$name]
  foreach($v in $a.Decision,$a.Reflex){if($v.pauseReferences-or(!$v.schedulerActive-and!($paused-and$v.pauseState-eq3))){throw 'Native AI did not resume or queue resume'}}
 }
}
function Arm {
 Command client 'coop_observer_isolation 30000';Start-Sleep -Milliseconds 300;$s=Sample Armed;Held $s;return $s
}
function WaitPlayable($role){
 $w=[Diagnostics.Stopwatch]::StartNew();$last=0
 do{$s=Status $role '';if($s.localGameLogicState-eq1-and!$s.localPendingGameLogicState){return}
  if($w.ElapsedMilliseconds-gt6000-and$s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-$last-gt900){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null;$last=$w.ElapsedMilliseconds}
  Start-Sleep -Milliseconds 150
 }while($w.ElapsedMilliseconds-lt25000)
 throw 'Reload did not finish'
}
try{
 Resolve
 foreach($role in 'host','client'){foreach($cmd in 'coop_guard_weapon off','coop_guard_body off','coop_guard_link off','coop_observer_isolation off'){Command $role $cmd}}
 $before=Sample Before;Command host 'coop_observer_isolation 30000';Start-Sleep -Milliseconds 250;$refused=Sample HostRefused
 if($refused.Host.active-or$refused.Host.started-ne$before.Host.started-or$refused.Host.refused-ne$before.Host.refused+1){throw 'Host accepted local observer isolation'}
 $checks.Add('Host refuses the client-only visual test fixture')
 $active=Arm;Command client 'coop_observer_isolation 30000';Start-Sleep -Milliseconds 350;$duplicate=Sample Duplicate;Held $duplicate
 if($duplicate.Client.started-ne$active.Client.started-or$duplicate.Client.remainingMs-ge$active.Client.remainingMs){throw 'Duplicate renewed the fixed lease'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 200;$s=Status client}while($s.active-and$w.ElapsedMilliseconds-lt31000)
 Start-Sleep -Milliseconds 300;$expired=Sample Expired;Released $active $expired
 $checks.Add('Exactly two native NPCs are paused, duplicate does not renew, and the fixed lease expires with four native releases')
 foreach($role in 'client','host'){
  $active=Arm;& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Start -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 500;$paused=Sample "Paused-$role";Released $active $paused ($role-eq'client')
  if((Status $role '').localGameLogicState-ne2){throw 'Native pause absent'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 500
  $resumed=Sample "Resumed-$role";Released $active $resumed;$checks.Add("$role pause releases all four native references and resume restores scheduling")
 }
 $active=Arm;Command client 'LoadGame 81';Start-Sleep -Milliseconds 500;$loading=Status client
 if($loading.active-or$loading.released-ne$active.Client.released+4-or$loading.lost-ne$active.Client.lost){throw 'Reload lost isolation references'}
 WaitPlayable client;& "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null;Resolve;$new=Arm
 if($new.Bridge.localEpoch-eq$active.Bridge.localEpoch){throw 'Reload did not replace world'}
 Command client 'coop_observer_isolation off';Start-Sleep -Milliseconds 350;$off=Sample Cancelled;Released $new $off
 $checks.Add('Native reload releases references before teardown; fresh world can acquire and explicitly release new references')
 if($DisconnectHost){
  $active=Arm;$l=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if($p.Path-ne$l.executable){throw 'Unexpected host process'};Stop-Process -Id $p.Id -Force;if(!$p.WaitForExit(10000)){throw 'Host did not exit'}
  Start-Sleep -Milliseconds 1200;$lost=Sample Disconnected $false;Released $active $lost
  if($lost.Bridge.remoteFresh){throw 'Peer still fresh'};$checks.Add('Actual host termination releases every local fixture reference')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){if(Get-Process -Id $pids[$role] -ErrorAction SilentlyContinue){try{Command $role 'coop_observer_isolation off'}catch{if(!$failure){$failure=$_.Exception.Message}}}}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Scope='Client-only 30-second visual test fixture: pause Ramsey escape and guard escape 2 decision/reflex AI with native references. No networking or shared-world authority.';Checks=$checks;Samples=$samples}|ConvertTo-Json -Depth 16|Set-Content "$PSScriptRoot/research/observer-isolation-verification.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Checks=$checks.Count}
