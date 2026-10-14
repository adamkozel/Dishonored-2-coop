from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
p=N/'Test-DoppelgangerVisual-v430.ps1';assert not p.exists()
s=(N/'Test-DoppelgangerVisual-v428.ps1').read_text(encoding='utf-8-sig')
s=s.replace("C host 'setviewpos -44.39 -153.27 68.1 203 0'","C host 'setviewpos -47.39 -156.87 68.1 90 0'")
s=s.replace(" $dirty=$true", " $visualStart=S host '.doppelganger-visual.json';$visualStart|ConvertTo-Json -Depth 8|Set-Content \"$out/visual-start.json\"\n $dirty=$true")
s=s.replace('$lastVisual.created-ne1','$lastVisual.created-$visualStart.created-ne1').replace('$lastVisual.shown-ne1','$lastVisual.shown-$visualStart.shown-ne1').replace('$lastVisual.hidden-ne1','$lastVisual.hidden-$visualStart.hidden-ne1')
s=s.replace('$lastVisual.applied-lt20','$lastVisual.applied-$visualStart.applied-lt20').replace('$lastVisual.interpolated-lt20','$lastVisual.interpolated-$visualStart.interpolated-lt20').replace('-or$lastVisual.poseRefused-or','-or($lastVisual.poseRefused-$visualStart.poseRefused)-or')
p.write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerVisual-v428.ps1').read_text(encoding='utf-8-sig').replace('v428','v430')
(W/'Run-DoppelgangerVisual-v430.ps1').write_text(s,encoding='utf-8')
print('430: observer behind source, per-take visual counter deltas; no proxy/asset changes')
