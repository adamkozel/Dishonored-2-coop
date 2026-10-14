from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-release-body-v336'
assert not A.exists()
log=(W/'possession-visibility-build-v336.log').read_text(encoding='utf-8-sig');assert log and not any(x in log for x in ('error C','error LNK','FAIL'))
A.mkdir();shutil.copytree(S/'src',A/'src');shutil.copytree(P/'possession-banks-v334',A/'banks')
for p in (Path(__file__),W/'prepare_possession_visibility_v336.py',W/'prepare_possession_visibility_test_v336.py',W/'possession-visibility-build-v336.log',W/'possession-visibility-tests-v336.log',W/'Invoke-NativeLabOperation.ps1',P/'build_possession_bank_v334.py',P/'Test-PossessionVisibilityNetwork-v336.ps1',P/'possession-bank-manifest-v334.json',P/'possession-bank-source-manifest-v334.json'):
 shutil.copy2(p,A/p.name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
x=json.loads((W/'candidate-possession-lifecycle-v328/identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,protocol=50,bankVersion=52,clips=316,scope='Cosmetic native human Possession transfer/occupied presentation semantic17 over protocol50, finite exactly-owned Corvo clone native visibility lease and cleanup. Release bank52/316 unchanged from334. Source contexts1/8 require exact native classes, physics, generation-owned human target and dormant acquired power. No NPC control/damage/VFX/local-player visibility mutations. Native runtime/visual validation pending.')
x.pop('dllSourceReference',None)
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
(A/'tests.json').write_text(json.dumps(dict(passed=True,commands=['cmake build Release d2_lab_proxy d2_shadow_action_tests','d2_shadow_action_tests.exe','d2_proxy_smoke.exe dinput8.dll'],scope='Compilation, semantic protocol50/context/freshness guards and proxy/Steam pose/skip protocol smoke. Actual bank52 tests passed334; unchanged bank hashes archived. Probe unchanged330. Native deployment pending'),indent=2),encoding='utf-8')
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe','protocol','bankVersion')}))
