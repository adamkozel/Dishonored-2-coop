from pathlib import Path
import datetime, hashlib, json, shutil

R = Path(__file__).resolve().parents[2]
W = R / 'work/movement-integration'
L = R / 'outputs/d2-coop-lab'
A = W / 'candidate-possession-target-fx-v351'
assert not A.exists(), 'Preserved archive already exists'
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
logs = [W / f'possession-target-fx-{s}-v351.log' for s in ('build','world-tests','action-tests','smoke')]
for p in logs:
    text = p.read_text(encoding='utf-8-sig')
    assert text and not any(t in text for t in ('error C','error LNK','FAIL:')), p
A.mkdir()
shutil.copytree(L/'src', A/'src')
for name in ('dinput8.dll','d2_runtime_probe.exe'):
    shutil.copy2(R/'work/build/Release'/name, A/name)
assets = A/'assets'
assets.mkdir()
for name in ('possession_target_remote.fx','possession_target_remote.provenance.json'):
    shutil.copy2(L/'assets'/name, assets/name)
for p in [Path(__file__), *logs, W/'Build-PossessionTargetFx-v351.ps1', W/'prepare_possession_fx_network_v351.py', W/'Invoke-NativeLabOperation.ps1', R/'work/natural-moveset/Test-PossessionTargetFx-v351.ps1', L/'Build-D2PossessionTargetFxAssets.ps1']:
    shutil.copy2(p, A/p.name)
x = json.loads((W/'candidate-possession-scoped-fx-v350/identity.json').read_text())
x.update(created=datetime.datetime.now().astimezone().isoformat(), dll=sha(A/'dinput8.dll'), probe=sha(A/'d2_runtime_probe.exe'), runtimeVerified=False, possessionFxProtocol=1, possessionFxBytes=172, scope='Scoped native human Possession target particles over native Steam, cosmetic receiver effect only. Exact ownership, packet ordering, world identity and proxy tests passed. Runtime pending; no NPC gameplay authority.')
x['assetHashes'].update({p.name: sha(p) for p in assets.iterdir()})
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for name in ('dinput8.dll','d2_runtime_probe.exe'):
    shutil.copy2(A/name, L/'bin'/name)
    assert sha(L/'bin'/name)==sha(A/name)
print(json.dumps(dict(archive=str(A),dll=x['dll'],probe=x['probe'])))
