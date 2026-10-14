"""Reuse bank65 and the existing native build/deploy/test pipeline."""
from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
s=(W/'Build-Lethal-v604.ps1').read_text().replace('v604.log','v610.log').replace('Build604','Build610')
(W/'Build-CombatTarget-v610.ps1').write_text(s)
s=(W/'Deploy-Lethal-v604.ps1').read_text().replace('ChokeDeploy604','CombatTargetDeploy610').replace('stage_lethal_v604.py','stage_combat_target_v610.py')
(W/'Deploy-CombatTarget-v610.ps1').write_text(s)
s=(N/'Test-LethalNetwork-v605.ps1').read_text().replace('LethalNetwork605','CombatTarget611').replace('candidate-lethal-v604','candidate-combat-target-v610').replace('archived604','archived610').replace("name='rear-lethal'","name='targeted-slash'")
for a,b in [('-1.15*$forwardX','+1.15*$forwardX'),('-1.15*$forwardY','+1.15*$forwardY'),('[Math]::Atan2($forwardY,$forwardX)','[Math]::Atan2(-$forwardY,-$forwardX)'),('-.9*$fx','+.9*$fx'),('-.9*$fy','+.9*$fy'),('-.85*$fx','+.85*$fx'),('-.85*$fy','+.85*$fy'),('[Math]::Atan2($fy,$fx)','[Math]::Atan2(-$fy,-$fx)')]:
 assert a in s,a;s=s.replace(a,b)
a=s.index('   $paired=@('); b=s.index('   $receiverHealthAfter=',a)
s=s[:a]+'''   $hits=@($allScenes|Where-Object {$_.state-eq3-and$_.asset-match'sword_slash_rightb?\\.midnightscene$'-and$_.bindingsResearch.Count-eq3})
   $hits|ConvertTo-Json -Depth 9|Set-Content "$out/targeted-scenes.json"
   if($hits.Count-lt2){throw 'Native target-bearing sword scene not observed'};$checks.Add('Ordinary sword input produces native three-actor targeted slash')
   $validated=@($samples|Where-Object {$_.player.combatSceneValid-and$_.player.combatScene.variant-in1,2-and$_.player.combatScene.targetHandle-eq$g.handle})
   if($validated.Count-lt2){throw 'Targeted combat reader did not validate'};$checks.Add('Independent reader validates exact player, right arm and native enemy target')
   if(!@($validated|Where-Object {$_.sword.equipped-and$_.sword.started}).Count){throw 'Sword premise absent'};$checks.Add('Native sword equipped and started during targeted slash')
   $played=@($samples|Where-Object {$_.movement.combatVariant-in1,2-and$_.movement.combatWeight-gt.65-and$_.movement.weight-gt.85})
   if(!$played.Count){throw 'Remote targeted attack animation absent'};$checks.Add('Existing full-body combat clip plays through native Steam source semantics')
   $lastEnd=($validated|Measure-Object ms -Maximum).Maximum
   if(!@($samples|Where-Object {$_.ms-gt($lastEnd+400)-and$_.movement.combatWeight-lt.05-and$_.movement.weight-gt.85}).Count){throw 'Standing recovery absent'};$checks.Add('Native standing recovery restores movement after targeted attack')
   $aligned=@($played|Where-Object {$_.peer.probeFollowEnabled-and[math]::Abs($_.peer.probeRenderPosition[0]-$_.player.position[0])-lt.3-and[math]::Abs($_.peer.probeRenderPosition[1]-$_.player.position[1])-lt.3})
   if(!$aligned.Count){throw 'Targeted combat root follow absent'};$checks.Add('Independent network root retained through target-bearing attack')
'''+s[b:]
s=s.replace('Native ordinary RT250ms behind exact authored guard2; independent source and Steam actor checks, victim/world outcome remains separate.','Native ordinary RT250ms facing guard2; validated target-bearing sword animation, no shared victim outcome.')
a=s.index("scope='Fast rear assassination");b=s.index("'",a+7)
s=s[:a]+"scope='Existing combat pipeline extended to observed ordinary target-bearing sword scenes. Read-only native source, unchanged protocol65/bank65/353clips. Actor presentation only; no native victim events or damage replay. All inventories, health and14 protected files restored."+s[b:]
s=s.replace('PASS_NATIVE_LETHAL','PASS_TARGETED_COMBAT')
(N/'Test-CombatTarget-v611.ps1').write_text(s)
print('Prepared610 build/deploy and611 runtime. Existing353-clip bank reused without copies.')
