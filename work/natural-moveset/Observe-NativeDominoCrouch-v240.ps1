param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12,[switch]$AimGuard)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$capture=$null;$monitor=$null;$strictMonitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
function OptionalFx($r){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --domino-effects;$code=$LASTEXITCODE;if($code){return [pscustomobject]@{stable=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true;C client 'setviewpos -48.59 -155.07 68.1 90 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $before=Q client '--player-domino';$before|ConvertTo-Json -Depth 7|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Domino already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 $dirty=$true;C client 'coop_domino_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.domino-fixture.json';if($f.result-eq'native_domino_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($f.result-ne'native_domino_ready'){
  $f|ConvertTo-Json -Depth 6|Set-Content "$out/acquisition-fixture.json"
  $debugInv=Q client '--player-inventory';$debugInv|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-inventory.json"
  $debugItem=@($debugInv.items|Where-Object {$_.name-eq'models/powers/phantasmallink/phantasmal_link.inventoryitem'})
  if($debugItem.Count-eq1-and$debugItem[0].component-ne'0x0'){
   $obj=Q client '--object' $debugItem[0].component;$obj|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-component.json"
   Q client '--object' $before.entry|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-entry.json"
   foreach($field in '0x8','0x10'){$ptr=($obj.words|Where-Object offset -eq $field).word;if($ptr-and$ptr-ne'0x0'){Q client '--object' $ptr|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-owner-$field.json"}}
   Q client '--components' $debugItem[0].handle|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-components.json"
  }
  throw "Native Domino acquisition failed: $($f.result)"
 }
 $power=Q client '--player-domino';$power|ConvertTo-Json -Depth 7|Set-Content "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'Native Domino equipment not confirmed'}
 $checks.Add('Exact native Emily Domino item acquired and selected without directly invoking power or VFX')
 & "$lab/Send-D2Input.ps1" -Role client -Button LeftThumb -DurationMs 100|Out-Null
 Start-Sleep -Milliseconds 500
 $crouch=Q client '--player-states';$crouch|ConvertTo-Json -Depth 10|Set-Content "$out/crouch-before.json"
 if($crouch.stance-ne1-or!$crouch.motionValid-or$crouch.motion-ne0){throw 'Native crouch not grounded before observation'}
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $strictMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-scenes.ndjson" -RedirectStandardError "$out/domino-scenes.stderr" -PassThru
 $w.Restart();$cast=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   if($AimGuard){
    $catalog=Q client '--world-npcs';$catalog|ConvertTo-Json -Depth 8|Set-Content "$out/aim-npc-catalog.json"
    $eligible=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5-and[Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+155.07,2)-lt400}|Sort-Object {[Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+155.07,2)})
    if(!$eligible.Count){throw 'No verified upper-floor tutorial guard within twenty metres'}
    $target=$eligible[0];$yaw=[Math]::Atan2([double]$target.position[1]+155.07,[double]$target.position[0]+48.59)*180/[Math]::PI
    C client ('setviewpos -48.59 -155.07 68.1 {0} 0' -f $yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
    [ordered]@{target=$target;yaw=$yaw;placement='Unchanged proven source floor; only yaw points toward read-only verified native guard position'}|ConvertTo-Json -Depth 6|Set-Content "$out/aim-guard.json"
   }
   I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT held 1800ms then released'})
  }
  $scene=Q client '--midnight';$entities=Q client '--entities'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');midnight=$scene;power=(Q client '--player-domino');component=(Q client '--object' $power.component);effects=(OptionalFx client);entities=@($entities.entities|Where-Object {$_.definitionName-match'phantasmal'})})
  Start-Sleep -Milliseconds 25
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native scene watcher failed'}
 $rawSamples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $rawScenes=@($rawSamples|ForEach-Object {$_.instances}|Where-Object {$_.state-eq3-and$_.asset-like'*power_phantasmal_cast*'})
 [ordered]@{inputSent=$cast;sceneSamples=$rawScenes.Count;subscenes=@($rawScenes|Group-Object subscene|ForEach-Object {[ordered]@{name=$_.Name;samples=$_.Count;durations=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStates=@($rows.player);raw=$rawScenes}|ConvertTo-Json -Depth 16|Set-Content "$out/domino-observations.json"
 if(!$cast-or!$rawScenes.Count){throw 'No native Domino phase observed after controller input'}
 if(@($rawScenes|Where-Object {@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count}).Count){throw 'Observed Domino scene includes a nonreciprocal entity binding'}
 $checks.Add('Controller input produces recorded native PhantasmalLink scenes with reciprocal bindings; phase names and counts are observed, not inferred')
 $strictMonitor.WaitForExit(4000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){throw 'Strict Domino watcher failed'}
 $strict=@(Get-Content "$out/domino-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $nativeCrouch=@($rows|Where-Object {$_.player.stance-eq1-and$_.player.motionValid-and$_.player.motion-eq0-and@($_.player.active|Where-Object id -eq 72).Count})
 if($nativeCrouch.Count-lt3){throw 'Too few independent native crouched cast observations'}
 foreach($phase in 'Cast_In','Cast_Loop','Cast_Select_out'){if(!@($rawScenes|Where-Object subscene -eq $phase).Count){throw "Native crouched phase $phase not observed"}}
 if(@($strict|Where-Object valid).Count){throw 'Standing-only strict reader unexpectedly accepts unimplemented crouch context'}
 [ordered]@{rawSceneSamples=$rawScenes.Count;crouchedCastingSamples=$nativeCrouch.Count;productionAccepted=@($strict|Where-Object valid).Count;activeStateSets=@($nativeCrouch|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);scope='Native input/raw scene provenance only; no remote crouch binding or visual acceptance'}|ConvertTo-Json -Depth 8|Set-Content "$out/crouched-phase-summary.json"
 $checks.Add('Native crouch plus state72 independently recorded with raw In/Loop/Out phases and reciprocal bindings; current standing-only production reader correctly refuses this context')

 $checks.Add('Native player state, equipment, raw component/entity and filtered effect samples retained; mana change and targeting outcome remain observations')
 C client 'coop_domino_fixture off';$w.Restart()
 do{$f=S client '.domino-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Domino inventory did not retire'}
 foreach($r in 'host','client'){$after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json";if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'};if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
 $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_domino_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(18000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(18000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene watcher failed').TrimStart('; ')}}
 if($strictMonitor){$strictMonitor.WaitForExit(18000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){$failure=($failure+'; strict watcher failed').TrimStart('; ')}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$p=Q client '--player-domino';if($p.quantity-or$p.equipped-or$p.started){throw 'Domino remains after checkpoint'};$fx=Q client '--domino-effects';if(@($fx.effects).Count){throw 'Domino effects remain after checkpoint'};$checks.Add('Both isolated checkpoints restore clean health/inventory and no filtered native Domino effects remain')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Crouched native Domino observation (no remote binding): exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_DOMINO_OBSERVATION $($checks.Count) checks"
