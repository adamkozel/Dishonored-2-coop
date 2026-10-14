from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(W/'Deploy-CrouchStance-v620.ps1').read_text().replace('CrouchStanceDeploy620','KickDeploy625').replace('stage_crouch_stance_v620.py','stage_kick_v625.py');(W/'Deploy-Kick-v625.ps1').write_text(s)
s=(W/'stage_crouch_stance_v620.py').read_text().replace("A=W/'candidate-crouch-stance-v620';P=W/'candidate-crouch-combat-v616'","A=W/'candidate-kick-v625';P=W/'candidate-crouch-stance-v620'")
s=s.replace('v620.log','v625.log').replace('prepare_crouch_stance_v620','prepare_kick_v625').replace('Build-CrouchStance-v620','Build-Kick-v625').replace('Deploy-CrouchStance-v620','Deploy-Kick-v625').replace('Test-CrouchCombat-v621','Test-NativeKick-v626')
s=s.replace("banks=rd(P/'identity.json')['banks'];assert len(banks)==2", "banks=[dict(path=str(p.relative_to(R)),sha256=sha(p)) for p in sorted((R/'work/natural-moveset/kick-banks-v625').glob('*.bin'))];assert len(banks)==2")
s=s.replace('protocol=66,bankVersion=65,clips=353','protocol=67,bankVersion=66,clips=354')
a=s.index("scope='");b=s.index("')",a);s=s[:a]+"scope='Native611 exact throwhit/kick800 owned right arm plus two references to player camera/body. Bank66/354 appends existing kick donor; prior353 payloads unchanged. Ground-crouched source only, protocol67. Source outcomes/events untouched; ordinary-input runtime verification pending.'"+s[b+1:]
(W/'stage_kick_v625.py').write_text(s)
s=(N/'Test-CrouchCombat-v621.ps1').read_text().replace('CrouchCombat621','NativeKick626').replace('candidate-crouch-stance-v620','candidate-kick-v625').replace('archived620','archived625')
s=s.replace('lethal-bank-manifest-v604','kick-bank-manifest-v625').replace('$m.version-eq65','$m.version-eq66').replace('$m.version-ne65','$m.version-ne66').replace('$readyBank.version-eq65','$readyBank.version-eq66').replace('$readyBank.version-ne65','$readyBank.version-ne66')
s=s.replace('crouched-slash','close-kick').replace("'sword_slash_sneak_(right|left)\\.midnightscene$'","'/throwhit\\.midnightscene$'")
s=s.replace('$_.player.combatScene.variant-in5,6-and$_.player.combatScene.targetHandle-eq$g.handle','$_.player.combatScene.variant-eq17')
s=s.replace('$_.movement.combatVariant-in5,6','$_.movement.combatVariant-eq17')
a=s.index('   $recovery=@(');b=s.index("   if(!@($samples|Where-Object {$_.ms-gt",a)
s=s[:a]+'''   [ordered]@{ownedKickSamples=$validated.Count;remoteKickSamples=$played.Count;sourceStances=@($validated.player.stance|Sort-Object -Unique);required='Exact native kick800, owned arm and two player references'}|ConvertTo-Json|Set-Content "$out/context-coverage.json"
   $lastEnd=($validated|Measure-Object ms -Maximum).Maximum
'''+s[b:]
# Reproduce the observed brief native collision/auto-crouch context without
# altering targets or source state. Stream readiness is established first.
marker='     Start-Sleep -Milliseconds 90;& "$lab/Send-D2Input.ps1"'
assert marker in s;s=s.replace(marker,'''     C $source ('setviewpos {0} {1} 68.1 {2} 0' -f $px.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$py.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$look.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
'''+marker)
s=s.replace('Native target-bearing sword scene not observed','Native kick scene not selected by ordinary input').replace('Targeted combat reader did not validate','Exact native kick ownership reader did not validate').replace('Remote targeted attack animation absent','Remote native kick animation absent')
s=s.replace('Ordinary crouched sword input produces native three-actor targeted slash','Ordinary input selects the exact native three-group kick800 scene').replace('Independent reader validates exact player, right arm and native enemy target','Independent reader validates owned right arm and two references to the same player').replace('Native crouched RT250ms facing guard2; target-bearing slash, native stance recovery and optional owned recovery when observed, no shared victim outcome.','Native close-range RT250ms; exact owned kick and native recovery, no shared victim outcome.')
s=s.replace('Existing combat pipeline extended to observed ordinary target-bearing sword scenes. Read-only native source, protocol66 and unchanged bank65/353clips; native stance recovery and optional owned hit follow-through. A missing optional scene is not counted as verified.','Existing combat pipeline binds exact native kick800. Protocol67 and bank66/354; prior353 unchanged. This take requires native17 selection, remote playback and stance recovery.')
(N/'Test-NativeKick-v626.ps1').write_text(s)
print('Prepared625 deploy and626 ordinary-input kick test; exact selection required, no clip forcing.')
