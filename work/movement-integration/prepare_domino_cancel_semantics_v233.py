from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeCancel-v232.ps1').read_text(encoding='utf-8-sig')
s=s.replace('-Pitch 45 -Evidence "$out/cancel-empty-aim.json"','-Pitch -35 -Evidence "$out/cancel-empty-aim.json"')
old="  if(!$afterStop.beams.Count-and!$afterMarks.marks.Count-and!$sourceNow.targets-and!$sourceNow.connections.Count){break}"
assert old in s;s=s.replace(old,"  # Observe the complete bounded native idle interval; established links are retained by native X aim cancellation.")
start=s.index(" if($sourceNow.targets-or$sourceNow.connections.Count){throw")
end=s.index(' $fade=[Collections.Generic.List[object]]::new()',start)
s=s[:start]+''' foreach($sample in $cancelDiagnostics){
  if(!$sample.source.valid-or$sample.source.targets-ne2-or$sample.source.connections.Count-ne2-or@($sample.source.connections|Where-Object hasBeam).Count-ne1-or@($sample.sourceState.active|Where-Object id -eq 72).Count){throw 'Native aim cancellation did not recover with links intact'}
  if($sample.peer.beams.Count-ne1-or$sample.peerMarks.marks.Count-ne2-or$sample.peer.started-ne$peerLinks.started-or$sample.peer.stopped-ne$peerLinks.stopped-or$sample.peer.refused-ne$peerLinks.refused){throw 'Peer changed established links during native aim cancellation'}
  if(!$sample.movement.active-or$sample.movement.weight-lt.95-or$sample.movement.powerWeight-gt.02-or!$sample.stream.ready-or!$sample.stream.armed-or$sample.stream.remoteState-ne0){throw 'Peer body failed native cancellation recovery'}
 }
 if($cancelDiagnostics.Count-lt6){throw 'Insufficient native cancellation recovery interval'}
 $checks.Add('Native X cancels aim/state72 while preserving two established target marks and one beam; native Steam peer preserves their incarnations and returns body to full-weight idle across a bounded interval')
 $stop.Restart();C host 'coop_movement_bank off';Start-Sleep -Milliseconds 700
 $afterStop=S host '.domino-links.json';$afterMarks=S host '.domino-world.json'
 if($afterStop.beams.Count-or$afterStop.stopped-ne($beamBefore.stopped+1)-or$afterMarks.marks.Count){throw 'Independent presentation stop did not retire slots'}
'''+s[end:]
start=s.index(" $sourceAfter=Q client '--domino-links';")
end=s.index(" C client 'coop_domino_fixture off';$w.Restart()",start)
s=s[:start]+''' $sourceAfter=Q client '--domino-links';$sourceAfter|ConvertTo-Json -Depth 10|Set-Content "$out/links-after-peer-presentation-stop.json"
 if(!$sourceAfter.valid-or$sourceAfter.targets-ne2-or$sourceAfter.connections.Count-ne2){throw 'Peer presentation stop unexpectedly changed source native links'}
 $checks.Add('Independent peer presentation stop retires cosmetics within bounded native fade while source native links persist; target deselection is a separate case')
'''+s[end:]
s=s.replace('Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations.', 'Native X aim cancellation is verified to preserve established native links, with the peer matching that state and recovering body to idle. Independent peer presentation stop is separately tested; native target deselection remains pending.')
s=s.replace("input='Native X cancel while LT held after two linked enemies; receiver bank remains active'", "input='Native X aim cancel while LT held after two linked enemies; established links and receiver bank should remain active (observedv232)'" )
(P/'Test-DominoNativeCancel-v233.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig').replace('Test-DominoNativeCancel-v232.ps1','Test-DominoNativeCancel-v233.ps1');runner.write_text(s,encoding='utf-8')
