from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];L=R/'outputs/d2-coop-lab';W=R/'work/movement-integration';P=R/'work/natural-moveset';A=W/'candidate-swarm-rat-mainclip-v294'
assert not A.exists();A.mkdir();(A/'assets').mkdir()
for phase in ('idle','walk','run'):
 p=L/f'assets/swarm_rat_{phase}.midnightscene';s=p.read_text();assert s.count('m_isMainClip = false;')==1;s=s.replace('m_isMainClip = false;','m_isMainClip = true;');p.write_text(s);shutil.copy2(p,A/'assets'/p.name)
shutil.copy2(L/'assets/swarm_rat_visual.def',A/'assets/swarm_rat_visual.def')
p=L/'Build-D2SwarmRatVisualAssets.ps1';s=p.read_text(encoding='utf-8-sig').replace('swarm-rat-visual-v291','swarm-rat-mainclip-v294');p.write_text(s)
shutil.copy2(p,A/p.name)
p=P/'Test-SwarmRatVisualNetwork-v293.ps1';s=p.read_text(encoding='utf-8-sig').replace('setviewpos -44.39 -153.27 68.1 180 0','setviewpos -46.3 -151.8 68.1 220 0').replace('-Role client -Pitch 26','-Role client -Pitch 45')
(P/'Test-SwarmRatVisualNetwork-v294.ps1').write_text(s);shutil.copy2(P/'Test-SwarmRatVisualNetwork-v294.ps1',A/'Test-SwarmRatVisualNetwork-v294.ps1')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace('Test-SwarmRatVisualNetwork-v293.ps1','Test-SwarmRatVisualNetwork-v294.ps1');p.write_text(s);shutil.copy2(p,A/p.name)
x=json.loads((W/'candidate-swarm-rat-probe-v292/identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),runtimeVerified=False,scope='Asset-only bounded experiment: rat animation track set main clip, matching native humanoid whole-body track convention. Candidate291 scene-clock activity did not prove changing joint output; v293 diagnostics found unchanged31-joint cache and unhidden model. Native rat compatibility remains hypothesis. Gameplay DLL291 and read-only probe292 unchanged. Closer floor-facing camera, per-run counters and early skeleton diagnostics. No additional models or gameplay functions.')
x['assetHashes']={p.name:hashlib.sha256(p.read_bytes()).hexdigest().upper()for p in(A/'assets').iterdir()}
(A/'identity.json').write_text(json.dumps(x,indent=2))
print(json.dumps(x['assetHashes']))
