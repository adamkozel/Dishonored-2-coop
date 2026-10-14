from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-trails-v318'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
for kind in ('build','smoke','action'):
 s=(W/f'swarm-trails-{kind}-v318.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:' not in s and 'error C' not in s and 'error LNK' not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in (S/'assets').glob('swarm_*'):
 if p.is_file():shutil.copy2(p,A/'assets'/p.name)
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in [Path(__file__),W/'prepare_swarm_trails_v318.py',W/'Invoke-NativeLabOperation.ps1',S/'Build-D2SwarmRatVisualAssets.ps1',S/'Build-D2SwarmSpawnFxAssets.ps1',S/'Send-D2Command.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v318.ps1',*W.glob('swarm-trails-*-v318.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-fx-observe-v312/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),assetHashes={p.name:sha(p)for p in(A/'assets').iterdir()},runtimeVerified=False,scope='Candidate318: version2 rat world packet carries optional strictly attributed native rat spawn-trail IDs, ages and transforms. Bounded source discovery7/16scans/750ms, read-only native frame/pool ownership checks, cached record samples expire at2500ms. Receiver starts each source ID once within200ms, interpolates matching IDs only, updates guarded owned particle record with no callback, preserves tombstones through lease gaps, and retires on source absence/scope changes. Native particle declaration reused unchanged; no AI/damage or first-person modification. Runtime verification pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe')}))

