from pathlib import Path
import datetime,hashlib,json,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab';A=W/'candidate-possession-crouch-v363'
assert not A.exists()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
logs=[W/f'possession-crouch-{s}-v363.log' for s in ('build','world-tests','action-tests','bank-tests','smoke','author')]
for p in logs:
 s=p.read_text(encoding='utf-8-sig');assert s and not any(x in s for x in ('error C','error LNK','FAIL:','FAIL ')),p
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(P/'possession-crouch-banks-v363',A/'banks')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in [Path(__file__),*logs,W/'Build-PossessionCrouch-v363.ps1',W/'prepare_possession_crouch_test_v363.py',W/'Invoke-NativeLabOperation.ps1',P/'build_possession_crouch_v363.py',P/'Test-PossessionCrouchNetwork-v363.ps1',P/'possession-crouch-bank-manifest-v363.json',P/'possession-crouch-source-manifest-v363.json']:
 shutil.copy2(p,A/p.name)
x=json.loads((W/'candidate-possession-fx-probe-v355/identity.json').read_text())
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),bankVersion=53,clips=318,banks=[dict(path=str(p),sha256=sha(p))for p in sorted((A/'banks').iterdir())],banksReference='banks',runtimeVerified=False,scope='Crouched Possession entry/aim from native Sneaking6+UsePower63 observations. Full-body authored stance bank preserves original316 clip payloads. Synthetic ownership, protocol, actualbank and proxy tests pass; runtime/visual validation pending. Native draw overlap and shared NPC gameplay remain unfinished.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):
 shutil.copy2(A/name,L/'bin'/name);assert sha(L/'bin'/name)==sha(A/name)
print(json.dumps(dict(archive=str(A),dll=x['dll'],probe=x['probe'],banks=x['banks'])))
