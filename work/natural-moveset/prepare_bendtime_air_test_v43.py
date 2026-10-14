"""Preserve the ground harness and derive guarded paired airborne coverage."""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Test-BendTimeContexts.ps1').read_text(encoding='utf-8-sig')
s=s.replace("[switch]$Crouched,",'')
s=s.replace("$ErrorActionPreference='Stop';", "$Crouched=$false\n$ErrorActionPreference='Stop';",1)
s=s.replace("'bendtime-corvo-context-v41-v1'","'bendtime-corvo-air-v43-v1'").replace("'bendtime-context-bank-manifest-v41.json'","'bendtime-balanced-bank-manifest-v42.json'")
s=s.replace("$cameraPitch=if($Crouched){21}else{15}","$cameraPitch=0 # Keep the whole jump in frame.")
old="I $Source 255 350 Neutral $walk;$events.Add"
new="I $Source 0 220 A $walk;Start-Sleep -Milliseconds 80;$jump=Q $Source '--player-states';$jump|ConvertTo-Json -Depth 10|Set-Content -Encoding utf8 \"$out/jump-before-cast-$next.json\";if(!$jump.motionValid-or$jump.motion-ne2){throw 'Native Jump must precede Bend Time casting'};I $Source 255 350 Neutral $walk;$events.Add"
assert old in s;s=s.replace(old,new).replace("action='Native LT press 350ms'","action='Confirmed native A jump, then LT cast 350ms'")
old=" $peerPower=Q $peer '--player-darkvision';"
new=""" $air=@($rows|Where-Object {$_.native.motion-in@(2,3)-and$_.native.powerSceneValid-and$_.movement.powerWeight-gt.5-and$_.movement.airWeight-gt.5})
 if(!$air.Count){throw 'No confirmed native airborne cast overlaps remote cast and jump presentation'}
 if(@($air|Where-Object {$_.movement.airPhase-gt0}).Count-lt1){throw 'Remote airborne cast has no advancing jump phase'}
 $checks.Add('Exact native airborne power scene overlaps remote cast and airborne leg presentation')
 $peerPower=Q $peer '--player-darkvision';"""
assert old in s;s=s.replace(old,new)
s=s.replace("character=$character;crouched=","character=$character;airborneExperiment=$true;crouched=")
s=s.replace('Corvo Bend Time ground stance/equipment/travel context integration.', 'Corvo Bend Time airborne cast, landing and equipment integration.')
s=s.replace("'Full-body cast retains original native crouch/standing stance throughout'","'Native standing physics stance is retained through jump/fall and landing'")
target=P/'Test-BendTimeAir.ps1';assert not target.exists();target.write_text(s,encoding='utf-8');print(target)
