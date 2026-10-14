from pathlib import Path
p=Path('work/natural-moveset/Test-MesmerizeWorldNetwork-v79.ps1');s=p.read_text().replace("'-t','28'","'-t','34'").replace('$w.ElapsedMilliseconds-lt26000','$w.ElapsedMilliseconds-lt32000')
s=s.replace(" C client 'setviewpos -33.5", " $visualBefore=S host '.mesmerize-visual.json';$worldBefore=S host '.mesmerize-world-stream.json'\n $visualBefore|ConvertTo-Json|Set-Content \"$out/visual-before.json\"\n C client 'setviewpos -33.5")
s=s.replace('$visual.refused-or$visual.created-ne1-or$visual.started[0]-ne1-or$visual.started[1]-ne1-or$visual.started[2]-ne1', '$visual.refused-ne$visualBefore.refused-or($visual.created-$visualBefore.created)-ne1-or($visual.started[0]-$visualBefore.started[0])-ne1-or($visual.started[1]-$visualBefore.started[1])-ne1-or($visual.started[2]-$visualBefore.started[2])-ne1')
s=s.replace('$visual.fxStarted-ne1-or$visual.fxStopped-ne1', '($visual.fxStarted-$visualBefore.fxStarted)-ne1-or($visual.fxStopped-$visualBefore.fxStopped)-ne1').replace('if(!$world.rejected)', 'if($world.rejected-le$worldBefore.rejected)')
s=s.replace('$duplicated=$false;$manaCommit=$null', '$duplicated=$false;$manaCommit=$null;$worldFramed=$false')
s=s.replace("  if(!$duplicated-and$w.ElapsedMilliseconds-ge8000)", """  if(!$worldFramed-and$w.ElapsedMilliseconds-ge5500){
   $committed=Q client '--mesmerize-world';$committed|ConvertTo-Json -Depth 8|Set-Content "$out/source-world-committed.json"
   if(!$committed.present){throw 'Committed world unavailable for evidence framing'}
   $sourcePlayer=Q client '--player';$centerX=($committed.transform[0]+$sourcePlayer.nativeViewPosition[0])/2;$centerY=($committed.transform[1]+$sourcePlayer.nativeViewPosition[1])/2
   C host ('setviewpos {0} {1} 68.1 180 0' -f ($centerX+7),$centerY)
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 8 -Evidence "$out/world-camera.json" -Quick
   $worldFramed=$true
  }
  if(!$duplicated-and$w.ElapsedMilliseconds-ge8000)""")
s=s.replace('Observer native body effect remains after retirement', 'Observer native body effect did not finish its native stop tail before end of recording')
s=s.replace('source retirement hides/stops it', 'source retirement hides the model and requests native effect stop; the native particle tail then leaves the manager')
Path('work/natural-moveset/Test-MesmerizeWorldNetwork-v80.ps1').write_text(s)
p=Path('work/movement-integration/Invoke-NativeLabOperation.ps1');s=p.read_text().replace('Test-MesmerizeWorldNetwork-v79.ps1','Test-MesmerizeWorldNetwork-v80.ps1');p.write_text(s)
