"""Widen the peer camera so the full jumping body stays in the recording."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeAirNetwork-v165.ps1').read_text(encoding='utf-8-sig')
a="-f ($sx+2.8),($sy+1.2),$sz)";assert s.count(a)==1
s=s.replace(a,"-f ($sx+4.2),($sy+1.8),$sz)")
a='-Role host -Pitch 15 -Evidence "$out/observer-camera.json"';assert s.count(a)==1
s=s.replace(a,'-Role host -Pitch -3 -Evidence "$out/observer-camera.json"')
(P/'Test-MesmerizeAirNetwork-v169.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';r=runner.read_text(encoding='utf-8-sig')
assert r.count('Test-MesmerizeAirNetwork-v165.ps1')==1
runner.write_text(r.replace('Test-MesmerizeAirNetwork-v165.ps1','Test-MesmerizeAirNetwork-v169.ps1'),encoding='utf-8')
