param([ValidateSet('corvo','emily')][string]$Character,[Parameter(Mandatory)][ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidatePattern('^body-lift-short-manifest-v[0-9]+\.json$')][string]$Manifest='body-lift-short-manifest-v6.json')
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path -LiteralPath $out){throw 'Take exists'}
New-Item -ItemType Directory -Path $out|Out-Null
$observer=if($Character-eq'corvo'){'client'}else{'host'};$source=if($observer-eq'host'){'client'}else{'host'}
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$chapters=[Collections.Generic.List[object]]::new();$capture=$null;$failure=$null
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt8;$i++){$v=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($v|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw 'Native read failed'}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid
  & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
  foreach($c in 'coop_carry_attachment off','coop_review_clip off','coop_movement_bank off','coop_authoring_proof off','coop_shadow_stream off','coop_shadow_equipment off','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_fixture 60000'){C $r $c}
 }
 $clip=Get-Content (Join-Path $PSScriptRoot $Manifest) -Raw|ConvertFrom-Json|Where-Object role -eq $Character
 if(@($clip).Count-ne1-or$clip.duration-ne1.4-or$clip.equipment_policy-ne'hands_free'){throw 'Unexpected study'}
 $file=Join-Path $PSScriptRoot $clip.binary
 if((Get-FileHash -LiteralPath $file).Hash-ne$clip.sha256){throw 'Clip hash differs'}
 Copy-Item -LiteralPath $file -Destination "$root/work/lab-instances/$observer/coop-status.json.review.bin" -Force
 C $source 'setviewpos -33 -153 68.1 0 0';C $observer 'setviewpos -29 -150.8 68.1 210 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $observer -Pitch 16 -Evidence "$out/camera-side.json"|Out-Null
 $before=S $observer;$handle=$before.probeActorHandle
 $healthBefore=@{source=(Q $source '--health' 1);observer=(Q $observer '--health' 1);avatar=(Q $observer '--health' $handle)}
 $swordBefore=Q $observer '--avatar-sword' $handle;$localSwordBefore=Q $source '--player-sword'
 if(!$swordBefore.equipped-or!$swordBefore.started){throw 'Native sword must start drawn for reversible holster'}
 $window=(Get-Process -Id $pids[$observer]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/native.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $clock=[Diagnostics.Stopwatch]::StartNew();Start-Sleep -Milliseconds 700
 foreach($view in 'side','front'){
  if($capture.HasExited){throw 'Capture ended early'}
  if($view-eq'front'){
   C $observer 'setviewpos -29 -153 68.1 180 0'
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $observer -Pitch 16 -Evidence "$out/camera-front.json"|Out-Null
  }
  # Review has a native 500ms start delay plus 300ms entry blend. Arm the
  # finite equipment lease after review so it covers the full authored motion.
  C $observer 'coop_review_clip play';$playMs=$clock.ElapsedMilliseconds
  C $observer 'coop_shadow_equipment 2500';$wait=[Diagnostics.Stopwatch]::StartNew()
  do{$eq=S $observer '.shadow-equipment.json';if($eq.active-and$eq.result-eq'native_holstered'){break};Start-Sleep -Milliseconds 20}while($wait.ElapsedMilliseconds-lt450)
  if(!$eq.active-or$eq.result-ne'native_holstered'){throw 'Native hands-free lease unavailable'}
  $heldSword=Q $observer '--avatar-sword' $handle
  if($heldSword.equipped-or$heldSword.started-or$heldSword.attachedTag-ne'holster_wpn'){throw 'Native sword did not holster'}
  $chapters.Add([pscustomobject]@{ms=$playMs;view=$view;sword=$heldSword})
  $watch=[Diagnostics.Stopwatch]::StartNew()
  while($watch.ElapsedMilliseconds-lt3300){
   $rows.Add([pscustomobject]@{ms=$clock.ElapsedMilliseconds;view=$view;review=(S $observer '.review.json');equipment=(S $observer '.shadow-equipment.json');root=(S $observer).probeRenderPosition})
   Start-Sleep -Milliseconds 40
  }
  $end=S $observer '.review.json';$eq=S $observer '.shadow-equipment.json'
  if($end.active-or$end.refused-or$end.applied-lt30-or$end.seconds-lt1.39){throw 'Complete authored playback not observed'}
  if($eq.active-or$eq.restoring-or$eq.result-ne'native_original_restored'){throw 'Sword restore incomplete'}
 }
 $healthAfter=@{source=(Q $source '--health' 1);observer=(Q $observer '--health' 1);avatar=(Q $observer '--health' $handle)}
 foreach($who in 'source','observer','avatar'){if($healthBefore[$who].current-ne$healthAfter[$who].current){throw 'Health changed'}}
 $swordAfter=Q $observer '--avatar-sword' $handle;$localSwordAfter=Q $source '--player-sword'
 if(!$swordAfter.equipped-or!$swordAfter.started-or$swordAfter.weaponHandle-ne$swordBefore.weaponHandle-or$localSwordBefore.equipped-ne$localSwordAfter.equipped){throw 'Equipment not preserved'}
 [ordered]@{character=$Character;clip=$clip.id;clipSha256=$clip.sha256;dllSha256=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;healthBefore=$healthBefore;healthAfter=$healthAfter;swordBefore=$swordBefore;swordAfter=$swordAfter;localSwordBefore=$localSwordBefore;localSwordAfter=$localSwordAfter;scope='Actual D2 authoring review, two views, native hands-free equipment lease. Carrier only: no live corpse partner or native pickup gameplay binding. First-person unchanged.'}|ConvertTo-Json -Depth 12|Set-Content "$out/identity.json"
}catch{$failure=$_.Exception.Message;throw}finally{
 foreach($r in 'host','client'){try{C $r 'coop_review_clip off';C $r 'coop_shadow_equipment off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(18000)|Out-Null}
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/observations.json"
 $chapters|ConvertTo-Json -Depth 8|Set-Content "$out/chapters.json"
 [ordered]@{passed=(!$failure);failure=$failure;samples=$rows.Count;complete=$false;visual_accepted=$false}|ConvertTo-Json|Set-Content "$out/result.json"
}
