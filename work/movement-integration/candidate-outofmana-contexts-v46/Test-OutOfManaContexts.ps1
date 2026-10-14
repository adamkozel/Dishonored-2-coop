param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='outofmana-corvo-standing-armed-v45',[switch]$FrontView,[switch]$Crouched,[switch]$Holstered,[ValidateRange(100,1500)][int]$TriggerMs=250)
$captures=@();$trace=$null;$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$start=0
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$trigger=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $trigger|ForEach-Object {$script:lastInputSequence=$_.Sequence}}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
function S($r,$suffix=''){return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_power_fixture off','coop_darkvision_fixture off','coop_bendtime_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 C host 'setviewpos -33.5 -153 68.1 90 0';C client 'setviewpos -30.7 -151.8 68.1 203 0'
 if($FrontView){C client 'setviewpos -33.5 -150 68.1 270 0'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 15 -Evidence "$out/observer-camera.json"
 if((Q host '--player-states').stance-ne[int][bool]$Crouched){I host 0 100 B;Start-Sleep -Milliseconds 700}
 if((Q host '--player-sword').equipped-eq[bool]$Holstered){$duration=if($Holstered){1500}else{650};I host 0 $duration X;Start-Sleep -Milliseconds 2300}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 0 -Evidence "$out/camera.json"
 $before=Q host '--player-bendtime';$inventory=Q host '--player-inventory';$peerBefore=Q client '--player-inventory'
 $before|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/before.json";$inventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Native BendTime already owned or active'}
 C host 'coop_bendtime_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew();do{$s=S host '.bendtime-fixture.json';if($s.result-eq'native_bendtime_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($s.result-ne'native_bendtime_ready'){throw "BendTime did not equip: $($s.result)"}
 $power=Q host '--player-bendtime';$power|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'BendTime acquisition unconfirmed'}
 $checks.Add('Exact native Corvo BendTime equipment acquired and selected')
 $entry=Get-Content "$PSScriptRoot/outofmana-context-bank-manifest-v46.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo';$bank=Join-Path $PSScriptRoot $entry.bank
 if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank identity mismatch'}
 Copy-Item $bank "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';if($m.active-and$m.version-eq41-and$m.weight-gt.9-and$m.presentedDrawn-ne[bool]$Holstered-and[Math]::Abs($m.stanceMix-[int][bool]$Crouched)-lt.02){break};Start-Sleep -Milliseconds 35}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne41-or$m.weight-lt.9-or$m.presentedDrawn-eq[bool]$Holstered-or[Math]::Abs($m.stanceMix-[int][bool]$Crouched)-ge.02){throw 'Native v41 stance/equipment body not ready'}
 $checks.Add('Exact v41 stance/equipment bank active on the native Steam remote avatar')
 $avatar=(S client).probeActorHandle;$fxBefore=S client '.power-fx.json'
 $trace=Start-Process -FilePath "$PSHOME/pwsh.exe" -ArgumentList @('-NoProfile','-File',('"'+"$PSScriptRoot/Trace-BendTimePresentation.ps1"+'"'),'-Take',$Take,'-DurationMs','12500') -WindowStyle Hidden -RedirectStandardOutput "$out/trace.stdout" -RedirectStandardError "$out/trace.stderr" -PassThru
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
 foreach($r in 'host','client'){
 $window=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$window){throw 'No game window'};$suffix=if($r-eq'host'){'-source'}else{''}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take$suffix.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture$suffix.log" -RedirectStandardOutput "$out/capture$suffix.stdout" -PassThru
 $captures+=$capture
 }
 Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$times=@(1000,3000,6000,8000);$sceneCount=0
 while($watch.ElapsedMilliseconds-lt10500){
  if($next-lt$times.Count-and$watch.ElapsedMilliseconds-ge$times[$next]){if($next-in@(0,2)){$mana=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-before-attempt-$next.json";if($next-eq2){$noManaBefore=$mana}};I host 255 $TriggerMs;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action="Native LT press ${TriggerMs}ms";index=$next;ms=$watch.ElapsedMilliseconds;inputSequence=$script:lastInputSequence});$next++}
  $native=Q host '--player-states';$midnight=Q host '--midnight'
  $wind=@()
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=$native;midnight=$midnight;wind=$wind;power=(Q host '--player-bendtime');movement=(S client '.movement.json');stream=(S client '.shadow-stream.json')})
  if(($midnight|ConvertTo-Json -Depth 12 -Compress)-match'power_bendtime_cast'){$sceneCount++}
  if(!(S host '.bendtime-fixture.json').active){throw 'BendTime fixture retired during native capture'};Start-Sleep -Milliseconds 25
 }
 $firstMana=Get-Content "$out/mana-before-attempt-0.json" -Raw|ConvertFrom-Json;$secondMana=Get-Content "$out/mana-before-attempt-2.json" -Raw|ConvertFrom-Json
 $nativeCost=$firstMana.currentCandidate-$secondMana.currentCandidate
 if($nativeCost-le0-or$noManaBefore.currentCandidate-ge$nativeCost){throw 'Failed attempts lack measured insufficient-mana precondition'}
 $checks.Add('Normal native activation consumes mana; the next activation attempt has less than the measured expenditure')
 $failScenes=@($rows|Where-Object {$_.ms-ge$events[2].ms}|ForEach-Object {$_.midnight.instances}|Where-Object {$_.state-eq3-and$_.asset-eq'midnight/gameplay/player/base/powers/power_outofmana.midnightscene'}|Select-Object -ExpandProperty id -Unique)
 if($failScenes.Count-ne2){throw 'Two independently identified native out-of-mana scenes required'}
 $checks.Add('Both failed requests produce distinct native out-of-mana scenes')
 $late=@($rows|Where-Object {$_.ms-gt($events[-1].ms+350)})
 if($late.Count-lt3-or@($late|Where-Object {$_.power.powerActive}).Count){throw 'No-mana request unexpectedly activated the power or lacks observations'}
 if(@($late|Where-Object {@($_.midnight.instances|Where-Object {$_.state-eq3-and$_.asset-match'power_bendtime_cast'}).Count}).Count){throw 'No-mana request unexpectedly created a BendTime cast scene'}
 $loader=Get-Content "$root/work/lab-instances/host/loader.log" -Tail 2000
 foreach($event in $events){$line=@($loader|Where-Object {$_-match("Consumed simulated controller packet "+$event.inputSequence+"[.]")});if(!$line.Count){throw 'Native controller consumption not confirmed'};$line|Add-Content "$out/controller-acknowledgements.txt"}
 $checks.Add('All four controller requests are consumed; failed requests produce no power activation or BendTime scene')
 $noManaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-after-failed-attempt.json"
 if($noManaAfter.currentCandidate-lt$noManaBefore.currentCandidate){throw 'Failed attempt unexpectedly spent additional mana'}
 $checks.Add('Failed request spends no further measured native mana')
 if(!$sceneCount){throw 'No native BendTime cast scene observed'};$checks.Add('Normal controller casting produced native BendTime scene observations')
 $trace.WaitForExit(4000)|Out-Null;if(!$trace.HasExited-or$trace.ExitCode){throw 'Presentation recorder did not finish'}
 $tr=Get-Content "$out/presentation-trace.json" -Raw|ConvertFrom-Json
 $nativeFailures=@($rows|Where-Object {$_.player.powerSceneValid-and$_.player.powerScene.variant-eq15}|ForEach-Object {$_.player.powerScene.handle}|Select-Object -Unique)
 $remoteFailures=@($tr|Where-Object {$_.active-and$_.ready-and$_.variant-eq15-and$_.weight-gt.5}|ForEach-Object {$_.event}|Select-Object -Unique)
 if($nativeFailures.Count-ne2-or$remoteFailures.Count-ne2){throw "Expected two owned native/remote failures, observed $($nativeFailures.Count)/$($remoteFailures.Count)"}
 $checks.Add('Two distinct owned native failure scenes each drive one remote body event over Steam')
 $m=S client '.movement.json';$stream=S client '.shadow-stream.json';if(!$m.active-or$m.weight-lt.9-or$m.powerWeight-gt.02-or!$stream.ready){throw 'Failure did not recover to original native stance'}
 $sword=Q client '--avatar-sword' $avatar;$sword|ConvertTo-Json -Depth 6|Set-Content "$out/sword-after.json"
 if($sword.equipped-eq[bool]$Holstered-or(Q host '--player-sword').equipped-eq[bool]$Holstered){throw 'Failed cast changed sword state'}
 if(@($rows|Where-Object {$_.player.stance-ne[int][bool]$Crouched-or[Math]::Abs($_.movement.stanceMix-[int][bool]$Crouched)-gt.05}).Count){throw 'Failed cast changed native or remote stance'}
 $checks.Add('Every observed frame retains native and remote stance through failure and recovery')
 if((Q client '--health' $avatar).current-ne40){throw 'Failed cast damaged remote body'}
 $fxAfter=S client '.power-fx.json';if($fxAfter.started-ne$fxBefore.started-or$fxAfter.active){throw 'Failure created unsolicited observer effect'}
 $checks.Add('Remote failure recovers to original stance with original sword, health and no observer screen effect')
 I host;C host 'coop_bendtime_fixture off';$w.Restart();do{$s=S host '.bendtime-fixture.json';if(!$s.active){break};Start-Sleep -Milliseconds 80}while($w.ElapsedMilliseconds-lt7000)
 if($s.active-or$s.result-ne'quantity_and_selection_restored'){throw "BendTime cleanup unconfirmed: $($s.result)"}
 $after=Q host '--player-bendtime';$after|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/after.json"
 if($after.quantity-or$after.equipped-or$after.started-or$after.powerActive){throw 'BendTime remained owned, equipped or active'}
 $afterInventory=Q host '--player-inventory';$afterInventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-after.json"
 $a=@($inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});$b=@($afterInventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});if(!$a.Count-or(Compare-Object $a $b)){throw 'Inventory quantities changed after cleanup'}
 $peerAfter=Q client '--player-inventory';if(Compare-Object @($peerBefore.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($peerAfter.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Peer inventory changed'}
 $checks.Add('Native cleanup restores source inventory quantity/selection; peer inventory unchanged')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Native player health changed'}}
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}}
 $checks.Add('Both player health and protected files unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_bendtime_fixture off';Start-Sleep -Milliseconds 1500;C $r 'coop_movement_fixture off'}catch{}}
 foreach($capture in $captures){$capture.WaitForExit(16000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Native capture incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);source='host';noManaExperiment=$true;crouched=[bool]$Crouched;holstered=[bool]$Holstered;failure=$failure;checks=$checks;events=$events;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bankSha256=(Get-FileHash $bank).Hash;capture='Paired actual D2 source and remote body videos with native scene, Steam and presentation evidence';scope='Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged.'}|ConvertTo-Json -Depth 7|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"OBSERVED_NATIVE_BENDTIME $($rows.Count) samples"
