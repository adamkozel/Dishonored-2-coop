param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-continuous-recovery-v778',[ValidatePattern('^[a-z0-9-]+$')][string]$Candidate='candidate-door-menu-v769')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$inventory=@{};$bindings=@{}
$captures=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$cycles=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new();$doorName='ld_door_350cm_int_rich_wood_01_3'
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt12;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 20}};throw "Status unavailable $r$suffix"}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
function Save($name,$value){$value|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $name}
function D($r){S $r '.door-sync.json'}
function RS($r){S $r '.reset.json'}
function Selected($d){@($d.entries|Where-Object name -eq $doorName)[0]}
function CatalogReady($d){$d.enabled-and$d.catalogCount-eq6-and$d.ineligible-eq4-and@($d.entries|Where-Object {!$_.enabled-or!$_.ready-or!$_.revision-or$_.revision-ne$_.applied}).Count-eq0}
function W($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing field $offset"};[Convert]::ToUInt64($w[0].word.Substring(2),16)}
function BindDoor($r){$e=@((Q $r '--entities').entities|Where-Object name -eq $doorName);if($e.Count-ne1-or$e[0].type-ne'.?AVarkDoor@@'){throw 'Exact closet missing'};$cp=@((Q $r '--components' $e[0].handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@');if($cp.Count-ne1-or$cp[0].owner-ne$e[0].pointer){throw 'Door owner mismatch'};$o=Q $r '--object' $cp[0].pointer;$bindings[$r]=@{handle=$e[0].handle;component=$cp[0].pointer;owner=[Convert]::ToUInt64($e[0].pointer.Substring(2),16);generation=((W $o '0x18')-band [uint64]4294967295)}}
function NativeDoor($r){$b=$bindings[$r];$o=Q $r '--object' $b.component;if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$b.owner-or((W $o '0x18')-band [uint64]4294967295)-ne$b.generation){throw 'Door lifetime changed'};$s=W $o '0x48';return @{current=[uint32]($s-band [uint64]4294967295);pending=[uint32]($s-shr32);side=[int]((W $o '0xc8')-band255)}}
function Pair(){ $v=@{};foreach($r in 'host','client'){$v[$r]=@{reset=(RS $r);doors=(D $r);bridge=((S $r)|Select-Object localEpoch,remoteEpoch,localGameLogicState,localPendingGameLogicState,remoteGameLogicState,connectionState,remoteFresh,probeActorHandle,probeFollowEnabled)}};return $v }
function Place($source,$peer,$path){C $source 'setviewpos -47.05 -163.5 68.1 270 0';C $peer 'setviewpos -44 -161 68.1 220 0';& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$path-source-camera.json" -Quick;& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 10 -Evidence "$path-peer-camera.json" -Quick}
function WaitDoor($target){$w=[Diagnostics.Stopwatch]::StartNew();do{$ok=$true;$v=@{};foreach($r in 'host','client'){$n=NativeDoor $r;$d=Selected (D $r);$v[$r]=@{native=$n;door=$d};if(!$d.enabled-or$d.applied-ne$d.revision-or$d.target-ne$target-or$n.current-ne$target-or$n.pending-ne$target-or($target-eq2-and$n.side-ne$d.side)){$ok=$false}};if($ok){return $v};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt6000);Save "$cycleDir/failed-door-wait.json" $v;throw 'Native door state failed to converge'}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='ContinuousRecovery778';take=$Take;state='running';started=(Get-Date).ToString('o')}
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if($dll-ne$pin.dll){throw 'Archived build mismatch'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1-or(Q $r '--character').character-ne$(if($r-eq'host'){1}else{2})){throw 'Known gameplay roles required'}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Living baseline required'}
 }
 $dirty=$true
 foreach($r in 'host','client'){
  foreach($cmd in 'coop_checkpoint_sync off','coop_door_sync off','coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  C $r 'coop_checkpoint_sync on';C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})
 }
 $w=[Diagnostics.Stopwatch]::StartNew();do{$h=RS host;$c=RS client;if($h.enabled-and$c.enabled-and$h.peerArm-and$c.peerArm-and!$h.phase-and!$c.phase-and$h.digest-eq$c.digest){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$h.peerArm-or!$c.peerArm-or$h.digest-ne$c.digest){throw 'Canonical checkpoint pair unavailable'}
 foreach($r in 'host','client'){C $r 'coop_door_sync session'}
 $w.Restart();do{if((CatalogReady (D host))-and(CatalogReady (D client))){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!(CatalogReady (D host))-or!(CatalogReady (D client))){throw 'Initial six-door session unavailable'}
 $sessionBefore=Pair;Save "$out/session-before.json" $sessionBefore
 $specs=@(@{id='01-host-reload';source='host';death=$false;pauseSurvivor=$false},@{id='02-client-reload';source='client';death=$false;pauseSurvivor=$false},@{id='03-corvo-death';source='host';death=$true;pauseSurvivor=$false},@{id='04-emily-death-paused-survivor';source='client';death=$true;pauseSurvivor=$true})
 foreach($spec in $specs){
  $cycleDir=Join-Path $out $spec.id;New-Item -ItemType Directory $cycleDir|Out-Null;$samples=[Collections.Generic.List[object]]::new();$source=$spec.source;$peer=if($source-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';BindDoor $r}
  Place $source $peer "$cycleDir/before"
  if((NativeDoor $source).current-eq2){I $source X 180;Save "$cycleDir/preparatory-close.json" (WaitDoor 0)}
  $before=Pair;Save "$cycleDir/before.json" $before;Save "$cycleDir/bindings-before.json" $bindings
  foreach($r in 'host','client'){if((NativeDoor $r).current-ne0-or!$before[$r].reset.enabled-or$before[$r].reset.phase-or!$before[$r].doors.session.enabled){throw 'Continuous session lost readiness'};Save "$cycleDir/loading-before-$r.json" (S $r '.loading.json')}
  $cycleCaptures=@{}
  foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Native window missing'};$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','35','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$cycleDir/$r.mp4");$cap=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$cycleDir/$r-capture.log" -RedirectStandardOutput "$cycleDir/$r-capture.stdout" -PassThru;$captures.Add($cap);$cycleCaptures[$r]=$cap}
  Start-Sleep -Milliseconds 600;I $source X 180
  if($spec.death){Save "$cycleDir/open-before-death.json" (WaitDoor 2)}else{
   $w.Restart();do{$ns=NativeDoor $source;$np=NativeDoor $peer;if($ns.current-eq1-and$np.current-eq1-and(Selected (D $peer)).nativeCalls-eq(Selected $before[$peer].doors).nativeCalls+1){break};Start-Sleep -Milliseconds 20}while($w.ElapsedMilliseconds-lt2500)
   if($ns.current-ne1-or$np.current-ne1-or(Selected (D $peer)).nativeCalls-ne(Selected $before[$peer].doors).nativeCalls+1){throw 'Both moving doors and the corresponding receiver queue count not observed before reload'}
  }
  $retiring=Pair;Save "$cycleDir/before-interruption.json" $retiring
  if($spec.death){
   if($spec.pauseSurvivor){I $peer Start 180;$w.Restart();do{if((S $peer).localGameLogicState-eq2){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt2500);if((S $peer).localGameLogicState-ne2){throw 'Survivor native menu not reached'}}
   C $source 'setviewpos -28.0921 -153.962 120 0 0 0'
   $w.Restart();do{$p=Pair;$samples.Add(@{tag='death';ms=$w.ElapsedMilliseconds;pair=$p});$ready=$true;foreach($r in 'host','client'){if($p[$r].reset.phase-eq7){throw "Death failed: $r $($p[$r].reset.result)"};if($p[$r].bridge.localGameLogicState-ne13-or!$p[$r].reset.deathReady-or!$p[$r].reset.peerDeathReady){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 70}}while(!$ready-and$w.ElapsedMilliseconds-lt20000)
   if(!$ready){throw 'Both native death screens did not become ready'}
   $screens=@{};foreach($r in 'host','client'){$g=Q $r '--gameover';$health=Q $r '--health' 1;$screens[$r]=@{gameover=$g;health=$health;player=(Q $r '--player');logic=(Q $r '--logic');pair=$p[$r]};Save "$cycleDir/death-screen-$r.json" $screens[$r];if($g.logic-ne13-or$g.pending-or$g.flash-eq'0x0'-or@($g.showFunction).Count-ne2-or$g.showFunction[0]-eq'0x0'-or$g.showFunction[1]-eq'0x0'){throw 'Native game-over Flash identity incomplete'};if($health.current-ne$(if($r-eq$source){0}else{56})-or$p[$r].reset.deathCount-ne$before[$r].reset.deathCount+1-or$p[$r].reset.loads-ne$before[$r].reset.loads-or$p[$r].doors.enabled-or!$p[$r].doors.session.enabled){throw 'Shared death did not retain session intent without harming survivor or loading early'}}
   Save "$cycleDir/death-screens.json" $screens
   & "$lab/Send-D2Input.ps1" -Role $peer -LeftY 16000 -DurationMs 450|Out-Null;Start-Sleep -Milliseconds 650;$stationary=Q $peer '--player';$distance=0.0;for($i=0;$i-lt3;$i++){$delta=$stationary.candidatePosition[$i]-$screens[$peer].player.candidatePosition[$i];$distance+=$delta*$delta};Save "$cycleDir/survivor-input-block.json" @{before=$screens[$peer].player;after=$stationary;distance=[Math]::Sqrt($distance)};if([Math]::Sqrt($distance)-gt.001-or(S $peer).localGameLogicState-ne13){throw 'Survivor moved during shared death'}
   Start-Sleep -Milliseconds 1400;C $peer 'coop_gameover_retry';C $source 'RestartFromCheckpoint'
  }else{C $source 'RestartFromCheckpoint';C $peer 'RestartFromMemoryCheckpoint'}
  $w.Restart();$lastPress=@{host=0;client=0};do{
   $p=Pair;$samples.Add(@{tag='reload';ms=$w.ElapsedMilliseconds;pair=$p});$ready=$true
   foreach($r in 'host','client'){$rs=$p[$r].reset;$br=$p[$r].bridge;$ds=$p[$r].doors;if($rs.phase-eq7){throw "Recovery failed: $r $($rs.result)"};if($rs.completed-ne$before[$r].reset.completed+1-or$br.localGameLogicState-ne1-or$br.localPendingGameLogicState-or!$rs.avatarReady-or$rs.avatarPending-or!(CatalogReady $ds)-or$ds.session.rearms-ne$before[$r].doors.session.rearms+1){$ready=$false};if(!$rs.held-and$w.ElapsedMilliseconds-gt6000-and$br.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-$lastPress[$r]-gt700){I $r A 100;$lastPress[$r]=$w.ElapsedMilliseconds}}
   if(!$ready){Start-Sleep -Milliseconds 70}
  }while(!$ready-and$w.ElapsedMilliseconds-lt50000)
  if(!$ready){throw 'Continuous recovery did not finish'}
  $after=Pair;Save "$cycleDir/after.json" $after
  foreach($r in 'host','client'){
   $a=$after[$r];$b=$before[$r];$rs=$a.reset;$d=$a.doors
   if($rs.arm-ne$sessionBefore[$r].reset.arm-or$rs.peerArm-ne$sessionBefore[$r].reset.peerArm-or$rs.digest-ne$sessionBefore[$r].reset.digest){throw 'Checkpoint session required rearming'}
   if($rs.loads-ne$b.reset.loads+1-or$rs.completed-ne$b.reset.completed+1-or$rs.aborted-ne$b.reset.aborted-or$rs.maxDistance-gt.001-or$rs.avatarRestored-ne$b.reset.avatarRestored+1-or$rs.avatarRefused-ne$b.reset.avatarRefused){throw 'Unexpected recovery/load/counterpart count'}
   if($a.bridge.localEpoch-eq$b.bridge.localEpoch-or!$a.bridge.probeFollowEnabled-or$d.session.reloads-ne$b.doors.session.reloads+1-or$d.session.retiredChannels-ne$b.doors.session.retiredChannels+6){throw 'New world and catalog recovery absent'}
   $oldCalls=($retiring[$r].doors.entries|Measure-Object nativeCalls -Sum).Sum;if($d.session.retiredNativeCalls-ne$b.doors.session.retiredNativeCalls+$oldCalls){throw 'Retired native queue count differs'}
   foreach($old in $b.doors.entries){$new=@($d.entries|Where-Object name -eq $old.name)[0];if($new.arm-le$old.arm-or$new.nativeCalls-ne0-or$new.revision-ne1-or$new.applied-ne1){throw 'New catalog reused prior-world actions'}}
   $entities=Q $r '--entities';$avatars=@($entities.entities|Where-Object name -match '^d2coop_probe_');$char=Q $r '--character';Save "$cycleDir/role-avatar-$r.json" @{character=$char;avatars=$avatars};if($char.character-ne$(if($r-eq'host'){1}else{2})-or$char.queued-or$avatars.Count-ne1-or$avatars[0].name-ne$(if($r-eq'host'){'d2coop_probe_emily'}else{'d2coop_probe_corvo'})-or$avatars[0].handle-ne('0x'+([uint32]$a.bridge.probeActorHandle).ToString('x'))){throw 'Assigned role or single counterpart not restored'}
   BindDoor $r;if((NativeDoor $r).current-ne0){throw 'Old open request replayed into checkpoint'}
   $sw=S $r '.loading.json';Save "$cycleDir/loading-after-$r.json" $sw;$sb=Get-Content "$cycleDir/loading-before-$r.json" -Raw|ConvertFrom-Json;$events=@($sw.events|Where-Object sequence -gt $sb.switches);if($sw.switches-$sb.switches-gt32-or@($events|Where-Object {$_.caller-eq'0x787a55'-and$_.to-eq15}).Count-ne1-or@($events|Where-Object {$_.caller-eq'0x7879cd'-and$_.to-eq1}).Count-ne1){throw 'Expected exactly one actual native load entry/finish'}
   $iso=S $r '.observer-isolation.json';$npcs=Q $r '--world-npcs';$ramsey=@($npcs.npcs|Where-Object spawnerName -eq 'ld_spawner_ramsey_escape');if($ramsey.Count-ne1){throw 'Ramsey native identity ambiguous'};$ai=Q $r '--ai-decision' $ramsey[0].handle;Save "$cycleDir/postload-ai-$r.json" @{isolation=$iso;decision=$ai};if($iso.active-or$iso.heldReferences-or!$ai.started-or!$ai.schedulerActive-or$ai.pauseReferences){throw 'Native AI not released by recovery'}
   if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $a.bridge.probeActorHandle).current-ne40){throw 'Recovered health differs'}
  }
  if($after.host.reset.transaction-ne$after.client.reset.transaction-or$after.host.reset.releasedAt-lt$after.client.reset.readyAt-or$after.client.reset.releasedAt-lt$after.host.reset.readyAt){throw 'Both-ready release mismatch'}
  # Reacquire only the independent finite NPC fixture for the next stimulus;
  # never rearm checkpoint or door intent inside this loop.
  foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000'}
  Place $source $peer "$cycleDir/after";I $source X 180;$used=WaitDoor 2;Save "$cycleDir/fresh-door-use.json" $used
  if($used[$source].door.nativeCalls-ne0-or$used[$peer].door.nativeCalls-ne1-or$used[$source].door.revision-ne2-or$used[$peer].door.revision-ne2){throw 'Fresh post-recovery interaction did not apply exactly once'}
  Save "$cycleDir/observations.json" $samples
  foreach($r in 'host','client'){$cap=$cycleCaptures[$r];$cap.WaitForExit(40000)|Out-Null;if(!$cap.HasExited-or$cap.ExitCode){throw 'Native paired recording incomplete'}}
  $row=@{id=$spec.id;source=$source;death=$spec.death;pauseSurvivor=$spec.pauseSurvivor;passed=$true;transaction=$after.host.reset.transaction;hostEpoch=$after.host.bridge.localEpoch;clientEpoch=$after.client.bridge.localEpoch;rearms=$after.host.doors.session.rearms};Save "$cycleDir/result.json" $row;$cycles.Add($row);"PASS_CONTINUOUS_CYCLE $($spec.id)"
 }
 $checks.Add('Four consecutive recoveries under unchanged checkpoint arms and continuously retained door-session IDs, alternating manual reloads and natural fall deaths')
 $checks.Add('Both native death screens, untouched survivor health, blocked survivor movement and one coordinated retry per actual death')
 $checks.Add('One real load and one counterpart per role per cycle; new epochs and six fresh bindings, no old door replay, fresh native use after every recovery')
 Save "$out/session-after.json" (Pair)
}catch{$failure=$_.Exception.Message;if($cycleDir-and(Test-Path $cycleDir)){Save "$cycleDir/failed-observations.json" $samples;Save "$cycleDir/failed-pair.json" (Pair)}}finally{
 if($owned){
  foreach($r in 'host','client'){try{I $r}catch{}}
  foreach($cap in $captures){$cap.WaitForExit(40000)|Out-Null;if(!$cap.HasExited-or$cap.ExitCode){$failure=($failure+'; capture incomplete').TrimStart('; ')}}
  foreach($r in 'host','client'){try{C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';C $r 'coop_door_sync off';C $r 'coop_movement_fixture off'}catch{}}
  if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline $out -Evidence "$out/note-restoration";foreach($r in 'host','client'){$restored=Q $r '--player-inventory';Save "$out/inventory-restored-$r.json" $restored;if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete inventory differs'};if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$checks.Add('Complete inventories and player/counterpart health restored')}catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path $out){Save "$out/result.json" @{passed=(!$failure);failure=$failure;dll=$dll;candidate=$Candidate;checks=$checks;cycles=$cycles;scope='Four continuous same-map fixed-SLOT82 recoveries without rearming checkpoint/door sessions. Real native fall deaths and survivor pause; both native game-over UI, one retry, restored roles/counterparts, fresh shared doors. Finite NPC isolation used between stimuli, independently released/rechecked after each load. No general campaign/save or shared enemy authority claim.'}}
  Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='ContinuousRecovery778';take=$Take;state='finished';ended=(Get-Date).ToString('o')};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_CONTINUOUS_RECOVERY $($checks.Count) checks"
