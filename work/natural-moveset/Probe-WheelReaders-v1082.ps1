param([string]$Out="$PSScriptRoot/wheel-readers-v1082")
# Read-only native reader observation while the host holds the native item
# wheel (ordinary LeftShoulder input) in the Dunwall fixture.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pid0=(Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json).pid
 foreach($m in '--logic','--player-stance','--player-states'){& "$lab/bin/d2_runtime_probe.exe" $pid0 $m > "$Out/before$($m.Replace('-','_')).json" 2>&1}
 $job=Start-Job -ScriptBlock {param($lab)& "$lab/Send-D2Input.ps1" -Role host -Button LeftShoulder -DurationMs 2500|Out-Null} -ArgumentList $lab
 Start-Sleep -Milliseconds 1100
 foreach($m in '--logic','--player-stance','--player-states'){& "$lab/bin/d2_runtime_probe.exe" $pid0 $m > "$Out/wheel$($m.Replace('-','_')).json" 2>&1;"$m exit $LASTEXITCODE"|Add-Content "$Out/exit-codes.txt"}
 Copy-Item "$root/work/lab-instances/host/coop-status.json" "$Out/wheel-status.json"
 Wait-Job $job -Timeout 10|Out-Null;Remove-Job $job -Force
 Start-Sleep -Milliseconds 1500
 & "$lab/Send-D2Input.ps1" -Role host -DurationMs 100|Out-Null
 $s=Get-Content "$root/work/lab-instances/host/coop-status.json" -Raw|ConvertFrom-Json;"after logic $($s.localGameLogicState)"|Add-Content "$Out/exit-codes.txt"
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
