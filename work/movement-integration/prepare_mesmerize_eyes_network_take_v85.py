from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeEyesMoving-v84.ps1').read_text(encoding='utf-8-sig').replace('[ValidateRange(1,15000)][int]$Move=12000','[ValidateRange(0,15000)][int]$Move=0',1)
s=s.replace(" $visualBefore|ConvertTo-Json|Set-Content", " $eyesBefore=S host '.mesmerize-eyes.json';$eyesBefore|ConvertTo-Json -Depth 8|Set-Content \"$out/eyes-before.json\"\n $visualBefore|ConvertTo-Json|Set-Content",1)
s=s.replace("visual=(S host '.mesmerize-visual.json');effects=$fx", "visual=(S host '.mesmerize-visual.json');eyes=(S host '.mesmerize-eyes.json');effects=$fx",1)
a=s.index(' $moving=@(');b=s.index(' $eyeMonitor.WaitForExit',a)
s=s[:a]+' if($Move){\n'+s[a:b]+' }\n'+s[b:]
needle=" $checks.Add('Read-only eye references resolve exact committed owner/effect IDs and finite native transforms; no eye playback yet')"
assert needle in s
s=s.replace(needle,""" $checks.Add('Read-only eye references resolve exact committed owner/effect IDs and finite native transforms')
 $peerEyes=S host '.mesmerize-eyes.json'
 if(($peerEyes.started-$eyesBefore.started)-lt1-or($peerEyes.started-$eyesBefore.started)-ne($peerEyes.stopped-$eyesBefore.stopped)-or$peerEyes.refused-ne$eyesBefore.refused-or($peerEyes.updated-$eyesBefore.updated)-lt3-or@($peerEyes.eyes).Count){throw 'Remote eye lifecycle or owned update failed'}
 $nativeIds=@($validEyes.eyes.id|Select-Object -Unique);$remoteIds=@($rows.eyes.eyes.sourceId|Select-Object -Unique)
 if(!$remoteIds.Count-or@(Compare-Object $nativeIds $remoteIds).Count){throw 'Remote/source eye incarnations differ'}
 $effects=Get-Content "$out/observer-effects-active.json" -Raw|ConvertFrom-Json
 $nativePeerEyes=@($effects.effects|Where-Object name -eq 'models/effects/gameplay/powers/mesmerize_v2/eye/mesmerize_v2_eye_01.fx')
 if(!$nativePeerEyes.Count-or@($nativePeerEyes|Where-Object ownerDefinition -ne 'models/powers/d2coop/mesmerize_visual.def').Count){throw 'Eye effect is not owned by the cosmetic model'}
 [ordered]@{nativeIds=$nativeIds;remoteIds=$remoteIds;started=$peerEyes.started-$eyesBefore.started;stopped=$peerEyes.stopped-$eyesBefore.stopped;updated=$peerEyes.updated-$eyesBefore.updated;refused=$peerEyes.refused-$eyesBefore.refused}|ConvertTo-Json|Set-Content "$out/eye-network-summary.json"
 $checks.Add('Source eye incarnation drives one native cosmetic-owned eye through Steam, bounded transforms update and all eye effects retire without duplicate starts')
""",1)
s=s.replace('Source eye ownership is inspected read-only. Remote eye/target beams and AI/gameplay outcome consistency remain pending.','Source-owned eye incarnations and native transforms drive cosmetic model-owned eye effects through Steam. Target beams and AI/gameplay outcome consistency remain pending.')
s=s.replace('drive standing armed walking Emily body','drive standing armed Emily body')
(P/'Test-MesmerizeEyesNetwork-v85.ps1').write_text(s,encoding='utf-8')
