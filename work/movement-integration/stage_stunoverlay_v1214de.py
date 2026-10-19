from pathlib import Path
import json, shutil, hashlib, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-npc-combat-v1214de'; P=W/'candidate-npc-combat-v1214dd'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=json.loads((P/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==previous['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke','resume-tests','checkpoint-tests','pose-tests','arm-reach-tests','tables-tests','rebuild-tests'):
    t=(W/f'stunoverlay-{tag}-v1214de.log').read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C','error LNK','FAIL ','Traceback'))
A.mkdir(exist_ok=False); shutil.copytree(L/'src', A/'src')
shutil.copy2(L/'Send-D2Command.ps1', A/'Send-D2Command.ps1')
for name in ('dinput8.dll','d2_runtime_probe.exe','d2coop_setup.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for name in ('Build-StunOverlay-v1214de.ps1','Deploy-StunOverlay-v1214de.ps1','stage_stunoverlay_v1214de.py'):shutil.copy2(W/name,A/name)
tables=R/'work/release/native-tables-v1/native-tables.bin'
banks=[{"path": "work/natural-moveset/recoil-banks-v1365/corvo-choke-bank.bin", "sha256": "0794EB13984D67AD4A5428048C5C103464BDA9AECB91C3C9BA00231C97CF8D66"}, {"path": "work/natural-moveset/recoil-banks-v1365/emily-choke-bank.bin", "sha256": "E786B01092F7AE27CE84660D87ED840FC8A190B4E7BCC411D4B8AF924A0E7A34"}]
for bank in banks:assert sha(R/bank['path'])==bank['sha256']
assert sha(tables)==previous['nativeTables']['sha256']
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),setup=sha(A/'d2coop_setup.exe'),previous=P.name,protocol=77,npcAttackPacketVersion=previous['npcAttackPacketVersion'],bankVersion=73,clips=380,banks=banks,nativeTables=previous['nativeTables'],runtimeVerified=False,coinProtocol=previous['coinProtocol'],coinCatalogListProtocol=previous['coinCatalogListProtocol'],scope='1214de (#11 hit_reactions_directions): 1214dd plus the stun admitted with the left hand EquipChange overlay (state 64, vtable 0x1c79cf8) beside Stun14 and the optional holster 53. A guard-fight stun with a power selected carries it (1371f [14,64,53]; 1328zzh/zzc/zzf), and failing closed refused the whole stun, so the partner dropped out instead of staggering. Also the left slashes that bind the struck guard (sword_slash_left Scene_1/Scene_2, sword_slash_leftb Corvo/emily, 1000 ms) and the right b-hit Scene_2 (733 ms), refused until now (recoil watcher 1371a/b), which starved the stream on connecting left swings.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
