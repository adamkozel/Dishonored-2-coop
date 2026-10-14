from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeWorldNetwork-v81.ps1').read_text(encoding='utf-8-sig')
s=s.replace('[int]$Pitch=12)', '[int]$Pitch=12,[ValidateRange(1,15000)][int]$Move=9000)',1)
s=s.replace('I client 255 1800;', '& "$lab/Send-D2Input.ps1" -Role client -Button Neutral -LeftTrigger 255 -LeftY $Move -DurationMs 1800|Out-Null;',1)
s=s.replace("input='Native LT held 1800ms then released'", "input='Native forward stick and LT held together 1800ms then released';stick=$Move",1)
needle=" $checks.Add('All exact source scene incarnations, including native aim-loop renewal, match remote phase events through Steam')"
assert needle in s
s=s.replace(needle,needle+"""
 $moving=@($rows|Where-Object {$_.movement.powerVariant-in16,17,18-and$_.movement.powerWeight-gt.5-and$_.movement.speed-gt.2})
 if($moving.Count-lt3){throw 'Insufficient simultaneous moving cast presentation'}
 $positions=@($rows|Where-Object {$_.ms-lt5000}|ForEach-Object {$_.player.position[1]})
 $distance=($positions|Measure-Object -Maximum).Maximum-($positions|Measure-Object -Minimum).Minimum
 if($distance-lt.2-or$distance-gt4){throw "Unproven or excessive native walking displacement: $distance"}
 [ordered]@{forwardRange=$distance;simultaneousSamples=$moving.Count;maxSpeed=($moving.movement.speed|Measure-Object -Maximum).Maximum}|ConvertTo-Json|Set-Content "$out/moving-cast-summary.json"
 $checks.Add('Native forward walking overlaps all-body cast presentation, with measured displacement and standing recovery')
""",1)
s=s.replace("source='client';checks=$checks", "source='client';move=$Move;checks=$checks",1)
s=s.replace('drive standing armed Emily body','drive standing armed walking Emily body')
(P/'Test-MesmerizeMoving-v83.ps1').write_text(s,encoding='utf-8')
