from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-rat-probe-v292'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists();assert read(R/'work/natural-moveset/swarm-rat-visual-network-v291/result.json')['passed']
for kind in ('build','identity'):
 s=(W/f'swarm-rat-probe-{kind}-v292.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');shutil.copy2(R/'work/asset-game/dinput8.dll',A/'dinput8.dll');shutil.copy2(R/'work/build/Release/d2_runtime_probe.exe',A/'d2_runtime_probe.exe')
for p in [W/'Invoke-NativeLabOperation.ps1',W/'prepare_rat_visibility_probe_v292.py',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v292.ps1',*W.glob('swarm-rat-probe-*-v292.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-visual-v291/identity.json');assert x['dll']==sha(A/'dinput8.dll')
x.update(created=datetime.datetime.now().astimezone().isoformat(),probe=sha(A/'d2_runtime_probe.exe'),scope='Probe-only exact cosmetic rat skeleton scope plus closer/floor-facing diagnostic capture. Same native visual DLL291. Prior native lifecycle checks pass16 but peer visibility is unresolved in visual review; do not treat scene start as visible success. Probe verifies dedicated definition, four owned component types, exact rat MD6/skeleton and bounded matrices; no native mutations added. Full pool and gameplay remain pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8');shutil.copy2(A/'d2_runtime_probe.exe',S/'bin/d2_runtime_probe.exe');print(json.dumps({k:x[k]for k in ('dll','probe')}))
