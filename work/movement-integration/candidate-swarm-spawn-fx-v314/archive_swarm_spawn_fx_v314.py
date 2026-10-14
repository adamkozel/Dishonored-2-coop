from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-spawn-fx-v314'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
for kind in ('build','smoke','action'):
 s=(W/f'swarm-spawn-fx-{kind}-v314.log').read_text(encoding='utf-8-sig');assert s and 'FAIL:' not in s and 'error C' not in s and 'error LNK' not in s
A.mkdir();shutil.copytree(S/'src',A/'src');(A/'assets').mkdir()
for p in (S/'assets').glob('swarm_*'):
 if p.is_file():shutil.copy2(p,A/'assets'/p.name)
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in [Path(__file__),W/'prepare_swarm_spawn_fx_v314.py',W/'Invoke-NativeLabOperation.ps1',S/'Build-D2SwarmRatVisualAssets.ps1',S/'Build-D2SwarmSpawnFxAssets.ps1',R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v314.ps1',*W.glob('swarm-spawn-fx-*-v314.log')]:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-fx-observe-v312/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Candidate314: source-owned native Swarm summon burst over a separate176-byte Steam packet. Bounded discovery requires a newly observed source world, reciprocal equipment/world owner, exact declaration+A0, native cast25, finite early window. Stream baseline prevents late-join replay. Receiver requires matching world event, power event, tokens and clock, plays only isolated native-derived particle/sound FX without rumble, retires after1600ms or world/stream change. No native gameplay invocation. Trails, marker, attack/death FX and audio perception remain unverified.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe')}))

