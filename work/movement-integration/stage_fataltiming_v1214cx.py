from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214cx'; P=W/'candidate-npc-combat-v1214cw'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests','tables-tests','rebuild-tests'):
    t=(W/f'fataltiming-{tag}-v1214cx.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe','d2coop_setup.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-FatalityTiming-v1214cx.ps1','Deploy-FatalityTiming-v1214cx.ps1','stage_fataltiming_v1214cx.py'):shutil.copy2(W/name,A/name)
tables=R/'work/release/native-tables-v1/native-tables.bin'
banks=[{"path": "work/natural-moveset/death-banks-v1362/corvo-choke-bank.bin", "sha256": "766E67DA2C97CA8D1CF8901F1CCE1610DEE18AB19CF6D305DE71619D9B865EA1"}, {"path": "work/natural-moveset/death-banks-v1362/emily-choke-bank.bin", "sha256": "A0FFE6E7C4432CF5DD91939DD43EA70751C127D8530BFC7AC425046A769F612E"}]
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
assert sha(tables)==previous['nativeTables']['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,protocol=76,npcAttackPacketVersion=previous['npcAttackPacketVersion'],bankVersion=72,clips=378,banks=banks,nativeTables=previous['nativeTables'],runtimeVerified=False,coinProtocol=previous['coinProtocol'],coinCatalogListProtocol=previous['coinCatalogListProtocol'],scope='1214cx (#11 death_variants): 1214cw with each finisher collapse starting 0.4 s earlier (guards 1.6 s; native GameOver retires the dead source stream about 3.8 s into the guards finisher and Emily vanished still kneeling in 1361o) and the fatality scene reader reporting the stage it fails at (the client read failed in 1361o).')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
