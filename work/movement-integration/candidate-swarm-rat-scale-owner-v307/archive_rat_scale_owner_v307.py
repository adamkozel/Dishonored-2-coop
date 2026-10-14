from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-rat-scale-owner-v307'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists();assert read(R/'work/natural-moveset/swarm-rat-clock-v303/result.json')['passed']
for kind in ('build','smoke'):
 s=(W/f'swarm-rat-scale-owner-{kind}-v307.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:'not in s and 'error C'not in s and 'error LNK'not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in(S/'assets').glob('swarm_rat*'):
 if p.is_file():shutil.copy2(p,A/'assets'/p.name)
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in[W/'Invoke-NativeLabOperation.ps1',W/'prepare_rat_scale_owner_v307.py',Path(__file__),S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v307.ps1',*W.glob('swarm-rat-scale-owner-*-v307.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-rat-cadence-v300/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),assetHashes={p.name:sha(p)for p in(A/'assets').iterdir()},runtimeVerified=False,scope='Candidate307: native753c10 self-driver copies authoritative entity330 scale into frame60;306 binding succeeded but scale reverted, so753630 is insufficient for generic self-driven models. Guard exact owned cosmetic identity/frame/render/self-driver and finite old scale, update only entity330 XYZ then native87c5d0 render flush. Native frame update settles frame60 with500ms bound; no native RatAgent/AI/gameplay. Tests require all source rat identities, XYZ scale at three native levels, gait and source-stop/checkpoint restoration. Native original files and local first-person unchanged; color/state/VFX/audio and gameplay authority remain incomplete.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in('dll','probe','assetHashes')}))
