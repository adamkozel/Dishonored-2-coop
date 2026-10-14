from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-heavy-v635';P=W/'candidate-heavy-v633'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==rd(P/'identity.json')['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke'):
 text=(W/f'shadowwalk-body-{tag}-v635.log').read_text(encoding='utf-8-sig');assert text and not any(x in text for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in (W/'prepare_heavy_fix_v635.py',W/'Build-Heavy-v635.ps1',W/'Deploy-Heavy-v635.ps1',R/'work/natural-moveset/Test-CrouchedHeavy-v636.ps1',Path(__file__)):shutil.copy2(p,A/p.name)
banks=[dict(path=str(p.relative_to(R)),sha256=sha(p)) for p in sorted((R/'work/natural-moveset/heavy-banks-v633').glob('*.bin'))];assert len(banks)==2
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=69,bankVersion=67,clips=357,banks=banks,runtimeVerified=False,scope='Exact native629/630 crouched heavy release18 with exact subscene/duration pairs; Corvo634 disproves role-specific selection. Existing complete-body heavy50/51/52 adapted through original crouch pipeline into354/355/356. Prior354 unchanged, protocol69/bank67. Native roots/events/outcomes untouched; ordinary input verification pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
