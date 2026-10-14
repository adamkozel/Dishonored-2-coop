from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-SwarmNetwork-v269.ps1').read_text()
a=s.index(' $phaseCoverage=@(');b=s.index(' $continuity=@(',a)
s=s[:a]+''' $sourceStream=S host '.shadow-stream.json';$peerStream=S client '.shadow-stream.json'
 [ordered]@{source=$sourceStream;peer=$peerStream}|ConvertTo-Json -Depth 12|Set-Content "$out/power-event-history.json"
 $sent=@($sourceStream.powerSentHistory|Where-Object {$_.variant-in23,24,25})
 $received=@($peerStream.powerReceivedHistory|Where-Object {$_.variant-in23,24,25})
 if(!$sent.Count-or$sent.Count-ne$received.Count){throw 'Swarm sender/receiver retained event counts differ'}
 if(Compare-Object @($sent|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"}) @($received|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"})){throw 'Swarm accepted receiver events differ from sent events'}
 $phaseCoverage=@(foreach($v in 23,24,25){
  $nativeIds=@($scenes|Where-Object variant -eq $v|ForEach-Object handle|Sort-Object -Unique)
  $sourceEvents=@($sent|Where-Object variant -eq $v)
  $nativeSent=@($sourceEvents.nativeScene|Sort-Object -Unique)
  if(!$nativeIds.Count-or(Compare-Object $nativeIds $nativeSent)){throw "Phase $v strict-native scene IDs differ from actual sent scene IDs"}
  [pscustomobject]@{variant=$v;nativeHandles=$nativeIds;sourceEvents=$sourceEvents;acceptedEvents=@($received|Where-Object variant -eq $v);sampledPoseEvents=@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Sort-Object -Unique)}
 })
 $phaseCoverage|ConvertTo-Json -Depth 8|Set-Content "$out/phase-incarnations.json"
 $checks.Add('Bounded send/accept histories exactly match strict native scene incarnations, including sub-status-interval final aim loops')
''' +s[b:]
(N/'Test-SwarmNetwork-v270.ps1').write_text(s)
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';p.write_text(p.read_text().replace('Test-SwarmNetwork-v269.ps1','Test-SwarmNetwork-v270.ps1'))
print('SWARM_EVENT_HISTORY_HARNESS_READY')
