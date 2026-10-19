from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214dp'; P=W/'candidate-npc-combat-v1214do'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests','tables-tests','rebuild-tests'):
    t=(W/f'stuneligible-{tag}-v1214dp.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe','d2coop_setup.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-StunEligible-v1214dp.ps1','Deploy-StunEligible-v1214dp.ps1','stage_stuneligible_v1214dp.py'):shutil.copy2(W/name,A/name)
tables=R/'work/release/native-tables-v1/native-tables.bin'
banks=[{"path": "work/natural-moveset/knockdown-banks-v1400/corvo-choke-bank.bin", "sha256": "2BA45F86AC7F35A68BEB91D212E8FD9B213CF4445B4DE3C43EB21021A3B115F5"}, {"path": "work/natural-moveset/knockdown-banks-v1400/emily-choke-bank.bin", "sha256": "91B4BFDC6EE44B0532608BD1986A3C29C14CBB4E863D016F951FA7C1B4E30DF9"}]
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
assert sha(tables)==previous['nativeTables']['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,protocol=77,npcAttackPacketVersion=previous['npcAttackPacketVersion'],bankVersion=74,clips=381,banks=banks,nativeTables=previous['nativeTables'],runtimeVerified=False,coinProtocol=previous['coinProtocol'],coinCatalogListProtocol=previous['coinCatalogListProtocol'],scope='1214dp (#11 hit_reactions, stagger_knockdown_recovery): 1214do plus a presented stun (protocol state 16) keeps the movement clock eligible, so the stun and knockdown clips present instead of the actor native pose (1397k/l), and the knockdown skips the standing foot plant.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
