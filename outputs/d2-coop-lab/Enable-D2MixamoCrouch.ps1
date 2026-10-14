[CmdletBinding()]
param([switch]$Off)
# Explicit isolated-session opt-in; never edits Steam assets or first-person rigs.
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$expected='29978C5426F33E2E96929D06C77E81E54044F0B57CC01672513B89583328E6B9'
foreach($role in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $l.pid).Path-ne$l.executable-or!$l.assetExperiment-or(Get-FileHash (Join-Path (Split-Path $l.executable) 'dinput8.dll')).Hash-ne$expected){throw 'Unexpected isolated test build'}
}
foreach($role in 'host','client'){
 & "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $(if($Off){'coop_mixamo_crouch off'}else{'coop_mixamo_crouch on'}) -WaitConsumed|Out-Null
}
Start-Sleep -Milliseconds 400
foreach($role in 'host','client'){
 $m=Get-Content "$root/work/lab-instances/$role/coop-status.json.mixamo.json" -Raw|ConvertFrom-Json
 if($Off){if($m.active){throw "$role did not disable"}}
 elseif(!$m.active-or!$m.persistent){throw "$role refused mirrored crouch. Confirm fresh follow, native crouch reference and equipped clone sword; command receipt alone is not success."}
 [pscustomobject]@{Role=$role;Active=$m.active;CurrentSession=$m.persistent;Phase=$m.phase;Applied=$m.applied}
}


