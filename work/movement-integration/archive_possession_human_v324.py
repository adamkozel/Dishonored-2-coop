from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-human-v324'
assert not A.exists()
for name in ('build','smoke'):
 s=(W/f'possession-human-{name}-v324.log').read_text(encoding='utf-8-sig');assert s and not any(t in s for t in ('FAIL','error C','error LNK'))
A.mkdir();shutil.copytree(S/'src',A/'src')
for p in (Path(__file__),W/'prepare_possession_human_v324.py',W/'prepare_possession_target_observe_v324.py',W/'Invoke-NativeLabOperation.ps1',S/'Send-D2Command.ps1',R/'work/natural-moveset/Observe-NativePossessionTarget-v324.ps1',W/'possession-human-build-v324.log',W/'possession-human-smoke-v324.log'):
 shutil.copy2(p,A/p.name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
x=json.loads((W/'candidate-possession-body-v323/identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Candidate324 finite native human40000 acquisition fixture: exact Corvo possession_parasite TYPE_POWER_UPGRADE20 plus Possession item. Normal controller input only. Native quantity rollback and mandatory isolated checkpoints; target transfer observations pending. Runtime movement protocol48/bank51/315 unchanged from323.',assetsReference='../candidate-possession-body-v323/assets',banksReference='../candidate-possession-body-v323/banks')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe')}))
