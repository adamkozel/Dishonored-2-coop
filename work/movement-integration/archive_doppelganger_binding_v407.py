from pathlib import Path
import hashlib,json,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab';A=W/'candidate-doppelganger-binding-v407';assert not A.exists()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=W/'candidate-doppelganger-fixture-v405'
result=json.loads((N/'native-doppelganger-source-v406b/result.json').read_text(encoding='utf-8-sig'));assert result['passed']
assert sha(previous/'dinput8.dll')==result['dll']
logs=[W/f'doppelganger-binding-{k}-v407.log' for k in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
 t=p.read_text(encoding='utf-8-sig');assert t and not any(s in t for s in ('error C','error LNK','FAIL:','FAIL ','Traceback')),p
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(N/'doppelganger-banks-v407',A/'banks')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in [Path(__file__),*logs,W/'Build-DoppelgangerBinding-v407.ps1',W/'implement_doppelganger_binding_v407.py',N/'build_doppelganger_bank_v407.py',N/'doppelganger-bank-manifest-v407.json']:shutil.copy2(p,A/p.name)
x=json.loads((previous/'identity.json').read_text(encoding='utf-8-sig'))
x.update(runtimeVerified=True,runtimeVerificationScope='Native Emily acquisition, controller input, three reciprocal source phases and clone observed406b; full checkpoint cleanup and protected14 passed. Remote binding not yet included.')
(previous/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,banks=[dict(path=str(p),sha256=sha(p)) for p in sorted((A/'banks').glob('*.bin'))],scope='Candidate407 adds exact standing armed Emily Doppelganger phases29/30/31 (466/1200/666ms), Steam semantic protocol52/336, and complete-body clips319-321 in bank55/322. Native first-person, prior bank payload, partner protections and movement collision retained. Source alone invokes clone and effects. Remote body runtime and all remote clone/gameplay/VFX remain pending.',previousVerified=str(previous))
x.pop('runtimeVerificationScope',None)
x.update(protocol=52,protocolBytes=336,bankVersion=55,clips=322)
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for name in ('dinput8.dll','d2_runtime_probe.exe'):
 shutil.copy2(A/name,L/'bin'/name);assert sha(A/name)==sha(L/'bin'/name)
print(json.dumps(dict(dll=x['dll'],probe=x['probe'],archive=str(A))))
