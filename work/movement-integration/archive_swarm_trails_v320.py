from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-trails-v320'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
for kind in ('build','smoke','action'):
 s=(W/f'swarm-trails-{kind}-v320.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:' not in s and 'error C' not in s and 'error LNK' not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in (S/'assets').glob('swarm_*'):
 if p.is_file():shutil.copy2(p,A/'assets'/p.name)
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in [Path(__file__),W/'prepare_source_reader_v320.py',W/'Invoke-NativeLabOperation.ps1',S/'Build-D2SwarmRatVisualAssets.ps1',S/'Build-D2SwarmSpawnFxAssets.ps1',S/'Send-D2Command.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v320.ps1',*W.glob('swarm-trails-*-v320.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-fx-observe-v312/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),assetHashes={p.name:sha(p)for p in(A/'assets').iterdir()},runtimeVerified=False,scope='Candidate320: reduce idle grenade inventory reads without relaxing scene, ownership, freshness or native event checks. Full reader remains for active scenes including unequip and empty recovery; selected-only reader double-validates slot1 and unique equipment membership for idle held presentation. Per-reader timing and root-buffer diagnostic evidence added. Trails319 completed7/7 without refusal;319 failed semantic starvation before cast. Runtime and grenade regressions pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe')}))

