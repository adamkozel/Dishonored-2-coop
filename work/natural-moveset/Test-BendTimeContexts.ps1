param([ValidateSet('host')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='bendtime-corvo-context-v41-v1',[switch]$Crouched,[switch]$Holstered,[switch]$Moving,[ValidatePattern('^[a-z0-9-]+\.json$')][string]$BankManifest='bendtime-context-bank-manifest-v41.json')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$peer=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$recorders=@();$start=0
$entry=Get-Content "$PSScriptRoot/$BankManifest" -Raw|ConvertFrom-Json|Where-Object role -eq $character;$bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw "Bank hash mismatch"}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$trigger=0,$ms=100,$button='Neutral',[double]$walk=0){if([Math]::Abs($walk)-gt1){throw 'Expected normalized walking input'};$stick=[int][Math]::Round($walk*32767);& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $trigger -LeftY $stick|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_power_fixture off','coop_bendtime_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 C $Source 'setviewpos -33.5 -153 68.1 90 0';C $peer 'setviewpos -30.7 -151.8 68.1 203 0'
 if((Q $Source '--player-states').stance-ne[int][bool]$Crouched){I $Source 0 100 B;Start-Sleep -Milliseconds 700}
 if((Q $Source '--player-sword').equipped-eq[bool]$Holstered){$duration=if($Holstered){1500}else{650};I $Source 0 $duration X;Start-Sleep -Milliseconds 2300}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json"
 $cameraPitch=if($Crouched){21}else{15}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch $cameraPitch -Evidence "$out/observer-camera.json"
 $before=Q $Source '--player-bendtime';$before|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Native BendTime already owned or active'}
 C $Source 'coop_bendtime_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew();do{$s=S $Source '.bendtime-fixture.json';if($s.result-eq'native_bendtime_ready'){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt6000)
 if($s.result-ne'native_bendtime_ready'){throw "BendTime did not equip: $($s.result)"}
 Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $peer 'coop_movement_bank on'
 $w.Restart();do{$m=S $peer '.movement.json';if($m.active-and$m.version-eq39-and$m.weight-gt.9-and$m.presentedDrawn-ne[bool]$Holstered-and[Math]::Abs($m.stanceMix-[int][bool]$Crouched)-lt.02){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne39-or$m.weight-lt.9-or$m.presentedDrawn-eq[bool]$Holstered-or[Math]::Abs($m.stanceMix-[int][bool]$Crouched)-ge.02){[ordered]@{movement=$m;stream=(S $peer '.shadow-stream.json');source=(Q $Source '--player-states');sword=(Q $Source '--player-sword');peerState=(S $peer)}|ConvertTo-Json -Depth 12|Set-Content -Encoding utf8 "$out/ready-failure.json";throw 'BendTime context bank did not become ready'}
 $checks.Add('Native Steam stream and exact v39 stance/equipment bank ready before the cast')
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
 $trace=Start-Process -FilePath "$PSHOME/pwsh.exe" -ArgumentList @('-NoProfile','-File',('"'+"$PSScriptRoot/Trace-BendTimePresentation.ps1"+'"'),'-Take',$Take) -WindowStyle Hidden -RedirectStandardOutput "$out/trace.stdout" -RedirectStandardError "$out/trace.stderr" -PassThru
 foreach($r in @($peer,$Source)){
  $window=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$window){throw 'No game window'};$suffix=if($r-eq$peer){''}else{'-source'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take$suffix.mp4")
  $recorders+=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture$suffix.log" -RedirectStandardOutput "$out/capture$suffix.stdout" -PassThru
 }
 Start-Sleep -Milliseconds 700;if(@($recorders|Where-Object HasExited).Count){throw 'Recorder failed'}
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$times=@(1000,4500);$seen=@{};$nativeScenes=@{};$nativePhases=@{};$remotePhases=@{};$activated=$false;$deactivated=$false;$moveStage=0
 while($watch.ElapsedMilliseconds-lt9500){
  $ms=$watch.ElapsedMilliseconds
  if($Moving){
   if($moveStage-eq0-and$ms-ge350){I $Source 0 1400 Neutral .35;$moveStage=1}
   if($moveStage-eq1-and$ms-ge1400){I $Source 0 1200 Neutral .35;$moveStage=2}
   if($moveStage-eq2-and$ms-ge2700){I $Source;$moveStage=3}
   if($moveStage-eq3-and$ms-ge3700){I $Source 0 1300 Neutral -.35;$moveStage=4}
   if($moveStage-eq4-and$ms-ge4900){I $Source 0 1300 Neutral -.35;$moveStage=5}
   if($moveStage-eq5-and$ms-ge6300){I $Source;$moveStage=6}
  }
  if($next-lt$times.Count-and$ms-ge$times[$next]){$walk=if(!$Moving){0}elseif(!$next){.35}else{-.35};I $Source 255 350 Neutral $walk;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native LT press 350ms';index=$next});$next++}
  $native=Q $Source '--player-states';$m=S $peer '.movement.json';$stream=S $peer '.shadow-stream.json'
  if(!$power-or($rows.Count%4)-eq0){$power=Q $Source '--player-bendtime'}
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;native=$native;power=$power;movement=$m;stream=$stream;source=(S $Source);target=(S $peer);peer=$peerPower})
  if(!$m.active-or!$stream.armed-or!$stream.ready){throw "Power presentation retired: $($stream.result)"}
  if($native.powerSceneValid-and$native.powerScene.variant-in@(12,14)){$nativeScenes[[string]$native.powerScene.handle]=$true;$nativePhases[[string]$native.powerScene.variant]=$true}
  if($m.powerVariant-in@(12,14)-and$m.powerWeight-gt.5){$seen[[string]$m.powerEvent]=$true;$remotePhases[[string]$m.powerVariant]=$true};if($power.powerActive){$activated=$true}elseif($activated){$deactivated=$true}
  
  if(!(S $Source '.bendtime-fixture.json').active){throw 'BendTime fixture retired during capture'};Start-Sleep -Milliseconds 25
 }
 if($trace){$trace.WaitForExit(3000)|Out-Null;if(!$trace.HasExited-or$trace.ExitCode){throw 'Presentation trace did not finalize'}}
 $traceRows=Get-Content "$out/presentation-trace.json" -Raw|ConvertFrom-Json
 $seen=@{};$remotePhases=@{}
 foreach($e in $traceRows){if($e.active-and$e.ready-and$e.variant-in@(12,14)-and$e.weight-gt.5){$seen[[string]$e.event]=$true;$remotePhases[[string]$e.variant]=$true}}
 if($seen.Count-ne4-or$nativeScenes.Count-ne4-or$nativePhases.Count-ne2-or$remotePhases.Count-ne2-or!$activated-or!$deactivated){throw "Expected four independently identified native/remote start/release phases and native active/inactive cycle, found $($nativeScenes.Count)/$($seen.Count)"};$checks.Add('Two native toggles produce four start/release phases driving the remote body through Steam')
 $peerPower=Q $peer '--player-darkvision';if($peerPower.quantity-or$peerPower.managerStarted){throw 'Observer native vision changed'}
 
 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0-or[Math]::Abs($m.stanceMix-[int][bool]$Crouched)-gt.02){throw 'Cast failed to recover to original stance'};$checks.Add('Both casts recover to original stance with network body ownership retained')
 $sword=Q $peer '--avatar-sword' $handles[$peer];if((Q $Source '--player-sword').equipped-eq[bool]$Holstered-or$sword.equipped-eq[bool]$Holstered){throw 'Native sword selection changed'};$sword|ConvertTo-Json -Depth 5|Set-Content -Encoding utf8 "$out/sword-after.json";$checks.Add('First-person and remote native sword retain the original drawn/holstered state')
 if(@($rows|Where-Object {$_.native.stance-ne[int][bool]$Crouched-or[Math]::Abs($_.movement.stanceMix-[int][bool]$Crouched)-gt.05}).Count){throw 'Cast changed native or remote stance'};$checks.Add('Full-body cast retains original native crouch/standing stance throughout')
 if($Moving){$travel=@($rows|Where-Object {$_.movement.powerWeight-gt.5-and$_.movement.speed-gt.1});if($travel.Count-lt2){throw 'Cast did not overlap confirmed travelling movement'};$phase=@($travel|ForEach-Object {$_.movement.phase}|Select-Object -Unique);if($phase.Count-lt2){throw 'Walking feet froze during the cast'};$checks.Add('Confirmed network travel continues to advance the gait during native casting')}
 I $Source;C $Source 'coop_bendtime_fixture off';$w.Restart();do{$s=S $Source '.bendtime-fixture.json';if(!$s.active){break};Start-Sleep -Milliseconds 80}while($w.ElapsedMilliseconds-lt32000)
 if($s.active-or$s.result-ne'quantity_and_selection_restored'){throw "BendTime cleanup unconfirmed: $($s.result)"}
 $after=Q $Source '--player-bendtime';$after|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/after.json";if($after.quantity-or$after.equipped-or$after.started-or$after.powerActive){throw 'BendTime cleanup incomplete'};$checks.Add('Native cleanup restores original power quantity and selection')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Native health changed'}};$checks.Add('Both players and remote avatars retain native health; no animation damage events')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}};$checks.Add('All 14 protected original settings/save and isolated checkpoint file hashes unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_bendtime_fixture off';Start-Sleep -Milliseconds 1500;if((Q $r '--player-states').stance-eq1){I $r 0 100 B;Start-Sleep -Milliseconds 700};C $r 'setviewpos -33 -153 68.1 90 0';if(!(Q $r '--player-sword').equipped){I $r 0 650 X;Start-Sleep -Milliseconds 2000};C $r 'coop_movement_fixture off'}catch{}}
 foreach($capture in $recorders){$capture.WaitForExit(16000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Native capture incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);source=$Source;character=$character;crouched=[bool]$Crouched;holstered=[bool]$Holstered;moving=[bool]$Moving;failure=$failure;checks=$checks;events=$events;seenEvents=@($seen.Keys);nativeScenes=@($nativeScenes.Keys);dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bankSha256=(Get-FileHash $bank).Hash;recorded=($recorders.Count-eq2);scope='Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified.'}|ConvertTo-Json -Depth 7|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"PASS_BENDTIME_INTEGRATION $Source $($checks.Count) checks"
