from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-fx-research-v348';old=W/'candidate-possession-exit-v343'
assert not A.exists();log=(W/'possession-fx-probe-build-v348.log').read_text(encoding='utf-8-sig');assert log and 'error C' not in log and 'error LNK' not in log
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
A.mkdir();shutil.copytree(S/'src',A/'src');shutil.copy2(old/'dinput8.dll',A/'dinput8.dll');shutil.copy2(R/'work/build/Release/d2_runtime_probe.exe',A/'d2_runtime_probe.exe')
for p in (Path(__file__),W/'Build-PossessionFxProbe-v348.ps1',W/'prepare_possession_fx_probe_v348.py',W/'possession-fx-probe-build-v348.log',P/'Observe-PossessionFx-v348.ps1'):shutil.copy2(p,A/p.name)
x=json.loads((old/'identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),banksReference='../candidate-possession-exit-v343/banks',runtimeVerified=False,scope='Production DLL343 unchanged and previously verified for native exit continuity and visibility interruption. Probe348 adds bounded external read-only Possession FX inventory plus candidate controller slot observations. No FX playback or runtime hook changes. Native research run pending.')
assert x['dll']=='7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C'
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8');shutil.copy2(A/'d2_runtime_probe.exe',S/'bin/d2_runtime_probe.exe')
print(json.dumps({k:x[k]for k in ('dll','probe','protocol','bankVersion')}))
