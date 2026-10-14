param([ValidateSet('host','client')][string]$Role)
$ErrorActionPreference='Stop';$root=Split-Path $PSScriptRoot -Parent
try {
 for($i=0;$i-lt12;$i++){
  $s=Get-Content "$root/work/lab-instances/$Role/coop-status.json" -Raw|ConvertFrom-Json
  if($s.localGameLogicState-ne1-or!$s.probeFollowExact){throw 'Visual fixture scope changed'}
  & "$root/outputs/d2-coop-lab/Send-D2Input.ps1" -Role $Role -RightTrigger 255 -DurationMs 220|Out-Null
  Start-Sleep -Milliseconds 900
 }
} finally {& "$root/outputs/d2-coop-lab/Send-D2Input.ps1" -Role $Role -Button Neutral -DurationMs 100|Out-Null}
