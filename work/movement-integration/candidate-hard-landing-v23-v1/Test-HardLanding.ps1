param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='hard-landing-corvo-v22-v1',[ValidateSet('host','client')][string]$Source='host',[switch]$Record,[switch]$CrouchedExit)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$src=$Source;$dst=if($src-eq'host'){'client'}else{'host'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$overall=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$b='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms -LeftY $y|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle
  I $r;foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_agility_fixture off','coop_movement_fixture off','coop_movement_fixture 60000','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_feet authored'){C $r $c}
  if((Q $r '--health' 1).current-ne56-or!(Q $r '--player-sword').equipped){throw 'Baseline not clean'}
  $role=if($r-eq'host'){'emily'}else{'corvo'};Copy-Item "$PSScriptRoot/hard-landing-crouch-banks-v1/$role-hard-landing-crouch-bank.bin" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
 }
 C $dst 'setviewpos -30 -150.5 68.1 212 0';C $src 'setviewpos -34 -153 68.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $src -Pitch 0 -Evidence "$out/source-camera.json"
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $dst -Pitch 8 -Evidence "$out/observer-camera.json"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C $dst 'coop_movement_bank on'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$m=S $dst '.movement.json';if($m.active-and$m.version-eq23-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne23-or$m.weight-lt.9){throw 'Bank readiness deadline'}
 if($Record){
  $window=(Get-Process -Id $pids[$dst]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','38','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'};$overall=[Diagnostics.Stopwatch]::StartNew()
 }
 foreach($armed in $true,$false){
  if(!$armed){I $src X 1500;Start-Sleep -Milliseconds 2800}
  foreach($repeat in 1,2){
   I $src;if((Q $src '--player-states').stance-eq1){I $src B 100;Start-Sleep -Milliseconds 650};C $src 'setviewpos -34 -153 68.1 0 0';Start-Sleep -Milliseconds 700
   C $src 'setviewpos -34 -153 74.6 0 0';$watch=[Diagnostics.Stopwatch]::StartNew();$native=$false;$presented=$false;$phase=0;$fall=$false;$event=0;$crouchSent=$false;$directCrouch=$false
   while($watch.ElapsedMilliseconds-lt3300){
    $n=Q $src '--player-states';$m=S $dst '.movement.json';$stream=S $dst '.shadow-stream.json'
    if($CrouchedExit-and!$crouchSent-and$n.landingSceneValid-and$n.landingScene.timeMs-ge200){I $src B 100;$crouchSent=$true}
    $rows.Add([pscustomobject]@{source=$src;armed=$armed;repeat=$repeat;ms=$watch.ElapsedMilliseconds;videoTime=$(if($overall){.7+$overall.Elapsed.TotalSeconds}else{$null});native=$n;movement=$m;stream=$stream;sourcePosition=(S $src).localPosition;target=(S $dst)})
    $native=$native-or$n.landingSceneValid;$fall=$fall-or@($n.active|Where-Object id -eq 3).Count-gt0
    if($m.traversalVariant-eq6-and$m.traversalWeight-gt.9-and$m.weight-gt.9){$presented=$true;$phase=[Math]::Max($phase,$m.traversalAmount);$event=$m.traversalEvent;$directCrouch=$directCrouch-or$m.landingCrouchMix-gt.85}
    if(!$m.active-or!$stream.armed-or!$stream.ready){throw "Stream retired: $($stream.result)"};Start-Sleep -Milliseconds 15
   }
   $minimumPhase=if($CrouchedExit){.5}else{.85}
   if(!$native-or!$presented-or!$fall-or$phase-lt$minimumPhase-or!$event){throw "Landing missing: native=$native remote=$presented phase=$phase fall=$fall event=$event"}
   $checks.Add("${src}: drawn=$armed repeat=$repeat native CatFall body scene drives remote complete-body landing through Steam")
   if($m.traversalWeight-gt.01-or$m.airWeight-gt.01-or$m.weight-lt.9){throw 'Recovery did not return to gait'}
   if($CrouchedExit-and(!$crouchSent-or!$directCrouch-or$n.stance-ne1-or$m.stanceMix-lt.99)){throw 'Native direct crouched exit not represented'}
   if([bool](Q $dst '--avatar-sword' $handles[$dst]).equipped-ne$armed){throw 'Remote equipment changed'}
   if(@((Q $dst '--player-states').active|Where-Object {$_.id-eq3-or$_.id-eq4}).Count){throw 'Observer entered landing'}
   $checks.Add("${src}: drawn=$armed repeat=$repeat native equipment and requested crouched=$CrouchedExit recovery/observer first-person preserved")
  }
 }
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}};$checks.Add('Players and avatars retain health')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original files and protected saves unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((Q $r '--player-states').stance-eq1){I $r B 100;Start-Sleep -Milliseconds 650};if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2500};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 13|Set-Content "$out/states.json"
 if($capture){$capture.WaitForExit(45000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Capture incomplete'}}
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;source=$src;crouchedExit=[bool]$CrouchedExit;recorded=[bool]$Record;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Native non-damaging 6.5m CatFall, drawn and holstered, repeated. Requested exit stance recorded separately. Strong/damaging falls, arbitrary geometry and interruptions remain separate.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"HARD_LANDING_VALIDATED $($checks.Count) checks"
