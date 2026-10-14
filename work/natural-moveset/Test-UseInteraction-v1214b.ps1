param([string]$Take='use-interaction-v1214b',[string]$Window='ld_rich_small_01_sndctr_4')
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
function UseCase($src,$obs){
 $e=@((Q $src '--entities').entities|Where-Object name -eq $Window);if($e.Count-ne1){throw "Window $Window not found"};$wp=$e[0].position
 # Source stands at the window and aims at the handle.
 C $src ("setviewpos {0:F3} {1:F3} 67.0 0 0"-f($wp[0]-1.28),$wp[1]);Start-Sleep -Milliseconds 1500
 & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $src -X $wp[0] -Y $wp[1] -Z $(if((Q $src '--usable' $e[0].handle).current-eq1){68.65}else{68.0}) -Evidence "$out/aim-$src.json"
 $live=WaitLive "before-$src";if(!$live.ok){throw "Partner presentation not live before the $src use"}
 # Observer camera 3.2 m behind-left of the source, looking at it; observer body re-parked.
 $sp=(Q $src '--player').candidatePosition;$ox=$sp[0]-2.6;$oy=$sp[1]+1.9;$oyaw=[Math]::Atan2($sp[1]-$oy,$sp[0]-$ox)*180/[Math]::PI
 C $obs ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$ox,$oy,($sp[2]+1.75),$oyaw);Start-Sleep -Milliseconds 700
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $obs -Pitch 6 -Quick|Out-Null;Start-Sleep -Milliseconds 300
 C $obs 'coop_observer_camera hold';Start-Sleep -Milliseconds 300;$hold=S $obs '.observer-camera.json'
 if(!$hold.active){throw "Observer camera hold failed on ${obs}: $($hold.result)"}
 C $obs ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f($sp[0]-3.6),($sp[1]+0.2),($sp[2]+1.75),0);Start-Sleep -Milliseconds 1200
 $live=WaitLive "camera-$src" 20000;if(!$live.ok){throw "Partner presentation not live with the $obs camera held"}
 $before=@{src=(S $src '.movement.json').sourceUses;obsVariant=(S $obs '.movement.json').useVariant}
 $hwnd=(Get-Process -Id $pids[$obs]).MainWindowHandle.ToInt64()
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','9','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/use-$src-observed.mp4")
 $cap=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/use-$src-capture.log" -PassThru;$captures.Add($cap)
 $pwsh=(Get-Process -Id $PID).Path
 $sampler=Start-Process $pwsh -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Sample-StatusFile-v1023.ps1",'-Path',"$root/work/lab-instances/$obs/coop-status.json.movement.json",'-Out',"$out/use-$src-observer-movement.ndjson",'-DurationMs',8000) -WindowStyle Hidden -PassThru;$samplers.Add($sampler)
 Start-Sleep -Milliseconds 1200
 $usable=Q $src '--usable' $e[0].handle;$changed=$false
 for($k=0;$k-lt3-and!$changed;$k++){I $src X 130;I $src;Start-Sleep -Milliseconds 1200;$u=Q $src '--usable' $e[0].handle;$changed=($u.current-ne$usable.current-or$u.pending-ne-1)}
 if(!$sampler.WaitForExit(15000)){Stop-Process -Id $sampler.Id};if(!$cap.WaitForExit(30000)){Stop-Process -Id $cap.Id}
 C $obs 'coop_observer_camera off'
 $after=@{src=(S $src '.movement.json')}
 $samples=@(Get-Content "$out/use-$src-observer-movement.ndjson"|ForEach-Object {($_|ConvertFrom-Json).status})
 $shown=@($samples|Where-Object {$_.useVariant-eq1-and$_.useWeight-gt.5})
 $row=@{source=$src;observer=$obs;usableChanged=$changed;sourceUsesBefore=$before.src;sourceUsesAfter=$after.src.sourceUses;sourceUseName=$after.src.sourceUseName;sourceUnfocused=$after.src.sourceUseUnfocused;
  observerSamples=$samples.Count;shownSamples=$shown.Count;maxWeight=(($samples|ForEach-Object {[double]$_.useWeight}|Measure-Object -Maximum).Maximum);maxAmount=(($samples|ForEach-Object {[double]$_.useAmount}|Measure-Object -Maximum).Maximum);bankVersion=($samples|Select-Object -Last 1).version}
 $rows.Add($row);Save "$out/use-$src.json" $row
 if(!$changed){throw "Native use did not change the window ($src)"}
 if($after.src.sourceUses-le$before.src){throw "Source did not register the native use state 75 ($src)"}
 if(!$shown.Count){throw "Observer remote body did not present the authored interaction ($src -> $obs)"}
}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1213/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1213 candidate'}
 Save "$out/identity.json" @{dll=$pin.dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash}
 $dirty=$true
 foreach($r in 'host','client'){C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(ProductStatus host).result-eq'armed'-and(ProductStatus client).result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 if(!$ok){throw 'Product controller not armed'}
 # 1214b: client first on the closed window (1214's client use of the open handle never registered).
 UseCase client host
 UseCase host client
 Save "$out/verified.json" @{passed=$true;scope='Native use (state 75) presented as the authored generic interaction on the partner body, both roles.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($s in $samplers){try{if(!$s.HasExited){Stop-Process -Id $s.Id}}catch{}}
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_observer_camera off';C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off','coop_checkpoint_sync off'){C $r $cmd}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Native use -> authored interaction presentation on the partner body (Dunwall window handle), both roles; fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
