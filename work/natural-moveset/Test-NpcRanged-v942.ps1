param([ValidateSet('host','client')][string]$Source='host',[string]$Take='npc-combat-reload-v942')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$handles=@{};$captures=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 22|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 30};throw "Native query failed $r $mode"}
function Snap($name){$pair=@{};foreach($r in 'host','client'){$pair[$r]=@{combat=(S $r '.npc-combat.json');stream=(S $r '.npc-stream.json');health=(Q $r '--health' $handles[$r]);player=(Q $r '--health' 1);bridge=(S $r);damage=(S $r '.damage.json')}};Save "$out/$name.json" $pair;return $pair}
function Retreat($r){C $r 'setviewpos -28.0921 -153.962 68.1 270 0'}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v936/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Wrong candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}
  Save "$out/inventory-before-$r.json" (Q $r '--player-inventory');Save "$out/health-before-$r.json" (Q $r '--health' 1)
  $world=Q $r '--world-npcs';Save "$out/world-before-$r.json" $world
  $target=@($world.npcs|Where-Object spawnerName -eq 'ld_spawner_guard_escape_1');if($target.Count-ne1-or!$target[0].unique-or!$target[0].reciprocal){throw 'Target identity unavailable'}
  $handles[$r]=[Convert]::ToUInt32($target[0].handle.Replace('0x',''),16)
 }
 $dirty=$true
 # Ordinary sword hits need native alert combat context. Let each local AI
 # see its player, retreat, then transfer only its inactive attack scheduler.
 foreach($r in 'host','client'){
  C $r 'coop_damage_observe 60000';C $r 'setviewpos -42.9563 -122 62.7 90 0'
  $watch=[Diagnostics.Stopwatch]::StartNew();do{$a=Q $r '--ai-targets' $handles[$r];if($a.combat.resolved-and$a.combat.handle-eq1){break};Start-Sleep -Milliseconds 80}while($watch.ElapsedMilliseconds-lt3500)
  Save "$out/awareness-$r.json" $a;Retreat $r
  if(!$a.combat.resolved-or$a.combat.handle-ne1){throw "Native alert context unavailable $r"}
 }
 $watch.Restart();do{$idle=$true;foreach($r in 'host','client'){$a=Q $r '--ai-attack' $handles[$r];if($a.schedulerActive){$idle=$false}};if(!$idle){Start-Sleep -Milliseconds 100}}while(!$idle-and$watch.ElapsedMilliseconds-lt6000)
 if(!$idle){throw 'Native attack scheduler did not settle'}
 foreach($r in 'host','client'){C $r 'coop_npc_stream on'}
 $watch.Restart();do{
  $ready=$true;foreach($r in 'host','client'){$n=S $r '.npc-stream.json';$entry=@($n.entries|Where-Object handle -eq $handles[$r]);if(!$n.enabled-or$entry.Count-ne1-or!$entry[0].ready){$ready=$false}}
  if(!$ready){Start-Sleep -Milliseconds 100}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt8000)
 Snap 'stream-ready'|Out-Null;if(!$ready){throw 'Target did not enter generic NPC stream'}
 foreach($r in 'host','client'){C $r 'coop_npc_combat on'}
 $watch.Restart();do{$ready=$true;foreach($r in 'host','client'){$n=S $r '.npc-combat.json';if(!$n.enabled-or$n.result-ne'active_native_melee'){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 100}}while(!$ready-and$watch.ElapsedMilliseconds-lt7000)
 $before=Snap 'combat-ready';if(!$ready){throw 'Combat negotiation failed'}
 C host 'coop_npc_combat fixture40000';$watch.Restart();do{$fx=S host '.npc-combat.json';if($fx.fixture.ready){break};Start-Sleep -Milliseconds 80}while($watch.ElapsedMilliseconds-lt2500)
 Save "$out/source-fixture.json" $fx;if(!$fx.fixture.ready){throw "Damage fixture failed $($fx.fixture.result)"}
 C $Source 'coop_ranged_fixture gun_loaded';$watch.Restart();do{$f=S $Source '.ranged-fixture.json';if($f.result-eq'native_ranged_ready'){break};Start-Sleep -Milliseconds 60}while($watch.ElapsedMilliseconds-lt5000);if($f.result-ne'native_ranged_ready'){throw 'Ranged acquisition failed'}
 & "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 1500|Out-Null;Start-Sleep -Milliseconds 1800
 & "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 650|Out-Null;Start-Sleep -Milliseconds 1800
 Save "$out/weapon.json" (Q $Source '--player-gun')
 $peer=if($Source-eq'host'){'client'}else{'host'}
 # Use the already pinned role-correct67/357 bank, without replacing it.
 $bank=$pin.banks|Where-Object {$_.path -like $(if($Source-eq'host'){'*corvo*'}else{'*emily*'})}
 if(@($bank).Count-ne1-or(Get-FileHash "$root/work/lab-instances/$peer/coop-status.json.movement.bin").Hash-ne$bank.sha256){throw 'Wrong installed movement bank'}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $peer 'coop_movement_bank on'
 $watch.Restart();do{$m=S $peer '.movement.json';$prop=S $peer '.ranged-prop.json';if($m.active-and$m.version-eq67-and$m.rangedWeight-gt.9-and$prop.active){break};Start-Sleep -Milliseconds 70}while($watch.ElapsedMilliseconds-lt5000)
 Save "$out/presentation-ready.json" @{movement=$m;prop=$prop}
 if(!$m.active-or$m.version-ne67-or$m.rangedWeight-lt.9-or!$prop.active){throw 'Combined ranged presentation not ready'}

 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','36','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 $rig=Q host '--npc-pose' $handles.host;Save "$out/target-before.json" $rig;$t=$rig.root
 # Real rear pistol input with two native damage contacts; no direct damage injection.
 $x=$t[0]-3*$t[3];$y=$t[1]-3*$t[4];$z=$t[2]+1.7;$yaw=[Math]::Atan2($t[4],$t[3])*180/[Math]::PI
 C $Source ("setviewpos {0} {1} {2} {3} 13.1"-f$x,$y,$z,$yaw)
 $observer=if($Source-eq'host'){'client'}else{'host'}
 # Oblique view includes the whole shooting avatar and victim together.
 $ox=$t[0]-4*$t[3]-3*$t[4];$oy=$t[1]-4*$t[4]+3*$t[3]
 $lookX=$t[0]-1.5*$t[3]-$ox;$lookY=$t[1]-1.5*$t[4]-$oy;$observerYaw=[Math]::Atan2($lookY,$lookX)*180/[Math]::PI
 C $observer ("setviewpos {0} {1} {2} {3} 7"-f$ox,$oy,($t[2]+1.7),$observerYaw)
 Start-Sleep -Milliseconds 350
 $attempt=0
 do{
  if($attempt){$rig=Q host '--npc-pose' $handles.host;$t=$rig.root;$x=$t[0]-3*$t[3];$y=$t[1]-3*$t[4];$z=$t[2]+1.7;$yaw=[Math]::Atan2($t[4],$t[3])*180/[Math]::PI;C $Source ("setviewpos {0} {1} {2} {3} 13.1"-f$x,$y,$z,$yaw);Start-Sleep -Milliseconds 150}
  $old=S host '.npc-combat.json';$previous=$old.committed
  & "$lab/Send-D2Input.ps1" -Role $Source -LeftTrigger 255 -DurationMs 220|Out-Null
  $sample=0
  $watch.Restart();do{
   if($sample-lt12){
    try{$sk=Q $Source '--skeleton' $handles[$Source];Save "$out/contact-$attempt-skeleton-$sample.json" $sk;Save "$out/contact-$attempt-root-$sample.json" (Q $Source '--npc-pose' $handles[$Source])}catch{}
    $sample++
   }
   $hs=S host '.npc-combat.json';$cs=S client '.npc-combat.json'
   if(!$hs.enabled-or!$cs.enabled){break}
   if($hs.committed-gt$previous-and$cs.applied-eq$hs.committed){break};if($watch.ElapsedMilliseconds-gt2200-and$hs.committed-eq$previous-and!@($hs.entries|Where-Object {$_.deathPending-or$_.pending}).Count-and!@($cs.entries|Where-Object {$_.deathPending-or$_.pending}).Count){break};Start-Sleep -Milliseconds 80
  }while($watch.ElapsedMilliseconds-lt5000)
  $contact=Snap "contact-$attempt"
  Save "$out/presentation-after-$attempt.json" @{movement=(S $peer '.movement.json');prop=(S $peer '.ranged-prop.json');fx=(S $peer '.power-fx.json')}

  if($hs.enabled-and$cs.enabled-and$hs.committed-eq$previous-and$cs.applied-eq$hs.committed-and$contact.host.health.current-eq$contact.client.health.current-and$contact.host.health.current-gt0){
   Save "$out/no-contact-$attempt.json" @{scope='Native input produced no observed damage; retry bounded with refreshed contact position, not a protocol acceptance.'};$attempt++;Start-Sleep -Milliseconds 400;continue
  }
  if(!$hs.enabled-or!$cs.enabled-or$hs.committed-le$previous-or$cs.applied-ne$hs.committed-or$contact.host.health.current-ne$contact.client.health.current){throw "Native contact failed: $($hs.result) / $($cs.result)"}
  $attempt++
  if(!$hs.deaths){Start-Sleep -Milliseconds 1200}
 }while(!$hs.deaths-and$attempt-lt2)
 $after=Snap 'native-death'
 if(!$hs.enabled-or!$cs.enabled-or$hs.deaths-ne1-or$cs.deaths-ne1-or$after.host.health.current-ne0-or$after.client.health.current-ne0-or$hs.committed-ne$cs.applied){throw "Native death did not converge: host $($hs.result); client $($cs.result)"}
 foreach($r in 'host','client'){
  $e=@($after[$r].stream.entries|Where-Object handle -eq $handles[$r])
  if($e.Count-ne1-or$e[0].enabled-or$e[0].references-or$e[0].lost-or$e[0].retired){throw "Pre-death release failed $r"}
  $terminal=@($after[$r].combat.entries|Where-Object arm -eq $e[0].arm)
  if($terminal.Count-ne1-or!$terminal[0].terminal){throw "Missing death tombstone $r"}
  foreach($ai in 'decision','reflex'){$a=Q $r "--ai-$ai" $handles[$r];Save "$out/dead-$r-$ai.json" $a;if($a.schedulerActive){throw "Dead AI still active $r $ai"}}
 }
 Start-Sleep -Milliseconds 2500
 $after=Snap 'before-duplicate'
 C $Source 'coop_npc_combat duplicate';if($Source-eq'client'){C host 'coop_npc_combat duplicate'};Start-Sleep -Milliseconds 500
 $dup=Snap 'duplicate-latest'
 foreach($r in 'host','client'){
  if($dup[$r].health.current-ne0-or$dup[$r].combat.committed-ne$after[$r].combat.committed-or$dup[$r].combat.applied-ne$after[$r].combat.applied-or$dup[$r].combat.deaths-ne1){throw 'Duplicate death reapplied'}
 }
 if($dup.client.combat.duplicates-le$after.client.combat.duplicates){throw 'Duplicate terminal commit was not recognized'}
 if($Source-eq'client'-and$dup.host.combat.duplicates-le$after.host.combat.duplicates){throw 'Duplicate terminal request was not recognized'}
 Start-Sleep -Milliseconds 1000
 $settled=Snap 'settled-death'
 if(!$settled.host.combat.enabled-or!$settled.client.combat.enabled-or$settled.host.combat.committed-ne$settled.client.combat.applied-or$settled.host.combat.worldCommitted-ne$settled.client.combat.worldApplied){throw 'Post-death world outcomes failed to converge'}
 $worldHealth=[Collections.Generic.List[object]]::new()
 foreach($e in $settled.host.stream.entries){
  $peer=@($settled.client.stream.entries|Where-Object {$_.arm-eq$e.peerArm-and$_.peerArm-eq$e.arm});if($peer.Count-ne1){throw 'NPC health comparison binding mismatch'}
  $hh=Q host '--health' $e.handle;$ch=Q client '--health' $peer[0].handle
  $worldHealth.Add(@{hostArm=$e.arm;clientArm=$peer[0].arm;host=$hh;client=$ch})
  if($hh.current-ne$ch.current-or$hh.maximum-ne$ch.maximum){Save "$out/world-health.json" @($worldHealth);throw 'World NPC health diverged after death'}
 }
 Save "$out/world-health.json" @($worldHealth)
 Save "$out/world-outcomes.json" @{host=$settled.host.combat.worldCommitted;client=$settled.client.combat.worldApplied;replicaSuppressed=$settled.client.combat.replicaWorldSuppressed;scope='Positive counters establish host-owned world outcomes; inspect declarations to distinguish authored trigger damage from corpse contact. Zero is a regression pass only.'}
 # Keep terminal combat identities present while the shared checkpoint owns
 # the world transition. Do not disable combat/stream ahead of this regression.
 $oldWorld=@{};foreach($r in 'host','client'){
  $oldWorld[$r]=@{bridge=(S $r);combat=(S $r '.npc-combat.json');stream=(S $r '.npc-stream.json');reset=(S $r '.reset.json')}
  C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_checkpoint_sync on';C $r 'coop_checkpoint_delay 0'
 }
 Save "$out/reload-before.json" $oldWorld
 $watch.Restart();do{$h=S host '.reset.json';$c=S client '.reset.json';if($h.enabled-and$c.enabled-and$h.peerArm-and$c.peerArm-and!$h.phase-and!$c.phase-and$h.digest-eq$c.digest){break};Start-Sleep -Milliseconds 60}while($watch.ElapsedMilliseconds-lt5000)
 if(!$h.peerArm-or!$c.peerArm-or$h.digest-ne$c.digest){throw 'Checkpoint negotiation failed'}
 C host 'RestartFromCheckpoint';C client 'RestartFromMemoryCheckpoint'
 $watch.Restart();$pressed=@{host=0;client=0}
 do{
  $ok=$true;$renewed=@{}
  foreach($r in 'host','client'){
   $rs=S $r '.reset.json';$br=S $r;$n=S $r '.npc-stream.json';$combat=S $r '.npc-combat.json'
   $renewed[$r]=@{reset=$rs;bridge=$br;stream=$n;combat=$combat};if($watch.ElapsedMilliseconds%1000-lt100){Save "$out/reload-last-$r.json" $renewed[$r];Save "$out/trace-last-$r.json" (S $r '.checkpoint.json')}
   if($rs.phase-eq7){Save "$out/reload-failed-$r.json" $renewed;foreach($rr in 'host','client'){foreach($suffix in 'reset','checkpoint','npc-stream','npc-combat'){Save "$out/reload-failure-$rr-$suffix.json" (S $rr ".$suffix.json")}};throw "Shared reload failed $($rs.result)"}
   if($rs.completed-ne$oldWorld[$r].reset.completed+1-or$br.localGameLogicState-ne1-or$br.localPendingGameLogicState-or!$rs.avatarReady-or$rs.avatarPending-or!$n.enabled-or$n.session.rearms-ne1-or@($n.entries|Where-Object {!$_.enabled-or!$_.ready}).Count){$ok=$false}
   if(!$rs.held-and$watch.ElapsedMilliseconds-gt6000-and$br.localGameLogicState-in8,15-and$watch.ElapsedMilliseconds-$pressed[$r]-gt700){& "$lab/Send-D2Input.ps1" -Role $r -Button A -DurationMs 100|Out-Null;$pressed[$r]=$watch.ElapsedMilliseconds}
  }
  if(!$ok){Start-Sleep -Milliseconds 80}
 }while(!$ok-and$watch.ElapsedMilliseconds-lt50000)
 Save "$out/reload-completed.json" $renewed;if(!$ok){throw 'NPC renewal after terminal combat did not finish'}
 foreach($r in 'host','client'){
  $new=$renewed[$r];$old=$oldWorld[$r]
  if($new.combat.enabled-or$new.combat.fixture.active-or$new.combat.fixture.lost-or$new.stream.session.lost-or$new.stream.session.retired-or$new.bridge.localEpoch-eq$old.bridge.localEpoch){throw 'Old combat or native ownership survived world change'}
  if($r-eq'client'-and$new.stream.session.released-ne$old.stream.session.acquired){throw 'Old client AI references were not released exactly once'}
  if(@($new.stream.entries|Where-Object {$_.arm-le($old.stream.entries.arm|Measure-Object -Maximum).Maximum}).Count){throw 'Fresh NPCs reused a terminal arm'}
  C $r 'coop_npc_combat duplicate';C $r 'coop_npc_combat on'
 }
 $watch.Restart();do{$ready=$true;$fresh=@{};foreach($r in 'host','client'){$fresh[$r]=S $r '.npc-combat.json';if(!$fresh[$r].enabled-or$fresh[$r].result-ne'active_native_melee'){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 80}}while(!$ready-and$watch.ElapsedMilliseconds-lt7000)
 Save "$out/rearmed-combat.json" $fresh;if(!$ready){throw 'Fresh combat negotiation failed'}
 foreach($r in 'host','client'){
  if($fresh[$r].nonce-le$oldWorld[$r].combat.nonce-or$fresh[$r].deaths-or$fresh[$r].committed-or$fresh[$r].applied-or@($fresh[$r].entries|Where-Object {$_.terminal-or$_.deathPending-or$_.event-or$_.pending}).Count){throw 'Terminal outcomes contaminated the new world'}
 }
 Save "$out/reload-verified.json" @{passed=$true;scope='Shared checkpoint after native death releases remaining old AI, discards terminal combat scope, restores fresh NPC arms and negotiates a new combat nonce with no stale damage.'}

 $sourceTrace=@($after[$Source].combat.sourceHistory|Where-Object nonce -eq $before[$Source].combat.nonce)
 Save "$out/source-trace.json" $sourceTrace
 Save "$out/verified.json" @{source=$Source;inputAttempts=$attempt;hostCommits=$hs.committed;pendingHighWater=$cs.pendingHighWater;sourceCaptures=$sourceTrace.Count;clientNativeApplications=$cs.applied;scope='Real controller-triggered native pistol spread and bullet contacts with exact actor release, native stopped AI and duplicate rejection. Full paired victim choreography and corpse physics require visual judgment and separate coverage.'}

}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Snap 'failure-state'|Out-Null;foreach($r in 'host','client'){foreach($ai in 'decision','reflex'){Save "$out/failure-$r-$ai.json" (Q $r "--ai-$ai" $handles[$r])}}}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){foreach($r in 'host','client'){foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_ranged_prop off','coop_ranged_fixture off','coop_npc_combat fixture_off','coop_npc_combat off','coop_npc_stream off','coop_checkpoint_sync off','coop_checkpoint_delay 0','coop_damage_observe off'){try{C $r $cmd}catch{$errors.Add("${r} $cmd $($_.Exception.Message)")}}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($p in $captures){try{if(!$p.WaitForExit(20000)){throw 'Capture did not finish'};if($p.ExitCode){throw 'Capture failed'}}catch{$errors.Add($_.Exception.Message)}}
  foreach($r in 'host','client'){try{
   $s=S $r;Save "$out/final-$r.json" $s;Save "$out/final-combat-$r.json" (S $r '.npc-combat.json');$n=S $r '.npc-stream.json';Save "$out/final-stream-$r.json" $n
   if($s.localGameLogicState-ne1-or!$s.remoteFresh-or$n.enabled-or@($n.entries|Where-Object {$_.references-or$_.lost}).Count){throw 'Final gameplay or ownership not restored'}
   $hp=Q $r '--health' 1;Save "$out/final-player-health-$r.json" $hp;if($hp.current-ne56){throw 'Player health not restored'};if((S $r '.npc-combat.json').fixture.lost){throw 'Fixture AI reference lost'}
  }catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add("Protected file changed $($f.Path)")}}
  Save "$out/restoration.json" @{passed=($errors.Count-eq0);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;candidate=$pin.dll;source=$Source;scope='Native lethal outcome and duplicates followed by shared checkpoint, old-reference release and fresh combat negotiation.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}






