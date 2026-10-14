from pathlib import Path
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'; W=R/'work/movement-integration'
read=lambda p:p.read_text(encoding='utf-8-sig')
s=read(N/'Test-DoppelgangerCancel-v502.ps1')
old='$visual.fx.started-ne$visualStart.fx.started'
assert s.count(old)==1
s=s.replace(old,'$visual.fx.started.Count-ne2-or$visualStart.fx.started.Count-ne2-or$visual.fx.started[0]-ne$visualStart.fx.started[0]-or$visual.fx.started[1]-ne$visualStart.fx.started[1]')
p=N/'Test-DoppelgangerCancel-v503.ps1'; assert not p.exists(); p.write_text(s,encoding='utf-8')
p=W/'Run-DoppelgangerCancel-v503.ps1'; assert not p.exists()
p.write_text(read(W/'Run-DoppelgangerCancel-v502.ps1').replace('v502','v503'),encoding='utf-8')
print('Compared the two FX start counters individually; original502 harness retained.')
