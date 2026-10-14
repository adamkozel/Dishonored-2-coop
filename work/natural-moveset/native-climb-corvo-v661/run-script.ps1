param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-climb-corvo-v661',[ValidateSet('host','client')][string]$Source='host')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$capture=$null
$pids=@{};$inventory=@{};$samples=[Collections.Generic.List[object]]::new();$inputs=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new()
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms -LeftY $y|Out-Null}
function S($r){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt5;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='NativeClimb661';take=$Take;state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/candidate-climb-fixture-v660/identity.json" -Raw|ConvertFrom-Json
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw 'Candidate660 required'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid
  I $r;foreach($cmd in 'coop_movement_resume off','coop_review_clip off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off'){C $r $cmd}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-$r.json"
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Living baseline required'}
 }
 $dirty=$true;C $Source 'setviewpos -33 -153 68.1 90 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json" -Quick
 C $Source 'coop_movement_fixture 60000';C $Source 'coop_decl_probe';Start-Sleep -Milliseconds 100
 Copy-Item "$root/work/lab-instances/$Source/coop-status.json.climb-definition.json" "$out/definition.json"
 $hwnd=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','22','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/source.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/source-capture.log" -RedirectStandardOutput "$out/source-capture.stdout" -PassThru
 C $Source 'coop_climb_fixture spawn';Start-Sleep -Milliseconds 350
 $fixture=Get-Content "$root/work/lab-instances/$Source/coop-status.json.climb-fixture.json" -Raw|ConvertFrom-Json
 $fixture|ConvertTo-Json -Depth 6|Set-Content "$out/fixture.json"
 if($fixture.stage-ne4){throw "Fixture refused: $($fixture.result)"}
 (Q $Source '--components' $fixture.handle)|ConvertTo-Json -Depth 9|Set-Content "$out/chain-components.json"
 (Q $Source '--object' $fixture.actor)|ConvertTo-Json -Depth 9|Set-Content "$out/chain-object.json"
 (Q $Source '--entities')|ConvertTo-Json -Depth 9|Set-Content "$out/entities-after-spawn.json"
 $watch=[Diagnostics.Stopwatch]::StartNew();$step=0
 while($watch.ElapsedMilliseconds-lt19000){
  if($step-eq0-and$watch.ElapsedMilliseconds-ge2200){I $Source X 250;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='X250'});$step++}
  if($step-eq1-and$watch.ElapsedMilliseconds-ge4800){I $Source Neutral 2800 22000;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='forward2800'});$step++}
  if($step-eq2-and$watch.ElapsedMilliseconds-ge9500){I $Source Neutral 2800 -22000;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='backward2800'});$step++}
  if($step-eq3-and$watch.ElapsedMilliseconds-ge14500){I $Source B 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='B200'});$step++}
  $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;player=(Q $Source '--player-states');view=(Q $Source '--player');source=(S $Source)})
  Start-Sleep -Milliseconds 75
 }
 I $Source
 $samples|ConvertTo-Json -Depth 15|Set-Content "$out/observations.json";$inputs|ConvertTo-Json|Set-Content "$out/inputs.json"
 (Q $Source '--components' $fixture.handle)|ConvertTo-Json -Depth 9|Set-Content "$out/chain-components-after.json"
 $checks.Add('Original loaded chain created once with native components; ordinary X, movement and B inputs recorded')
}catch{$failure=$_.Exception.Message;if(Test-Path -LiteralPath $out){$samples|ConvertTo-Json -Depth 15|Set-Content "$out/partial-observations.json";$inputs|ConvertTo-Json|Set-Content "$out/partial-inputs.json"}}finally{
 if($owned){
  foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_fixture off'}catch{}}
  if($capture){$capture.WaitForExit(25000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture incomplete').TrimStart('; ')}}
  if($dirty){try{
   $dead=$false;foreach($r in 'host','client'){if(!(Get-Process -Id $pids[$r] -ErrorAction SilentlyContinue)){$dead=$true}}
   if($dead){& "$root/work/movement-integration/Deploy-SourceClock.ps1" *> "$out/crash-restore.log";foreach($r in 'host','client'){$pids[$r]=(Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json).pid}}
   else{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"}
   foreach($r in 'host','client'){
    $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-restored-$r.json"
    if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Checkpoint inventory differs'}
    if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}
   }
   if(@((Q $Source '--entities').entities|Where-Object name -eq 'd2coop_climb_fixture').Count){throw 'Fixture survived checkpoint reset'}
   $checks.Add('Checkpoint removed fixture and restored both inventories and player/partner health')
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path -LiteralPath $out){[ordered]@{passed=(!$failure);failure=$failure;dll=$dll;checks=$checks;samples=$samples.Count;scope='Native climb fixture and source input discovery only. Does not establish remote body binding, shared chain physics, water or complete climbing.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"}
  [ordered]@{pid=$PID;operation='NativeClimb661';take=$Take;state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json";$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_NATIVE_CLIMB_DISCOVERY $($checks.Count) checks"
