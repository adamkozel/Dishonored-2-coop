from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(W/'Deploy-Kick-v625.ps1').read_text().replace('KickDeploy625','HeavyDeploy633').replace('stage_kick_v625.py','stage_heavy_v633.py');(W/'Deploy-Heavy-v633.ps1').write_text(s)
s=(W/'stage_kick_v625.py').read_text().replace("A=W/'candidate-kick-v625';P=W/'candidate-crouch-stance-v620'","A=W/'candidate-heavy-v633';P=W/'candidate-kick-v625'")
s=s.replace('v625.log','v633.log').replace('prepare_kick_v625','prepare_heavy_v633').replace('Build-Kick-v625','Build-Heavy-v633').replace('Deploy-Kick-v625','Deploy-Heavy-v633').replace('Test-NativeKick-v626','Test-CrouchedHeavy-v634').replace('kick-banks-v625','heavy-banks-v633')
s=s.replace('protocol=67,bankVersion=66,clips=354','protocol=68,bankVersion=67,clips=357')
a=s.index("scope='");b=s.index("')",a);s=s[:a]+"scope='Exact native629/630 crouched heavy release18 with role-specific scene/duration. Existing complete-body heavy50/51/52 adapted through original crouch pipeline into354/355/356. Prior354 unchanged, protocol68/bank67. Native roots/events/outcomes untouched; ordinary input verification pending.'"+s[b+1:]
(W/'stage_heavy_v633.py').write_text(s)
s=(N/'Observe-NativeHeavy-v630.ps1').read_text().replace('NativeHeavy630','CrouchedHeavy634').replace('candidate-kick-v625','candidate-heavy-v633').replace('archived625','archived633').replace('kick-bank-manifest-v625','heavy-bank-manifest-v633').replace('$m.version-eq66','$m.version-eq67').replace('$m.version-ne66','$m.version-ne67')
# Readiness after camera/capture setup, reusing the proven627 lifetime gate.
ref=(N/'Test-CrouchRegression-v627.ps1').read_text();a=ref.index('     # Capture startup');b=ref.index('     Start-Sleep -Milliseconds 90;',a)
ready=ref[a:b].replace('$m.version-eq66','$m.version-eq67').replace('$m.version-ne66','$m.version-ne67')
s=s.replace('     Start-Sleep -Milliseconds 90;',ready+'     Start-Sleep -Milliseconds 90;')
a=s.index("   if($combat.Count-lt2)");b=s.index("   C $peer 'coop_movement_bank off'",a)
s=s[:a]+'''   $release=@($samples|Where-Object {$_.player.combatSceneValid-and$_.player.combatScene.variant-eq18})
   $charge=@($samples|Where-Object {$_.player.combatSceneValid-and$_.player.combatScene.variant-eq7})
   if($release.Count-lt2-or$charge.Count-lt2){throw 'Owned native charge and crouched release absent'};$checks.Add('Independent reader validates ordinary crouched charge and exact role-specific release')
   $expectedDuration=if($source-eq'host'){1000}else{833}
   if(@($release|Where-Object {$_.player.combatScene.durationMs-ne$expectedDuration-or$_.player.stance-ne1}).Count){throw 'Release contract differs'};$checks.Add('Native crouched release duration and stance retained')
   foreach($phase in 1,2){
    $present=@($samples|Where-Object {$_.movement.combatVariant-eq7-and$_.movement.combatPhase-eq$phase-and$_.movement.combatWeight-gt.65-and$_.movement.stanceMix-gt.85-and$_.movement.weight-gt.85})
    if(!$present.Count){throw "Remote crouched charge phase$phase absent"}
   };$checks.Add('Remote body performs both crouched charge entry and hold')
   $played=@($samples|Where-Object {$_.movement.combatVariant-eq18-and$_.movement.combatClip-eq356-and$_.movement.combatWeight-gt.65-and$_.movement.weight-gt.85})
   if($played.Count-lt2){throw 'Remote full-body crouched release absent'};$checks.Add('Native Steam source semantics drive full-body crouched heavy release')
   if(!@($release|Where-Object {$_.sword.equipped-and$_.sword.started}).Count){throw 'Native sword missing'};$checks.Add('Native sword remains equipped during crouched heavy attack')
   $lastEnd=($release|Measure-Object ms -Maximum).Maximum
   if(!@($samples|Where-Object {$_.ms-gt($lastEnd+400)-and$_.movement.combatWeight-lt.05-and$_.movement.weight-gt.85-and[math]::Abs($_.movement.stanceMix-$_.player.stance)-lt.15}).Count){throw 'Native stance recovery absent'};$checks.Add('Native stance locomotion resumes after heavy release')
   $aligned=@($played|Where-Object {$_.peer.probeFollowEnabled-and[math]::Abs($_.peer.probeRenderPosition[0]-$_.player.position[0])-lt.3-and[math]::Abs($_.peer.probeRenderPosition[1]-$_.player.position[1])-lt.3})
   if(!$aligned.Count){throw 'Independent root absent'};$checks.Add('Independent network root retained through crouched heavy attack')
   [ordered]@{chargeSamples=$charge.Count;releaseSamples=$release.Count;remoteReleaseSamples=$played.Count;durationMs=$expectedDuration;scope='Stationary crouched charge/hold/release. Moving, cancellation, target outcomes and visual polish remain separate.'}|ConvertTo-Json|Set-Content "$out/context-coverage.json"
'''+s[b:]
s=s.replace("scope='Source-only native crouched heavy-attack observation, ordinary RT1500ms. Captures exact source phases/state/target and actual game video. No new crouched heavy binding or shared victim outcome is claimed. All checkpoint inventory/health and14 protected files restored.'","scope='Ordinary crouched heavy charge/hold/release actor presentation, native Steam source clocks, existing adapted full-body clips. Source roots/events/outcomes unchanged. Complete movement, shared victim outcomes and visual acceptance not claimed. Restoration status is recorded in checks and failure.'")
s=s.replace('held/released source phases, no forced scene or new binding claim.','owned charge/hold/release through native Steam, no scene forcing or victim outcome replay.')
(N/'Test-CrouchedHeavy-v634.ps1').write_text(s)
print('Prepared heavy633 archive/deploy and634 ordinary-input test for both roles.')
