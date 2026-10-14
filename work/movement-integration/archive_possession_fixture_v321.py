from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-fixture-v321'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
for kind in ('build','smoke','action'):
 s=(W/f'possession-fixture-{kind}-v321.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:' not in s and 'error C' not in s and 'error LNK' not in s
A.mkdir();shutil.copytree(S/'src',A/'src');shutil.copytree(W/'candidate-swarm-trails-v320/assets',A/'assets')
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in [Path(__file__),W/'prepare_possession_fixture_v321.py',W/'Invoke-NativeLabOperation.ps1',R/'work/natural-moveset/Observe-NativePossession-v321.ps1',*W.glob('possession-fixture-*-v321.log'),*W.glob('possession-*-v320.*')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-trails-v320/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Candidate321: strict static-identity Possession inventory reader and finite native acquisition fixture for isolated Corvo Dunwall. Existing native inventory APIs only; normal controller input performs empty-target casting. Read-only raw Midnight/state/component observation and mandatory checkpoint cleanup. No remote Possession binding, direct transfer API, target possession or gameplay outcome implementation. Reader320 and Swarm trail320 retained. Runtime pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe')}))
