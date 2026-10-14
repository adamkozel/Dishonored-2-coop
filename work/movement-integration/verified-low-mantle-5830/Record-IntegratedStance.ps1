param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='integrated-stance-corvo-v1',[switch]$Sprint,[switch]$Lean,[switch]$Slide,[switch]$SlideJump,[switch]$Air,[switch]$Combat,[switch]$Equipment,[switch]$CloseEquipment,[switch]$Unarmed,[switch]$VerticalPeek,[switch]$Mantle)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'}
$liveDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
function C($r,$text){& "$lab/Send-D2Command.ps1" -Role $r -Command $text -WaitConsumed|Out-Null}
function ReadTestStatus($path){for($attempt=0;$attempt-lt8;$attempt++){try{return(Get-Content -LiteralPath $path -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{if($attempt-eq7){throw};Start-Sleep -Milliseconds 15}}}
function B($r){ReadTestStatus "$root/work/lab-instances/$r/coop-status.json"}
function I($r,$x=0,$y=0,$ms=100,$button='Neutral',$trigger=0,$rightX=0){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -DurationMs $ms -Button $button -RightTrigger $trigger -RightX $rightX|Out-Null}
$events=@(
 @(0,0,0,100,'Neutral',0,'Armed standing'),@(1000,0,14000,900,'Neutral',0,'Walk forward'),@(2400,0,-14000,900,'Neutral',0,'Walk backward'),
 @(3800,-14000,0,700,'Neutral',0,'Walk left'),@(4900,14000,0,700,'Neutral',0,'Walk right'),
 @(6400,0,0,180,'B',0,'Stand to crouch'),@(8200,0,0,100,'Neutral',0,'Crouched idle'),
 @(9400,0,19000,900,'Neutral',0,'Crouch forward'),@(10800,0,-19000,900,'Neutral',0,'Crouch backward'),
 @(12200,-19000,0,900,'Neutral',0,'Crouch left'),@(13600,19000,0,900,'Neutral',0,'Crouch right'),
 @(15000,-14000,14000,800,'Neutral',0,'Crouch forward left'),@(16300,14000,-14000,800,'Neutral',0,'Crouch backward right'),
 @(17600,14000,14000,800,'Neutral',0,'Crouch forward right'),@(18900,-14000,-14000,800,'Neutral',0,'Crouch backward left'),
 @(20400,0,0,100,'Neutral',0,'Crouch stop'),@(21600,0,0,180,'B',0,'Crouch to stand'),
 @(23500,0,0,900,'RightShoulder',0,'Native block takes priority'),@(25200,0,0,180,'Neutral',255,'Native sword attack takes priority'),
 @(27200,0,0,180,'A',0,'Native jump takes priority'),@(29300,0,0,100,'Neutral',0,'Standing recovery'),
 @(30600,0,32767,450,'Neutral',0,'Run forward'),@(31900,0,-32767,450,'Neutral',0,'Run backward'),@(33500,0,0,100,'Neutral',0,'Final idle')
)
$rows=[Collections.Generic.List[object]]::new();$chapters=[Collections.Generic.List[object]]::new();$failure=$null;$capture=$null;$started=0;$captureStart=0;$fixture=@{};$healthBefore=@{};$healthAfter=@{};$pids=@{}
function NativeHealth($r){$raw=& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] --health 1;if($LASTEXITCODE){throw 'Native health unavailable'};return($raw|ConvertFrom-Json)}
function F($r){ReadTestStatus "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json"}
$captureDuration=if($Mantle){12}elseif($Equipment){30}elseif($Combat){31}elseif($Slide){14}elseif($Air){26}else{38};$inputDuration=($captureDuration-2)*1000
if($Combat){$events=@(
 @(0,0,0,100,'Neutral',0,'Armed standing'),@(1300,0,0,180,'Neutral',255,'Native short slash'),
 @(4200,0,0,180,'Neutral',255,'Combo first slash'),@(4750,0,0,180,'Neutral',255,'Combo second slash'),@(5300,0,0,180,'Neutral',255,'Combo third slash'),
 @(7800,0,0,1500,'Neutral',255,'Heavy windup, hold and release'),@(11300,0,0,1500,'RightShoulder',0,'Standing guard and release'),
 @(15100,0,0,180,'B',0,'Crouch'),@(17300,0,0,180,'Neutral',255,'Crouched slash'),
 @(20400,0,0,1500,'RightShoulder',0,'Crouched guard and release'),@(24300,0,0,180,'B',0,'Return to standing'),
 @(27300,0,0,100,'Neutral',0,'Final armed idle')
)}
if($Equipment){$events=@(
 @(0,0,0,100,'Neutral',0,'Armed standing'),@(1200,0,0,1500,'X',0,'Holster sword'),
 @(4300,0,14000,700,'Neutral',0,'Unarmed forward walk'),@(5800,0,-14000,700,'Neutral',0,'Unarmed backward walk'),
 @(7600,0,0,650,'X',0,'Draw sword at body-relative hand contact'),@(10500,0,0,180,'B',0,'Crouch'),
 @(12600,0,0,1500,'X',0,'Crouched holster'),@(15700,-16000,0,650,'Neutral',0,'Unarmed crouch left'),@(17100,16000,0,650,'Neutral',0,'Unarmed crouch right'),
 @(19000,0,0,650,'X',0,'Crouched draw'),@(21900,0,0,180,'B',0,'Stand'),
 @(24300,0,0,700,'RightShoulder',0,'Guard after equipment change'),@(26800,0,0,100,'Neutral',0,'Final recovery')
)}
if($CloseEquipment){
 if(!$Equipment){throw 'CloseEquipment requires Equipment'}
 # Stationary contact study complements, rather than replaces, the travelling take.
 $events=@($events|ForEach-Object {if($_[1]-ne0-or$_[2]-ne0){,@($_[0],0,0,100,'Neutral',0,'Stationary grip inspection')}else{,$_}})
}
if($Air){$events=@(
 @(0,0,0,100,'Neutral',0,'Armed standing'),@(1500,0,0,180,'A',0,'Standing jump'),
 @(5000,0,0,800,'A',0,'Held jump'),@(9000,0,0,180,'B',0,'Crouch'),
 @(11000,0,0,180,'A',0,'Crouched jump and landing'),@(15000,0,0,180,'B',0,'Standing recovery'),
 @(17000,0,18000,500,'Neutral',0,'Move into jump'),@(17300,0,18000,180,'A',0,'Moving jump'),
 @(21000,0,0,100,'Neutral',0,'Landing recovery'),@(23000,0,0,100,'Neutral',0,'Final idle')
)}
if($Sprint){
 $events=@(
  @(0,0,0,100,'Neutral',0,'Armed standing'),@(1500,0,32767,650,'LeftThumb',0,'Native sprint forward'),
  @(4000,0,-24000,900,'Neutral',0,'Run backward recovery'),@(6500,0,0,100,'Neutral',0,'Standing stop'),
  @(8500,-16000,28000,650,'LeftThumb',0,'Native sprint diagonal left'),@(11000,15000,-26000,900,'Neutral',0,'Run back right recovery'),
  @(13500,0,0,100,'Neutral',0,'Standing stop'),@(15500,16000,28000,650,'LeftThumb',0,'Native sprint diagonal right'),
  @(18000,-15000,-26000,900,'Neutral',0,'Run back left recovery'),@(20500,0,0,180,'B',0,'Stand to crouch'),
  @(22500,0,18000,900,'Neutral',0,'Crouch forward'),@(24500,0,-18000,900,'Neutral',0,'Crouch backward'),
  @(27000,0,0,180,'B',0,'Crouch to stand'),@(29500,0,0,800,'RightShoulder',0,'Native block priority'),
  @(31500,0,0,180,'Neutral',255,'Native slash priority'),@(33500,0,0,100,'Neutral',0,'Standing recovery')
 )
}
if($Lean){
 $events=@(
  @(0,0,0,100,'Neutral',0,'Armed standing'),@(1500,-32767,0,650,'Y',0,'Native standing lean left'),
  @(4500,0,-22000,350,'Neutral',0,'Movement exits left lean'),@(6000,0,22000,350,'Neutral',0,'Return forward'),
  @(8000,32767,0,650,'Y',0,'Native standing lean right'),@(11000,0,-22000,350,'Neutral',0,'Movement exits right lean'),
  @(12500,0,22000,350,'Neutral',0,'Return forward'),@(14500,0,0,180,'B',0,'Stand to crouch'),
  @(17000,-32767,0,650,'Y',0,'Native crouched lean left'),@(20000,0,-22000,350,'Neutral',0,'Movement exits crouched left lean'),
  @(21500,0,22000,350,'Neutral',0,'Crouch forward'),@(23500,32767,0,650,'Y',0,'Native crouched lean right'),
  @(26500,0,-22000,350,'Neutral',0,'Movement exits crouched right lean'),@(28000,0,22000,350,'Neutral',0,'Crouch forward'),
  @(30000,0,0,180,'B',0,'Crouch to stand'),@(33500,0,0,100,'Neutral',0,'Standing recovery')
 )
}
if($Slide){
 $events=@(
  @(0,0,0,100,'Neutral',0,'Armed standing'),@(1500,0,32767,1000,'LeftThumb',0,'Native sprint'),
  @(1850,0,32767,500,'B',0,'Native slide'),@(4500,0,0,100,'Neutral',0,'Full-body recovery to crouch'),
  @(6000,0,0,180,'B',0,'Stand after slide'),@(9500,0,0,100,'Neutral',0,'Standing recovery')
 )
 if($SlideJump){$events=@(
  @(0,0,0,100,'Neutral',0,'Armed standing'),@(1500,0,32767,1000,'LeftThumb',0,'Native sprint'),
  @(1850,0,32767,500,'B',0,'Native slide'),@(2180,0,20000,180,'A',0,'Native jump interrupts slide'),
  @(4500,0,0,100,'Neutral',0,'Landing and stance recovery'),@(9500,0,0,100,'Neutral',0,'Final idle')
 )}
}
if($VerticalPeek){$events=@(
 @(0,0,0,100,'Neutral',0,'Standing idle'),@(1500,0,32767,1000,'Y',0,'Standing upward peek'),
 @(5000,32767,0,850,'Y',0,'Combine upward and sideways peek'),@(8000,0,-32767,1000,'Y',0,'Lower native peek'),
 @(10500,0,0,1000,'Neutral',0,'Native look releases lean',22000),@(12300,0,0,1000,'Neutral',0,'Return look',-22000),
 @(15000,0,0,180,'B',0,'Crouch'),@(17500,0,32767,1000,'Y',0,'Crouched upward peek'),
 @(21500,0,-32767,1000,'Y',0,'Lower crouched peek'),@(25000,0,0,1000,'Neutral',0,'Native look releases crouched lean',22000),
 @(26800,0,0,1000,'Neutral',0,'Return look',-22000),@(30000,0,0,180,'B',0,'Standing recovery'),@(33500,0,0,100,'Neutral',0,'Final idle')
)}
if($Unarmed){
 if(!$Sprint-and!$Lean-and!$Slide-and!$Air-and!$VerticalPeek-and!$Mantle){throw 'Unarmed recording requires a supported movement family'}
 $events=@($events|ForEach-Object {if($_[4]-eq'RightShoulder'-or$_[5]){,@($_[0],0,0,100,'Neutral',0,'Unarmed recovery')}else{,$_}})
}
if($Mantle){$events=@(
 @(0,0,0,100,'Neutral',0,'Standing at native desk'),@(1500,0,22000,1300,'A',0,'Native jump into low mantle'),
 @(4500,0,0,100,'Neutral',0,'Native landing and equipment recovery'),@(7500,0,0,100,'Neutral',0,'Final idle')
)}
try {
 foreach($r in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $launch.pid
  if(!$launch.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'}
  $pids[$r]=$launch.pid;$healthBefore[$r]=NativeHealth $r
  if((B $r).localGameLogicState-ne1-or$healthBefore[$r].current-ne56){throw 'Clean living native fixture required'}
  I $r;C $r 'coop_review_clip off';C $r 'coop_movement_bank off';C $r 'coop_authoring_proof off';C $r 'coop_shadow_stream off';C $r 'coop_mixamo_crouch off';C $r 'coop_probe_body off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'
  if((B $r).localPlayerStance-eq1){I $r 0 0 180 B;Start-Sleep -Milliseconds 1200}
 }
 if($Mantle){C $Source 'setviewpos -32 -148.5 68.1 90 0';C $target 'setviewpos -35.5 -147.7 68.1 12 0'}
 elseif($VerticalPeek){C $Source 'setviewpos -33.5 -153 68.1 0 0';C $target 'setviewpos -30.5 -149 68.1 233.13 0'}
 elseif($CloseEquipment){C $Source 'setviewpos -33.5 -153 68.1 0 0';C $target 'setviewpos -31.5 -151.4 68.1 218.66 0'}
 elseif($Combat-or$Equipment){C $Source 'setviewpos -33.5 -153 68.1 0 0';C $target 'setviewpos -30.5 -149 68.1 233.13 0'}
 elseif($Slide){C $Source 'setviewpos -34 -153 68.1 90 0';C $target 'setviewpos -31 -154.5 68.1 130 0'}
 elseif($Air){C $Source 'setviewpos -33.5 -153 68.1 0 0';C $target 'setviewpos -31 -146 68.1 250 8'}
 elseif($Sprint){C $Source 'setviewpos -35 -153 68.1 0 0';C $target 'setviewpos -31 -146 68.1 250 8'}
 elseif($Lean){C $Source 'setviewpos -35 -153 68.1 0 0';C $target 'setviewpos -30.5 -150.2 68.1 214 8'}
 else{C $Source 'setviewpos -34.3 -154.8 68.1 0 0';C $target 'setviewpos -30.5 -152.2 68.1 214 13'}
 Start-Sleep -Milliseconds 800
 if($Mantle){& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json";& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch 0 -Evidence "$out/camera.json"}
 if($Air-or$Sprint-or$Slide-or$Combat-or$Equipment-or$VerticalPeek){& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch $(if($VerticalPeek){12}elseif($Combat-or$Equipment){15}elseif($Air-or$Sprint){7}else{5}) -Evidence "$out/camera.json"}
 foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000'}
 Start-Sleep -Milliseconds 500
 foreach($r in 'host','client'){$fixture[$r]=F $r;if(!$fixture[$r].ready-or!$fixture[$r].movementFixture-or$fixture[$r].heldReferences-ne6){throw 'Movement fixture could not hold all three nearby NPCs'}}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C $target 'coop_movement_bank on'
 $readyWatch=[Diagnostics.Stopwatch]::StartNew();do{$m=ReadTestStatus "$root/work/lab-instances/$target/coop-status.json.movement.json";if($m.active-and$m.weight-ge.9){break};Start-Sleep -Milliseconds 35}while($readyWatch.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-lt3-or($Sprint-and$m.version-lt4)-or($Lean-and$m.version-lt5)-or($Slide-and$m.version-lt6)-or($Combat-and$m.version-lt8)-or($Equipment-and$m.version-lt9)-or$m.weight-lt.9){throw 'Candidate was not ready'}
 if($Unarmed){
  if($m.version-lt10){throw 'Weaponless special bank unavailable'}
  I $Source 0 0 1500 X;Start-Sleep -Milliseconds 3000
  if((ReadTestStatus "$root/work/lab-instances/$target/coop-status.json.movement.json").armedMix-gt.01){throw 'Remote sword did not holster'}
 }
 $launch=Get-Content "$root/work/lab-instances/$target/launch.json" -Raw|ConvertFrom-Json;$window=(Get-Process -Id $launch.pid).MainWindowHandle.ToInt64()
 if(!$window){throw 'Game capture window unavailable'}
 $ffmpeg=(Get-Command ffmpeg).Source
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$captureDuration",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $captureStart=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
 $capture=Start-Process -FilePath $ffmpeg -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 1100
 if($capture.HasExited){throw 'Window recorder failed at startup'}
 $started=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$watch=[Diagnostics.Stopwatch]::StartNew();$next=0
 while($watch.ElapsedMilliseconds-lt$inputDuration){
  if($next-lt$events.Count-and$watch.ElapsedMilliseconds-ge$events[$next][0]){
   $e=$events[$next];$at=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();I $Source $e[1] $e[2] $e[3] $e[4] $e[5] $(if($e.Count-gt7){$e[7]}else{0})
   $chapters.Add([pscustomobject]@{name=$e[6];seconds=($at-$captureStart)/1000;timing_precision='Command issue relative to recorder launch; native response and recorder initialization add latency.'});$next++
  }
  $m=ReadTestStatus "$root/work/lab-instances/$target/coop-status.json.movement.json";$s=B $Source;$t=B $target
  $stream=ReadTestStatus "$root/work/lab-instances/$target/coop-status.json.shadow-stream.json"
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;nativeStance=$s.localPlayerStance;nativeMotion=$s.localPlayerMotion;sourcePosition=$s.localPosition;observerPosition=$t.localPosition;remotePosition=$t.probeRenderPosition;fresh=$t.remoteFresh;movement=$m;stream=$stream})
  if(!$m.active-or!$t.remoteFresh-or$s.localGameLogicState-ne1-or$t.localGameLogicState-ne1){throw 'Gameplay presentation retired during capture'}
  if($Unarmed-and($m.armedMix-gt.01-or$m.presentedDrawn)){throw 'Weaponless recording unexpectedly drew a sword'}
  if(!(F $Source).ready-or!(F $target).ready){throw 'Movement fixture expired during capture'}
  Start-Sleep -Milliseconds 35
 }
 if($Slide){
  if(!@($rows|Where-Object {$_.stream.remoteState-eq6-and$_.movement.slideWeight-gt.8}).Count){throw 'Recording did not contain native-bound slide'}
  if($SlideJump-and!@($rows|Where-Object {$_.stream.remoteState-eq3-and(($_.movement.version-lt7-and$_.movement.weight-lt.01)-or($_.movement.version-ge7-and$_.movement.airWeight-gt.6))}).Count){throw 'Recording did not contain native jump interruption'}
  if(!$SlideJump-and!@($rows|Where-Object {$_.movement.slideState-eq2}).Count){throw 'Recording did not contain full-body slide recovery'}
 }
 if($Air){foreach($phase in 1,2,3){if(!@($rows|Where-Object {$_.movement.airState-eq$phase-and$_.movement.airWeight-gt.6}).Count){throw "Recording missing authored air phase $phase"}}}
 if($Mantle){if(!@($rows|Where-Object {$_.movement.traversalWeight-gt.7-and$_.movement.weight-gt.8}).Count-or$rows[-1].movement.traversalWeight-gt.01){throw 'Recording missing native low mantle and recovery'}}
 if($VerticalPeek){foreach($stance in 0,1){if(!@($rows|Where-Object {$_.nativeStance-eq$stance-and$_.movement.leanUp-gt.9-and$_.movement.leanWeight-gt.9}).Count){throw "Recording missing vertical peek stance $stance"}};if($rows[-1].movement.leanWeight-gt.01){throw 'Recording did not recover from native lean'}}
 if($Equipment){foreach($variant in 1,2,3,4){if(!@($rows|Where-Object {$_.movement.equipmentVariant-eq$variant-and$_.movement.equipmentWeight-gt.8}).Count){throw "Recording missing equipment variant $variant"}}}
 if($Combat){
  foreach($variant in 7,8,11){if(!@($rows|Where-Object {$_.movement.combatVariant-eq$variant-and$_.movement.combatWeight-gt.7}).Count){throw "Recording missing authored combat variant $variant"}}
 }
 $capture.WaitForExit(12000)|Out-Null
 if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Recording did not finalize'}
 foreach($r in 'host','client'){$healthAfter[$r]=NativeHealth $r;if($healthAfter[$r].current-ne$healthBefore[$r].current){throw 'Native player health changed during movement capture'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((B $r).localGameLogicState-eq1){if($Unarmed-and$r-eq$Source){I $r 0 0 650 X;Start-Sleep -Milliseconds 1400};C $r 'setviewpos -33.5 -153 68.1 0 0'};C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(45000)|Out-Null}
 $rows|ConvertTo-Json -Depth 9|Set-Content "$out/states.json"
 [ordered]@{isolation=$fixture;healthBefore=$healthBefore;healthAfter=$healthAfter;scope='Finite native Decision/Reflex holds on three exact fixture NPCs; no health, physics, animation or world asset writes.'}|ConvertTo-Json -Depth 8|Set-Content "$out/fixture.json"
 [ordered]@{character=$character;source=$Source;observer=$target;start=$started;captureStart=$captureStart;failure=$failure;chapters=@($chapters);events=$events;dll=$liveDll;bank=(Get-FileHash "$root/work/lab-instances/$target/coop-status.json.movement.bin").Hash;capture='FFmpeg gfxcapture, one game HWND, real-time timestamps, no duplicated output frames';scope=$(if($Equipment){'Actual native draw/holster scenes and source sword state drive authored whole-body movement and native clone equipment over Steam. Unarmed directions use native body clips. Visual contact and interruption acceptance remain pending.'}elseif($Combat){'Actual native sword scene semantics over Steam drive authored standing/crouched body attacks and guards. Root placement and local first-person controls remain native. Visual acceptance, hits, air combat and other movement families remain pending.'}else{'Actual player inputs and native Steam movement drive the candidate whole-body bank; see mode and bank version for covered actions. Complete movement integration remains pending.'})}|ConvertTo-Json -Depth 8|Set-Content "$out/take.json"
}
if($Unarmed){$takeFile=Join-Path $out 'take.json';$data=Get-Content $takeFile -Raw|ConvertFrom-Json;$data.scope='Actual native weaponless movement over Steam selects authored hands-free full-body variants. Native sword remains holstered throughout; donor motion retains native foot paths and bone lengths. Whole-family visual acceptance remains pending.';$data|Add-Member -NotePropertyName unarmed -NotePropertyValue $true;$data|ConvertTo-Json -Depth 8|Set-Content $takeFile}
if($failure){throw $failure};"INTEGRATED_STANCE_RECORDED $character"
