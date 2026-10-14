"""Keep the original air test; require uninterrupted body ownership in v170."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeAirNetwork-v169.ps1').read_text(encoding='utf-8-sig')
at=" $checks.Add('Exact source airborne phase drives remote cast plus native-air locomotion; jump stays bounded and returns to the same floor')"
assert s.count(at)==1
s=s.replace(at,at+'''
 $bodyRows=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le5000})
 $clockBefore=[int]$rows[0].movement.clockRejection.count
 $dropouts=@($bodyRows|Where-Object {$_.movement.clockRejection.count-ne$clockBefore-or!$_.movement.active-or$_.movement.weight-lt.9})
 [ordered]@{samples=$bodyRows.Count;baselineRejects=$clockBefore;minimumWeight=($bodyRows.movement.weight|Measure-Object -Minimum).Minimum;dropouts=@($dropouts|Select-Object ms,movement)}|ConvertTo-Json -Depth 12|Set-Content "$out/air-clock-continuity.json"
 if($bodyRows.Count-lt20-or$dropouts.Count){throw 'Airborne native cast introduced a body-clock rejection, lease loss or body-weight dropout'}
 $checks.Add('Takeoff, airborne cast, contact settlement and standing recovery retain body ownership without a clock rejection or body-weight dropout')
''')
(P/'Test-MesmerizeAirNetwork-v170.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig')
assert s.count('Test-MesmerizeAirNetwork-v169.ps1')==1
runner.write_text(s.replace('Test-MesmerizeAirNetwork-v169.ps1','Test-MesmerizeAirNetwork-v170.ps1'),encoding='utf-8')
