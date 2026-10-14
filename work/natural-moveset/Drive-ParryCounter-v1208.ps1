param([ValidateSet('host','client')][string]$Role='host',[int]$BlockMs=700,[int]$AttackMs=150,[int]$GapMs=450,[int]$TotalMs=30000,[string]$Log)
# Ordinary controller input only: hold block (RightShoulder) for BlockMs so an
# incoming swing can be parried, then attack (RightTrigger) at once - the
# native follow-up into an off-balance attacker - then neutral for GapMs (longer
# than the measured 0.266 s block re-entry gate). No game memory access.
$ErrorActionPreference='Stop';$lab=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../outputs/d2-coop-lab'))
$sw=[Diagnostics.Stopwatch]::StartNew();$marks=[Collections.Generic.List[object]]::new()
while($sw.ElapsedMilliseconds-lt$TotalMs){
 $t=$sw.ElapsedMilliseconds;& "$lab/Send-D2Input.ps1" -Role $Role -Button RightShoulder -DurationMs $BlockMs|Out-Null
 while($sw.ElapsedMilliseconds-lt$t+$BlockMs){Start-Sleep -Milliseconds 5}
 $a=$sw.ElapsedMilliseconds;& "$lab/Send-D2Input.ps1" -Role $Role -RightTrigger 255 -DurationMs $AttackMs|Out-Null
 while($sw.ElapsedMilliseconds-lt$a+$AttackMs){Start-Sleep -Milliseconds 5}
 $u=$sw.ElapsedMilliseconds;& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs $GapMs|Out-Null;$marks.Add(@{block=$t;attack=$a;neutral=$u})
 while($sw.ElapsedMilliseconds-lt$u+$GapMs){Start-Sleep -Milliseconds 5}
}
& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null
if($Log){@{blockMs=$BlockMs;attackMs=$AttackMs;gapMs=$GapMs;cycles=$marks}|ConvertTo-Json -Depth 5|Set-Content -LiteralPath $Log}
