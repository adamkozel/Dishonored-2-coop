$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
& (Join-Path $lab 'Test-D2AvatarAi.ps1')|ConvertTo-Json
& (Join-Path $lab 'Test-D2SharedCheckpoint.ps1') -RequestRole host -DelayRole host|ConvertTo-Json
& (Join-Path $lab 'Test-D2CheckpointMismatch.ps1')|ConvertTo-Json
foreach($stage in 'Held','Loading') {
    foreach($role in 'client','host') {
        & (Join-Path $lab 'Test-D2CheckpointDisconnect.ps1') -DisconnectRole $role -Stage $stage|ConvertTo-Json
        & (Join-Path $lab 'Start-D2Lab.ps1') -Role $role|Out-Null
        & (Join-Path $lab 'Restore-D2Lab.ps1')|ConvertTo-Json
    }
}
& (Join-Path $lab 'Test-D2SessionDisconnect.ps1') -DisconnectRole client -Stage Gameplay|ConvertTo-Json
& (Join-Path $lab 'Start-D2Lab.ps1') -Role client|Out-Null
& (Join-Path $lab 'Restore-D2Lab.ps1')|ConvertTo-Json
& (Join-Path $lab 'Test-D2SessionDisconnect.ps1') -DisconnectRole host -Stage Death|ConvertTo-Json
& (Join-Path $lab 'Start-D2Lab.ps1') -Role host|Out-Null
& (Join-Path $lab 'Restore-D2Lab.ps1')|ConvertTo-Json
& (Join-Path $lab 'Test-D2LoadingHold.ps1')|ConvertTo-Json -Depth 3
& (Join-Path $lab 'Test-D2LoadingCharacter.ps1')|ConvertTo-Json -Depth 3
& (Join-Path $lab 'Restore-D2Lab.ps1')|ConvertTo-Json
& (Join-Path $lab 'Test-D2GuardLink.ps1') -ReloadClient|ConvertTo-Json -Depth 3
& (Join-Path $lab 'Test-D2WorldObservation.ps1') -ReloadClient|ConvertTo-Json -Depth 3
& (Join-Path $lab 'Test-D2Reconnect.ps1')|ConvertTo-Json -Depth 3
& (Join-Path $lab 'Restore-D2Lab.ps1')|ConvertTo-Json
& (Join-Path $lab 'Test-D2Baseline.ps1')|ConvertTo-Json -Depth 3
& (Join-Path $PSScriptRoot 'Record-D2SharedRecovery.ps1')|ConvertTo-Json
