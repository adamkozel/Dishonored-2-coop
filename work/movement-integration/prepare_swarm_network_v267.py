from pathlib import Path
P=Path(__file__).resolve().parents[2];N=P/'work/natural-moveset'
s=(N/'Observe-NativeSwarm-v265.ps1').read_text()
s=s.replace('$capture=$null;$monitor=$null;', '$recorders=@();$capture=$null;$monitor=$null;')
s=s.replace("C host 'setviewpos -48.59 -155.07 68.1 90 0';", "C host 'setviewpos -48.59 -155.07 68.1 90 0';C client 'setviewpos -44.39 -153.27 68.1 203 0';\n & \"$PSScriptRoot/Set-ObserverPitch.ps1\" -Role client -Pitch -3 -Evidence \"$out/observer-camera.json\" -Quick\n")
needle=" $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();"
insert=''' Copy-Item "$PSScriptRoot/swarm-banks-v267/corvo-swarm-bank.bin" "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';$stream=S client '.shadow-stream.json';if($m.active-and$m.version-eq49-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne49-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Swarm bank/native Steam stream not ready'}
 $checks.Add('Remote Corvo uses exact bank49/304 and native Steam pose transport')
'''
assert needle in s;s=s.replace(needle,insert+needle)
s=s.replace('"$PSScriptRoot/$Take.mp4")','"$PSScriptRoot/$Take-source.mp4")')
needle=' Start-Sleep -Milliseconds 600;if($capture.HasExited)'
insert=''' $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Observer window missing'}
 $idx=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}));if($idx-lt0){throw 'Capture argument absent'}
 $args[$idx]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
'''
assert needle in s;s=s.replace(needle,insert+' Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count)')
s=s.replace("'--watch-midnight'","'--watch-swarm-scenes'").replace('midnight.ndjson','power-scenes.ndjson').replace('midnight.stderr','power-scenes.stderr')
a=s.index("  $entities=Q host '--entities'");b=s.index('  Start-Sleep -Milliseconds 30',a)
s=s[:a]+'''  $m=S client '.movement.json';$stream=S client '.shadow-stream.json'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q host '--player-states');movement=$m;stream=$stream})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw "Native transport/presentation retired: $($stream.result)"}
''' +s[b:]
a=s.index(' $scenes=@($samples|');b=s.index(" C host 'coop_swarm_fixture off'",a)
s=s[:a]+''' $scenes=@($samples|Where-Object valid)
 foreach($v in 23,24,25){
  if(!@($scenes|Where-Object variant -eq $v).Count){throw "Strict native Swarm phase $v absent"}
  if(!@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}).Count){throw "Remote Swarm phase $v absent"}
 }
 if(@($scenes|Where-Object {!$_.armHandle-or!$_.equipmentHandle}).Count-or@($scenes.armHandle|Sort-Object -Unique).Count-ne1-or@($scenes.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Source Swarm ownership changed'}
 $checks.Add('All three exact native phases with reciprocal Corvo arm/body ownership drive remote full-body phases')
 $phaseCoverage=@(foreach($v in 23,24,25){
  $src=@($scenes|Where-Object variant -eq $v|ForEach-Object handle|Sort-Object -Unique)
  $dst=@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Sort-Object -Unique)
  if($src.Count-ne1-or$dst.Count-ne1){throw "Phase $v incarnation mismatch $($src.Count)/$($dst.Count)"}
  [pscustomobject]@{variant=$v;nativeHandles=$src;remoteEvents=$dst}
 })
 $phaseCoverage|ConvertTo-Json -Depth 8|Set-Content "$out/phase-incarnations.json"
 $checks.Add('Native phase changes produce one corresponding remote semantic event per phase')
 $continuity=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le6200})
 if($continuity.Count-lt20-or@($continuity|Where-Object {$_.movement.weight-lt.95-or$_.movement.clockRejection.count-ne0}).Count){throw 'Swarm root/body continuity failed'}
 [ordered]@{samples=$continuity.Count;minimumBodyWeight=($continuity.movement.weight|Measure-Object -Minimum).Minimum;rejections=@($continuity.movement.clockRejection.count|Sort-Object -Unique)}|ConvertTo-Json|Set-Content "$out/continuity.json"
 $checks.Add('Swarm cast preserves root/body continuity without rejected presentation clocks')
 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}
 $sword=Q client '--avatar-sword' (S client).probeActorHandle;$sword|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if(!$sword.equipped-or!(Q host '--player-sword').equipped){throw 'Sword selection changed'}
 $checks.Add('Remote body recovers to standing with native sword selection retained')
 $sourceEntities=Q host '--entities';$peerEntities=Q client '--entities'
 $nativeRats=@($sourceEntities.entities|Where-Object {$_.definitionName-match'^models/characters/animals/rat/devouring_swarm(_big)?\\.def$'})
 $peerRats=@($peerEntities.entities|Where-Object {$_.definitionName-match'^models/characters/animals/rat/devouring_swarm(_big)?\\.def$'})
 [ordered]@{source=$nativeRats;peer=$peerRats;scope='Source native rats are observational; body playback must not duplicate gameplay spawns'}|ConvertTo-Json -Depth 10|Set-Content "$out/swarm-world-observation.json"
 if(!$nativeRats.Count-or$peerRats.Count){throw 'Expected native source rats only; missing source or duplicated peer swarm'}
 $checks.Add('Native source swarm remains source-owned; body playback creates no peer rats or duplicate power actions')
 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/swarm-scenes.json"
''' +s[b:]
s=s.replace("foreach($r in 'host','client'){try{I $r;C $r 'coop_swarm_fixture off'}", "foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_swarm_fixture off'}")
s=s.replace('foreach($job in @($capture,$monitor))','foreach($job in (@($recorders)+@($monitor)))')
s=s.replace("scope='Native source Corvo Swarm acquisition, controller input, raw scene clocks/state/equipment and spawned entity observations. AI held in the isolated Dunwall fixture; neither general swarm behavior nor remote movement/VFX/gameplay verified. Checkpoint and protected save restoration mandatory.'", "bankSha256=(Get-FileHash \"$PSScriptRoot/swarm-banks-v267/corvo-swarm-bank.bin\").Hash;scope='Standing armed native Corvo Swarm phases drive remote full-body bank49 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, alternate contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory.'")
(N/'Test-SwarmNetwork-v267.ps1').write_text(s)
p=P/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text();s=s.replace("'SwarmObserve',","'SwarmObserve','SwarmNetwork',")
needle="  'SwarmObserve'{";assert needle in s;s=s.replace(needle,"  'SwarmNetwork'{& \"$root/work/natural-moveset/Test-SwarmNetwork-v267.ps1\" -Take $Take -Pitch $Pitch}\n"+needle);p.write_text(s)
print('SWARM_NETWORK_HARNESS_READY')
