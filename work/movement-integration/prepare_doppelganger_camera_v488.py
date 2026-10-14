from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=(N/'Test-DoppelgangerFx-v485.ps1').read_text(encoding='utf-8-sig')
old='setviewpos -47.39 -156.87 68.1 90 0';assert s.count(old)==1
new(N/'Test-DoppelgangerFx-v488.ps1',s.replace(old,'setviewpos -54 -127 62.7 45 0'))
new(W/'Run-DoppelgangerFx-v488.ps1',(W/'Run-DoppelgangerFx-v480.ps1').read_text(encoding='utf-8-sig').replace('480','488'))
print('Prepared488 fixed observer position for body/disappearance view; source and native input unchanged')
