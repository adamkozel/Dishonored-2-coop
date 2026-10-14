param([ValidateSet('host','client')][string]$Role='host',[int]$AttackMs=150,[int]$GapMs=650,[int]$TotalMs=20000,[string]$Log)
# Ordinary controller input only: RightTrigger (sword attack) for AttackMs, then
# neutral for GapMs, repeated. Single input owner. No game memory access.
$ErrorActionPreference='Stop';$lab=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../outputs/d2-coop-lab'))
$sw=[Diagnostics.Stopwatch]::StartNew();$marks=[Collections.Generic.List[object]]::new()
while($sw.ElapsedMilliseconds-lt$TotalMs){
 $a=$sw.ElapsedMilliseconds;& "$lab/Send-D2Input.ps1" -Role $Role -RightTrigger 255 -DurationMs $AttackMs|Out-Null;$marks.Add($a)
 while($sw.ElapsedMilliseconds-lt$a+$AttackMs){Start-Sleep -Milliseconds 5}
 $u=$sw.ElapsedMilliseconds;& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs $GapMs|Out-Null
 while($sw.ElapsedMilliseconds-lt$u+$GapMs){Start-Sleep -Milliseconds 5}
}
& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null
if($Log){@{attackMs=$AttackMs;gapMs=$GapMs;attacks=$marks}|ConvertTo-Json -Depth 4|Set-Content -LiteralPath $Log}
