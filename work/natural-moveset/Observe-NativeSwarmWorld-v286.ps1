param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=30,[ValidateSet('standing','crouched')][string]$Stance='standing',[switch]$Holstered)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$capture=$null;$monitor=$null;$holsterMonitor=$null;$worldMonitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true
 C host 'setviewpos -48.59 -155.07 68.1 90 0';Start-Sleep -Milliseconds 800
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground=Q host '--player-states';$ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
 if(!$ground.motionValid-or$ground.motion-ne0-or$ground.stance-ne0-or[Math]::Abs([double]$ground.position[2]-66.4370575)-gt.03){throw 'Corvo is not grounded on measured test floor'}
 $before=Q host '--player-swarm';$before|ConvertTo-Json -Depth 8|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Swarm already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-before.json"
 C host 'coop_swarm_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S host '.swarm-fixture.json';if($f.result-eq'native_swarm_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6500)
 if($f.result-ne'native_swarm_ready'){
  $f|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-fixture.json"
  $inv=Q host '--player-inventory';$inv|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-inventory.json"
  $item=@($inv.items|Where-Object name -eq 'models/powers/devouring_swarm/devouring_swarm.inventoryitem')
  if($item.Count-eq1-and$item[0].component-ne'0x0'){Q host '--object' $item[0].component|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-component.json";Q host '--components' $item[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-components.json"}
  throw "Native Swarm acquisition failed: $($f.result)"
 }
 $power=Q host '--player-swarm';$power|ConvertTo-Json -Depth 8|Set-Content "$out/enabled.json"
 if(!$power.instantiated-or!$power.started-or!$power.equipped-or$power.quantity-ne1){throw 'Exact native Swarm item not ready'}
 $checks.Add('Exact Corvo Swarm item acquired through native inventory API; no direct spawn, AI or damage invocation')
 $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500}
 if($Holstered){
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
  $holsterMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-midnight','5000') -WindowStyle Hidden -RedirectStandardOutput "$out/holster-midnight.ndjson" -RedirectStandardError "$out/holster-midnight.stderr" -PassThru
  $holsterRows=[Collections.Generic.List[object]]::new();$holsterClock=[Diagnostics.Stopwatch]::StartNew();Start-Sleep -Milliseconds 100
  & "$lab/Send-D2Input.ps1" -Role host -Button X -DurationMs 1000|Out-Null
  while($holsterClock.ElapsedMilliseconds-lt2600){$holsterRows.Add([pscustomobject]@{ms=$holsterClock.ElapsedMilliseconds;states=(Q host '--player-states');sword=(Q host '--player-sword')});Start-Sleep -Milliseconds 20}
  $holsterRows|ConvertTo-Json -Depth 14|Set-Content "$out/holster-states.json"
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/holster-look-return.json" -Quick
 }
 $context=Q host '--player-states';$sword=Q host '--player-sword'
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;native=$context;sword=$sword}|ConvertTo-Json -Depth 10|Set-Content "$out/context-before.json"
 if($context.stance-ne$expectedStance-or!$context.motionValid-or$context.motion-ne$expectedStance-or$sword.equipped-eq[bool]$Holstered){throw 'Requested native grounded equipment context not established'}
 $checks.Add('Requested native stance/sword context confirmed before input; cast may natively change stance')
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $worldMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-swarm-world','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/swarm-world.ndjson" -RedirectStandardError "$out/swarm-world.stderr" -PassThru
 Start-Sleep -Milliseconds 100;if($worldMonitor.HasExited){throw 'World watcher exited before cast'}
 $w.Restart();$cast=$false;$worldSnapshots=0
 while($w.ElapsedMilliseconds-lt14000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I host 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Corvo LT1800 then release at grounded floor'})}
  $entities=Q host '--entities'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q host '--player-states');sword=(Q host '--player-sword');power=(Q host '--player-swarm');component=(Q host '--object' $power.component);entities=@($entities.entities|Where-Object {$_.definitionName-match'(?i)(devouring_swarm|rat_swarm)'})})
  if($w.ElapsedMilliseconds-ge5000-and$worldSnapshots-lt2){
   $swarm=@($entities.entities|Where-Object {$_.definitionName-eq'models/characters/animals/rat/devouring_swarm.def'})
   if($swarm.Count-eq1){
    $components=Q host '--components' $swarm[0].handle
    $rat=@($components.components|Where-Object vtableRva -eq '0x1c42ab8')
    if($rat.Count-ne1-or$rat[0].owner-ne$swarm[0].pointer){
     # Preserve raw identities rather than authorizing a guessed ownership link.
     [ordered]@{entity=$swarm[0];components=$components;note='Raw ownership fields need inspection'}|ConvertTo-Json -Depth 12|Set-Content "$out/world-identity-$worldSnapshots.json"
    }
    if($rat.Count-eq1){
     $object=Q host '--object' $rat[0].pointer
     $worldObject=Q host '--object' $swarm[0].pointer
     $declPointer=@($object.words|Where-Object offset -eq '0x8')[0].word
     $declObject=Q host '--object' $declPointer
     [ordered]@{entity=$worldObject;component=$object;declaration=$declObject}|ConvertTo-Json -Depth 12|Set-Content "$out/world-declaration-$worldSnapshots.json"

     $array=@($object.words|Where-Object offset -eq '0x68')
     $agents=if($array.Count-eq1-and$array[0].word-ne'0x0'){Q host '--object' $array[0].word}else{$null}
     $liveRat=@($object.words|Where-Object offset -eq '0x80');$livePrefix=if($liveRat.Count-eq1-and$liveRat[0].pointerType-eq'.?AVarkRat@@'){Q host '--object' $liveRat[0].word}else{$null}
     $pipelineLink=@($object.words|Where-Object offset -eq '0x138')
     $pipeline=$null;$transforms=$null;$secondary=$null;$agentObjects=@()
     if($pipelineLink.Count-eq1-and$pipelineLink[0].pointerType-eq'.?AVarkRatFxPipeline@@'){
      $pipeline=Q host '--object' $pipelineLink[0].word
      $transformLink=@($pipeline.words|Where-Object offset -eq '0x138')
      $secondaryLink=@($pipeline.words|Where-Object offset -eq '0x170')
      if($transformLink.Count-eq1-and$transformLink[0].word-ne'0x0'){$transforms=Q host '--object' $transformLink[0].word}
      if($secondaryLink.Count-eq1-and$secondaryLink[0].word-ne'0x0'){$secondary=Q host '--object' $secondaryLink[0].word}
     }
     $indexLink=@($object.words|Where-Object offset -eq '0x90');$indexCountLink=@($object.words|Where-Object offset -eq '0x98')
     $indexPrefix=$null;$candidateIndices=@()
     if($indexLink.Count-eq1-and$indexLink[0].word-ne'0x0'-and$indexCountLink.Count-eq1){
      $indexCount=[int]([Convert]::ToUInt64($indexCountLink[0].word.Substring(2),16)-band[uint64]16777215)
      if($indexCount-gt0-and$indexCount-le60){
       $indexPrefix=Q host '--object' $indexLink[0].word
       foreach($word in $indexPrefix.words){$raw=[Convert]::ToUInt64($word.word.Substring(2),16);$candidateIndices+=[uint32]($raw-band[uint64]4294967295);$candidateIndices+=[uint32]($raw-shr32);if($candidateIndices.Count-ge$indexCount){break}}
       $candidateIndices=@($candidateIndices|Select-Object -First $indexCount)
      }
     }
     [ordered]@{entity=$swarm[0];components=$components;ratComponent=$object;agentPrefix=$agents;liveRatPrefix=$livePrefix;pipeline=$pipeline;pipelineTransforms=$transforms;pipelineSecondary=$secondary;indexPrefix=$indexPrefix;candidateIndices=$candidateIndices;note='Raw identity snapshot saved before bounded live-agent traversal; no mutation'}|ConvertTo-Json -Depth 16|Set-Content "$out/world-components-$worldSnapshots-prefix.json"
     if($livePrefix){
      $ratBase=[Convert]::ToUInt64($livePrefix.pointer.Substring(2),16)
      foreach($agentIndex in $candidateIndices){
       if(@($agentObjects|Where-Object activeLinks).Count-ge3){break}
       if($agentIndex-ge4096){$agentObjects+=[pscustomobject]@{index=$agentIndex;activeLinks=$false;note='Candidate index exceeds research bound; no traversal'};continue}
       $agent=if(!$agentIndex){$livePrefix}else{Q host '--object' ('0x'+($ratBase+[uint64]($agentIndex*0x108)).ToString('x'))}
       if($agent.type-ne'.?AVarkRat@@'){$agentObjects+=[pscustomobject]@{index=$agentIndex;activeLinks=$false;rat=$agent;note='Non-rat slot stops bounded traversal'};break}
       $entityLink=@($agent.words|Where-Object offset -eq '0x10');$frameLink=@($agent.words|Where-Object offset -eq '0x8');$animLink=@($agent.words|Where-Object offset -eq '0x78')
       if($entityLink.Count-ne1-or$entityLink[0].pointerType-ne'.?AVarkSwarmAgentEntity@@'-or$frameLink.Count-ne1-or$frameLink[0].pointerType-ne'.?AVarkCpntReferenceFrame@@'){$agentObjects+=[pscustomobject]@{index=$agentIndex;activeLinks=$false;rat=$agent;note='Retained inactive or unrecognized pool slot; no pointer traversal'};continue}
       $entityObject=Q host '--object' $entityLink[0].word
       $frameObject=Q host '--object' $frameLink[0].word
       $handleWord=@($entityObject.words|Where-Object offset -eq '0xa8')[0].word
       $entityHandle=[uint32]([Convert]::ToUInt64($handleWord.Substring(2),16)-band[uint64]4294967295)
       $agentComponents=Q host '--components' $entityHandle
       $animObject=if($animLink.Count-eq1-and$animLink[0].pointerType-eq'.?AVarkCpntAnimRat@@'){Q host '--object' $animLink[0].word}else{$null}
       $agentObjects+=[pscustomobject]@{index=$agentIndex;activeLinks=$true;rat=$agent;entity=$entityObject;frame=$frameObject;components=$agentComponents;animation=$animObject}
      }
     }
     [ordered]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;entity=$swarm[0];components=$components;ratComponent=$object;agentPrefix=$agents;liveRatPrefix=$livePrefix;pipeline=$pipeline;pipelineTransforms=$transforms;pipelineSecondary=$secondary;indexPrefix=$indexPrefix;candidateIndices=$candidateIndices;liveAgentObjects=$agentObjects;note='Read-only static candidate +68 pointer/+70 stride/+78 count, first1024 bytes only; no world mutation or semantic claim'}|ConvertTo-Json -Depth 16|Set-Content "$out/world-components-$worldSnapshots.json"
    }
    $worldSnapshots++
   }
  }
  Start-Sleep -Milliseconds 30
 }
 I host;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|ForEach-Object instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_swarm_cast*'})
 foreach($part in 'in','loop','out'){if(!@($scenes|Where-Object asset -eq "midnight/gameplay/player/base/powers/power_swarm_cast_$part.midnightscene").Count){throw "Native Swarm $part phase not observed"}}
 # --watch-midnight is explicitly non-atomic. Preserve and bound terminal samples
 # where the reader saw state3 before teardown cleared the bindings and clock.
 $terminal=[Collections.Generic.List[object]]::new()
 for($i=0;$i-lt$samples.Count;$i++){
  foreach($scene in @($samples[$i].instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_swarm_cast*'})){
   if($scene.bindingsResearch.Count){
    if($scene.bindingsResearch.Count-ne2-or@($scene.bindingsResearch|Where-Object {!$_.reciprocal}).Count){throw 'Native Swarm populated bindings not reciprocal'}
    continue
   }
   if(!$i-or$i-ge($samples.Count-1)-or$scene.clockWords[0]-ne0){throw 'Unexplained empty Swarm bindings'}
   $prior=@($samples[$i-1].instances|Where-Object {$_.id-eq$scene.id-and$_.pointer-eq$scene.pointer-and$_.asset-eq$scene.asset-and$_.state-eq3})
   $next=@($samples[$i+1].instances|Where-Object {$_.id-eq$scene.id-and$_.pointer-eq$scene.pointer-and$_.state-eq3})
   if($prior.Count-ne1-or$prior[0].bindingsResearch.Count-ne2-or@($prior[0].bindingsResearch|Where-Object {!$_.reciprocal}).Count-or$next.Count-or($samples[$i].tick-$samples[$i-1].tick)-gt100-or($samples[$i+1].tick-$samples[$i].tick)-gt100){throw 'Empty Swarm bindings not a bounded terminal sample'}
   $terminal.Add([pscustomobject]@{tick=$samples[$i].tick;scene=$scene;preceding=$prior[0];note='Non-atomic terminal read: exact incarnation was reciprocal immediately before, zero clock/empty bindings, absent on next sample; never authorizes runtime playback'})
  }
 }
 @($terminal)|ConvertTo-Json -Depth 16|Set-Content "$out/non-atomic-terminal-samples.json"
 $scenes=@($scenes|Where-Object {$_.bindingsResearch.Count-eq2})

 [ordered]@{scenes=@($scenes|Group-Object asset,subscene|ForEach-Object {[ordered]@{scene=$_.Name;samples=$_.Count;durationMs=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStateSets=@($rows|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);observedEntityDefinitions=@($rows.entities.definitionName|Sort-Object -Unique);scope='Source-only native phase/lifecycle observation in isolated AI-held Dunwall; no remote gameplay or body binding'}|ConvertTo-Json -Depth 12|Set-Content "$out/swarm-observations.json"
 $checks.Add('Native input produces reciprocal Swarm entry/loop/out scenes with observed clocks and state sets')
 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/swarm-scenes.json"
 $worldMonitor.WaitForExit(4000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){throw 'World watcher failed'}
 $worldRows=@(Get-Content "$out/swarm-world.ndjson"|ForEach-Object{$_|ConvertFrom-Json})
 $liveWorld=@($worldRows|Where-Object{$_.valid-and$_.present-and$_.agents.Count-eq25})
 [ordered]@{samples=$worldRows.Count;validSamples=@($worldRows|Where-Object valid).Count;fullSwarmSamples=$liveWorld.Count;stages=@($worldRows|Group-Object stage|ForEach-Object{@{stage=$_.Name;count=$_.Count}});maximumReadMs=($worldRows.readMs|Measure-Object -Maximum).Maximum}|ConvertTo-Json -Depth 6|Set-Content "$out/world-reader-summary.json"
 if($liveWorld.Count-lt20){throw 'Fewer than20 strict full native Swarm snapshots'}
 foreach($sample in $liveWorld){if(@($sample.agents.handle|Sort-Object -Unique).Count-ne25-or@($sample.agents.index|Sort-Object -Unique).Count-ne25){throw 'Duplicate strict rat identity'}}
 $checks.Add('At least20 strict snapshots validate all25 indexed native rats, reciprocal entities/frames/animations and stable generations')

 C host 'coop_swarm_fixture off';$w.Restart()
 do{$f=S host '.swarm-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 70}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Swarm inventory restoration unconfirmed'}
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or partner health changed'}
 }
 $checks.Add('Both original inventory quantities and player/partner health retained; world reset still required')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_swarm_fixture off'}catch{}}
 foreach($job in @($capture,$monitor,$holsterMonitor,$worldMonitor)){if($job){$job.WaitForExit(18000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q host '--player-swarm';if($p.quantity-or$p.equipped-or$p.started){throw 'Swarm remains acquired after checkpoint'}
  $entities=Q host '--entities';if(@($entities.entities|Where-Object {$_.definitionName-match'^models/characters/animals/rat/devouring_swarm(_big)?\.def$'}).Count){throw 'Spawned swarm remains after checkpoint'}

  $empty=Q host '--swarm-world';$empty|ConvertTo-Json -Depth 8|Set-Content "$out/world-after-checkpoint.json"
  if(!$empty.valid-or$empty.present){throw 'Strict Swarm world reader retains a world after checkpoint'}
  $checks.Add('Both isolated checkpoints restore original health/inventory and retire spawned Swarm entities')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='host';stance=$Stance;holstered=[bool]$Holstered;checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Native source Corvo Swarm acquisition, controller input, raw scene clocks/state/equipment and spawned entity observations. AI held in the isolated Dunwall fixture; neither general swarm behavior nor remote movement/VFX/gameplay verified. Checkpoint and protected save restoration mandatory.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_SWARM_OBSERVATION $($checks.Count) checks"
