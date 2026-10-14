from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Test-MidnightFx-v446.ps1').read_text(encoding='utf-8-sig')
anchor=' $recorders+=$midnightMonitor';assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $peerFxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-doppelganger-effects','30000') -WindowStyle Hidden -RedirectStandardOutput "$out/peer-effects.ndjson" -RedirectStandardError "$out/peer-effects.stderr" -PassThru
 $peerMidnightMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-midnight','30000') -WindowStyle Hidden -RedirectStandardOutput "$out/peer-midnight.ndjson" -RedirectStandardError "$out/peer-midnight.stderr" -PassThru
 $recorders+=$peerFxMonitor;$recorders+=$peerMidnightMonitor
''')
s=s.replace('if($monitor.HasExited-or$fxMonitor.HasExited-or$midnightMonitor.HasExited)','if($monitor.HasExited-or$fxMonitor.HasExited-or$midnightMonitor.HasExited-or$peerFxMonitor.HasExited-or$peerMidnightMonitor.HasExited)')
anchor=" $checks.Add('One isolated cosmetic clone follows native219-joint pose and interpolated root, then hides on source retirement')";assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $peerMidnight=@(Get-Content "$out/peer-midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $peerFxRows=@(Get-Content "$out/peer-effects.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|Where-Object valid)
 $fxHistory=@($lastVisual.fx.history|Where-Object {$_.event-eq$live[-1].event})
 if($fxHistory.Count-ne2-or@($fxHistory|Where-Object {!$_.verified}).Count-or$lastVisual.fx.scene-or$lastVisual.fx.refused-$visualStart.fx.refused){throw "Remote effect start/cleanup refused: $($lastVisual.fx|ConvertTo-Json -Depth 6 -Compress)"}
 $fxEvidence=@(foreach($v in 1,2){
  $h=@($fxHistory|Where-Object variant -eq $v);if($h.Count-ne1-or[Math]::Abs([int]$h[0].elapsed-[int]$h[0].nativeTime)-gt35){throw 'Native remote effect clock mismatch or duplicate start'}
  $sourceIds=@($strict|Where-Object {$_.cloneScene.variant-eq$v}|ForEach-Object {$_.cloneScene.id}|Sort-Object -Unique)
  if($h[0].sourceScene-notin$sourceIds){throw 'Remote FX scene has no strict native source incarnation'}
  $resource=if($v-eq1){'midnight/d2coop/doppelganger_fx_spawn.midnightscene'}else{'midnight/d2coop/doppelganger_fx_despawn.midnightscene'}
  $shown=@($peerMidnight.instances|Where-Object {$_.id-eq$h[0].nativeScene-and$_.asset-eq$resource-and$_.state-eq3-and$_.bindingsResearch.Count-eq1-and$_.bindingsResearch[0].reciprocal-and$_.bindingsResearch[0].handle-eq$lastVisual.handle-and$_.bindingsResearch[0].fxResearch.valid})
  if(!$shown.Count){throw 'Remote cosmetic effect scene lacks independent native binding'}
  $trace=$shown[0].bindingsResearch[0].fxResearch
  $types=@($trace.tracks.type|Sort-Object);$wanted=@('.?AVarkMidnightTrackInstFx@@','.?AVarkMidnightTrackInstMaterialRenderParm@@')|Sort-Object
  if(Compare-Object $types $wanted){throw 'Remote scene has a non-cosmetic track'}
  $records=@($trace.tracks.records|Where-Object {$null-ne$_});if($records.Count-ne5-or@($records|Where-Object {!$_.keyResolved}).Count){throw 'Remote effect helpers unresolved'}
  foreach($rec in $records){if(!@($peerFxRows.effects|Where-Object {$_.owner-eq$rec.helperHandle-and$_.name-eq$rec.effect}).Count){throw 'Remote effect helper absent from independent FX map'}}
  [pscustomobject]@{variant=$v;sourceScene=$h[0].sourceScene;nativeScene=$h[0].nativeScene;startElapsed=$h[0].elapsed;nativeElapsed=$h[0].nativeTime;observations=$shown.Count;trace=$trace}
 })
 $fxEvidence|ConvertTo-Json -Depth 18|Set-Content "$out/remote-effects-verification.json"
 $checks.Add('One native cosmetic spawn and disappearance scene follow exact source scene IDs/clocks, with five independently verified FX helpers each and only FX/material tracks')
''')
s=s.replace('peer animation does not execute clone, VFX or damage events.','peer animation does not execute gameplay clone or damage events; separate native FX/material-only scenes reproduce source-owned summon/disappearance effects.')
s=s.replace('remote VFX/gameplay, alternate contexts and visual acceptance remain pending.','shared clone gameplay, alternate contexts and visual acceptance remain pending.')
p=N/'Test-DoppelgangerFx-v450.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
s=(W/'Run-MidnightFx-v446.ps1').read_text(encoding='utf-8-sig').replace('midnight-fx-v446','doppelganger-fx-v450').replace('Test-MidnightFx-v446','Test-DoppelgangerFx-v450')
(W/'Run-DoppelgangerFx-v450.ps1').write_text(s,encoding='utf-8');print('450 native source/peer effects comparison prepared')
