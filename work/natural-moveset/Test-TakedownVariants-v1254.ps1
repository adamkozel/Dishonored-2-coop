param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='takedown-variants-v1254a',[string]$Candidate='candidate-npc-combat-v1214c',[ValidateSet('back','left','right','front','drop','slide')][string[]]$Approaches=@('left','front','drop','slide'),[string[]]$Sources=@('client','host'),[string[]]$Kinds=@('choke','lethal'))
$takedownTable=@((Get-Content "$PSScriptRoot/../movement-integration/native-takedown-scenes-v1253.json" -Raw|ConvertFrom-Json).table)
# 1254 (1214c, #11): takedown variants under the product controller (natural guard AI, NPC stream on). Derived from
# Test-PairedVictim-v1248l.ps1 by make_takedown_variants_v1254.py. The attacker is placed at the guard's side (0.9 m,
# facing it), in front (1 m), 3 m above and slightly behind (drop; attack/choke pressed while falling) or 4.5 m behind
# (sprint, crouch into a slide, then attack/choke). Pass (runtime): the source plays a native takedown, the partner
# presents every census-1253 scene id it played, both worlds agree on the outcome and the victim ends within 2 m.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$inv=[Globalization.CultureInfo]::InvariantCulture
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
function F($v){([double]$v).ToString('F5',$inv)}
function PlaceCmd($g,$approach){$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$x=[double]$g.position[0];$y=[double]$g.position[1];$z=68.1;$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
 switch($approach){'back'{$x-=.9*$fx;$y-=.9*$fy}'left'{$x-=.9*$fy;$y+=.9*$fx;$look-=90}'right'{$x+=.9*$fy;$y-=.9*$fx;$look+=90}'front'{$x+=$fx;$y+=$fy;$look+=180}'drop'{$x-=.35*$fx;$y-=.35*$fy;$z+=3}'slide'{$x-=4.5*$fx;$y-=4.5*$fy}}
 'setviewpos {0} {1} {2} {3} 0' -f (F $x),(F $y),$z.ToString('F2',$inv),$look.ToString('F4',$inv)}
function SceneLive($path){try{$t=Get-Content -LiteralPath $path -Tail 1 -ErrorAction Stop;$t-match'/player/base/(assassination|choke)/[^"]*","subscene"'}catch{$false}}
function Guard($r){$g=@((Q $r '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'});if($g.Count-ne1){throw "Exact upper-floor guard2 absent ($r)"};$g[0]}
function Live($r){$s=S $r;$m=S $r '.movement.json';$x=S $r '.shadow-stream.json';$a=$s.probeRenderPosition;$b=$s.remotePosition
 $d=if($a-and$b){[Math]::Sqrt(($a[0]-$b[0])*($a[0]-$b[0])+($a[1]-$b[1])*($a[1]-$b[1])+($a[2]-$b[2])*($a[2]-$b[2]))}else{999}
 @{follow=$s.probeFollowEnabled;distance=[Math]::Round($d,3);product=(S $r '.product.json').result;bank=[bool]$m.active;stream=[bool]($x.armed-and$x.ready);ok=($s.probeActorHandle-and$s.probeFollowEnabled-and$d-lt1.0-and$m.active-and$x.armed-and$x.ready-and$s.remoteFresh)}}
function WaitLive($tag,$limit=30000){$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 300;$p=@{host=(Live host);client=(Live client)};$ok=$p.host.ok-and$p.client.ok}while(!$ok-and$w.ElapsedMilliseconds-lt$limit);$p.ok=$ok;$p.waitMs=$w.ElapsedMilliseconds;Save "$out/live-$tag.json" $p;$p}
function Arm{foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 if(!$ok){throw 'Product controller not armed'}}
function Disarm{$cw=[Diagnostics.Stopwatch]::StartNew()
 do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_observer_camera off';C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off'
   if((S $r '.reset.json').enabled){$off=$false;$st=S $r;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
  if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
 foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}}
