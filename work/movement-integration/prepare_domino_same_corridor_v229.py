from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeCancel-v228.ps1').read_text(encoding='utf-8-sig')
key=" $firstX=$anchor[0];$firstY=$anchor[1];$ci=[Globalization.CultureInfo]::InvariantCulture"
new=""" # The initial eastern guard position is behind the closed door from the balcony anchors.
 # Use a source-only point along his corridor; native physics must confirm the floor.
 if([double]$initialTargets[0].position[0]-gt-43){$anchor=@([double]$initialTargets[0].position[0],[double]$initialTargets[0].position[1]-2.5,[double]$initialTargets[0].position[2])}
 $firstX=$anchor[0];$firstY=$anchor[1];$ci=[Globalization.CultureInfo]::InvariantCulture"""
assert key in s;s=s.replace(key,new,1)
s=s.replace("placement='Measured grounded source anchor with at least two metres target separation and independent native look feedback; no NPC mutation'","placement='Verified grounded source point in the same guard corridor or measured balcony anchor, at least two metres separation and independent native look feedback; no NPC mutation'")
(P/'Test-DominoNativeCancel-v229.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig').replace('Test-DominoNativeCancel-v228.ps1','Test-DominoNativeCancel-v229.ps1');runner.write_text(s,encoding='utf-8')
