from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1026'; P=W/'candidate-npc-combat-v1022'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests'):
    t=(W/f'npc-parry-{tag}-v1026.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcAttack-v1026.ps1','Deploy-NpcAttack-v1026.ps1','stage_npc_attack_v1026.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
banks=[dict(path=f'work/natural-moveset/parry-banks-v1022/{r}-choke-bank.bin',sha256=sha(R/f'work/natural-moveset/parry-banks-v1022/{r}-choke-bank.bin')) for r in ('corvo','emily')]
manifest=json.loads((R/'work/natural-moveset/parry-bank-manifest-v1022.json').read_text(encoding='utf-8'))
assert [b['sha256'] for b in banks]==[m['sha256'] for m in manifest]
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=70,npcAttackPacketVersion=3,bankVersion=68,clips=359,banks=banks,runtimeVerified=False,scope='Experimental1026 on 1022 (verified defense presentation 1025): adds lab-only observer camera (hook on native render-view setter 140890c50, 18-byte prologue; coop_observer_camera hold/off freezes the most active native view pose for at most 120 s; no player/entity transform change; not network-visible). Probe adds read-only --watch-combat-scene. Protocol70, bank68/359 and relay unchanged. Runtime pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
















