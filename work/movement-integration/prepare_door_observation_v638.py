from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Observe-NativeHeavy-v630.ps1').read_text().replace("$Take='native-guard-choke-v574'","$Take='native-door-corvo-v638'").replace('NativeHeavy630','NativeDoor638').replace('candidate-kick-v625','candidate-heavy-v635').replace('archived625','archived635').replace('kick-bank-manifest-v625','heavy-bank-manifest-v633').replace('$m.version-eq66','$m.version-eq67').replace('$m.version-ne66','$m.version-ne67')
s=s.replace("name='crouched-heavy';yaw=90;stance=1;distance=1.15","name='door-tap';yaw=180;stance=0;distance=1.0")
a=s.index('   $guard=@(');b=s.index("   $character=",a)
s=s[:a]+'''   C $peer 'setviewpos -32.5 -151 68.1 217 0'
   C $source 'setviewpos -35.9 -153.55 68.1 180 0'
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$out/source-camera.json" -Quick
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 10 -Evidence "$out/observer-camera.json" -Quick
   $components=Q $source '--components' 1
   $components|ConvertTo-Json -Depth 8|Set-Content "$out/player-components-before.json"
   $sensors=@($components.components|Where-Object {$_.type-match'(?i)(PlayerSensor.*(Interaction|Use)|PlayerFocus)'})
   foreach($r in 'host','client'){@((Q $r '--entities').entities|Where-Object {$_.type-match'Door'})|ConvertTo-Json -Depth 8|Set-Content "$out/doors-before-$r.json"}
'''+s[b:]
s=s.replace("'-t','30'","'-t','20'").replace("'29000'","'19500'").replace('$watch.ElapsedMilliseconds-lt28000','$watch.ElapsedMilliseconds-lt19000').replace('$sent=$false','$sent=$false;$second=$false;$inputs=[Collections.Generic.List[object]]::new()')
a=s.index('     # The client guard');b=s.index('    $samples.Add(',a)
s=s[:a]+'''     $nativeBefore=Q $source '--player-states';$nativeBefore|ConvertTo-Json -Depth 8|Set-Content "$out/native-before-input.json"
     if($nativeBefore.stance-ne0){throw 'Standing source required for initial door observation'}
     I $source X 250;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='X250';attempt=1});$sent=$true
    }
    if(!$second-and$watch.ElapsedMilliseconds-ge7000){I $source X 250;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='X250';attempt=2});$second=$true}
'''+s[b:]
a=s.index('   $combat=@(');b=s.index("   C $peer 'coop_movement_bank off'",a)
s=s[:a]+'''   $allScenes|ConvertTo-Json -Depth 9|Set-Content "$out/native-scenes.json"
   $inputs|ConvertTo-Json|Set-Content "$out/inputs.json"
   $samples|ForEach-Object { [pscustomobject]@{ms=$_.ms;stance=$_.player.stance;mode=$_.player.mode;active=$_.player.active} }|ConvertTo-Json -Depth 9|Set-Content "$out/native-action-states.json"
   foreach($r in 'host','client'){@((Q $r '--entities').entities|Where-Object {$_.type-match'Door'})|ConvertTo-Json -Depth 8|Set-Content "$out/doors-after-$r.json"}
   if($inputs.Count-ne2-or$samples.Count-lt40){throw 'Door observation input/sampling incomplete'}
   $checks.Add('Two ordinary short-X requests recorded at the known native office door; scene/state/door results require inspection')
'''+s[b:]
s=s.replace("$checks.Add('Ordinary crouched RT1500ms in known central-room fixture; held/released source phases, no forced scene or new binding claim.')","$checks.Add('Native source observation only; no forced door state, native scene, inventory grant or new binding claim.')")
a=s.index("scope='Source-only native crouched");b=s.index("'}|ConvertTo-Json",a)
s=s[:a]+"scope='Source-only native office door short-X observation. Both actual views, scene/state trace and before/after door entities retained. Successful script execution does not prove an open/close action or new remote binding. Restoration is established only by checks/failure.'"+s[b+1:]
(N/'Observe-NativeDoor-v638.ps1').write_text(s)
print('Prepared638: existing door fixture, two ordinary taps, source states/scenes and both door snapshots; no new binding claim.')
