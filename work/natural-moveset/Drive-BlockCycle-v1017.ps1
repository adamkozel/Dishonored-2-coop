param([ValidateSet('host','client')][string]$Role='client',[int]$OnMs=600,[int]$OffMs=350,[int]$TotalMs=40000,[string]$Log)
# Ordinary controller input only: RightShoulder held for OnMs, then neutral
# for OffMs (longer than the measured native 0.266 s block re-entry gate),
# so each press opens a fresh native parry window. No game memory access.
$ErrorActionPreference='Stop';$lab=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../outputs/d2-coop-lab'))
$sw=[Diagnostics.Stopwatch]::StartNew();$marks=[Collections.Generic.List[object]]::new()
while($sw.ElapsedMilliseconds-lt$TotalMs){
 $t=$sw.ElapsedMilliseconds;& "$lab/Send-D2Input.ps1" -Role $Role -Button RightShoulder -DurationMs $OnMs|Out-Null;$marks.Add(@{press=$t;sent=$sw.ElapsedMilliseconds})
 while($sw.ElapsedMilliseconds-lt$t+$OnMs){Start-Sleep -Milliseconds 5}
 $u=$sw.ElapsedMilliseconds;& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs $OffMs|Out-Null
 while($sw.ElapsedMilliseconds-lt$u+$OffMs){Start-Sleep -Milliseconds 5}
}
& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null
if($Log){@{onMs=$OnMs;offMs=$OffMs;presses=$marks}|ConvertTo-Json -Depth 5|Set-Content -LiteralPath $Log}
