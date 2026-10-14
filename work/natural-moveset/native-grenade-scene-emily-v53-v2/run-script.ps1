param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-grenade-equip-v52')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventories=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$start=0
function C($r,$text){& "$lab/Send-D2Command.ps1" -Role $r -Command $text -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$handle=0){for($a=0;$a-lt12;$a++){$raw=if($handle){& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode $handle}else{& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw "Native read $r $mode failed"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_grenade_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventories[$r]=Q $r '--player-inventory';$inventories[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 if((Q $Source '--player-grenade-scene').valid){throw 'Unexpected baseline scene'}
 $before=Q $Source '--player-grenade';if($before.quantity-or$before.equipped-or$before.started){throw 'Grenade already owned/selected'};$before|ConvertTo-Json|Set-Content "$out/before.json"
 if((Q $Source '--player-states').stance-ne0){I $Source B 180;Start-Sleep -Milliseconds 700}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json"
 $hwnd=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();Start-Sleep -Milliseconds 650;if($capture.HasExited){throw 'Capture failed'}
 C $Source 'coop_grenade_fixture 12000';$watch=[Diagnostics.Stopwatch]::StartNew();$step=0;$equipped=$false;$scenes=@{}
 while($watch.ElapsedMilliseconds-lt11500){
  # Native inventory selection alone leaves the first-person hand in its old
  # presentation. Ordinary holster/draw input starts actual equipment scenes.
  if($step-eq0-and$watch.ElapsedMilliseconds-ge500){I $Source X 1500;$step=1}
  if($step-eq1-and$watch.ElapsedMilliseconds-ge2500){I $Source X 650;$step=2}
  if($step-eq2-and$watch.ElapsedMilliseconds-ge4000){I $Source B 180;$step=3}
  if($step-eq3-and$watch.ElapsedMilliseconds-ge5200){I $Source B 180;$step=4}
  if($step-eq4-and$watch.ElapsedMilliseconds-ge6200){I $Source X 1500;$step=5}
  if($step-eq5-and$watch.ElapsedMilliseconds-ge8200){I $Source X 650;$step=6}
  if($step-eq6-and$watch.ElapsedMilliseconds-ge9700){C $Source 'coop_grenade_fixture off';$step=7}
  $ownedScene=Q $Source '--player-grenade-scene';$midnight=Q $Source '--midnight';$item=Q $Source '--player-grenade';$player=Q $Source '--player-states';$fixture=S $Source '.grenade-fixture.json'
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;ownedScene=$ownedScene;midnight=$midnight;item=$item;player=$player;fixture=$fixture})
  foreach($scene in $midnight.instances){if($scene.state-eq3-and$scene.asset-match'/gadget_grenade_'){$scenes[$scene.asset]=$true}}
  # The generic probe runs later than the exact reader. A 233 ms unequip can
  # finish between those samples; retain the independently validated scene.
  if($ownedScene.valid){$scenes[$ownedScene.asset]=$true}
  if($item.quantity-eq1-and$item.equipped-and$item.started-and$fixture.result-eq'native_grenade_ready'){$equipped=$true}
  Start-Sleep -Milliseconds 20
 }
 foreach($v in 1,2){if(@($rows|Where-Object {$_.ownedScene.valid-and$_.ownedScene.variant-eq$v}).Count-lt1){throw "Owned scene variant $v not observed"}};$checks.Add('Read-only exact owned arm/item scene reader observes both native equip variants')
 if(!$equipped){throw 'Native grenade equip never reached ready'};$checks.Add('Native quantity callback and native equip produce one selected grenade with exact stable owner')
 if(!$scenes.ContainsKey('midnight/gameplay/player/base/gadgets/gadget_grenade_equip.midnightscene')-or!$scenes.ContainsKey('midnight/gameplay/player/base/gadgets/gadget_grenade_unequip.midnightscene')){throw 'Both native equip/unequip scenes required'};$checks.Add('Actual native grenade equip and unequip Midnight scenes recorded')
 if(@($rows|Where-Object {$_.item.equipped-and$_.player.stance-eq1}).Count-lt2){throw 'Crouched equipped observations missing'};$checks.Add('Native grenade remains selected while crouching and returns to standing')
 if(@($scenes.Keys|Where-Object {$_-match'cooking|throw|slash'}).Count){throw 'Unexpected grenade action'};$checks.Add('No native cooking, throw or slash scene observed in the equip-only test')
 if((Q $Source '--player-grenade-scene').valid){throw 'Scene reader did not clear'};$checks.Add('Scene reader refuses baseline and clears after native rollback')
 $after=Q $Source '--player-grenade';$after|ConvertTo-Json|Set-Content "$out/after.json";$f=S $Source '.grenade-fixture.json'
 if($after.quantity-or$after.equipped-or$after.started-or$f.active-or$f.result-ne'quantity_and_selection_restored'-or$after.handle-ne$before.handle-or$after.component-ne$before.component){throw 'Original grenade state not restored'};$checks.Add('Native rollback restores zero quantity, empty slot and original component/handle')
 foreach($r in 'host','client'){
  $afterInv=Q $r '--player-inventory';$afterInv|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventories[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($afterInv.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw "Inventory quantities changed for $r"}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed'}
 }
 $checks.Add('Both full inventory quantities and player/avatar health remain unchanged')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected settings/save hashes retained')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_grenade_fixture off'}catch{}}
 Start-Sleep -Milliseconds 1200
 foreach($r in 'host','client'){try{if((Q $r '--player-states').stance-eq1){I $r B 180};if(!(Q $r '--player-sword').equipped){I $r X 650};C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(15000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure='Capture incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;checks=$checks;scenes=@($scenes.Keys);dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$root/work/build/Release/d2_runtime_probe.exe").Hash;scope='Native source grenade equip/unequip and stance observations only. No cooking, throw, remote prop/body binding or gameplay replication is claimed.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_GRENADE_EQUIP $Source $($checks.Count) checks"
