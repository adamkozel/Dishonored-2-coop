Set-Location 'C:\Users\adako\Documents\Codex\2026-10-08\i-x20'
. ./tools/vm/D2LabVM.Common.ps1; . ./tools/vm/D2LabVM.Lab.ps1
$lab = 'outputs/d2-coop-lab'
function HS { for ($i = 0; $i -lt 40; $i++) { try { return (Get-Content work/lab-instances/host/coop-status.json -Raw | ConvertFrom-Json) } catch { Start-Sleep -Milliseconds 50 } } }
function Brief($o) { if (!$o) { return 'n/a' }; ($o.PSObject.Properties | Where-Object { $_.Name -in 'active','host','phase','result','connectionState','role' } | ForEach-Object { "$($_.Name)=$($_.Value)" }) -join ' ' }
'1) restart the host lab instance alone (no local client -> no stale loopback link)'
$l = Get-Content work/lab-instances/host/launch.json -Raw | ConvertFrom-Json
# (host already restarted alone in the previous run)
if (!(Get-Process -Id (Get-Content work/lab-instances/host/launch.json -Raw | ConvertFrom-Json).pid -ErrorAction SilentlyContinue)) { & "$lab/Start-D2Lab.ps1" -Role host -AssetExperiment | Out-Null }
$deadline = (Get-Date).AddSeconds(90); do { Start-Sleep 3; $hs = HS } while ((Get-Date) -lt $deadline -and !($hs -and $hs.engineThread))
if ((HS).localGameLogicState -eq 6) { & "$lab/Send-D2Input.ps1" -Role host -DurationMs 100 | Out-Null; Start-Sleep -Milliseconds 2000; & "$lab/Send-D2Input.ps1" -Role host -Button A -DurationMs 400 | Out-Null; Start-Sleep -Milliseconds 1200 }; & "$lab/Send-D2Command.ps1" -Role host -Command "LoadGame 81" -WaitConsumed -ReceiptTimeoutMs 8000 | Out-Null
$deadline = (Get-Date).AddSeconds(120)
do { Start-Sleep 3; $hs = HS; if ($hs.localGameLogicState -in 13, 8, 15) { & "$lab/Send-D2Input.ps1" -Role host -Button A -DurationMs 300 | Out-Null } } while ((Get-Date) -lt $deadline -and $hs.localGameLogicState -ne 1)
"   host logic=$($hs.localGameLogicState) map=$($hs.localMap)"
'2) host: coop_session host'
& "$lab/Send-D2Command.ps1" -Role host -Command 'coop_session host' -WaitConsumed -ReceiptTimeoutMs 8000 | Out-Null
$deadline = (Get-Date).AddSeconds(30); do { Start-Sleep 2; $ho = Get-Content work/lab-instances/host/coop-status.json.online.json -Raw | ConvertFrom-Json } while ((Get-Date) -lt $deadline -and !($ho.lobby -and "$($ho.lobby)" -ne '0'))
"   host online: $(Brief $ho)  hostConnection=$((HS).connection)"
'3) VM: coop_session join'
Send-D2LabVMCommand -Command 'coop_session off' -WaitConsumed | Out-Null; Start-Sleep 3
Send-D2LabVMCommand -Command "coop_session join $($ho.lobby)" -WaitConsumed | Out-Null
$deadline = (Get-Date).AddSeconds(60); do { Start-Sleep 4; $gs = Get-D2LabVMStatus; $hs = HS } while ((Get-Date) -lt $deadline -and !($gs.remoteFresh -and $hs.remoteFresh))
"   guest online: $(Brief (Get-D2LabVMStatus -Sidecar online))"
"   host  online: $(Brief (Get-Content work/lab-instances/host/coop-status.json.online.json -Raw | ConvertFrom-Json))"
"   guest: connectionState=$($gs.connectionState) remoteFresh=$($gs.remoteFresh) sent=$($gs.sent) received=$($gs.received)"
"   host : connectionState=$($hs.connectionState) remoteFresh=$($hs.remoteFresh) sent=$($hs.sent) received=$($hs.received)"
if ($gs.remoteFresh -and $hs.remoteFresh) {
    $before = $hs.remotePosition; Send-D2LabVMKey -VirtualKey 0x57 -HoldMs 1500; Start-Sleep 2; $after = (HS).remotePosition
    "4) host sees the VM player move {0:N2} m (VM walked 1.5 s, keyboard)" -f [math]::Sqrt([math]::Pow($after[0]-$before[0],2)+[math]::Pow($after[1]-$before[1],2))
    $gb = (Get-D2LabVMStatus).remotePosition; & "$lab/Send-D2Input.ps1" -Role host -Button Neutral -LeftY 30000 -DurationMs 1500 | Out-Null; Start-Sleep 2; $ga = (Get-D2LabVMStatus).remotePosition
    "   VM sees the host player move {0:N2} m (host walked 1.5 s, simulated stick)" -f [math]::Sqrt([math]::Pow($ga[0]-$gb[0],2)+[math]::Pow($ga[1]-$gb[1],2))
}
Save-D2LabVMScreen -Path 'C:/Users/adako/AppData/Local/Temp/claude/C--Users-adako-Documents-Codex-2026-10-08-i-x20/0e285604-1fc3-425c-82af-b837ec659e79/scratchpad/two-account-vm-4.png' -Width 1920 -Height 1080 | Out-Null
