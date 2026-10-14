from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(N/'Test-ShadowWalkSneak-v551.ps1').read_text(encoding='utf-8-sig')
old="C host 'setviewpos -46.3 -157.4 68.1 135 0'"
assert s.count(old)==1
s=s.replace(old,"C host 'setviewpos -46 -154.5 68.1 194 0'")
p=N/'Test-ShadowWalkClearView-v554.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
s=(W/'Run-ShadowWalkSneak-v551.ps1').read_text(encoding='utf-8-sig').replace('native-shadowwalk-sneak-v551','native-shadowwalk-sneak-clear-v554').replace('Test-ShadowWalkSneak-v551','Test-ShadowWalkClearView-v554').replace('ShadowWalkSneak551','ShadowWalkClearView554')
(W/'Run-ShadowWalkClearView-v554.ps1').write_text(s,encoding='utf-8')
print('Prepared clear-side observer view; source, runtime candidate and assertions unchanged')
