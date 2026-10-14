$ErrorActionPreference='Stop'
$lab=Join-Path (Split-Path $PSScriptRoot -Parent) 'outputs/d2-coop-lab'
$rows=[Collections.Generic.List[object]]::new();$launch=@{};$fail=$null
foreach($role in 'host','client'){$launch[$role]=Get-Content "$PSScriptRoot/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json}
function Capture([string]$role,[string]$stage,[int]$count){
 for($i=0;$i -lt $count;$i++){
  $raw=& "$lab/bin/d2_runtime_probe.exe" $launch[$role].pid --skeleton 1;$code=$LASTEXITCODE
  $rawMotion=& "$lab/bin/d2_runtime_probe.exe" $launch[$role].pid --player-states;$motionCode=$LASTEXITCODE
  $rawStance=& "$lab/bin/d2_runtime_probe.exe" $launch[$role].pid --player-stance;$stanceCode=$LASTEXITCODE
  $rows.Add([pscustomobject]@{Role=$role;Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Exit=$code;Skeleton=$(if(!$code){$raw|ConvertFrom-Json}else{$null});Motion=$(if(!$motionCode){$rawMotion|ConvertFrom-Json}else{$null});Stance=$(if(!$stanceCode){$rawStance|ConvertFrom-Json}else{$null})})
  Start-Sleep -Milliseconds 70
 }
}
try {
 foreach($role in 'host','client'){
  & "$lab/Send-D2Input.ps1" -Role $role -DurationMs 100|Out-Null
  Capture $role stand 8
  & "$lab/Send-D2Input.ps1" -Role $role -Button B -DurationMs 150|Out-Null;Start-Sleep -Milliseconds 600
  Capture $role crouch 12
  & "$lab/Send-D2Input.ps1" -Role $role -Button A -DurationMs 150|Out-Null
  Capture $role crouch_jump 18
  Start-Sleep -Milliseconds 600
  & "$lab/Send-D2Input.ps1" -Role $role -Button B -DurationMs 150|Out-Null;Start-Sleep -Milliseconds 600
  & "$lab/Send-D2Input.ps1" -Role $role -Button A -DurationMs 150|Out-Null
  Capture $role stand_jump 18
  Start-Sleep -Milliseconds 600
  Capture $role returned_stand 8
 }
}catch{$fail=$_.Exception.Message}finally{
 foreach($role in 'host','client'){& "$lab/Send-D2Input.ps1" -Role $role -DurationMs 100|Out-Null}
 [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Failure=$fail;DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;Scope='Read-only actual player skeleton observations under native stand, crouch and jump inputs; no pose retargeting or remote skeleton mutation.';Samples=$rows.ToArray()}|ConvertTo-Json -Depth 20|Set-Content "$PSScriptRoot/player-skeleton-observations.json"
}
if($fail){throw $fail}
$rows|Group-Object Role,Stage|ForEach-Object{[pscustomobject]@{Stage=$_.Name;Samples=$_.Count;Stable=@($_.Group|Where-Object {$_.Skeleton.stable}).Count;Rejected=@($_.Group|Where-Object Exit -ne 0).Count;RootHeight=($_.Group.Skeleton|Where-Object {$_}|ForEach-Object {$_.root[2]}|Measure-Object -Minimum -Maximum);HipHeight=($_.Group.Skeleton|Where-Object {$_}|ForEach-Object {($_.phases[2].matrices|Where-Object joint -eq 1).matrix[14]}|Measure-Object -Minimum -Maximum)}}|ConvertTo-Json -Depth 5
