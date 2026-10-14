from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-doppelganger-world-probe-v414'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
base=W/'candidate-doppelganger-binding-v407';x=json.loads((base/'identity.json').read_text(encoding='utf-8-sig'));assert x['runtimeVerified'] and sha(R/'work/asset-game/dinput8.dll')==x['dll']
for name in ('doppelganger-world-build-v414.log','doppelganger-world-tests-v414.log'):
 t=(W/name).read_text(encoding='utf-8-sig');assert t and 'FAIL:' not in t and 'error C' not in t
if not A.exists():
 A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(base/'banks',A/'banks');shutil.copy2(base/'dinput8.dll',A/'dinput8.dll');shutil.copy2(R/'work/build/Release/d2_runtime_probe.exe',A/'d2_runtime_probe.exe')
 x.update(created=datetime.datetime.now().astimezone().isoformat(),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Probe-only414 adds read-only native equipment-array to exact summoned clone ownership and lifetime observation. Running game DLL remains verified407, protocol52/bank55. No native game mutation or new remote clone/FX logic.',previousVerified=str(base),banks=[dict(path=str(p),sha256=sha(p)) for p in (A/'banks').glob('*.bin')]);x.pop('runtimeVerificationScope',None)
 (A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
 for p in [W/'Build-DoppelgangerWorldProbe-v414.ps1',W/'implement_doppelganger_world_probe_v414.py',W/'doppelganger-world-build-v414.log',W/'doppelganger-world-tests-v414.log']:shutil.copy2(p,A/p.name)
 shutil.copy2(A/'d2_runtime_probe.exe',L/'bin/d2_runtime_probe.exe')
else:
 x=json.loads((A/'identity.json').read_text(encoding='utf-8-sig'))
 assert x['dll']==sha(R/'work/asset-game/dinput8.dll') and x['probe']==sha(A/'d2_runtime_probe.exe')==sha(L/'bin/d2_runtime_probe.exe')==sha(R/'work/build/Release/d2_runtime_probe.exe')
s=(N/'Observe-NativeDoppelganger-v406.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$capture=$null;$monitor=$null;', '$capture=$null;$monitor=$null;$worldMonitor=$null;$cloneCaptured=$false;')
anchor=' $w.Restart();$cast=$false'
assert s.count(anchor)==1
s=s.replace(anchor,''' $worldMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-doppelganger-world','30000') -WindowStyle Hidden -RedirectStandardOutput "$out/world.ndjson" -RedirectStandardError "$out/world.stderr" -PassThru
'''+anchor)
anchor="  $entities=Q client '--entities'\n"
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
  $clones=@($entities.entities|Where-Object definitionName -eq 'models/powers/doppelganger/decoy_entity.def')
  if($clones.Count-eq1-and!$cloneCaptured){
   $cc=Q client '--components' $clones[0].handle;$cc|ConvertTo-Json -Depth 10|Set-Content "$out/clone-components.json"
   $dc=@($cc.components|Where-Object type -eq '.?AVarkCpntDecoy@@');if($dc.Count-ne1){throw 'Native clone lacks unique decoy component'}
   $obj=Q client '--object' $dc[0].pointer;$obj|ConvertTo-Json -Depth 12|Set-Content "$out/decoy-component.json"
   $dp=($obj.words|Where-Object offset -eq '0x8').word;Q client '--object' $dp|ConvertTo-Json -Depth 12|Set-Content "$out/decoy-declaration.json"
   Q client '--object' $clones[0].pointer|ConvertTo-Json -Depth 12|Set-Content "$out/clone-object.json"
   $cloneCaptured=$true
  }
''')
anchor=" $checks.Add('Both original inventory quantities and player/partner health retained; world reset still required')"
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $worldMonitor.WaitForExit(20000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){throw 'World observer did not finish'}
 $world=@(Get-Content "$out/world.ndjson"|ForEach-Object {$_|ConvertFrom-Json});$live=@($world|Where-Object {$_.valid-and$_.present})
 if($live.Count-lt20){throw 'Exact equipment/clone ownership not observed in twenty samples'}
 if(@($live.handle|Sort-Object -Unique).Count-ne1-or@($live.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Native clone/equipment incarnation changed unexpectedly'}
 $emptyAfter=@($world|Where-Object {$_.valid-and!$_.present-and$_.tick-gt$live[-1].tick})
 if(!$emptyAfter.Count){throw 'Native clone absence after fixture retirement not observed'}
 [ordered]@{presentSamples=$live.Count;afterAbsent=$emptyAfter.Count;states=@($live.stateCandidate|Sort-Object -Unique);handles=@($live.handle|Sort-Object -Unique);refusalStages=@($world|Where-Object {!$_.valid}|Group-Object stage|Select-Object Name,Count);maxReadMs=($world.readMs|Measure-Object -Maximum).Maximum;scope='Read-only reciprocal equipment/array/entity/component ownership. Raw decoy state has no inferred meaning; expiry versus fixture retirement is not conflated.'}|ConvertTo-Json -Depth 8|Set-Content "$out/clone-ownership-observations.json"
 $checks.Add('Exact equipment array, cloned NPC and owned native Decoy component agree in repeated live samples, then report absence after fixture cleanup')
''')
s=s.replace('@($capture,$monitor)', '@($capture,$monitor,$worldMonitor)')
s=s.replace("no remote movement/VFX/gameplay verified", "verified407 body playback unchanged; remote clone/VFX/gameplay remain unimplemented")
(N/'Observe-DoppelgangerWorld-v415.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerObserve-v406.ps1').read_text(encoding='utf-8-sig').replace('native-doppelganger-source-v406','doppelganger-world-ownership-v415').replace('DoppelgangerObserve','DoppelgangerWorldObserve').replace('Observe-NativeDoppelganger-v406.ps1','Observe-DoppelgangerWorld-v415.ps1')
(W/'Run-DoppelgangerWorld-v415.ps1').write_text(s,encoding='utf-8')
print(json.dumps(dict(probe=x['probe'],dll=x['dll'],archive=str(A),gameRestartRequired=False)))
