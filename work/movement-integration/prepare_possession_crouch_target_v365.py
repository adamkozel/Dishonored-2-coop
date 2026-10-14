from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionTargetFxView-v355.ps1').read_text()
s=s.replace('possession-bank-manifest-v334','possession-crouch-bank-manifest-v363').replace('possession-banks-v334/corvo-possession-bank.bin','possession-crouch-banks-v363/corvo-possession-crouch-bank.bin')
s=s.replace('version-eq52','version-eq53').replace('version-ne52','version-ne53').replace('bank52/316','bank53/318').replace('bank52','bank53').replace('protocol50','protocol51').replace('315 prior clip','316 prior clip')
a="   I host 255 1000;$cast=$true;"
assert s.count(a)==1
s=s.replace(a,'''   & "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500
   $low=Q host '--player-states';$low|ConvertTo-Json -Depth 10|Set-Content "$out/crouch-before-cast.json"
   if(!$low.defaultMode-or$low.stance-ne1-or!$low.motionValid-or$low.motion-ne1-or!(Q host '--player-sword').equipped){throw 'Native crouched armed target context absent'}
'''+a)
a=" $checks.Add('Strict probe observes entry/aim and successful release400ms with EnterPossessedHost26/internal0 and exact NPC target')"
assert a in s
s=s.replace(a,a+'''
 $nativeLow=@($accepted|Where-Object {$_.stance-eq1-and$_.variant-in26,27})
 $peerLow=@($rows|Where-Object {$_.movement.stanceMix-gt.95-and$_.movement.powerVariant-in26,27-and$_.movement.powerWeight-gt.5})
 [ordered]@{nativeCrouchedSamples=$nativeLow.Count;peerCrouchedSamples=$peerLow.Count;releaseStances=@($accepted|Where-Object variant -eq 28|ForEach-Object stance|Sort-Object -Unique)}|ConvertTo-Json|Set-Content "$out/crouch-transfer-summary.json"
 if($nativeLow.Count-lt3-or$peerLow.Count-lt3){throw 'Crouched target aim not observed on both source and peer'}
 $checks.Add('Native crouched target aim reaches peer before successful host release/occupation')
''')
s=s.replace('interruption/alternate contexts pending.','Starts crouched before cast; records actual release stance and validates subsequent standing native exit. Other alternate contexts and shared NPC outcomes remain pending.')
(P/'Test-PossessionCrouchTarget-v365.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace("'PossessionDrawNetwork','PossessionObserve'","'PossessionDrawNetwork','PossessionCrouchTarget','PossessionObserve'")
a="  'PossessionObserve'{";assert s.count(a)==1
s=s.replace(a,"  'PossessionCrouchTarget'{& \"$root/work/natural-moveset/Test-PossessionCrouchTarget-v365.ps1\" -Take $Take -Pitch $Pitch}\n"+a);p.write_text(s)
print('Crouched target transfer test prepared; runtime pending')
