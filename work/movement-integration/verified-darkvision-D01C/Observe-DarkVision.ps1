param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-darkvision-corvo-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$start=0;$peer=if($Source-eq'host'){'client'}else{'host'}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$trigger=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $trigger|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
function S($r,$suffix=''){return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_power_fixture off','coop_darkvision_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 C $Source 'setviewpos -33.5 -153 68.1 270 0';C $peer 'setviewpos -33.5 -149 68.1 90 0'
 if((Q $Source '--player-states').stance-ne0){I $Source 0 100 B;Start-Sleep -Milliseconds 700}
 if(!(Q $Source '--player-sword').equipped){I $Source 0 650 X;Start-Sleep -Milliseconds 2200}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/camera.json"
 $before=Q $Source '--player-darkvision';$inventory=Q $Source '--player-inventory';$before|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/before.json";$inventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-before.json"
 if($before.quantity-or$before.equipped-or$before.managerStarted){throw 'Native Dark Vision already owned or active'}
 C $Source 'coop_darkvision_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew();do{$s=S $Source '.darkvision-fixture.json';if($s.result-eq'native_darkvision_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($s.result-ne'native_darkvision_ready'){throw "Dark Vision did not equip: $($s.result)"}
 $power=Q $Source '--player-darkvision';$power|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1-or(Q $peer '--player-darkvision').quantity){throw 'Dark Vision acquisition was not exclusive to source'}
 $checks.Add('Exact native Dark Vision equipment acquired and selected only on source')
 $window=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$window){throw 'No source game window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','14','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$times=@(1000,4500,8000)
 while($watch.ElapsedMilliseconds-lt11500){
  if($next-lt$times.Count-and$watch.ElapsedMilliseconds-ge$times[$next]){I $Source 255 250;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native LT press 250ms';index=$next});$next++}
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q $Source '--player-states');midnight=(Q $Source '--midnight');power=(Q $Source '--player-darkvision');component=(Q $Source '--object' $power.component);manager=(Q $Source '--object' $power.manager);peer=(Q $peer '--player-darkvision')})
  if(!(S $Source '.darkvision-fixture.json').active){throw 'Dark Vision fixture retired during native capture'};Start-Sleep -Milliseconds 30
 }
 if(!@($rows|Where-Object {$_.power.managerStarted}).Count){throw 'Native vision manager never activated'}
 if(@($rows|Where-Object {$_.peer.managerStarted-or$_.peer.quantity}).Count){throw 'Peer vision was activated'}
 $checks.Add('Native controller casting starts the source vision manager; peer remains unchanged')
 I $Source;C $Source 'coop_darkvision_fixture off';$w.Restart();do{$s=S $Source '.darkvision-fixture.json';if(!$s.active){break};Start-Sleep -Milliseconds 80}while($w.ElapsedMilliseconds-lt32000)
 if($s.active-or$s.result-ne'quantity_selection_and_effect_inactive'){throw "Dark Vision cleanup unconfirmed: $($s.result)"}
 $after=Q $Source '--player-darkvision';$after|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/after.json"
 if($after.quantity-or$after.equipped-or$after.started-or$after.managerStarted){throw 'Dark Vision remained owned, equipped or active'}
 $afterInventory=Q $Source '--player-inventory';$afterInventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-after.json"
 $a=@($inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});$b=@($afterInventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});if(!$a.Count-or(Compare-Object $a $b)){throw 'Inventory quantities changed after cleanup'}
 $checks.Add('Native cleanup restores inventory quantities and selection; vision manager inactive')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Native health changed'}}
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}}
 $checks.Add('Both player health and protected files unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_darkvision_fixture off';Start-Sleep -Milliseconds 1500;C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(18000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Native capture incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);source=$Source;failure=$failure;checks=$checks;events=$events;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;capture='Actual native source POV and read-only state/scene observations';scope='Native Dark Vision investigation only; remote body and world-visible effects are not certified. Runtime acquired entity and discovery metadata require isolated checkpoint reload for exact baseline restoration.'}|ConvertTo-Json -Depth 7|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"OBSERVED_NATIVE_DARKVISION $Source $($rows.Count) samples"
