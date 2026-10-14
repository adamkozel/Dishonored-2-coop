from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeInterrupt-v82.ps1').read_text(encoding='utf-8-sig')
s=s.replace(" $visualBefore|ConvertTo-Json|Set-Content", " $eyesBefore=S host '.mesmerize-eyes.json';$eyesBefore|ConvertTo-Json -Depth 8|Set-Content \"$out/eyes-before.json\"\n $visualBefore|ConvertTo-Json|Set-Content",1)
s=s.replace("visual=(S host '.mesmerize-visual.json');effects=$fx", "visual=(S host '.mesmerize-visual.json');eyes=(S host '.mesmerize-eyes.json');sourceWorld=(S client '.mesmerize-world-stream.json');effects=$fx",1)
needle=" $checks.Add('Committed source world drives one model and body effect; explicit stream stop or native pause retires ownership, hides model and lets the native effect tail finish')"
assert needle in s
s=s.replace(needle,""" $peerEyes=S host '.mesmerize-eyes.json'
 $eyeStarts=$peerEyes.started-$eyesBefore.started;$eyeStops=$peerEyes.stopped-$eyesBefore.stopped
 if($eyeStarts-lt1-or$eyeStarts-ne$eyeStops-or$peerEyes.refused-ne$eyesBefore.refused-or($peerEyes.updated-$eyesBefore.updated)-lt3-or@($peerEyes.eyes).Count){throw 'Interrupted remote eye lifecycle failed'}
 if(@($rows|Where-Object {$_.ms-gt15000-and@($_.eyes.eyes).Count}).Count){throw 'Old eye revived after interruption'}
 $activeFx=Get-Content "$out/observer-effects-active.json" -Raw|ConvertFrom-Json
 $eyeFx=@($activeFx.effects|Where-Object name -eq 'models/effects/gameplay/powers/mesmerize_v2/eye/mesmerize_v2_eye_01.fx')
 if(!$eyeFx.Count-or@($eyeFx|Where-Object ownerDefinition -ne 'models/powers/d2coop/mesmerize_visual.def').Count){throw 'No cosmetic-owned native eye before interruption'}
 [ordered]@{started=$eyeStarts;stopped=$eyeStops;updated=$peerEyes.updated-$eyesBefore.updated;refused=$peerEyes.refused-$eyesBefore.refused;sourceIds=@($rows.eyes.eyes.sourceId|Select-Object -Unique);interruption=$Interrupt}|ConvertTo-Json|Set-Content "$out/eye-interrupt-summary.json"
 $checks.Add('Native cosmetic-owned eye updates before interruption, retires once per incarnation with the world, and cannot revive from stale packets')
"""+needle,1)
s=s.replace('WaitForExit(18000)','WaitForExit(40000)')
s=s.replace('AI distraction/target beams/gameplay outcome consistency remain pending.','Source-referenced eye effects are owned by the cosmetic model and retire on interruption. AI distraction/target beams/gameplay outcome consistency remain pending.')
(P/'Test-MesmerizeEyeInterrupt-v88.ps1').write_text(s,encoding='utf-8')
