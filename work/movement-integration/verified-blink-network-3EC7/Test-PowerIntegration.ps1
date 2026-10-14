param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='integrated-power-corvo-v1',[switch]$Record,[switch]$ExpectBlinkFx)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function InputState($r,$trigger=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $trigger|Out-Null}
function Probe($r,$mode,$id=0){for($i=0;$i-lt12;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
function State($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
try{
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;$handles[$r]=(State $r).probeActorHandle;InputState $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_power_fixture off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';if((Probe $r '--health' 1).current-ne56){throw 'Clean living fixture required'}}
 C $target 'setviewpos -28.5 -151.2 68.1 180 0';Start-Sleep -Milliseconds 300;C $Source 'setviewpos -33.5 -153 68.1 90 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 25 -Evidence "$out/source-camera.json"
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch 7 -Evidence "$out/observer-camera.json"
 Copy-Item "$PSScriptRoot/power-banks-v1/$character-power-bank.bin" "$root/work/lab-instances/$target/coop-status.json.movement.bin"
 C $Source 'coop_power_fixture 20000';$watch=[Diagnostics.Stopwatch]::StartNew();do{$power=State $Source '.power-fixture.json';if($power.result-eq'native_power_ready'){break};Start-Sleep -Milliseconds 50}while($watch.ElapsedMilliseconds-lt6000)
 if($power.result-ne'native_power_ready'){throw "Power did not equip: $($power.result)"};Start-Sleep -Milliseconds 1100
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $target 'coop_movement_bank on'
 $watch.Restart();do{$m=State $target '.movement.json';if($m.active-and$m.version-eq15-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($watch.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne15-or$m.weight-lt.9){throw 'Power bank did not become ready'}
 $checks.Add('Native Steam stream and v15 bank ready before the cast')
 $fxBefore=State $target '.power-fx.json';$wireBefore=State $target '.power-fx-stream.json';$sendBefore=State $Source '.power-fx-stream.json';$castStart=(State $Source).localPosition;$castEnd=$null;$duplicateSent=$false
 if($Record){
  $window=(Get-Process -Id $pids[$target]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer game window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 }
 $watch.Restart();$stage=0;$seen=@{};$events=@{};$cancelStart=$null
 while($watch.ElapsedMilliseconds-lt9500){
  $ms=$watch.ElapsedMilliseconds
  if($stage-eq0-and$ms-ge1000){InputState $Source 255 1200;$stage=1}
  if($stage-eq1-and$ms-ge4300){$castEnd=(State $Source).localPosition;C $Source 'setviewpos -33.5 -153 68.1 90 0';$stage=2}
  if($stage-eq2-and$ms-ge5500){$cancelStart=(State $Source).localPosition;InputState $Source 255 3000;$stage=3}
  if($stage-eq3-and$ms-ge6500){InputState $Source 255 180 X;$stage=4}
  if($stage-eq4-and$ms-ge6750){InputState $Source;$stage=5}
  if($ExpectBlinkFx-and!$duplicateSent-and$ms-ge7600){C $Source 'coop_power_fx duplicate';$duplicateSent=$true}
  $native=Probe $Source '--player-states';$m=State $target '.movement.json';$stream=State $target '.shadow-stream.json';$sourceState=State $Source;$targetState=State $target
  $rows.Add([pscustomobject]@{ms=$ms;stage=$stage;native=$native;movement=$m;stream=$stream;source=$sourceState;target=$targetState;sourceFx=(State $Source '.power-fx-stream.json');targetFx=(State $target '.power-fx-stream.json');effects=(State $target '.power-fx.json')})
  if(!$m.active-or!$stream.armed-or!$stream.ready){throw "Power presentation retired: $($stream.result)"}
  if($m.powerWeight-gt.5-and$m.weight-gt.8){$seen[[string]$m.powerVariant]=$true;$events[[string]$m.powerEvent]=$true}
  if(!(State $Source '.power-fixture.json').active){throw 'Power fixture retired during test'}
  Start-Sleep -Milliseconds 15
 }
 $expected=if($Source-eq'host'){@(1,2,3)}else{@(4,5,6,7)}
 foreach($v in $expected){if(!$seen.ContainsKey([string]$v)){throw "Remote power phase missing $v"}}
 $checks.Add("$character native cast phases $($expected -join ',') drive remote whole-body presentation")
 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Power failed to recover to native standing'};$checks.Add('Native phase end restores standing movement')
 $pos=(State $Source).localPosition;$distance=[Math]::Sqrt([Math]::Pow($pos[0]-$cancelStart[0],2)+[Math]::Pow($pos[1]-$cancelStart[1],2)+[Math]::Pow($pos[2]-$cancelStart[2],2))
 if($distance-gt.1){throw "Native cancellation still moved player $distance metres"};$checks.Add('Native X cancellation does not teleport or pull the source')
 foreach($r in 'host','client'){if((Probe $r '--health' 1).current-ne56-or(Probe $r '--health' $handles[$r]).current-ne40){throw 'Native health changed'}}
 if((Probe $target '--player-power').quantity-ne0-or!(Probe $Source '--player-sword').equipped-or!(Probe $target '--avatar-sword' $handles[$target]).equipped){throw 'Power changed peer loadout or sword attachment'}
 $checks.Add('Both players/avatar health, source first-person sword, remote native sword and peer power quantity preserved')
 if($ExpectBlinkFx){
  $fx=State $target '.power-fx.json';$wire=State $target '.power-fx-stream.json';$send=State $Source '.power-fx-stream.json'
  if($Source-ne'host'-or$fx.started-ne($fxBefore.started+2)-or$wire.applied-ne($wireBefore.applied+2)-or$send.sent-ne($sendBefore.sent+2)-or$fx.active){throw 'Blink needs exactly one departure/arrival and no effects for cancellation'}
  $checks.Add('Native Blink travel creates exactly one network departure and one arrival; aim cancellation creates neither')
  if($wire.rejected-ne($wireBefore.rejected+1)-or$wire.phase-ne2){throw 'Duplicate native FX event was not rejected cleanly'};$checks.Add('Repeated reliable arrival packet cannot replay effects')
  [double]$maxEndpointError=0;[double]$maxHeight=0
  foreach($row in $rows|Where-Object {$_.ms-ge1000-and$_.ms-lt4200}){[double]$a=0;[double]$b=0;for($k=0;$k-lt3;$k++){[double]$v=$row.target.probeRenderPosition[$k];$a+=[Math]::Pow($v-$castStart[$k],2);$b+=[Math]::Pow($v-$castEnd[$k],2)};$maxEndpointError=[Math]::Max($maxEndpointError,[Math]::Sqrt([Math]::Min($a,$b)));$maxHeight=[Math]::Max($maxHeight,[double]($row.target.probeRenderPosition[2]-$castStart[2]))}
  if($maxEndpointError-gt.35-or$maxHeight-gt.35){throw "Remote Blink body traversed eye-height/intermediate positions: endpoint=$maxEndpointError height=$maxHeight"}
  $checks.Add("Remote Blink uses a discrete endpoint transfer with no eye-height body jump (max endpoint error $maxEndpointError m)")
 }
 if($capture){$capture.WaitForExit(7000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Recorder did not finalize'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{InputState $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_power_fixture off';Start-Sleep -Milliseconds 1500;C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(16000)|Out-Null}
 $rows|ConvertTo-Json -Depth 14|Set-Content "$out/states.json"
 [ordered]@{source=$Source;character=$character;failure=$failure;checks=$checks;seenPhases=@($seen.Keys);dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bank=(Get-FileHash "$PSScriptRoot/power-banks-v1/$character-power-bank.bin").Hash;recorded=[bool]$Record;blinkFxTested=[bool]$ExpectBlinkFx;scope='Native source power scene phases transported over Steam into full-body pose candidates. Native source controls gameplay outcomes. Blink FX only claimed when explicitly checked. Armed, standing Dunwall fixture; not complete powers or movement.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_POWER_INTEGRATION $character $($checks.Count) checks"
