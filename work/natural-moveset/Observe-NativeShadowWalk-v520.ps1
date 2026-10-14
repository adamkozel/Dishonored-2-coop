param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=30)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$capture=$null;$monitor=$null;$strictMonitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
function O($r,$mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};$code=$LASTEXITCODE;if($code){return [pscustomobject]@{stable=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_possession_fixture off','coop_doppelganger_fixture off','coop_shadowwalk_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true
 C host 'setviewpos -34.3 -154.8 68.1 90 0'
 C client 'setviewpos -48.59 -155.07 68.1 90 0';Start-Sleep -Milliseconds 800
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground=Q client '--player-states';$ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
 if(!$ground.motionValid-or$ground.motion-ne0-or$ground.stance-ne0-or[Math]::Abs([double]$ground.position[2]-66.4370575)-gt.03){throw 'Emily is not grounded on measured test floor'}
 $baselineEntities=Q client '--entities';$baselineEntities|ConvertTo-Json -Depth 8|Set-Content "$out/entities-before.json"
 $before=Q client '--player-shadowwalk';$before|ConvertTo-Json -Depth 8|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'ShadowWalk already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 C client 'coop_shadowwalk_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.shadowwalk-fixture.json';if($f.result-eq'native_shadowwalk_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6500)
 if($f.result-ne'native_shadowwalk_ready'){
  $f|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-fixture.json"
  $inv=Q client '--player-inventory';$inv|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-inventory.json"
  $item=@($inv.items|Where-Object name -eq 'models/powers/shadow_walk/shadow_walk.inventoryitem')
  if($item.Count-eq1-and$item[0].component-ne'0x0'){Q client '--object' $item[0].component|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-component.json";Q client '--components' $item[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-components.json"}
  throw "Native ShadowWalk acquisition failed: $($f.result)"
 }
 $power=Q client '--player-shadowwalk';$power|ConvertTo-Json -Depth 8|Set-Content "$out/enabled.json"
 if(!$power.instantiated-or!$power.started-or!$power.equipped-or$power.quantity-ne1){throw 'Exact native ShadowWalk item not ready'}
 $checks.Add('Exact Emily Shadow Walk item acquired through native inventory API; no direct state transition or damage invocation')
 $context=Q client '--player-states';$sword=Q client '--player-sword'
 $context|ConvertTo-Json -Depth 10|Set-Content "$out/context-before.json"
 if($context.stance-ne0-or!$context.motionValid-or$context.motion-ne0-or!$sword.equipped){throw 'Native grounded armed context not established'}
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','28','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','27000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru

 Q client '--object' $power.component|ConvertTo-Json -Depth 14|Set-Content "$out/component-before.json"
 Q client '--components' $power.handle|ConvertTo-Json -Depth 14|Set-Content "$out/components-before.json"
 $strictMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-shadowwalk-state','27000') -WindowStyle Hidden -RedirectStandardOutput "$out/shadowwalk-state.ndjson" -RedirectStandardError "$out/shadowwalk-state.stderr" -PassThru
 $w.Restart();$cast=$false;$forward=$false;$back=$false;$entityNext=2000
 while($w.ElapsedMilliseconds-lt26000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I client 255 220;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Emily LT220 activation'})}
  $player=Q client '--player-states'
  $form=@($player.active|Where-Object vtableRva -eq '0x1c7ed28').Count-gt0
  if($form-and!$forward-and$w.ElapsedMilliseconds-ge4000){
   & "$lab/Send-D2Input.ps1" -Role client -LeftY 18000 -DurationMs 550|Out-Null;$forward=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Forward550ms only after observed exact native ShadowWalk state'})
  }
  if($form-and$forward-and!$back-and$w.ElapsedMilliseconds-ge5500){
   & "$lab/Send-D2Input.ps1" -Role client -LeftY -18000 -DurationMs 550|Out-Null;$back=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Backward550ms in observed native ShadowWalk state'})
  }
  $entities=$null;if($w.ElapsedMilliseconds-ge$entityNext){$entityNext=$w.ElapsedMilliseconds+2500;$entities=Q client '--entities'}
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=$player;midnight=(Q client '--midnight');sword=(O client '--player-sword');power=(O client '--player-shadowwalk');component=(O client '--object' $power.component);effects=(O client '--shadowwalk-effects');entities=@($entities.entities|Where-Object {$_.definitionName-match'(?i)(shadow_walk|shadowwalk)'})})
  Start-Sleep -Milliseconds 30
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|ForEach-Object instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_shadow_walk_*'})
 $forms=@($rows|Where-Object {@($_.player.active|Where-Object vtableRva -eq '0x1c7ed28').Count})
 $last=$rows[-1].player
 [ordered]@{scenes=@($scenes|Group-Object asset,subscene|ForEach-Object {[ordered]@{scene=$_.Name;samples=$_.Count;durationMs=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStateSets=@($rows|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);stances=@($rows.player.stance|Sort-Object -Unique);formSamples=$forms.Count;forwardInput=$forward;backwardInput=$back;observedEntityDefinitions=@($rows.entities.definitionName|Sort-Object -Unique);scope='Source-only native activation/form/movement/exit observation. No phase enum meaning, remote presentation or gameplay authority assumed.'}|ConvertTo-Json -Depth 14|Set-Content "$out/shadowwalk-observations.json"
 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/shadowwalk-scenes.json"
 if(!$forms.Count){throw 'Native ShadowWalk state not directly observed'}
 $checks.Add('Exact native ShadowWalk player state observed after normal LT input')
 if(!$scenes.Count){throw 'Native Shadow Walk scene not observed'}
 if(@($scenes|Where-Object {!$_.bindingsResearch.Count-or@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count}).Count){throw 'Observed Shadow Walk scene binding not reciprocal'}
 $checks.Add('Native Shadow Walk scenes observed with reciprocal bindings and recorded clocks')
 if(!$forward-or!$back){throw 'Bounded form movement inputs not exercised'}
 if(@($last.active|Where-Object vtableRva -eq '0x1c7ed28').Count-or!$last.defaultMode-or$last.stance-gt1-or[Math]::Abs([double]$last.position[2]-$context.position[2])-gt.3){throw 'Natural Shadow Walk exit to the same floor not confirmed'}
 $checks.Add('Bounded forward/backward inputs issued during exact form state; natural exit recovers normal mode on same floor')
 if($manaAfter.currentCandidate-ge$manaBefore.currentCandidate){throw 'Native mana expenditure not observed'}
 $checks.Add('Native mana decreases; replenishment prevents interpreting this as exact cost')
 $strictMonitor.WaitForExit(4000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){throw 'Shadow Walk mode watcher failed'}
 $strict=@(Get-Content "$out/shadowwalk-state.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $valid=@($strict|Where-Object valid);$phases=@($valid.phase|Sort-Object -Unique)
 foreach($phase in 0,2,3,4,5){if($phase-notin$phases){throw "Strict phase $phase not observed"}}
 $form=@($valid|Where-Object {$_.form-and$_.phase-eq4-and$_.stance-eq2})
 if(!$form.Count){throw 'Strict alternate native mode not observed'}
 $fx=@($rows|Where-Object {$_.effects.stable-ne$false}|ForEach-Object {$_.effects.records})
 [ordered]@{samples=$strict.Count;accepted=$valid.Count;phases=$phases;formSamples=$form.Count;effectNames=@($fx.name|Sort-Object -Unique);effectOwners=@($fx.owner|Sort-Object -Unique);scope='Read-only exact equipment, reciprocal state-machine owner, native alternate mode and phase guards. FX records independently enumerated; no attribution or remote binding inferred.'}|ConvertTo-Json -Depth 10|Set-Content "$out/strict-state-observations.json"
 $checks.Add('Strict native mode reader observes all recorded phases including exact alternate ShadowWalk form and retirement')

 C client 'coop_shadowwalk_fixture off';$w.Restart()
 do{$f=S client '.shadowwalk-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 70}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native ShadowWalk inventory restoration unconfirmed'}
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or partner health changed'}
 }
 $checks.Add('Both original inventory quantities and player/partner health retained; world reset still required')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_shadowwalk_fixture off'}catch{}}
 foreach($job in @($capture,$monitor,$strictMonitor)){if($job){$job.WaitForExit(18000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q client '--player-shadowwalk';if($p.quantity-or$p.equipped-or$p.started){throw 'ShadowWalk remains acquired after checkpoint'}
  $entities=Q client '--entities';$entities|ConvertTo-Json -Depth 8|Set-Content "$out/entities-restored.json"
  $checks.Add('Both isolated checkpoints restore original health/inventory and unowned Shadow Walk item')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Source-only native Shadow Walk acquisition, normal LT activation, conditional bounded movement, natural exit, scene/state/equipment observations. No remote form/VFX/gameplay binding. Checkpoint and protected saves must restore.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_SHADOWWALK_OBSERVATION $($checks.Count) checks"
