from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-choke-breakout-v592';P=W/'candidate-choke-network-v584'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==rd(P/'identity.json')['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke'):
 text=(W/f'shadowwalk-body-{tag}-v592.log').read_text(encoding='utf-8-sig');assert text and not any(x in text for x in ('error C','error LNK','FAIL ', 'Traceback'))
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src');shutil.copytree(R/'work/natural-moveset/choke-breakout-banks-v592',A/'banks')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in (W/'implement_choke_breakout_v592.py',W/'Build-Choke-v592.ps1',W/'prepare_choke_runtime_v583.py',Path(__file__),R/'work/natural-moveset/build_choke_breakout_v592.py'):shutil.copy2(p,A/p.name)
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=64,bankVersion=64,clips=352,runtimeVerified=False,scope='Native591 combat-lose1500 binding, using existing release/front-hit/idle body data. All previous351 clip payloads unchanged. Actor presentation only: no remote victim events, attachment, health or shared outcome. Exact state/target/arm ownership preserved; native cancellation400 still unverified. Runtime validation pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))