function Restore($tag){Disarm
 & "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"
 & "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration-$tag" *>> "$out/note-restoration.log"
 foreach($r in 'host','client'){$restored=Q $r '--player-inventory'
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw "Checkpoint inventory differs ($r)"}
  if((Q $r '--health' 1).current-ne56){throw "Restored player health differs ($r)"}}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='TakedownVariants1254';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;$dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
 }
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;dll=$dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;sources=$Sources})
 foreach($approach in $Approaches){foreach($source in $Sources){foreach($kind in $Kinds){
  $peer=if($source-eq'host'){'client'}else{'host'};$key="$approach-$source-$kind"
  $row=[ordered]@{approach=$approach;source=$source;kind=$kind;observer=$peer;passed=$false;failure=$null;checks=[Collections.Generic.List[string]]::new()}
  $capture=$null;$peerCapture=$null;$monitor=$null
  try{
   $dirty=$true;Arm
   foreach($q in 'host','client'){C $q 'setviewpos -33 -153 68.1 90 0'}
   $live=WaitLive "$key-armed" 45000;if(!$live.ok){throw 'Partner presentation not live after arming'}
   # Camera behind the guard's current position along its facing (1247d framing), test-dropped first.
   $g=Guard $source;$row.guardHandleSource=$g.handle;$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
   $picked=$null;$camTrials=[Collections.Generic.List[object]]::new()
   foreach($back in 3.5,2.9,4.1){$tx=[double]$g.position[0]-$back*$fx;$ty=[double]$g.position[1]-$back*$fy
    C $peer ('setviewpos {0} {1} 68.1 {2} 0' -f (F $tx),(F $ty),$look.ToString('F4',$inv));Start-Sleep -Milliseconds 650;$feet=(Q $peer '--player-states').position
    $ok=[math]::Abs($feet[2]-$g.position[2])-lt.3-and[math]::Abs($feet[0]-$tx)-lt.4-and[math]::Abs($feet[1]-$ty)-lt.4
    $camTrials.Add([pscustomobject]@{back=$back;x=$tx;y=$ty;feet=$feet;ok=$ok});if($ok){$picked=$back;$ax=$tx;$ay=$ty;break}}
   Save "$out/$key-camera-trials.json" @($camTrials);$row.filmed=$null-ne$picked
   if($row.filmed){& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 22 -Quick|Out-Null
   C $peer ('setviewpos {0} {1} 69.1 {2} 0' -f (F $ax),(F $ay),$look.ToString('F4',$inv));Start-Sleep -Milliseconds 120
   C $peer 'coop_observer_camera hold';Start-Sleep -Milliseconds 300;Save "$out/$key-observer-camera.json" (S $peer '.observer-camera.json')}
   C $peer 'setviewpos -33 -153 68.1 90 0';Start-Sleep -Milliseconds 400
   $obsGuard=Guard $peer;$row.guardHandleObserver=$obsGuard.handle;$row.guardBefore=@{source=$g.position;observer=$obsGuard.position}
   foreach($pair in @(@($source,'source'),@($peer,'observer'))){$hwnd=(Get-Process -Id $pids[$pair[0]]).MainWindowHandle.ToInt64()
    $p=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','13','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$key-$($pair[1]).mp4") -WindowStyle Hidden -RedirectStandardError "$out/$key-$($pair[1])-capture.log" -RedirectStandardOutput "$out/$key-$($pair[1])-capture.stdout" -PassThru
    if($pair[1]-eq'source'){$capture=$p}else{$peerCapture=$p}}
   $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$source],'--watch-midnight','11000') -WindowStyle Hidden -RedirectStandardOutput "$out/$key-scenes.ndjson" -RedirectStandardError "$out/$key-scenes.stderr" -PassThru
   # Fresh placement right behind the walking guard, then the choke at once.
   $g=Guard $source;$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
   C $source (PlaceCmd $g $approach)
   $press={if($approach-eq'slide'){& "$lab/Send-D2Input.ps1" -Role $source -Button LeftThumb -DurationMs 1100 -LeftY 32767|Out-Null;Start-Sleep -Milliseconds 350;& "$lab/Send-D2Input.ps1" -Role $source -Button B -DurationMs 700 -LeftY 32767|Out-Null;Start-Sleep -Milliseconds 250}
    elseif($approach-eq'drop'){Start-Sleep -Milliseconds 150}
    if($kind-eq'choke'){I $source RightShoulder 6500}else{foreach($n in 1..$(if($approach-in'drop','slide'){4}else{1})){& "$lab/Send-D2Input.ps1" -Role $source -RightTrigger 255 -DurationMs 200|Out-Null;if($approach-in'drop','slide'){Start-Sleep -Milliseconds 180}}}}
   Start-Sleep -Milliseconds 120;& $press;$row.inputAt=(Get-Date).ToString('o');$attempts=1;$pairedSeen=$false;$lastPress=0
   $samples=[Collections.Generic.List[object]]::new();$watch=[Diagnostics.Stopwatch]::StartNew()
   while($watch.ElapsedMilliseconds-lt10500){
    if(!$pairedSeen-and$attempts-lt3-and$watch.ElapsedMilliseconds-$lastPress-ge$(if($approach-in'drop','slide'){2600}else{1200})-and!(SceneLive "$out/$key-scenes.ndjson")){$g=Guard $source;$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
     C $source (PlaceCmd $g $approach);Start-Sleep -Milliseconds 120;& $press;$attempts++;$lastPress=$watch.ElapsedMilliseconds}
    $player=try{Q $source '--player-states'}catch{$null};$mv=S $peer '.movement.json'
    $ns=try{S $peer '.npc-stream.json'}catch{$null};$entry=if($ns){@($ns.entries|Where-Object {$_.handle-eq$row.guardHandleObserver})|Select-Object -First 1}else{$null}
    $sns=try{S $source '.npc-stream.json'}catch{$null};$sentry=if($sns){@($sns.entries|Where-Object {$_.handle-eq$row.guardHandleSource})|Select-Object -First 1}else{$null}
    $body=if($ns){@($ns.bodyEntries|Where-Object {$_.handle-eq$row.guardHandleObserver})|Select-Object -First 1}else{$null}
    $og=try{Guard $peer}catch{$null}
    if(@($player.active|Where-Object {$_.id-eq41-or$_.id-eq13}).Count-or$player.chokeSceneValid-or(SceneLive "$out/$key-scenes.ndjson")){$pairedSeen=$true}
    $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;stance=$player.stance;active=@($player.active|ForEach-Object {$_.id});chokeSceneValid=$player.chokeSceneValid;chokeVariant=$player.chokeScene.variant;
     remote=[pscustomobject]@{chokeVariant=$mv.chokeVariant;chokeWeight=$mv.chokeWeight;weight=$mv.weight;active=$mv.active};
     victim=[pscustomobject]@{entryResult=$entry.result;ready=$entry.ready;poses=$entry.poses;roots=$entry.roots;bodyApplied=$body.applied;position=$og.position;paired=$entry.pairedResult;hostLease=$entry.hostPairedLease;unconscious=$entry.unconscious};
     sourceVictim=[pscustomobject]@{result=$sentry.result;paired=$sentry.pairedResult;localPaired=$sentry.localPaired;unconscious=$sentry.unconscious;position=$(try{(Guard $source).position}catch{$null})};
     reverse=[pscustomobject]@{observerPoses=$entry.reversePoses;observerReverseRoot=$entry.reverseRoot;observerPresentedRoot=$entry.presentedRoot;sourcePoses=$sentry.reversePoses}})
    Start-Sleep -Milliseconds 80}
   if(!$monitor.WaitForExit(15000)){Stop-Process -Id $monitor.Id};$monitor=$null
   foreach($c in $capture,$peerCapture){if(!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}};$capture=$peerCapture=$null
   Save "$out/$key-samples.json" @($samples)
   foreach($q in 'host','client'){foreach($suffix in '.npc-stream.json','.npc-combat.json','.native-service-timing.json','.product.json'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json$suffix" -Destination "$out/$key-$q$suffix" -ErrorAction Stop}catch{}}}
   $scenes=@{};foreach($line in Get-Content "$out/$key-scenes.ndjson"){try{$o=$line|ConvertFrom-Json}catch{continue};foreach($x in @($o.instances)){if("$($x.asset)"-match'/player/.*/(choke|assassination)/'){$scenes["$($x.asset)|$($x.sceneDurationMs)"]=1}}}
   $row.scenes=@($scenes.Keys|Sort-Object);$want=if($kind-eq'choke'){'*/choke/*'}else{'*/assassination/*'};if(!@($row.scenes|Where-Object {$_-like$want}).Count){throw 'Native takedown not performed on the source (guard moved or noticed the source)'}
   $ids=@(foreach($sc in $row.scenes){$asset=($sc-split'\|')[0];$k=$asset-replace'^midnight/gameplay/player/base/','' -replace'\.midnightscene$','';$t=@($takedownTable|Where-Object {$_.scene-eq$k});if($t.Count-and$t[0].kind-ne4){$t[0].id}});$row.sceneIds=$ids;if(!$ids.Count){throw 'No census takedown scene played'}foreach($v in $ids){if(!@($samples|Where-Object {$_.remote.chokeVariant-eq$v-and$_.remote.chokeWeight-gt.8}).Count){throw "Remote phase $v not presented"};$row.checks.Add("Remote phase $v presented")}
   Start-Sleep -Milliseconds 3500
   $row.attempts=$attempts
   $readHealth={param($r,$h)try{Q $r '--health' $h}catch{$null}}
   $row.final=[ordered]@{source=(& $readHealth $source $row.guardHandleSource);observer=(& $readHealth $peer $row.guardHandleObserver)}
   $deaths=@{};foreach($q in 'host','client'){$deaths[$q]=try{(S $q '.npc-combat.json').deaths}catch{0}}
   $outcome={param($h,$r)if(!$h){if($deaths[$r]-ge1){'dead'}else{'unreadable'}}elseif($h.current-le0){'dead'}elseif($h.ownerSuppressed){'unconscious'}else{'conscious'}}
   $row.outcome=[ordered]@{source=(& $outcome $row.final.source $source);observer=(& $outcome $row.final.observer $peer)}
   $expected=if($kind-eq'choke'){'unconscious'}else{'dead'}
   if($row.outcome.source-ne$expected-or$row.outcome.observer-ne$expected){throw "Outcome differs: source $($row.outcome.source), observer $($row.outcome.observer), expected $expected"};$row.checks.Add("Same outcome in both worlds: $expected")
   $sv=try{(Guard $source).position}catch{$null};$ov=try{(Guard $peer).position}catch{$null};$row.victimEnd=[ordered]@{source=$sv;observer=$ov}
   if($sv-and$ov){$gap=[Math]::Sqrt(([double]$sv[0]-$ov[0])*([double]$sv[0]-$ov[0])+([double]$sv[1]-$ov[1])*([double]$sv[1]-$ov[1]));$row.victimGapM=[Math]::Round($gap,2);if($gap-gt2){throw "Victim ends $([Math]::Round($gap,2)) m apart in the two worlds"};$row.checks.Add("Victim ends $([Math]::Round($gap,2)) m apart in the two worlds")}
   $first=$samples[0].victim;$last=$samples[$samples.Count-1].victim
   $row.victim=[ordered]@{result=$last.entryResult;posesDelta=($last.poses-$first.poses);rootsDelta=($last.roots-$first.roots);bodyAppliedDelta=($last.bodyApplied-$first.bodyApplied);positionBefore=$first.position;positionAfter=$last.position}
   $row.guardHealthObserverAfter=try{(Q $peer '--health' $row.guardHandleObserver).current}catch{$null}
   $row.passed=$true
  }catch{$row.failure=$_.Exception.Message}
  foreach($c in $capture,$peerCapture,$monitor){if($c){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}}
  try{Restore $key;$row.restored=$true}catch{$row.restored=$false;$row.failure=(("$($row.failure); restore: "+$_.Exception.Message)).TrimStart('; ')}
  $rows.Add([pscustomobject]$row);Save "$out/$key.json" ([pscustomobject]$row)
  if(!$row.restored){throw "Fixture restore failed after $key"}
 }}}
 $dirty=$false
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}finally{
 if($owned){
  if($dirty){try{Restore 'final'}catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}else{Disarm}
  $errors=[Collections.Generic.List[string]]::new()
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}}catch{$errors.Add($_.Exception.Message)}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh-or(Q $r '--health' 1).current-ne56){$errors.Add("$r gameplay/transport/health not restored")}}catch{$errors.Add("$r status unavailable")}}
  if(Test-Path -LiteralPath $out){
   Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)}
   $bad=@($rows|Where-Object {!$_.passed});if(!$failure-and$bad.Count){$failure="$($bad.Count) case(s) failed: "+(($bad|ForEach-Object {"$($_.approach)-$($_.source)-$($_.kind): $($_.failure)"})-join ' | ')}
   if($errors.Count-and!$failure){$failure=$errors-join '; '}
   Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;cases=@($rows);scope='Takedown variants (side, front, drop, slide; choke and lethal; both source roles) on a host-owned NPC under the product controller: native scene, partner phases, shared outcome and victim position; fixture restored after every case.'})
  }
  [ordered]@{pid=$PID;operation='TakedownVariants1254';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_TAKEDOWN_VARIANTS $($rows.Count) cases"
