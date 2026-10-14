from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeDominoLinks-v221.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$capture=$null;', '$peerCapture=$null;$networkStarted=$false;$peerRows=[Collections.Generic.List[object]]::new();$capture=$null;',1)
s=s.replace("'-t','16'","'-t','30'").replace('"$PSScriptRoot/$Take.mp4"','"$PSScriptRoot/$Take-source.mp4"')
key=' Start-Sleep -Milliseconds 600;if($capture.HasExited)'
assert key in s
s=s.replace(key,''' $peerArgs=@($args);$peerHwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $inputIndex=[Array]::IndexOf($peerArgs,($peerArgs|Where-Object {$_ -like 'gfxcapture=*'}));if($inputIndex-lt0-or!$peerHwnd){throw 'Peer capture input unavailable'}
 $peerArgs[$inputIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$peerArgs[-1]="$PSScriptRoot/$Take.mp4"
 $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $peerArgs -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
'''+key)
s=s.replace('while($w.ElapsedMilliseconds-lt14000)', 'while($w.ElapsedMilliseconds-lt23000)')
key='   $networkSnapshot=$true'
assert key in s
s=s.replace(key,'''   $linkBefore=Q client '--domino-links';$linkBefore|ConvertTo-Json -Depth 10|Set-Content "$out/links-before-stream.json"
   if(!$linkBefore.valid-or@($linkBefore.connections|Where-Object hasBeam).Count-ne1){throw 'One strict native beam required before streaming'}
   $ci=[Globalization.CultureInfo]::InvariantCulture;$observerX=$sourceX+3.5;$observerY=$sourceY-1
   $observerYaw=[Math]::Atan2([double]$target2.position[1]-$observerY,[double]$target2.position[0]-$observerX)*180/[Math]::PI
   C host ('setviewpos {0} {1} {2} {3} 0'-f$observerX.ToString('F4',$ci),$observerY.ToString('F4',$ci),$sourceZ.ToString('F4',$ci),$observerYaw.ToString('F4',$ci))
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch -8 -Evidence "$out/peer-camera.json" -Quick
   Copy-Item "$PSScriptRoot/domino-banks-v192/emily-domino-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
   $worldBefore=S host '.domino-world.json';$beamBefore=S host '.domino-links.json'
   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
   $readyWatch=[Diagnostics.Stopwatch]::StartNew()
   do{$movement=S host '.movement.json';$stream=S host '.shadow-stream.json';if($movement.active-and$movement.version-eq47-and$movement.weight-gt.9-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 75}while($readyWatch.ElapsedMilliseconds-lt5000)
   if(!$movement.active-or$movement.version-ne47-or$movement.weight-lt.9-or!$stream.ready-or!$stream.armed){throw 'Native beam transport did not arm'}
   $networkStarted=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native links established before stream; exact body bank and native Steam presentation enabled after source settled'})
'''+key)
key='  Start-Sleep -Milliseconds 25'
assert s.count(key)==1
s=s.replace(key,'''  if($networkStarted){$peerRows.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;movement=(S host '.movement.json');stream=(S host '.shadow-stream.json');marks=(S host '.domino-world.json');links=(S host '.domino-links.json')})}
'''+key)
key=" C client 'coop_domino_fixture off';$w.Restart()"
assert s.count(key)==1
s=s.replace(key,''' if(!$networkStarted-or$peerRows.Count-lt10){throw 'Insufficient paired beam samples'}
 $peerWorld=S host '.domino-world.json';$peerLinks=S host '.domino-links.json';$sourceLinks=Q client '--domino-links';$peerFx=Q host '--domino-effects'
 [ordered]@{source=$sourceLinks;peer=$peerLinks;world=$peerWorld;effects=$peerFx}|ConvertTo-Json -Depth 12|Set-Content "$out/network-link-proof.json"
 if($peerLinks.beams.Count-ne1-or$peerLinks.started-ne($beamBefore.started+1)-or$peerLinks.refused-ne$beamBefore.refused-or$peerLinks.updated-lt($beamBefore.updated+20)-or$peerWorld.marks.Count-ne2-or$peerWorld.started-ne($worldBefore.started+2)-or!$peerWorld.leased){throw 'Exactly one stable beam and two owned marks not reproduced'}
 $native=@($sourceLinks.connections|Where-Object hasBeam);$remoteBeam=$peerLinks.beams[0]
 if($native.Count-ne1-or$native[0].id-ne$remoteBeam.sourceId-or$native[0].from-ne$remoteBeam.from-or$native[0].to-ne$remoteBeam.to-or$remoteBeam.owner-ne(S host).probeActorHandle){throw 'Beam incarnation/endpoint keys differ across native Steam'}
 $beamFx=@($peerFx.effects|Where-Object {$_.id-eq$remoteBeam.id-and$_.owner-eq$remoteBeam.owner-and$_.name-eq'models/effects/gameplay/powers/phantasmal_link/links/phlink_links_01.fx'})
 if($beamFx.Count-ne1-or$peerFx.effects.Count-ne3){throw 'Unexpected duplicate or missing receiver effects'}
 $raw=Q host '--object' $beamFx[0].record;$raw|ConvertTo-Json -Depth 8|Set-Content "$out/receiver-beam-record.json"
 $endTransform=@();foreach($offset in 0x80,0x88,0x90,0x98,0xa0,0xa8){$word=($raw.words|Where-Object offset -eq ('0x{0:x}'-f$offset)).word;$bytes=[BitConverter]::GetBytes([Convert]::ToUInt64($word.Substring(2),16));$endTransform+=[BitConverter]::ToSingle($bytes,0);$endTransform+=[BitConverter]::ToSingle($bytes,4)}
 $sourceCompare=Q client '--domino-links';$sourceBeam=@($sourceCompare.connections|Where-Object hasBeam)[0]
 $startDistance=0;$endDistance=0;for($k=0;$k-lt3;$k++){$startDistance+=[Math]::Pow([double]$beamFx[0].origin[$k]-[double]$sourceBeam.start[$k],2);$endDistance+=[Math]::Pow([double]$endTransform[$k]-[double]$sourceBeam.end[$k],2)}
 [ordered]@{source=$sourceBeam;receiverEnd=$endTransform;startSquaredDistance=$startDistance;endSquaredDistance=$endDistance}|ConvertTo-Json -Depth 8|Set-Content "$out/beam-endpoint-comparison.json"
 if($startDistance-gt.25-or$endDistance-gt.25){throw 'Beam endpoints do not follow observed native source'}
 $checks.Add('Native Steam reproduces two owned marks and exactly one native cosmetic beam, preserving its source FX incarnation, opaque target keys and independently read start/end positions; no receiver NPC or Domino gameplay call')
 $stop=[Diagnostics.Stopwatch]::StartNew();C host 'coop_movement_bank off';$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Receiver presentation stop; native mark and beam fade begins'})
 Start-Sleep -Milliseconds 700;$afterStop=S host '.domino-links.json';$afterMarks=S host '.domino-world.json'
 if($afterStop.beams.Count-or$afterStop.stopped-ne($beamBefore.stopped+1)-or$afterMarks.marks.Count){throw 'Receiver beam/mark ownership did not retire'}
 $fade=[Collections.Generic.List[object]]::new()
 do{$fx=Q host '--domino-effects';$status=S host '.domino-links.json';$fade.Add([pscustomobject]@{ms=$stop.ElapsedMilliseconds;effects=$fx;links=$status});if($status.beams.Count-or$status.started-ne$peerLinks.started-or$status.updated-ne$afterStop.updated){throw 'Retired beam resumed'};if(!$fx.effects.Count){break};Start-Sleep -Milliseconds 150}while($stop.ElapsedMilliseconds-lt5000)
 $fade|ConvertTo-Json -Depth 10|Set-Content "$out/beam-fade.json"
 if($fx.effects.Count){throw 'Native beam/marks survive bounded fade'}
 $checks.Add('Receiver beam and mark slots retire within700ms with no resumed update; native effects finish withinfive seconds')
'''+key)
s=s.replace("try{I $r;C $r 'coop_domino_fixture off'", "try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_domino_fixture off'")
key=' if($capture){$capture.WaitForExit(18000)'
s=s.replace(key,''' if($peerCapture){$peerCapture.WaitForExit(18000)|Out-Null;if(!$peerCapture.HasExited-or$peerCapture.ExitCode){$failure=($failure+'; peer capture failed').TrimStart('; ')}}
'''+key)
s=s.replace(' $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"',' $peerRows|ConvertTo-Json -Depth 14|Set-Content "$out/peer-states.json"\n $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"')
s=s.replace("$fx=Q client '--domino-effects';if(@($fx.effects).Count){throw 'Domino effects remain after checkpoint'}", "foreach($r in 'host','client'){$fx=Q $r '--domino-effects';if(@($fx.effects).Count){throw 'Domino effects remain after checkpoint'}}")
s=s.replace('No remote body/VFX or gameplay replication.', 'After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. NPC alignment, AI, damage/outcomes and complete-power acceptance remain pending.')
(P/'Test-DominoLinks-v223.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig')
s=s.replace("'DominoLinksObserve'","'DominoLinksNetwork','DominoLinksObserve'",2)
key="  'DominoLinksObserve'{";assert key in s
s=s.replace(key,"  'DominoLinksNetwork'{& \"$root/work/natural-moveset/Test-DominoLinks-v223.ps1\" -Take $Take -Pitch $Pitch -AimGuard}\n"+key)
runner.write_text(s,encoding='utf-8')
