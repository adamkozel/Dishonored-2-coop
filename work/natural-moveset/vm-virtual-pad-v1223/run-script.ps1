Set-Location 'C:\Users\adako\Documents\Codex\2026-10-08\i-x20'
. ./tools/vm/D2LabVM.Common.ps1; . ./tools/vm/D2LabVM.Lab.ps1
try { Stop-D2LabVMRole -Role client | Out-Null } catch { }
Start-D2LabVMRole -Role client | Out-Null
$d = (Get-Date).AddSeconds(90); do { Start-Sleep 3; $gs = Get-D2LabVMStatus } while ((Get-Date) -lt $d -and $gs.localGameLogicState -ne 6)
# Menu: try the simulated controller first (A), then LoadGame 81 as the lab does.
Send-D2LabVMInput -Button Neutral -DurationMs 100; Start-Sleep -Milliseconds 2000; Send-D2LabVMInput -Button A -DurationMs 400; Start-Sleep -Milliseconds 1200
Send-D2LabVMCommand -Command 'LoadGame 81' -WaitConsumed | Out-Null
$d = (Get-Date).AddSeconds(120); $viaPad = $true
do { Start-Sleep 3; $gs = Get-D2LabVMStatus; if ($gs.localGameLogicState -in 13, 8, 15) { Send-D2LabVMInput -Button A -DurationMs 400 } } while ((Get-Date) -lt $d -and $gs.localGameLogicState -ne 1)
"reached gameplay via simulated controller only: $($gs.localGameLogicState -eq 1)"
if ($gs.localGameLogicState -ne 1) { $viaPad = $false; Send-D2LabVMKey 0x0D; Start-Sleep 8 }
Start-Sleep 2
$p1 = (Invoke-D2LabVMProbe -Mode '--player').candidatePosition
Send-D2LabVMInput -Button Neutral -LeftY 30000 -DurationMs 1500; Start-Sleep -Milliseconds 2000
$p2 = (Invoke-D2LabVMProbe -Mode '--player').candidatePosition
"simulated left stick 1.5 s: moved {0:N2} m" -f [math]::Sqrt([math]::Pow($p2[0]-$p1[0],2)+[math]::Pow($p2[1]-$p1[1],2))
Send-D2LabVMInput -Button A -LeftY 22000 -DurationMs 1300; Start-Sleep -Milliseconds 300
$st = Invoke-D2LabVMProbe -Mode '--player-states'; "jump+forward active states: $(($st.active | ForEach-Object id) -join ',')"
