from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-rat-pool-v298'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists();assert read(R/'work/natural-moveset/swarm-rat-leaf-v297/result.json')['passed']
for kind in ('build','smoke'):
 s=(W/f'swarm-rat-pool-{kind}-v298.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in(S/'assets').glob('swarm_rat*'):shutil.copy2(p,A/'assets'/p.name)
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in[W/'Invoke-NativeLabOperation.ps1',W/'prepare_rat_pool_v298.py',S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v298.ps1',*W.glob('swarm-rat-pool-*-v298.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-leaf-v296/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Bounded cosmetic native rat pool:100 slots keyed by validated source index, handle/incarnation/event/token retirement; one cached entity per slot and at mostone new allocation per frame. Four native cosmetic components only. Verified296 walk graph/295 live dispatch. Test normal25-source agent pool, unique local identities, all actual transform readbacks, lease hide/checkpoint retirement, sampled31-joint walking. Assets unchanged. Full100 capacity, smooth interpolation, source-specific speed/phase/scale, effects/audio and shared gameplay remain pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in('dll','probe')}))
