"""Derive a bounded, source-only native final-selection observation.

No runtime writes beyond the existing reversible inventory fixture and native
input. Look away before X so holstering cannot also pickpocket the target.
"""
from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'; W=R/'work/movement-integration'
s=(P/'Observe-NativeDominoCrouchFinal-v251.ps1').read_text(encoding='utf-8-sig')
s=s.replace('[switch]$AimGuard)',"[switch]$AimGuard,[ValidateSet('standing','crouched')][string]$Stance='crouched')")
s=s.replace('coop_domino_fixture 20000','coop_domino_fixture 40000')
a=s.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180')
b=s.index(' $hwnd=',a)
s=s[:a]+''' $expectedStance=if($Stance-eq'crouched'){1}else{0}
 $groundState=if($expectedStance){6}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
 & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1200
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 0 -Evidence "$out/holster-look-return.json" -Quick
 $context=Q client '--player-states';$sword=Q client '--player-sword'
 [ordered]@{stance=$Stance;native=$context;sword=$sword}|ConvertTo-Json -Depth 10|Set-Content "$out/context-before.json"
 if($context.stance-ne$expectedStance-or!$context.motionValid-or$context.motion-ne$expectedStance-or$sword.equipped){throw 'Native grounded holstered context not established'}
''' +s[b:]
s=s.replace("player=(Q client '--player-states');midnight=$scene", "player=(Q client '--player-states');sword=(Q client '--player-sword');midnight=$scene")
s=s.replace("$_.player.stance-eq1-and$_.player.defaultMode-and@($_.player.active|Where-Object {$_.id-eq6-and$_.vtableRva-eq'0x1c84258'}).Count", "$_.player.stance-eq$expectedStance-and!$_.sword.equipped-and$_.player.defaultMode-and@($_.player.active|Where-Object {$_.id-eq$groundState}).Count")
s=s.replace('$_.id-notin6,72','$_.id-notin$groundState,72')
s=s.replace(" $final|ConvertTo-Json -Depth 14|Set-Content", " if(@($rows|Where-Object {$_.sword.equipped}).Count){throw 'Sword redrawn during holstered observation'}\n $final|ConvertTo-Json -Depth 14|Set-Content")
s=s.replace('Unimplemented crouched final selection unexpectedly accepted','Unimplemented holstered final selection unexpectedly accepted')
s=s.replace('crouchedCastingSamples=$nativeCrouch.Count;', 'stance=$Stance;holstered=$true;holsteredCastingSamples=$nativeCrouch.Count;')
s=s.replace("source='client';checks=", "source='client';stance=$Stance;holstered=$true;checks=")
s=s.replace('while crouched', 'while holstered in the selected ground stance')
s=s.replace('Targeted crouched native Domino final-selection observation', 'Targeted holstered native Domino final-selection observation')
s=s.replace('Native crouch plus state72 independently recorded', 'Native selected ground stance plus state72 independently recorded with sword remaining holstered')
(P/'Observe-NativeDominoHolsteredFinal-v263.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig')
if "'DominoHolsteredFinalObserve'" not in s:
 s=s.replace("'DominoHolsteredObserve',", "'DominoHolsteredFinalObserve','DominoHolsteredObserve',")
 s=s.replace("  'DominoHolsteredObserve'{", "  'DominoHolsteredFinalObserve'{& \"$root/work/natural-moveset/Observe-NativeDominoHolsteredFinal-v263.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance}\n  'DominoHolsteredObserve'{")
 p.write_text(s,encoding='utf-8')
print('Prepared holstered final observation v263; production reader unchanged.')
