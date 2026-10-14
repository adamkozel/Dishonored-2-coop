from pathlib import Path
import json, hashlib, datetime, shutil

R = Path(__file__).resolve().parents[2]
W = R / 'work/movement-integration'
L = R / 'outputs/d2-coop-lab'
A = W / 'candidate-doppelganger-holstered-v495'
previous = W / 'candidate-doppelganger-crouch-v490'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
logs = [W / f'doppelganger-visual-{x}-v495.log' for x in
        ('build', 'world-tests', 'action-tests', 'bank-tests', 'smoke')]
for p in logs:
    t = p.read_text(encoding='utf-8-sig')
    assert t and not any(x in t for x in ('error C', 'error LNK', 'FAIL:', 'FAIL ', 'Traceback')), p
assert sha(R / 'work/asset-game/dinput8.dll') == rd(previous / 'identity.json')['dll']
A.mkdir()
shutil.copytree(L / 'src', A / 'src')
shutil.copytree(R / 'work/natural-moveset/doppelganger-holstered-banks-v495', A / 'banks')
shutil.copytree(previous / 'assets', A / 'assets')
for name in ('dinput8.dll', 'd2_runtime_probe.exe'):
    shutil.copy2(R / 'work/build/Release' / name, A / name)
identity = dict(created=datetime.datetime.now().astimezone().isoformat(),
                dll=sha(A / 'dinput8.dll'), probe=sha(A / 'd2_runtime_probe.exe'),
                runtimeVerified=False, previous=previous.name, protocol=53, bankVersion=57, clips=331, cloneWireVersion=4,
                scope='493/494 confirm holstered standing/crouched native UsePower63 with unchanged466/1200/666ms phases. Six full-body hands-free clips325-330 in bank57 retain all325 old payloads. Semantic53 admits only observed ground sword states; clone4 unchanged. No FP/root/events changes. Runtime and visual verification pending.',
                banks=[dict(path=str(p), sha256=sha(p)) for p in (A / 'banks').glob('*.bin')],
                assets=rd(previous / 'identity.json')['assets'])
(A / 'identity.json').write_text(json.dumps(identity, indent=2), encoding='utf-8')
for p in [Path(__file__), W / 'Build-DoppelgangerHolstered-v495.ps1', R/'work/natural-moveset/build_doppelganger_holstered_v495.py', R/'work/natural-moveset/doppelganger-holstered-bank-manifest-v495.json', R/'work/natural-moveset/doppelganger-holstered-source-manifest-v495.json', W/'doppelganger-holstered-authoring-v495.log', *logs]:
    shutil.copy2(p, A / p.name)
for name in ('dinput8.dll', 'd2_runtime_probe.exe'):
    shutil.copy2(A / name, L / 'bin' / name)
s = (W / 'Deploy-DoppelgangerVisual-v427.ps1').read_text(encoding='utf-8-sig')
s = s.replace('doppelganger-visual-v424', 'doppelganger-holstered-v495').replace('DoppelgangerDeploy427', 'DoppelgangerHolsteredDeploy495')
(W / 'Deploy-DoppelgangerHolstered-v495.ps1').write_text(s, encoding='utf-8')
print(json.dumps({k: identity[k] for k in ('dll', 'probe', 'runtimeVerified', 'scope')}))
