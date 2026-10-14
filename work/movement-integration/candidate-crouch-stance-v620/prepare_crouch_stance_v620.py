from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(W/'Build-CrouchCombat-v616.ps1').read_text().replace('v616.log','v620.log').replace('Build616','Build620');(W/'Build-CrouchStance-v620.ps1').write_text(s)
s=(W/'Deploy-CrouchCombat-v616.ps1').read_text().replace('CrouchCombatDeploy616','CrouchStanceDeploy620').replace('stage_crouch_combat_v616.py','stage_crouch_stance_v620.py');(W/'Deploy-CrouchStance-v620.ps1').write_text(s)
s=(W/'stage_crouch_combat_v616.py').read_text().replace("A=W/'candidate-crouch-combat-v616';P=W/'candidate-combat-target-v610'","A=W/'candidate-crouch-stance-v620';P=W/'candidate-crouch-combat-v616'")
s=s.replace('v616.log','v620.log').replace('prepare_crouch_combat_v616','prepare_crouch_stance_v620').replace('Build-CrouchCombat-v616','Build-CrouchStance-v620').replace('Deploy-CrouchCombat-v616','Deploy-CrouchStance-v620').replace('Test-CrouchCombat-v617','Test-CrouchCombat-v621')
s=s.replace('Native613 crouched target-bearing right/left933 and owned hit-recovery right866/left666.','Native613/619 target-bearing crouched strikes can keep their native933 scene while rising to standing, or switch to optional owned hit-recovery866/666.')
(W/'stage_crouch_stance_v620.py').write_text(s)
s=(N/'Test-CrouchCombat-v619.ps1').read_text().replace('CrouchCombat619','CrouchCombat621').replace('candidate-crouch-combat-v616','candidate-crouch-stance-v620').replace('archived616','archived620')
a=s.index("   if($recovery.Count-lt2)");b=s.index('   $lastEnd=',a)
s=s[:a]+'''   if($recovery.Count){
    $checks.Add('Independent reader observes an optional owned hit-recovery scene')
    $recoveryPlayed=@($samples|Where-Object {$_.movement.combatVariant-in15,16-and$_.movement.combatWeight-gt.65-and$_.movement.weight-gt.85})
    if(!$recoveryPlayed.Count){throw 'Observed native hit recovery did not reach remote body'};$checks.Add('Observed hit-recovery scene drives existing authored follow-through')
    foreach($sample in $recoveryPlayed){if($sample.movement.combatAuthoredAmount-lt$sample.movement.combatRecoveryStart-or$sample.movement.combatAuthoredAmount-gt1){throw 'Recovery authored phase outside continuation'}}
    $checks.Add('Hit recovery remains inside the preserved attack follow-through interval')
   }
   [ordered]@{ownedHitRecoverySamples=$recovery.Count;sourceRiseSamples=@($validated|Where-Object {$_.player.stance-eq0}).Count;required='Native targeted crouched strike and locomotion recovery';optional='Separate native hit-recovery scenes only when actually observed'}|ConvertTo-Json|Set-Content "$out/context-coverage.json"
'''+s[b:]
s=s.replace('$_.player.combatScene.targetHandle-eq$g.handle','$_.player.combatScene.targetHandle-eq$g.handle')
s=s.replace('target-bearing slash and owned recovery','target-bearing slash, native stance recovery and optional owned recovery when observed')
s=s.replace('owned hit follow-through and native stance recovery.','native stance recovery and optional owned hit follow-through. A missing optional scene is not counted as verified.')
(N/'Test-CrouchCombat-v621.ps1').write_text(s)
print('Prepared620 native stance continuity and621 tests that distinguish optional recovery coverage.')
