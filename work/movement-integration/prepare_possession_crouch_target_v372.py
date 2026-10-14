from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionCrouchTarget-v371.ps1').read_text()
old="if(!$final.defaultMode-or$final.stance-ne0){throw 'Native leave-host input did not recover grounded default mode'}"
assert s.count(old)==1
s=s.replace(old,"if(!$final.defaultMode-or$final.stance-ne1-or!$final.motionValid-or$final.motion-ne1){throw 'Native leave-host input did not recover verified crouched default mode'}")
s=s.replace('Corvo standing dual-equip exit','Corvo crouched dual-equip exit')
old=" $observerYaw=[Math]::Atan2([double]$target.position[1]-$observerY,[double]$target.position[0]-$observerX)*180/[Math]::PI\n C client"
assert s.count(old)==1
s=s.replace(old,""" # Camera-only bisector includes both the caster and native target. The
 # prior target-only yaw put the close crouched caster outside the frame.
 $tx=[double]$target.position[0]-$observerX;$ty=[double]$target.position[1]-$observerY
 $cx=$sourceX-$observerX;$cy=$sourceY-$observerY
 $td=[Math]::Sqrt($tx*$tx+$ty*$ty);$cd=[Math]::Sqrt($cx*$cx+$cy*$cy)
 if($td-lt.1-or$cd-lt.1){throw 'Degenerate review camera bisector'}
 $observerYaw=[Math]::Atan2($ty/$td+$cy/$cd,$tx/$td+$cx/$cd)*180/[Math]::PI
 C client""")
s=s.replace("-Role client -Pitch 0 -Evidence", "-Role client -Pitch 15 -Evidence")
(P/'Test-PossessionCrouchTarget-v372.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert 'Test-PossessionCrouchTarget-v371.ps1' in s
p.write_text(s.replace('Test-PossessionCrouchTarget-v371.ps1','Test-PossessionCrouchTarget-v372.ps1'))
print('372 harness final stance and camera corrected; production code/bank unchanged')
