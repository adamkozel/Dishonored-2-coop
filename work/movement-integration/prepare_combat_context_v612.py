from pathlib import Path
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'
s=(N/'Test-CombatTarget-v611.ps1').read_text()
a='@(@(2,2),@(2,-2),@(-2,2),@(-2,-2),@(3,2),@(3,-2))'
b='@(@(2,.75),@(2,-.75),@(2,2),@(2,-2),@(-2,2),@(-2,-2),@(3,2),@(3,-2))'
assert a in s;s=s.replace(a,b).replace('CombatTarget611','CombatTarget612')
# Unlike the rear-takedown fixture, ordinary slash does not need .85m contact.
# Verify actual native stance again after teleport/collision resolution.
s=s.replace('+ .85*$fx','+1.25*$fx').replace('+.85*$fx','+1.25*$fx').replace('+.85*$fy','+1.25*$fy')
mark='     Start-Sleep -Milliseconds 90;& "$lab/Send-D2Input.ps1"'
assert mark in s
s=s.replace(mark,'''     Start-Sleep -Milliseconds 200
     for($stanceTry=0;$stanceTry-lt3;$stanceTry++){
      $nativeBefore=Q $source '--player-states'
      if($nativeBefore.stance-eq$case.stance){break}
      I $source B 180;Start-Sleep -Milliseconds 250
     }
     $nativeBefore=Q $source '--player-states';$nativeBefore|ConvertTo-Json -Depth 8|Set-Content "$out/native-before-input.json"
     if($nativeBefore.stance-ne$case.stance){throw 'Requested native source stance did not settle'}
'''+mark)
(N/'Test-CombatTarget-v612.ps1').write_text(s)
s=(N/'Observe-NativeFront-v607.ps1').read_text().replace('NativeFront607','NativeCrouchCombat613').replace('candidate-lethal-v604','candidate-combat-target-v610').replace('archived604','archived610').replace("name='front-lethal';yaw=90;stance=0","name='crouch-combat';yaw=90;stance=1")
assert a in s;s=s.replace(a,b)
start=s.index('   $paired=@(');end=s.index("   C $peer 'coop_movement_bank off'",start)
s=s[:start]+'''   $combat=@($allScenes|Where-Object {$_.state-eq3-and$_.asset-match'/combat/'})
   $combat|ConvertTo-Json -Depth 9|Set-Content "$out/combat-scenes.json"
   $samples|ForEach-Object { [pscustomobject]@{ms=$_.ms;stance=$_.player.stance;mode=$_.player.mode;active=$_.player.active} }|ConvertTo-Json -Depth 9|Set-Content "$out/native-action-states.json"
   if($combat.Count-lt2){throw 'Ordinary native crouched sword combat not observed'}
   $checks.Add('Ordinary crouched RT native combat observed; exact scene contract needs inspection')
'''+s[end:]
s=s.replace('Native ordinary RT250ms in front of exact authored guard2; source-only action research, not a remote lethal binding.','Native crouched RT250ms facing guard2; source-only combat context research, no new binding claim.')
s=s.replace('Source-only native frontal lethal takedown observation','Source-only native crouched targeted combat observation').replace('No new remote lethal binding','No new remote crouched combat binding').replace('PASS_NATIVE_LETHAL','PASS_NATIVE_CROUCH_COMBAT')
(N/'Observe-NativeCrouchCombat-v613.ps1').write_text(s)
print('Prepared612 unobstructed-camera attempts and613 bounded native crouched combat observation.')
