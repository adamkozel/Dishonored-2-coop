from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
target=N/'Test-MidnightFx-v439.ps1';assert not target.exists()
s=(N/'Test-DoppelgangerMidnight-v437.ps1').read_text(encoding='utf-8-sig')
anchor=" $checks.Add('Native1000ms clone spawn scene observed with reciprocal binding to independently enumerated source clone; raw instance retained for strict reader development')"
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $midnightSamples=@(Get-Content "$out/native-midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $fxScenes=@($midnightSamples.instances|Where-Object {$_.asset-eq'midnight/gameplay/human/common/doppelganger/doppelganger_spawn.midnightscene'-and$_.state-eq3})
 $resolved=@($fxScenes|Where-Object {$_.bindingsResearch.Count-eq1-and$_.bindingsResearch[0].reciprocal-and$_.bindingsResearch[0].fxResearch.valid-and@($_.bindingsResearch[0].fxResearch.tracks.records).Count-eq5})
 if(!$resolved.Count){throw 'No stable reciprocal native five-helper FX track snapshot; research capture retained'}
 $proof=$resolved[0];$fxRecords=@($proof.bindingsResearch[0].fxResearch.tracks.records)
 if(@($fxRecords|Where-Object {!$_.keyResolved-or$_.helperType-ne'.?AVidEntity@@'}).Count-or@($fxRecords.helperHandle|Sort-Object -Unique).Count-ne5){throw 'Native key/helper identity unresolved'}
 $expectedSockets=@('decoy_root','sl_l_arm','sl_r_arm','decoy_spawn_l_hand','decoy_spawn_r_hand')|Sort-Object
 if(Compare-Object $expectedSockets ($fxRecords.socket|Sort-Object)){throw 'Native spawn sockets differ from exported exact scene'}
 $fxRows=@(Get-Content "$out/native-effects.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|Where-Object valid)
 foreach($rec in $fxRecords){if(!@($fxRows.effects|Where-Object {$_.owner-eq$rec.helperHandle-and$_.name-eq$rec.effect}).Count){throw 'Track helper/effect absent from independent native FX map'}}
 $proof|ConvertTo-Json -Depth 16|Set-Content "$out/confirmed-fx-scene.json"
 $checks.Add('Native clone scene group and five FX keys resolve exact sockets and distinct helpers independently matched to native FX-map owners/declarations')
''')
target.write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerMidnight-v437.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-midnight-v437','midnight-fx-v439').replace('Test-DoppelgangerMidnight-v437','Test-MidnightFx-v439')
(W/'Run-MidnightFx-v439.ps1').write_text(s,encoding='utf-8')
print('439 research test ready; unchanged427 DLL, probe438')
