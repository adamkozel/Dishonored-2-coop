from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v948'; P=W/'candidate-npc-combat-v944'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests'):
    t=(W/f'shadowwalk-body-{tag}-v948.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcCombat-v948.ps1','Deploy-NpcCombat-v948.ps1','stage_npc_combat_v948.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=69,bankVersion=67,clips=357,banks=previous['banks'],runtimeVerified=False,scope='Allows any nonempty native8-bit spread hit-region subset, including945 maskFE and multi-victim callbacks. Retains test equipment after an ordered native victim death; allows only network muzzle kind9 in the validated combat isolation scope. Optional remote cosmetic spread inflictor; missing render attachment retains native absent-inflictor default rather than refusing reliable gameplay contact. Experimental native pistol spread and regular bullet profiles, callback-time weapon/projectile attribution, hit distance and geometry, version5/276 bytes, queued multiple contacts. Native source-class modifier equivalence, full impact VFX and projectile physics unverified. Runtime pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
















