from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214fn'; P=W/'candidate-npc-combat-v1214fm'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests','tables-tests','rebuild-tests'):
    t=(W/f'linkedsleep-{tag}-v1214fn.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe','d2coop_setup.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-LinkedSleep-v1214fn.ps1','Deploy-LinkedSleep-v1214fn.ps1','stage_linkedsleep_v1214fn.py'):shutil.copy2(W/name,A/name)
tables=R/'work/release/native-tables-v1/native-tables.bin'
banks=[{"path": "work/natural-moveset/shop-banks-v1409/corvo-choke-bank.bin", "sha256": "AA8DA1AE7F5123B257D482CA73AFA56CDB2F9987D1F4298EE2EBE63631E96154"}, {"path": "work/natural-moveset/shop-banks-v1409/emily-choke-bank.bin", "sha256": "0CC6EBD2CC349B3462985412C91F314F8972B5149AE9366982EE8A145CA815AD"}]
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
assert sha(tables)==previous['nativeTables']['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,protocol=80,npcAttackPacketVersion=previous['npcAttackPacketVersion'],bankVersion=75,clips=382,banks=banks,nativeTables=previous['nativeTables'],runtimeVerified=False,coinProtocol=previous['coinProtocol'],coinCatalogListProtocol=previous['coinCatalogListProtocol'],scope='1214fn (protocol 80, #117): 1214fm plus linked knockouts. Domino repeats a choke on each linked NPC as sleep damage with the link flag; sleep damage passed through the client natively, so the client copy was knocked out locally, its channel ended (ai_ownership_changed) and only the Life roster carried the knockout to the host (carry-body-v1653b). On the client a linked sleep hit is now captured and sent as a Linked request, or as PeerNative without flags; the host knocks its NPC out and Unconscious shares it. generic_sleep_damage joins the declaration names; unit tests cover both requests.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
