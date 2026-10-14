from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(W/'Build-CombatTarget-v610.ps1').read_text().replace('v610.log','v616.log').replace('Build610','Build616')
(W/'Build-CrouchCombat-v616.ps1').write_text(s)
s=(W/'Deploy-CombatTarget-v610.ps1').read_text().replace('CombatTargetDeploy610','CrouchCombatDeploy616').replace('stage_combat_target_v610.py','stage_crouch_combat_v616.py')
(W/'Deploy-CrouchCombat-v616.ps1').write_text(s)
s=(W/'stage_combat_target_v610.py').read_text().replace("A=W/'candidate-combat-target-v610';P=W/'candidate-lethal-v604'","A=W/'candidate-crouch-combat-v616';P=W/'candidate-combat-target-v610'")
s=s.replace('v610.log','v616.log').replace('prepare_combat_target_v610','prepare_crouch_combat_v616').replace('Build-CombatTarget-v610','Build-CrouchCombat-v616').replace('Deploy-CombatTarget-v610','Deploy-CrouchCombat-v616').replace('Test-CombatTarget-v611','Test-CrouchCombat-v617').replace('protocol=65','protocol=66')
a="banks=[dict(path=str(p.relative_to(R)),sha256=sha(p)) for p in sorted((P/'banks').glob('*.bin'))];assert len(banks)==2"
b="banks=rd(P/'identity.json')['banks'];assert len(banks)==2\nfor bank in banks:assert sha(R/bank['path'])==bank['sha256']"
assert a in s;s=s.replace(a,b)
a=s.index("scope='");end=s.index("')",a)
s=s[:a]+"scope='Native613 crouched target-bearing right/left933 and owned hit-recovery right866/left666. Existing bank65/353 reused; protocol66 adds combat variants15/16. Continue prior authored strike into follow-through and blend native stance recovery. Source owns all victim events/outcomes. Build and runtime validation pending.'"+s[end+1:]
(W/'stage_crouch_combat_v616.py').write_text(s)
s=(N/'Test-CombatTarget-v614.ps1').read_text().replace('CombatTarget614','CrouchCombat617').replace('candidate-combat-target-v610','candidate-crouch-combat-v616').replace('archived610','archived616').replace("name='targeted-slash';yaw=90;stance=0","name='crouched-slash';yaw=90;stance=1")
assert "name='crouched-slash'" in s
s=s.replace('+.9*$fx','+.85*$fx').replace('+.9*$fy','+.85*$fy').replace('+1.25*$fx','+.85*$fx').replace('+1.25*$fy','+.85*$fy')
s=s.replace("'sword_slash_rightb?\\.midnightscene$'","'sword_slash_sneak_(right|left)\\.midnightscene$'")
s=s.replace('$_.player.combatScene.variant-in1,2','$_.player.combatScene.variant-in5,6').replace('$_.movement.combatVariant-in1,2','$_.movement.combatVariant-in5,6')
marker="   $lastEnd=($validated|Measure-Object ms -Maximum).Maximum"
assert marker in s
s=s.replace(marker,'''   $recovery=@($samples|Where-Object {$_.player.combatSceneValid-and$_.player.combatScene.variant-in15,16})
   if($recovery.Count-lt2){throw 'Native owned hit-recovery scene absent'};$checks.Add('Independent reader validates native owned hit-recovery clock')
   $recoveryPlayed=@($samples|Where-Object {$_.movement.combatVariant-in15,16-and$_.movement.combatWeight-gt.65-and$_.movement.weight-gt.85})
   if($recoveryPlayed.Count-lt2){throw 'Remote hit follow-through absent'};$checks.Add('Remote full-body hit recovery uses existing authored attack follow-through')
   foreach($sample in $recoveryPlayed){if($sample.movement.combatAuthoredAmount-lt$sample.movement.combatRecoveryStart-or$sample.movement.combatAuthoredAmount-gt1){throw 'Recovery authored phase outside continuation'}}
   $checks.Add('Hit recovery remains inside the preserved attack follow-through interval')
   $lastEnd=(@($validated)+@($recovery)|Measure-Object ms -Maximum).Maximum''')
s=s.replace('Ordinary sword input produces native three-actor targeted slash','Ordinary crouched sword input produces native three-actor targeted slash')
s=s.replace('Native ordinary RT250ms facing guard2; validated target-bearing sword animation, no shared victim outcome.','Native crouched RT250ms facing guard2; target-bearing slash and owned recovery, no shared victim outcome.')
s=s.replace('Read-only native source, unchanged protocol65/bank65/353clips.','Read-only native source, protocol66 and unchanged bank65/353clips; owned hit follow-through and native stance recovery.')
(N/'Test-CrouchCombat-v617.ps1').write_text(s)
print('Prepared616 existing-bank build/deploy and617 crouched hit-recovery test. No new authoring pipeline.')
