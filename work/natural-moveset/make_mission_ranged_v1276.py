"""Derives Test-MissionRanged-v1276.ps1 from Test-MissionMultiCombat-v1270d.ps1 (Edge of the World under the product
controller, modal clearing, Dunwall fixture restoration) for stage 4 (#14): real ranged items on a mission. Each source
(host Corvo shoots the first checkpoint guard, client Emily the second) stands $Range m from its target, selects the
quick slot ($Slot, D-pad; Up is sleep darts on Corvo's Edge of the World HUD) and fires with LT. Sleep darts
(bolt_tranquilizer) are not replayable: the host applies its own natively (1214w) and the client's arrives as a
PeerNative request (1214z). Both windows are filmed per shot. Pass (runtime): each target leaves the alive state in the
host world and ends in the same state in both worlds."""
from pathlib import Path
P = Path(__file__).resolve().parent
s = P.joinpath('Test-MissionMultiCombat-v1270d.ps1').read_bytes().decode('utf-8'); nl = '\r\n' if '\r\n' in s else '\n'
lines = s.split(nl)
def idx(prefix):
    for i, l in enumerate(lines):
        if l.startswith(prefix): return i
    raise AssertionError(prefix)
fs, fe = idx('function Fight{'), idx('function WaitGameplay(')
shoot = r'''function Shoot{
 $npcs=Living host;$anchor=@($npcs|Where-Object spawnerName -eq $Guard);if($anchor.Count-ne1){throw "Guard $Guard missing"};$ap=$anchor[0].position
 $group=@($npcs|Where-Object {$_.unique-and$_.spawnerName-match'guard|watch|officer|soldier'-and(Dist2 $_.position $ap)-lt16-and[Math]::Abs([double]$_.position[2]-[double]$ap[2])-lt3}|Sort-Object spawnerName)
 Save "$out/guards-before.json" @($group|ForEach-Object {@{spawner=$_.spawnerName;definition=$_.definition;position=$_.position;handle=$_.handle}})
 if($group.Count-lt$Sources.Count){throw "Only $($group.Count) guards near the anchor"}
 $shots=[Collections.Generic.List[object]]::new();$k=0
 foreach($src in $Sources){$target=$group[$k];$k++
  # 1276b: $Range m behind the guard along its facing (unaware, nothing in between; 1276a's client dart hit a pillar).
  $tn=@(Living $src|Where-Object spawnerName -eq $target.spawnerName)[0];$tp=$tn.position;$fx=[double]$tn.orientation[0];$fy=[double]$tn.orientation[1];$fl=[Math]::Max([Math]::Sqrt($fx*$fx+$fy*$fy),0.01);$fx/=$fl;$fy/=$fl;$yaw=[Math]::Atan2($fy,$fx)*180/[Math]::PI
  C $src ('setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0' -f ([double]$tp[0]-$Range*$fx),([double]$tp[1]-$Range*$fy),([double]$tp[2]+1.7),$yaw);Start-Sleep -Milliseconds 4000;ClearModals "placed-$src"
  # 1276k: the source aims from its own world's position of the target (1276j: the client's unbound guard patrolled elsewhere).
  # 1276f: wait until the target is bound and ready on both peers (1276e: never bound, its knockout stayed host-only).
  $th=[int64]$target.handle;$bw=[Diagnostics.Stopwatch]::StartNew();$boundReady=$false
  do{Start-Sleep -Milliseconds 500;$he=@((S host '.npc-stream.json').entries|Where-Object {[int64]$_.handle-eq$th});$ce=@((S client '.npc-stream.json').entries|Where-Object {[int64]$_.handle-eq$th})
   $boundReady=$he.Count-and$he[0].ready-and$he[0].enabled-and$ce.Count-and$ce[0].ready-and$ce[0].enabled}while(!$boundReady-and$bw.ElapsedMilliseconds-lt$(if($Action-eq'choke'){0}else{10000}))
  $bindWaitMs=$bw.ElapsedMilliseconds
  # 1276h: when the target stays unbound, record why (eligible catalog, world NPCs, bound set, player positions) on both peers.
  if(!$boundReady){foreach($q in 'host','client'){try{Q $q '--npc-authority'|ConvertTo-Json -Depth 8|Set-Content "$out/unbound-$src-$q-authority.json"}catch{}
    try{Q $q '--world-npcs'|ConvertTo-Json -Depth 8|Set-Content "$out/unbound-$src-$q-world.json"}catch{}
    try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json.npc-stream.json" -Destination "$out/unbound-$src-$q-stream.json"}catch{}
    try{$st=S $q;[ordered]@{probeRenderPosition=$st.probeRenderPosition;remotePosition=$st.remotePosition;player=(Q $q '--player').candidatePosition}|ConvertTo-Json|Set-Content "$out/unbound-$src-$q-positions.json"}catch{}}}
  $pitched=if($Action-eq'dart'){Pitch $src 6}else{$null}
  if($Action-eq'dart'){I $src $Slot 200;Start-Sleep -Milliseconds 1300}
  foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
   $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','9','-an','-c:v','libx264','-preset','veryfast','-crf','20','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/shot-$src-$r.mp4") -WindowStyle Hidden -RedirectStandardError "$out/shot-$src-$r-capture.log" -PassThru))}
  Start-Sleep -Milliseconds 500
  $before=[ordered]@{host=(Outcome host $target.spawnerName);client=(Outcome client $target.spawnerName)}
  # 1276c: 1276b's press fell into the placement autosave ("Saving"); the dart count proves each shot, retried up to 3 times.
  $ammo={param($r)$it=@((Q $r '--player-inventory').items|Where-Object {$_.name-like'*bolt_tranquilizer*'});if($it.Count){[int]$it[0].quantity}else{-1}}
  $presses=[Collections.Generic.List[object]]::new();$fired=$false
  # 1276l: Action choke places the source 0.9 m behind the guard (its own world) and holds RB through the native rear choke.
  if($Action-eq'choke'){$tn=@(Living $src|Where-Object spawnerName -eq $target.spawnerName)[0];$tp=$tn.position;$fx=[double]$tn.orientation[0];$fy=[double]$tn.orientation[1];$fl=[Math]::Max([Math]::Sqrt($fx*$fx+$fy*$fy),0.01);$fx/=$fl;$fy/=$fl;$yaw=[Math]::Atan2($fy,$fx)*180/[Math]::PI
   C $src ('setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0' -f ([double]$tp[0]-0.9*$fx),([double]$tp[1]-0.9*$fy),([double]$tp[2]+1.7),$yaw);Start-Sleep -Milliseconds 120;I $src RightShoulder 6500;$fired=$true;$presses.Add([pscustomobject]@{press=0;action='choke'})}
  for($pt=0;$pt-lt3-and!$fired;$pt++){$a0=& $ammo $src
   # 1276d: the patrolling guard moved during the autosave wait (1276c missed); re-place behind its current position.
   $tn=@(Living $src|Where-Object spawnerName -eq $target.spawnerName)[0];$tp=$tn.position;$fx=[double]$tn.orientation[0];$fy=[double]$tn.orientation[1];$fl=[Math]::Max([Math]::Sqrt($fx*$fx+$fy*$fy),0.01);$fx/=$fl;$fy/=$fl;$yaw=[Math]::Atan2($fy,$fx)*180/[Math]::PI
   # 1276e: $Front places the source in front of the guard, facing it (1276c/d: shots at its back from 3 m missed; 1276a's frontal shot hit).
   if($Front){C $src ('setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0' -f ([double]$tp[0]+$Range*$fx),([double]$tp[1]+$Range*$fy),([double]$tp[2]+1.7),($yaw+180))}else{
   C $src ('setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0' -f ([double]$tp[0]-$Range*$fx),([double]$tp[1]-$Range*$fy),([double]$tp[2]+1.7),$yaw)};Start-Sleep -Milliseconds 250
   & "$lab/Send-D2Input.ps1" -Role $src -LeftTrigger 255 -DurationMs 350|Out-Null;Start-Sleep -Milliseconds 1500
   $a1=& $ammo $src;$fired=($a0-gt0-and$a1-ge0-and$a1-lt$a0);$presses.Add([pscustomobject]@{press=$pt;before=$a0;after=$a1;fired=$fired})}
  # 1276b: poll both worlds until they agree on a changed state (a native dart takes seconds; the knockout then crosses the network).
  $timeline=[Collections.Generic.List[object]]::new();$sw=[Diagnostics.Stopwatch]::StartNew()
  do{Start-Sleep -Milliseconds 500;$o=[ordered]@{ms=$sw.ElapsedMilliseconds;host=(Outcome host $target.spawnerName);client=(Outcome client $target.spawnerName);hostLogic=(S host).localGameLogicState;clientLogic=(S client).localGameLogicState};$timeline.Add([pscustomobject]$o)}
  while($sw.ElapsedMilliseconds-lt16000-and!($o.host-ne'alive'-and$o.host-eq$o.client)-and$o.hostLogic-ne13-and$o.clientLogic-ne13)
  WaitCaptures
  $after=[ordered]@{host=$o.host;client=$o.client;ms=$o.ms;gameOver=($o.hostLogic-eq13-or$o.clientLogic-eq13)}
  $hc=S host '.npc-combat.json';$cc=S client '.npc-combat.json'
  $shots.Add([pscustomobject]@{source=$src;target=$target.spawnerName;boundReady=$boundReady;bindWaitMs=$bindWaitMs;pitched=$pitched;presses=@($presses);fired=$fired;before=$before;after=$after;timeline=@($timeline);
   host=($hc|Select-Object result,hostNativePassed,hostNativeLast,peerNativeApplied,hostWorldCommitted,committed,deaths,lastRetire,lastDecl);
   client=($cc|Select-Object result,peerNativeRequests,peerNativeBusy,hostWorldApplied,applied,deaths,lastRetire,lastDecl);
   hostLife=((S host '.npc-stream.json').relevance|Select-Object lifeSent,lifeReceived,lifeApplied,lifeRefused,bound,evicted);clientLife=((S client '.npc-stream.json').relevance|Select-Object lifeSent,lifeReceived,lifeApplied,lifeRefused,bound)})
  Save "$out/shots.json" @($shots)
  if($after.gameOver){break}
 }
 foreach($q in 'host','client'){foreach($suffix in '.npc-stream.json','.npc-combat.json','.product.json'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json$suffix" -Destination "$out/shots-$q$suffix" -ErrorAction Stop}catch{}}}
 $row=[ordered]@{shots=@($shots);passed=$false;failure=$null;checks=[Collections.Generic.List[string]]::new()}
 $bad=@($shots|Where-Object {$_.after.host-eq'alive'-or$_.after.host-ne$_.after.client})
 if(@($shots|Where-Object {$_.after.gameOver}).Count){$row.failure='A player died (Game Over); the shot is inconclusive'}
 elseif(@($shots|Where-Object {!$_.fired}).Count){$row.failure='The sleep dart was not fired (dart count unchanged after 3 presses)'}
 elseif($bad.Count){$row.failure=($bad|ForEach-Object {"$($_.source) shot $($_.target): host $($_.after.host), client $($_.after.client)"})-join '; '}
 else{$row.passed=$true;foreach($x in $shots){$row.checks.Add("$($x.source) shot $($x.target): $($x.after.host) in both worlds")}}
 $rows.Add([pscustomobject]$row)
}'''
body = lines[:fs] + shoot.split('\n') + lines[fe:]
s = nl.join(body)
def rep(old, new, count=1):
    global s
    assert s.count(old) == count, (s.count(old), old[:80]); s = s.replace(old, new)
