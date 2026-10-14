from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1012'; P=W/'candidate-npc-combat-v1002'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests'):
    t=(W/f'shadowwalk-body-{tag}-v1012.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcAttack-v1012.ps1','Deploy-NpcAttack-v1012.ps1','stage_npc_attack_v1012.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=69,npcAttackPacketVersion=3,bankVersion=67,clips=357,banks=previous['banks'],runtimeVerified=False,scope='Experimental1012: receiver-side native melee defense. Host attaches weapon blockable/heavy bits (arkCpntWeaponMeleeNPC decl+0x180, +0x3428 bit2) to packet v3; receiver runs native arkCpntBlock camera cone 60d750 and arkPlayerStateBlock 859980 against the local replica attacker before any 717950 call; ack carries Blocked/ParryLose/ParryWin; host posts native event 0xe6 to its authoritative attacker for block/parry-lose. Parry-win attacker stagger not mirrored. Retains 1002 recovery/renewal. Runtime pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
















