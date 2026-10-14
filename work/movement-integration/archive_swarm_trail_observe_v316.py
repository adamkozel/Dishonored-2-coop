from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-trail-observe-v316'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
assert 'error C' not in (W/'swarm-trail-observe-build-v316.log').read_text()
assert 'Target is not Dishonored2.exe' in (W/'swarm-trail-observe-cli-v316.log').read_text()
previous=read(W/'candidate-swarm-spawn-fx-v315/identity.json')
assert sha(R/'work/build/Release/dinput8.dll')==previous['dll']
A.mkdir();shutil.copytree(S/'src',A/'src');shutil.copytree(W/'candidate-swarm-spawn-fx-v315/assets',A/'assets')
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in [Path(__file__),W/'Invoke-NativeLabOperation.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v315.ps1',*W.glob('swarm-trail-observe-*-v316.log')]:shutil.copy2(p,A/p.name)
previous.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Candidate316: unchanged315 gameplay DLL. Read-only temporary FX owner reference-frame attachment probe checks center_spine_jnt binding, parent actor membership in the current strict native world rat pool and reciprocal rat/component/actor identities. Changing samples fail closed. No source writes, effect playback or gameplay added.')
(A/'identity.json').write_text(json.dumps(previous,indent=2))
shutil.copy2(A/'d2_runtime_probe.exe',S/'bin/d2_runtime_probe.exe')
print(json.dumps({k:previous[k] for k in ('dll','probe')}))
