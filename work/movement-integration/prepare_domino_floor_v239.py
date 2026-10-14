from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'
s=(P/'Aim-NativeDominoGuard-v234.ps1').read_text(encoding='utf-8-sig')
s=s.replace("-lt.5})", "-lt5})").replace('Exact same-floor tutorial guard unavailable','Exact tutorial guard outside measured five-metre vertical range')
s=s.replace("if($settled-lt2){throw 'Native guard yaw did not converge'}", """if($settled-lt2){throw 'Native guard yaw did not converge'}
 $view=Q '--player';$state=Q '--player-states';$catalog=Q '--world-npcs'
 $target=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq$TargetSpawner})
 if($target.Count-ne1-or$view.nativeViewPosition.Count-ne3){throw 'Native target/view changed before pitch correction'}
 $dx=[double]$target[0].position[0]-[double]$view.nativeViewPosition[0];$dy=[double]$target[0].position[1]-[double]$view.nativeViewPosition[1]
 $dz=[double]$target[0].position[2]+1.15-[double]$view.nativeViewPosition[2]
 $targetPitch=-[Math]::Atan2($dz,[Math]::Sqrt($dx*$dx+$dy*$dy))*180/[Math]::PI
 if([Math]::Abs($targetPitch)-gt60){throw 'Native target elevation exceeds bounded aim range'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch $targetPitch -Evidence ($Evidence+'.pitch.json') -Quick
 $after=Q '--player-states';for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$after.position[$k]-[double]$initial[$k])-gt.01){throw 'Native pitch correction changed source floor'}}
""")
(P/'Aim-NativeDominoGuard-v239.ps1').write_text(s,encoding='utf-8')
s=(P/'Test-DominoFinalSelect-v238.ps1').read_text(encoding='utf-8-sig')
s=s.replace("if(!$ground.motionValid-or$ground.motion-ne0-or[Math]::Abs([double]$ground.position[2]-[double]$target2.position[2])-gt.2){throw 'Source did not settle on same native floor as second guard'}", """if(!$ground.motionValid-or$ground.motion-ne0-or[Math]::Abs([double]$ground.position[2]-[double]$target2.position[2])-gt5){throw 'Source not grounded within measured target elevation range'}
   Start-Sleep -Milliseconds 150;$ground2=Q client '--player-states'
   for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$ground2.position[$k]-[double]$ground.position[$k])-gt.01){throw 'Second source floor still moving'}}
   & "$PSScriptRoot/Aim-NativeDominoGuard-v239.ps1" -Role client -TargetSpawner ld_spawner_guard_tutorial_1 -Evidence "$out/second-native-aim.json"
""")
s=s.replace('Aim-NativeDominoGuard-v234.ps1','Aim-NativeDominoGuard-v239.ps1')
s=s.replace('Source-only test moves beside exact second guard on its current floor, validates native grounded state, then casts with native input.', 'Source-only test moves near exact second guard, validates stable native ground and uses independent native camera/target yaw and elevation feedback. Guard may occupy the stairs above source.')
(P/'Test-DominoFinalSelect-v239.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace('Test-DominoFinalSelect-v238.ps1','Test-DominoFinalSelect-v239.ps1');p.write_text(s,encoding='utf-8')
