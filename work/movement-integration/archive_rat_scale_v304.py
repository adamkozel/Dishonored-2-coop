from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-rat-scale-v304'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists();assert read(R/'work/natural-moveset/swarm-rat-clock-v303/result.json')['passed']
for kind in ('build','smoke'):
 s=(W/f'swarm-rat-scale-{kind}-v304.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in(S/'assets').glob('swarm_rat*'):
 if p.is_file():shutil.copy2(p,A/'assets'/p.name)
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in[W/'Invoke-NativeLabOperation.ps1',W/'prepare_rat_scale_v304.py',Path(__file__),S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v304.ps1',*W.glob('swarm-rat-scale-*-v304.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-cadence-v300/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),assetHashes={p.name:sha(p)for p in(A/'assets').iterdir()},runtimeVerified=False,scope='Candidate304: native rat size replication through exact guarded753630 self-driver scale setter. Validate generic frame vt, self-driver vt1c574c0, reciprocal ownership, no attachment parent and same animation/render. Native queued update must settle within500ms; no direct entity/render scale writes. Read back frame/entity/render three axes. Walking stride uses1.11m/s multiplied by each rat size. Previous303 passed22checks with96.24percent bracketing and movement clock fix.120ms cosmetic buffer retained. Color, native attack/state phase, effects/audio, upgraded100rats and shared gameplay outcomes incomplete. Startup first-packet clock bias remains a separate observed issue.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in('dll','probe','assetHashes')}))
