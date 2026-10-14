$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot
$path="$root/outputs/d2-coop-lab/research/doppelganger-crouch-relative-6FDE-component-metric.json"
$report=Get-Content $path -Raw|ConvertFrom-Json
$measurements=@(foreach($role in 'host','client'){
 $walking=@($report.Samples|Where-Object {$_.Source-eq$role-and$_.Stage-eq'Walking'-and$_.Stable})
 $angle=0.
 foreach($name in 'left_femur_jnt','left_tibia_jnt','right_femur_jnt','right_tibia_jnt'){
  foreach($a in $walking){$aj=($a.Joints|Where-Object nativeName -eq $name).localPose
   foreach($b in $walking){$bj=($b.Joints|Where-Object nativeName -eq $name).localPose;$trace=0.
    foreach($k in 0,1,2,4,5,6,8,9,10){$trace+=$aj[$k]*$bj[$k]}
    $angle=[Math]::Max($angle,[Math]::Acos([Math]::Clamp(($trace-1.0)*.5,-1.0,1.0)))
   }
  }
 }
 [pscustomobject]@{Role=$role;CoherentWalking=$walking.Count;MaximumAngularExcursionRadians=$angle;Degrees=$angle*180/[Math]::PI;ExceedsPointOneRadian=$angle-ge.1}
})
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');SourceReport=$path;SourceSha256=(Get-FileHash $path).Hash;OriginalTestPassed=$report.Passed;Reason='The former metric compared coordinate-dependent matrix entries with the first coherent sample. New metric uses maximum relative rotation angle over all coherent samples. Original failure remains preserved; a fresh runtime run is still required.';Measurements=$measurements}|ConvertTo-Json -Depth 5|Set-Content "$root/outputs/d2-coop-lab/research/doppelganger-crouch-relative-6FDE-angular-reanalysis.json"
$measurements
