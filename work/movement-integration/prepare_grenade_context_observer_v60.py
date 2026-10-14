"""Build a source-only native crouch/movement observation; no new action whitelist."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeGrenadeActions-v54.ps1').read_text(encoding='utf-8-sig')
def replace(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a))
 s=s.replace(a,b)
replace('[switch]$ValidateReader)',"[switch]$ValidateReader,[ValidateSet('standing','crouched')][string]$Stance='crouched',[ValidateRange(0,15000)][int]$Move=0)")
replace("$ms=100,$lt=0)","$ms=100,$lt=0,$y=0)")
replace('-LeftTrigger $lt|Out-Null','-LeftTrigger $lt -LeftY $y|Out-Null')
replace(" C $Source 'setviewpos -33 -153 68.1 180 0'"," C $Source 'setviewpos -34.3 -154.8 68.1 0 0'")
replace(" $checks.Add('One native grenade selected through normal holster/draw input')", """ if($Stance-eq'crouched'){I $Source B 180;Start-Sleep -Milliseconds 650}
 $nativeStance=Q $Source '--player-states'
 if($nativeStance.stance-ne$(if($Stance-eq'crouched'){1}else{0})){throw 'Requested native stance unavailable'}
 $nativeStance|ConvertTo-Json -Depth 10|Set-Content "$out/source-stance.json"
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch $Pitch -Evidence "$out/pitch-after-draw.json"
 $checks.Add('One native grenade selected and requested native stance confirmed')""")
# Always collect the strict reader's refusals too; validation is optional for
# a newly observed context. Do not turn its false results into accepted phases.
replace(" if($ValidateReader){if((Q $Source '--player-grenade-scene').valid)"," if($true){if((Q $Source '--player-grenade-scene').valid)")
replace("if($monitor){\n  $monitor.WaitForExit", "if($monitor-and$ValidateReader){\n  $monitor.WaitForExit")
replace("$dirty=$true;I $Source Neutral 1300 255;", "$dirty=$true;I $Source Neutral 1300 255 $Move;")
replace("input='LT held for 1300ms then native release';pitch=$Pitch", "input='LT held for 1300ms then native release';pitch=$Pitch;stance=$Stance;leftY=$Move")
replace("pitch=$Pitch;checks=$checks", "pitch=$Pitch;stance=$Stance;leftY=$Move;checks=$checks")
replace("if($failure){throw $failure};\"PASS_NATIVE_GRENADE_ACTIONS", "if($failure){throw $failure};\"PASS_NATIVE_GRENADE_CONTEXT_OBSERVATION")
(P/'Observe-NativeGrenadeContexts-v60.ps1').write_text(s,encoding='utf-8')
print('Prepared source-only native context observer; runtime recognition unchanged')
