from pathlib import Path
import hashlib,json,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-doppelganger-fixture-v405';assert not A.exists()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=W/'candidate-partner-interaction-v396'
assert sha(previous/'dinput8.dll')=='B8120436A3242BDD30342F11A20638B32872255641EE6E969B163B1B7EE02289'
assert json.loads((previous/'identity.json').read_text(encoding='utf-8-sig'))['runtimeVerified']
logs=[W/f'doppelganger-fixture-{k}-v405.log' for k in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
 t=p.read_text(encoding='utf-8-sig');assert t and not any(s in t for s in ('error C','error LNK','FAIL:','FAIL ','Traceback')),p
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(previous/'banks',A/'banks')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in [Path(__file__),*logs,W/'Build-DoppelgangerFixture-v405.ps1',W/'implement_doppelganger_fixture_v405.py']:shutil.copy2(p,A/p.name)
x=json.loads((previous/'identity.json').read_text(encoding='utf-8-sig'))
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,banks=[dict(path=str(p),sha256=sha(p)) for p in sorted((A/'banks').glob('*.bin'))],scope='Candidate405 adds opt-in isolated Emily Doppelganger native inventory fixture and read-only exact item/effect probes. Native source cast/lifecycle observation pending. Existing396 partner protections, native collision and bank54/319 unchanged; no Doppelganger remote body, clone, gameplay or VFX replication yet.',previousVerified=str(previous))
x.pop('runtimeVerificationScope',None)
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for name in ('dinput8.dll','d2_runtime_probe.exe'):
 shutil.copy2(A/name,L/'bin'/name);assert sha(A/name)==sha(L/'bin'/name)
print(json.dumps(dict(dll=x['dll'],probe=x['probe'],archive=str(A))))
