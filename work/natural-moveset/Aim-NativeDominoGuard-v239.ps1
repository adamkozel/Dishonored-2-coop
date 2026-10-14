param([ValidateSet('host','client')][string]$Role='client',[Parameter(Mandatory)][string]$Evidence,[ValidateSet('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')][string]$TargetSpawner='ld_spawner_guard_tutorial_2')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
if(!$launch.assetExperiment-or(Get-Process -Id $launch.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated camera process'}
function Q($mode){for($j=0;$j-lt8;$j++){$raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 30};throw 'Read-only native aim snapshot unavailable'}
function Angle($x){while($x-gt180){$x-=360};while($x-le-180){$x+=360};return $x}
$rows=[Collections.Generic.List[object]]::new();$settled=0;$gain=-32.0;$lastYaw=$null;$lastAxis=0;$lastMs=0;$initial=$null
try{
 for($i=0;$i-lt18;$i++){
  $view=Q '--player';$state=Q '--player-states';$catalog=Q '--world-npcs'
  if($view.nativeViewOrientation.Count-ne9-or!$state.motionValid-or$state.motion-ne0){throw 'Native camera or grounded motion unavailable'}
  if(!$initial){$initial=$state.position}
  for($axis=0;$axis-lt3;$axis++){if([Math]::Abs([double]$state.position[$axis]-[double]$initial[$axis])-gt.01){throw 'Native aim changed source position'}}
  $targets=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq$TargetSpawner-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-[double]$state.position[2])-lt5})
  if($targets.Count-ne1){throw 'Exact tutorial guard outside measured five-metre vertical range'};$target=$targets[0]
  $dx=[double]$target.position[0]-[double]$state.position[0];$dy=[double]$target.position[1]-[double]$state.position[1]
  if($dx*$dx+$dy*$dy-lt1-or$dx*$dx+$dy*$dy-gt400){throw 'Aim target outside bounded native range'}
  $desired=[Math]::Atan2($dy,$dx)*180/[Math]::PI;$yaw=[Math]::Atan2([double]$view.nativeViewOrientation[1],[double]$view.nativeViewOrientation[0])*180/[Math]::PI
  if($null-ne$lastYaw-and$lastAxis){$delta=Angle ($yaw-$lastYaw);$observed=$delta/($lastMs/1000.0)*[Math]::Sign($lastAxis);if([Math]::Abs($observed)-gt2-and[Math]::Abs($observed)-lt200){$gain=$observed}}
  $error=Angle ($desired-$yaw)
  $rows.Add([pscustomobject]@{iteration=$i;yaw=$yaw;targetYaw=$desired;error=$error;gain=$gain;target=$target;playerPosition=$state.position})
  if([Math]::Abs($error)-le1.5){$settled++;if($settled-ge2){break};$lastAxis=0;Start-Sleep -Milliseconds 80;continue};$settled=0
  $axis=20000*[Math]::Sign($error/$gain);$ms=[int][Math]::Clamp([Math]::Round([Math]::Abs($error/$gain)*1000),100,350)
  $lastYaw=$yaw;$lastAxis=$axis;$lastMs=$ms
  & "$lab/Send-D2Input.ps1" -Role $Role -RightX $axis -DurationMs $ms|Out-Null
  Start-Sleep -Milliseconds ($ms+70)
 }
 if($settled-lt2){throw 'Native guard yaw did not converge'}
 $view=Q '--player';$state=Q '--player-states';$catalog=Q '--world-npcs'
 $target=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq$TargetSpawner})
 if($target.Count-ne1-or$view.nativeViewPosition.Count-ne3){throw 'Native target/view changed before pitch correction'}
 $dx=[double]$target[0].position[0]-[double]$view.nativeViewPosition[0];$dy=[double]$target[0].position[1]-[double]$view.nativeViewPosition[1]
 $dz=[double]$target[0].position[2]+1.15-[double]$view.nativeViewPosition[2]
 $targetPitch=-[Math]::Atan2($dz,[Math]::Sqrt($dx*$dx+$dy*$dy))*180/[Math]::PI
 if([Math]::Abs($targetPitch)-gt60){throw 'Native target elevation exceeds bounded aim range'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch $targetPitch -Evidence ($Evidence+'.pitch.json') -Quick
 $after=Q '--player-states';for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$after.position[$k]-[double]$initial[$k])-gt.01){throw 'Native pitch correction changed source floor'}}

}finally{
 & "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null
 [ordered]@{settled=($settled-ge2);method='Native right-stick input with independent current view/guard feedback, measured look direction/rate, and stable grounded source position. No teleport or NPC mutation.';samples=@($rows)}|ConvertTo-Json -Depth 9|Set-Content $Evidence
}
