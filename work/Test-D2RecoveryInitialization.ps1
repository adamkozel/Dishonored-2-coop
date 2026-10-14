$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
function Command([string]$role,[string]$text){& (Join-Path $lab 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null}
$results=@()
foreach($role in 'host','client'){Command $role 'coop_probe_follow off';Start-Sleep -Milliseconds 200}
& (Join-Path $lab 'Test-D2SharedCheckpoint.ps1') -RequestRole client -DelayRole client|ConvertTo-Json
foreach($role in 'host','client') {
    Command $role $(if($role -eq 'host'){'coop_spawn_probe emily'}else{'coop_spawn_probe corvo'})
    Start-Sleep -Milliseconds 800
    Command $role 'coop_probe_follow on';Start-Sleep -Milliseconds 300
}
try{& (Join-Path $lab 'Test-D2Locomotion.ps1')|Out-Null}catch{Write-Output $_.Exception.Message}
$r=Get-Content -LiteralPath (Join-Path $lab 'research/locomotion-verification.json') -Raw|ConvertFrom-Json
Copy-Item -LiteralPath (Join-Path $lab 'research/locomotion-verification.json') -Destination (Join-Path $root 'work/locomotion-canonical82-manual-initialization.json') -Force
$results+=[pscustomobject]@{Mode='Manual SLOT82';Passed=$r.Passed;Dll=$r.DllSha256;Results=$r.Results}
foreach($role in 'host','client'){Command $role 'coop_checkpoint_sync off';Start-Sleep -Milliseconds 350}
& (Join-Path $lab 'Test-D2SharedCheckpoint.ps1') -RequestRole client -DelayRole client|ConvertTo-Json
try{& (Join-Path $lab 'Test-D2Locomotion.ps1')|Out-Null}catch{Write-Output $_.Exception.Message}
$r=Get-Content -LiteralPath (Join-Path $lab 'research/locomotion-verification.json') -Raw|ConvertFrom-Json
Copy-Item -LiteralPath (Join-Path $lab 'research/locomotion-verification.json') -Destination (Join-Path $root 'work/locomotion-canonical82-auto-initialization.json') -Force
$results+=[pscustomobject]@{Mode='Automatic SLOT82';Passed=$r.Passed;Dll=$r.DllSha256;Results=$r.Results}
$results|ConvertTo-Json -Depth 12|Set-Content -LiteralPath (Join-Path $root 'work/recovery-initialization-comparison.json')
$results|Select-Object Mode,Passed,Dll|ConvertTo-Json
