param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12,[ValidateSet('standing','crouched')][string]$Stance='standing')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$capture=$null;$monitor=$null;$rawMonitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
function OptionalFx($r){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --mesmerize-effects;$code=$LASTEXITCODE;if($code){return [pscustomobject]@{stable=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true;C client 'setviewpos -48.59 -155.07 68.1 90 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $before=Q client '--player-mesmerize';$before|ConvertTo-Json -Depth 7|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Mesmerize already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 $dirty=$true;C client 'coop_mesmerize_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.mesmerize-fixture.json';if($f.result-eq'native_mesmerize_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($f.result-ne'native_mesmerize_ready'){throw "Native Mesmerize acquisition failed: $($f.result)"}
 $power=Q client '--player-mesmerize';$power|ConvertTo-Json -Depth 7|Set-Content "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'Native Mesmerize equipment not confirmed'}
 $checks.Add('Exact native Emily Mesmerize item acquired and selected without directly invoking power or VFX')
 if($Stance-eq'crouched'){
  & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500
 }
 $nativeState=Q client '--player-states';$nativeState|ConvertTo-Json -Depth 9|Set-Content "$out/state-before.json"
 $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($nativeState.stance-ne$expectedStance-or!$nativeState.defaultMode){throw 'Native stance not established'}
 $checks.Add('Selected native stance established before the equipment observation')
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-power-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 $rawMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $w.Restart();$cast=$false;$holstered=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$holstered-and$w.ElapsedMilliseconds-ge1000){
   & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null;$holstered=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native X held1000ms to holster equipment'})
  }
  if(!$cast-and$w.ElapsedMilliseconds-ge4500){I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT held 1800ms then released'})}
  $scene=Q client '--midnight';$entities=Q client '--entities'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');sword=(Q client '--player-sword');midnight=$scene;power=(Q client '--player-mesmerize');component=(Q client '--object' $power.component);effects=(OptionalFx client);entities=@($entities.entities|Where-Object {$_.definitionName-match'mesmerize'})})
  Start-Sleep -Milliseconds 25
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native scene watcher failed'}
 $fastScenes=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $rawMonitor.WaitForExit(4000)|Out-Null;if(!$rawMonitor.HasExited-or$rawMonitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $rawSamples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $rawScenes=@($rawSamples|ForEach-Object {$_.instances}|Where-Object {$_.state-eq3-and$_.asset-like'*power_mesmerize_cast_*'})
 $holsteredRows=@($rows|Where-Object {$_.ms-gt2600-and$_.ms-lt4400-and!$_.sword.equipped})
 if(!$holstered-or!$holsteredRows.Count){throw 'Native X did not establish holstered sword before LT'}
 if(@($rows.player|Where-Object {$_.stance-ne$expectedStance-or!$_.defaultMode}).Count){throw 'Unexpected stance/mode during holstered observation'}
 $checks.Add('Native sword is confirmed holstered before LT, with native stance preserved throughout observation')
 foreach($scene in $rawScenes){if(@($scene.bindingsResearch).Count-ne1-or!$scene.bindingsResearch[0].reciprocal){throw 'Observed power phase has an unexpected native binding'}}
 $phaseAssets=@($rawScenes.asset|Select-Object -Unique)
 [ordered]@{stance=$Stance;strictAccepted=@($fastScenes|Where-Object valid).Count;nativePhaseAssets=$phaseAssets;holsteredBeforeUseSamples=$holsteredRows.Count;equippedAfterUseSamples=@($rows|Where-Object {$_.ms-gt5000-and$_.sword.equipped}).Count;manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;raw=$rawScenes;playerStates=@($rows.player)}|ConvertTo-Json -Depth 14|Set-Content "$out/holster-scene-observations.json"
 $checks.Add('Native phase, reciprocal arm, sword equipment and mana observations saved without inferring automatic draw or a successful cast')
 C client 'coop_mesmerize_fixture off';$w.Restart()
 do{$f=S client '.mesmerize-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Mesmerize inventory did not retire'}
 foreach($r in 'host','client'){$after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json";if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'};if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
 $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_mesmerize_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 if($rawMonitor){$rawMonitor.WaitForExit(18000)|Out-Null;if(!$rawMonitor.HasExited-or$rawMonitor.ExitCode){$failure=($failure+'; raw scene watcher failed').TrimStart('; ')}}
 if($capture){$capture.WaitForExit(18000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(18000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene watcher failed').TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$p=Q client '--player-mesmerize';if($p.quantity-or$p.equipped-or$p.started){throw 'Mesmerize remains after checkpoint'};$fx=Q client '--mesmerize-effects';if(@($fx.effects).Count){throw 'Mesmerize effects remain after checkpoint'};$checks.Add('Both isolated checkpoints restore clean health/inventory and retire native Mesmerize effects')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Source-only native holstered Mesmerize observation. Native X holsters, then one LT hold/release tests the actual draw/use behavior in the requested stance. Raw scene/equipment/mana evidence determines whether a cast occurs and which equipment context it uses. No remote integration or complete movement coverage claimed. Isolated native checkpoints and protected hashes restore.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_MESMERIZE_OBSERVATION $($checks.Count) checks"
