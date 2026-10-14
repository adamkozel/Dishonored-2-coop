from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
s=(P/'Test-SwarmRatVisualNetwork-v299.ps1').read_text(encoding='utf-8-sig')
needle=" $checks.Add('More than500 actual native-transform updates interpolate between source snapshots; scope changes and missing samples hold rather than extrapolate')";assert needle in s
s=s.replace(needle,needle+"\n if($visual.interpolatedTransforms/[double]$visual.verifiedTransforms-lt.85){throw 'Rat interpolation coverage below85percent after cadence increase'}\n $checks.Add('At least85percent of verified native rat updates have both source samples available at the existing80ms presentation delay')\n")
(P/'Test-SwarmRatVisualNetwork-v300.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v299.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v299.ps1','Test-SwarmRatVisualNetwork-v300.ps1'))
p=W/'archive_rat_interpolation_v299.py';s=p.read_text().replace('interpolation-v299','cadence-v300').replace('interpolation_v299','cadence_v300').replace('interpolation-{kind}-v299','cadence-{kind}-v300').replace('candidate-swarm-rat-pool-v298','candidate-swarm-rat-interpolation-v299').replace('swarm-rat-pool-v298/result.json','swarm-rat-interpolation-v299/result.json').replace('Test-SwarmRatVisualNetwork-v299','Test-SwarmRatVisualNetwork-v300')
s=s.replace("('build','protocol','pose','smoke')","('build','smoke')")
s=s.replace('Per-frame source-time cosmetic rat XYZ/quaternion interpolation between current presented and next accepted world snapshot,80ms delayed.','Increase known native rat snapshot cadence from80ms to33ms, keeping80ms presentation delay and bounded wire/ownership reader. Previous299 measured67.3percent interpolated updates; test now requires85percent. Per-frame XYZ/quaternion presentation unchanged.')
(W/'archive_rat_cadence_v300.py').write_text(s)
