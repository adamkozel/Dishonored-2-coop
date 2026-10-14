from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1018'; P=W/'candidate-npc-combat-v1016'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests'):
    t=(W/f'npc-parry-{tag}-v1018.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcAttack-v1018.ps1','Deploy-NpcAttack-v1018.ps1','stage_npc_attack_v1018.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=69,npcAttackPacketVersion=3,bankVersion=67,clips=357,banks=previous['banks'],runtimeVerified=False,scope='Experimental1018 on accepted1012 (1016 status-key defect fixed: blowback object no longer collides with the parryWin counter): passive observer on native arkPlayerStateBlock resolution 859980 (17-byte prologue trampoline, records result/attacker/window/relay); host applies receiver ParryWin through native 140636140 on its authoritative attacker with the owned clone as defender (scoped firewall exemption for exactly blowback_player.damage from the clone, zero-health-change required) and native m_blowBackNpcFX 635bf0; receiver plays the same FX on its replica. Probe adds read-only --watch-block-state. Packet v3/176 unchanged. Runtime pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
















