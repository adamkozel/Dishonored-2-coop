"""Derive a bounded source-only air investigation from the verified ground fixture."""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Observe-BendTime.ps1').read_text(encoding='utf-8-sig')
s=s.replace("'native-bendtime-corvo-v1'", "'native-bendtime-corvo-air-v43'")
old="{I host 255 $TriggerMs;$events.Add"
new="{I host 0 220 A;Start-Sleep -Milliseconds 80;$jump=Q host '--player-states';$jump|ConvertTo-Json -Depth 10|Set-Content -Encoding utf8 \"$out/jump-before-cast-$next.json\";if(!$jump.motionValid-or$jump.motion-ne2){throw 'Native jump did not precede the cast'};I host 255 $TriggerMs;$events.Add"
assert old in s;s=s.replace(old,new)
s=s.replace('action="Native LT press ${TriggerMs}ms"', 'action="Confirmed native A jump then LT press ${TriggerMs}ms"')
s=s.replace(";$entities=Q host '--entities'",'')
s=s.replace("$wind=@($entities.entities|Where-Object {$_.definitionName-match'bendtime'})",'$wind=@()')
s=s.replace("component=(Q host '--object' $power.component)","power=(Q host '--player-bendtime')")
old="if(($midnight|ConvertTo-Json -Depth 12 -Compress)-match'power_bendtime_cast'){$sceneCount++}"
new="if(@($midnight.instances|Where-Object {$_.state-eq3-and$_.asset-match'power_bendtime_cast'}).Count){$sceneCount++}"
assert old in s;s=s.replace(old,new)
old="if(!$sceneCount){throw 'No native BendTime cast scene observed'};"
new="""$airCasts=@($rows|Where-Object {$_.player.motion-in@(2,3)-and@($_.midnight.instances|Where-Object {$_.state-eq3-and$_.asset-match'power_bendtime_cast'}).Count})
 if(!$airCasts.Count){throw 'No overlapping native Jump/Fall and active BendTime scene observed'};$checks.Add('Native Jump/Fall overlaps the exact active BendTime scene')
 if(!$sceneCount){throw 'No native BendTime cast scene observed'};"""
assert old in s;s=s.replace(old,new)
s=s.replace("source='host';crouched=", "source='host';airborneExperiment=$true;crouched=")
s=s.replace('Native BendTime investigation only;', 'Native airborne BendTime investigation only;')
s=s.replace("-lt7000)","-lt32000)")
target=P/'Observe-BendTimeAir.ps1';assert not target.exists();target.write_text(s,encoding='utf-8')
print(target)
