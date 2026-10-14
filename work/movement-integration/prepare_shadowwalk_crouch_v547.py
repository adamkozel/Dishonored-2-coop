from pathlib import Path

R = Path(__file__).resolve().parents[2]
W = R / 'work/movement-integration'
N = R / 'work/natural-moveset'
s = (N / 'Observe-NativeShadowWalk-v522.ps1').read_text(encoding='utf-8-sig')

def once(before, after):
    global s
    assert s.count(before) == 1, before
    s = s.replace(before, after)

once("try{\n foreach($r in 'host','client'){", """try{
 if($initialDll-ne'E7C012BAE02C2F7ADCCE655142C965CA1EE8F9969BCA732C24E086C39662174A'-or$initialProbe-ne'EEE9BDDD20B69B14E47CFC9EA9CF4DAFDE81FFA7FB1C44563494551B65CFBCAE'){throw 'Expected verified533 proxy and read-only541 probe'}
 foreach($r in 'host','client'){""")
once(" $context=Q client '--player-states';$sword=Q client '--player-sword'", """ & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 100|Out-Null
 Start-Sleep -Milliseconds 800
 $context=Q client '--player-states';$sword=Q client '--player-sword'""")
once("if($context.stance-ne0-or!$context.motionValid-or$context.motion-ne0-or!$sword.equipped){throw 'Native grounded armed context not established'}", "if($context.stance-ne1-or!$context.motionValid-or$context.motion-ne0-or!$sword.equipped){throw 'Native grounded crouched armed context not established'}")
once(" $sceneRows=@($strict|Where-Object sceneValid)\n foreach($variant in 32,33,34){if(!@($sceneRows|Where-Object variant -eq $variant).Count){throw \"Exact native scene variant $variant not observed\"}}\n $sceneRows|Group-Object variant,durationMs|Select-Object Name,Count|ConvertTo-Json|Set-Content \"$out/strict-scene-observations.json\"", """ $sceneRows=@($strict|Where-Object sceneValid)
 [ordered]@{accepted=@($sceneRows|Group-Object variant,durationMs|Select-Object Name,Count);refused=@($strict|Where-Object {$_.valid-and!$_.sceneValid}|Group-Object phase|Select-Object Name,Count);scope='Diagnostic coverage only. Existing standing-context scene reader may intentionally refuse unverified crouched entry/exit. Raw reciprocal native scenes are recorded independently.'}|ConvertTo-Json -Depth 8|Set-Content "$out/strict-scene-observations.json"
""")
once(" $checks.Add('Owned native seven-binding entry and exit scenes plus sustained form observed; native LT request ends form')", " $checks.Add('Native crouched source lifecycle observed; strict presentation-reader coverage recorded without assuming standing scene equivalence')")
once("scope='Source-only native Shadow Walk acquisition, normal LT activation, conditional bounded movement, requested exit, scene/state/equipment observations. No remote form/VFX/gameplay binding. Checkpoint and protected saves must restore.'", "scope='Source-only crouched Shadow Walk activation, conditional bounded forward/back movement, requested exit and reciprocal native scene/state/equipment observations. Strict presentation-reader coverage measured separately; no remote crouched animation acceptance. Verified533 and probe541 pinned. Checkpoint and protected saves must restore.'")
target = N / 'Observe-NativeShadowWalkCrouch-v547.ps1'
assert not target.exists()
target.write_text(s, encoding='utf-8')
s = (W / 'Run-ShadowWalkMaterialSource-v542.ps1').read_text(encoding='utf-8-sig')
s = s.replace('native-shadowwalk-material-source-v542', 'native-shadowwalk-crouch-source-v547').replace('Observe-NativeShadowWalk-v522', 'Observe-NativeShadowWalkCrouch-v547').replace("operation='ShadowWalkObserve'", "operation='ShadowWalkCrouchObserve547'")
(W / 'Run-ShadowWalkCrouchSource-v547.ps1').write_text(s, encoding='utf-8')
print('Prepared source-only crouched native Shadow Walk observation547')
