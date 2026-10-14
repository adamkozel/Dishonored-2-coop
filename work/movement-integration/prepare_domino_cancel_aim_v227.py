from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeCancel-v226.ps1').read_text(encoding='utf-8-sig')
old=" $dirty=$true;C client 'setviewpos -44.39 -153.27 68.1 90 0'"
new=''' $dirty=$true
 $initialCatalog=Q client '--world-npcs'
 $initialTargets=@($initialCatalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5})
 if($initialTargets.Count-ne1){throw 'Exact first guard unavailable before choosing source floor'}
 $anchor=$null
 foreach($candidate in @(@(-44.39,-153.27,66.4036484),@(-48.59,-155.07,66.4370575))){
  $distance=[Math]::Pow([double]$initialTargets[0].position[0]-$candidate[0],2)+[Math]::Pow([double]$initialTargets[0].position[1]-$candidate[1],2)
  if($distance-ge9-and$distance-le225){$anchor=$candidate;break}
 }
 if(!$anchor){throw 'Neither measured upper-floor anchor has a three-metre targeting gap'}
 $firstX=$anchor[0];$firstY=$anchor[1];$ci=[Globalization.CultureInfo]::InvariantCulture
 C client ('setviewpos {0} {1} 68.1 90 0'-f$firstX.ToString('F4',$ci),$firstY.ToString('F4',$ci))
 Start-Sleep -Milliseconds 750
 $floors=@();foreach($n in 1..3){$floors+=Q client '--player-states';Start-Sleep -Milliseconds 100}
 [ordered]@{anchor=$anchor;target=$initialTargets[0];samples=$floors}|ConvertTo-Json -Depth 10|Set-Content "$out/first-source-floor.json"
 foreach($floor in $floors){if(!$floor.motionValid-or$floor.motion-ne0-or$floor.stance-ne0-or[Math]::Abs([double]$floor.position[2]-$anchor[2])-gt.03){throw 'First source not grounded at measured anchor'};for($a=0;$a-lt3;$a++){if([Math]::Abs([double]$floor.position[$a]-[double]$floors[0].position[$a])-gt.002){throw 'First source floor unstable'}}}
'''
assert old in s;s=s.replace(old,new)
start=s.index('    $target=$eligible[0];$yaw=');end=s.index('\n   }',start)
s=s[:start]+'''    $target=$eligible[0];$distance=[Math]::Pow([double]$target.position[0]-$firstX,2)+[Math]::Pow([double]$target.position[1]-$firstY,2)
    if($distance-lt4){throw 'First guard moved within two metres of source'}
    $yaw=[Math]::Atan2([double]$target.position[1]-$firstY,[double]$target.position[0]-$firstX)*180/[Math]::PI
    C client ('setviewpos {0} {1} 68.1 {2} 0'-f$firstX.ToString('F4',$ci),$firstY.ToString('F4',$ci),$yaw.ToString('F4',$ci))
    Start-Sleep -Milliseconds 600
    & "$PSScriptRoot/Aim-NativeDominoGuard-v218.ps1" -Role client -Evidence "$out/native-guard-aim.json"
    [ordered]@{target=$target;yaw=$yaw;anchor=$anchor;placement='Measured grounded source anchor with at least two metres target separation and independent native look feedback; no NPC mutation'}|ConvertTo-Json -Depth 6|Set-Content "$out/aim-guard.json"
'''+s[end:]
# The capture has forty seconds of content even on early setup failures.
s=s.replace('$peerCapture.WaitForExit(18000)','$peerCapture.WaitForExit(50000)').replace('$capture.WaitForExit(18000)','$capture.WaitForExit(50000)')
(P/'Test-DominoNativeCancel-v227.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1'
s=runner.read_text(encoding='utf-8-sig').replace('Test-DominoNativeCancel-v226.ps1','Test-DominoNativeCancel-v227.ps1')
runner.write_text(s,encoding='utf-8')
