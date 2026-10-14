param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-heart-v563')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$monitor=$null
function C($r,$text){& "$lab/Send-D2Command.ps1" -Role $r -Command $text -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$button='Neutral',$ms=100,$lt=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $lt|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$handle=0){for($a=0;$a-lt8;$a++){$raw=if($handle){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $handle}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode failed"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='HeartObserve563';take=$Take;state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_heart_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  Q $r '--player-inventory'|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $before=Q $Source '--player-heart';$before|ConvertTo-Json|Set-Content "$out/heart-before.json";if($before.quantity-or$before.equipped){throw 'Heart already owned'}
 C $Source 'setviewpos -33.5 -153 68.1 0 0'
 $hwnd=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No game window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','21','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/source.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 500;if($capture.HasExited){throw 'Capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$Source],'--watch-midnight','19000') -WindowStyle Hidden -RedirectStandardOutput "$out/scenes.ndjson" -RedirectStandardError "$out/scenes.stderr" -PassThru
 C $Source 'coop_heart_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew();$step=0
 while($w.ElapsedMilliseconds-lt17000){
  if($step-eq0-and$w.ElapsedMilliseconds-ge700){I $Source X 1500;$step++}
  if($step-eq1-and$w.ElapsedMilliseconds-ge2900){I $Source X 650;$step++}
  if($step-eq2-and$w.ElapsedMilliseconds-ge4700){I $Source Neutral 250 255;$step++}
  if($step-eq3-and$w.ElapsedMilliseconds-ge7500){I $Source B 180;$step++}
  if($step-eq4-and$w.ElapsedMilliseconds-ge8700){I $Source Neutral 250 255;$step++}
  if($step-eq5-and$w.ElapsedMilliseconds-ge11200){I $Source B 180;$step++}
  if($step-eq6-and$w.ElapsedMilliseconds-ge12500){I $Source X 1500;$step++}
  if($step-eq7-and$w.ElapsedMilliseconds-ge15000){C $Source 'coop_heart_fixture off';$step++}
  $item=$null;try{$item=Q $Source '--player-heart'}catch{}
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;item=$item;player=(Q $Source '--player-states');fixture=(S $Source '.heart-fixture.json')})
  Start-Sleep -Milliseconds 25
 }
 if(!@($rows|Where-Object {$_.item.equipped-and$_.item.quantity-eq1-and$_.fixture.result-eq'native_heart_ready'}).Count){throw 'Native Heart never ready'}
 $checks.Add('Exact native Heart acquisition and selection observed')
 $monitor.WaitForExit(5000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Scene monitor incomplete'}
 $scenes=@(Get-Content "$out/scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|ForEach-Object {$_.instances}|Where-Object {$_.state-eq3-and$_.asset-match'heart'})
 $scenes|ConvertTo-Json -Depth 8|Set-Content "$out/heart-scenes.json"
 if(!$scenes.Count){throw 'No Heart scenes recorded'};$checks.Add('Native Heart scenes captured at15ms cadence')
 $after=Q $Source '--player-heart';$after|ConvertTo-Json|Set-Content "$out/heart-after.json"
 if($after.quantity-or$after.equipped-or$after.started){throw 'Heart lease rollback incomplete'};$checks.Add('Quantity and selected slot rolled back')
}catch{$failure=$_.Exception.Message}finally{
 if($owned){
  foreach($r in $pids.Keys){try{I $r;C $r 'coop_heart_fixture off';C $r 'coop_movement_fixture off'}catch{}}
  foreach($proc in @($capture,$monitor)){if($proc-and!$proc.HasExited){$proc.WaitForExit(22000)|Out-Null}}
  if($pids.Count-eq2){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--player-heart').quantity){throw 'Checkpoint state differs'}};foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Both isolated checkpoints and14 protected save/settings hashes restored')}catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  [ordered]@{pid=$PID;operation='HeartObserve563';take=$Take;state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json";$mutex.ReleaseMutex()
 };$mutex.Dispose()
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Native Heart source observation; no remote body/prop binding yet.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_HEART $Source $($checks.Count) checks"
