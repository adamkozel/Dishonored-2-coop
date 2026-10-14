from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v915'; P=W/'candidate-npc-combat-v911'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests'):
    t=(W/f'shadowwalk-body-{tag}-v915.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcCombat-v915.ps1','Deploy-NpcCombat-v915.ps1','stage_npc_combat_v915.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=69,bankVersion=67,clips=357,banks=previous['banks'],runtimeVerified=False,scope='Suppresses client-owned replica world callbacks for bound host-authoritative NPC victims unless an exact current native local-player source is attributable; local player requests retain profile validation. Adds host-owned corpse collision outcomes with native NPC source-arm mapping and replica duplicate-world suppression; combat wire3/232bytes, bounded source-death ordering wait, zero harmless corpse results. Runtime verification pending. Adds a bounded per-actor FIFO for compound native client damage contacts, preserving pending request order until acknowledgement instead of dropping the second contact. Adds native evaluated-health subtraction ownership hook, floating-point damage conservation, and nested native damage capture. Experimental native melee and terminal outcome authority: client request, host native outcome ordering, ordinary sword, generic fatality and assassination damage, per-actor incarnation and session nonces, deduplication. Native pose/AI references retire before death; scoped terminal tombstones reject duplicate damage. Runtime lethal verification pending. Extends native fixed-slot checkpoint retry permission through pre-cache save-I/O deferral observed878; runtime verification pending. Retains native AI and combat bindings through skeletal packet gaps while reliable heartbeats and exact native ownership stay current. Old render data still expires. Releases finite test source-frame ownership on verified native host ground-assassination state13 while retaining AI leases. Adds an explicit finite AI-only fixture without reference-frame holds to compare native finisher root motion. Records a bounded failing native joint transform/root and adds source pose rejection diagnostics and explicit failure on uncommitted actor retirement. Runtime verification pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
















