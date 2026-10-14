from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab'
A=W/'candidate-auto-crouch-v651';P=W/'candidate-heavy-v635'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
previous=rd(P/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke'):
 t=(W/f'shadowwalk-body-{tag}-v651.log').read_text(encoding='utf-8-sig');assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('prepare_auto_crouch_v651.py','Build-AutoCrouch-v651.ps1','Deploy-AutoCrouch-v651.ps1','stage_auto_crouch_v651.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=69,bankVersion=67,clips=357,banks=previous['banks'],runtimeVerified=False,scope='649/650 observed native automatic crouch10/raw2 and owned low mantle next10 map to existing crouch presentation. Owned333ms right-arm recovery; exact classes/scenes/owners/clocks retained. Unchanged animation banks and wire. Both-role ordinary-input runtime verification pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
