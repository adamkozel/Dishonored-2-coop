from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-SwarmRatVisualNetwork-v294.ps1').read_text(encoding='utf-8-sig')
needle=" Q client '--components' $visual.handle|ConvertTo-Json -Depth 12|Set-Content \"$out/visual-components.json\""
assert needle in s
s=s.replace(needle,""" $verified=@($visualRows|Where-Object {$_.visual.verifiedTransforms-gt20})
 if($verified.Count-lt20-or@($verified|Where-Object {$_.visual.maxPositionError-gt.001}).Count){throw 'Live rat transform readback did not match source target'}
 $extent=@(foreach($axis in 0,1,2){$values=@($verified|ForEach-Object {$_.visual.actualTransform[$axis]});$range=$values|Measure-Object -Minimum -Maximum;[double]$range.Maximum-[double]$range.Minimum})
 if(($extent|Measure-Object -Maximum).Maximum-lt.1){throw 'Readback rat body did not travel at least10cm'}
 [ordered]@{samples=$verified.Count;axisExtent=$extent;maximumPositionError=($verified.visual.maxPositionError|Measure-Object -Maximum).Maximum;verifiedTransforms=$visual.verifiedTransforms}|ConvertTo-Json|Set-Content "$out/live-transform-summary.json"
 $checks.Add('Owned live rat transform readback matches received target within1mm and moves at least10cm; animation output assessed separately')
"""+needle)
s=s.replace('One cosmetic rat follows a peer agent using a native walk scene; this proves model/playback compatibility only.','One cosmetic rat receives live transforms through guarded native dispatch with actual position readback. Midnight clock activity alone does not prove animation; joint output and visual acceptance remain separately assessed.')
(P/'Test-SwarmRatVisualNetwork-v295.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v294.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v294.ps1','Test-SwarmRatVisualNetwork-v295.ps1'))
