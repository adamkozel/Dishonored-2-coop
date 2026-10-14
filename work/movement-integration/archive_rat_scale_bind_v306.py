from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-rat-scale-bind-v306'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists();assert read(R/'work/natural-moveset/swarm-rat-clock-v303/result.json')['passed']
for kind in ('build','smoke'):
 s=(W/f'swarm-rat-scale-bind-{kind}-v306.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in(S/'assets').glob('swarm_rat*'):
 if p.is_file():shutil.copy2(p,A/'assets'/p.name)
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in[W/'Invoke-NativeLabOperation.ps1',W/'prepare_rat_scale_bind_v306.py',Path(__file__),S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v306.ps1',*W.glob('swarm-rat-scale-bind-*-v306.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-cadence-v300/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),assetHashes={p.name:sha(p)for p in(A/'assets').iterdir()},runtimeVerified=False,scope='Candidate306:305 read-only evidence confirms exact self-driver vt but null driver+8 backlink; initialize this already-current self-driver using native751050(frame,frame+20), verify reciprocal links, then753630 native scale with500ms frame/entity/render settlement. Never bind foreign driver/frame.304 failed before scale invocation.305 unit-scale playback/clock retained; test failed arbitrary20 full25 snapshots while source naturally became24, yielding18 full25 and80 full24 matching rows.306 matches every currently indexed rat, retaining exact25 models created/cached, unique identities, native transform/gait/retirement checks. Original Steam/source gameplay unaffected. Startup clock bias and remaining effects/state/color/gameplay remain incomplete.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in('dll','probe','assetHashes')}))
