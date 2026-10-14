param([string]$Take='npc-native-perception-v985')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$handles=@{};$captures=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 22|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){Get-Process -Id $pids[$r] -ErrorAction Stop|Out-Null;Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw "Native query failed $r $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v983/identity.json" -Raw|ConvertFrom-Json
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
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 C client 'setviewpos -42.9563 -122 62.7 90 0'
 $watch.Restart();$selected=0;$attacking=0
 do{
  $allTargets=@();foreach($eh in $enemyHandles){$allTargets+=Q host '--ai-targets' $eh}
  $targets=Q host '--ai-targets' $handles.host;$attack=Q host '--ai-attack' $handles.host;$pose=Q host '--npc-pose' $handles.host;$bridge=S host
  $row=@{ms=$watch.ElapsedMilliseconds;allTargets=$allTargets;targets=$targets;attack=$attack;pose=$pose;bridge=$bridge;avatarHealth=(Q host '--health' $avatar);clientPlayerHealth=(Q client '--health' 1)};$samples.Add($row)
  if(@($allTargets|Where-Object {$_.combat.resolved-and$_.combat.handle-eq$avatar}).Count){$selected++};if($attack.schedulerActive){$attacking++}
  if($row.avatarHealth.current-le0){break}
  Start-Sleep -Milliseconds 100
 }while($watch.ElapsedMilliseconds-lt8500)
 Save "$out/native-samples.json" @($samples)
 foreach($r in 'host','client'){Save "$out/damage-$r.json" (S $r '.damage.json');Save "$out/stream-$r.json" (S $r '.npc-stream.json');Save "$out/player-after-$r.json" (Q $r '--health' 1)}
 Save "$out/guard-faction-after.json" (Q host '--faction' $handles.host)
 foreach($capture in $captures){if(!$capture.WaitForExit(15000)-or$capture.ExitCode){throw 'Recording did not finish before cleanup'}}
 foreach($r in 'host','client'){Save "$out/perception-after-$r.json" (S $r '.avatar-perception.json')}
 $ap=S host '.avatar-perception.json';if(!$ap.enabled-or!$ap.installed-or$ap.retained-lt100-or!$ap.avatarAiHeld){throw 'Perception retention not active or owned clone AI was resumed'}
 if(!(S host).probeFollowEnabled){throw 'Remote following must remain enabled'}
 if($selected-lt1){throw 'No native enemy selected the remote avatar during this bounded encounter'}
 Save "$out/verified.json" @{passed=$true;selectedSamples=$selected;attackSamples=$attacking;partner=$avatar;guard=$handles.host;nativeOverrideUsed=$false;scope='Candidate983 owned continuous-transform observable retention. Native host NPC combat-target selection of remote Doppelganger with ordinary Player faction, while NPC root/body stream owns client AI. Attack scheduler observations are separate from landed damage and enemy-to-client health relay; those are not accepted by this test.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};Save "$out/native-samples.json" @($samples)}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($p in $captures){try{if(!$p.WaitForExit(20000)-or$p.ExitCode){throw 'Recording failed'}}catch{$errors.Add($_.Exception.Message)}}
  if($dirty){
   foreach($r in 'host','client'){foreach($cmd in 'coop_avatar_perception off','coop_shadow_stream off','coop_npc_stream off','coop_damage_observe off'){try{C $r $cmd}catch{$errors.Add("${r}: $($_.Exception.Message)")}}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{
   $s=S $r;$n=S $r '.npc-stream.json';$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;stream=$n;health=$hp}
   if($s.localGameLogicState-ne1-or!$s.remoteFresh-or$n.enabled-or$n.session.lost-or@($n.entries|Where-Object {$_.references-or$_.lost}).Count-or$hp.current-ne56){throw 'Final gameplay/health/ownership not restored'}
  }catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;candidate=$pin.dll;scope='Bounded native enemy perception of remote player, no custom faction override or enemy damage relay.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
