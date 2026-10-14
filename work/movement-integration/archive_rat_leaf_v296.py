from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-rat-leaf-v296'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists();assert read(R/'work/natural-moveset/swarm-rat-transform-v295/result.json')['passed']
for kind in ('build','smoke'):
 s=(W/f'swarm-rat-leaf-{kind}-v296.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in(S/'assets').glob('swarm_rat*'):shutil.copy2(p,A/'assets'/p.name)
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in[W/'Invoke-NativeLabOperation.ps1',W/'prepare_rat_leaf_v296.py',S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v296.ps1',*W.glob('swarm-rat-leaf-*-v296.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-transform-v295/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='One-model native walk animleaf feasibility. Dedicated cosmetic AnimEvolved declaration/graph, native rat MD6/rat_walk alias; no Midnight scene invocation. Guarded native live transform dispatcher retained from verified295. Probe unchanged. Significant31-joint local motion threshold added, runtime/visual verification pending. No SwarmAgent/AI/gameplay components, original assets untouched.')
x['assetHashes']={p.name:sha(p)for p in(A/'assets').iterdir()};(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in('dll','probe')}))
