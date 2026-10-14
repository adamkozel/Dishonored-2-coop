from pathlib import Path
import json, hashlib, datetime, shutil

R = Path(__file__).resolve().parents[2]
W = R / 'work/movement-integration'
L = R / 'outputs/d2-coop-lab'
A = W / 'candidate-doppelganger-air-v512'
previous = W / 'candidate-doppelganger-air-v508'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
logs = [W / f'doppelganger-air-{x}-v512.log' for x in
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
                runtimeVerified=False, previous=previous.name, protocol=54, bankVersion=57, clips=331, cloneWireVersion=4,
                scope='Diagnostic512 freezes at most32 movement inputs and16 root samples on the first clock rejection.509 failed landing continuity; no guard relaxation or gameplay changes. Native airborne extension508 remains experimental until corrected runtime checks pass.',
                banks=[dict(path=str(p), sha256=sha(p)) for p in (A / 'banks').glob('*.bin')],
                assets=rd(previous / 'identity.json')['assets'])
(A / 'identity.json').write_text(json.dumps(identity, indent=2), encoding='utf-8')
for p in [Path(__file__), W / 'Build-DoppelgangerAir-v512.ps1', W/'prepare_doppelganger_air_build_v508.py', *logs]:
    shutil.copy2(p, A / p.name)
for name in ('dinput8.dll', 'd2_runtime_probe.exe'):
    shutil.copy2(A / name, L / 'bin' / name)
s = (W / 'Deploy-DoppelgangerHolstered-v495.ps1').read_text(encoding='utf-8-sig')
s=s.replace('doppelganger-holstered-v495','doppelganger-air-v512').replace('DoppelgangerHolsteredDeploy495','DoppelgangerAirDeploy512')
(W/'Deploy-DoppelgangerAir-v512.ps1').write_text(s,encoding='utf-8')
print(json.dumps({k:identity[k] for k in ('dll','probe','runtimeVerified','scope')}))
