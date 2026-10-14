param([string]$Take='npc-perception-v974')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$inventory=@{};$samples=[Collections.Generic.List[object]]::new();$reloadCount=0
function Save($p,$v){$v|ConvertTo-Json -Depth 24|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=120){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null;Start-Sleep -Milliseconds ($ms+30)}
function S($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function N($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.npc-stream.json" -Raw|ConvertFrom-Json}
function ResetState($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.reset.json" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw "Native query failed $r $mode"}
function Ready($tag,[int]$count=0){
 $w=[Diagnostics.Stopwatch]::StartNew();do{$ok=$true;$pair=@{};foreach($r in 'host','client'){$v=N $r;$pair[$r]=$v;if(!$v.enabled-or!$v.session.enabled-or$v.session.menuSuspended-or$v.session.reloadPending-or($count-and$v.entries.Count-ne$count)-or!$v.entries.Count-or@($v.entries|Where-Object {!$_.enabled-or!$_.ready}).Count){$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 80}}while(!$ok-and$w.ElapsedMilliseconds-lt8000)
 Save "$out/ready-$tag.json" $pair;if(!$ok){throw "NPC channels not ready: $tag expected$count"};return $pair
}
function P($role='host'){Get-Content "$root/work/lab-instances/$role/coop-status.json.npc-perception.json" -Raw|ConvertFrom-Json}
function Factions($tag){
 $map=@{};$state=N host;$partner=(S host).probeActorHandle
 foreach($e in $state.entries){if($e.enabled){$map[[string]$e.handle]=Q host '--faction' $e.handle}}
 $map['player']=Q host '--faction' 1;$map['partner']=Q host '--faction' $partner
 Save "$out/factions-$tag.json" $map;return $map
}
function PerceptionReady($tag){
 $watch=[Diagnostics.Stopwatch]::StartNew();do{$p=P;if($p.enabled-and!$p.suspended-and$p.entries.Count-and$p.result-eq'active_host_perception'){break};Start-Sleep -Milliseconds 80}while($watch.ElapsedMilliseconds-lt6000)
 Save "$out/perception-$tag.json" $p;if(!$p.enabled-or$p.suspended-or!$p.entries.Count-or$p.lost-or$p.superseded){throw "Perception not ready $tag"};return $p
}
function AssertRelations($before,$after,$owned,[bool]$active){
 foreach($key in $before.Keys){
  $a=$before[$key];$b=$after[$key];if(!$b-or$a.actor-ne$b.actor-or$a.handle-ne$b.handle){throw "Faction identity changed $key"}
  foreach($r in $a.relations){
   $now=@($b.relations|Where-Object handle -eq $r.handle);if($now.Count-ne1){throw 'Faction relation membership changed'}
   $changed=$key-notin@('player','partner')-and@($owned.entries|Where-Object {[string]$_.npc-eq$key-and$_.partner-eq$r.handle}).Count-eq1
   if($active-and$changed){if(!$now[0].overridden-or$now[0].effective-ne2-or$now[0].overrideValue-ne2-or$now[0].base-ne$r.base){throw 'Owned hostility missing or faction base changed'}}
   elseif(($now[0]|ConvertTo-Json -Compress)-ne($r|ConvertTo-Json -Compress)){throw "Unowned/native relation changed $key -> $($r.handle)"}
  }
 }
}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v973/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Wrong candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[datetime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  $s=S $r;Save "$out/before-$r.json" $s;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){throw 'Gameplay required'}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
  Save "$out/health-before-$r.json" (Q $r '--health' 1);Save "$out/catalog-before-$r.json" (Q $r '--npc-authority')
 }
 $dirty=$true;foreach($r in 'host','client'){C $r 'coop_npc_stream on'};$initial=Ready 'initial';$initialCount=$initial.host.entries.Count
 $factionBefore=Factions 'baseline';$clientBefore=P client
 C client 'coop_npc_perception on';Start-Sleep -Milliseconds 700;$denied=P client;Save "$out/client-refused.json" $denied
 if($denied.enabled-or$denied.entries.Count-or$denied.acquired-or$denied.refused-ne$clientBefore.refused+1){throw 'Client perception mutation was not refused'}
 C host 'coop_npc_perception on';$ownedRelations=PerceptionReady 'initial';$factionActive=Factions 'active';AssertRelations $factionBefore $factionActive $ownedRelations $true
 C host 'coop_npc_perception on';Start-Sleep -Milliseconds 650;$duplicate=P;Save "$out/perception-duplicate.json" $duplicate
 if($duplicate.acquired-ne$ownedRelations.acquired-or$duplicate.refused-ne$ownedRelations.refused+1-or$duplicate.entries.Count-ne$ownedRelations.entries.Count){throw 'Duplicate perception acquired extra overrides'}
 $index=0
 foreach($source in 'host','client'){
  ++$index;I $source Start 180;$w=[Diagnostics.Stopwatch]::StartNew()
  do{$a=N host;$b=N client;if((S $source).localGameLogicState-eq2-and$a.session.menuSuspended-and$b.session.menuSuspended){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt4000)
  if(!$a.session.menuSuspended-or!$b.session.menuSuspended){throw 'Menu did not suspend both NPC sessions'}
  Start-Sleep -Milliseconds 400;$menu=@{host=(N host);client=(N client)};Save "$out/menu-$source-entered.json" $menu
  $w.Restart();do{
   $pair=@{host=(N host);client=(N client)};$samples.Add(@{phase="menu-$source";ms=$w.ElapsedMilliseconds;pair=$pair})
   foreach($r in 'host','client'){
    $v=$pair[$r];$old=$menu[$r];if(!$v.enabled-or!$v.session.enabled-or!$v.session.menuSuspended-or$v.entries.Count-ne$initialCount-or$v.bodyApplied-ne$old.bodyApplied){throw 'Menu changed ownership or applied a body'}
    foreach($e in $v.entries){$previous=@($old.entries|Where-Object arm -eq $e.arm);if($previous.Count-ne1-or$e.roots-ne$previous[0].roots-or$e.poses-ne$previous[0].poses-or$e.references-ne$previous[0].references){throw 'NPC mutation, send, or rearm during held menu'}}
   };Start-Sleep -Milliseconds 150
  }while($w.ElapsedMilliseconds-lt6200)
  Save "$out/menu-$source-held.json" $pair
  $held=P;Save "$out/perception-menu-$source.json" $held
  if(!$held.enabled-or!$held.suspended-or$held.entries.Count-or$held.acquired-ne$held.released-or$held.lost){throw 'Menu did not release all owned faction overrides'}
  $fm=Factions "menu-$source";AssertRelations $factionBefore $fm $ownedRelations $false
  I $source A 180;$resumed=Ready "resumed-$source";$ownedRelations=PerceptionReady "resumed-$source"
  $fr=Factions "resumed-$source";AssertRelations $factionBefore $fr $ownedRelations $true
  Start-Sleep -Milliseconds 1400
  foreach($r in 'host','client'){
   $v=N $r;Save "$out/menu-$source-resumed-$r.json" $v
   if($v.session.menuSuspensions-ne$index-or$v.session.menuResumes-ne$index-or$v.session.menuPausedMs-lt6000*$index){throw 'Menu accounting differs'}
   foreach($e in $v.entries){$old=@($initial[$r].entries|Where-Object handle -eq $e.handle)[0];$paused=@($menu[$r].entries|Where-Object handle -eq $e.handle)[0];if($e.arm-ne$old.arm-or$e.poses-le$paused.poses+10-or($r-eq'client'-and$e.roots-le$paused.roots)){throw 'Stream did not resume on retained arms'}}
   if($v.session.released-or$v.session.lost){throw 'Menu unnecessarily released or lost owned AI'}
  }
 }
 foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_checkpoint_sync on';C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})}
 $w.Restart();do{$h=ResetState host;$c=ResetState client;if($h.enabled-and$c.enabled-and$h.peerArm-and$c.peerArm-and!$h.phase-and!$c.phase-and$h.digest-eq$c.digest){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt5000)
 if(!$h.peerArm-or!$c.peerArm-or$h.digest-ne$c.digest){throw 'Checkpoint pair not ready'}
 $reload=@{};foreach($r in 'host','client'){$reload[$r]=@{reset=(ResetState $r);bridge=(S $r);npc=(N $r)};Save "$out/reload-before-$r.json" $reload[$r]}
 C host 'RestartFromCheckpoint';C client 'RestartFromMemoryCheckpoint'
 $w.Restart();$pressed=@{host=0;client=0}
 do{$ok=$true;$pair=@{};foreach($r in 'host','client'){
  $rs=ResetState $r;$s=S $r;$v=N $r;$pair[$r]=@{reset=$rs;bridge=$s;npc=$v}
  if($rs.phase-eq7){throw "Shared checkpoint failed: $r $($rs.result)"}
  if($rs.completed-ne$reload[$r].reset.completed+1-or$s.localGameLogicState-ne1-or$s.localPendingGameLogicState-or!$rs.avatarReady-or$rs.avatarPending-or!$v.enabled-or!$v.session.enabled-or$v.session.rearms-ne1-or@($v.entries|Where-Object {!$_.enabled-or!$_.ready}).Count){$ok=$false}
  if(!$rs.held-and$w.ElapsedMilliseconds-gt6000-and$s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-$pressed[$r]-gt700){I $r A 100;$pressed[$r]=$w.ElapsedMilliseconds}
 };$samples.Add(@{phase='shared-reload';ms=$w.ElapsedMilliseconds;pair=$pair});if(!$ok){Start-Sleep -Milliseconds 80}}while(!$ok-and$w.ElapsedMilliseconds-lt50000)
 Save "$out/reload-completed.json" $pair;if(!$ok){throw 'NPC session did not rebind after shared checkpoint'}
 $retired=P;Save "$out/perception-after-checkpoint.json" $retired
 if($retired.enabled-or$retired.entries.Count-or$retired.acquired-ne$retired.released-or$retired.lost){throw 'Old perception ownership survived checkpoint'}
 $freshCatalog=Q 'host' '--npc-authority';Save "$out/fresh-host-catalog.json" $freshCatalog;$reloadCount=$freshCatalog.entries.Count
 if($reloadCount-lt1-or$reloadCount-gt128){throw 'Fresh eligible host catalog is empty or unbounded'}
 $fresh=Ready 'fresh-checkpoint' $reloadCount
 if(Compare-Object @($freshCatalog.entries.handle) @($fresh.host.entries.handle)){throw 'Stream does not cover the fresh native host catalog'}
 foreach($e in $fresh.host.entries){$peer=@($fresh.client.entries|Where-Object handle -eq $e.peerHandle);if($peer.Count-ne1-or$peer[0].peerHandle-ne$e.handle-or$peer[0].peerArm-ne$e.arm-or$peer[0].arm-ne$e.peerArm-or$peer[0].rig-ne$e.rig){throw 'Fresh reciprocal actor/rig binding differs'}}
 Start-Sleep -Milliseconds 2000
 foreach($r in 'host','client'){
  $v=N $r;$rs=ResetState $r;$s=S $r;$character=Q $r '--character';Save "$out/reload-after-$r.json" @{npc=$v;reset=$rs;bridge=$s;character=$character;catalog=(Q $r '--npc-authority')}
  $old=$reload[$r];if($rs.loads-ne$old.reset.loads+1-or$rs.aborted-ne$old.reset.aborted-or$s.localEpoch-eq$old.bridge.localEpoch-or$v.session.reloads-ne1-or$v.session.rearms-ne1-or$v.entries.Count-ne$reloadCount){throw 'Checkpoint lifecycle counters differ'}
  foreach($e in $v.entries){$prior=@($old.npc.entries|Where-Object handle -eq $e.handle);if($prior.Count-eq1-and$e.arm-le$prior[0].arm){throw 'Old arm survived checkpoint'};if($e.poses-lt20){throw 'Fresh body packets did not sustain'}}
  if($character.character-ne$(if($r-eq'host'){1}else{2})){throw 'Player role changed'}
  if($r-eq'client'-and($v.session.acquired-ne(2*$initialCount+2*$reloadCount)-or$v.session.released-ne(2*$initialCount)-or$v.session.lost-or$v.session.retired)){throw 'Checkpoint did not release old AI and acquire exactly two references per fresh native actor'}
 }
 $freshBaseline=Factions 'fresh-baseline';C host 'coop_npc_perception on';$freshPerception=PerceptionReady 'fresh';$freshActive=Factions 'fresh-active';AssertRelations $freshBaseline $freshActive $freshPerception $true
 C host 'coop_npc_perception off';Start-Sleep -Milliseconds 700;$released=P;Save "$out/perception-explicit-off.json" $released
 if($released.enabled-or$released.entries.Count-or$released.acquired-ne$released.released-or$released.lost){throw 'Explicit perception cleanup failed'}
 $afterOff=Factions 'explicit-off';AssertRelations $freshBaseline $afterOff $freshPerception $false
 Save "$out/verified.json" @{passed=$true;initialNpcCount=$initialCount;initialEnemyCount=$ownedRelations.entries.Count;freshNpcCount=$reloadCount;freshEnemyCount=$freshPerception.entries.Count;acquired=$released.acquired;released=$released.released;lost=$released.lost;scope='Independent native directional relation reads across entire bound roster, unrelated/player/partner relationships unchanged; client/duplicate refused, host/client native menus release/reacquire, shared reload retires old ownership, fresh rearm and explicit off restore exact base relations. Active target choice and attack outcomes not yet proved.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};foreach($r in 'host','client'){try{Save "$out/failure-$r.json" @{npc=(N $r);bridge=(S $r);reset=(ResetState $r)}}catch{}}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($r in 'host','client'){if(!$pids.ContainsKey($r)){continue};try{C $r 'coop_npc_perception off';C $r 'coop_npc_stream off';if((S $r).localGameLogicState-eq2){I $r A 180};C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';I $r}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  Start-Sleep -Milliseconds 350
  foreach($r in 'host','client'){if(!$pids.ContainsKey($r)){continue};try{$p=P $r;Save "$out/final-perception-$r.json" $p;if($p.enabled-or$p.entries.Count-or$p.acquired-ne$p.released-or$p.lost){throw 'Perception overrides not balanced'};$v=N $r;Save "$out/final-stream-$r.json" $v;if($v.enabled-or$v.session.enabled-or$v.session.lost-or@($v.entries|Where-Object {$_.references-or$_.lost}).Count-or$v.session.acquired-ne$v.session.released+$v.session.retired){throw 'Native references not balanced'}}catch{$errors.Add("${r} ownership: $($_.Exception.Message)")}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/restore.log"
   & "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline $out -Evidence "$out/note-restoration"
   foreach($r in 'host','client'){
    $inv=Q $r '--player-inventory';Save "$out/inventory-restored-$r.json" $inv;if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($inv.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory restoration differs'}
    $hp=Q $r '--health' 1;Save "$out/health-restored-$r.json" $hp;if($hp.current-ne56){throw 'Health not restored'};Save "$out/after-$r.json" (S $r)
   }
  }catch{$errors.Add($_.Exception.Message)}}
  try{foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}}catch{$errors.Add($_.Exception.Message)}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count){$failure=($failure+'; '+($errors-join '; ')).TrimStart('; ')}
  Save "$out/samples.json" @($samples);Save "$out/result.json" @{passed=(!$failure);failure=$failure;freshActorCount=$reloadCount;scope='Two native menus held over6s, no NPC root/body mutation, retained arms and AI; resumed traffic; coordinated checkpoint with both new epochs/arms, old10 references released before teardown and two fresh references per eligible loaded actor; exact isolated fixture restored. No mid-world spawn/despawn or general combat claim.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
