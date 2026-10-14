param([string]$Take='npc-native-death-v993')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$handles=@{};$captures=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 22|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){Get-Process -Id $pids[$r] -ErrorAction Stop|Out-Null;Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw "Native query failed $r $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v992/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Wrong candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  $s=S $r;if($s.localGameLogicState-ne1){throw 'Gameplay required'}
  Save "$out/inventory-before-$r.json" (Q $r '--player-inventory');Save "$out/health-before-$r.json" (Q $r '--health' 1)
  $world=Q $r '--world-npcs';Save "$out/world-before-$r.json" $world;$target=@($world.npcs|Where-Object spawnerName -eq 'ld_spawner_guard_escape_1')
  if($target.Count-ne1-or!$target[0].unique-or!$target[0].reciprocal){throw 'Target identity unavailable'};$handles[$r]=[Convert]::ToUInt32($target[0].handle.Replace('0x',''),16)
 }
 $dirty=$true
 foreach($r in 'host','client'){C $r 'coop_avatar_perception on';Save "$out/perception-before-$r.json" (S $r '.avatar-perception.json')}
 # Positive local-player control also lets the native room encounter leave
 # its initial scripted behavior. Do not infer blindness from an idle scene.
 foreach($r in 'host','client'){
  C $r 'setviewpos -42.9563 -122 62.7 90 0';$localWatch=[Diagnostics.Stopwatch]::StartNew()
  do{$control=Q $r '--ai-targets' $handles[$r];if($control.combat.resolved-and$control.combat.handle-eq1){break};Start-Sleep -Milliseconds 80}while($localWatch.ElapsedMilliseconds-lt4000)
  Save "$out/local-positive-control-$r.json" $control
  C $r 'setviewpos -33 -153 68.1 90 0'
  if(!$control.combat.resolved-or$control.combat.handle-ne1){throw "Native local positive control did not detect player $r"}
 }
 foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_damage_observe 60000';C $r 'coop_npc_stream on'}
 $watch=[Diagnostics.Stopwatch]::StartNew();do{
  $ready=$true;foreach($r in 'host','client'){$n=S $r '.npc-stream.json';if(!$n.enabled-or!$n.entries.Count-or@($n.entries|Where-Object {!$_.enabled-or!$_.ready}).Count){$ready=$false}}
  if(!$ready){Start-Sleep -Milliseconds 80}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt8000)
 if(!$ready){throw 'Native NPC stream did not become ready'}
 $boundHandles=@((S host '.npc-stream.json').entries.handle)
 $enemyHandles=@((Get-Content "$out/world-before-host.json" -Raw|ConvertFrom-Json).npcs|Where-Object {($_.spawnerName -like '*guard*' -or $_.definition -like '*mortimer_ramsey*') -and ([Convert]::ToUInt32($_.handle.Replace('0x',''),16) -in $boundHandles)}|ForEach-Object {[Convert]::ToUInt32($_.handle.Replace('0x',''),16)})
 if($enemyHandles.Count-lt3){throw 'At least three active native enemy identities required'}
 Save "$out/watched-enemies.json" $enemyHandles
 foreach($r in 'host','client'){C $r 'coop_npc_attack on'}
 $attackReady=[Diagnostics.Stopwatch]::StartNew();do{
  $both=$true;foreach($r in 'host','client'){$a=S $r '.npc-attack.json';if(!$a.enabled-or!$a.entries.Count-or@($a.entries|Where-Object {!$_.ready}).Count){$both=$false}}
  if(!$both){Start-Sleep -Milliseconds 50}
 }while(!$both-and$attackReady.ElapsedMilliseconds-lt8000)
 if(!$both){throw 'NPC sword attack handshake failed'}
 foreach($r in 'host','client'){Save "$out/attack-ready-$r.json" (S $r '.npc-attack.json')}
 $playerInitial=Q client '--health' 1;Save "$out/player-initial.json" $playerInitial
 foreach($r in 'host','client'){C $r 'coop_checkpoint_sync on';C $r 'coop_checkpoint_delay 0'}
 $cw=[Diagnostics.Stopwatch]::StartNew();do{$h=S host '.reset.json';$c=S client '.reset.json';if($h.enabled-and$c.enabled-and$h.peerArm-and$c.peerArm-and!$h.phase-and!$c.phase-and$h.digest-eq$c.digest){break};Start-Sleep -Milliseconds 50}while($cw.ElapsedMilliseconds-lt5000)
 if(!$h.peerArm-or!$c.peerArm-or$h.digest-ne$c.digest){throw 'Checkpoint negotiation failed'}
 $oldWorld=@{};foreach($r in 'host','client'){$oldWorld[$r]=@{bridge=(S $r);reset=(S $r '.reset.json');stream=(S $r '.npc-stream.json');attack=(S $r '.npc-attack.json')}}
 Save "$out/before-death.json" $oldWorld
 $avatar=(S host).probeActorHandle;$f=Q host '--faction' $handles.host;Save "$out/guard-faction-before.json" $f
 Save "$out/avatar-faction-before.json" (Q host '--faction' $avatar)
 $relation=@($f.relations|Where-Object handle -eq $avatar);if($relation.Count-ne1-or$relation[0].overridden-or$relation[0].effective-ne2){throw 'Expected native hostility without overrides'}
 Save "$out/target-before.json" (Q host '--ai-targets' $handles.host)
 $rig=Q host '--npc-pose' $handles.host;Save "$out/guard-pose-before.json" $rig;$t=$rig.root
 # Keep real host away. Put the remote player in the native enemy's view;
 # host AI remains running and client AI stays owned by the NPC stream.
 C host 'setviewpos -33 -153 68.1 90 0'
 $x=$t[0]+5*$t[3];$y=$t[1]+5*$t[4];$yaw=[Math]::Atan2(-$t[4],-$t[3])*180/[Math]::PI
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','25','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 C client 'setviewpos -42.9563 -122 62.7 90 0'
 $watch.Restart();$screensReady=$false
 do{
  $pair=@{};$screensReady=$true
  foreach($r in 'host','client'){
   $rs=S $r '.reset.json';$br=S $r;$pair[$r]=@{bridge=$br;reset=$rs;attack=(S $r '.npc-attack.json');stream=(S $r '.npc-stream.json');health=(Q $r '--health' 1)}
   if($rs.phase-eq7){throw "Shared enemy death failed: $r $($rs.result)"}
   if($br.localGameLogicState-ne13-or!$rs.deathReady-or!$rs.peerDeathReady){$screensReady=$false}
  }
  $samples.Add(@{ms=$watch.ElapsedMilliseconds;pair=$pair})
  if(!$screensReady){Start-Sleep -Milliseconds 100}
 }while(!$screensReady-and$watch.ElapsedMilliseconds-lt23000)
 Save "$out/death-samples.json" @($samples);Save "$out/death-pair.json" $pair
 if(!$screensReady){throw 'Both native death screens were not reached from repeated relayed enemy hits'}
 foreach($r in 'host','client'){
  $g=Q $r '--gameover';Save "$out/gameover-$r.json" $g
  if($g.logic-ne13-or$g.pending-or$g.flash-eq'0x0'-or@($g.showFunction).Count-ne2-or$g.showFunction[0]-eq'0x0'-or$g.showFunction[1]-eq'0x0'){throw 'Native death UI identity incomplete'}
  if($pair[$r].health.current-ne$(if($r-eq'client'){0}else{56})-or$pair[$r].reset.deathCount-ne$oldWorld[$r].reset.deathCount+1-or$pair[$r].reset.loads-ne$oldWorld[$r].reset.loads-or$pair[$r].attack.enabled){throw 'Shared death applied wrong health or failed to retire attack capture'}
 }
 foreach($r in 'host','client'){if(!$pair[$r].attack.session.enabled-or!$pair[$r].attack.session.waitingCheckpoint-or!$pair[$r].attack.session.terminal-or$pair[$r].attack.pending){throw 'Native terminal outcome did not retain clean checkpoint intent'}}
 if($pair.host.attack.event-ne$pair.client.attack.applied){throw 'Extra host contact was emitted after the lethal client result'}
 if($pair.client.attack.applied-lt2){throw 'Repeated native enemy applications not established'}
 foreach($r in 'host','client'){Save "$out/damage-final-$r.json" (S $r '.damage.json')}
 foreach($capture in $captures){if(!$capture.WaitForExit(15000)-or$capture.ExitCode){throw 'Death capture did not finish before reload'}}
 Save "$out/capture-ended-before-reload.json" @{passed=$true}
 C host 'coop_gameover_retry';C client 'RestartFromCheckpoint'
 $watch.Restart();$pressed=@{host=0;client=0}
 do{
  $ok=$true;$renewed=@{}
  foreach($r in 'host','client'){
   $rs=S $r '.reset.json';$br=S $r;$n=S $r '.npc-stream.json';$attack=S $r '.npc-attack.json'
   $renewed[$r]=@{reset=$rs;bridge=$br;stream=$n;attack=$attack}
   if($rs.phase-eq7){Save "$out/reload-failed-$r.json" $renewed;throw "Shared reload failed $($rs.result)"}
   if($rs.completed-ne$oldWorld[$r].reset.completed+1-or$br.localGameLogicState-ne1-or$br.localPendingGameLogicState-or!$rs.avatarReady-or$rs.avatarPending-or!$n.enabled-or$n.session.rearms-ne1-or@($n.entries|Where-Object {!$_.enabled-or!$_.ready}).Count){$ok=$false}
   if(!$rs.held-and$watch.ElapsedMilliseconds-gt6000-and$br.localGameLogicState-in8,15-and$watch.ElapsedMilliseconds-$pressed[$r]-gt700){& "$lab/Send-D2Input.ps1" -Role $r -Button A -DurationMs 100|Out-Null;$pressed[$r]=$watch.ElapsedMilliseconds}
  }
  if(!$ok){Start-Sleep -Milliseconds 80}
 }while(!$ok-and$watch.ElapsedMilliseconds-lt50000)
 Save "$out/reload-completed.json" $renewed;if(!$ok){throw 'NPC renewal after enemy-caused shared death did not finish'}
 foreach($r in 'host','client'){
  $new=$renewed[$r];$old=$oldWorld[$r]
  if($new.stream.session.lost-or$new.bridge.localEpoch-eq$old.bridge.localEpoch-or$new.reset.loads-ne$old.reset.loads+1){throw 'Old attack ownership survived or native load count differs'}
  if(@($new.stream.entries|Where-Object {$_.arm-le($old.stream.entries.arm|Measure-Object -Maximum).Maximum}).Count){throw 'Fresh NPCs reused prior-world arms'}
  Save "$out/recovered-character-$r.json" (Q $r '--character')
  if((Q $r '--character').character-ne$(if($r-eq'host'){1}else{2})-or(Q $r '--health' 1).current-ne56-or(Q $r '--health' $new.bridge.probeActorHandle).current-ne40){throw 'Roles or native health not recovered'}
 }
 $watch.Restart();do{$ready=$true;$fresh=@{};foreach($r in 'host','client'){$fresh[$r]=S $r '.npc-attack.json';if(!$fresh[$r].enabled-or$fresh[$r].result-ne'active_native_sword'){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 80}}while(!$ready-and$watch.ElapsedMilliseconds-lt7000)
 Save "$out/rearmed-attack.json" $fresh;if(!$ready){throw 'Fresh enemy attack negotiation failed'}
 foreach($r in 'host','client'){if($fresh[$r].nonce-le$oldWorld[$r].attack.nonce-or$fresh[$r].event-or$fresh[$r].consumed-or$fresh[$r].applied-or$fresh[$r].pending-or$fresh[$r].session.renewals-ne1-or!$fresh[$r].session.enabled-or$fresh[$r].session.waitingCheckpoint){throw 'Old damage contaminated fresh attack scope or automatic renewal failed'}}
 C host 'coop_npc_attack off';$watch.Restart();do{$stopped=@{};foreach($r in 'host','client'){$stopped[$r]=S $r '.npc-attack.json'};if(!$stopped.host.enabled-and!$stopped.client.enabled-and!$stopped.client.session.enabled){break};Start-Sleep -Milliseconds 80}while($watch.ElapsedMilliseconds-lt5000)
 Save "$out/one-sided-stop.json" $stopped
 if($stopped.host.enabled-or$stopped.client.enabled-or$stopped.host.session.enabled-or$stopped.client.session.enabled-or$stopped.client.result-ne'peer_stopped'){throw 'One-sided stop failed to retire both attack scopes'}
 Save "$out/verified.json" @{passed=$true;applications=$pair.client.attack.applied;nativeOverrideUsed=$false;scope='Repeated host-owned enemy sword contacts cause actual client death, both native death screens without harming host, one coordinated checkpoint retry, fresh roles/avatars/NPC arms and automatic attack renewal without prior hits and one-sided peer stop. Not native-menu renewal, all damage types, or full campaign.'}

}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};Save "$out/native-samples.json" @($samples)}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($p in $captures){try{if(!$p.WaitForExit(20000)-or$p.ExitCode){throw 'Recording failed'}}catch{$errors.Add($_.Exception.Message)}}
  if($dirty){
   foreach($r in 'host','client'){foreach($cmd in 'coop_checkpoint_sync off','coop_checkpoint_delay 0','coop_npc_attack off','coop_avatar_perception off','coop_shadow_stream off','coop_npc_stream off','coop_damage_observe off'){try{C $r $cmd}catch{$errors.Add("${r}: $($_.Exception.Message)")}}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{
   $s=S $r;$n=S $r '.npc-stream.json';$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;stream=$n;health=$hp}
   if($s.localGameLogicState-ne1-or!$s.remoteFresh-or$n.enabled-or$n.session.lost-or@($n.entries|Where-Object {$_.references-or$_.lost}).Count-or$hp.current-ne56){throw 'Final gameplay/health/ownership not restored'}
  }catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;candidate=$pin.dll;scope='Repeated native enemy hits, shared death screens and one coordinated checkpoint; restored isolated lab.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
