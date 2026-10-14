param([string]$Take='window-mantle-probe-v1201',[ValidateSet('host','client')][string]$Role='host',[string]$Window='ld_rich_small_01_sndctr_4')
# Stage 3 window_enter_exit feasibility (native only, no co-op presentation):
# open a real Dunwall window natively (aim + X), then hold jump+forward toward
# it and record whether the native mantle (player state 8) enters the window
# frame, where the player ends up, and whether a reverse mantle exits back in.
# Dunwall fixture restored (PrepareShadowSword) whatever happens.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$capture=$null;$rows=[Collections.Generic.List[object]]::new();$notes=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='Neutral',[int]$ms=100,[int]$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms -LeftY $y|Out-Null}
function Watch($tag,$ms){$w=[Diagnostics.Stopwatch]::StartNew();$seen=$false
 while($w.ElapsedMilliseconds-lt$ms){$st=Q $Role '--player-states';$pl=Q $Role '--player';$b=S $Role
  $mantle=@($st.active|Where-Object id -eq 8).Count-gt0;if($mantle){$seen=$true}
  $rows.Add([pscustomobject]@{tag=$tag;ms=$w.ElapsedMilliseconds;mantle=$mantle;active=@($st.active|ForEach-Object id);stance=$st.stance;traversalSceneValid=$st.traversalSceneValid;traversal=$st.traversalScene;position=$pl.candidatePosition;logic=$b.localGameLogicState})
  Start-Sleep -Milliseconds 30}
 return $seen}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;window=$Window;role=$Role}
 $e=@((Q $Role '--entities').entities|Where-Object name -eq $Window);if($e.Count-ne1){throw "Window $Window not found"};Save "$out/window-entity.json" $e[0]
 $wp=$e[0].position
 $dirty=$true
 foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_movement_bank off','coop_shadow_stream off'){C $Role $cmd}
 # Native open: stand 1.3 m in front, aim at the handle, press X.
 C $Role ("setviewpos {0:F3} {1:F3} 67.0 0 0"-f($wp[0]-1.28),$wp[1]);Start-Sleep -Milliseconds 1500
 & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Role -X $wp[0] -Y $wp[1] -Z 68.0 -Evidence "$out/aim.json"
 $before=Q $Role '--usable' $e[0].handle;Save "$out/usable-before.json" $before
 $opened=$false;for($t=0;$t-lt3-and!$opened;$t++){I $Role X 130;I $Role;Start-Sleep -Milliseconds 1800;$u=Q $Role '--usable' $e[0].handle;$opened=($u.current-ne$before.current-or$u.pending-ne-1)}
 Save "$out/usable-after-open.json" $u;$notes.Add(@{opened=$opened;before=$before.current;after=$u.current})
 if(!$opened){throw 'Window did not open natively'}
 # Approach: face the window (yaw 0 = +x), level camera, record the source window.
 C $Role ("setviewpos {0:F3} {1:F3} 68.1 0 0"-f($wp[0]-1.6),$wp[1]);Start-Sleep -Milliseconds 900
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Quick|Out-Null;Start-Sleep -Milliseconds 400
 $hwnd=(Get-Process -Id $pids[$Role]).MainWindowHandle.ToInt64()
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','30','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/source-$Role.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -PassThru;Start-Sleep -Milliseconds 700
 $start=(Q $Role '--player').candidatePosition;Save "$out/player-start.json" (Q $Role '--player')
 # Enter: hold jump + forward.
 I $Role A 1300 22000;$enter=Watch 'enter' 3500
 $mid=(Q $Role '--player').candidatePosition;Save "$out/player-mid.json" (Q $Role '--player');Save "$out/states-mid.json" (Q $Role '--player-states')
 $notes.Add(@{enterMantle=$enter;start=$start;afterEnter=$mid;logic=(S $Role).localGameLogicState})
 Start-Sleep -Milliseconds 1500
 # Exit: turn around (yaw 180) and hold jump + forward back into the room.
 if((S $Role).localGameLogicState-eq1){
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Quick|Out-Null
  $p=(Q $Role '--player').nativeViewPosition;C $Role ("setviewpos {0:F3} {1:F3} {2:F3} 180 0"-f$p[0],$p[1],$p[2]);Start-Sleep -Milliseconds 700
  I $Role A 1300 22000;$exit=Watch 'exit' 3500
  $end=(Q $Role '--player').candidatePosition;Save "$out/player-end.json" (Q $Role '--player')
  $notes.Add(@{exitMantle=$exit;afterExit=$end;logic=(S $Role).localGameLogicState})
 }
 if(!$capture.WaitForExit(60000)){throw 'Recording did not finish'}
 Save "$out/samples.json" @($rows);Save "$out/notes.json" @($notes)
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Save "$out/samples.json" @($rows);Save "$out/notes.json" @($notes)}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($capture){try{if(!$capture.HasExited-and!$capture.WaitForExit(40000)){Stop-Process -Id $capture.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r;$st=S $r;if($st.localGameLogicState-in13,8,15){I $r A 300;Start-Sleep -Seconds 4}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;notes=@($notes);scope='Native window mantle feasibility probe on a real Dunwall window; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
