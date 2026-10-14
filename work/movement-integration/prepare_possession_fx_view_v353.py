from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionTargetFxView-v352.ps1').read_text()
a="foreach($offset in @(@(0.0,-3.0),@(0.0,3.0),@(-3.0,0.0),@(3.0,0.0)))"
assert s.count(a)==1
s=s.replace(a,"foreach($offset in @(@(0.0,-1.5),@(-1.5,0.0),@(1.5,0.0),@(0.0,1.5),@(0.0,-3.0),@(-3.0,0.0),@(3.0,0.0)))")
a='$observerX=$sourceX+$offset[0];$observerY=$sourceY+$offset[1]'
s=s.replace(a,a+"\n  if([Math]::Pow([double]$target.position[0]-$observerX,2)+[Math]::Pow([double]$target.position[1]-$observerY,2)-lt3.24){continue}")
a='if($observerGround.defaultMode-and$observerGround.motionValid'
s=s.replace(a,'if([Math]::Pow([double]$target.position[0]-[double]$observerGround.position[0],2)+[Math]::Pow([double]$target.position[1]-[double]$observerGround.position[1],2)-ge3.24-and$observerGround.defaultMode-and$observerGround.motionValid')
a=" if(!$observerFound){throw 'No independently verified close observer floor'}"
s=s.replace(a,a+'''
 $observerYaw=[Math]::Atan2([double]$target.position[1]-$observerY,[double]$target.position[0]-$observerX)*180/[Math]::PI
 C client ('setviewpos {0} {1} 66.40365 {2} 0' -f $observerX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
''')
(P/'Test-PossessionTargetFxView-v353.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Test-PossessionTargetFxView-v352.ps1')==1;s=s.replace('Test-PossessionTargetFxView-v352.ps1','Test-PossessionTargetFxView-v353.ps1');p.write_text(s)
print('353 excludes observer placement on NPC and recomputes aim from actual floor coordinates')
