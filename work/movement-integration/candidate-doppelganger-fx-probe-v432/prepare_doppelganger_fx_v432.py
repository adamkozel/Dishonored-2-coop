from pathlib import Path
import json,hashlib,datetime,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab';A=W/'candidate-doppelganger-fx-probe-v432'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
x=rd(W/'candidate-doppelganger-visual-v427/identity.json');assert x['dll']==sha(R/'work/build/Release/dinput8.dll')==sha(R/'work/asset-game/dinput8.dll');assert not A.exists()
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(W/'candidate-doppelganger-visual-v427/banks',A/'banks');shutil.copytree(W/'candidate-doppelganger-visual-v427/assets',A/'assets')
logs=[W/f'doppelganger-visual-{k}-v432.log' for k in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
    t=p.read_text(encoding='utf-8-sig');assert t and not any(s in t for s in ('error C','error LNK','FAIL:','Traceback'));shutil.copy2(p,A/p.name)
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
x.update(created=datetime.datetime.now().astimezone().isoformat(),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Read-only432 FX watcher; DLL427 unchanged. Native FX map plus clone identity double snapshots; candidate owner/parent matches only, no effect playback or inferred attachment.',previousVerified=str(W/'candidate-doppelganger-visual-v427'),banks=[dict(path=str(p),sha256=sha(p)) for p in (A/'banks').glob('*.bin')]);x.pop('runtimeVerificationScope',None)
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8');shutil.copy2(A/'d2_runtime_probe.exe',L/'bin/d2_runtime_probe.exe');shutil.copy2(Path(__file__),A/Path(__file__).name)
s=(N/'Test-DoppelgangerVisual-v430.ps1').read_text(encoding='utf-8-sig')
anchor=' Start-Sleep -Milliseconds 100;if($monitor.HasExited)';assert s.count(anchor)==1
s=s.replace(anchor,''' $fxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-doppelganger-effects','18000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-effects.ndjson" -RedirectStandardError "$out/native-effects.stderr" -PassThru
 $recorders+=$fxMonitor
'''+anchor)
(N/'Test-DoppelgangerFx-v433.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerVisual-v430.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-visual-v430','doppelganger-fx-v433').replace('Test-DoppelgangerVisual-v430','Test-DoppelgangerFx-v433')
(W/'Run-DoppelgangerFx-v433.ps1').write_text(s,encoding='utf-8')
print(json.dumps(dict(dll=x['dll'],probe=x['probe'],scope=x['scope'])))
