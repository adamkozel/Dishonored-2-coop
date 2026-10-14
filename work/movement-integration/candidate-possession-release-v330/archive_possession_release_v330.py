from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-release-v330'
assert not A.exists();s=(W/'possession-release-build-v330.log').read_text(encoding='utf-8-sig');assert s and not any(t in s for t in ('FAIL','error C','error LNK'))
A.mkdir();shutil.copytree(S/'src',A/'src')
for p in (Path(__file__),W/'prepare_possession_release_reader_v330.py',W/'Invoke-NativeLabOperation.ps1',S/'Send-D2Command.ps1',R/'work/natural-moveset/Observe-NativePossessionTarget-v330.ps1',W/'possession-release-build-v330.log'):
 shutil.copy2(p,A/p.name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
x=json.loads((W/'candidate-possession-lifecycle-v328/identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,dllSourceReference='../candidate-possession-lifecycle-v328/src',scope='Candidate330 probe-only strict successful release400ms under EnterPossessedHost26/internal0 plus exact generation-validated NPC target and Corvo arm. Default runtime reader release disabled. DLL unchanged from328; archive src reflects updated probe/shared headers, with exact compiled DLL source retained at dllSourceReference. Protocol48 bank51/315 unchanged. Observed entry context accepts only dormant, unselected acquired equipment; stage diagnostics included. Native validation pending; no peer transfer/hide/VFX claim.')
(A/'identity.json').write_text(json.dumps(x,indent=2));shutil.copy2(A/'d2_runtime_probe.exe',S/'bin/d2_runtime_probe.exe')
print(json.dumps({k:x[k]for k in ('dll','probe')}))

