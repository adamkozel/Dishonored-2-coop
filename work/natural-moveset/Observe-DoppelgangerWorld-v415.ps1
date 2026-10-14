param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=30)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$capture=$null;$monitor=$null;$worldMonitor=$null;$cloneCaptured=$false;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
function OptionalFx($r){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --doppelganger-effects;$code=$LASTEXITCODE;if($code){return [pscustomobject]@{stable=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_possession_fixture off','coop_doppelganger_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
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
 $before=Q client '--player-doppelganger';$before|ConvertTo-Json -Depth 8|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Doppelganger already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 C client 'coop_doppelganger_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.doppelganger-fixture.json';if($f.result-eq'native_doppelganger_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6500)
 if($f.result-ne'native_doppelganger_ready'){
  $f|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-fixture.json"
  $inv=Q client '--player-inventory';$inv|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-inventory.json"
  $item=@($inv.items|Where-Object name -eq 'models/powers/doppelganger/doppelganger.inventoryitem')
  if($item.Count-eq1-and$item[0].component-ne'0x0'){Q client '--object' $item[0].component|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-component.json";Q client '--components' $item[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-components.json"}
  throw "Native Doppelganger acquisition failed: $($f.result)"
 }
 $power=Q client '--player-doppelganger';$power|ConvertTo-Json -Depth 8|Set-Content "$out/enabled.json"
 if(!$power.instantiated-or!$power.started-or!$power.equipped-or$power.quantity-ne1){throw 'Exact native Doppelganger item not ready'}
 $checks.Add('Exact Emily Doppelganger item acquired through native inventory API; no direct spawn, AI or damage invocation')
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $worldMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-doppelganger-world','30000') -WindowStyle Hidden -RedirectStandardOutput "$out/world.ndjson" -RedirectStandardError "$out/world.stderr" -PassThru
 $w.Restart();$cast=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Emily LT1800 then release at grounded floor'})}
  $entities=Q client '--entities'

  $clones=@($entities.entities|Where-Object definitionName -eq 'models/powers/doppelganger/decoy_entity.def')
  if($clones.Count-eq1-and!$cloneCaptured){
   $cc=Q client '--components' $clones[0].handle;$cc|ConvertTo-Json -Depth 10|Set-Content "$out/clone-components.json"
   $dc=@($cc.components|Where-Object type -eq '.?AVarkCpntDecoy@@');if($dc.Count-ne1){throw 'Native clone lacks unique decoy component'}
   $obj=Q client '--object' $dc[0].pointer;$obj|ConvertTo-Json -Depth 12|Set-Content "$out/decoy-component.json"
   $dp=($obj.words|Where-Object offset -eq '0x8').word;Q client '--object' $dp|ConvertTo-Json -Depth 12|Set-Content "$out/decoy-declaration.json"
   Q client '--object' $clones[0].pointer|ConvertTo-Json -Depth 12|Set-Content "$out/clone-object.json"
   $cloneCaptured=$true
  }
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');sword=(Q client '--player-sword');power=(Q client '--player-doppelganger');component=(Q client '--object' $power.component);effects=(OptionalFx client);entities=@($entities.entities|Where-Object {$_.definitionName-match'(?i)(doppelganger|decoy_entity|deadlyshadow)'})})
  Start-Sleep -Milliseconds 30
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|ForEach-Object instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_decoy_cast*'})
 foreach($part in 'in','loop','out'){if(!@($scenes|Where-Object asset -eq "midnight/gameplay/player/base/powers/power_decoy_cast_$part.midnightscene").Count){throw "Native Doppelganger $part phase not observed"}}
 if(@($scenes|Where-Object {!$_.bindingsResearch.Count-or@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count}).Count){throw 'Native Doppelganger scene bindings not reciprocal'}
 [ordered]@{scenes=@($scenes|Group-Object asset,subscene|ForEach-Object {[ordered]@{scene=$_.Name;samples=$_.Count;durationMs=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStateSets=@($rows|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);observedEntityDefinitions=@($rows.entities.definitionName|Sort-Object -Unique);scope='Source-only native phase/lifecycle observation in isolated AI-held Dunwall; no remote gameplay or body binding'}|ConvertTo-Json -Depth 12|Set-Content "$out/doppelganger-observations.json"
 $checks.Add('Native input produces reciprocal Doppelganger entry/loop/out scenes with observed clocks and state sets')
 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/doppelganger-scenes.json"
 C client 'coop_doppelganger_fixture off';$w.Restart()
 do{$f=S client '.doppelganger-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 70}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Doppelganger inventory restoration unconfirmed'}
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or partner health changed'}
 }
 $checks.Add('Both original inventory quantities and player/partner health retained; world reset still required')
 $worldMonitor.WaitForExit(20000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){throw 'World observer did not finish'}
 $world=@(Get-Content "$out/world.ndjson"|ForEach-Object {$_|ConvertFrom-Json});$live=@($world|Where-Object {$_.valid-and$_.present})
 if($live.Count-lt20){throw 'Exact equipment/clone ownership not observed in twenty samples'}
 if(@($live.handle|Sort-Object -Unique).Count-ne1-or@($live.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Native clone/equipment incarnation changed unexpectedly'}
 $emptyAfter=@($world|Where-Object {$_.valid-and!$_.present-and$_.tick-gt$live[-1].tick})
 if(!$emptyAfter.Count){throw 'Native clone absence after fixture retirement not observed'}
 [ordered]@{presentSamples=$live.Count;afterAbsent=$emptyAfter.Count;states=@($live.stateCandidate|Sort-Object -Unique);handles=@($live.handle|Sort-Object -Unique);refusalStages=@($world|Where-Object {!$_.valid}|Group-Object stage|Select-Object Name,Count);maxReadMs=($world.readMs|Measure-Object -Maximum).Maximum;scope='Read-only reciprocal equipment/array/entity/component ownership. Raw decoy state has no inferred meaning; expiry versus fixture retirement is not conflated.'}|ConvertTo-Json -Depth 8|Set-Content "$out/clone-ownership-observations.json"
 $checks.Add('Exact equipment array, cloned NPC and owned native Decoy component agree in repeated live samples, then report absence after fixture cleanup')

}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_doppelganger_fixture off'}catch{}}
 foreach($job in @($capture,$monitor,$worldMonitor)){if($job){$job.WaitForExit(18000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q client '--player-doppelganger';if($p.quantity-or$p.equipped-or$p.started){throw 'Doppelganger remains acquired after checkpoint'}
  $entities=Q client '--entities';$entities|ConvertTo-Json -Depth 8|Set-Content "$out/entities-restored.json"
  $partnerHandle='0x{0:x}' -f [uint32](S client).probeActorHandle
  $restoredClones=@($entities.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\.def$'-and$_.handle-ne$partnerHandle});if($restoredClones.Count){throw 'Unexpected native clone remains after checkpoint'}
  $checks.Add('Both isolated checkpoints restore original health/inventory and retire spawned Doppelganger entities')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Native source Emily Doppelganger acquisition, controller input, raw scene clocks/state/equipment and spawned entity observations. AI held in the isolated Dunwall fixture; neither general doppelganger behavior nor remote movement/VFX/gameplay verified. Checkpoint and protected save restoration mandatory.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_DOPPELGANGER_OBSERVATION $($checks.Count) checks"
