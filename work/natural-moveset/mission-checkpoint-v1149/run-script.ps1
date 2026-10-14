param([string]$Take='mission-checkpoint-v1149',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Stage 6 on a mission (candidate 1147): both peers arm checkpoint sync in
# mission mode (+ usable session). The host dies by a native fall; the client
# must enter its native GameOver too (shared death). The host chooses the
# native "Restart From Last Checkpoint" and the client presses it as a
# duplicate; both peers restart exactly once from their own mission autosave
# behind the both-ready barrier. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function W($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing object field $offset"};[Convert]::ToUInt64($w[0].word.Substring(2),16)}
function Bind($r,$name){$door=@((Q $r '--entities').entities|Where-Object name -eq $name);if($door.Count-ne1){return $null}
 $cp=@((Q $r '--components' $door[0].handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@');if($cp.Count-ne1-or$cp[0].owner-ne$door[0].pointer){return $null}
 $o=Q $r '--object' $cp[0].pointer;@{name=$name;position=$door[0].position;component=$cp[0].pointer;owner=[Convert]::ToUInt64($door[0].pointer.Substring(2),16);generation=((W $o '0x18')-band [uint64]4294967295)}}
function NativeDoor($r,$b){$o=Q $r '--object' $b.component;if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$b.owner-or((W $o '0x18')-band [uint64]4294967295)-ne$b.generation){throw 'Native door lifetime changed'};$s=W $o '0x48';[pscustomobject]@{current=[uint32]($s-band [uint64]4294967295);pending=[uint32]($s-shr32);side=[int]((W $o '0xc8')-band255)}}
function Entry($r,$name){@((S $r '.door-sync.json').entries|Where-Object name -eq $name)[0]}
function Capture($name,$seconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$name-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$name-$r-capture.log" -PassThru))}}
function UseSweep($r,$b,$want,[int[]]$Offsets=@(0,20,-20,40,-40)){
 # The door origin is the hinge: sweep the bearing until native use changes it.
 $v=Q $r '--player';$here=$v.candidatePosition
 foreach($off in $Offsets){
  if($off){C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$here[0],$here[1],($here[2]+1.7),($script:baseYaw+$off));Start-Sleep -Milliseconds 500}
  I $r X 200;$t=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 80;$n=NativeDoor $r $b}while($n.current-ne$want-and$n.pending-ne$want-and$t.ElapsedMilliseconds-lt1200)
  if($n.current-eq$want-or$n.pending-eq$want){return $off}}
 return $null}
function WaitCaptures(){foreach($c in @($captures)){if(!$c.WaitForExit(200000)-or$c.ExitCode){throw 'Recording failed'}};$captures.Clear()}
function Approach($r,$door,$from){
 $p=$door.position;$dx=$from[0]-$p[0];$dy=$from[1]-$p[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001);$ux=$dx/$n;$uy=$dy/$n
 $script:standX=$p[0]+1.4*$ux;$script:standY=$p[1]+1.4*$uy;$script:standZ=$p[2]+1.7;$script:baseYaw=[Math]::Atan2(-$uy,-$ux)*180/[Math]::PI
 C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$standX,$standY,$standZ,$baseYaw);Start-Sleep -Milliseconds 1300
 $v=Q $r '--player';$eye=$v.nativeViewPosition;$hd=[Math]::Sqrt(($p[0]-$eye[0])*($p[0]-$eye[0])+($p[1]-$eye[1])*($p[1]-$eye[1]))
 $pitch=[Math]::Max(-45,[Math]::Min(45,[Math]::Atan2($eye[2]-($p[2]+1.0),$hd)*180/[Math]::PI))
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $r -Pitch $pitch -Quick|Out-Null;return @{eye=$eye;pitch=$pitch}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1147b/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1147b candidate'}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;mapInfo=$MapInfo}
 $dirty=$true
 C host "campaign $MapInfo $Chaos corvo";C client "campaign $MapInfo $Chaos emily"
 $w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{
  Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt240000)
 Save "$out/load-log.json" @($log)
 if(!$ready){throw 'Both instances did not reach mission gameplay'}
 Start-Sleep -Seconds 2
 if(!(S host).remoteFresh-or!(S client).remoteFresh){throw 'Transport not fresh on mission map'}
 function RS($r){S $r '.reset.json'}
 function Y($r){S $r '.usable-sync.json'}
 function T($r){S $r '.checkpoint.json'}
 $timeline=[Collections.Generic.List[object]]::new()
 function Snap($tag){$row=@{tag=$tag;ms=$clock.ElapsedMilliseconds};foreach($r in 'host','client'){$b=S $r;$x=RS $r;$row[$r]=@{logic=$b.localGameLogicState;pending=$b.localPendingGameLogicState;epoch=$b.localEpoch;phase=$x.phase;result=$x.result;loads=$x.loads;completed=$x.completed;aborted=$x.aborted;deathActive=$x.deathActive;deathReady=$x.deathReady;peerDeathReady=$x.peerDeathReady;mission=$x.mission;slot=$x.missionSlot;gen=$x.missionGeneration;details=$x.missionDetails;digest=$x.digest}};$timeline.Add($row);$row}
 $clock=[Diagnostics.Stopwatch]::StartNew()
 $start=@{};foreach($r in 'host','client'){Save "$out/player-start-$r.json" (Q $r '--player');$start[$r]=(Q $r '--player').candidatePosition}
 $traceBefore=@{host=(T host).callbacks;client=(T client).callbacks}
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off'){C $r $cmd}}
 foreach($r in 'host','client'){C $r 'coop_checkpoint_sync on';C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'});C $r 'coop_usable_sync session'}
 $w=[Diagnostics.Stopwatch]::StartNew()
 # 1147b refuses an arm until that peer's mission-start autosave exists;
 # re-arm a refused peer (recorded) for up to 60 s.
 $rearms=[Collections.Generic.List[object]]::new()
 do{Start-Sleep -Milliseconds 400;$s=Snap 'arming'
  foreach($r in 'host','client'){if(!$s[$r].mission-or$s[$r].result-in'enable_refused','map_scope_unavailable','disabled'){$rearms.Add(@{ms=$w.ElapsedMilliseconds;role=$r;result=$s[$r].result});C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_sync on'}}
  $ok=$s.host.result-eq'armed'-and$s.client.result-eq'armed'-and$s.host.mission-and$s.client.mission}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 Save "$out/arm-retries.json" @($rearms)
 foreach($r in 'host','client'){Save "$out/armed-reset-$r.json" (RS $r);Save "$out/armed-usable-$r.json" (Y $r)}
 if(!$ok){throw "Mission checkpoint arm failed host=$($s.host.result) client=$($s.client.result)"}
 if($s.host.digest-ne$s.client.digest){throw 'Mission checkpoint identities differ'}
 Capture 'checkpoint' 120;Start-Sleep -Milliseconds 800
 C host ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$start.host[0],$start.host[1],($start.host[2]+45))
 $w.Restart()
 do{Start-Sleep -Milliseconds 200;$s=Snap 'death';$ok=$s.host.logic-eq13-and$s.client.logic-eq13-and$s.host.deathReady-and$s.host.peerDeathReady-and$s.client.deathReady-and$s.client.peerDeathReady
  foreach($r in 'host','client'){if($s[$r].phase-eq7){throw "Death failed $r $($s[$r].result)"}}}while(!$ok-and$w.ElapsedMilliseconds-lt25000)
 if(!$ok){throw 'Shared mission death screens not ready on both peers'}
 Start-Sleep -Seconds 3
 I host A 200;Start-Sleep -Milliseconds 300;I client A 200
 $w.Restart();$lastA=0
 do{Start-Sleep -Milliseconds 250;$s=Snap 'restart'
  foreach($r in 'host','client'){if($s[$r].phase-eq7){throw "Restart failed $r $($s[$r].result)"}
   if($s[$r].logic-in8,15-and$w.ElapsedMilliseconds-$lastA-gt2500-and$w.ElapsedMilliseconds-gt6000){I $r A 300;$lastA=$w.ElapsedMilliseconds}}
  $ok=$s.host.completed-ge1-and$s.client.completed-ge1-and$s.host.logic-eq1-and$s.client.logic-eq1-and!$s.host.pending-and!$s.client.pending-and$s.host.phase-eq0-and$s.client.phase-eq0
 }while(!$ok-and$w.ElapsedMilliseconds-lt120000)
 Start-Sleep -Seconds 3;$final=Snap 'final'
 foreach($r in 'host','client'){Save "$out/final-reset-$r.json" (RS $r);Save "$out/final-usable-$r.json" (Y $r);Save "$out/player-final-$r.json" (Q $r '--player');Save "$out/character-$r.json" (Q $r '--character')}
 Save "$out/trace-after.json" @{host=(T host);client=(T client);before=$traceBefore}
 Save "$out/timeline.json" @($timeline)
 WaitCaptures
 if(!$ok){throw 'Shared mission restart did not complete on both peers'}
 $checks=@{}
 foreach($r in 'host','client'){$x=RS $r;$ch=Q $r '--character';$p=(Q $r '--player').candidatePosition
  $d=[Math]::Sqrt([Math]::Pow($p[0]-$start[$r][0],2)+[Math]::Pow($p[1]-$start[$r][1],2)+[Math]::Pow($p[2]-$start[$r][2],2))
  $checks[$r]=@{loads=$x.loads;completed=$x.completed;aborted=$x.aborted;character=$ch.character;spawnDistance=$d;usableSession=(Y $r).session.result;usableRearms=(Y $r).session.rearms}
  if($x.loads-ne1-or$x.completed-ne1-or$x.aborted-or$ch.character-ne$(if($r-eq'host'){1}else{2})){throw "Unexpected restart counts/role on $r"}}
 $rows.Add(@{checks=$checks;armDigest=$s.host.digest;startEpochs=@{host=$timeline[0].host.epoch;client=$timeline[0].client.epoch};finalEpochs=@{host=$final.host.epoch;client=$final.client.epoch}})
 Save "$out/verified.json" @{passed=$true;scope='Shared mission death and checkpoint restart.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_usable_sync off';C $r 'coop_checkpoint_sync off'}catch{}}
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Mission checkpoint mode: shared native death and one shared restart from each peer own mission autosave; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
