from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-crouch-stance-v620';P=W/'candidate-crouch-combat-v616'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==rd(P/'identity.json')['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke'):
 text=(W/f'shadowwalk-body-{tag}-v620.log').read_text(encoding='utf-8-sig');assert text and not any(x in text for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in (W/'prepare_crouch_stance_v620.py',W/'Build-CrouchStance-v620.ps1',W/'Deploy-CrouchStance-v620.ps1',R/'work/natural-moveset/Test-CrouchCombat-v621.ps1',Path(__file__)):shutil.copy2(p,A/p.name)
banks=rd(P/'identity.json')['banks'];assert len(banks)==2
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=66,bankVersion=65,clips=353,banks=banks,runtimeVerified=False,scope='Native613/619 target-bearing crouched strikes can keep their native933 scene while rising to standing, or switch to optional owned hit-recovery866/666. Existing bank65/353 reused; protocol66 adds combat variants15/16. Continue prior authored strike into follow-through and blend native stance recovery. Source owns all victim events/outcomes. Build and runtime validation pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
