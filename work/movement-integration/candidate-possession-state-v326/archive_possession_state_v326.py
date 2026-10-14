from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-state-v326'
assert not A.exists();s=(W/'possession-state-build-v326.log').read_text(encoding='utf-8-sig');assert s and not any(t in s for t in ('FAIL','error C','error LNK'))
A.mkdir();shutil.copytree(S/'src',A/'src')
for p in (Path(__file__),W/'prepare_possession_state_v326.py',W/'Invoke-NativeLabOperation.ps1',S/'Send-D2Command.ps1',R/'work/natural-moveset/Observe-NativePossessionTarget-v326.ps1',W/'possession-state-build-v326.log'):
 shutil.copy2(p,A/p.name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
x=json.loads((W/'candidate-possession-human-v324/identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Candidate326 same finite native fixture DLL as324. Probe separates internal mode state from physics stance and active-state IDs, validates target weak generation, exact NPCPossession RTTI/entity/component membership. Normal LT exit and native quantity/checkpoint restoration required. CLI registered and initial watcher preflight passed. Internal14 is inactive sentinel, not occupied state; runtime transfer pending; no peer transfer/hide/VFX control.')
(A/'identity.json').write_text(json.dumps(x,indent=2));shutil.copy2(A/'d2_runtime_probe.exe',S/'bin/d2_runtime_probe.exe')
print(json.dumps({k:x[k]for k in ('dll','probe')}))

