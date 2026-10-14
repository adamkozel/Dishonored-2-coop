from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-trap-network-v575';P=W/'candidate-trap-network-v571'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
assert sha(R/'work/asset-game/dinput8.dll')==rd(P/'identity.json')['dll']
for tag in ('build','world-tests','action-tests','bank-tests','smoke'):
    text=(W/f'shadowwalk-body-{tag}-v575.log').read_text(encoding='utf-8-sig');assert text and not any(x in text for x in ('error C','error LNK','FAIL ', 'Traceback'))
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src');shutil.copytree(R/'work/natural-moveset/trap-banks-v575',A/'banks');shutil.copy2(L/'assets/stunmine_prop.def',A/'stunmine_prop.def');shutil.copy2(L/'assets/springrazor_prop.def',A/'springrazor_prop.def')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in (W/'extend_trap_recovery_v575.py',W/'Build-Traps-v575.ps1',W/'prepare_trap_runtime_v570.py',Path(__file__),L/'Send-D2Command.ps1'):shutil.copy2(p,A/p.name)
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=62,bankVersion=62,clips=348,runtimeVerified=False,scope='Preserves prior340 clips. Adds native trap body/held-prop presentation through Steam, state20, protocol62. No remote native placement/damage calls. Preserves corrected native clock and all prior347 clips; adds observed866ms reload/recovery branch from Emily572. Runtime576 verification pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
print(json.dumps(identity))


