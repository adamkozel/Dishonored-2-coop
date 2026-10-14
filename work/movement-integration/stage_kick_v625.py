from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-kick-v625';P=W/'candidate-crouch-stance-v620'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==rd(P/'identity.json')['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke'):
 text=(W/f'shadowwalk-body-{tag}-v625.log').read_text(encoding='utf-8-sig');assert text and not any(x in text for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in (W/'prepare_kick_v625.py',W/'Build-Kick-v625.ps1',W/'Deploy-Kick-v625.ps1',R/'work/natural-moveset/Test-NativeKick-v626.ps1',Path(__file__)):shutil.copy2(p,A/p.name)
banks=[dict(path=str(p.relative_to(R)),sha256=sha(p)) for p in sorted((R/'work/natural-moveset/kick-banks-v625').glob('*.bin'))];assert len(banks)==2
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=67,bankVersion=66,clips=354,banks=banks,runtimeVerified=False,scope='Native611 exact throwhit/kick800 owned right arm plus two references to player camera/body. Bank66/354 appends existing kick donor; prior353 payloads unchanged. Ground-crouched source only, protocol67. Source outcomes/events untouched; ordinary-input runtime verification pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
