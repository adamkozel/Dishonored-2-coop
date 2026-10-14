from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionTargetFx-v351.ps1').read_text()
a='$observerYaw=[Math]::Atan2($sourceY-$observerY,$sourceX-$observerX)*180/[Math]::PI'
assert s.count(a)==1
s=s.replace(a,'$observerYaw=[Math]::Atan2([double]$target.position[1]-$observerY,[double]$target.position[0]-$observerX)*180/[Math]::PI')
s=s.replace('-Role client -Pitch 12 -Evidence', '-Role client -Pitch 0 -Evidence')
a=" $visibilityBefore=S client '.possession-body.json';"
assert a in s
s=s.replace(a," $sourceFxBefore=S host '.possession-fx.json';$peerFxBefore=S client '.possession-fx.json'\n [ordered]@{source=$sourceFxBefore;peer=$peerFxBefore}|ConvertTo-Json -Depth 6|Set-Content \"$out/network-fx-before.json\"\n"+a)
s=s.replace('$sourceFx.sent-lt3-or$peerFx.received-lt3-or$peerFx.started-ne1-or$peerFx.stopped-ne1-or$peerFx.refused-or$peerFx.retired-or$peerFx.sourceId', '$sourceFx.sent-$sourceFxBefore.sent-lt3-or$peerFx.received-$peerFxBefore.received-lt3-or$peerFx.started-$peerFxBefore.started-ne1-or$peerFx.stopped-$peerFxBefore.stopped-ne1-or$peerFx.refused-ne$peerFxBefore.refused-or$peerFx.retired-ne$peerFxBefore.retired-or$peerFx.sourceId')
s=s.replace('remote target control, VFX and shared gameplay remain unimplemented.', 'remote target control and shared gameplay remain unimplemented; scoped target particles use a native-derived asset through Steam.')
s=s.replace('Read-only source Possession FX records captured with target-context stability and candidate controller slots; no remote VFX invocation','Read-only source Possession FX records captured with target-context stability and candidate controller slots; remote effect verified separately')
(P/'Test-PossessionTargetFxView-v352.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Test-PossessionTargetFx-v351.ps1')==1;s=s.replace('Test-PossessionTargetFx-v351.ps1','Test-PossessionTargetFxView-v352.ps1');p.write_text(s)
print('352 target-centered view and per-take counter deltas; production DLL351 unchanged')
