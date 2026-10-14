param([Parameter(Mandatory)][ValidateSet('host','client')][string]$Role,[Parameter(Mandatory)][double]$X,[Parameter(Mandatory)][double]$Y,[Parameter(Mandatory)][double]$Z,[Parameter(Mandatory)][string]$Evidence)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
if(!$launch.assetExperiment-or(Get-Process -Id $launch.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'}
function V{for($a=0;$a-lt8;$a++){$raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid --player 2>$null;if(!$LASTEXITCODE){$v=$raw|ConvertFrom-Json;if($v.nativeViewPosition.Count-eq3-and$v.nativeViewOrientation.Count-eq9){return $v}};Start-Sleep -Milliseconds 50};throw 'Native camera unavailable'}
function A($degrees){while($degrees-gt180){$degrees-=360};while($degrees-lt-180){$degrees+=360};$degrees}
$rows=[Collections.Generic.List[object]]::new();$settled=0;$yawRate=$null
try{
 for($i=0;$i-lt50;$i++){
  $v=V;$p=$v.nativeViewPosition;$axis=$v.nativeViewOrientation
  $dx=$X-$p[0];$dy=$Y-$p[1];$dz=$Z-$p[2];$flat=[Math]::Sqrt($dx*$dx+$dy*$dy)
  if($flat-lt.1){throw 'Camera directly above target; choose a ground vantage'}
  $targetYaw=[Math]::Atan2($dy,$dx)*180/[Math]::PI;$yaw=[Math]::Atan2($axis[1],$axis[0])*180/[Math]::PI;$yawError=A ($targetYaw-$yaw)
  $targetPitch=-[Math]::Atan2($dz,$flat)*180/[Math]::PI;$pitch=-[Math]::Asin([Math]::Clamp([double]$axis[2],-1,1))*180/[Math]::PI;$pitchError=$targetPitch-$pitch
  if([Math]::Abs($targetPitch)-gt70){throw 'Ground vantage produces unsupported steep pitch'}
  $rows.Add([pscustomobject]@{iteration=$i;eye=$p;yaw=$yaw;targetYaw=$targetYaw;yawError=$yawError;pitch=$pitch;targetPitch=$targetPitch;pitchError=$pitchError})
  if([Math]::Abs($yawError)-le.9-and[Math]::Abs($pitchError)-le.9){$settled++;if($settled-ge2){break};Start-Sleep -Milliseconds 100;continue};$settled=0
  if([Math]::Abs($yawError)-gt.9){
   if($null-eq$yawRate){
    & "$lab/Send-D2Input.ps1" -Role $Role -RightX 20000 -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 330
    $after=V;$newYaw=[Math]::Atan2($after.nativeViewOrientation[1],$after.nativeViewOrientation[0])*180/[Math]::PI;$delta=A ($newYaw-$yaw)
    if([Math]::Abs($delta)-lt.2){throw 'Native horizontal look did not respond'};$yawRate=$delta/.2;continue
   }
   # A close target moves angularly as the native eye offset rotates. The
   # previous16000/100ms minimum overshot that feedback loop at steep pitch.
   # Use a small above-deadzone pulse for final alignment, then re-read camera.
   $magnitude=if([Math]::Abs($yawError)-lt4){10000}elseif([Math]::Abs($yawError)-lt10){16000}else{20000}
   $rate=if($magnitude-eq10000){$yawRate*.08}elseif($magnitude-eq16000){$yawRate*.4}else{$yawRate}
   $xAxis=if($yawError*$yawRate-gt0){$magnitude}else{-$magnitude}
   $ms=if($magnitude-eq10000){100}else{[int][Math]::Clamp([Math]::Abs($yawError/$rate)*700,100,500)}
   & "$lab/Send-D2Input.ps1" -Role $Role -RightX $xAxis -DurationMs $ms|Out-Null;Start-Sleep -Milliseconds ($ms+110)
  }else{
   $yAxis=if($pitchError-gt0){-16000}else{16000};$ms=[int][Math]::Clamp([Math]::Abs($pitchError)/13*1000,100,500)
   & "$lab/Send-D2Input.ps1" -Role $Role -RightY $yAxis -DurationMs $ms|Out-Null;Start-Sleep -Milliseconds ($ms+110)
  }
 }
 if($settled-lt2){throw 'Measured native target aim did not settle'}
}finally{
 & "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null
 [ordered]@{target=@($X,$Y,$Z);settled=($settled-ge2);yawRateMeasured=$yawRate;method='Native controller look and read-only camera feedback; no camera memory write';samples=@($rows)}|ConvertTo-Json -Depth 8|Set-Content $Evidence
}

