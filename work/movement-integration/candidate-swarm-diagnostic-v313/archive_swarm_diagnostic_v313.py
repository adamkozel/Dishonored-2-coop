from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-diagnostic-v313'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
for kind in ('build','smoke','pose','action'):
 s=(W/f'swarm-diagnostic-{kind}-v313.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:' not in s and 'error C' not in s and 'error LNK' not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in (S/'assets').glob('swarm_rat*'):
 if p.is_file():shutil.copy2(p,A/'assets'/p.name)
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in [Path(__file__),W/'prepare_swarm_diagnostic_v313.py',W/'Invoke-NativeLabOperation.ps1',S/'Build-D2SwarmRatVisualAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v313.ps1',*W.glob('swarm-diagnostic-*-v313.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-fx-observe-v312/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Candidate313: preserves309 gameplay behavior, adds bounded16-record movement eligibility diagnostics without changing freshness limits or action permissions. Read-only Swarm FX watcher polls100ms and records validated equipment/current swarm handle. Camera20 degrees improves combined avatar/floor visibility. v312 observation captured13 FX IDs but failed body-weight continuity; no passing movement claim. No native FX replication yet.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe')}))
