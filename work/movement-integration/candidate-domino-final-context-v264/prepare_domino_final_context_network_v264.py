"""Generalize the proven armed crouch final test to explicit equipment/stance.
This harness is prepared before reader promotion; native evidence must come first.
"""
from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'; W=R/'work/movement-integration'
s=(P/'Test-DominoCrouchFinalNetwork-v256.ps1').read_text(encoding='utf-8-sig')
c=(P/'Test-DominoTargetContextNetwork-v262.ps1').read_text(encoding='utf-8-sig')
s=s.replace('[int]$Pitch=12)',"[int]$Pitch=0,[ValidateSet('standing','crouched')][string]$Stance='crouched',[switch]$Holstered)")
a=s.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180')
b=s.index(' $hwnd=',a)
ca=c.index(' $expectedStance='); cb=c.index(' $hwnd=',ca)
s=s[:a]+c[ca:cb].replace('move=$Move;', 'move=0;')+' $groundState=if($expectedStance){6}else{0}\n'+s[b:]
s=s.replace('if(!$stood-and$w.ElapsedMilliseconds-ge9500){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;', 'if(!$stood-and$w.ElapsedMilliseconds-ge9500){if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null};')
s=s.replace("player=(Q client '--player-states');movement=$m", "player=(Q client '--player-states');sword=(Q client '--player-sword');movement=$m")
s=s.replace("stream=$stream;effects=$fx", "stream=$stream;sourceStream=(S client '.shadow-stream.json');effects=$fx")
s=s.replace('$_.activeStates[0]-eq6','$_.activeStates[0]-eq$groundState').replace('$_.stance-ne1','$_.stance-ne$expectedStance')
s=s.replace("if(!$sword.equipped-or!(Q client '--player-sword').equipped)","if($sword.equipped-eq[bool]$Holstered-or(Q client '--player-sword').equipped-eq[bool]$Holstered)")
s=s.replace('$_.player.stance-ne1-or$_.movement.stanceMix-lt.98', '$_.player.stance-ne$expectedStance-or[Math]::Abs($_.movement.stanceMix-$expectedStance)-gt.02-or$_.movement.presentedDrawn-eq[bool]$Holstered-or$_.sword.equipped-eq[bool]$Holstered')
s=s.replace('finalSamples=$final.Count;', 'stance=$Stance;holstered=[bool]$Holstered;move=0;finalSamples=$final.Count;')
s=s.replace("source='client';checks=", "source='client';stance=$Stance;holstered=[bool]$Holstered;move=0;checks=")
s=s.replace('crouched controller casting drives observed armed Emily complete-body crouched phases', 'native controller casting drives the explicit Emily stance/equipment phases')
s=s.replace('Sneaking6 recovery past300ms','the matching grounded recovery state past300ms')
s=s.replace('then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment', 'then standing recovery through native Steam, with source scene timing and unchanged equipment state')
(P/'Test-DominoFinalContextNetwork-v264.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig')
if "'DominoFinalContext'" not in s:
 s=s.replace("'DominoContextTarget',", "'DominoFinalContext','DominoContextTarget',")
 s=s.replace("  'DominoContextTarget'{", "  'DominoFinalContext'{& \"$root/work/natural-moveset/Test-DominoFinalContextNetwork-v264.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered}\n  'DominoContextTarget'{")
 p.write_text(s,encoding='utf-8')
print('Prepared explicit native final-selection network contexts v264.')
