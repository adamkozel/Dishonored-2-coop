param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='ranged-prop-corvo-gun-v1',[ValidateSet('corvo','emily')][string]$Character='corvo',[switch]$Closeup)
$Kind='grenade';$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$target=if($Character-eq'corvo'){'client'}else{'host'};$source=if($target-eq'client'){'host'}else{'client'}
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$id=0){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if($LASTEXITCODE){throw "Probe failed $mode"};$raw|ConvertFrom-Json}
try{
 $before=@{}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
  foreach($c in 'coop_movement_resume off','coop_review_clip off','coop_movement_bank off','coop_authoring_proof off','coop_shadow_stream off','coop_ranged_prop off','coop_grenade_prop off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  $before[$r]=Q $r '--player-inventory'
 }
 C $source 'setviewpos -34.3 -154.8 68.1 0 0'
 C $target $(if($Closeup){'setviewpos -33.0 -153.8 68.1 223 0'}else{'setviewpos -31.7 -152.9 68.1 216 0'})
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch 15 -Evidence "$out/camera.json"
 $clip=Get-Content "$PSScriptRoot/grenade-authoring-manifest-v55.json" -Raw|ConvertFrom-Json|Where-Object {$_.role-eq$Character-and$_.selection.alias-eq'grenade_idle'}
 $clip|Select-Object id,binary,sha256,selection|ConvertTo-Json -Depth 4|Set-Content "$out/donor.json"
 Copy-Item (Join-Path $PSScriptRoot $clip.binary) "$root/work/lab-instances/$target/coop-status.json.review.bin" -Force
 C $target 'coop_review_clip hold 0';Start-Sleep -Milliseconds 950
 $review=S $target '.review.json';if(!$review.active-or!$review.applied-or$review.refused){throw 'Review pose refused'}
 if($Closeup){
  $skeleton=Q $target '--skeleton' (S $target).probeActorHandle;$wrist=$skeleton.joints[9].globalPose
  $hand=@(0.0,0.0,0.0)
  for($k=0;$k-lt3;$k++){$hand[$k]=$skeleton.root[$k]+$skeleton.root[3+$k]*$wrist[12]+$skeleton.root[6+$k]*$wrist[13]+$skeleton.root[9+$k]*$wrist[14]}
  $eye=(Q $target '--player').nativeViewPosition
  $dx=$hand[0]-$eye[0];$dy=$hand[1]-$eye[1];$dz=$hand[2]-$eye[2]
  $yaw=[Math]::Atan2($dy,$dx)*180/[Math]::PI;$pitch=-[Math]::Atan2($dz,[Math]::Sqrt($dx*$dx+$dy*$dy))*180/[Math]::PI
  if([Math]::Abs($pitch)-gt25){throw 'Close view exceeds verified pitch range'}
  C $target ('setviewpos {0} {1} {2} {3}' -f $eye[0],$eye[1],$eye[2],$yaw)
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch $pitch -Evidence "$out/hand-camera.json"
  [ordered]@{hand=$hand;eye=$eye;yaw=$yaw;pitch=$pitch}|ConvertTo-Json|Set-Content "$out/framing.json"
 }
 C $target "coop_grenade_prop 15000";Start-Sleep -Milliseconds 650
 $prop=S $target '.grenade-prop.json';$prop|ConvertTo-Json|Set-Content "$out/initial.json"
 if($prop.handle){Q $target '--components' $prop.handle|ConvertTo-Json -Depth 10|Set-Content "$out/components.json"}
 if(!$prop.active-or$prop.result-ne'native_grenade_prop_attached'){throw "Prop attachment refused: $($prop.result)"}
 $checks.Add('Native cosmetic prop created and attached to reciprocal avatar left-hand tag; component whitelist and damage-source parent ownership passed')
 $window=(Get-Process -Id $pids[$target]).MainWindowHandle.ToInt64();if(!$window){throw 'No game window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','17','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $watch=[Diagnostics.Stopwatch]::StartNew();$played=$false
 while($watch.ElapsedMilliseconds-lt16300){
  # Hold the authored idle for a clear contact/attachment comparison.
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;prop=(S $target '.grenade-prop.json');review=(S $target '.review.json')});Start-Sleep -Milliseconds 120
 }
 $prop=S $target '.grenade-prop.json';if($prop.active-or$prop.pendingHide-or!$prop.hidden){throw 'Prop lease did not hide natively'}
 $checks.Add('Finite presentation lease hides prop through native Hide while the authored idle remains held')
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory'
  if(Compare-Object @($before[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory changed'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player/avatar health changed'}
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $checks.Add('Both native player inventories, health and protected files unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'coop_grenade_prop off';C $r 'coop_review_clip off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(10000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Capture failed'}}
 $rows|ConvertTo-Json -Depth 6|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;character=$Character;kind=$Kind;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Bounded native grenade cosmetic attachment and authored idle experiment. No source grenade grant, projectile, native gameplay event or network grenade binding. Visual acceptance separate.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"GRENADE_PROP_VERIFIED $Character $Kind"
