from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-release-body-v334'
assert not A.exists()
log=(W/'possession-release-build-v334.log').read_text(encoding='utf-8-sig');assert log and not any(x in log for x in ('error C','error LNK','FAIL'))
A.mkdir();shutil.copytree(S/'src',A/'src');shutil.copytree(P/'possession-banks-v334',A/'banks')
for p in (Path(__file__),W/'prepare_possession_release_body_v334.py',W/'prepare_possession_network_v334.py',W/'possession-release-build-v334.log',W/'possession-release-bank-v334.log',W/'Invoke-NativeLabOperation.ps1',P/'build_possession_bank_v334.py',P/'Test-PossessionReleaseNetwork-v334.ps1',P/'possession-bank-manifest-v334.json',P/'possession-bank-source-manifest-v334.json'):
 shutil.copy2(p,A/p.name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
x=json.loads((W/'candidate-possession-lifecycle-v328/identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,protocol=49,bankVersion=52,clips=316,scope='Native observed Corvo successful release400ms with dormant acquired power, holstered sword and exact generation-owned human target drives a cosmetic full-body clip. All315 previous clip payloads retained. Remote occupied-host control/visibility/VFX remain unimplemented; existing stream expires when native action is unsupported. Source-only330/332 release reader evidence is valid within otherwise failed health/inventory experiments; complete333 has no release. Runtime and visual checks of334 pending.')
x.pop('dllSourceReference',None)
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
(A/'tests.json').write_text(json.dumps(dict(passed=True,commands=['cmake build Release d2_lab_proxy d2_proxy_smoke d2_runtime_probe d2_shadow_action_tests d2_movement_bank_tests','d2_shadow_action_tests.exe','d2_movement_bank_tests.exe corvo-possession-bank.bin emily-possession-bank.bin','d2_proxy_smoke.exe dinput8.dll'],scope='Compilation, semantic protocol49, actual two bank52 payloads, proxy/Steam pose/skip protocol smoke; native deployment pending'),indent=2),encoding='utf-8')
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe','protocol','bankVersion')}))
