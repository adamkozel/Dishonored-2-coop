from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativePerception-v985.ps1').read_text().replace('npc-native-perception-v985','npc-native-perception-v986')
a=' $watch.Restart();$selected=0;$attacking=0';assert a in s;s=s.replace(a,a+';$moveAt=-1;$movingSelections=0;$moveStart=$null;$maxDistance=0')
a="  if($row.avatarHealth.current-le0){break}";assert a in s;s=s.replace(a,'''  if($selected-and$moveAt-lt0){
   $moveAt=$watch.ElapsedMilliseconds;$moveStart=@($bridge.remotePosition)
   Save "$out/movement-input.json" (& "$lab/Send-D2Input.ps1" -Role client -LeftY -26000 -LeftX 11000 -DurationMs 3500)
  }
  if($moveAt-ge0){
   $distance=0;for($axis=0;$axis-lt3;$axis++){$distance+=[Math]::Pow($bridge.remotePosition[$axis]-$moveStart[$axis],2)}
   $maxDistance=[Math]::Max($maxDistance,[Math]::Sqrt($distance))
   if($maxDistance-gt1-and@($allTargets|Where-Object {$_.combat.resolved-and$_.combat.handle-eq$avatar}).Count){$movingSelections++}
  }
  if($row.avatarHealth.current-le0){break}''')
a=' if($selected-lt1)';assert a in s;s=s.replace(a,''' Save "$out/moving-perception.json" @{startMs=$moveAt;startPosition=$moveStart;maxDistance=$maxDistance;selectedAfterMoving=$movingSelections}
 if($maxDistance-lt2-or$movingSelections-lt2){throw 'Insufficient native movement with retained enemy targeting'}
 if($selected-lt1)''')
a="  if($dirty){\n";assert s.count(a)==1;s=s.replace(a,a+'''   try{& "$lab/Send-D2Input.ps1" -Role client -DurationMs 100|Out-Null}catch{$errors.Add($_.Exception.Message)}
''')
(N/'Test-NpcNativePerception-v986.ps1').write_text(s)
