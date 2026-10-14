[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$handles=@{};$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Status($role,$suffix='.guard-equipment.json'){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle){
 for($n=0;$n-lt8;$n++){$r=& $probe $pids[$role] $option $handle;if(!$LASTEXITCODE){return($r|ConvertFrom-Json)};Start-Sleep -Milliseconds 15}
 throw "Native read unavailable: $role $option"
}
function Sample($stage){
 $r=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=(Native host '--sword-equipment' $handles.host);Client=(Native client '--sword-equipment' $handles.client);
  HostControl=(Status host);ClientControl=(Status client);Guard=(Status client '.guard.json');PlayerHealth=(Native client '--health' '1');
  Attack=(Native client '--ai-attack' $handles.client)}
 $samples.Add($r);return $r
}
function Bind($mode='escape 60000'){
 foreach($role in 'host','client'){Command $role "coop_guard_link $mode"}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 100;$h=Status host '.guard.json';$c=Status client '.guard.json'}while(($h.phase-ne7-or$c.phase-ne7)-and$w.ElapsedMilliseconds-lt2500)
 if($h.phase-ne7-or$c.phase-ne7){throw 'Native guard ownership unavailable'}
}
function Holstered($s){
 if($s.Client.equipped-or$s.Client.started-or$s.Client.attachedTag-ne'holster_wpn'-or$s.Host.equipped-or$s.Host.started-or$s.Host.attachedTag-ne'holster_wpn'){throw "Native holster state differs: $($s.Stage)"}
 if($s.PlayerHealth.current-ne56-or$s.Attack.schedulerActive){throw 'Equipment presentation caused local combat or damage'}
}
try{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  Command $role 'coop_guard_equipment off';Command $role 'coop_guard_link off'
  $r=& $probe $pids[$role] '--world-npcs';if($LASTEXITCODE){throw 'Catalog unavailable'}
  $npc=@(($r|ConvertFrom-Json).npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($npc.Count-ne1-or!$npc[0].unique-or!$npc[0].reciprocal){throw 'Ambiguous escape guard'};$handles[$role]=$npc[0].handle
 }
 $before=Sample Before;Holstered $before
 foreach($role in 'host','client'){Command $role 'coop_guard_equipment 2000'}
 Start-Sleep -Milliseconds 350;$unbound=Sample Unbound;Holstered $unbound
 if($unbound.ClientControl.started-ne$before.ClientControl.started-or$unbound.HostControl.started-ne$before.HostControl.started){throw 'Unbound equipment mutation'}
 $checks.Add('Without native guard ownership neither peer can start the equipment experiment')
 Bind
 Command host 'coop_guard_equipment 2000';Start-Sleep -Milliseconds 300;$hostSample=Sample HostRefused;Holstered $hostSample
 if($hostSample.HostControl.started-ne$before.HostControl.started){throw 'Host accepted client-only equipment change'}
 $checks.Add('Host equipment remains unchanged even while the guard is bound')
 Command client 'coop_guard_equipment 2000';$watch=[Diagnostics.Stopwatch]::StartNew();$drawn=0;$duplicate=$false
 do{
  $s=Sample Equipped
  if($s.Client.equipped-and$s.Client.started-and$s.Client.attachedTag-eq'right_hand_wpn'){
   ++$drawn
   if($s.Client.parent-ne$before.Client.parent-or$s.Client.weaponHandle-ne$before.Client.weaponHandle-or$s.Client.component-ne$before.Client.component-or$s.Client.joint-eq$before.Client.joint){throw 'Native equip replaced identity or kept the holster joint'}
   if(!$duplicate){Command client 'coop_guard_equipment 2000';$duplicate=$true}
  }
  if($s.Host.equipped-or$s.Host.started-or$s.PlayerHealth.current-ne56-or$s.Attack.schedulerActive-or$s.Guard.heldReferences-ne2){throw 'Local equipment experiment lost isolation'}
  Start-Sleep -Milliseconds 80
 }while(($watch.ElapsedMilliseconds-lt2500-or$s.ClientControl.restoring)-and$watch.ElapsedMilliseconds-lt4000)
 $expired=Sample Expired;Holstered $expired
 if($drawn-lt3-or$expired.ClientControl.active-or$expired.ClientControl.restoring-or$expired.ClientControl.started-ne$before.ClientControl.started+1-or
    $expired.ClientControl.observed-ne$before.ClientControl.observed+1-or$expired.ClientControl.released-ne$before.ClientControl.released+1-or
    $expired.ClientControl.restored-ne$before.ClientControl.restored+1-or$expired.ClientControl.lost-ne$before.ClientControl.lost-or
    ($expired.Client.slots-join',')-ne($before.Client.slots-join',')){throw 'Native equip expiry/duplicate/restoration failed'}
 $checks.Add('Existing sword moves from holster to native hand joint, duplicate does not renew, and expiry restores its original slot and holster')

 foreach($cancel in 'coop_guard_equipment off','coop_guard_link off'){
  Command client 'coop_guard_equipment 2000';Start-Sleep -Milliseconds 250;$active=Sample BeforeCancel
  if(!$active.Client.equipped-or!$active.Client.started-or$active.Client.attachedTag-ne'right_hand_wpn'){throw 'Second native equip unavailable'}
  Command client $cancel;Start-Sleep -Milliseconds 500;$after=Sample $cancel;Holstered $after
  if($after.ClientControl.active-or$after.ClientControl.restoring-or$after.ClientControl.restored-ne$active.ClientControl.restored+1-or$after.ClientControl.lost-ne$active.ClientControl.lost){throw 'Cancellation did not restore native equipment'}
  $checks.Add("$cancel restores the original native sword state")
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){try{Command $role 'coop_guard_equipment off';Command $role 'coop_guard_link off'}catch{if(!$failure){$failure=$_.Exception.Message}}}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Two-second local client sword equip/restore using native inventory calls and existing weapon entity; independent slot/component/parent/tag/joint reads. No weapon-state networking, blade collision, hit delivery or visual parity claim.';
 Checks=$checks;DrawnSamples=$drawn;Samples=$samples}|ConvertTo-Json -Depth 14|Set-Content "$PSScriptRoot/research/guard-equipment-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count;DrawnSamples=$drawn}
