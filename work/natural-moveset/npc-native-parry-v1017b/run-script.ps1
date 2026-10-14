param([string]$Take='npc-native-parry-v1017',[int]$OnMs=600,[int]$OffMs=350,[int]$DurationMs=40000)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$handles=@{};$captures=[Collections.Generic.List[object]]::new();$driver=$null;$samples=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 22|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){Get-Process -Id $pids[$r] -ErrorAction Stop|Out-Null;Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw "Native query failed $r $mode"}
function WrappedAngle($x){while($x-gt180){$x-=360};while($x-lt-180){$x+=360};return $x}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1018/identity.json" -Raw|ConvertFrom-Json
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
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]([int]($DurationMs/1000)+6),'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 $ramsey=@((Get-Content "$out/world-before-host.json" -Raw|ConvertFrom-Json).npcs|Where-Object {$_.definition -like '*mortimer_ramsey*' -and ([Convert]::ToUInt32($_.handle.Replace('0x',''),16) -in $boundHandles)})
 if($ramsey.Count-ne1){throw 'Unique native Ramsey required for frontal defense setup'}
 $ramseyHandle=[Convert]::ToUInt32($ramsey[0].handle.Replace('0x',''),16);$rp=Q host '--npc-pose' $ramseyHandle
 $faceYaw=[Math]::Atan2($rp.root[1]+122,$rp.root[0]+42.9563)*180/[Math]::PI
 C client ("setviewpos -42.9563 -122 62.7 {0} 0"-f$faceYaw)
 # Host view watches the encounter from its parked position (yaw toward the
 # fight) so the authoritative attacker's native reaction is on camera.
 C host ("setviewpos -33 -153 68.1 {0} -8"-f([Math]::Atan2(-122+153,-42.9563+33)*180/[Math]::PI))
 Save "$out/defense-setup.json" @{input="Ordinary native RightShoulder cycle on=${OnMs}ms off=${OffMs}ms";ramsey=$rp;faceYaw=$faceYaw}
 $pwsh=(Get-Process -Id $PID).Path
 $driver=Start-Process $pwsh -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Drive-BlockCycle-v1017.ps1",'-Role','client','-OnMs',$OnMs,'-OffMs',$OffMs,'-TotalMs',$DurationMs,'-Log',"$out/input-cycle.json") -WindowStyle Hidden -PassThru
 $watch.Restart();$selected=0;$attacking=0
 do{
  $allTargets=@();foreach($eh in $enemyHandles){$allTargets+=Q host '--ai-targets' $eh}
  $viewNow=Q client '--player';$choices=@()
  foreach($target in $allTargets){if($target.combat.resolved-and$target.combat.handle-eq$avatar){
   $body=Q host '--npc-pose' $target.handle;$dx=$body.root[0]-$viewNow.nativeViewPosition[0];$dy=$body.root[1]-$viewNow.nativeViewPosition[1]
   $choices+=@{handle=$target.handle;root=$body.root;dx=$dx;dy=$dy;distance=($dx*$dx+$dy*$dy)}
  }}
  $closest=$choices|Sort-Object distance|Select-Object -First 1;$defenseAim=$null
  if($closest){
   $want=[Math]::Atan2($closest.dy,$closest.dx)*180/[Math]::PI;$actual=[Math]::Atan2($viewNow.nativeViewOrientation[1],$viewNow.nativeViewOrientation[0])*180/[Math]::PI
   $yawError=WrappedAngle ($want-$actual);$defenseAim=@{target=$closest.handle;position=$closest.root;distance=[Math]::Sqrt($closest.distance);yawError=$yawError;view=$viewNow}
   if([Math]::Abs($yawError)-gt12){C client ("setviewpos -42.9563 -122 62.7 {0} 0"-f$want)}
  }
  $targets=Q host '--ai-targets' $handles.host;$attack=Q host '--ai-attack' $handles.host;$pose=Q host '--npc-pose' $handles.host;$bridge=S host
  $row=@{ms=$watch.ElapsedMilliseconds;allTargets=$allTargets;targets=$targets;attack=$attack;pose=$pose;bridge=$bridge;avatarHealth=(Q host '--health' $avatar);clientPlayerHealth=(Q client '--health' 1);playerStates=(Q client '--player-states');view=(Q client '--player');incoming=(S client '.npc-attack.json');defenseAim=$defenseAim};$samples.Add($row)
  if(@($allTargets|Where-Object {$_.combat.resolved-and$_.combat.handle-eq$avatar}).Count){$selected++};if($attack.schedulerActive){$attacking++}
  $defended=$row.incoming.blocked+$row.incoming.parryLose+$row.incoming.parryWin
  if($row.incoming.parryWin-ge2-or$row.incoming.applied-ge3-or$row.clientPlayerHealth.current-le16){
   C client 'setviewpos -33 -148 68.1 270 0'
   Save "$out/retreated-after-native-hit.json" @{atMs=$watch.ElapsedMilliseconds;health=$row.clientPlayerHealth.current}
   break
  }
  if($row.avatarHealth.current-lt40){throw 'Owned clone absorbed gameplay damage instead of routing it'}
  Start-Sleep -Milliseconds 100
 }while($watch.ElapsedMilliseconds-lt$DurationMs)
 if($driver-and!$driver.HasExited){Stop-Process -Id $driver.Id};$driver=$null
 & "$lab/Send-D2Input.ps1" -Role client -DurationMs 100|Out-Null
 C client 'setviewpos -33 -148 68.1 270 0'
 Save "$out/native-samples.json" @($samples)
 foreach($r in 'host','client'){Save "$out/damage-$r.json" (S $r '.damage.json');Save "$out/stream-$r.json" (S $r '.npc-stream.json');Save "$out/player-after-$r.json" (Q $r '--health' 1)}
 Save "$out/guard-faction-after.json" (Q host '--faction' $handles.host)
 foreach($capture in $captures){if(!$capture.WaitForExit($DurationMs+20000)-or$capture.ExitCode){throw 'Recording did not finish before cleanup'}}
 foreach($r in 'host','client'){Save "$out/perception-after-$r.json" (S $r '.avatar-perception.json')}
 $ap=S host '.avatar-perception.json';if(!$ap.enabled-or!$ap.installed-or$ap.retained-lt100-or!$ap.avatarAiHeld){throw 'Perception retention not active or owned clone AI was resumed'}
 if(!(S host).probeFollowEnabled){throw 'Remote following must remain enabled'}
 $settle=[Diagnostics.Stopwatch]::StartNew();do{
  $hs=S host '.npc-attack.json';$cs=S client '.npc-attack.json'
  if($hs.event-and$hs.acked-eq$hs.event-and$cs.consumed-eq$hs.event-and!$hs.pending){break};Start-Sleep -Milliseconds 80
 }while($settle.ElapsedMilliseconds-lt5000)
 $beforeHealth=Q client '--health' 1;$avatarAfter=Q host '--health' $avatar
 Save "$out/native-hit-result.json" @{host=$hs;client=$cs;player=$beforeHealth;avatar=$avatarAfter}
 foreach($r in 'host','client'){Save "$out/damage-final-$r.json" (S $r '.damage.json')}
 if(!$hs.enabled-or!$cs.enabled-or!$hs.event-or$hs.acked-ne$hs.event-or$cs.consumed-ne$hs.event-or$hs.pending-or$beforeHealth.current-le0-or$avatarAfter.current-ne40){throw 'Native enemy sword contact, health ownership or acknowledgement failed'}
 $clientDamage=S client '.damage.json'
 $swordDamage=@($clientDamage.events|Where-Object {$_.handle-eq1-and$_.damageType-eq'80'-and$_.damageAmount-eq20})
 $defended=$cs.blocked+$cs.parryLose+$cs.parryWin
 # Native resolution: every consumed contact is either defended (no 717950
 # call) or one native sword damage application. Health loss must equal the
 # applied subset only; the host must see the same outcomes in its acks.
 $expectedLoss=($swordDamage|Measure-Object -Property inflictedDamage -Sum).Sum;if(!$expectedLoss){$expectedLoss=0}
 $defenseEvents=@(@{defended=$defended;blocked=$cs.blocked;parryLose=$cs.parryLose;parryWin=$cs.parryWin;applied=$cs.applied;consumed=$cs.consumed;swordDamageCalls=$swordDamage.Count;expectedLoss=$expectedLoss;initial=$playerInitial.current;final=$beforeHealth.current;hostBlocked=$hs.blocked;hostParryLose=$hs.parryLose;hostParryWin=$hs.parryWin;hostReactions=$hs.reactions;hostReactionFailures=$hs.reactionFailures})
 if($defended-lt1-or($defended+$cs.applied)-ne$cs.consumed-or$swordDamage.Count-ne$cs.applied-or[Math]::Abs($playerInitial.current-$expectedLoss-$beforeHealth.current)-gt.01-or
    $hs.blocked-ne$cs.blocked-or$hs.parryLose-ne$cs.parryLose-or$hs.parryWin-ne$cs.parryWin-or$hs.reactionFailures-or$hs.reactions-ne($hs.blocked+$hs.parryLose+$hs.parryWin)){$defenseEvents=@()}
 # Parry-win: host native blowback applied once per acknowledged win with no
 # health change; receiver replica FX once per win; observer agrees.
 $parry=@{hostApplied=$hs.blowback.applied;hostFailures=$hs.blowback.failures;hostLastInflicted=$hs.blowback.lastInflicted;hostLastAmount=$hs.blowback.lastAmount;hostLastResult=$hs.blowback.lastResult;
  clientFx=$cs.blowback.replicaFx;clientFxFailures=$cs.blowback.replicaFxFailures;clientRelayed=$cs.defenseObserver.relayed;clientNative=$cs.defenseObserver.native;hostNative=$hs.defenseObserver.native;
  hostHooked=$hs.defenseObserver.hooked;clientHooked=$cs.defenseObserver.hooked}
 Save "$out/parry-results.json" $parry
 $parryOk=$hs.parryWin-ge1-and$parry.hostApplied-eq$hs.parryWin-and!$parry.hostFailures-and$parry.hostLastInflicted-eq0-and$parry.clientFx-eq$cs.parryWin-and!$parry.clientFxFailures-and
  $parry.hostHooked-and$parry.clientHooked-and$cs.defenseObserver.relayed[0]-eq$cs.blocked-and$cs.defenseObserver.relayed[1]-eq$cs.parryLose-and$cs.defenseObserver.relayed[2]-eq$cs.parryWin
 $nativePoses=@($samples|Where-Object {$_.playerStates.combatSceneValid-and$_.playerStates.combatScene.variant-in10,11,13})
 Save "$out/defense-results.json" @{blocked=$defenseEvents;nativeBlockSamples=$nativePoses.Count;host=$hs;client=$cs;health=$beforeHealth}
 if(!$defenseEvents.Count-or!$nativePoses.Count){throw 'No receiver-native defended contact with consistent health, acknowledgement outcomes and block animation'}
 if(!$parryOk){throw 'No consistent native timed parry-win with host blowback and replica FX'}
 C host 'coop_npc_attack duplicate';$dups=[Diagnostics.Stopwatch]::StartNew();do{$afterDup=S client '.npc-attack.json';if($afterDup.duplicates-gt$cs.duplicates){break};Start-Sleep -Milliseconds 80}while($dups.ElapsedMilliseconds-lt5000)
 $dupHealth=Q client '--health' 1;Save "$out/duplicate.json" @{attack=$afterDup;health=$dupHealth}
 if($afterDup.duplicates-le$cs.duplicates-or$afterDup.applied-ne$cs.applied-or($afterDup.blocked+$afterDup.parryLose+$afterDup.parryWin)-ne($cs.blocked+$cs.parryLose+$cs.parryWin)-or$dupHealth.current-ne$beforeHealth.current){throw 'Duplicate native enemy hit was not idempotent'}

 Save "$out/verified.json" @{passed=$true;selectedSamples=$selected;attackSamples=$attacking;partner=$avatar;guard=$handles.host;nativeOverrideUsed=$false;scope='Candidate1016: ordinary native block-press cycle; relayed enemy sword contacts resolved natively on the actual client including at least one native parry-win; host applied native 140636140 blowback (zero health change) on its authoritative attacker and native blowback FX; client played the same FX on its replica; outcome counts agree across ack, host and the passive 859980 observer. Earlier text: actual client native held-block scene with controller facing toward the closest engaged enemy, at least one relayed enemy sword contact defended by native 1406357d0-equivalent resolution (no 717950 call), host-acknowledged outcome and native attacker reaction event, clone40 and duplicate rejection. Not timed parry or host-attacker recoil/stagger acceptance.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};Save "$out/native-samples.json" @($samples)}
finally{
 if($driver-and!$driver.HasExited){try{Stop-Process -Id $driver.Id}catch{}}
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($p in $captures){try{if(!$p.WaitForExit($DurationMs+20000)-or$p.ExitCode){throw 'Recording failed'}}catch{$errors.Add($_.Exception.Message)}}
  if($dirty){
   try{& "$lab/Send-D2Input.ps1" -Role client -DurationMs 100|Out-Null}catch{$errors.Add($_.Exception.Message)}
   foreach($r in 'host','client'){foreach($cmd in 'coop_npc_attack off','coop_avatar_perception off','coop_shadow_stream off','coop_npc_stream off','coop_damage_observe off'){try{C $r $cmd}catch{$errors.Add("${r}: $($_.Exception.Message)")}}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{
   $s=S $r;$n=S $r '.npc-stream.json';$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;stream=$n;health=$hp}
   if($s.localGameLogicState-ne1-or!$s.remoteFresh-or$n.enabled-or$n.session.lost-or@($n.entries|Where-Object {$_.references-or$_.lost}).Count-or$hp.current-ne56){throw 'Final gameplay/health/ownership not restored'}
  }catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;candidate=$pin.dll;scope='Bounded generic native enemy sword routing to actual remote player; restored isolated lab.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
