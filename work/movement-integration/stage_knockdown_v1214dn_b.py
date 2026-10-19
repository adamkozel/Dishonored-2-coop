from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214dn'; P=W/'candidate-npc-combat-v1214dm'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests','tables-tests','rebuild-tests'):
    t=(W/f'knockdown-{tag}-v1214dn.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
# 1214dn-b: resume. The first stage created the archive (src, Send-D2Command, binaries, scripts) and then stopped on the
# bank hashes it had kept from bank73; the archive is append-only, so verify its copies and continue.
assert A.is_dir()
for p in (L/'src').rglob('*'):
    if p.is_file():assert sha(p)==sha(A/'src'/p.relative_to(L/'src')),p
assert sha(L/'Send-D2Command.ps1')==sha(A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe','d2coop_setup.exe'):assert sha(R/'work/build/Release'/name)==sha(A/name),name
shutil.copy2(W/'stage_knockdown_v1214dn_b.py',A/'stage_knockdown_v1214dn_b.py');shutil.copy2(W/'Deploy-Knockdown-v1214dn-b.ps1',A/'Deploy-Knockdown-v1214dn-b.ps1')
tables=R/'work/release/native-tables-v1/native-tables.bin'
banks=[{"path": "work/natural-moveset/knockdown-banks-v1400/corvo-choke-bank.bin", "sha256": "2BA45F86AC7F35A68BEB91D212E8FD9B213CF4445B4DE3C43EB21021A3B115F5"}, {"path": "work/natural-moveset/knockdown-banks-v1400/emily-choke-bank.bin", "sha256": "91B4BFDC6EE44B0532608BD1986A3C29C14CBB4E863D016F951FA7C1B4E30DF9"}]
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
assert sha(tables)==previous['nativeTables']['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,protocol=77,npcAttackPacketVersion=previous['npcAttackPacketVersion'],bankVersion=74,clips=381,banks=banks,nativeTables=previous['nativeTables'],runtimeVerified=False,coinProtocol=previous['coinProtocol'],coinCatalogListProtocol=previous['coinCatalogListProtocol'],scope='1214dn (#11 stagger_knockdown_recovery): 1214dm plus the partner knockdown: the native knockdown (KnockedDown 16, hit_react_knocked_down Scene_1 2599 ms, an arm then the player bound) is read as stun variant 5 and presented with bank74 clip 380 (a native sleep-dart collapse, the face-down get-up and the guard rise, build_knockdown_bank_v1400.py); the lab sword hit starts off the player on its side with the nearest NPC as attacker.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
