from pathlib import Path
p=Path(__file__).parent
s=(p/'Test-DominoWorld-v205.ps1').read_text(encoding='utf-8-sig')
s=s.replace("'coop_domino_fixture 20000'","'coop_domino_fixture 40000'").replace("'-t','16'","'-t','23'")
old=""" C host 'coop_movement_bank off';Start-Sleep -Milliseconds 700
 $stopped=S host '.domino-world.json';$stopped|ConvertTo-Json -Depth 8|Set-Content \"$out/mark-stopped.json\"
 if($stopped.marks.Count-or$stopped.stopped-ne($worldBaseline.stopped+1)-or@((Q host '--domino-effects').effects).Count){throw 'Receiver-owned mark did not retire with presentation'}
 $checks.Add('Disabling receiver body presentation retires its owned mark; native source mark remains source-owned until checkpoint cleanup')"""
new=""" $stopWatch=[Diagnostics.Stopwatch]::StartNew()
 C host 'coop_movement_bank off';$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Receiver presentation disabled; native particle fade begins'})
 Start-Sleep -Milliseconds 700
 $stopped=S host '.domino-world.json';$stopped|ConvertTo-Json -Depth 8|Set-Content \"$out/mark-stopped.json\"
 if($stopped.marks.Count-or$stopped.stopped-ne($worldBaseline.stopped+1)){throw 'Receiver mark slot not stopped within 700ms'}
 $fade=[Collections.Generic.List[object]]::new();$fadeFailure=$null;$stopOrigin=$null
 do{
  $afterStopFx=Q host '--domino-effects';$afterStopWorld=S host '.domino-world.json'
  $fade.Add([pscustomobject]@{ms=$stopWatch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;effects=$afterStopFx;status=$afterStopWorld})
  if($afterStopWorld.marks.Count-or$afterStopWorld.started-ne$peerWorld.started-or$afterStopWorld.updated-ne$stopped.updated-or$afterStopWorld.stopped-ne$stopped.stopped){$fadeFailure='Stopped mark resumed or ownership changed';break}
  if(@($afterStopFx.effects).Count){
   if(@($afterStopFx.effects).Count-ne1-or$afterStopFx.effects[0].id-ne$peerMark.id-or$afterStopFx.effects[0].owner-ne$peerMark.owner){$fadeFailure='Unexpected FX appeared during retirement';break}
   if($null-eq$stopOrigin){$stopOrigin=@($afterStopFx.effects[0].origin)}elseif(Compare-Object $stopOrigin @($afterStopFx.effects[0].origin)){$fadeFailure='Stopped native mark transform continued moving';break}
  }else{break}
  Start-Sleep -Milliseconds 150
 }while($stopWatch.ElapsedMilliseconds-lt5000)
 $fade|ConvertTo-Json -Depth 12|Set-Content \"$out/mark-stop-timeline.json\"
 if($fadeFailure){throw $fadeFailure};if(@($afterStopFx.effects).Count){throw 'Native mark particle record survives bounded five-second fade'}
 $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native owned mark FX record absent after bounded fade'})
 $checks.Add('Receiver presentation stop retires mark ownership within 700ms with no new effects or transform updates; native particle record disappears within five seconds, observed separately from stream shutdown')"""
assert old in s
(p/'Test-DominoWorld-v209.ps1').write_text(s.replace(old,new),encoding='utf-8')
r=p.parent/'movement-integration/Invoke-NativeLabOperation.ps1'
s=r.read_text(encoding='utf-8-sig');assert 'Test-DominoWorld-v208.ps1' in s
r.write_text(s.replace('Test-DominoWorld-v208.ps1','Test-DominoWorld-v209.ps1'),encoding='utf-8')
