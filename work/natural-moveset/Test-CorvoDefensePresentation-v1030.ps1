param([string]$Take='corvo-defense-presentation-v1030',[int]$OnMs=1200,[int]$OffMs=320,[int]$DurationMs=25000)
# Both-role parity for candidate1026 defense presentation: Corvo (host) defends
# natively against host-owned enemies (no relay); the client renders remote
# Corvo with bank68 and must present the native block impact (variants19/20,
# clip357) and timed parry (21, clip358). Client observer camera films Corvo.
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
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1026/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Wrong candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}
  Save "$out/inventory-before-$r.json" (Q $r '--player-inventory');Save "$out/health-before-$r.json" (Q $r '--health' 1)
 }
 $bankEntry=Get-Content "$PSScriptRoot/parry-bank-manifest-v1022.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo'
 if((Get-FileHash "$root/work/lab-instances/client/coop-status.json.movement.bin").Hash-ne$bankEntry.sha256){throw 'Client Corvo bank68 not installed'}
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
 foreach($r in 'host','client'){C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'}
 C client 'coop_movement_bank on'
 $bw=[Diagnostics.Stopwatch]::StartNew();do{$m=S client '.movement.json';if($m.active-and$m.version-eq68){break};Start-Sleep -Milliseconds 40}while($bw.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne68){throw 'Client bank68 preload failed'}
 C host ("setviewpos -42.9563 -122 62.7 {0} 0"-f$faceYaw)
 # Client observer camera behind/beside remote Corvo, then client body re-parked.
 $oa=($faceYaw+150)*[Math]::PI/180;$ox=-42.9563+4*[Math]::Cos($oa);$oy=-122+4*[Math]::Sin($oa);$oyaw=[Math]::Atan2(-122-$oy,-42.9563-$ox)*180/[Math]::PI
 C client ("setviewpos {0:F3} {1:F3} 63.4 {2:F2} -6"-f$ox,$oy,$oyaw);Start-Sleep -Milliseconds 700
 C client 'coop_observer_camera hold';Start-Sleep -Milliseconds 300;$hold=S client '.observer-camera.json'
 C client 'setviewpos -33 -148 68.1 270 0';Start-Sleep -Milliseconds 300
 Save "$out/observer-camera.json" @{hold=$hold;position=@($ox,$oy,63.4);yaw=$oyaw}
 if(!$hold.active){throw "Client observer camera hold failed: $($hold.result)"}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]([int]($DurationMs/1000)+6),'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 $base=(S host '.npc-attack.json').defenseObserver;Save "$out/observer-before-host.json" $base
 $pwsh=(Get-Process -Id $PID).Path
 $sampler=Start-Process $pwsh -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Sample-StatusFile-v1023.ps1",'-Path',"$root/work/lab-instances/client/coop-status.json.movement.json",'-Out',"$out/client-movement.ndjson",'-DurationMs',$DurationMs) -WindowStyle Hidden -PassThru
 $driver=Start-Process $pwsh -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Drive-BlockCycle-v1017.ps1",'-Role','host','-OnMs',$OnMs,'-OffMs',$OffMs,'-TotalMs',$DurationMs,'-Log',"$out/input-cycle.json") -WindowStyle Hidden -PassThru
 $watch.Restart();$lastAim=-1000
 do{
  $view=Q host '--player';$engaged=@()
  foreach($e in $enemies){$t=Q host '--ai-targets' $e;if($t.combat.resolved-and$t.combat.handle-eq1){$pose=Q host '--npc-pose' $e;$dx=$pose.root[0]-$view.candidatePosition[0];$dy=$pose.root[1]-$view.candidatePosition[1];$engaged+=@{handle=$e;dx=$dx;dy=$dy;d=[Math]::Sqrt($dx*$dx+$dy*$dy)}}}
  $closest=$engaged|Sort-Object d|Select-Object -First 1
  if($closest-and$watch.ElapsedMilliseconds-$lastAim-gt700){C host ("setviewpos -42.9563 -122 62.7 {0} 0"-f([Math]::Atan2($closest.dy,$closest.dx)*180/[Math]::PI));$lastAim=$watch.ElapsedMilliseconds}
  $obs=(S host '.npc-attack.json').defenseObserver;$hp=Q host '--health' 1
  $row=@{ms=$watch.ElapsedMilliseconds;engaged=$engaged;observer=$obs;health=$hp;states=(Q host '--player-states')};$samples.Add($row)
  $defended=($obs.native[0]-$base.native[0])+($obs.native[1]-$base.native[1])+($obs.native[2]-$base.native[2])
  if(($obs.native[1]-$base.native[1])-ge2-and($obs.native[0]-$base.native[0])-ge1){break}
  if($hp.current-le16){break}
  Start-Sleep -Milliseconds 100
 }while($watch.ElapsedMilliseconds-lt$DurationMs)
 if($driver-and!$driver.HasExited){Stop-Process -Id $driver.Id};$driver=$null
 & "$lab/Send-D2Input.ps1" -Role host -DurationMs 100|Out-Null
 C host 'setviewpos -33 -153 68.1 90 0'
 Save "$out/native-samples.json" @($samples)
 if($sampler-and!$sampler.WaitForExit($DurationMs+10000)){Stop-Process -Id $sampler.Id};$sampler=$null
 foreach($capture in $captures){if(!$capture.WaitForExit($DurationMs+20000)-or$capture.ExitCode){throw 'Recording did not finish before cleanup'}}
 $after=(S host '.npc-attack.json').defenseObserver;Save "$out/observer-after-host.json" $after
 $nativeOutcomes=@{block=$after.native[0]-$base.native[0];parryLose=$after.native[1]-$base.native[1];parryWin=$after.native[2]-$base.native[2];relayed=@(($after.relayed[0]-$base.relayed[0]),($after.relayed[1]-$base.relayed[1]),($after.relayed[2]-$base.relayed[2]))}
 $presented=@(Get-Content "$out/client-movement.ndjson"|ForEach-Object {($_|ConvertFrom-Json).status}|Where-Object {$_.combatVariant-ge19-and$_.combatVariant-le21})
 $shown=@{impact=@($presented|Where-Object {$_.combatVariant-le20-and$_.combatClip-eq357-and$_.combatWeight-gt.5}).Count;parry=@($presented|Where-Object {$_.combatVariant-eq21-and$_.combatClip-eq358-and$_.combatWeight-gt.5}).Count;all=$presented.Count;wrongClip=@($presented|Where-Object {$_.combatClip-ne($(if($_.combatVariant-eq21){358}else{357}))}).Count}
 Save "$out/presentation-results.json" @{native=$nativeOutcomes;shown=$shown}
 if(($nativeOutcomes.block+$nativeOutcomes.parryLose)-lt1){throw 'No native host block or parry occurred'}
 if(@($nativeOutcomes.relayed|Where-Object {$_}).Count){throw 'Unexpected relayed defense on host'}
 if(($nativeOutcomes.block-and!$shown.impact)-or($nativeOutcomes.parryLose-and!$shown.parry)-or$shown.wrongClip){throw 'Corvo native defense scene did not reach the client remote body as clips357/358'}
 Save "$out/verified.json" @{passed=$true;scope='Candidate1026 both-role parity: host Corvo native block impact / timed parry against host-native enemies presented on the client remote Corvo body (bank68 clips357/358) through Steam protocol70; client observer camera films remote Corvo.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};Save "$out/native-samples.json" @($samples)}
finally{
 if($driver-and!$driver.HasExited){try{Stop-Process -Id $driver.Id}catch{}}
 if($sampler-and!$sampler.HasExited){try{Stop-Process -Id $sampler.Id}catch{}}
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($p in $captures){try{if(!$p.WaitForExit($DurationMs+20000)-or$p.ExitCode){throw 'Recording failed'}}catch{$errors.Add($_.Exception.Message)}}
  if($dirty){
   try{& "$lab/Send-D2Input.ps1" -Role host -DurationMs 100|Out-Null}catch{$errors.Add($_.Exception.Message)}
   foreach($r in 'host','client'){foreach($cmd in 'coop_observer_camera off','coop_movement_bank off','coop_shadow_stream off','coop_npc_stream off','coop_damage_observe off'){try{C $r $cmd}catch{$errors.Add("${r}: $($_.Exception.Message)")}}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{
   $s=S $r;$n=S $r '.npc-stream.json';$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;stream=$n;health=$hp}
   if($s.localGameLogicState-ne1-or!$s.remoteFresh-or$n.enabled-or$n.session.lost-or@($n.entries|Where-Object {$_.references-or$_.lost}).Count-or$hp.current-ne56){throw 'Final gameplay/health/ownership not restored'}
  }catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;candidate=$pin.dll;scope='Corvo-defender parity for native defense presentation; restored isolated lab.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
