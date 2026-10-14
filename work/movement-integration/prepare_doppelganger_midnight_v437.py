from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
target=N/'Test-DoppelgangerMidnight-v437.ps1';assert not target.exists()
s=(N/'Test-DoppelgangerFx-v435.ps1').read_text(encoding='utf-8-sig')
s=s.replace(' $recorders+=$fxMonitor',''' $recorders+=$fxMonitor
 $midnightMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','18000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-midnight.ndjson" -RedirectStandardError "$out/native-midnight.stderr" -PassThru
 $recorders+=$midnightMonitor''')
s=s.replace('if($monitor.HasExited-or$fxMonitor.HasExited)','if($monitor.HasExited-or$fxMonitor.HasExited-or$midnightMonitor.HasExited)')
s=s.replace('$cloneCaptured=$false','$cloneCaptured=$false;$cloneSceneCaptured=$false;$sceneNext=2900')
anchor='  if($w.ElapsedMilliseconds-ge$worldNext){';assert s.count(anchor)==1
s=s.replace(anchor,'''  if($cast-and!$cloneSceneCaptured-and$w.ElapsedMilliseconds-ge$sceneNext-and$w.ElapsedMilliseconds-lt6500){
   $sceneNext=$w.ElapsedMilliseconds+80
   $sceneSnapshot=Q client '--midnight'
   $candidates=@($sceneSnapshot.instances|Where-Object {$_.asset-eq'midnight/gameplay/human/common/doppelganger/doppelganger_spawn.midnightscene'-and$_.state-eq3})
   if($candidates.Count-eq1){
    $cloneScene=$candidates[0];$cloneScene|ConvertTo-Json -Depth 10|Set-Content "$out/clone-spawn-scene.json"
    $raw=Q client '--object' $cloneScene.pointer;$raw|ConvertTo-Json -Depth 10|Set-Content "$out/clone-spawn-instance-object.json"
    $cloneSceneCaptured=$true
   }
  }
'''+anchor)
anchor=" $checks.Add('Native source clone observed during its lifetime; body playback creates no peer clones or duplicate power actions')";assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 if(!$cloneSceneCaptured-or$cloneScene.bindingsResearch.Count-ne1-or!$cloneScene.bindingsResearch[0].reciprocal-or$cloneScene.sceneDurationMs-ne1000){throw 'Native clone spawn scene binding/clock observation missing'}
 $nativeCloneHandles=@($worldSamples.source.handle|ForEach-Object {[Convert]::ToUInt32($_,16)}|Sort-Object -Unique)
 if([uint32]$cloneScene.bindingsResearch[0].handle-notin$nativeCloneHandles){throw 'Spawn scene actor differs from independently enumerated clone'}
 $checks.Add('Native1000ms clone spawn scene observed with reciprocal binding to independently enumerated source clone; raw instance retained for strict reader development')
''')
target.write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerFx-v435.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-fx-v435','doppelganger-midnight-v437').replace('Test-DoppelgangerFx-v435','Test-DoppelgangerMidnight-v437')
(W/'Run-DoppelgangerMidnight-v437.ps1').write_text(s,encoding='utf-8')
print('437: unchanged verified DLL/probe; native Midnight and FX observers, isolated cast/recordings/recovery')
