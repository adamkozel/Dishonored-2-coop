from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
read=lambda p:p.read_text(encoding='utf-8-sig')
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=read(W/'Build-DoppelgangerVisual-v481.ps1').replace('v481','v490').replace('Build481','Build490').replace('actual322','actual325')
s=s.replace('doppelganger-banks-v407/','doppelganger-crouch-banks-v490/').replace('corvo-doppelganger-bank.bin','corvo-doppelganger-crouch-bank.bin').replace('emily-doppelganger-bank.bin','emily-doppelganger-crouch-bank.bin')
new(W/'Build-DoppelgangerCrouch-v490.ps1',s)
s=read(W/'prepare_doppelganger_fx_v481.py').replace('v481','v490').replace("previous = W / 'candidate-doppelganger-fx-v479'","previous = W / 'candidate-doppelganger-fx-v481'")
s=s.replace('candidate-doppelganger-fx-v490','candidate-doppelganger-crouch-v490').replace('Build-DoppelgangerVisual-v490','Build-DoppelgangerCrouch-v490')
s=s.replace("for directory in ('banks', 'assets'):\n    shutil.copytree(previous / directory, A / directory)","shutil.copytree(R / 'work/natural-moveset/doppelganger-crouch-banks-v490', A / 'banks')\nshutil.copytree(previous / 'assets', A / 'assets')")
s=s.replace('runtimeVerified=False, previous=previous.name,','runtimeVerified=False, previous=previous.name, protocol=52, bankVersion=56, clips=325, cloneWireVersion=4,')
s=s.replace('Native480 exact rigid leaf marker offsets exceed former3m bound. Wire4 keeps10712 bytes; only verified219-rig leaves1/10/17 allow10m, other joints3m. Shared sender/receiver/interpolation validation and regression cases; cadence475 and finite native FX468 retained','489 native Sneaking6 plus UsePower63 and exact466/1200/666ms scenes drive full-body armed crouch clips322-324. Original322 payloads unchanged. Supported feet, native rig lengths, grip/socket; no root/FP/event changes.481 clone marker and FX fixes retained. Runtime and visual comparison pending')
s=s.replace("for p in [Path(__file__), W / 'Build-DoppelgangerCrouch-v490.ps1', *logs]:", "for p in [Path(__file__), W / 'Build-DoppelgangerCrouch-v490.ps1', R/'work/natural-moveset/build_doppelganger_crouch_v490.py', R/'work/natural-moveset/doppelganger-crouch-bank-manifest-v490.json', R/'work/natural-moveset/doppelganger-crouch-source-manifest-v490.json', W/'doppelganger-crouch-authoring-v490.log', *logs]:")
s=s.replace('DoppelgangerFxDeploy481','DoppelgangerCrouchDeploy490').replace('Deploy-DoppelgangerFx-v490','Deploy-DoppelgangerCrouch-v490')
new(W/'archive_doppelganger_crouch_v490.py',s)
s=read(N/'Test-DoppelgangerFx-v485.ps1').replace('doppelganger-banks-v407/','doppelganger-crouch-banks-v490/').replace('emily-doppelganger-bank.bin','emily-doppelganger-crouch-bank.bin').replace('bank55/322','bank56/325').replace('version-eq55','version-eq56').replace('version-ne55','version-ne56')
s=s.replace('setviewpos -47.39 -156.87 68.1 90 0','setviewpos -44.39 -153.27 68.1 203 0')
needle=" $checks.Add('Exact Emily Doppelganger item acquired through native inventory API; no direct spawn, AI or damage invocation')"
assert s.count(needle)==1
s=s.replace(needle,needle+'''
 & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500
 $low=Q client '--player-states';$low|ConvertTo-Json -Depth 10|Set-Content "$out/crouch-before.json"
 if($low.stance-ne1-or$low.motion-ne1-or!$low.motionValid){throw 'Native crouch not grounded before cast'}
''')
needle=" $checks.Add('All three exact native phases with reciprocal Emily arm/body ownership drive remote full-body phases')"
assert s.count(needle)==1
s=s.replace(needle,needle+'''
 $low=@($rows|Where-Object {$_.movement.powerVariant-in29,30,31-and$_.movement.powerWeight-gt.5})
 if(!$low.Count-or@($low|Where-Object {$_.player.stance-ne1-or$_.movement.stanceMix-lt.95}).Count){throw 'Crouched source and complete-body remote cast disagree'}
 [ordered]@{samples=$low.Count;minimumStance=($low.movement.stanceMix|Measure-Object -Minimum).Minimum;variants=@($low.movement.powerVariant|Sort-Object -Unique)}|ConvertTo-Json|Set-Content "$out/crouch-verification.json"
 $checks.Add('All three authored full-body phases retain native crouched stance over Steam')
''')
s=s.replace("if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}", "if($m.powerWeight-gt.02-or$m.weight-lt.9-or$m.stanceMix-lt.95-or$stream.remoteState-ne0){throw 'Remote cast did not recover to crouch'}")
s=s.replace('Remote body recovers to standing with native sword selection retained','Remote body recovers to crouch with native sword selection retained')
s=s.replace('Standing armed native Emily Doppelganger phases','Crouched armed native Emily Doppelganger phases')
s=s.replace('Native Emily LT1800 then release at grounded floor','Native crouched Emily LT1800 then release at grounded floor')
new(N/'Test-DoppelgangerCrouch-v491.ps1',s)
w=read(W/'Run-DoppelgangerFx-v480.ps1').replace('doppelganger-fx-v480','doppelganger-crouch-v491').replace('Test-DoppelgangerFx-v480','Test-DoppelgangerCrouch-v491')
new(W/'Run-DoppelgangerCrouch-v491.ps1',w)
print('Prepared490 build/archive and491 paired crouched cast/body/clone/effect regression')
