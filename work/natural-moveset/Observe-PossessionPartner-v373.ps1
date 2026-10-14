param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12,[ValidateSet('standing','crouched')][string]$Stance='standing',[switch]$Holstered)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$capture=$null;$monitor=$null;$ownedMonitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_possession_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true
 C host 'setviewpos -33.5 -153 68.1 90 0';C client 'setviewpos -33.5 -150 68.1 270 0';Start-Sleep -Milliseconds 1000
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground=Q host '--player-states';$ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
 if(!$ground.motionValid-or$ground.motion-ne0-or$ground.stance-ne0-or[Math]::Abs([double]$ground.position[2]-66.4)-gt.1){throw 'Corvo is not grounded on measured test floor'}
 $before=Q host '--player-possession';$before|ConvertTo-Json -Depth 8|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Possession already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-before.json"
 C host 'coop_possession_fixture human40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S host '.possession-fixture.json';if($f.result-eq'native_possession_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6500)
 if($f.result-ne'native_possession_ready'){
  $f|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-fixture.json"
  $inv=Q host '--player-inventory';$inv|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-inventory.json"
  $item=@($inv.items|Where-Object name -eq 'models/powers/possession/possession.inventoryitem')
  if($item.Count-eq1-and$item[0].component-ne'0x0'){Q host '--object' $item[0].component|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-component.json";Q host '--components' $item[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-components.json"}
  throw "Native Possession acquisition failed: $($f.result)"
 }
 $power=Q host '--player-possession';$power|ConvertTo-Json -Depth 8|Set-Content "$out/enabled.json"
 if(!$power.instantiated-or!$power.started-or!$power.equipped-or$power.quantity-ne1){throw 'Exact native Possession item not ready'}
 $checks.Add('Exact Corvo Possession item acquired through native inventory API; no direct spawn, AI or damage invocation')
 $upgrade=@((Q host '--player-inventory').items|Where-Object name -eq 'components/characters/player/base/attributes/possession_parasite.inventoryitem')
 if($upgrade.Count-ne1-or$upgrade[0].quantity-ne1){throw 'Native human upgrade missing'}
 $partner=S host;$partner|ConvertTo-Json -Depth 8|Set-Content "$out/partner-before.json"
 if([Math]::Abs([double]$partner.probeRenderPosition[0]+33.5)-gt.15-or[Math]::Abs([double]$partner.probeRenderPosition[1]+150)-gt.15){throw 'Partner not in measured native aim corridor'}
 Q host '--components' $partner.probeActorHandle|ConvertTo-Json -Depth 12|Set-Content "$out/partner-components.json"
 $selected=[Collections.Generic.List[object]]::new()
 $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500}
 if($Holstered){
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
  & "$lab/Send-D2Input.ps1" -Role host -Button X -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1200
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/holster-look-return.json" -Quick
 }
 $context=Q host '--player-states';$sword=Q host '--player-sword'
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;native=$context;sword=$sword}|ConvertTo-Json -Depth 10|Set-Content "$out/context-before.json"
 if($context.stance-ne$expectedStance-or!$context.motionValid-or$context.motion-ne$expectedStance-or$sword.equipped-eq[bool]$Holstered){throw 'Requested native grounded equipment context not established'}
 $checks.Add('Requested native stance/sword context confirmed before input; cast may natively change stance')
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','11','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-midnight','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $ownedMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-scenes','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/owned-scenes.ndjson" -RedirectStandardError "$out/owned-scenes.stderr" -PassThru
 $w.Restart();$cast=$false;$cancelled=$false
 while($w.ElapsedMilliseconds-lt9000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I host 255 8000;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Corvo LT held toward exact partner, cancellation before release'})}
  if($cast-and!$cancelled-and$w.ElapsedMilliseconds-ge3600){
   $state=Q host '--player-states'
   if(!$state.defaultMode-or$state.active.id-notcontains63){throw 'Native aim scene absent before cancellation'}
   & "$lab/Send-D2Input.ps1" -Role host -Button B -LeftTrigger 255 -DurationMs 150|Out-Null
   Start-Sleep -Milliseconds 200;I host;$cancelled=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native B cancel while LT held, then neutral'})
  }
  $cp=Q host '--object' $power.component
  $targetPointer=($cp.words|Where-Object offset -eq '0xc0').word
  $targetObject=if($targetPointer-and$targetPointer-ne'0x0'){Q host '--object' $targetPointer}else{$null}
  $selected.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;component=$cp;target=$targetObject;partnerHandle=(S host).probeActorHandle;partnerActor=(S host).probeActor})

  $entities=Q host '--entities'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q host '--player-states');sword=(Q host '--player-sword');power=(Q host '--player-possession');component=(Q host '--object' $power.component);entities=@($entities.entities|Where-Object {$_.definitionName-match'(?i)(possession|rat)'})})
  Start-Sleep -Milliseconds 30
 }
 I host;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|ForEach-Object instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_possession_cast*'})
 if(!$scenes.Count){throw 'No native Possession cast scene observed'}
 $ownedMonitor.WaitForExit(4000)|Out-Null;if(!$ownedMonitor.HasExited-or$ownedMonitor.ExitCode){throw 'Strict Possession watcher incomplete'}
 $owned=@(Get-Content "$out/owned-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $accepted=@($owned|Where-Object {$_.valid})
 if(@($accepted|Where-Object {$_.armHandle-le1-or$_.equipmentHandle-ne$power.handle}).Count){throw 'Accepted reader sample has wrong native owner'}
 $accepted|ConvertTo-Json -Depth 8|Set-Content "$out/accepted-scenes.json"
 $incomplete=@($scenes|Where-Object {$_.bindingsResearch.Count-ne1-or@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count})
 [ordered]@{rawActive=$scenes.Count;incompleteRaw=$incomplete.Count;strictAccepted=$accepted.Count;strictRejected=($owned.Count-$accepted.Count);scope='Raw observations may catch incomplete loop initialization. Only double-validated arm/equipment scenes are accepted.'}|ConvertTo-Json|Set-Content "$out/reader-summary.json"
 $selected|ConvertTo-Json -Depth 14|Set-Content "$out/selected-target-observations.json"
 $checks.Add('Native selected-target references and partner component membership recorded; eligibility remains an observation, not an exclusion claim')
 $checks.Add('Raw native scenes and accepted/rejected ownership reader samples recorded without assuming context support')
 [ordered]@{scenes=@($scenes|Group-Object asset,subscene|ForEach-Object {[ordered]@{scene=$_.Name;samples=$_.Count;durationMs=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStateSets=@($rows|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);observedEntityDefinitions=@($rows.entities.definitionName|Sort-Object -Unique);scope='Source-only native phase/lifecycle observation in isolated AI-held Dunwall; no remote gameplay or body binding'}|ConvertTo-Json -Depth 12|Set-Content "$out/possession-observations.json"
 $checks.Add('Native input produces reciprocal Possession cast scenes; observed phase set recorded without assuming complete coverage')
 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/possession-scenes.json"
 if(!$cancelled){throw 'Native cancellation not sent'}
 $final=Q host '--player-states';if(!$final.defaultMode-or$final.stance-ne0){throw 'Native cancel did not retain standing player'}
 C host 'coop_possession_fixture off';$w.Restart()
 do{$f=S host '.possession-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 70}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Possession inventory restoration unconfirmed'}
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or partner health changed'}
 }
 $checks.Add('Both original inventory quantities and player/partner health retained; world reset still required')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_possession_fixture off'}catch{}}
 foreach($job in @($capture,$monitor,$ownedMonitor)){if($job){$job.WaitForExit(18000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 if($selected){$selected|ConvertTo-Json -Depth 14|Set-Content "$out/selected-target-observations.json"}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q host '--player-possession';if($p.quantity-or$p.equipped-or$p.started){throw 'Possession remains acquired after checkpoint'}
  foreach($r in 'host','client'){
   $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-checkpoint-restored-$r.json"
   if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete checkpoint inventory differs'}
  }
  $checks.Add('Both isolated checkpoints restore original health/inventory and clear the temporary Possession acquisition')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='host';stance=$Stance;holstered=[bool]$Holstered;checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Native source Corvo Possession acquisition, controller input, raw scene clocks/state/equipment and candidate target/entity observations. AI held in the isolated Dunwall fixture; Partner aim observation with native human upgrade and B cancellation before release. No partner exclusion implemented by this test. Existing standing human-target remote evidence is separate; no new remote context or gameplay claim. Checkpoint and protected save restoration mandatory.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_POSSESSION_OBSERVATION $($checks.Count) checks"
