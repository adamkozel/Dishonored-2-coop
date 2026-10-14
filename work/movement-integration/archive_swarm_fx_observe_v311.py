from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-fx-observe-v311'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists();assert read(R/'work/natural-moveset/swarm-rat-profile-v310/result.json')['passed']
for kind in ('build','smoke'):
 s=(W/f'swarm-fx-observe-{kind}-v311.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in(S/'assets').glob('swarm_rat*'):
 if p.is_file():shutil.copy2(p,A/'assets'/p.name)
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in[W/'Invoke-NativeLabOperation.ps1',W/'prepare_swarm_fx_observe_v311.py',Path(__file__),S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v311.ps1',*W.glob('swarm-fx-observe-*-v311.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-cadence-v300/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),assetHashes={p.name:sha(p)for p in(A/'assets').iterdir()},runtimeVerified=False,scope='Candidate311: unchanged309 gameplay DLL; new read-only bounded15-second Swarm FX watcher in runtime probe, using existing stable FX-map reader and exact rat_swarm path prefix. Captures observed record IDs, native owner definitions/parents and transforms. Inventory scan is research only and cannot authorize playback. Read-only exact native FX assets exported with provenance. No new peer FX or gameplay mutation. Native source ownership and lifecycle interpretation pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in('dll','probe','assetHashes')}))
