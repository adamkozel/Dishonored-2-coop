param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='double-jump-corvo-v21-v1',[ValidateSet('host','client')][string]$Source='host',[switch]$Record)
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
  if((Q $r '--player-agility').quantity-or(Q $r '--health' 1).current-ne56-or!(Q $r '--player-sword').equipped){throw 'Baseline not clean'}
  $role=if($r-eq'host'){'emily'}else{'corvo'};Copy-Item "$PSScriptRoot/double-jump-banks-v1/$role-double-jump-bank.bin" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
 }
 C $dst 'setviewpos -28 -149.2 68.1 225 0';C $src 'setviewpos -34 -153 68.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $src -Pitch 0 -Evidence "$out/source-camera.json"
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $dst -Pitch -12 -Evidence "$out/observer-camera.json"
 C $src 'coop_agility_fixture 30000';Start-Sleep -Milliseconds 600;if((Q $src '--player-agility').quantity-ne1){throw 'Agility grant failed'}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C $dst 'coop_movement_bank on'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$m=S $dst '.movement.json';if($m.active-and$m.version-eq21-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne21-or$m.weight-lt.9){throw 'Bank readiness deadline'}
 if($Record){
  $window=(Get-Process -Id $pids[$dst]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','38','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'};$overall=[Diagnostics.Stopwatch]::StartNew()
 }
 foreach($armed in $true,$false){
  if(!$armed){I $src X 1500;Start-Sleep -Milliseconds 2800}
  foreach($moving in $false,$true){
   I $src;C $src 'setviewpos -34 -153 68.1 0 0';Start-Sleep -Milliseconds 800
   $before=(S $dst '.movement.json').secondJumpSerial;$y=if($moving){18000}else{0};I $src A 100 $y
   $watch=[Diagnostics.Stopwatch]::StartNew();$sent=$false;$nativeFirst=$false;$nativeSecond=$false;$presented=$false;$progress=0;$fall=$false;$land=$false
   while($watch.ElapsedMilliseconds-lt3500){
    if(!$sent-and$watch.ElapsedMilliseconds-ge180){I $src A 650 $y;$sent=$true}
    $n=Q $src '--player-states';$m=S $dst '.movement.json';$stream=S $dst '.shadow-stream.json'
    $rows.Add([pscustomobject]@{source=$src;armed=$armed;moving=$moving;ms=$watch.ElapsedMilliseconds;videoTime=$(if($overall){.7+$overall.Elapsed.TotalSeconds}else{$null});native=$n;movement=$m;stream=$stream;sourcePosition=(S $src).localPosition;target=(S $dst)})
    foreach($j in @($n.active|Where-Object id -eq 2)){$nativeFirst=$nativeFirst-or$j.jumpResearch.remaining94-eq1;$nativeSecond=$nativeSecond-or$j.jumpResearch.remaining94-eq0}
    if($m.secondJump-and$m.weight-gt.8){$presented=$true;$progress=[Math]::Max($progress,$m.secondJumpProgress)}
    $fall=$fall-or($m.airState-eq2-and$m.airWeight-gt.8);$land=$land-or($m.airState-eq3-and$m.airWeight-gt.8)
    if(!$m.active-or!$stream.armed-or!$stream.ready){throw "Stream retired: $($stream.result)"};Start-Sleep -Milliseconds 15
   }
   if(!$nativeFirst-or!$nativeSecond-or!$presented-or$progress-lt.8-or!$fall-or!$land-or$m.secondJumpSerial-ne($before+1)){throw "Double jump missing: first=$nativeFirst second=$nativeSecond remote=$presented phase=$progress fall=$fall land=$land serial=$($m.secondJumpSerial)/$before"}
   $checks.Add("${src}: drawn=$armed moving=$moving native second impulse produces one Steam-driven full-body second ascent, fall and landing")
   if($m.secondJump-or$m.airWeight-gt.01-or$m.weight-lt.9){throw 'Landing did not recover gait'}
   if([bool](Q $dst '--avatar-sword' $handles[$dst]).equipped-ne$armed){throw 'Remote equipment changed'}
   if(@((Q $dst '--player-states').active|Where-Object {$_.id-eq2-or$_.id-eq3}).Count){throw 'Observer entered jump'}
   $checks.Add("${src}: drawn=$armed moving=$moving grounded recovery and native equipment/observer first-person preserved")
  }
 }
 C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'};C $src 'coop_agility_fixture off';Start-Sleep -Milliseconds 650
 if((Q $src '--player-agility').quantity-or(S $src '.agility-fixture.json').active){throw 'Upgrade restoration failed'};$checks.Add('Temporary native Agility upgrade restored')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}};$checks.Add('Players and avatars retain health')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original files and protected saves unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_agility_fixture off';Start-Sleep -Milliseconds 650;if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2500};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 13|Set-Content "$out/states.json"
 if($capture){$capture.WaitForExit(45000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Capture incomplete'}}
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;source=$src;recorded=[bool]$Record;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Native Agility two-input jump, stationary and moving, drawn and holstered. Same delayed Steam root drives full-body presentation. Visual review, midair attacks, arbitrary geometry and interruptions remain separate.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"DOUBLE_JUMP_VALIDATED $($checks.Count) checks"
