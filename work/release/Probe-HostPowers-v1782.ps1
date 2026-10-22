param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='host-powers-v1782a',[string]$Candidate='candidate-npc-combat-v1214ia',[ValidateSet('corvo','emily')][string]$HostCharacter='emily',[ValidateSet('corvo','emily')][string]$PartnerCharacter='corvo',[int]$VideoSeconds=420)
# 1782 (#159, 1214ia): the host's powers presented to the partner in a new pairing, on the local two-instance lab (lab
# link). Both instances load Edge of the World: the host as -HostCharacter (default Emily), the partner as
# -PartnerCharacter (default Corvo); the product controller arms both (avatars by character). The host then casts
# each of its character's powers (granted with coop_grant_named, cast with the left trigger) and the partner's
# presenter status is sampled for 6 s after each cast: the stream must arrive (received grows) and the presentation
# must run (Emily: the Doppelganger clone created or visible, the Mesmerize spirit active, Far Reach beams, Domino
# marks presented; Corvo: Blink and Windblast effects, Swarm rats). Both windows are filmed and framed after each cast.
# Restores: product controller off, both instances relaunched on the Dunwall fixture (Deploy-SourceClock) and checked,
# protected saves checked.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$notes=[ordered]@{steps=@();casts=@()};$checks=[ordered]@{};$captures=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=200,[int]$lt=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms -LeftTrigger $lt|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};$null}
function Def($r){try{(& "$lab/bin/d2_runtime_probe.exe" $pids[$r] '--character'|ConvertFrom-Json).definition}catch{$null}}
function Frame($r,$name){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
 & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$name-$r.png" 2>$null|Out-Null}
function Step($tag){foreach($r in 'host','client'){Frame $r $tag};$row=[ordered]@{step=$tag}
 foreach($r in 'host','client'){$s=S $r;$c=S $r '.character.json';$m=S $r '.movement.json';$row[$r]=[ordered]@{logic=$s.localGameLogicState;map=$s.localMap;fresh=$s.remoteFresh;local=$c.local;remote=$c.remote;avatar=$c.avatar;bank=$m.active;joints=$m.joints}}
 $script:notes.steps+=$row}
function Settle($label,$sec){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 700;$ready=$true
  foreach($r in 'host','client'){$x=S $r;if($x.localGameLogicState-ne1-or$x.localPendingGameLogicState-or"$($x.localMap)"-notmatch'eotw'){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt3500){continue}
   switch($x.localGameLogicState){5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}{$_-in 8,15,9,11}{I $r A 300;$last[$r]=$w.ElapsedMilliseconds}12{I $r B 300;$last[$r]=$w.ElapsedMilliseconds}{$_-in 2,4}{I $r B 250;$last[$r]=$w.ElapsedMilliseconds}}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt($sec*1000));$script:notes["settle-$label"]=[ordered]@{ready=$ready;seconds=[math]::Round($w.Elapsed.TotalSeconds)};$ready}
# The host's powers and the partner sidecars that present them; `ok` judges the best sample after the cast.
$emily=@(
 [ordered]@{name='doppelganger';item='models/powers/doppelganger/doppelganger.inventoryitem';cast=1200;files=@('.doppelganger-world.json','.doppelganger-visual.json');ok={param($a,$b)[int]$a.received-gt0-and([int]$b.created-gt0-or[bool]$b.visible)}},
 [ordered]@{name='mesmerize';item='models/powers/mesmerize/mesmerize.inventoryitem';cast=1200;files=@('.mesmerize-world-stream.json','.mesmerize-visual.json');ok={param($a,$b)[int]$a.received-gt0-and([bool]$b.active-or[int]$b.created-gt0)}},
 [ordered]@{name='grapple';item='models/powers/grapple/grapple.inventoryitem';cast=900;files=@('.grapple-fx.json','.grapple-fx.json');ok={param($a,$b)[int]$a.received-gt0-and[int]$a.beams-gt0}},
 [ordered]@{name='domino';item='models/powers/phantasmallink/phantasmal_link.inventoryitem';cast=900;files=@('.domino-world.json','.domino-world.json');ok={param($a,$b)[int]$a.received-gt0}})
