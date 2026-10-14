param([ValidateSet('stream-stop','pause')][string]$Mode='stream-stop',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='grapple-fx-stream-stop-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$paused=$false
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function I($r,$button='Neutral',$trigger=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -LeftTrigger $trigger -DurationMs $ms|Out-Null}
function P($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw 'Probe failed'}
function Snap($stage){$row=[pscustomobject]@{stage=$stage;tick=[Environment]::TickCount64;client=(S client);host=(S host);fx=(S host '.power-fx.json');stream=(S host '.grapple-fx.json');shadow=(S host '.shadow-stream.json')};$rows.Add($row);return $row}
try{
 foreach($r in 'client','host'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r;C $r 'coop_shadow_stream off';C $r 'coop_movement_bank off';C $r 'coop_power_fixture off';C $r 'coop_power_fx off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';if((P $r '--health' 1).current-ne56-or(P $r '--health' $handles[$r]).current-ne40){throw 'Healthy fixture required'}}
 C host 'setviewpos -28.5 -151.2 68.1 180 0';Start-Sleep -Milliseconds 250;C client 'setviewpos -33.5 -153 68.1 90 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 25 -Evidence "$out/camera.json"
 Copy-Item "$PSScriptRoot/power-banks-v1/emily-power-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
 C client 'coop_power_fixture 20000';$watch=[Diagnostics.Stopwatch]::StartNew();do{$power=S client '.power-fixture.json';if($power.result-eq'native_power_ready'){break};Start-Sleep -Milliseconds 40}while($watch.ElapsedMilliseconds-lt5000)
 if($power.result-ne'native_power_ready'){throw 'Power not ready'};Start-Sleep -Milliseconds 1100
 foreach($r in 'client','host'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C host 'coop_movement_bank on'
 $watch.Restart();do{$m=S host '.movement.json';if($m.active-and$m.version-eq15-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($watch.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne15-or$m.weight-lt.9){throw 'Movement not ready'};$before=Snap before
 I client Neutral 255 1000;$watch.Restart();do{$state=Snap cast;if($state.fx.started-gt$before.fx.started-and$state.fx.active){break};Start-Sleep -Milliseconds 15}while($watch.ElapsedMilliseconds-lt4500)
 if($state.fx.started-le$before.fx.started-or!$state.fx.active){throw 'No active native network effect to interrupt'};$checks.Add('Actual native Far Reach produced an owned remote effect before interruption')
 if($Mode-eq'stream-stop'){
  C host 'coop_shadow_stream off';$watch.Restart();do{$state=Snap stopped;if(!$state.fx.active){break};Start-Sleep -Milliseconds 30}while($watch.ElapsedMilliseconds-lt1000)
  if($state.fx.active-or$state.shadow.armed){throw 'Stream cancellation did not retire owned effects'};$checks.Add('Stopping the receiving stream promptly retires its active native FX records')
 }else{
  I host Start 0 180;$watch.Restart();do{$state=Snap pausing;if($state.host.localGameLogicState-eq2){break};Start-Sleep -Milliseconds 35}while($watch.ElapsedMilliseconds-lt2000)
  if($state.host.localGameLogicState-ne2){throw 'Native pause was not observed'};$paused=$true;$pauseStarted=$state.fx.started
  Start-Sleep -Milliseconds 1000;$state=Snap paused;if($state.fx.started-ne$pauseStarted){throw 'New FX spawned while paused'};$checks.Add('Native pause prevents additional effects and retires the presentation stream')
  I host A 0 180;$paused=$false;$watch.Restart();do{$state=Snap resume;if($state.host.localGameLogicState-eq1-and!$state.fx.active){break};Start-Sleep -Milliseconds 35}while($watch.ElapsedMilliseconds-lt3500)
  if($state.host.localGameLogicState-ne1-or$state.fx.active-or$state.shadow.armed){throw 'Pause/resume did not complete owned FX cleanup'};$checks.Add('Resume cleans up retained FX ownership and leaves the stream disarmed')
 }
 $started=$state.fx.started;Start-Sleep -Milliseconds 500;$state=Snap settled;if($state.fx.active-or$state.fx.started-ne$started-or$state.stream.beamId-ne4294967295){throw 'Retired events replayed after interruption'};$checks.Add('No deferred beam or impact replays after retirement')
 foreach($r in 'client','host'){if((P $r '--health' 1).current-ne56-or(P $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}};$checks.Add('Both players and avatars retain native health')
 foreach($f in (Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original game/Steam files and protected saves unchanged')
}catch{$failure=$_.Exception.Message}finally{
 if($paused){try{I host A 0 180;Start-Sleep -Milliseconds 350}catch{}}
 foreach($r in 'client','host'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_power_fx off';C $r 'coop_power_fixture off';Start-Sleep -Milliseconds 1500;C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 9|Set-Content "$out/states.json"
 [ordered]@{mode=$Mode;failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_GRAPPLE_FX_LIFECYCLE $Mode $($checks.Count) checks"
