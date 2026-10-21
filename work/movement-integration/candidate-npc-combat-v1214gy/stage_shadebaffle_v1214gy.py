from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214gy'; P=W/'candidate-npc-combat-v1214gx'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests','tables-tests','rebuild-tests'):
    t=(W/f'shadebaffle-{tag}-v1214gy.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe','d2coop_setup.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-ShadeBaffle-v1214gy.ps1','Deploy-ShadeBaffle-v1214gy.ps1','stage_shadebaffle_v1214gy.py'):shutil.copy2(W/name,A/name)
tables=R/'work/release/native-tables-v1/native-tables.bin'
banks=[{"path": "work/natural-moveset/shop-banks-v1409/corvo-choke-bank.bin", "sha256": "AA8DA1AE7F5123B257D482CA73AFA56CDB2F9987D1F4298EE2EBE63631E96154"}, {"path": "work/natural-moveset/shop-banks-v1409/emily-choke-bank.bin", "sha256": "0CC6EBD2CC349B3462985412C91F314F8972B5149AE9366982EE8A145CA815AD"}]
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
assert sha(tables)==previous['nativeTables']['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,protocol=80,npcAttackPacketVersion=previous['npcAttackPacketVersion'],bankVersion=75,clips=382,banks=banks,nativeTables=previous['nativeTables'],runtimeVerified=False,coinProtocol=previous['coinProtocol'],coinCatalogListProtocol=previous['coinCatalogListProtocol'],scope='1214gy (protocol 80, Doppelganger world packet v6, shade kill packet v1, #131): 1214gx with Baffling Shade confusion (decoy_baffle.damage on her copies as her shade dies; partner-shade-v1731b: 9 hits suppressed as replica-world events) counted as hers on her game and sent to the host as PeerNative, so the host NPCs are baffled natively; and the lab-only fixture coop_shade_native kill.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
