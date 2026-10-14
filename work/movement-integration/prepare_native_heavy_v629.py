from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Observe-NativeKick-v623.ps1').read_text().replace('NativeKick623','NativeHeavy629').replace('candidate-crouch-stance-v620','candidate-kick-v625').replace('archived620','archived625').replace('lethal-bank-manifest-v604','kick-bank-manifest-v625').replace('version-eq65','version-eq66').replace('version-ne65','version-ne66')
s=s.replace("name='close-combat';yaw=90;stance=0", "name='crouched-heavy';yaw=90;stance=1")
s=s.replace('-RightTrigger 255 -DurationMs 250','-RightTrigger 255 -DurationMs 1500')
# The known central-room combat fixture avoids victim-dependent outcomes.
lines=s.splitlines()
for i,l in enumerate(lines):
 if "C $source ('setviewpos" in l:lines[i]="   C $source 'setviewpos -33.5 -153 68.1 0 0'"
s='\n'.join(lines)+'\n';a=s.index('     $cameraTrials=');b=s.index('     Start-Sleep -Milliseconds 90;',a)
s=s[:a]+'''     C $peer 'setviewpos -30.5 -149 68.1 233.13 0'
     & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 12 -Evidence "$out/observer-camera.json" -Quick
     C $source 'setviewpos -33.5 -153 68.1 0 0'
     Start-Sleep -Milliseconds 400
     $nativeBefore=Q $source '--player-states';$nativeBefore|ConvertTo-Json -Depth 8|Set-Content "$out/native-before-input.json"
     if($nativeBefore.stance-ne1){throw 'Crouched central-room source required'}
'''+s[b:]
s=s.replace('Ordinary close-range RT native combat observed; exact scene contract needs inspection','Ordinary crouched held-RT native combat observed; exact phase contract needs inspection')
s=s.replace('Ordinary close-range RT250ms facing guard2; reproduce611 native kick context if selected, no forced scene or new binding claim.','Ordinary crouched RT1500ms in known central-room fixture; held/released source phases, no forced scene or new binding claim.')
s=s.replace('Source-only native close-range combat observation, ordinary RT250ms.','Source-only native crouched heavy-attack observation, ordinary RT1500ms.').replace('No new remote kick binding','No new crouched heavy binding')
(N/'Observe-NativeHeavy-v629.ps1').write_text(s)
print('Prepared629 native crouched heavy source capture using existing central-room fixture.')
