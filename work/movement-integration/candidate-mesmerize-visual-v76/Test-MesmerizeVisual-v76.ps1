param([ValidatePattern('^[a-z0-9-]+$')][string]$Take)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$before=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$dirty=$false
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 20}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Probe failed $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Isolated process identity changed'};$pids[$r]=$l.pid
  & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_mesmerize_visual off','coop_mesmerize_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  $before[$r]=Q $r '--player-inventory';$before[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean health required'}
 }
 C host 'setviewpos -30.7 -151.8 68.1 230 0';C client 'setviewpos -34.3 -154.8 68.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 15 -Evidence "$out/camera.json" -Quick
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No observer window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Capture failed'}
 $w=[Diagnostics.Stopwatch]::StartNew();$step=0
 while($w.ElapsedMilliseconds-lt14500){
  if($step-eq0-and$w.ElapsedMilliseconds-ge700){$dirty=$true;C host 'coop_mesmerize_visual in';$step=1}
  elseif($step-eq1-and$w.ElapsedMilliseconds-ge3100){C host 'coop_mesmerize_visual loop';$step=2}
  elseif($step-eq2-and$w.ElapsedMilliseconds-ge8000){C host 'coop_mesmerize_visual out';$step=3}
  $v=S host '.mesmerize-visual.json'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;visual=$v})
  if($v.handle-and!(Test-Path "$out/components.json")){Q host '--components' $v.handle|ConvertTo-Json -Depth 12|Set-Content "$out/components.json"}
  if($v.refused){throw "Native visual refused: $($v.result)"}
  Start-Sleep -Milliseconds 60
 }
 $v=S host '.mesmerize-visual.json';$v|ConvertTo-Json -Depth 5|Set-Content "$out/final.json"
 if($v.created-ne1-or$v.started[0]-ne1-or$v.started[1]-ne1-or$v.started[2]-ne1){throw 'Three animation phases not confirmed on one model'}
 $checks.Add('One exact cosmetic idEntity with four whitelisted native components plays all three animation-only scene derivatives')
 if($v.active-or$v.pendingHide-or$v.scene-or$v.hidden-ne1){throw 'Finite lease did not stop scene and hide model'}
 $checks.Add('Twelve-second lease stops the owned Midnight scene and hides the model without gameplay or delete events')
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($before[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory changed'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed'}
  $m=Q $r '--player-mesmerize';if($m.quantity-or$m.started-or$m.equipped){throw 'Gameplay Mesmerize equipment unexpectedly active'}
 }
 $checks.Add('Both players and avatars retain health and inventory; no native Mesmerize equipment granted')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'coop_mesmerize_visual off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(18000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 10|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";if(!$?){throw 'Checkpoint restoration failed'};foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$checks.Add('Both isolated checkpoints reload and discard the temporary cosmetic model')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;scope='Finite cosmetic Mesmerize model and native animation lifecycle. No networking, particles, sound or native gameplay outcomes in this proof. Visual acceptance is separate.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_MESMERIZE_VISUAL $($checks.Count) checks"
