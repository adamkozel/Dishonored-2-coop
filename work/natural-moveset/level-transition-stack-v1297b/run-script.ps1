param([string]$Take='level-transition-stack-v1297a',[string]$Candidate='candidate-npc-combat-v1214bc')
# Stage 6 diagnostic (chapter 97): a real script LevelTransition on the lab's
# own fixture map. In dunwall_escape_tower_p the vault exit ring locker
# (ld_ring_locker_01_3) plays player_usering, then Start_Level_Transition
# fires the ld_kiscule LevelTransition to dunwall_escape_streets_p (confirmed
# static). The locker needs the Imperial Ring (ld_imperial_ring_pickup_corvo).
# The host picks the ring up, uses the locker, and 1214bc records the call
# stack at the native loading-entry switch (.loading.json entryStacks). The
# client stays; its behaviour is recorded. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$notes=[ordered]@{};$capture=$null
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Snap($r){$s=S $r;[ordered]@{logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMap;epoch=$s.localEpoch;cine=$s.localCinematicEpoch;pos=$s.localPosition;fresh=$s.remoteFresh}}
function Face($x,$y,$z,$look){
 $yaw=[Math]::Atan2($look[1]-$y,$look[0]-$x)*180/[Math]::PI
 C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,$z,$yaw);Start-Sleep -Milliseconds 900
 $v=Q host '--player';$eye=$v.nativeViewPosition;$hd=[Math]::Sqrt(($look[0]-$eye[0])*($look[0]-$eye[0])+($look[1]-$eye[1])*($look[1]-$eye[1]))
 $pitch=[Math]::Max(-60,[Math]::Min(60,[Math]::Atan2($eye[2]-$look[2],[Math]::Max($hd,.2))*180/[Math]::PI))
 $settled=$true;try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $pitch -Quick|Out-Null}catch{$settled=$false}
 @{yaw=$yaw;pitch=$pitch;eye=$eye;pitchSettled=$settled}}
function UseFrom($label,$target,$offsets,$z){
 foreach($o in $offsets){
  $f=Face ($target[0]+$o[0]) ($target[1]+$o[1]) $z $target;$before=Snap host;I host X 250
  $w=[Diagnostics.Stopwatch]::StartNew();$hit=$false
  do{Start-Sleep -Milliseconds 150;$now=Snap host;if($now.cine-ne$before.cine-or$now.logic-ne1-or$now.pending){$hit=$true}}while(!$hit-and$w.ElapsedMilliseconds-lt2500)
  $notes["$label-$($o[0])_$($o[1])"]=@{face=$f;hit=$hit;before=$before;after=$now}
  if($hit){return $true}}
 return $false}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 Save "$out/identity.json" @{dll=$pin.dll;candidate=$Candidate;map=(S host).localMap}
 $dirty=$true
 C host 'coop_player_health_hold on'
 $ents=(Q host '--entities').entities
 $want='ld_imperial_ring_pickup_corvo','ld_ring_locker_01_3','mdnt_midnight_loco_usering_3'
 $at=@{};foreach($n in $want){$e=@($ents|Where-Object name -eq $n);if($e.Count){$at[$n]=$e[0].position}};Save "$out/entities.json" $at
 if(!$at['ld_ring_locker_01_3']){throw 'Ring locker not present'}
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $cargs=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','120','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/host.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $cargs -WindowStyle Hidden -RedirectStandardError "$out/host-capture.log" -PassThru
 # 1. The ring (skipped when the fixture player already took it).
 if($at['ld_imperial_ring_pickup_corvo']){$r0=$at['ld_imperial_ring_pickup_corvo']
  $notes.ring=UseFrom 'ring' $r0 @(@(0.9,0),@(-0.9,0),@(0,0.9),@(0,-0.9)) ($r0[2]+1.6)
  $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 300;$s=Snap host;if($s.logic-eq5-and$w.ElapsedMilliseconds-gt4000){I host X 3000}}while(($s.logic-ne1-or$s.pending)-and$w.ElapsedMilliseconds-lt30000)}
 # 2. The vault exit locker; the scene's player target gives the floor height.
 $lk=$at['ld_ring_locker_01_3'];$floor=if($at['mdnt_midnight_loco_usering_3']){$at['mdnt_midnight_loco_usering_3'][2]}else{$lk[2]-1.5}
 $notes.locker=UseFrom 'locker' $lk @(@(0.9,0),@(-0.9,0),@(0,0.9),@(0,-0.9),@(0.7,0.7),@(-0.7,-0.7)) ($floor+1.65)
 if(!$notes.locker){throw 'Ring locker use not registered'}
 # 3. Watch the scene, fade and map change; keep the loading trace at entry.
 $timeline=[Collections.Generic.List[object]]::new();$w=[Diagnostics.Stopwatch]::StartNew();$saved=$false
 do{Start-Sleep -Milliseconds 300;$h=Snap host;$c=Snap client;$timeline.Add([ordered]@{ms=$w.ElapsedMilliseconds;host=$h;client=$c})
  if($h.logic-eq15-and!$saved){Copy-Item "$root/work/lab-instances/host/coop-status.json.loading.json" "$out/loading-host-entry.json";$saved=$true}
  if($h.logic-in 8,15-and$w.ElapsedMilliseconds%8000-lt300){I host A 300}
 }while(!($h.map-like'*escape_streets*'-and$h.logic-eq1-and!$h.pending)-and$w.ElapsedMilliseconds-lt90000)
 Start-Sleep 2;Copy-Item "$root/work/lab-instances/host/coop-status.json.loading.json" "$out/loading-host-after.json";Copy-Item "$root/work/lab-instances/client/coop-status.json.loading.json" "$out/loading-client-after.json"
 Save "$out/timeline.json" @($timeline)
 $notes.hostFinal=Snap host;$notes.clientFinal=Snap client
 $stacks=(Get-Content "$out/loading-host-after.json" -Raw|ConvertFrom-Json).entryStacks;$notes.entryStacks=$stacks
 if(!$stacks){throw 'No loading-entry stack recorded'}
 if($notes.hostFinal.map-notlike'*escape_streets*'){throw "Host did not reach the streets (map $($notes.hostFinal.map))"}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($capture){try{if(!$capture.HasExited-and!$capture.WaitForExit(130000)){Stop-Process -Id $capture.Id}}catch{}}
  if($dirty){
   try{I host;C host 'coop_player_health_hold off'}catch{}
   $w=[Diagnostics.Stopwatch]::StartNew()
   do{$ok=$true;foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-notin 1,2,6,13-or$s.localPendingGameLogicState){$ok=$false;if($s.localGameLogicState-eq5){I $r X 3000}else{I $r A 300}}}catch{$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 1500}}while(!$ok-and$w.ElapsedMilliseconds-lt120000)
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;notes=$notes;scope='Dunwall tower vault exit LevelTransition (host): native loading-entry call stack recorded; client behaviour observed; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
