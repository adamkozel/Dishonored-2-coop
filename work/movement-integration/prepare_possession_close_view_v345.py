from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-PossessionExitNetwork-v343.ps1').read_text()
start=s.index(' $observerX=-44.39;');end=s.index(' $placed=Q host',start)
s=s[:start]+''' $observerAttempts=[Collections.Generic.List[object]]::new();$observerFound=$false
 foreach($offset in @(@(0.0,-3.0),@(0.0,3.0),@(-3.0,0.0),@(3.0,0.0))){
  $observerX=$sourceX+$offset[0];$observerY=$sourceY+$offset[1]
  $observerYaw=[Math]::Atan2($sourceY-$observerY,$sourceX-$observerX)*180/[Math]::PI
  C client ('setviewpos {0} {1} 68.1 {2} 0' -f $observerX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
  Start-Sleep -Milliseconds 650
  $observerGround=Q client '--player-states';$observerHealth=Q client '--health' 1
  $observerAttempts.Add([pscustomobject]@{requested=@($observerX,$observerY);actual=$observerGround;health=$observerHealth.current})
  if($observerHealth.current-ne56){throw 'Observer floor test changed health'}
  if($observerGround.defaultMode-and$observerGround.motionValid-and$observerGround.motion-eq0-and$observerGround.stance-eq0-and[Math]::Abs([double]$observerGround.position[2]-66.4)-lt.1){$observerFound=$true;$observerX=[double]$observerGround.position[0];$observerY=[double]$observerGround.position[1];break}
  C client 'setviewpos -44.39 -153.27 68.1 180 0'
 }
 $observerAttempts|ConvertTo-Json -Depth 10|Set-Content "$out/observer-floor-attempts.json"
 if(!$observerFound){throw 'No independently verified close observer floor'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 12 -Evidence "$out/observer-camera.json" -Quick
''' + s[end:]
s=s.replace('[double]$returned.position[1]+153.27,[double]$returned.position[0]+44.39','[double]$returned.position[1]-$sourceY,[double]$returned.position[0]-$sourceX')
s=s.replace("C client ('setviewpos -44.39 -153.27 68.1 {0} 0' -f $returnYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))", "C client ('setviewpos {0} {1} 68.1 {2} 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$returnYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))")
s=s.replace('Previously verified observer floor from323; camera-only reframing.','Source floor measured in this take; camera-only reframing.')
s=s.replace("observer=(Q client '--player-states');visibility=", "observer=(Q client '--player-states');nativeCamera=(Q client '--player');visibility=")
(P/'Test-PossessionExitCloseView-v345.ps1').write_text(s,encoding='utf-8')
print('345 adds independently measured close observer floor and native pitch feedback; runner unchanged')
