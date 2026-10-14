from pathlib import Path

R = Path(__file__).resolve().parents[2]
W, N = R / 'work/movement-integration', R / 'work/natural-moveset'

def replace_once(s, old, new):
    assert s.count(old) == 1, old
    return s.replace(old, new)

s = (N / 'Observe-NativeDoppelgangerCrouch-v489.ps1').read_text(encoding='utf-8-sig')
s = replace_once(s, '[int]$Pitch=30)', "[int]$Pitch=30,[ValidateSet('standing','crouched')][string]$Stance='standing')")
start = s.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B')
end = s.index(' $hwnd=', start)
s = s[:start] + ''' $expectedStance=if($Stance-eq'crouched'){1}else{0}
 $groundState=if($expectedStance){6}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500}
 # Native X can interact; look away from all actors before the holster hold.
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
 & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null
 Start-Sleep -Milliseconds 1200
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/holster-look-return.json" -Quick
 $context=Q client '--player-states';$sword=Q client '--player-sword'
 [ordered]@{stance=$Stance;native=$context;sword=$sword}|ConvertTo-Json -Depth 12|Set-Content "$out/context-before.json"
 if($context.stance-ne$expectedStance-or!$context.motionValid-or$context.motion-ne$expectedStance-or$sword.equipped){throw 'Native grounded holstered context not established'}

''' + s[end:]
start = s.index(' $nativeCrouch=')
end = s.index(' $scenes|ConvertTo-Json', start)
s = s[:start] + ''' $nativeCasting=@($strict|Where-Object {$_.playerStatesValid-and$_.stance-eq$expectedStance-and$groundState-in$_.activeStates-and63-in$_.activeStates-and!@($_.activeStates|Where-Object {$_-notin$groundState,63}).Count})
 $equipmentRows=@($rows|Where-Object {$_.player.stance-eq$expectedStance-and@($_.player.active|Where-Object id -eq 63).Count})
 [ordered]@{stance=$Stance;initiallyHolstered=$true;castingSamples=$nativeCasting.Count;productionAccepted=@($strict|Where-Object valid).Count;stateSets=@($strict|Where-Object playerStatesValid|ForEach-Object {($_.activeStates|Sort-Object)-join','}|Sort-Object -Unique);rawSceneSamples=$scenes.Count;swordEquippedWhileCasting=@($equipmentRows|Where-Object {$_.sword.equipped}).Count;swordHolsteredWhileCasting=@($equipmentRows|Where-Object {!$_.sword.equipped}).Count;equipmentObservations=@($equipmentRows|ForEach-Object {[ordered]@{ms=$_.ms;videoTime=$_.videoTime;sword=$_.sword}});scope='Source observation of native cast beginning holstered. Equipment changes and strict-reader acceptance are observations; no new remote context verified.'}|ConvertTo-Json -Depth 12|Set-Content "$out/holstered-observations.json"
 if($nativeCasting.Count-lt3-or$equipmentRows.Count-lt3){throw 'Too few independent native state/equipment casting samples'}
 $checks.Add('Native grounded cast from sheathed sword independently observed with reciprocal In/Loop/Out scenes; actual equipment behavior recorded without assuming auto-draw or empty-hand support')

''' + s[end:]
s = s.replace('Native crouched Emily LT1800 then release at grounded floor', 'Native Emily LT1800 from initially holstered ground stance then release')
s = replace_once(s, "$entities=$null;if($w.ElapsedMilliseconds-ge$entityNext){$entityNext=$w.ElapsedMilliseconds+2000;$entities=Q client '--entities'};$entities|ConvertTo-Json", "$entities=Q client '--entities';$entities|ConvertTo-Json")
s = s.replace("source='client';checks=$checks", "source='client';stance=$Stance;initiallyHolstered=$true;checks=$checks")
s = s.replace('Native source crouched Emily Doppelganger acquisition', 'Native source initially holstered Emily Doppelganger acquisition')
p = N / 'Observe-NativeDoppelgangerHolstered-v493.ps1'
assert not p.exists()
p.write_text(s, encoding='utf-8')
w = (W / 'Run-DoppelgangerCrouchObserve-v489.ps1').read_text(encoding='utf-8-sig')
w = replace_once(w, "[string]$Take='native-doppelganger-crouch-v489')", "[string]$Take='native-doppelganger-holstered-standing-v493',[ValidateSet('standing','crouched')][string]$Stance='standing')")
w = w.replace('Observe-NativeDoppelgangerCrouch-v489.ps1" -Take $Take', 'Observe-NativeDoppelgangerHolstered-v493.ps1" -Take $Take -Stance $Stance')
p = W / 'Run-DoppelgangerHolsteredObserve-v493.ps1'
assert not p.exists()
p.write_text(w, encoding='utf-8')
print('Prepared isolated native holstered Doppelganger context observer; no production edits.')
