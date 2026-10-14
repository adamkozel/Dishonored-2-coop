from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-choke-network-v581';P=W/'candidate-trap-network-v575'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==rd(P/'identity.json')['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke'):
 text=(W/f'shadowwalk-body-{tag}-v581.log').read_text(encoding='utf-8-sig');assert text and not any(x in text for x in ('error C','error LNK','FAIL ', 'Traceback'))
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src');shutil.copytree(R/'work/natural-moveset/choke-banks-v581',A/'banks')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in (W/'implement_choke_v581.py',W/'Build-Choke-v581.ps1',W/'prepare_choke_runtime_v583.py',Path(__file__),R/'work/natural-moveset/build_choke_bank_v581.py'):shutil.copy2(p,A/p.name)
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=63,bankVersion=63,clips=351,runtimeVerified=False,scope='Existing authored rear choke entry600/loop1400/finish1800 bound to native owned source scenes and unique authored NPC identity. Only remote player body presentation; no remote victim animation, attachment, damage, knockout or native gameplay event. Original348 clip payloads unchanged. Runtime583 pending. Preserves unresolved intermittent trap576 freshness dip.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))
