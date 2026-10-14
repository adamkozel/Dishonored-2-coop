from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativeBlock-v1004.ps1').read_text().replace('npc-native-block-v1004','npc-native-block-v1008').replace('$watch.ElapsedMilliseconds-lt8500','$watch.ElapsedMilliseconds-lt10500')
needle='try{\n try{$owned=';assert needle in s
s=s.replace(needle,'''function WrappedAngle($x){while($x-gt180){$x-=360};while($x-lt-180){$x+=360};return $x}
try{
 try{$owned=''',1)
needle=' $watch.Restart();$selected=0;$attacking=0';assert needle in s
s=s.replace(needle,''' $v0=Q client '--player'
 & "$lab/Send-D2Input.ps1" -Role client -Button RightShoulder -RightX 20000 -DurationMs 1500|Out-Null
 Start-Sleep -Milliseconds 180
 $v1=Q client '--player'
 & "$lab/Send-D2Input.ps1" -Role client -Button RightShoulder -DurationMs 10000|Out-Null
 $yawRate=(WrappedAngle (([Math]::Atan2($v1.nativeViewOrientation[1],$v1.nativeViewOrientation[0])-[Math]::Atan2($v0.nativeViewOrientation[1],$v0.nativeViewOrientation[0]))*180/[Math]::PI))/.18
 if([Math]::Abs($yawRate)-lt2){throw 'Native controller look calibration failed'}
 Save "$out/native-defense-look-calibration.json" @{before=$v0;after=$v1;rate=$yawRate;method='Native controller feedback while holding block'}
''' +needle)
needle="  $targets=Q host '--ai-targets' $handles.host;";assert needle in s
s=s.replace(needle,'''  $viewNow=Q client '--player';$choices=@()
  foreach($target in $allTargets){if($target.combat.resolved-and$target.combat.handle-eq$avatar){
   $body=Q host '--npc-pose' $target.handle;$dx=$body.root[0]-$viewNow.nativeViewPosition[0];$dy=$body.root[1]-$viewNow.nativeViewPosition[1]
   $choices+=@{handle=$target.handle;root=$body.root;dx=$dx;dy=$dy;distance=($dx*$dx+$dy*$dy)}
  }}
  $closest=$choices|Sort-Object distance|Select-Object -First 1;$defenseAim=$null
  if($closest){
   $want=[Math]::Atan2($closest.dy,$closest.dx)*180/[Math]::PI;$actual=[Math]::Atan2($viewNow.nativeViewOrientation[1],$viewNow.nativeViewOrientation[0])*180/[Math]::PI
   $error=WrappedAngle ($want-$actual);$defenseAim=@{target=$closest.handle;position=$closest.root;distance=[Math]::Sqrt($closest.distance);yawError=$error;view=$viewNow}
   if([Math]::Abs($error)-gt12){
    $axis=if($error*$yawRate-gt0){20000}else{-20000};$ms=[int][Math]::Clamp([Math]::Abs($error/$yawRate)*1000,100,180)
    & "$lab/Send-D2Input.ps1" -Role client -Button RightShoulder -RightX $axis -DurationMs 1500|Out-Null
    Start-Sleep -Milliseconds $ms
   }
  }
  & "$lab/Send-D2Input.ps1" -Role client -Button RightShoulder -DurationMs 1500|Out-Null
''' +needle)
s=s.replace('incoming=(S client \'.npc-attack.json\')','incoming=(S client \'.npc-attack.json\');defenseAim=$defenseAim')
s=s.replace('actual client native held-block scene, at least one protected', 'actual client native held-block scene with controller facing toward the closest engaged enemy, at least one protected')
(N/'Test-NpcNativeBlock-v1008.ps1').write_text(s)
print('Prepared1008: measured native controller facing and continuous held block')
