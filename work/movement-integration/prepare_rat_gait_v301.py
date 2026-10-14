from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab'
p=L/'Build-D2SwarmRatVisualAssets.ps1';s=p.read_text(encoding='utf-8-sig');assert 'swarm-rat-leaf-v296'in s;p.write_text(s.replace('swarm-rat-leaf-v296','swarm-rat-gait-v301'))
p=L/'src/swarm_rat_visual_fixture.h';s=p.read_text()
needle='fputs("]}",f);\n    }';assert needle in s
s=s.replace(needle,'fprintf(f,"],\\\"gaitSpeed\\\":%.6f,\\\"gaitRate\\\":%.6f,\\\"gaitMoving\\\":%.6f,\\\"gaitUpdates\\\":%llu}",a.gaitSpeed,a.gaitRate,a.gaitMoving,a.gaitUpdates);\n    }')
p.write_text(s)
s=(P/'Test-SwarmRatVisualNetwork-v300.ps1').read_text(encoding='utf-8-sig')
needle=" $checks.Add('25 unique owned cosmetic models match25 opaque source indices/handles/incarnations for at least20 snapshots, all with verified live transforms')";assert needle in s
s=s.replace(needle,needle+r'''
 $gaits=@($fullPool|ForEach-Object {$_.visual.models}|Where-Object {$_.active-and$_.gaitUpdates-gt10})
 if($gaits.Count-lt500){throw 'Insufficient native gait input readback samples'}
 foreach($g in $gaits){$expected=[Math]::Min(3.0,$g.gaitSpeed/1.11);if([Math]::Abs($g.gaitRate-$expected)-gt.0001-or$g.gaitMoving-lt0-or$g.gaitMoving-gt1){throw 'Native gait rate or blend differs from measured travel'}}
 $moving=@($gaits|Where-Object {$_.gaitSpeed-gt.05-and$_.gaitRate-gt.04});$held=@($gaits|Where-Object {$_.gaitSpeed-lt.001-and$_.gaitRate-lt.001})
 if($moving.Count-lt100-or$held.Count-lt10){throw 'Measured moving/held gait contexts missing'}
 [ordered]@{nativeInputSamples=$gaits.Count;movingSamples=$moving.Count;heldSamples=$held.Count;minimumRate=($gaits.gaitRate|Measure-Object -Minimum).Minimum;maximumRate=($gaits.gaitRate|Measure-Object -Maximum).Maximum;strideSpeed=1.11;visualAccepted=$false}|ConvertTo-Json|Set-Content "$out/gait-summary.json"
 $checks.Add('Named native gait inputs read back correctly and track measured horizontal travel divided by native1.11m/s stride speed; idle/moving samples covered')
''')
s=s.replace('remoteVisuals=$false;remoteDamage=$false','remoteCosmeticVisuals=$true;remoteDamage=$false')
s=s.replace('body playback creates no peer rats or duplicate power actions','body playback creates no peer gameplay rats or duplicate power actions')
s=s.replace('source animation phase, speed, scale, subjective interpolation quality','source-specific animation phase/state, scale, subjective gait/interpolation quality')
(P/'Test-SwarmRatVisualNetwork-v301.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v300.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v300.ps1','Test-SwarmRatVisualNetwork-v301.ps1'))
