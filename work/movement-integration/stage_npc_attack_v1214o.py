from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214o'; P=W/'candidate-npc-combat-v1214n'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests'):
    t=(W/f'npc-parry-{tag}-v1214o.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcAttack-v1214o.ps1','Deploy-NpcAttack-v1214o.ps1','stage_npc_attack_v1214o.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
banks=[dict(path=f'work/natural-moveset/use-banks-v1266/{r}-choke-bank.bin',sha256=sha(R/f'work/natural-moveset/use-banks-v1266/{r}-choke-bank.bin')) for r in ('corvo','emily')]
manifest=json.loads((R/'work/natural-moveset/use-bank-manifest-v1266.json').read_text(encoding='utf-8'))
assert [b['sha256'] for b in banks]==[m['sha256'] for m in manifest]
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=75,npcAttackPacketVersion=3,bankVersion=71,clips=374,banks=banks,runtimeVerified=False,coinProtocol=7,coinCatalogListProtocol=1,scope="Experimental1214o on 1214n (#14 multi-enemy combat): host-world damage (neither attacker nor inflictor is the local player, a player-owned entity or a world sentinel; take 1270e: bloodflies on a bound checkpoint guard were refused, its channel retired, and the host's next sword hits and the death went unshared) passes the host capture natively; the host commits its observed native loss as an ordered HostWorld commit and the client removes exactly that loss by the native health subtraction (health vtable +0xf8). A Hello with a lower host health makes the client follow it (the host waits) instead of retiring the NPC for good. Combat JSON records each channel's retirement reason and shared health.")
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
