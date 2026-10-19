from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214ew'; P=W/'candidate-npc-combat-v1214ev'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
t=(W/'probeairegistry-build-v1214ew.log').read_text(encoding='utf-8-sig')
assert t and not any(x in t for x in ('error C','error LNK','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
# The DLL and setup are 1214eu's (their sources did not change); the probe is the new build.
for name in ('dinput8.dll','d2coop_setup.exe'):shutil.copy2(P/name,A/name)
shutil.copy2(R/'work/build/Release'/'d2_runtime_probe.exe',A/'d2_runtime_probe.exe')
for name in ('Build-ProbeAiRegistry-v1214ew.ps1','Deploy-ProbeAiRegistry-v1214ew.ps1','stage_probeairegistry_v1214ew.py'):shutil.copy2(W/name,A/name)
identity=dict(previous)
identity.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,runtimeVerified=False,
    scope='1214ew: probe-only candidate on the 1214eu DLL (protocol 80). Read-only research probe mode --npc-ai-registries HANDLE (each AI component of one NPC, including the civilian decision arkCpntAIDecisionCivilian VT 1ccb1f8, and the AI-manager registry holding it) for #113; 1214ev modes kept.')
assert identity['dll']==previous['dll']
(A/'identity.json').write_text(json.dumps(identity,indent=2))
shutil.copy2(A/'d2_runtime_probe.exe',L/'bin'/'d2_runtime_probe.exe')
print(json.dumps({k:identity[k] for k in ('dll','probe','previous')}))
