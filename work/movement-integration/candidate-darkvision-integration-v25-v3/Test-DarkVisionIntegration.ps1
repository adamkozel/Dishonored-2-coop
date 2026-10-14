param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='darkvision-corvo-v25-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$peer=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$recorders=@();$start=0
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$trigger=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $trigger|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_power_fixture off','coop_darkvision_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 C $Source 'setviewpos -33.5 -153 68.1 90 0';C $peer 'setviewpos -28.5 -151.2 68.1 180 0'
 if((Q $Source '--player-states').stance-ne0){I $Source 0 100 B;Start-Sleep -Milliseconds 700}
 if(!(Q $Source '--player-sword').equipped){I $Source 0 650 X;Start-Sleep -Milliseconds 2200}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json"
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 7 -Evidence "$out/observer-camera.json"
 $before=Q $Source '--player-darkvision';$before|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.managerStarted){throw 'Native Dark Vision already owned or active'}
 C $Source 'coop_darkvision_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew();do{$s=S $Source '.darkvision-fixture.json';if($s.result-eq'native_darkvision_ready'){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt6000)
 if($s.result-ne'native_darkvision_ready'){throw "Dark Vision did not equip: $($s.result)"}
 Copy-Item "$PSScriptRoot/darkvision-banks-v1/$character-darkvision-bank.bin" "$root/work/lab-instances/$peer/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $peer 'coop_movement_bank on'
 $w.Restart();do{$m=S $peer '.movement.json';if($m.active-and$m.version-eq25-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne25-or$m.weight-lt.9){throw 'Dark Vision bank did not become ready'}
 $checks.Add('Native Steam stream and exact v25 bank ready before the cast')
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
 foreach($r in @($peer,$Source)){
  $window=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$window){throw 'No game window'};$suffix=if($r-eq$peer){''}else{'-source'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take$suffix.mp4")
  $recorders+=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture$suffix.log" -RedirectStandardOutput "$out/capture$suffix.stdout" -PassThru
 }
 Start-Sleep -Milliseconds 700;if(@($recorders|Where-Object HasExited).Count){throw 'Recorder failed'}
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$times=@(1000,4500);$seen=@{};$nativeScenes=@{};$activated=$false;$deactivated=$false
 while($watch.ElapsedMilliseconds-lt9500){
  if($next-lt$times.Count-and$watch.ElapsedMilliseconds-ge$times[$next]){I $Source 255 250;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native LT press 250ms';index=$next});$next++}
  $native=Q $Source '--player-states';$power=Q $Source '--player-darkvision';$m=S $peer '.movement.json';$stream=S $peer '.shadow-stream.json';$peerPower=Q $peer '--player-darkvision'
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;native=$native;power=$power;movement=$m;stream=$stream;source=(S $Source);target=(S $peer);peer=$peerPower})
  if(!$m.active-or!$stream.armed-or!$stream.ready){throw "Power presentation retired: $($stream.result)"}
  if($native.powerSceneValid-and$native.powerScene.variant-eq10){$nativeScenes[[string]$native.powerScene.handle]=$true}
  if($m.powerVariant-eq10-and$m.powerWeight-gt.5){$seen[[string]$m.powerEvent]=$true}
  if($power.managerStarted){$activated=$true}elseif($activated-and$next-eq2){$deactivated=$true}
  if($peerPower.quantity-or$peerPower.managerStarted){throw 'Observer received caster-only Dark Vision'}
  if(!(S $Source '.darkvision-fixture.json').active){throw 'Dark Vision fixture retired during capture'};Start-Sleep -Milliseconds 25
 }
 if($seen.Count-ne2){throw "Expected two independently identified remote casts, found $($seen.Count)"};$checks.Add('Two native cast events drive remote whole-body Dark Vision animation through Steam')
 if(!$activated-or!$deactivated){throw 'Native Dark Vision did not toggle on and off'};$checks.Add('Native source vision overlay manager toggles on/off; observer vision and loadout remain untouched')
 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Cast failed to recover to standing'};$checks.Add('Both casts recover to standing with network body ownership retained')
 $sword=Q $peer '--avatar-sword' $handles[$peer];if(!(Q $Source '--player-sword').equipped-or!$sword.equipped){throw 'Native sword equipment lost'};$sword|ConvertTo-Json -Depth 5|Set-Content -Encoding utf8 "$out/sword-after.json";$checks.Add('First-person sword and remote native sword remain equipped')
 I $Source;C $Source 'coop_darkvision_fixture off';$w.Restart();do{$s=S $Source '.darkvision-fixture.json';if(!$s.active){break};Start-Sleep -Milliseconds 80}while($w.ElapsedMilliseconds-lt32000)
 if($s.active-or$s.result-ne'quantity_selection_and_effect_inactive'){throw "Dark Vision cleanup unconfirmed: $($s.result)"}
 $after=Q $Source '--player-darkvision';$after|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/after.json";if($after.quantity-or$after.equipped-or$after.started-or$after.managerStarted){throw 'Dark Vision cleanup incomplete'};$checks.Add('Native cleanup restores quantity, selection and inactive vision')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Native health changed'}};$checks.Add('Both players and remote avatars retain native health; no animation damage events')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}};$checks.Add('Protected original executable, Steam API and save hashes unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_darkvision_fixture off';Start-Sleep -Milliseconds 1500;C $r 'coop_movement_fixture off'}catch{}}
 foreach($capture in $recorders){$capture.WaitForExit(16000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Native capture incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);source=$Source;character=$character;failure=$failure;checks=$checks;events=$events;seenEvents=@($seen.Keys);nativeScenes=@($nativeScenes.Keys);dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bankSha256=(Get-FileHash "$PSScriptRoot/darkvision-banks-v1/$character-darkvision-bank.bin").Hash;recorded=($recorders.Count-eq2);scope='Native armed standing Dark Vision on/off casts, source POV and remote full-body observer. Native source retains vision overlay/reveal, controls, camera and sword. Pose carries no effects or gameplay events. Crouched/moving/airborne casts and broad lifecycle remain unverified.'}|ConvertTo-Json -Depth 7|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"PASS_DARKVISION_INTEGRATION $Source $($checks.Count) checks"
