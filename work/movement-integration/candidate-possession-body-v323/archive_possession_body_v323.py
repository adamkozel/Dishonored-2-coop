from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-body-v323'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
for kind in ('build','smoke','action','bank'):
 s=(W/f'possession-fixture-{kind}-v323.log').read_text(encoding='utf-8-sig');assert s and 'FAIL' not in s and 'error C' not in s and 'error LNK' not in s
A.mkdir();shutil.copytree(S/'src',A/'src');shutil.copytree(W/'candidate-swarm-trails-v320/assets',A/'assets')
shutil.copytree(R/'work/natural-moveset/possession-banks-v323',A/'banks')
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in [Path(__file__),W/'prepare_possession_body_v323.py',W/'prepare_possession_network_v323.py',R/'work/natural-moveset/build_possession_bank_v323.py',R/'work/natural-moveset/possession-bank-manifest-v323.json',R/'work/natural-moveset/possession-bank-source-manifest-v323.json',W/'Invoke-NativeLabOperation.ps1',R/'work/natural-moveset/Test-PossessionNetwork-v323.ps1',S/'Send-D2Command.ps1',*W.glob('possession-fixture-*-v323.log'),*W.glob('possession-*-v320.*')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-trails-v320/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,protocol=48,bankVersion=51,clips=315,scope='Candidate323: exact observed Corvo standing armed empty-target Possession entry333/aim200 scene reader drives semantic protocol48 and full-body bank51/315. Native first-person and network root remain authoritative. Source scene retirement returns remote body to idle. No successful-target out phase, transfer, visibility, VFX, damage or alternate-context claim. All previous313 clip payloads retained. Native runtime and visuals pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe')}))
