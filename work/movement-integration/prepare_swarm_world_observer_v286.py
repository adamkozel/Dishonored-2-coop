from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
t=(P/'Observe-NativeSwarmWorld-v281.ps1').read_text()
t=t.replace('$holsterMonitor=$null;','$holsterMonitor=$null;$worldMonitor=$null;')
old=' $w.Restart();$cast=$false;$worldSnapshots=0'
assert old in t
t=t.replace(old,''' $worldMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-swarm-world','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/swarm-world.ndjson" -RedirectStandardError "$out/swarm-world.stderr" -PassThru
 Start-Sleep -Milliseconds 100;if($worldMonitor.HasExited){throw 'World watcher exited before cast'}
'''+old)
old="     $object=Q host '--object' $rat[0].pointer"
assert old in t
t=t.replace(old,old+'''
     $worldObject=Q host '--object' $swarm[0].pointer
     $declPointer=@($object.words|Where-Object offset -eq '0x8')[0].word
     $declObject=Q host '--object' $declPointer
     [ordered]@{entity=$worldObject;component=$object;declaration=$declObject}|ConvertTo-Json -Depth 12|Set-Content "$out/world-declaration-$worldSnapshots.json"
''')
old=" $scenes|ConvertTo-Json -Depth 16|Set-Content \"$out/swarm-scenes.json\""
assert old in t
t=t.replace(old,old+'''
 $worldMonitor.WaitForExit(4000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){throw 'World watcher failed'}
 $worldRows=@(Get-Content "$out/swarm-world.ndjson"|ForEach-Object{$_|ConvertFrom-Json})
 $liveWorld=@($worldRows|Where-Object{$_.valid-and$_.present-and$_.agents.Count-eq25})
 [ordered]@{samples=$worldRows.Count;validSamples=@($worldRows|Where-Object valid).Count;fullSwarmSamples=$liveWorld.Count;stages=@($worldRows|Group-Object stage|ForEach-Object{@{stage=$_.Name;count=$_.Count}});maximumReadMs=($worldRows.readMs|Measure-Object -Maximum).Maximum}|ConvertTo-Json -Depth 6|Set-Content "$out/world-reader-summary.json"
 if($liveWorld.Count-lt20){throw 'Fewer than20 strict full native Swarm snapshots'}
 foreach($sample in $liveWorld){if(@($sample.agents.handle|Sort-Object -Unique).Count-ne25-or@($sample.agents.index|Sort-Object -Unique).Count-ne25){throw 'Duplicate strict rat identity'}}
 $checks.Add('At least20 strict snapshots validate all25 indexed native rats, reciprocal entities/frames/animations and stable generations')
''')
t=t.replace('@($capture,$monitor,$holsterMonitor)','@($capture,$monitor,$holsterMonitor,$worldMonitor)')
old="  $checks.Add('Both isolated checkpoints restore original health/inventory and retire spawned Swarm entities')"
assert old in t
t=t.replace(old,'''
  $empty=Q host '--swarm-world';$empty|ConvertTo-Json -Depth 8|Set-Content "$out/world-after-checkpoint.json"
  if(!$empty.valid-or$empty.present){throw 'Strict Swarm world reader retains a world after checkpoint'}
'''+old)
(P/'Observe-NativeSwarmWorld-v286.ps1').write_text(t)
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';t=p.read_text();assert 'Observe-NativeSwarmWorld-v281.ps1' in t;p.write_text(t.replace('Observe-NativeSwarmWorld-v281.ps1','Observe-NativeSwarmWorld-v286.ps1'))
print('Prepared v286 strict native-world observation; runner stays serialized.')
