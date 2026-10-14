from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab'
p=L/'src/swarm_rat_visual_fixture.h';s=p.read_text();s=s.replace('std::uint64_t verified=0;','std::uint64_t verified=0,interpolated=0;').replace('verified+=p.verifiedTransforms;}','verified+=p.verifiedTransforms;interpolated+=p.interpolatedTransforms;}')
needle='    fputs("],\\\"models\\\":[",f);bool comma=false;';assert needle in s
s=s.replace(needle,'    fprintf(f,"],\\\"interpolatedTransforms\\\":%llu,\\\"models\\\":[",interpolated);bool comma=false;')
p.write_text(s)
p=P/'Test-SwarmRatVisualNetwork-v298.ps1';s=p.read_text(encoding='utf-8-sig')
needle=" $checks.Add('25 unique owned cosmetic models match25 opaque source indices/handles/incarnations for at least20 snapshots, all with verified live transforms')";assert needle in s
s=s.replace(needle,needle+"""
 if($visual.interpolatedTransforms-lt500){throw 'Insufficient per-frame rat interpolation between native Steam snapshots'}
 [ordered]@{interpolatedUpdates=$visual.interpolatedTransforms;verifiedUpdates=$visual.verifiedTransforms;ratio=$visual.interpolatedTransforms/[double]$visual.verifiedTransforms;extrapolation=$false}|ConvertTo-Json|Set-Content "$out/interpolation-summary.json"
 $checks.Add('More than500 actual native-transform updates interpolate between source snapshots; scope changes and missing samples hold rather than extrapolate')
""")
s=s.replace('source animation phase, speed, scale, smooth interpolation','source animation phase, speed, scale, subjective interpolation quality')
(P/'Test-SwarmRatVisualNetwork-v299.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v298.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v298.ps1','Test-SwarmRatVisualNetwork-v299.ps1'))
