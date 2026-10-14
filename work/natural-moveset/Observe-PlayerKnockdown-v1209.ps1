param([string]$Take='native-player-knockdown-v1209',[int]$AttackMs=150,[int]$GapMs=650,[int]$DurationMs=20000)
# 1209 Stage 3 stagger_knockdown_recovery / counterattack - NATIVE OBSERVATION ONLY:
# host Corvo attacks the arena enemies continuously (Drive-AttackCycle-v1209,
# single input owner); guards that parry the player can blow Corvo back
# (PLAYER_HITREACT_KNOCKDOWN). --watch-player-states and --watch-combat-scene
# record every native state/scene change. Earlier header (1208):
# 1208 Stage 3 counterattack - NATIVE OBSERVATION ONLY (no presentation change):
# host Corvo in the 1030 arena against host-owned guards/Ramsey runs the 1030
# block cycle (1200/320 ms; 1208's 700 ms blocks + attacks lost 45 health in
# 4.6 s) and attacks (RightTrigger 150 ms) as soon as the native defense
# observer reports a timed parry, i.e. into the off-balance attacker. The read-only
# --watch-combat-scene probe records every native player midnight scene (asset,
# subscene, duration, bindings); the defense observer counts block/parry
# outcomes. The question: which native scene a counterattack plays.
# Earlier header (1030): Both-role parity for candidate1026 defense presentation.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new();$driver=$null;$sampler=$null
function Save($p,$v){$v|ConvertTo-Json -Depth 22|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){Get-Process -Id $pids[$r] -ErrorAction Stop|Out-Null;Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw "Native query failed $r $mode"}
function HandleOf($h){[Convert]::ToUInt32($h.Replace('0x',''),16)}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=@{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}
  Save "$out/inventory-before-$r.json" (Q $r '--player-inventory');Save "$out/health-before-$r.json" (Q $r '--health' 1)
 }
 $dirty=$true
 # Client parked away from the encounter; host takes the arena position.
 C client 'setviewpos -33 -148 68.1 270 0'
 foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_damage_observe 60000';C $r 'coop_npc_stream on'}
 $watch=[Diagnostics.Stopwatch]::StartNew();do{
  $ready=$true;foreach($r in 'host','client'){$n=S $r '.npc-stream.json';if(!$n.enabled-or!$n.entries.Count-or@($n.entries|Where-Object {!$_.enabled-or!$_.ready}).Count){$ready=$false}}
  if(!$ready){Start-Sleep -Milliseconds 80}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt8000)
 if(!$ready){throw 'Native NPC stream did not become ready'}
 $world=Q host '--world-npcs';Save "$out/world-before-host.json" $world
 $boundHandles=@((S host '.npc-stream.json').entries.handle)
 $enemies=@($world.npcs|Where-Object {($_.spawnerName -like '*guard*' -or $_.definition -like '*mortimer_ramsey*') -and ((HandleOf $_.handle) -in $boundHandles)}|ForEach-Object {HandleOf $_.handle})
 if($enemies.Count-lt3){throw 'At least three bound native enemies required'}
 $ramsey=@($world.npcs|Where-Object {$_.definition -like '*mortimer_ramsey*' -and ((HandleOf $_.handle) -in $boundHandles)});if($ramsey.Count-ne1){throw 'Unique Ramsey required'}
 $rp=Q host '--npc-pose' (HandleOf $ramsey[0].handle);$faceYaw=[Math]::Atan2($rp.root[1]+122,$rp.root[0]+42.9563)*180/[Math]::PI
 C host ("setviewpos -42.9563 -122 62.7 {0} 0"-f$faceYaw)
 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]([int]($DurationMs/1000)+6),'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 $scenes=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-combat-scene',[string]([Math]::Min($DurationMs,20000))) -WindowStyle Hidden -RedirectStandardOutput "$out/host-combat-scene.json" -RedirectStandardError "$out/host-scenes.stderr" -PassThru
 $states=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-player-states',[string]([Math]::Min($DurationMs,20000))) -WindowStyle Hidden -RedirectStandardOutput "$out/host-player-states.json" -RedirectStandardError "$out/host-states.stderr" -PassThru
 $base=(S host '.npc-attack.json').defenseObserver;Save "$out/observer-before-host.json" $base
 $pwsh=(Get-Process -Id $PID).Path
 $driver=Start-Process $pwsh -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Drive-AttackCycle-v1209.ps1",'-Role','host','-AttackMs',$AttackMs,'-GapMs',$GapMs,'-TotalMs',$DurationMs,'-Log',"$out/input-cycle.json") -WindowStyle Hidden -PassThru
 $watch.Restart();$lastAim=-1000;$lastParry=$base.native[1];$lastBlock=$base.native[0];$counters=[Collections.Generic.List[object]]::new()
 do{
  $view=Q host '--player';$engaged=@()
  foreach($e in $enemies){$t=Q host '--ai-targets' $e;if($t.combat.resolved-and$t.combat.handle-eq1){$pose=Q host '--npc-pose' $e;$dx=$pose.root[0]-$view.candidatePosition[0];$dy=$pose.root[1]-$view.candidatePosition[1];$engaged+=@{handle=$e;dx=$dx;dy=$dy;d=[Math]::Sqrt($dx*$dx+$dy*$dy)}}}
  $closest=$engaged|Sort-Object d|Select-Object -First 1
  if($closest-and$watch.ElapsedMilliseconds-$lastAim-gt700){C host ("setviewpos -42.9563 -122 62.7 {0} 0"-f([Math]::Atan2($closest.dy,$closest.dx)*180/[Math]::PI));$lastAim=$watch.ElapsedMilliseconds}
  $obs=(S host '.npc-attack.json').defenseObserver
  $hp=Q host '--health' 1
  $row=@{ms=$watch.ElapsedMilliseconds;engaged=$engaged;observer=$obs;health=$hp;states=(Q host '--player-states')};$samples.Add($row)
  $defended=($obs.native[0]-$base.native[0])+($obs.native[1]-$base.native[1])+($obs.native[2]-$base.native[2])
  if($hp.current-le16){break}
  Start-Sleep -Milliseconds 100
 }while($watch.ElapsedMilliseconds-lt$DurationMs)
 if($driver-and!$driver.HasExited){Stop-Process -Id $driver.Id};$driver=$null
 & "$lab/Send-D2Input.ps1" -Role host -DurationMs 100|Out-Null
 C host 'setviewpos -33 -153 68.1 90 0'
 Save "$out/native-samples.json" @($samples)
 if(!$scenes.WaitForExit(30000)){Stop-Process -Id $scenes.Id};if(!$states.WaitForExit(30000)){Stop-Process -Id $states.Id}
 $stateIds=@{};try{foreach($x in (Get-Content "$out/host-player-states.json" -Raw|ConvertFrom-Json).records){foreach($a in $x.active){$k="$($a.id)";if(!$stateIds[$k]){$stateIds[$k]=@{count=0;vtable=$a.vtableRva;firstMs=$x.ms}};$stateIds[$k].count++}}}catch{}
 foreach($capture in $captures){if(!$capture.WaitForExit($DurationMs+20000)){throw 'Recording did not finish before cleanup'}}
 $after=(S host '.npc-attack.json').defenseObserver;Save "$out/observer-after-host.json" $after
 $nativeOutcomes=@{block=$after.native[0]-$base.native[0];parryLose=$after.native[1]-$base.native[1];parryWin=$after.native[2]-$base.native[2]}
 $census=@{};try{$rec=(Get-Content "$out/host-combat-scene.json" -Raw|ConvertFrom-Json).records;foreach($x in $rec){$k=$x.sample.asset;if(!$k){continue};if(!$census[$k]){$census[$k]=@{count=0;variant=$x.sample.variant;duration=$x.sample.duration;state=$x.sample.state;firstMs=$x.ms}};$census[$k].count++}}catch{}
 Save "$out/scene-census.json" @{native=$nativeOutcomes;scenes=$census;states=$stateIds}
 Save "$out/verified.json" @{passed=$true;scope='Native attack-spree observation: host player state and combat scene census recorded.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};Save "$out/native-samples.json" @($samples)}
finally{
 if($driver-and!$driver.HasExited){try{Stop-Process -Id $driver.Id}catch{}}
 if($sampler-and!$sampler.HasExited){try{Stop-Process -Id $sampler.Id}catch{}}
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($p in $captures){try{if(!$p.WaitForExit($DurationMs+20000)-or$p.ExitCode){throw 'Recording failed'}}catch{$errors.Add($_.Exception.Message)}}
  if($dirty){
   try{& "$lab/Send-D2Input.ps1" -Role host -DurationMs 100|Out-Null}catch{$errors.Add($_.Exception.Message)}
   foreach($r in 'host','client'){foreach($cmd in 'coop_npc_stream off','coop_damage_observe off'){try{C $r $cmd}catch{$errors.Add("${r}: $($_.Exception.Message)")}}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{
   $s=S $r;$n=S $r '.npc-stream.json';$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;stream=$n;health=$hp}
   if($s.localGameLogicState-ne1-or!$s.remoteFresh-or$n.enabled-or$n.session.lost-or@($n.entries|Where-Object {$_.references-or$_.lost}).Count-or$hp.current-ne56){throw 'Final gameplay/health/ownership not restored'}
  }catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;candidate=$pin.dll;scope='Native player attack-spree observation (knockdown/blowback search) in the Dunwall arena; restored isolated lab.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
