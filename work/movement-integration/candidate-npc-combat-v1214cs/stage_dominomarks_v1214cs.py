from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214cs'; P=W/'candidate-npc-combat-v1214cr'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests','tables-tests','rebuild-tests'):
    t=(W/f'dominomarks-{tag}-v1214cs.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe','d2coop_setup.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-DominoMarks-v1214cs.ps1','Deploy-DominoMarks-v1214cs.ps1','stage_dominomarks_v1214cs.py'):shutil.copy2(W/name,A/name)
tables=R/'work/release/native-tables-v1/native-tables.bin'
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
assert sha(tables)==previous['nativeTables']['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,protocol=previous['protocol'],npcAttackPacketVersion=previous['npcAttackPacketVersion'],bankVersion=previous['bankVersion'],clips=previous['clips'],banks=previous['banks'],nativeTables=previous['nativeTables'],runtimeVerified=False,coinProtocol=previous['coinProtocol'],coinCatalogListProtocol=previous['coinCatalogListProtocol'],scope='1214cs (#11): 1214cr with the partner Domino marks kept between casts: when only the connection part of the native link read fails (stages 11-16; after the casts in mission-coverage-v1328zl a link beam FX no longer validated at stage 14), the source publishes the exactly read marks alone (domino-world.json marksOnly). The Far Reach pull loop subscene pull (100 ms, power-scenes-probe-v1351b) maps to variant 8. The probe --domino-links lists each beam FX lookup on a connection failure.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
