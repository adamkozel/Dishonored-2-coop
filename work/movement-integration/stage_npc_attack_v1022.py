from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1022'; P=W/'candidate-npc-combat-v1018'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests'):
    t=(W/f'npc-parry-{tag}-v1022.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcAttack-v1022.ps1','Deploy-NpcAttack-v1022.ps1','stage_npc_attack_v1022.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
banks=[dict(path=f'work/natural-moveset/parry-banks-v1022/{r}-choke-bank.bin',sha256=sha(R/f'work/natural-moveset/parry-banks-v1022/{r}-choke-bank.bin')) for r in ('corvo','emily')]
manifest=json.loads((R/'work/natural-moveset/parry-bank-manifest-v1022.json').read_text(encoding='utf-8'))
assert [b['sha256'] for b in banks]==[m['sha256'] for m in manifest]
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=70,npcAttackPacketVersion=3,bankVersion=68,clips=359,banks=banks,runtimeVerified=False,scope='Experimental1022 on verified1018: remote-body presentation of the receiving player native defense scenes. Reader variants19/20 sword_parry_hit_1/hit_2 (Scene_1/533) and21 sword_parry_win (Scene_1/500) inside block state44 with exact player/right-arm/attacker bindings; action protocol70 accepts them only in the block state; bank68/359 appends native NPC deflection donors357 (sword_parry_ll_out) and358 (sword_parry_rl_out) with prior357 payloads unchanged. Retains 1018 timed-parry relay. Runtime pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
















