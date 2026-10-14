from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v926'; P=W/'candidate-npc-combat-v922'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests'):
    t=(W/f'shadowwalk-body-{tag}-v926.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcCombat-v926.ps1','Deploy-NpcCombat-v926.ps1','stage_npc_combat_v926.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=69,bankVersion=67,clips=357,banks=previous['banks'],runtimeVerified=False,scope='Accepts both native absent and validated contact geometry for authored trigger effect continuations, including zero-input terminal outcomes. Adds an explicit once-per-combat-session native event reconstruction fixture for the observed map trigger damage, requiring the active finite isolated test lease. Source activation remains separately unverified. Adds generic host-owned authored map-trigger damage with unique native trigger names/incarnations and an exact-installation declaration catalog, dynamic bounded amount, native absent hit geometry, duplicate and native zero-result validation. Combat wire4/256 bytes. Retains candidate915 corpse, melee, replica authority, evaluated subtraction and pending-request fixes. Runtime verification pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
















