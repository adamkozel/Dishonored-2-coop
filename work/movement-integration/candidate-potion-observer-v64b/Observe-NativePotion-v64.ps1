param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateSet('health','mana')][string]$Kind='health',[ValidateSet('None','X','Y')][string]$WheelButton='None')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$beforeInv=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$monitor=$null;$dirty=$false
$kindId=if($Kind-eq'health'){1}else{2};$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$root/work/build/Release/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string[]]$buttons=@('Neutral'),$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $buttons -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_grenade_fixture off','coop_potion_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $beforeInv[$r]=Q $r '--player-inventory';$beforeInv[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 C $Source 'setviewpos -34.3 -154.8 68.1 0 0'
 if((Q $Source '--player-states').stance-ne0){I $Source B 180;Start-Sleep -Milliseconds 650}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json" -Quick
 $before=Q $Source '--player-potion' $kindId;$before|ConvertTo-Json -Depth 6|Set-Content "$out/potion-before.json"
 if(!$before.valid-or$before.quantity){throw 'Requested native scalar potion must be unowned'}
 $dirty=$true;C $Source "coop_potion_fixture $Kind";$wait=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 50;$f=S $Source '.potion-fixture.json'}while($f.result-ne'native_potion_ready'-and$wait.ElapsedMilliseconds-lt2500)
 if($f.result-ne'native_potion_ready'){throw 'Native finite potion grant did not become ready'}
 $f|ConvertTo-Json|Set-Content "$out/fixture-ready.json";$held=Q $Source '--player-potion' $kindId
 if(!$held.valid-or$held.quantity-ne1){throw 'Native potion quantity differs'};$checks.Add('Exactly one scalar native potion granted without equipping or invoking consumption')
 $hwnd=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','9','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();Start-Sleep -Milliseconds 500;if($capture.HasExited){throw 'Capture failed'}
 $monitor=Start-Process "$root/work/build/Release/d2_runtime_probe.exe" -ArgumentList @($pids[$Source],'--watch-potion-states',$kindId,'8000') -WindowStyle Hidden -RedirectStandardOutput "$out/potion-states.ndjson" -RedirectStandardError "$out/potion-states.stderr" -PassThru
 $watch=[Diagnostics.Stopwatch]::StartNew();$opened=$false;$pressed=$false
 while($watch.ElapsedMilliseconds-lt7500){
  if(!$opened-and$watch.ElapsedMilliseconds-ge350){I $Source LeftShoulder 3500;$opened=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LeftShoulder item wheel'})}
  if($WheelButton-ne'None'-and!$pressed-and$watch.ElapsedMilliseconds-ge1700){I $Source @('LeftShoulder',$WheelButton) 250;$pressed=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input="Native item wheel plus $WheelButton"})}
  $scene=Q $Source '--midnight';$rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;status=(S $Source);player=(Q $Source '--player-states');scenes=@($scene.instances|Where-Object {$_.state-eq3});fixture=(S $Source '.potion-fixture.json')})
  Start-Sleep -Milliseconds 50
 }
 I $Source;Start-Sleep -Milliseconds 300;$after=Q $Source '--player-potion' $kindId;$after|ConvertTo-Json -Depth 6|Set-Content "$out/potion-after.json"
 if(!$after.valid){throw 'Native final potion read unavailable'}
 if($WheelButton-eq'None'){if($after.quantity-ne1){throw 'Observation-only wheel unexpectedly consumed the potion'};$checks.Add('Native wheel observation preserves the granted potion')}
 else{if(!$pressed-or$after.quantity-ne0){throw 'Native wheel input did not consume the granted potion'};$checks.Add('Native input consumed one potion');if($Kind-eq'health'){if(!$after.healthValid-or$after.health-le$before.health){throw 'Native health did not increase'};$checks.Add('Native health increases after consumption without a health setter')}}
 C $Source 'coop_potion_fixture off';Start-Sleep -Milliseconds 700
 foreach($r in 'host','client'){
  $inv=Q $r '--player-inventory';$inv|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($beforeInv[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($inv.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory rollback differs'}
 }
 $checks.Add('Both inventories restore exactly after finite native grant')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_potion_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(12000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(10000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; monitor failed').TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 15|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'};$p=Q $r '--player-potion' $kindId;if(!$p.valid-or$p.quantity){throw 'Potion remains after reload'}};$checks.Add('Both native isolated checkpoints restore health and unowned potion state')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;kind=$Kind;wheelButton=$WheelButton;checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;finalDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_POTION_OBSERVATION $Source $($checks.Count) checks"
