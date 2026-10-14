from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-heart-network-v564';P=W/'candidate-heart-observation-v562'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==rd(P/'identity.json')['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke'):
    text=(W/f'shadowwalk-body-{tag}-v564.log').read_text(encoding='utf-8-sig');assert text and not any(x in text for x in ('error C','error LNK','FAIL ', 'Traceback'))
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src');shutil.copytree(R/'work/natural-moveset/heart-banks-v564',A/'banks');shutil.copy2(L/'assets/heart_prop.def',A/'heart_prop.def')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in (W/'implement_heart_v564.py',W/'Build-Heart-v564.ps1',W/'implement_heart_prop_v564.py',W/'test_heart_source_v564.py',Path(__file__),L/'Send-D2Command.ps1'):shutil.copy2(p,A/p.name)
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=60,bankVersion=60,clips=340,runtimeVerified=False,scope='Native Heart full-body presentation and render-only prop through Steam; exact clocks/ownership. Isolated20s acquisition fixture. Runtime integration pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
