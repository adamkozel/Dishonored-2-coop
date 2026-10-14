from pathlib import Path
import json, hashlib, datetime, shutil

R = Path(__file__).resolve().parents[2]
W = R / 'work/movement-integration'
L = R / 'outputs/d2-coop-lab'
A = W / 'candidate-doppelganger-fx-v481'
previous = W / 'candidate-doppelganger-fx-v479'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
logs = [W / f'doppelganger-visual-{x}-v481.log' for x in
        ('build', 'world-tests', 'action-tests', 'bank-tests', 'smoke')]
for p in logs:
    t = p.read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C', 'error LNK', 'FAIL:', 'Traceback')), p
assert sha(R / 'work/asset-game/dinput8.dll') == rd(previous / 'identity.json')['dll']
A.mkdir()
shutil.copytree(L / 'src', A / 'src')
for directory in ('banks', 'assets'):
    shutil.copytree(previous / directory, A / directory)
for name in ('dinput8.dll', 'd2_runtime_probe.exe'):
    shutil.copy2(R / 'work/build/Release' / name, A / name)
identity = dict(created=datetime.datetime.now().astimezone().isoformat(),
                dll=sha(A / 'dinput8.dll'), probe=sha(A / 'd2_runtime_probe.exe'),
                runtimeVerified=False, previous=previous.name,
                scope='Native480 exact rigid leaf marker offsets exceed former3m bound. Wire4 keeps10712 bytes; only verified219-rig leaves1/10/17 allow10m, other joints3m. Shared sender/receiver/interpolation validation and regression cases; cadence475 and finite native FX468 retained',
                banks=[dict(path=str(p), sha256=sha(p)) for p in (A / 'banks').glob('*.bin')],
                assets=rd(previous / 'identity.json')['assets'])
(A / 'identity.json').write_text(json.dumps(identity, indent=2), encoding='utf-8')
for p in [Path(__file__), W / 'Build-DoppelgangerVisual-v481.ps1', *logs]:
    shutil.copy2(p, A / p.name)
for name in ('dinput8.dll', 'd2_runtime_probe.exe'):
    shutil.copy2(A / name, L / 'bin' / name)
s = (W / 'Deploy-DoppelgangerVisual-v427.ps1').read_text(encoding='utf-8-sig')
s = s.replace('doppelganger-visual-v424', 'doppelganger-fx-v481').replace('DoppelgangerDeploy427', 'DoppelgangerFxDeploy481')
(W / 'Deploy-DoppelgangerFx-v481.ps1').write_text(s, encoding='utf-8')
print(json.dumps({k: identity[k] for k in ('dll', 'probe', 'runtimeVerified', 'scope')}))
