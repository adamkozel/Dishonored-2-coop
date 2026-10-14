from pathlib import Path
p=Path(__file__).parent
s=(p/'Test-DominoWorld-v205.ps1').read_text(encoding='utf-8-sig')
old=" if($stopped.marks.Count-or$stopped.stopped-ne($worldBaseline.stopped+1)-or@((Q host '--domino-effects').effects).Count){throw 'Receiver-owned mark did not retire with presentation'}"
new=""" $stopFx=Q host '--domino-effects';$stopFx|ConvertTo-Json -Depth 8|Set-Content \"$out/mark-stopped-effects-700ms.json\"
 $stopSamples=[Collections.Generic.List[object]]::new()
 $stopSamples.Add([pscustomobject]@{ms=700;effects=$stopFx;status=$stopped})
 foreach($delay in 500,1000,2000){Start-Sleep -Milliseconds $delay;$stopSamples.Add([pscustomobject]@{ms=700+(@($stopSamples|Select-Object -Skip 1|ForEach-Object delay)|Measure-Object -Sum).Sum+$delay;delay=$delay;effects=(Q host '--domino-effects');status=(S host '.domino-world.json')})}
 $stopSamples|ConvertTo-Json -Depth 12|Set-Content \"$out/mark-stop-timeline.json\"
 if($stopped.marks.Count-or$stopped.stopped-ne($worldBaseline.stopped+1)-or@($stopFx.effects).Count){throw 'Receiver-owned mark did not retire with presentation at 700ms; diagnostic timeline saved'}"""
assert old in s
(p/'Test-DominoWorld-v208.ps1').write_text(s.replace(old,new),encoding='utf-8')
r=p.parent/'movement-integration/Invoke-NativeLabOperation.ps1'
s=r.read_text(encoding='utf-8-sig');assert 'Test-DominoWorld-v205.ps1' in s
r.write_text(s.replace('Test-DominoWorld-v205.ps1','Test-DominoWorld-v208.ps1'),encoding='utf-8')
