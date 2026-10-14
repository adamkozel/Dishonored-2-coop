from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-combat-target-v610';P=W/'candidate-lethal-v604'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==rd(P/'identity.json')['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke'):
 text=(W/f'shadowwalk-body-{tag}-v610.log').read_text(encoding='utf-8-sig');assert text and not any(x in text for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in (W/'prepare_combat_target_v610.py',W/'Build-CombatTarget-v610.ps1',W/'Deploy-CombatTarget-v610.ps1',R/'work/natural-moveset/Test-CombatTarget-v611.ps1',Path(__file__)):shutil.copy2(p,A/p.name)
banks=[dict(path=str(p.relative_to(R)),sha256=sha(p)) for p in sorted((P/'banks').glob('*.bin'))];assert len(banks)==2
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=65,bankVersion=65,clips=353,banks=banks,runtimeVerified=False,scope='Read-only target-bearing combat reader: exact native607 right/rightb, Scene_1/2,1000ms,standing. Validates owned player/arm and one reciprocal unique NPC; excludes partner. No bank/protocol changes, no native damage/event replay. Live validation pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
