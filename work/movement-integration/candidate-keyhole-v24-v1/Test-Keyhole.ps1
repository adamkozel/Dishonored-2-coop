param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='keyhole-corvo-left-stand-v24-v1',[ValidateSet('host','client')][string]$Source='host',[switch]$Record,[switch]$CrouchedStart,[switch]$Reverse)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$src=$Source;$dst=if($src-eq'host'){'client'}else{'host'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$overall=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle
  I $r;foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_agility_fixture off','coop_movement_fixture off','coop_movement_fixture 60000','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_feet authored'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Baseline health changed'}
  if((Q $r '--player-states').stance-eq1){I $r B 100;Start-Sleep -Milliseconds 650}
  if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2300}
  $role=if($r-eq'host'){'emily'}else{'corvo'};Copy-Item "$PSScriptRoot/keyhole-banks-v1/$role-keyhole-bank.bin" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
 }
 $sourcePosition=if($Reverse){'setviewpos -37.9 -153.55 68.1 0 0'}else{'setviewpos -35.9 -153.55 68.1 180 0'}
 if($Reverse){C $dst 'setviewpos -41 -151 68.1 320 0'}else{C $dst 'setviewpos -32.5 -151 68.1 217 0'};C $src $sourcePosition
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $src -Pitch 0 -Evidence "$out/source-camera.json"
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $dst -Pitch 10 -Evidence "$out/observer-camera.json"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C $dst 'coop_movement_bank on'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$m=S $dst '.movement.json';if($m.active-and$m.version-eq24-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne24-or$m.weight-lt.9){throw 'Bank readiness deadline'}
 if($Record){
  $window=(Get-Process -Id $pids[$dst]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','30','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'};$overall=[Diagnostics.Stopwatch]::StartNew()
 }
 foreach($repeat in 1,2){
  I $src;if((Q $src '--player-states').stance-eq1){I $src B 100;Start-Sleep -Milliseconds 650};C $src $sourcePosition;Start-Sleep -Milliseconds 500
  if($CrouchedStart){I $src B 100;Start-Sleep -Milliseconds 650}
  I $src X 1000;$watch=[Diagnostics.Stopwatch]::StartNew();$exitSent=$false;$seen=@{};$nativeSeen=@{};$events=@{};$holster=$false
  while($watch.ElapsedMilliseconds-lt7500){
   $n=Q $src '--player-states';$m=S $dst '.movement.json';$stream=S $dst '.shadow-stream.json'
   if(!$exitSent-and$watch.ElapsedMilliseconds-ge4500-and@($n.active|Where-Object id -eq 24).Count){I $src B 100;$exitSent=$true}
   if($n.keyholeSceneValid){$nativeSeen[[int]$n.keyholeScene.variant]=$true}
   if($m.traversalVariant-ge7-and$m.traversalVariant-le16-and$m.traversalWeight-gt.9-and$m.weight-gt.9){$seen[[int]$m.traversalVariant]=$true;$events[[string]$m.traversalEvent]=$true;$holster=$holster-or(!$m.presentedDrawn-and$m.armedMix-lt.05)}
   $rows.Add([pscustomobject]@{source=$src;reverse=[bool]$Reverse;crouched=[bool]$CrouchedStart;repeat=$repeat;ms=$watch.ElapsedMilliseconds;videoTime=$(if($overall){.7+$overall.Elapsed.TotalSeconds}else{$null});native=$n;movement=$m;stream=$stream;sourcePosition=(S $src).localPosition;target=(S $dst)})
   if(!$m.active-or!$stream.armed-or!$stream.ready){throw "Stream retired: $($stream.result)"};Start-Sleep -Milliseconds 15
  }
  $offset=if($Reverse){5}else{0};$expected=if($CrouchedStart){@(10+$offset;8+$offset;11+$offset)}else{@(7+$offset;8+$offset;9+$offset)}
  foreach($v in $expected){if(!$nativeSeen[$v]-or!$seen[$v]){throw "Missing native/remote keyhole phase $v"}}
  if(!$exitSent-or$events.Count-lt3){throw 'Keyhole phases did not own independent events'};$checks.Add("Repeat ${repeat}: native entry, continuous look and exit drive remote body on distinct Steam events")
  if(!$holster-or(Q $dst '--avatar-sword' $handles[$dst]).equipped){throw 'Keyhole failed to holster remote sword'};$checks.Add("Repeat ${repeat}: native hands-free state moves the remote sword to its holster")
  if($m.traversalWeight-gt.02-or$m.weight-lt.9-or$n.stance-ne[int][bool]$CrouchedStart-or[Math]::Abs($m.stanceMix-[int][bool]$CrouchedStart)-gt.01){throw 'Keyhole recovery did not preserve native stance'}
  if(@((Q $dst '--player-states').active|Where-Object id -eq 24).Count){throw 'Observer entered keyhole'};$checks.Add("Repeat ${repeat}: native exit stance restored and observer first person unaffected")
 }
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}};$checks.Add('Players and avatars retain health')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original files and protected saves unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;if(@((Q $r '--player-states').active|Where-Object id -eq 24).Count){I $r B 100;Start-Sleep -Milliseconds 1800};C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((Q $r '--player-states').stance-eq1){I $r B 100;Start-Sleep -Milliseconds 650};C $r 'setviewpos -33.5 -153 68.1 0 0';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2500};C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 13|Set-Content -Encoding utf8 "$out/states.json"
 if($capture){$capture.WaitForExit(35000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Capture incomplete'}}
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;source=$src;reverse=[bool]$Reverse;crouchedStart=[bool]$CrouchedStart;recorded=[bool]$Record;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Native Dunwall office door keyhole, two normal controller entry/hold/exit sequences. Full-body NPC donor, native phase clock, native equipment state. No camera, root, door state or animation-event writes. Arbitrary doors, moving/early cancellation and full lifecycle remain separate.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"KEYHOLE_VALIDATED $($checks.Count) checks"
