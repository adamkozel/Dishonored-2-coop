param([Parameter(Mandatory)][ValidateSet('host','client')][string]$Role,[ValidateRange(-70,70)][double]$Pitch=15,[string]$Evidence,[switch]$Quick)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
if(!$launch.assetExperiment-or(Get-Process -Id $launch.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong camera process'}
$rows=[Collections.Generic.List[object]]::new();$settled=0
try{
 for($i=0;$i-lt24;$i++){
  # The read-only probe can race a native player snapshot even when the world
  # is ready. Retry briefly without sending camera input on an invalid read.
  $view=$null
  for($readAttempt=0;$readAttempt-lt8;$readAttempt++){
   $raw=& "$root/work/build/Release/d2_runtime_probe.exe" $launch.pid --player 2> $null
   if(!$LASTEXITCODE){try{$view=$raw|ConvertFrom-Json}catch{$view=$null}}
   if($view.nativeViewOrientation.Count-eq9){break}
   $view=$null;Start-Sleep -Milliseconds 125
  }
  if($view.nativeViewOrientation.Count-ne9){throw 'Native camera read failed after bounded retries'}
  $z=[double]$view.nativeViewOrientation[2];if([Math]::Abs($z)-gt1){throw 'Invalid native camera forward vector'}
  $current=-[Math]::Asin($z)*180/[Math]::PI;$error=$Pitch-$current
  $rows.Add([pscustomobject]@{iteration=$i;pitch=$current;target=$Pitch;view=$view})
  if([Math]::Abs($error)-le.85){$settled++;if($settled-ge2){break};Start-Sleep -Milliseconds 120;continue};$settled=0
  $axis=if([Math]::Abs($error)-gt8){20000}elseif([Math]::Abs($error)-gt3){16000}else{12000}
  $ms=if([Math]::Abs($error)-gt8){220}elseif([Math]::Abs($error)-gt3){180}else{130}
  if($Quick){
   # Observed native look rate: axis 20000 about 32 deg/s; 16000 about 13.
   # Longer bounded pulses reduce process/read overhead during finite item
   # fixtures. Every pulse still ends with independent native-matrix feedback.
   $axis=if([Math]::Abs($error)-gt3){20000}else{16000}
   $rate=if($axis-eq20000){32.0}else{13.0}
   $ms=[int][Math]::Clamp([Math]::Round([Math]::Abs($error)/$rate*1000),100,600)
  }
  if($error-gt0){$axis=-$axis}
  & "$lab/Send-D2Input.ps1" -Role $Role -RightY $axis -DurationMs $ms|Out-Null
  Start-Sleep -Milliseconds ($ms+120)
 }
 if($settled-lt2){throw 'Observer pitch did not settle'}
}finally{
 & "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null
 if($Evidence){[ordered]@{target=$Pitch;settled=($settled-ge2);quick=$Quick.IsPresent;method='Native controller look input with read-only native view-matrix feedback. Native setviewpos preserves existing camera pitch; its extra pitch argument does not set this field.';samples=@($rows)}|ConvertTo-Json -Depth 8|Set-Content $Evidence}
}
