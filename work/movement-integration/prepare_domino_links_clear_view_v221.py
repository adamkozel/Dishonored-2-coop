from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeDominoLinks-v219.ps1').read_text(encoding='utf-8-sig')
a=s.index('   $target2=$lower[0];');b=s.index('   Start-Sleep -Milliseconds 350;I client 255 1800;',a)
s=s[:a]+'''   $target2=$lower[0];$sourceX=[double]$target2.position[0];$sourceY=[double]$target2.position[1]-3.5;$sourceZ=[double]$target2.position[2]+1.7
   $ci=[Globalization.CultureInfo]::InvariantCulture
   C client ('setviewpos {0} {1} {2} 90 0'-f$sourceX.ToString('F4',$ci),$sourceY.ToString('F4',$ci),$sourceZ.ToString('F4',$ci))
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 0 -Evidence "$out/second-target-pitch.json" -Quick
   Start-Sleep -Milliseconds 550;$ground=Q client '--player-states';$ground|ConvertTo-Json -Depth 8|Set-Content "$out/second-source-floor.json"
   if(!$ground.motionValid-or$ground.motion-ne0-or[Math]::Abs([double]$ground.position[2]-[double]$target2.position[2])-gt.2){throw 'Source did not settle on same native floor as second guard'}
   [ordered]@{target=$target2;source=@($sourceX,$sourceY,$sourceZ);yaw=90;pitch=0;note='Source-only test moves beside exact second guard on its current floor, validates native grounded state, then casts with native input. No active body stream and no NPC mutation. Movement fixture owns reversible AI holds.'}|ConvertTo-Json -Depth 6|Set-Content "$out/second-target-aim.json"
'''+s[b:]
(P/'Observe-NativeDominoLinks-v221.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1'
s=runner.read_text(encoding='utf-8-sig');assert 'Observe-NativeDominoLinks-v219.ps1' in s
runner.write_text(s.replace('Observe-NativeDominoLinks-v219.ps1','Observe-NativeDominoLinks-v221.ps1'),encoding='utf-8')
