param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='sprint-root-trace-source-clock')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));Set-Location $root
function C($r,$s){& outputs/d2-coop-lab/Send-D2Command.ps1 -Role $r -Command $s -WaitConsumed|Out-Null}
try{
 C host 'setviewpos -35 -153 68.1 0 0';C client 'setviewpos -30.5 -150.2 68.1 214 13';Start-Sleep -Milliseconds 900
 C host 'coop_shadow_stream on';C client 'coop_shadow_stream on';Start-Sleep -Milliseconds 600
 C client 'coop_movement_bank on';C client 'coop_pose_trace 3000'
 & outputs/d2-coop-lab/Send-D2Input.ps1 -Role host -LeftY 28000 -LeftX -16000 -Button LeftThumb -DurationMs 750|Out-Null
 Start-Sleep -Milliseconds 3300
 Copy-Item work/lab-instances/client/coop-status.json.pose-cadence.json "$PSScriptRoot/$Take.json"
 Get-Content work/lab-instances/client/coop-status.json.movement.json -Raw|Set-Content "$PSScriptRoot/$Take-movement.json"
}finally{C client 'coop_movement_bank off';C host 'coop_shadow_stream off';C client 'coop_shadow_stream off'}
