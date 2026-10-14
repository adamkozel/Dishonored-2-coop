param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-grenade-actions-v54',[ValidateRange(-15,45)][int]$Pitch=0,[switch]$ValidateReader)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$peer=if($Source-eq'host'){'client'}else{'host'};$pids=@{};$beforeInv=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$sourceCapture=$null;$monitor=$null;$dirty=$false;$start=0
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$root/work/build/Release/d2_runtime_probe.exe").Hash
function C($r,$text){& "$lab/Send-D2Command.ps1" -Role $r -Command $text -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$button='Neutral',$ms=100,$lt=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $lt|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$handle=0){for($a=0;$a-lt8;$a++){$raw=if($handle){& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode $handle}else{& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode failed"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_grenade_fixture off','coop_grenade_prop off','coop_review_clip off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $beforeInv[$r]=Q $r '--player-inventory';$beforeInv[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 if((Q $Source '--player-states').stance-ne0){I $Source B 180;Start-Sleep -Milliseconds 700}
 # Known Dunwall room positions. Normal native input performs the throw;
 # no projectile function or animation event is called by the harness.
 C $Source 'setviewpos -34.3 -154.8 68.1 180 0';C $peer 'setviewpos -31.7 -152.9 68.1 216 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 15 -Evidence "$out/observer-camera.json"
 $before=Q $Source '--player-grenade';if($before.quantity-or$before.equipped){throw 'Grenade already owned'};$before|ConvertTo-Json|Set-Content "$out/before.json"
 Q $Source '--entities'|ConvertTo-Json -Depth 10|Set-Content "$out/entities-before.json"
 C $Source 'coop_grenade_fixture 12000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 50;$f=S $Source '.grenade-fixture.json'}while($f.result-ne'native_grenade_ready'-and$w.ElapsedMilliseconds-lt2500)
 if($f.result-ne'native_grenade_ready'){throw 'Native grant did not become ready'}
 I $Source X 1500;Start-Sleep -Milliseconds 1900;I $Source X 650;Start-Sleep -Milliseconds 1350
 $held=Q $Source '--player-grenade';if(!$held.equipped-or$held.quantity-ne1){throw 'Native grenade presentation not selected'}
 # Drawing can recenter the native view. Set and verify the requested throw
 # angle after draw; a pre-draw camera record is not proof of the stimulus.
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch $Pitch -Evidence "$out/source-camera.json"
 $checks.Add('One native grenade selected through normal holster/draw input')
 $character=if($Source-eq'host'){'corvo'}else{'emily'}
 $entry=Get-Content "$PSScriptRoot/grenade-bank-manifest-v56.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
 $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank identity differs'}
 Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C $peer 'coop_movement_bank on'
 $w.Restart();do{$m=S $peer '.movement.json';$prop=S $peer '.grenade-prop.json';if($m.active-and$m.version-eq42-and$m.grenadeWeight-gt.9-and$prop.active-and$prop.network){break};Start-Sleep -Milliseconds 45}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne42-or$m.grenadeWeight-lt.9-or!$prop.active-or!$prop.network){[ordered]@{movement=$m;prop=$prop;stream=(S $peer '.shadow-stream.json');sourceStream=(S $Source '.shadow-stream.json');player=(Q $Source '--player-states')}|ConvertTo-Json -Depth 10|Set-Content "$out/ready-failure.json";throw 'Grenade network body/prop not ready'}
 $checks.Add('Native Steam handshake and exact candidate bank drive the remote held grenade without a prop command')
 $prop|ConvertTo-Json|Set-Content "$out/prop-initial.json"
 $hwnd=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $sourceWindow=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64()
 $sourceArgs=@($args|ForEach-Object {if($_-like 'gfxcapture=*'){"gfxcapture=hwnd=${sourceWindow}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"}elseif($_-eq"$PSScriptRoot/$Take.mp4"){"$PSScriptRoot/$Take-source.mp4"}else{$_}})
 $sourceCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $sourceArgs -WindowStyle Hidden -RedirectStandardError "$out/capture-source.log" -RedirectStandardOutput "$out/capture-source.stdout" -PassThru
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();Start-Sleep -Milliseconds 500;if($capture.HasExited){throw 'Capture failed'}
 if($ValidateReader){if((Q $Source '--player-grenade-scene').valid){throw 'Action reader incorrectly active before input'};$monitor=Start-Process "$root/work/build/Release/d2_runtime_probe.exe" -ArgumentList @($pids[$Source],'--watch-grenade-scenes','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/owned-scenes.ndjson" -RedirectStandardError "$out/owned-scenes.stderr" -PassThru}
 $watch=[Diagnostics.Stopwatch]::StartNew();$fired=$false;$duplicated=$false;$scenes=@{}
 while($watch.ElapsedMilliseconds-lt9000){
  if(!$fired-and$watch.ElapsedMilliseconds-ge350){
   $view=Q $Source '--player';$actualPitch=-[Math]::Asin([double]$view.nativeViewOrientation[2])*180/[Math]::PI
   [ordered]@{expected=$Pitch;actual=$actualPitch;view=$view}|ConvertTo-Json -Depth 8|Set-Content "$out/pitch-at-trigger.json"
   if([Math]::Abs($actualPitch-$Pitch)-gt2){throw 'Native throw pitch changed after preparation'}
   $dirty=$true;I $Source Neutral 1300 255;$fired=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='LT held for 1300ms then native release';pitch=$Pitch;actualPitch=$actualPitch})
  }
  $m=S $peer '.movement.json';$prop=S $peer '.grenade-prop.json'
  if(!$duplicated-and$m.grenadeVariant-eq3){C $Source 'coop_shadow_stream_duplicate';$duplicated=$true}
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;movement=$m;prop=$prop;stream=(S $peer '.shadow-stream.json');sourceStream=(S $Source '.shadow-stream.json');fixture=(S $Source '.grenade-fixture.json')})
  Start-Sleep -Milliseconds 45
 }
 if($monitor){
  $monitor.WaitForExit(2000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native action monitor incomplete'}
  $samples=@(Get-Content "$out/owned-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json});$expected=if($Pitch-ge30){@(3,4,7,8)}else{@(3,4,5,6)}
  foreach($v in $expected){if(!@($samples|Where-Object {$_.valid-and$_.variant-eq$v}).Count){throw "Owned native action variant $v missing"}}
  if((Q $Source '--player-grenade-scene').valid){throw 'Native action reader failed to clear'}
  $checks.Add('Exact native scene reader validates cooking, release and empty recovery then clears')
  foreach($s in $samples){if($s.valid){$scenes[$s.asset]=$true}}
 }
 foreach($part in 'cooking_in','cooking_idle') {if(!$scenes.ContainsKey("midnight/gameplay/player/base/gadgets/gadget_grenade_$part.midnightscene")){throw "Native $part missing"}}
 if(!@($scenes.Keys|Where-Object {$_-match'/gadget_grenade_throw_(high|low)\.midnightscene$'}).Count){throw 'Native throw scene missing'}
 $checks.Add('Actual native cooking entry, cooking idle and throw scene observed')
 if((Q $Source '--player-grenade').quantity-ne0){throw 'No native grenade consumption observed'};$checks.Add('Native action consumed the single granted grenade')
 $expected=if($Pitch-ge30){@(3,4,7,8)}else{@(3,4,5,6)}
 foreach($v in $expected){if(!@($rows|Where-Object {$_.movement.active-and$_.movement.grenadeVariant-eq$v-and$_.movement.grenadeWeight-gt.75}).Count){throw "Remote grenade phase $v absent"}}
 $checks.Add('Native cook entry, cook loop, release and empty recovery drive remote whole-body clips on source clock')
 $release=if($Pitch-ge30){7}else{5};$recovery=$release+1
 if(!@($rows|Where-Object {$_.movement.grenadeVariant-eq$release-and$_.movement.grenadeAmount-lt.7-and$_.prop.active}).Count){throw 'Remote held prop absent before release'}
 if(!@($rows|Where-Object {$_.movement.grenadeVariant-eq$recovery-and$_.movement.grenadeAmount-gt.15-and!$_.prop.active-and!$_.prop.pendingHide}).Count){throw 'Prop remained visible in empty recovery'}
 if(!@($rows|Where-Object {$_.ms-gt6000-and!$_.movement.grenadeHeld-and$_.movement.grenadeWeight-lt.01-and!$_.prop.active}).Count){throw 'Grenade presentation did not recover to standing'}
 $checks.Add('Cosmetic grenade disappears at release and stays hidden through empty recovery and native standing')
 if(!$duplicated){throw 'Duplicate packet stimulus missed'};$checks.Add('Duplicate packet sent during native cook; prop remains one cached native entity')
 if(@($rows|Where-Object {$_.prop.created-ne$prop.created}).Count){throw 'Duplicate prop creation'}
 C $Source 'coop_grenade_fixture off';Start-Sleep -Milliseconds 1200
 foreach($r in 'host','client'){
  $inv=Q $r '--player-inventory';$inv|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($beforeInv[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($inv.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory rollback mismatch'}
  [ordered]@{player=(Q $r '--health' 1);avatar=(Q $r '--health' (S $r).probeActorHandle)}|ConvertTo-Json -Depth 6|Set-Content "$out/health-before-reload-$r.json"
 }
 $checks.Add('Both inventories restored to the pre-grant quantities')
 Q $Source '--entities'|ConvertTo-Json -Depth 10|Set-Content "$out/entities-after.json"
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_grenade_prop off';C $r 'coop_grenade_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(15000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure='Capture incomplete'}}
 if($sourceCapture){$sourceCapture.WaitForExit(15000)|Out-Null;if(!$sourceCapture.HasExited-or$sourceCapture.ExitCode){$failure='Source capture incomplete'}}
 if($monitor){$monitor.WaitForExit(11000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure='Native action monitor incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content "$out/states.json"
 # A native grenade changes the temporary world. Restore both isolated
 # checkpoints even if an observation/check failed. Never save this world.
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Post-reload health differs'};if((Q $r '--player-grenade').quantity){throw 'Grenade remains after reload'}};$checks.Add('Both isolated checkpoints restored with original player/avatar health and no grenades')}catch{$failure=($failure+'; Checkpoint restoration failed: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=$_.Exception.Message}
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;pitch=$Pitch;checks=$checks;events=$events;scenes=@($scenes.Keys);dll=$initialDll;probe=$initialProbe;finalDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Standing armed native grenade body/held-prop presentation over Steam. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Both checkpoints restored after firing. Visual acceptance separate.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_GRENADE_NETWORK $Source $($checks.Count) checks"

