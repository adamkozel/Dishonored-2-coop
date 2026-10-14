from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeCancel-v231.ps1').read_text(encoding='utf-8-sig')
key=" if($afterStop.beams.Count-or$afterStop.stopped-ne($beamBefore.stopped+1)-or$afterMarks.marks.Count){throw 'Receiver beam/mark ownership did not retire'}"
new=''' $cancelDiagnostics=[Collections.Generic.List[object]]::new()
 do{
  $sourceNow=Q client '--domino-links';$sourcePower=Q client '--player-domino';$sourceState=Q client '--player-states'
  $afterStop=S host '.domino-links.json';$afterMarks=S host '.domino-world.json'
  $cancelDiagnostics.Add([pscustomobject]@{ms=$stop.ElapsedMilliseconds;source=$sourceNow;sourcePower=$sourcePower;sourceState=$sourceState;peer=$afterStop;peerMarks=$afterMarks;sourceWorld=(S client '.domino-world.json');sourceFx=(OptionalFx client);peerFx=(OptionalFx host);movement=(S host '.movement.json');stream=(S host '.shadow-stream.json')})
  if(!$afterStop.beams.Count-and!$afterMarks.marks.Count-and!$sourceNow.targets-and!$sourceNow.connections.Count){break}
  Start-Sleep -Milliseconds 120
 }while($stop.ElapsedMilliseconds-lt3500)
 $cancelDiagnostics|ConvertTo-Json -Depth 16|Set-Content "$out/native-cancel-diagnostics.json"
 if($sourceNow.targets-or$sourceNow.connections.Count){throw 'Native X cancel left established native links active; see paired diagnostics'}
 if($afterStop.beams.Count-or$afterStop.stopped-ne($beamBefore.stopped+1)-or$afterMarks.marks.Count){throw 'Receiver beam/mark ownership did not retire; see paired diagnostics'}'''
assert key in s;s=s.replace(key,new)
# Keep AI references through the end of recording, including failed assertions.
s=s.replace("C $r 'coop_domino_fixture off';C $r 'coop_movement_fixture off'}catch{}", "C $r 'coop_domino_fixture off'}catch{}")
s=s.replace(" if($dirty){try{&", " foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}\n if($dirty){try{&",1)
(P/'Test-DominoNativeCancel-v232.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig').replace('Test-DominoNativeCancel-v231.ps1','Test-DominoNativeCancel-v232.ps1');runner.write_text(s,encoding='utf-8')
