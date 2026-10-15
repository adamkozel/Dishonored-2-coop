from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1213m'; P=W/'candidate-npc-combat-v1213l'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests'):
    t=(W/f'npc-parry-{tag}-v1213m.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcAttack-v1213m.ps1','Deploy-NpcAttack-v1213m.ps1','stage_npc_attack_v1213m.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
banks=[dict(path=f'work/natural-moveset/use-banks-v1212/{r}-choke-bank.bin',sha256=sha(R/f'work/natural-moveset/use-banks-v1212/{r}-choke-bank.bin')) for r in ('corvo','emily')]
manifest=json.loads((R/'work/natural-moveset/use-bank-manifest-v1212.json').read_text(encoding='utf-8'))
assert [b['sha256'] for b in banks]==[m['sha256'] for m in manifest]
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=75,npcAttackPacketVersion=3,bankVersion=71,clips=374,banks=banks,runtimeVerified=False,coinProtocol=7,coinCatalogListProtocol=1,scope='Experimental1213m on 1213l: one product-aware hold for every partner-presentation timeline (presentation_hold.h). Under the product controller the movement semantics and all 12 action timelines hold their last source sample for the 1 s presentation lease, bridge source gaps up to 1 s without clearing, and run a held action on real time; eligibility mask 16 (stale source state) uses presentationFreshMs(). Lab rules (200 ms hold, 250 ms gap clear, 100 ms extrapolation, 250 ms state age) are unchanged. Take 1235d showed 250-375 ms state gaps on Edge of the World snapping the partner to its native pose every ~0.6 s.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
