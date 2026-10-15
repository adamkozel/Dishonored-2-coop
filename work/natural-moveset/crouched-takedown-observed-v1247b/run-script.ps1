param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='crouched-takedown-observed-v1247b',[string]$Candidate='candidate-npc-combat-v1213r',[string[]]$Sources=@('host','client'),[string[]]$Kinds=@('choke','lethal'))
# 1247b: 1247 passed, but from in front of the guard the guard hid the partner. The held camera now looks across from
# the side: 2.8 m off the midpoint between source and guard, raised 0.4 m, pitched down 10 degrees.
# 1247 (visual take for crouched_takedown): 1245d passed every case, but the observer stood behind the upper-floor
# railing and filmed a wall. The observer now films through a held native camera (coop_observer_camera hold,
# chapter 80): about 2 m beyond the guard, 0.6 m to the side, raised 0.6 m and pitched down, looking back at the
# source; its body is parked away. Runtime criteria are unchanged.
# 1245d (1213r): the choke hold now keeps a valid wire stance (1245c: ~8 rejected pose packets per hold, no
# engine stall). Kinds: crouched choke, crouched lethal, and a standing choke as the regression check (the same
# rejection hit any choke hold); the standing case requires a standing partner (stance mix < 0.2) before input.
# 1245c (1213q): 1245b plus the engine_call_gap timing; the retirement snapshot is fully guarded (1245b's
# client case stopped on a status-file sharing violation).
# 1245b: in 1245 the crouched lethal passed in both roles, but during the crouched choke the observer retired its
# movement bank about 1 s into the choke (choke loop, source stance 12) in both roles. Choke only, with per-sample
# observer refusal/stream/follow fields, full observer status snapshots at the first retirement, and both peers'
# native service timings after each case.
# 1245 (#11, crouched_takedown): the partner presentation of rear takedowns started from CROUCH, both source
# roles. Probe 1244b: a crouched choke stands the player into Choke41 (choke_back/loop/win, as standing); a
# crouched fast-back lethal plays assassination_fast_back while the player stays crouched (Assassination13 at
# stance 1), which 1213p admits. Per case: the presentation is armed as the native choke harnesses do (exact
# follow, movement bank, shadow stream; AI held by the movement fixture), the source crouches 0.9 m behind the
# Dunwall upper-floor tutorial guard 2, the observer stands 2-3 m to the side, and ordinary input performs the
# takedown (RightShoulder held to finish, or RightTrigger). Both windows are recorded and the observer's movement
# status is sampled. Pass per case: the native paired scene is observed on the source; the observer presents
# every phase (choke 1/2/3, lethal 5) at weight > 0.8 from a crouched partner (stance mix > 0.8 before input);
# locomotion returns after the scene. The fixture (PrepareShadowSword + note restore) is restored after every case.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$inv=[Globalization.CultureInfo]::InvariantCulture
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
function F($v){([double]$v).ToString('F5',$inv)}
function Guard($r){$g=@((Q $r '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'});if($g.Count-ne1){throw 'Exact upper-floor guard2 absent'};$g[0]}
function Disarm{foreach($r in 'host','client'){try{I $r;foreach($cmd in 'coop_observer_camera off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off'){C $r $cmd}}catch{}}}
function Restore($tag){Disarm
 & "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"
 & "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration-$tag" *>> "$out/note-restoration.log"
 foreach($r in 'host','client'){$restored=Q $r '--player-inventory'
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw "Checkpoint inventory differs ($r)"}
  if((Q $r '--health' 1).current-ne56){throw "Restored player health differs ($r)"}}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='CrouchedTakedownObserved1247b';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;$dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
 }
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;dll=$dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;sources=$Sources;kinds=$Kinds})
 foreach($source in $Sources){
  $peer=if($source-eq'host'){'client'}else{'host'}
  foreach($kind in $Kinds){
   $key="$source-$kind";$row=[ordered]@{source=$source;observer=$peer;kind=$kind;passed=$false;failure=$null;checks=[Collections.Generic.List[string]]::new()}
   $capture=$null;$peerCapture=$null;$monitor=$null
   try{
    $dirty=$true
    foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}}
    foreach($r in 'host','client'){if((Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Living partner fixture required'}}
    C $peer 'coop_movement_bank on'
    $bw=[Diagnostics.Stopwatch]::StartNew();do{$m=S $peer '.movement.json';if($m.active-and$m.version-ge65){break};Start-Sleep -Milliseconds 50}while($bw.ElapsedMilliseconds-lt8000)
    if(!$m.active-or$m.version-lt65){throw "Movement bank not active on the observer (version $($m.version))"};$row.bankVersion=$m.version
    foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
    $g=Guard $source;$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1]
    if([Math]::Abs($fx*$fx+$fy*$fy-1)-gt.01){throw 'Guard forward not horizontal'}
    $px=[double]$g.position[0]-.9*$fx;$py=[double]$g.position[1]-.9*$fy;$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
    C $source ('setviewpos {0} {1} 68.1 {2} 0' -f (F $px),(F $py),$look.ToString('F4',$inv));Start-Sleep -Milliseconds 800
    $want=if($kind-eq'standing-choke'){0}else{1}
    $st=Q $source '--player-states';if($st.stance-ne$want){I $source B 180;Start-Sleep -Milliseconds 800;$st=Q $source '--player-states'}
    if($st.stance-ne$want){throw "Source stance $($st.stance), expected $want"}
    # Observer camera: held native view beyond the guard, looking back at the source; body parked away.
    $mx=$px+.45*$fx;$my=$py+.45*$fy;$side=if($source-eq'host'){2.8}else{-2.8};$ax=$mx-$side*$fy;$ay=$my+$side*$fx;$ayaw=[Math]::Atan2($my-$ay,$mx-$ax)*180/[Math]::PI
    & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 10 -Quick|Out-Null
    C $peer ('setviewpos {0} {1} 68.5 {2} 0' -f (F $ax),(F $ay),$ayaw.ToString('F4',$inv));Start-Sleep -Milliseconds 700
    C $peer 'coop_observer_camera hold';Start-Sleep -Milliseconds 300;$hold=S $peer '.observer-camera.json';Save "$out/$key-observer-camera.json" $hold
    C $peer 'setviewpos -33 -153 68.1 90 0';Start-Sleep -Milliseconds 500
    $row.camera=@{x=$ax;y=$ay;z=68.5;yaw=$ayaw;pitch=10;side=$side}
    $stanceOk={param($x)if($want){$x-gt.8}else{$x-lt.2}}
    $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 200;$m=S $peer '.movement.json';$ps=S $peer}while(!($m.weight-gt.85-and(& $stanceOk $m.stanceMix)-and$ps.probeFollowEnabled)-and$w.ElapsedMilliseconds-lt8000)
    $row.before=@{weight=$m.weight;stanceMix=$m.stanceMix;follow=$ps.probeFollowEnabled;waitMs=$w.ElapsedMilliseconds}
    if(!($m.weight-gt.85-and(& $stanceOk $m.stanceMix))){throw "Partner stance not presented before input (weight $($m.weight), stance mix $($m.stanceMix))"};$row.checks.Add("Observer presents the partner at stance $want before the takedown")
    foreach($pair in @(@($source,'source'),@($peer,'observer'))){$hwnd=(Get-Process -Id $pids[$pair[0]]).MainWindowHandle.ToInt64()
     $p=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$key-$($pair[1]).mp4") -WindowStyle Hidden -RedirectStandardError "$out/$key-$($pair[1])-capture.log" -RedirectStandardOutput "$out/$key-$($pair[1])-capture.stdout" -PassThru
     if($pair[1]-eq'source'){$capture=$p}else{$peerCapture=$p}}
    $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$source],'--watch-midnight','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/$key-scenes.ndjson" -RedirectStandardError "$out/$key-scenes.stderr" -PassThru
    $snapped=$false;$samples=[Collections.Generic.List[object]]::new();$watch=[Diagnostics.Stopwatch]::StartNew();$sent=$false
    while($watch.ElapsedMilliseconds-lt9500){
     if(!$sent-and$watch.ElapsedMilliseconds-ge800){$sent=$true;$row.inputMs=$watch.ElapsedMilliseconds
      if($kind-ne'lethal'){I $source RightShoulder 6500}
      else{& "$lab/Send-D2Input.ps1" -Role $source -RightTrigger 255 -DurationMs 250|Out-Null}}
     $player=try{Q $source '--player-states'}catch{$null};$mv=S $peer '.movement.json';$ps=S $peer
     $xs=S $peer '.shadow-stream.json'
     if(!$mv.active-and!$snapped){$snapped=$true;try{foreach($suffix in '','.movement.json','.shadow-stream.json','.movement-resume.json'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$peer/coop-status.json$suffix" -Destination "$out/$key-retired-observer$suffix" -ErrorAction Stop}catch{}};try{Copy-Item -LiteralPath "$root/work/lab-instances/$source/coop-status.json.shadow-stream.json" -Destination "$out/$key-retired-source.shadow-stream.json"}catch{}}catch{}}
     $samples.Add([pscustomobject]@{diag=[pscustomobject]@{end=$mv.end;refuseStreak=$mv.refuseStreak;refuseLast=$mv.refuseLast;refuseReasons=$mv.refuseReasons;beginRefusal=$mv.beginRefusal;eligibility=$mv.eligibility.mask;streamArmed=$xs.armed;streamReady=$xs.ready;streamResult=$xs.result;remoteFresh=$ps.remoteFresh;lastReceiveAge=$ps.lastReceiveAgeMs};ms=$watch.ElapsedMilliseconds;stance=$player.stance;active=@($player.active|ForEach-Object {$_.id});chokeSceneValid=$player.chokeSceneValid;chokeVariant=$player.chokeScene.variant;sourcePosition=$player.position;
      remote=[pscustomobject]@{chokeVariant=$mv.chokeVariant;chokeWeight=$mv.chokeWeight;chokeAmount=$mv.chokeAmount;weight=$mv.weight;stanceMix=$mv.stanceMix;active=$mv.active};render=$ps.probeRenderPosition;follow=$ps.probeFollowEnabled})
     Start-Sleep -Milliseconds 60}
    if(!$monitor.WaitForExit(15000)){Stop-Process -Id $monitor.Id;throw 'Native scene monitor did not finish'};$monitor=$null
    foreach($c in $capture,$peerCapture){if(!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}};$capture=$peerCapture=$null
    Save "$out/$key-samples.json" @($samples)
    foreach($q in 'host','client'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json.native-service-timing.json" -Destination "$out/$key-$q.native-service-timing.json"}catch{}}
    $scenes=@{};foreach($line in Get-Content "$out/$key-scenes.ndjson"){try{$o=$line|ConvertFrom-Json}catch{continue};foreach($x in @($o.instances)){if("$($x.asset)"-match'/player/.*/(choke|assassination)/'){$scenes["$($x.asset)|$($x.sceneDurationMs)"]=1}}}
    $row.scenes=@($scenes.Keys|Sort-Object)
    $expected=if($kind-ne'lethal'){@(1,2,3)}else{@(5)}
    if($kind-ne'lethal'-and!(@($row.scenes|Where-Object {$_-like'*choke_back*'}).Count)){throw 'Native choke scenes not observed'}
    if($kind-eq'lethal'-and!(@($row.scenes|Where-Object {$_-like'*assassination_fast_back*'}).Count)){throw 'Native fast-back lethal scene not observed'}
    $row.checks.Add("Native scene(s) on the source: $($row.scenes -join ', ')")
    if($kind-eq'lethal'){$crouchedLethal=@($samples|Where-Object {$_.stance-eq1-and($_.active-contains13)}).Count;$row.crouchedAssassinationSamples=$crouchedLethal;if(!$crouchedLethal){throw 'Crouched Assassination13 not observed on the source'}}
    foreach($v in $expected){
     if(!@($samples|Where-Object {$_.chokeSceneValid-and$_.chokeVariant-eq$v}).Count){throw "Owned native phase $v not read on the source"}
     if(!@($samples|Where-Object {$_.remote.chokeVariant-eq$v-and$_.remote.chokeWeight-gt.8-and$_.remote.weight-gt.85}).Count){throw "Remote phase $v not presented"}
     $row.checks.Add("Source phase $v read and presented on the observer at weight > 0.8")}
    $last=($samples|Where-Object {$_.remote.chokeWeight-gt.8}|Measure-Object ms -Maximum).Maximum
    $recovered=@($samples|Where-Object {$_.ms-gt($last+300)-and$_.remote.chokeWeight-lt.05-and$_.remote.weight-gt.85})
    $row.recoveredSamples=$recovered.Count;if(!$recovered.Count){throw 'Locomotion did not return after the takedown'};$row.checks.Add('Locomotion returns after the takedown')
    $row.passed=$true
   }catch{$row.failure=$_.Exception.Message}
   foreach($c in $capture,$peerCapture,$monitor){if($c){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}}
   try{Restore $key;$row.restored=$true}catch{$row.restored=$false;$row.failure=(("$($row.failure); restore: "+$_.Exception.Message)).TrimStart('; ')}
   $rows.Add([pscustomobject]$row);Save "$out/$key.json" ([pscustomobject]$row)
   if(!$row.restored){throw "Fixture restore failed after $key"}
  }
 }
 $dirty=$false
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}finally{
 if($owned){
  if($dirty){try{Restore 'final'}catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}else{Disarm}
  $errors=[Collections.Generic.List[string]]::new()
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}}catch{$errors.Add($_.Exception.Message)}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh-or(Q $r '--health' 1).current-ne56){$errors.Add("$r gameplay/transport/health not restored")}}catch{$errors.Add("$r status unavailable")}}
  if(Test-Path -LiteralPath $out){
   Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)}
   $bad=@($rows|Where-Object {!$_.passed});if(!$failure-and$bad.Count){$failure="$($bad.Count) case(s) failed: "+(($bad|ForEach-Object {"$($_.source)-$($_.kind): $($_.failure)"})-join ' | ')}
   if($errors.Count-and!$failure){$failure=$errors-join '; '}
   Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;cases=@($rows);scope='crouched_takedown visual take: partner presentation of rear choke and fast-back lethal started from crouch filmed through a held observer camera, both source roles, Dunwall upper-floor guard 2; fixture restored after every case.'})
  }
  [ordered]@{pid=$PID;operation='CrouchedTakedownObserved1247b';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_CROUCHED_TAKEDOWN_NETWORK $($rows.Count) cases"
