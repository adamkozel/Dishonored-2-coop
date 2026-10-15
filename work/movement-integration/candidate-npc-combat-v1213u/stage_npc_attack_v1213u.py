from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1213u'; P=W/'candidate-npc-combat-v1213t'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests'):
    t=(W/f'npc-parry-{tag}-v1213u.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcAttack-v1213u.ps1','Deploy-NpcAttack-v1213u.ps1','stage_npc_attack_v1213u.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
banks=[dict(path=f'work/natural-moveset/use-banks-v1212/{r}-choke-bank.bin',sha256=sha(R/f'work/natural-moveset/use-banks-v1212/{r}-choke-bank.bin')) for r in ('corvo','emily')]
manifest=json.loads((R/'work/natural-moveset/use-bank-manifest-v1212.json').read_text(encoding='utf-8'))
assert [b['sha256'] for b in banks]==[m['sha256'] for m in manifest]
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=75,npcAttackPacketVersion=3,bankVersion=71,clips=374,banks=banks,runtimeVerified=False,coinProtocol=7,coinCatalogListProtocol=1,scope='Experimental1213u on 1213t (#46): client paired takedowns on host-owned NPCs. NPC control messages PairedHold/PairedKnockout (variant in reserved: choke 1-3, lethal 5; unit-tested). Client: reads its own native choke/assassination scene; while it targets a bound replica the replica keeps its native victim animation (host poses not applied), it sends PairedHold every 200 ms and PairedKnockout once at choke_win (3), and a natively unconscious replica keeps its channel. Host (npc_paired.h): PairedHold with the partner within 3 m pauses the NPC AI with the native AI lease (lapses 1.5 s after the last hold); PairedKnockout releases it and applies sleep_damage (probe 1251b drowsy collapse). Lethal holds only; death rides the assassination relay.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
