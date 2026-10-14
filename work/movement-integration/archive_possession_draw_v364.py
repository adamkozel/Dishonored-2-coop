from pathlib import Path
import datetime,hashlib,json,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab';A=W/'candidate-possession-draw-v364'
assert not A.exists();sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
logs=[W/f'possession-draw-{s}-v364.log' for s in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
 s=p.read_text(encoding='utf-8-sig');assert s and not any(x in s for x in ('error C','error LNK','FAIL:','FAIL ')),p
A.mkdir();shutil.copytree(L/'src',A/'src')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in [Path(__file__),*logs,W/'Build-PossessionDraw-v364.ps1',W/'prepare_possession_draw_test_v364.py',W/'Invoke-NativeLabOperation.ps1',P/'Test-PossessionDrawNetwork-v364.ps1']:shutil.copy2(p,A/p.name)
x=json.loads((W/'candidate-possession-crouch-v363/identity.json').read_text())
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),protocol=51,protocolBytes=336,banksReference='../candidate-possession-crouch-v363/banks',runtimeVerified=False,scope='Exact Corvo native right-arm sword draw plus left-arm Possession cast ownership with independent scene clocks. Protocol51/336 admits only observed draw variants1/3 with1066/1133ms while Possession26/27. Full right-arm subtree retains draw/contact, full-body cast remains authored. Bank53/318 unchanged. Synthetic dual ownership/races, protocol/bank and proxy tests pass. Runtime and visual validation pending; shared NPC gameplay unfinished.')
(A/'identity.json').write_text(json.dumps(x,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):
 shutil.copy2(A/name,L/'bin'/name);assert sha(L/'bin'/name)==sha(A/name)
print(json.dumps(dict(archive=str(A),dll=x['dll'],probe=x['probe'])))
