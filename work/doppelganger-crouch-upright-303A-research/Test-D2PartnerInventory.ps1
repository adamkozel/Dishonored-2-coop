[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$pids=@{}
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Lease($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.avatar-sword.json" -Raw|ConvertFrom-Json}
function Native($role,$opt,$handle){
 for($i=0;$i-lt12;$i++){
  $raw=& $probe $pids[$role] $opt $handle
  if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)}
  Start-Sleep -Milliseconds 20
 }
 throw "Native read failed: $role $opt"
}
function Capture($role,$stage){
 $h=(Bridge $role).probeActorHandle
 $r=[pscustomobject]@{Role=$role;Stage=$stage;Epoch=(Bridge $role).localEpoch;Lease=(Lease $role);Sword=(Native $role '--avatar-sword' $h);Attachment=(Native $role '--avatar-sword-attachment' $h)}
 $rows.Add($r);return $r
}
function Until($role,$drawn){
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{
  Start-Sleep -Milliseconds 70
  $h=(Bridge $role).probeActorHandle;$s=Native $role '--avatar-sword' $h;$l=Lease $role
  $ok=if($drawn){$l.active-and$s.equipped-and$s.started-and$s.attachedTag-eq'right_hand_wpn'}else{!$l.active-and!$l.restoring-and!$s.equipped-and!$s.started-and$s.attachedTag-eq'holster_wpn'}
 }while(!$ok-and$w.ElapsedMilliseconds-lt3500)
 if(!$ok){throw "Native attachment failed: $role drawn=$drawn result=$($l.result)"}
}
try{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Requires the isolated asset experiment.'}
  $pids[$role]=$l.pid
  Command $role 'coop_avatar_weapon off';Command $role 'coop_avatar_sword off'
  Command $role 'coop_probe_arm off';Command $role 'coop_probe_follow exact'
 }
 Start-Sleep -Milliseconds 400
 foreach($role in 'host','client'){
  $before=Capture $role Before
  if($before.Sword.equipped-or$before.Sword.quantity-ne1){throw 'Unexpected starting inventory'}
  foreach($mode in 'Expiry','Cancellation'){
   Command $role 'coop_avatar_sword 2000';Until $role $true
   $drawn=Capture $role "$mode-Drawn"
   if($drawn.Sword.weaponHandle-ne$before.Sword.weaponHandle-or$drawn.Sword.quantity-ne1){throw 'Equip replaced native inventory identity'}
   if($mode-eq'Cancellation'){Command $role 'coop_avatar_sword off'}
   Until $role $false
   $after=Capture $role "$mode-Holstered"
   if($after.Sword.weaponHandle-ne$before.Sword.weaponHandle-or$after.Sword.quantity-ne1-or$after.Lease.lost-ne$before.Lease.lost){throw 'Cleanup lost native weapon identity'}
  }
  if($after.Lease.started-ne$before.Lease.started+2-or$after.Lease.restored-ne$before.Lease.restored+2){throw 'Equipment lifecycle counters differ'}
  if((Native $role '--health' 1).current-ne56){throw 'Equipment experiment changed player health'}
 }
}catch{$failure=$_.Exception.Message}finally{foreach($role in 'host','client'){Command $role 'coop_avatar_sword off'}}
[ordered]@{Time=(Get-Date).ToString('o');Passed=(!$failure);Failure=$failure;LoaderSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;Rows=$rows;Scope='Both independently owned partner inventories; local native equip, fixed expiry, explicit cancellation and stable quantity/weapon handles. Visual grip and remote equipment are separate tests.'} | ConvertTo-Json -Depth 20 | Set-Content "$PSScriptRoot/research/partner-inventory-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Characters=2;NativeEquipRestoreCycles=4;Samples=$rows.Count}
