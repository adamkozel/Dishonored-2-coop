from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-DominoCrouchFinalNetwork-v254.ps1').read_text(encoding='utf-8-sig')
a=s.index(" $ground=Q client '--player-states';");b=s.index(" $before=Q client '--player-domino'",a)
s=s[:a]+''' $ground=Q client '--player-states';$ci=[Globalization.CultureInfo]::InvariantCulture
 $observerAnchor=$null
 foreach($candidate in @(@(-44.39,-153.27,66.4036484),@(-48.59,-155.07,66.4370575))){
  $gap=[Math]::Pow([double]$ground.position[0]-$candidate[0],2)+[Math]::Pow([double]$ground.position[1]-$candidate[1],2)
  if($gap-ge9-and$gap-le100){$observerAnchor=$candidate;break}
 }
 if(!$observerAnchor){throw 'No previously measured observer floor with bounded source separation'}
 $ox=$observerAnchor[0];$oy=$observerAnchor[1];$oz=$observerAnchor[2]+1.7
 $oyaw=[Math]::Atan2([double]$ground.position[1]-$oy,[double]$ground.position[0]-$ox)*180/[Math]::PI
 C host ('setviewpos {0} {1} {2} {3} 0'-f$ox.ToString('F4',$ci),$oy.ToString('F4',$ci),$oz.ToString('F4',$ci),$oyaw.ToString('F4',$ci))
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 12 -Evidence "$out/observer-camera.json" -Quick
 $floorRows=[Collections.Generic.List[object]]::new();$stableFloor=0;$lastFloor=$null;$floorWatch=[Diagnostics.Stopwatch]::StartNew()
 do{
  $f=Q host '--player-states';$floorRows.Add($f)
  $same=$f.motionValid-and$f.motion-eq0-and$f.stance-eq0-and[Math]::Abs([double]$f.position[2]-$observerAnchor[2])-lt.03-and$null-ne$lastFloor
  if($same){for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$f.position[$k]-[double]$lastFloor.position[$k])-gt.005){$same=$false}}}
  $stableFloor=if($same){$stableFloor+1}else{0};$lastFloor=$f;if($stableFloor-ge4){break};Start-Sleep -Milliseconds 125
 }while($floorWatch.ElapsedMilliseconds-lt4000)
 [ordered]@{anchor=$observerAnchor;source=$ground;stable=$stableFloor;rows=$floorRows}|ConvertTo-Json -Depth 10|Set-Content "$out/peer-floor-before-stream.json"
 if($stableFloor-lt4){throw 'Observer not stably grounded at previously measured floor'}
''' +s[b:]
(P/'Test-DominoCrouchFinalNetwork-v256.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace('Test-DominoCrouchFinalNetwork-v254.ps1','Test-DominoCrouchFinalNetwork-v256.ps1');p.write_text(s,encoding='utf-8')
print('Observer now uses previously measured balcony floors, avoiding east furniture and requiring four stable samples.')
