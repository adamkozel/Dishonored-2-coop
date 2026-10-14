from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionCrouchNetwork-v363.ps1').read_text()
s=s.replace('[int]$Pitch=30)',"[int]$Pitch=30,[ValidateSet('standing','crouched')][string]$Stance='standing')")
s=s.replace(" & \"$lab/Send-D2Input.ps1\" -Role host -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500",''' $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
 & "$lab/Send-D2Input.ps1" -Role host -Button X -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1200
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/holster-look-return.json" -Quick''')
s=s.replace('$context.stance-ne1-or!$context.motionValid-or$context.motion-ne1-or!(Q host', '$context.stance-ne$expectedStance-or!$context.motionValid-or$context.motion-ne$expectedStance-or(Q host')
s=s.replace("'Exact native armed crouch not established'","'Exact native initially holstered stance not established'")
s=s.replace("'Native Sneaking6 and armed sword verified before crouched cast'","'Native stance and holstered sword verified before draw/cast input'")
s=s.replace('-and$m.presentedDrawn-and$stream.ready','-and!$m.presentedDrawn-and$stream.ready').replace('-or!$m.presentedDrawn-or!$stream.ready','-or$m.presentedDrawn-or!$stream.ready')
s=s.replace('$_.player.stance-ne1-or$_.movement.stanceMix-lt.95', '$_.player.stance-ne$expectedStance-or[Math]::Abs($_.movement.stanceMix-$expectedStance)-gt.05')
s=s.replace("'Crouched source/remote stance diverged during cast'","'Source/remote stance diverged during cast'")
s=s.replace("'Both native source and remote body remain crouched throughout sampled cast phases'","'Source and remote retain matching native stance during draw/cast'")
s=s.replace('recovers to crouched idle','recovers to its original stance').replace('recover to crouched idle','recover to its original stance')
a=" $checks.Add('Both observed native phases with reciprocal Corvo left-arm/equipment ownership drive remote full-body phases')"
assert a in s
s=s.replace(a,a+'''
 $drawVariant=if($expectedStance){3}else{1};$drawDuration=if($expectedStance){1133}else{1066}
 $nativeDraw=@($scenes|Where-Object {$_.swordDrawVariant-eq$drawVariant-and$_.swordDrawDurationMs-eq$drawDuration})
 $peerDraw=@($rows|Where-Object {$_.movement.equipmentVariant-eq$drawVariant-and$_.movement.equipmentWeight-gt.5-and$_.movement.powerWeight-gt.5-and$_.movement.powerVariant-in26,27})
 [ordered]@{nativeSamples=$nativeDraw.Count;peerSamples=$peerDraw.Count;nativeHandles=@($nativeDraw.swordDrawHandle|Sort-Object -Unique);peerAmounts=@($peerDraw.movement.equipmentAmount);nativeTimes=@($nativeDraw.swordDrawTimeMs);duration=$drawDuration}|ConvertTo-Json -Depth 6|Set-Content "$out/draw-overlap-summary.json"
 if($nativeDraw.Count-lt3-or$peerDraw.Count-lt3-or@($nativeDraw|Where-Object {$_.swordDrawHandle-eq$_.handle}).Count){throw 'Independent owned draw/cast overlap not demonstrated'}
 if(!@($peerDraw|Where-Object {!$_.movement.presentedDrawn}).Count-or!@($peerDraw|Where-Object {$_.movement.presentedDrawn}).Count){throw 'Remote sword contact transition absent during cast'}
 $checks.Add('Independent owned native sword draw overlaps remote Possession and crosses native-authored sword contact timing')
''')
s=s.replace('protocol50','protocol51')
s=s.replace("scope='Crouched armed native Corvo", "stance=$Stance;scope='Initially holstered native Corvo")
s=s.replace('native draw overlap, crouched successful-host release','crouched successful-host release')
(P/'Test-PossessionDrawNetwork-v364.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace("'PossessionCrouchNetwork','PossessionObserve'","'PossessionCrouchNetwork','PossessionDrawNetwork','PossessionObserve'")
a="  'PossessionObserve'{";assert s.count(a)==1
s=s.replace(a,"  'PossessionDrawNetwork'{& \"$root/work/natural-moveset/Test-PossessionDrawNetwork-v364.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance}\n"+a);p.write_text(s)
print('Initially holstered native draw/cast runtime harness prepared')
