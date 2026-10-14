from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-lifecycle-v328'
assert not A.exists()
for kind in ('build','smoke'):
 s=(W/f'possession-human-{kind}-v328.log').read_text(encoding='utf-8-sig');assert s and not any(t in s for t in ('FAIL','error C','error LNK'))
A.mkdir();shutil.copytree(S/'src',A/'src')
for p in (Path(__file__),W/'Invoke-NativeLabOperation.ps1',S/'Send-D2Command.ps1',R/'work/natural-moveset/Observe-NativePossessionTarget-v328.ps1',W/'possession-human-build-v328.log',W/'possession-human-smoke-v328.log'):
 shutil.copy2(p,A/p.name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
x=json.loads((W/'candidate-possession-state-v326/identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Candidate328: finite human-Possession fixture equips only until first native-ready observation. Native host entry/exit owns subsequent slot changes. Defers inventory rollback after isolation retirement until verified inactive state14 and restored default mode, with a finite lease+5s ceiling. Requires acquisition retained throughout host occupation; runtime pending. Read-only target probe, protocol48 and bank51/315 unchanged. No peer transfer, body hiding or VFX claim.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe')}))