$corvo=@(
 [ordered]@{name='blink';item='models/powers/blink/blink.inventoryitem';cast=700;files=@('.power-fx-stream.json','.power-fx-stream.json');ok={param($a,$b)[int]$a.received-gt0}},
 [ordered]@{name='windblast';item='models/powers/windblast/windblast.inventoryitem';cast=900;files=@('.windblast-fx.json','.windblast-fx.json');ok={param($a,$b)[int]$a.received-gt0}})
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$pids[$r]=$l.pid;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw "Wrong isolated $r process"}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 $dirty=$true
 if($VideoSeconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $va=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$VideoSeconds,'-an','-c:v','libx264','-preset','veryfast','-crf','23','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+frag_keyframe+empty_moov+default_base_moof',"$out/take-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $va -WindowStyle Hidden -RedirectStandardError "$out/take-$r-capture.log" -PassThru))}}
 C host "campaign campaign/edge_of_the_world/eotw.mapinfo low $HostCharacter";C client "campaign campaign/edge_of_the_world/eotw.mapinfo low $PartnerCharacter"
 if(!(Settle 'mission' 420)){Step 'mission-timeout';throw 'Both games did not reach Edge of the World'}
 $notes.definitions=[ordered]@{host=(Def host);client=(Def client)}
 $checks.characters=("$($notes.definitions.host)"-match"/$HostCharacter/"-and"$($notes.definitions.client)"-match"/$PartnerCharacter/")
 foreach($r in 'host','client'){C $r 'coop_product_auto on';C $r 'coop_player_health_hold on'}
 $hj=if($PartnerCharacter-eq'corvo'){215}else{219};$cj=if($HostCharacter-eq'corvo'){215}else{219}
 $d=(Get-Date).AddSeconds(120);do{Start-Sleep 3;$hm=S host '.movement.json';$cm=S client '.movement.json'}while((Get-Date)-lt$d-and!($hm.active-and[int]$hm.joints-eq$hj-and$cm.active-and[int]$cm.joints-eq$cj))
 Step 'armed';$checks.armed=([bool]$hm.active-and[int]$hm.joints-eq$hj-and[bool]$cm.active-and[int]$cm.joints-eq$cj)
 if(!$checks.armed){throw 'The pairing did not arm (avatars and banks by character)'}
 $powers=if($HostCharacter-eq'emily'){$emily}else{$corvo}
 foreach($p in $powers){
  C host "coop_grant_named $($p.item)";Start-Sleep -Milliseconds 1500
  $b0=@{};foreach($f in $p.files){$b0[$f]=S client $f}
  I host Neutral $p.cast 255;$t=[Diagnostics.Stopwatch]::StartNew();$best=$null;$frame=$false
  while($t.ElapsedMilliseconds-lt6000){Start-Sleep -Milliseconds 300;$a=S client $p.files[0];$b=S client $p.files[1]
   if(& $p.ok $a $b){$best=[ordered]@{ms=$t.ElapsedMilliseconds;a=$a;b=$b};if(!$frame){Step "$($p.name)-presented";$frame=$true}}}
  if(!$frame){Step "$($p.name)-after"}
  $notes.casts+=[ordered]@{power=$p.name;presented=[bool]$best;at=$(if($best){$best.ms}else{$null});before=$b0;after=[ordered]@{a=(S client $p.files[0]);b=(S client $p.files[1])}}
  $checks["$($p.name)-presented"]=[bool]$best;Start-Sleep 4}
 $bad=@($checks.Keys|Where-Object{!$checks[$_]});if($bad.Count){throw "Checks failed: $($bad-join ', ')"}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Step 'failure'}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){foreach($r in 'host','client'){try{C $r 'coop_product_auto off';C $r 'coop_player_health_hold off'}catch{}}}
  $mutex.ReleaseMutex();$owned=$false
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;checks=$checks;notes=$notes;scope="The host's powers presented to the partner (#159, 1214ia) in the pairing host $HostCharacter, partner $PartnerCharacter, on the local lab: each power cast by the host must reach the partner and run its presentation."})
 }
 $mutex.Dispose()
}
Get-Content "$out/result.json"|Select-Object -First 30;if($failure){throw $failure}
