from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-pose-bootstrap-v308'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists();assert read(R/'work/natural-moveset/swarm-rat-scale-owner-v307/result.json')['passed']
for kind in ('build','smoke'):
 s=(W/f'swarm-pose-bootstrap-{kind}-v308.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in(S/'assets').glob('swarm_rat*'):
 if p.is_file():shutil.copy2(p,A/'assets'/p.name)
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in[W/'Invoke-NativeLabOperation.ps1',W/'prepare_pose_bootstrap_v308.py',Path(__file__),S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v308.ps1',*W.glob('swarm-pose-bootstrap-*-v308.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-cadence-v300/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),assetHashes={p.name:sha(p)for p in(A/'assets').iterdir()},runtimeVerified=False,scope='Candidate308: before publishing a new root clock scope, collect at least8 distinct samples over250ms real/source time plus100ms near the minimum observed arrival-source offset. Pending calibration publishes no source session, preventing action/world admission. This addresses observed303 startup first-packet bias; steady-state303 mapper/freshness rules remain unchanged, and outages do not restart calibration. Durable16-scope boundary records supplement rejection ring. Cosmetic scale307 passed23 checks; assets/bank/packet schemas unchanged. Native runtime pending. Real Internet clock accuracy, performance, colors/VFX and full shared gameplay remain incomplete.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in('dll','probe','assetHashes')}))
