[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[object]]::new();$failure=$null;$pids=@{}
function Cmd($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function Bridge($r){$f=Get-Item "$root/work/lab-instances/$r/coop-status.json";if(([DateTime]::UtcNow-$f.LastWriteTimeUtc).TotalSeconds-gt1.5){throw 'Stale bridge'};Get-Content $f.FullName -Raw|ConvertFrom-Json}
function Stream($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json}
function Native($r,$flag,$handle=''){
 $args=@($pids[$r],$flag);if($handle){$args+=$handle}
 for($i=0;$i-lt6;$i++){$v=& $probe @args;if(!$LASTEXITCODE){return($v|ConvertFrom-Json)};Start-Sleep -Milliseconds 10}
 throw "Native read failed: $r $flag"
}
function Distance($a,$b){$sum=0.;for($i=0;$i-lt$a.Count;$i++){$sum+=[Math]::Pow($a[$i]-$b[$i],2)};[Math]::Sqrt($sum)}
function Capture($source,$cycle,$stage){
 $target=if($source-eq'host'){'client'}else{'host'};$b=Bridge $target
 if($b.localGameLogicState-ne1-or!$b.remoteFresh-or!$b.probeFollowExact-or!$b.locomotionEnabled){throw 'Avatar scope lost'}
 $player=Native $source '--player-states';$a=Native $target '--animation' $b.probeActorHandle
 $sk=Native $target '--skeleton' $b.probeActorHandle;$w=Native $target '--avatar-sword-attachment' $b.probeActorHandle
 $after=Bridge $target
 if($after.localEpoch-ne$b.localEpoch-or$after.probeActorHandle-ne$b.probeActorHandle){throw 'Actor changed during observation'}
 $expected=@(0.,0.,0.);for($i=0;$i-lt3;$i++){$expected[$i]=$w.parentNativeRoot[$i];for($j=0;$j-lt3;$j++){$expected[$i]+=$w.parentNativeRoot[3+$j*3+$i]*$w.jointMatrix[12+$j]}}
 $r=[pscustomobject]@{Role=$source;Cycle=$cycle;Stage=$stage;Player=$player;Bridge=$b;After=$after;Stream=(Stream $target);SkeletonStable=$sk.stable;Root=$sk.root;
  Joints=@($sk.joints|Where-Object nativeName -match '(leg|knee|thigh|foot|ankle|pelvis)'|Select-Object nativeName,localPose,globalPose);
  Input=@($a.declaredInputs|Where-Object name -eq 'animBasicInputData1')[0];AttachmentStable=$w.stable;
  AttachmentError=(Distance $expected $w.weaponNativeRoot[0..2]);WeaponRenderError=(Distance $w.weaponNativeRoot $w.weaponRenderRoot);
  RootFollowError=(Distance $b.probeRenderPosition $(if($b.poseSmoothingEnabled){$b.probeTargetPosition}else{$b.remotePosition}))}
 $rows.Add($r);$r
}
function OffBoth{foreach($r in 'host','client'){Cmd $r 'coop_shadow_stream off'};Start-Sleep -Milliseconds 400}
function ArmBoth{
 foreach($r in 'host','client'){Cmd $r 'coop_shadow_stream 15000'}
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 60;$h=Stream host;$c=Stream client}while((!$h.ready-or!$c.ready)-and$watch.ElapsedMilliseconds-lt2200)
 if(!$h.ready-or!$c.ready-or$h.token-ne$c.peerToken-or$c.token-ne$h.peerToken){throw 'Both peers did not arm'}
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$r]=$l.pid
  Cmd $r 'coop_shadow_stream off';Cmd $r 'coop_shadow_action off';Cmd $r 'coop_probe_follow exact';Cmd $r 'coop_probe_locomotion on'
  if((Native $r '--health' 1).current-ne56){throw 'Expected original health'}
 }
 Cmd host 'setviewpos -33 -153 68.1 90 0';Cmd client 'setviewpos -28.0921 -153.962 68.1 180 0';Start-Sleep -Seconds 3
 foreach($r in 'host','client'){
  $player=Native $r '--player-states';$sword=Native $r '--player-sword'
  if(!$sword.equipped-or!$sword.started){throw 'Complete native Mayhew sword acquisition before this test'}
  if(!$player.motionValid-or$player.motion-ne0-or[Math]::Abs($player.position[2]-66.4036484)-gt0.02){throw 'Expected separated flat-floor fixture'}
 }
 foreach($source in 'host','client'){
  $target=if($source-eq'host'){'client'}else{'host'}
  foreach($cycle in 1,2){
   OffBoth;ArmBoth;Start-Sleep -Milliseconds 200
   $before=Capture $source $cycle Baseline;$observer=(Bridge $target).localPosition
   & "$PSScriptRoot/Send-D2Input.ps1" -Role $source -Button A -DurationMs 220|Out-Null
   $watch=[Diagnostics.Stopwatch]::StartNew()
   # Per-frame root writes reduce the chance of a coherent external multi-read
   # skeleton snapshot. Collect longer; keep all stability/pose thresholds.
   do{Capture $source $cycle JumpLand|Out-Null;Start-Sleep -Milliseconds 15}while($watch.ElapsedMilliseconds-lt4000)
   $after=Capture $source $cycle Settled
   $samples=@($rows|Where-Object {$_.Role-eq$source-and$_.Cycle-eq$cycle-and$_.Stage-eq'JumpLand'})
   $air=@($samples|Where-Object {($_.Input.rawWords[0]-band255)-eq9})
   $land=@($samples|Where-Object {($_.Input.rawWords[0]-band255)-eq10})
   $stable=@($samples|Where-Object {$_.SkeletonStable-and$_.AttachmentStable})
   $rotation=0.;foreach($r in @($air|Where-Object SkeletonStable)){foreach($j in $r.Joints){$base=@($before.Joints|Where-Object nativeName -eq $j.nativeName);if(!$base.Count){continue};foreach($k in 0,1,2,4,5,6,8,9,10){$rotation=[Math]::Max($rotation,[Math]::Abs($j.localPose[$k]-$base[0].localPose[$k]))}}}
   $height=($samples|ForEach-Object {$_.Player.position[2]}|Measure-Object -Maximum).Maximum-$before.Player.position[2]
   $remoteHeight=($samples|ForEach-Object {$_.Root[2]}|Measure-Object -Maximum).Maximum-$before.Root[2]
   $errs=[Collections.Generic.List[string]]::new()
   if(!@($samples.Player.active|Where-Object id -eq 2).Count-or!@($samples.Player.active|Where-Object id -eq 3).Count){$errs.Add('Native jump and fall were not both observed')}
   if($air.Count-lt2-or!$land.Count-or!@($air|Where-Object {$_.Input.rawWords[2]-eq2}).Count){$errs.Add('Native looping airborne and single landing clips missing')}
   if($stable.Count-lt6-or$rotation-lt0.1){$errs.Add('Insufficient stable lower-body pose response')}
   if($height-lt0.5-or$remoteHeight-lt0.5){$errs.Add('Source or clone did not actually leave the ground')}
   if($after.Stream.applied-ne$before.Stream.applied+2-or$after.Stream.refused-ne$before.Stream.refused){$errs.Add('Jump/landing did not play exactly once per episode')}
   if(@($stable|Where-Object {$_.AttachmentError-gt0.03-or$_.WeaponRenderError-gt0.03}).Count-or@($samples|Where-Object RootFollowError -gt 0.001).Count){$errs.Add('Attachment or network root lost')}
   if((Distance $observer $after.After.localPosition)-gt0.02){$errs.Add('Other local player moved')}
   if(($after.Input.rawWords[0]-band255)-ne255-or$after.Stream.actionActive){$errs.Add('Landing did not release native channel')}
   if([Math]::Abs($after.Player.position[2]-$before.Player.position[2])-gt0.02){$errs.Add('Player did not land at original height')}
   $checks.Add([pscustomobject]@{Role=$source;Cycle=$cycle;Passed=(!$errs.Count);Failures=$errs;AirSamples=$air.Count;LandingSamples=$land.Count;StableSamples=$stable.Count;MaxLegRotationDelta=$rotation;SourceHeight=$height;AvatarHeight=$remoteHeight;Applied=$after.Stream.applied-$before.Stream.applied;ObserverDisplacement=(Distance $observer $after.After.localPosition)})
   if($errs.Count){throw "Airborne case failed: $source $cycle"}
  }
 }
 foreach($r in 'host','client'){if((Native $r '--health' 1).current-ne56-or(Native $r '--health' (Bridge $r).probeActorHandle).current-ne40){throw 'Player or clone health changed'}}
}catch{$failure=$_.Exception.Message}finally{foreach($r in 'host','client'){try{Cmd $r 'coop_shadow_stream off';& "$PSScriptRoot/Send-D2Input.ps1" -Role $r -Button Neutral -DurationMs 100|Out-Null}catch{}}}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;Dll=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Checks=$checks;Samples=$rows;
 Scope='Standing sword-equipped jump/fall presentation over the native Steam action stream. Real Jump/Fall classes select inherited fall loop; return to grounded idle selects one landing clip. Root height remains network-owned. No dedicated takeoff, crouch animation, aerial combat, moving/directional landing or traversal claim.'}|ConvertTo-Json -Depth 19|Set-Content "$PSScriptRoot/research/doppelganger-airborne-verification.json"
$checks|Format-Table -AutoSize
if($failure){throw $failure}
