from pathlib import Path
import json,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-midnight-fx-probe-v442'
x=json.loads((A/'identity.json').read_text(encoding='utf-8-sig'));x['previous']='candidate-midnight-fx-probe-v438';x['scope']='Strict source clone scene/clock/key/helper reader442; external probe only. DLL427 and bank55/322 unchanged; source reader runtime verification and remote playback pending.';(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8');shutil.copy2(W/'midnight-fx-reader-tests-v442.log',A)
s=(N/'Test-MidnightFx-v440.ps1').read_text(encoding='utf-8-sig')
anchor=" $checks.Add('Native clone scene group and five FX keys resolve exact sockets and distinct helpers independently matched to native FX-map owners/declarations')"
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $strict=@($fxRows|Where-Object {$_.worldStable-and$_.cloneScene.valid-and$_.cloneScene.variant})
 $coverage=@(foreach($variant in 1,2){
  $phase=@($strict|Where-Object {$_.cloneScene.variant-eq$variant})
  if($phase.Count-lt3){throw "Strict source clone scene variant $variant lacks three observations"}
  $nativeIds=@($midnightSamples.instances|Where-Object {$_.asset-eq$(if($variant-eq1){'midnight/gameplay/human/common/doppelganger/doppelganger_spawn.midnightscene'}else{'midnight/gameplay/human/common/doppelganger/doppelganger_dispawn.midnightscene'})-and$_.state-eq3}|ForEach-Object id|Sort-Object -Unique)
  $ids=@($phase.cloneScene.id|Sort-Object -Unique)
  if(Compare-Object $ids $nativeIds){throw 'Strict clone scene IDs differ from independent Midnight observer'}
  if(@($phase.cloneHandle|Sort-Object -Unique).Count-ne1-or$phase[0].cloneHandle-notin$nativeCloneHandles){throw 'Strict source scene changed native clone ownership'}
  foreach($row in $phase){if(@($row.cloneScene.helpers|Sort-Object -Unique).Count-ne5-or@($row.cloneScene.helpers|Where-Object {$_-notin$row.effects.owner}).Count){throw 'Strict helper handle missing from same-snapshot native FX map'}}
  [pscustomobject]@{variant=$variant;ids=$ids;samples=$phase.Count;firstMs=$phase[0].cloneScene.timeMs;lastMs=$phase[-1].cloneScene.timeMs;maxReadMs=($phase.cloneScene.readMs|Measure-Object -Maximum).Maximum}
 })
 $coverage|ConvertTo-Json -Depth 10|Set-Content "$out/strict-scene-coverage.json"
 $checks.Add('Strict source reader confirms both native spawn1000ms and dispawn466ms scene incarnations, clocks and all five helpers against independent Midnight and same-snapshot FX map')
''')
target=N/'Test-MidnightFx-v443.ps1';assert not target.exists();target.write_text(s,encoding='utf-8')
s=(W/'Run-MidnightFx-v440.ps1').read_text(encoding='utf-8-sig').replace('midnight-fx-v440','midnight-fx-v443').replace('Test-MidnightFx-v440','Test-MidnightFx-v443')
(W/'Run-MidnightFx-v443.ps1').write_text(s,encoding='utf-8');print('443 strict source observer test ready')