rep("[string]$Take='mission-multicombat-v1270d',[string]$Candidate='candidate-npc-combat-v1214m',[string]$Guard='script_spawner_streets_windmill_guard_01',",
    "[string]$Take='mission-ranged-v1276m',[string]$Candidate='candidate-npc-combat-v1214ad',[string]$Guard='script_spawner_checkpoint_guard_01',[string[]]$Sources=@('host'),[double]$Range=5,[switch]$Front=$true,[string]$Slot='Up',[ValidateSet('dart','choke')][string]$Action='dart',")
rep("# 1270 (stage 4, #14): sustained multi-enemy fight on Edge of the World, both players at once. Generated by make_mission_multicombat_v1270.py",
    "# 1276 (stage 4, #14): real ranged items (sleep darts) on Edge of the World guards from both players. Generated by make_mission_ranged_v1276.py from 1270d;\n# 1270 (stage 4, #14): sustained multi-enemy fight on Edge of the World, both players at once. Generated by make_mission_multicombat_v1270.py".replace('\n', nl))
rep(" Fight" + nl, " Shoot" + nl)
rep("Save \"$out/identity.json\" @{candidate=$Candidate;dll=$pin.dll;guard=$Guard;fightMs=$FightMs}", "Save \"$out/identity.json\" @{candidate=$Candidate;dll=$pin.dll;guard=$Guard;sources=$Sources;range=$Range;slot=$Slot}")
i = s.index("scope='Stage 4 sustained multi-enemy fight on Edge of the World"); j = s.index("'", i + 7)
s = s[:i] + "scope='Stage 4 real ranged items on a mission: on Edge of the World under the product controller each source fires the quick-slot item (sleep darts) at its own checkpoint guard; every target leaves the alive state and ends in the same state in both worlds; Dunwall fixture restored." + s[j:]
P.joinpath('Test-MissionRanged-v1276.ps1').write_bytes(s.encode('utf-8'))
print('ok')
