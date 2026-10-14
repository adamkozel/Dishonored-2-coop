from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1188'; P=W/'candidate-npc-combat-v1181'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests'):
    t=(W/f'npc-parry-{tag}-v1188.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-NpcAttack-v1188.ps1','Deploy-NpcAttack-v1188.ps1','stage_npc_attack_v1188.py'):shutil.copy2(W/name,A/name)
for bank in previous['banks']:assert sha(R/bank['path'])==bank['sha256']
banks=[dict(path=f'work/natural-moveset/climb-banks-v1042/{r}-choke-bank.bin',sha256=sha(R/f'work/natural-moveset/climb-banks-v1042/{r}-choke-bank.bin')) for r in ('corvo','emily')]
manifest=json.loads((R/'work/natural-moveset/climb-bank-manifest-v1042.json').read_text(encoding='utf-8'))
assert [b['sha256'] for b in banks]==[m['sha256'] for m in manifest]
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=73,npcAttackPacketVersion=3,bankVersion=70,clips=369,banks=banks,runtimeVerified=False,coinProtocol=7,coinCatalogListProtocol=1,scope='Experimental1188 on 1181: join-follow - the host publishes its current mission (idCommonLocal+0x2b8 idDeclMapInfo name, chaos from the map name) on D2MF packets; a joiner on another map loads the same mission natively with campaign <mapinfo> <chaos> emily (product mode automatic; lab: coop_follow_host on). Host plays Corvo. Runtime pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
















