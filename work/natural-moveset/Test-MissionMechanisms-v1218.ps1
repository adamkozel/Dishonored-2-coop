param([string]$Take='mission-mechanisms-v1218',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo')
# 1218 (candidate 1213c): real Edge of the World mechanisms - the bench vise
# (env_vise_01_1_3, side 0 as native 1045, expected variant 3 crank) used by
# each role, observed on the partner; then a non-fatal attempt on the wind
# turbine lever (variant 2). Earlier header (1214):
# Stage 3 lever_button_crank_wheel / key_use (candidate 1213): a native use of
# the Dunwall window handle (player state 75) must start the authored generic
# interaction (variant 1, clip 369, 2.3 s) on the OTHER peer's remote body
# through the product controller's stream/bank. Both roles: host uses while the
# client observer camera films remote Corvo, then the client uses while the
# host observer camera films remote Emily. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new();$samplers=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='Neutral',[int]$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function ProductStatus($r){S $r '.product.json'}
function Live($r){$s=S $r;$m=S $r '.movement.json';$x=S $r '.shadow-stream.json';$a=$s.probeRenderPosition;$b=$s.remotePosition
 $d=if($a-and$b){[Math]::Sqrt(($a[0]-$b[0])*($a[0]-$b[0])+($a[1]-$b[1])*($a[1]-$b[1])+($a[2]-$b[2])*($a[2]-$b[2]))}else{999}
 @{follow=$s.probeFollowEnabled;distance=[Math]::Round($d,3);bank=[bool]$m.active;bankVersion=$m.version;stream=[bool]($x.armed-and$x.ready);ok=($s.probeActorHandle-and$s.probeFollowEnabled-and$d-lt1.0-and$m.active-and$x.armed-and$x.ready-and$s.remoteFresh)}}
function WaitLive($tag,$limit=30000){$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 300;$p=@{host=(Live host);client=(Live client)};$ok=$p.host.ok-and$p.client.ok}while(!$ok-and$w.ElapsedMilliseconds-lt$limit);$p.ok=$ok;$p.waitMs=$w.ElapsedMilliseconds;Save "$out/live-$tag.json" $p;return $p}
function UseCase($src,$obs,$name,$side,$want,[switch]$Optional){
 $e=@((Q $src '--entities').entities|Where-Object name -eq $name);if($e.Count-ne1){if($Optional){$rows.Add(@{source=$src;target=$name;skipped='not found'});return};throw "$name not found"};$p=$e[0].position
 $a=$side*[Math]::PI/180;$x=$p[0]+1.0*[Math]::Cos($a);$y=$p[1]+1.0*[Math]::Sin($a);$yaw=($side+180)%360
 C $src ("setviewpos {0:F3} {1:F3} {2:F3} {3} 0"-f$x,$y,($p[2]+0.7),$yaw);Start-Sleep -Milliseconds 1200
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $src -Pitch 25 -Quick|Out-Null;Start-Sleep -Milliseconds 300
 $live=WaitLive "before-$src-$name" 25000;if(!$live.ok){if($Optional){$rows.Add(@{source=$src;target=$name;skipped='presentation not live'});return};throw "Partner presentation not live before the $src use of $name"}
 $sp=(Q $src '--player').candidatePosition;$oa=($side+70)*[Math]::PI/180;$ox=$sp[0]+3.0*[Math]::Cos($oa);$oy=$sp[1]+3.0*[Math]::Sin($oa);$oyaw=[Math]::Atan2($sp[1]-$oy,$sp[0]-$ox)*180/[Math]::PI
 C $obs ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$ox,$oy,($sp[2]+1.75),$oyaw);Start-Sleep -Milliseconds 700
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $obs -Pitch 8 -Quick|Out-Null;Start-Sleep -Milliseconds 300
 C $obs 'coop_observer_camera hold';Start-Sleep -Milliseconds 300;$hold=S $obs '.observer-camera.json'
 if(!$hold.active){throw "Observer camera hold failed on ${obs}: $($hold.result)"}
 C $obs ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f($ox+0.8*[Math]::Cos($oa)),($oy+0.8*[Math]::Sin($oa)),($sp[2]+1.75),$oyaw);Start-Sleep -Milliseconds 1200
 $live=WaitLive "camera-$src-$name" 20000;if(!$live.ok){C $obs 'coop_observer_camera off';if($Optional){$rows.Add(@{source=$src;target=$name;skipped='presentation not live with camera'});return};throw "Partner presentation not live with the $obs camera held"}
 $before=(S $src '.movement.json').sourceUses
 $hwnd=(Get-Process -Id $pids[$obs]).MainWindowHandle.ToInt64()
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','11','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/use-$src-$name-observed.mp4")
 $cap=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/use-$src-$name-capture.log" -PassThru;$captures.Add($cap)
 $pwsh=(Get-Process -Id $PID).Path
 $sampler=Start-Process $pwsh -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Sample-StatusFile-v1023.ps1",'-Path',"$root/work/lab-instances/$obs/coop-status.json.movement.json",'-Out',"$out/use-$src-$name-observer-movement.ndjson",'-DurationMs',10000) -WindowStyle Hidden -PassThru;$samplers.Add($sampler)
 Start-Sleep -Milliseconds 1000
 $used=$false;for($k=0;$k-lt3-and!$used;$k++){I $src X 600;I $src;Start-Sleep -Milliseconds 1500;$used=(S $src '.movement.json').sourceUses-gt$before}
 if(!$sampler.WaitForExit(15000)){Stop-Process -Id $sampler.Id};if(!$cap.WaitForExit(30000)){Stop-Process -Id $cap.Id}
 C $obs 'coop_observer_camera off'
 $sm=S $src '.movement.json'
 $samples=@(Get-Content "$out/use-$src-$name-observer-movement.ndjson"|ForEach-Object {($_|ConvertFrom-Json).status})
 $shown=@($samples|Where-Object {$_.useVariant-eq$want-and$_.useWeight-gt.5})
 $row=@{source=$src;observer=$obs;target=$name;side=$side;expected=$want;used=$used;sourceUses=$sm.sourceUses;sourceUseName=$sm.sourceUseName;sourceLastVariant=$sm.sourceUseLastVariant;endReason=$sm.sourceUseEndReason;endAction=$sm.sourceUseEndAction;
  observerSamples=$samples.Count;shownSamples=$shown.Count;observedVariants=@($samples|ForEach-Object {$_.useVariant}|Sort-Object -Unique);maxWeight=(($samples|ForEach-Object {[double]$_.useWeight}|Measure-Object -Maximum).Maximum);maxAmount=(($samples|ForEach-Object {[double]$_.useAmount}|Measure-Object -Maximum).Maximum)}
 $rows.Add($row);Save "$out/use-$src-$name.json" $row
 if($Optional){return}
 if(!$used){throw "Source did not register a native use of $name ($src)"}
 if($sm.sourceUseLastVariant-ne$want){throw "Source classified $name as variant $($sm.sourceUseLastVariant), expected $want"}
 if(!$shown.Count){throw "Observer remote body did not present variant $want ($src -> $obs)"}
}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMapId})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);return @{ready=$ready;log=@($log)}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1213c/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1213c candidate'}
 Save "$out/identity.json" @{dll=$pin.dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash}
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 $dirty=$true
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 $g=WaitGameplay 300000;Save "$out/load-log.json" $g.log;if(!$g.ready){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'});C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(ProductStatus host).result-eq'armed'-and(ProductStatus client).result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 if(!$ok){throw 'Product controller not armed'}
 UseCase host client 'env_vise_01_1_3' 0 3
 UseCase client host 'env_vise_01_1_3' 0 3
 foreach($side in 0,90,180,270){UseCase host client 'script_lever_windturbine_1_1_1' $side 2 -Optional;if(@($rows|Where-Object {$_.target-eq'script_lever_windturbine_1_1_1'-and$_.used}).Count){break}}
 Save "$out/verified.json" @{passed=$true;scope='Vise use presented as the crank interaction on the partner body, both roles.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($s in $samplers){try{if(!$s.HasExited){Stop-Process -Id $s.Id}}catch{}}
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_observer_camera off';C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Native use of real Edge of the World mechanisms (vise both roles, optional turbine lever) -> authored interaction on the partner body; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
