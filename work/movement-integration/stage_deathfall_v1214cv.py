from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214cv'; P=W/'candidate-npc-combat-v1214cu'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests','tables-tests','rebuild-tests'):
    t=(W/f'deathfall-{tag}-v1214cv.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe','d2coop_setup.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-DeathFall-v1214cv.ps1','Deploy-DeathFall-v1214cv.ps1','stage_deathfall_v1214cv.py'):shutil.copy2(W/name,A/name)
tables=R/'work/release/native-tables-v1/native-tables.bin'
banks=[{"path": "work/natural-moveset/death-banks-v1362/corvo-choke-bank.bin", "sha256": "766E67DA2C97CA8D1CF8901F1CCE1610DEE18AB19CF6D305DE71619D9B865EA1"}, {"path": "work/natural-moveset/death-banks-v1362/emily-choke-bank.bin", "sha256": "A0FFE6E7C4432CF5DD91939DD43EA70751C127D8530BFC7AC425046A769F612E"}]
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
assert sha(tables)==previous['nativeTables']['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,protocol=76,npcAttackPacketVersion=previous['npcAttackPacketVersion'],bankVersion=72,clips=378,banks=banks,nativeTables=previous['nativeTables'],runtimeVerified=False,coinProtocol=previous['coinProtocol'],coinCatalogListProtocol=previous['coinCatalogListProtocol'],scope='1214cv (#11 death_variants, #14): 1214cu plus the partner death collapse (protocol 76: native Dead state 15 -> action 25 with one directional collapse 24-27 chosen from the nearest NPC; bank72 clips 374-377 from the native sleep-dart falls, held at the end pose), a firewall returning the native empty result for unrouted world damage to the presentation clone (a host guard killed the Emily clone in take 1328zzh and the stream could not restart), and NPC attack capture-miss counters by reason.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
