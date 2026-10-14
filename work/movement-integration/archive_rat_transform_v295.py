from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-rat-transform-v295'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
for kind in ('build-v295-final','identity-v295','protocol-v295','smoke-v295'):
 s=(W/f'swarm-rat-transform-{kind}.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in (S/'assets').glob('swarm_rat*'):shutil.copy2(p,A/'assets'/p.name)
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in [W/'Invoke-NativeLabOperation.ps1',W/'prepare_rat_transform_v295.py',S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v295.ps1',*W.glob('swarm-rat-transform-*-v295*.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-mainclip-v294/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Fix cosmetic live transform dispatch: use exact native87d200 after owned entity/referenceframe/API checks; verify actor+300 readback against target within1mm. Prior spawn setters did not move live model. Same294assets/one-model/Midnight animation experiment. Native animation output remains unresolved. Full pool, animation speed, smooth interpolation, scale, effects and gameplay pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe')}))
