from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-rat-interpolation-v299'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists();assert read(R/'work/natural-moveset/swarm-rat-pool-v298/result.json')['passed']
for kind in ('build','protocol','pose','smoke'):
 s=(W/f'swarm-rat-interpolation-{kind}-v299.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in(S/'assets').glob('swarm_rat*'):shutil.copy2(p,A/'assets'/p.name)
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in[W/'Invoke-NativeLabOperation.ps1',W/'prepare_rat_interpolation_v299.py',S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v299.ps1',*W.glob('swarm-rat-interpolation-*-v299.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-pool-v298/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Per-frame source-time cosmetic rat XYZ/quaternion interpolation between current presented and next accepted world snapshot,80ms delayed. No extrapolation, no blending across entity/incarnation/event/world/token/session/map changes, absence, >250ms gaps or >3m teleports. Existing25-rat native graph/pool/live dispatcher retained, assets/wire unchanged. Pure tests cover diagonal/vertical/yaw plus identity and missing samples; native recording pending. Speed/phase/scale, effects/audio,100upgrades and shared gameplay incomplete.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in('dll','probe')}))
