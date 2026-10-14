[CmdletBinding()]
param([switch]$PlayerSpace)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Stream($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.avatar-weapon.json" -Raw|ConvertFrom-Json}
function Lease($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.avatar-sword.json" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $a=@($pids[$role],$option);if($handle){$a+=$handle}
 for($i=0;$i-lt12;$i++){$raw=& $probe @a;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15}
 throw "Native read unavailable: $role $option"
}
function Capture($source,$stage){
 $target=if($source-eq'host'){'client'}else{'host'}
 $h=(Bridge $target).probeActorHandle;$armBefore=Get-Content "$root/work/lab-instances/$target/coop-status.json.arm-stream.json" -Raw|ConvertFrom-Json
 $r=[pscustomobject]@{SourceRole=$source;TargetRole=$target;Stage=$stage;Time=(Get-Date).ToString('o');SourceStream=(Stream $source);TargetStream=(Stream $target);Lease=(Lease $target);Source=(Native $source '--player-sword');Target=(Native $target '--avatar-sword' $h);Attachment=(Native $target '--avatar-sword-attachment' $h);ArmBefore=$armBefore;ArmAfter=(Get-Content "$root/work/lab-instances/$target/coop-status.json.arm-stream.json" -Raw|ConvertFrom-Json)}
 $rows.Add($r);return $r
}
function Await($source,$drawn,$stage){
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{
  Start-Sleep -Milliseconds 90;$r=Capture $source $stage
  $ok=if($drawn){$r.Lease.active-and$r.Target.equipped-and$r.Target.started-and$r.Target.attachedTag-eq'right_hand_wpn'}else{!$r.Lease.active-and!$r.Lease.restoring-and!$r.Target.equipped-and!$r.Target.started-and$r.Target.attachedTag-eq'holster_wpn'}
 }while(!$ok-and$w.ElapsedMilliseconds-lt3000)
 if(!$ok){throw "Partner equipment mismatch: $source $stage ($($r.Lease.result))"}
 return $r
}
function StopBoth{foreach($r in 'host','client'){Command $r 'coop_avatar_weapon off'}}
function ArmBoth{
 StopBoth;Start-Sleep -Milliseconds 350
 foreach($r in 'host','client'){Command $r 'coop_probe_follow exact';Command $r 'coop_probe_arm on';Command $r 'coop_avatar_weapon 15000'}
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$h=Stream host;$c=Stream client}while((!$h.ready-or!$c.ready)-and$w.ElapsedMilliseconds-lt2000)
 if(!$h.ready-or!$c.ready){throw "Both native equipment streams did not bind: $($h.result), $($c.result)"}
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected test process or assets'}
  $pids[$r]=$l.pid
  Command $r 'coop_avatar_sword off';Command $r 'coop_avatar_weapon off';Command $r 'coop_probe_arm off'
  Command $r $(if($PlayerSpace){'coop_probe_arm_space on'}else{'coop_probe_arm_space off'})
  Command $r 'coop_damage_observe off';Command $r 'coop_damage_observe 60000'
 }
 Command host 'setviewpos -33 -153 68.1 90 0';Command client 'setviewpos -33 -148 68.1 270 8'
 foreach($source in 'host','client'){
  $before=Capture $source Before
  ArmBoth
  $drawn=Await $source $true Drawn
  if(!$drawn.Source.equipped-or$drawn.Target.weaponHandle-ne$before.Target.weaponHandle){throw 'Initial source state or target identity mismatch'}
  Command $source 'coop_avatar_weapon_duplicate'
  Start-Sleep -Milliseconds 200;$dup=Capture $source Duplicate
  if($dup.TargetStream.rejected-le$drawn.TargetStream.rejected-or$dup.Lease.started-ne$drawn.Lease.started){throw 'Replay caused an equipment transition'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $source -Button X -DurationMs 1500|Out-Null
  $holstered=Await $source $false Holstered
  if($holstered.Source.equipped-or$holstered.Lease.restored-ne$before.Lease.restored+1){throw 'Source holster was not mirrored once'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $source -RightTrigger 255 -DurationMs 220|Out-Null
  $redrawn=Await $source $true Redrawn
  for($i=0;$i-lt10;$i++){Capture $source Drawn|Out-Null;Start-Sleep -Milliseconds 45}
  if($redrawn.Lease.started-ne$before.Lease.started+2-or$redrawn.Target.weaponHandle-ne$before.Target.weaponHandle){throw 'Redraw replaced inventory or repeated native equip'}
  Command $source 'coop_avatar_weapon off';$off=Await $source $false SourceStopped
  if(!(Bridge $off.TargetRole).remoteFresh-or$off.Lease.lost-ne$before.Lease.lost-or$off.Lease.restored-ne$before.Lease.restored+2){throw 'Stream-only stop failed to restore target'}
  $checks.Add("$source draw, native holster, redraw, replay rejection and source-only stop preserve the counterpart weapon and root connection")
  StopBoth;Start-Sleep -Milliseconds 400
 }
 ArmBoth
 foreach($source in 'host','client'){$null=Await $source $true ExpiryStart}
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 250;$h=Stream host;$c=Stream client}while(($h.armed-or$c.armed)-and$w.ElapsedMilliseconds-lt17000)
 foreach($source in 'host','client'){
  $expired=Await $source $false Expired
  if($expired.SourceStream.armed-or$expired.TargetStream.armed){throw 'Fixed 15-second stream did not expire'}
  if((Native $source '--health' 1).current-ne56){throw 'Presentation test changed player health'}
  $damage=Get-Content "$root/work/lab-instances/$source/coop-status.json.damage.json" -Raw|ConvertFrom-Json
  if(@($damage.events).Count){throw 'Presentation test produced a native damage event'}
 }
 $checks.Add('Both equipment directions expire and restore holsters; both players retain health with no native damage events')
}catch{$failure=$_.Exception.Message}finally{
 StopBoth
 foreach($r in 'host','client'){Command $r 'coop_probe_arm off';Command $r 'coop_probe_arm_space off';Command $r 'coop_damage_observe off'}
}
[ordered]@{Time=(Get-Date).ToString('o');Passed=(!$failure);Failure=$failure;PlayerSpace=[bool]$PlayerSpace;DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;Checks=$checks;Samples=$rows;Scope='Real Corvo/Emily draw and holster states over native Steam loopback into separately owned counterpart swords. Fifteen-second opt-ins, independent read-only inventory/attachment evidence; no combat, grip or two-account internet claim.'}|ConvertTo-Json -Depth 23|Set-Content "$PSScriptRoot/research/partner-weapon$(if($PlayerSpace){'-player-space'})-observations.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Directions=2;Checks=$checks.Count;Samples=$rows.Count}
