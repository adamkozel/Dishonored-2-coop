from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-rat-visual-v291'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
for kind in ('build','smoke'):
 s=(W/f'swarm-rat-visual-{kind}-v291.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
assert read(R/'work/natural-moveset/swarm-world-network-v290/result.json')['passed']
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in (S/'assets').glob('swarm_rat*'):shutil.copy2(p,A/'assets'/p.name)
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in [W/'Invoke-NativeLabOperation.ps1',W/'prepare_swarm_rat_visual_v291.py',W/'prepare_swarm_rat_test_v291.py',S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v291.ps1',R/'work/natural-moveset/build_swarm_rat_visual_v291.py',R/'work/natural-moveset/swarm-rat-visual-assets-v291.json',*W.glob('swarm-rat-visual-*-v291.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-world-v290/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Bounded one-rat native cosmetic visual proof. Generic idEntity/native rat MD6 with exactly AnimEvolved/MidnightProxy/Audio/ReferenceFrame, animation-only native walk scene. No AI/attack/SwarmAgent/possession components. Follows one opaque received rat identity, own model identity guards, finite peer lease/scene stop/hide, world invalidation. At most one model cached per epoch. Full pool, source phase/speed/scale, smooth interpolation, VFX and shared damage remain pending. Wire and bank unchanged. Build/load checks pass; isolated asset install/live proof pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k] for k in ('dll','probe','swarmWorldProtocol')}))
