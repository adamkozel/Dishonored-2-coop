from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214dm'; P=W/'candidate-npc-combat-v1214dl'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests','tables-tests','rebuild-tests'):
    t=(W/f'productcarry-{tag}-v1214dm.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe','d2coop_setup.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-ProductCarry-v1214dm.ps1','Deploy-ProductCarry-v1214dm.ps1','stage_productcarry_v1214dm.py'):shutil.copy2(W/name,A/name)
tables=R/'work/release/native-tables-v1/native-tables.bin'
banks=[{"path": "work/natural-moveset/recoil-banks-v1365/corvo-choke-bank.bin", "sha256": "0794EB13984D67AD4A5428048C5C103464BDA9AECB91C3C9BA00231C97CF8D66"}, {"path": "work/natural-moveset/recoil-banks-v1365/emily-choke-bank.bin", "sha256": "E786B01092F7AE27CE84660D87ED840FC8A190B4E7BCC411D4B8AF924A0E7A34"}]
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
assert sha(tables)==previous['nativeTables']['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,protocol=77,npcAttackPacketVersion=previous['npcAttackPacketVersion'],bankVersion=73,clips=380,banks=banks,nativeTables=previous['nativeTables'],runtimeVerified=False,coinProtocol=previous['coinProtocol'],coinCatalogListProtocol=previous['coinCatalogListProtocol'],scope='1214dm (#11 carry_body_lift_idle_move_drop_throw): 1214dl plus the product body carry: when the partner picks up or carries a body in an armed product session, the observer attaches its own copy (any human NPC, dead or unconscious) to the presented partner with the carry fixture native machinery (victim scene, snapshot blend, shoulder retargeting, native drop, compact drop and throw), on a rolling lease, while the NPC stream leaves that body alone.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
