from pathlib import Path
import json,hashlib,datetime,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-unbound-presentation-v558'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=W/'candidate-shadowwalk-sneak-v550';probe=previous
assert rd(W/'native-operation-owner.json')['state']=='finished'
assert sha(R/'work/asset-game/dinput8.dll')==rd(previous/'identity.json')['dll']
assert sha(L/'bin/d2_runtime_probe.exe')==rd(probe/'identity.json')['probe']
logs=[W/f'shadowwalk-body-{x}-v558.log' for x in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
    s=p.read_text(encoding='utf-8-sig');assert s and not any(t in s for t in ('error C','error LNK','FAIL ', 'Traceback')),p
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src')
shutil.copytree(R/'work/natural-moveset/shadowwalk-sneak-banks-v548',A/'banks')
shutil.copytree(previous/'assets',A/'assets')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=previous.name,previousProbe=probe.name,protocol=59,bankVersion=59,clips=336,cloneWireVersion=4,runtimeVerified=False,
    scope='Explicit state18 for unbound grounded presentation. Known native physical standing/crouching and owned stable sword are revalidated independently; incomplete action fields are discarded. Unsupported gesture cannot stop otherwise valid movement. Air, alternate modes, invalid owners, world changes and timeouts remain rejected. Ground bank59/336 unchanged. Bounded6000ms test command suppresses only known cosmetic equipment binding for an isolated integration test. Full moveset and visual acceptance remain incomplete.',
    banks=[dict(path=str(p),sha256=sha(p)) for p in (A/'banks').glob('*.bin')],assets=rd(previous/'identity.json')['assets'])
(A/'identity.json').write_text(json.dumps(identity,indent=2),encoding='utf-8')
for p in [Path(__file__),W/'prepare_unbound_presentation_v558.py',W/'prepare_unbound_runtime_v559.py',W/'Build-UnboundPresentation-v558.ps1',*logs]:shutil.copy2(p,A/p.name)
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps({k:identity[k] for k in ('dll','probe','protocol','bankVersion','runtimeVerified')}))
