Set-Location 'C:\Users\adako\Documents\Codex\2026-10-08\i-x20'
. ./tools/vm/D2LabVM.Common.ps1; . ./tools/vm/D2LabVM.Lab.ps1
$pre = Invoke-D2LabVM { [pscustomobject]@{
    system32 = @('msvcp140.dll','vcruntime140.dll','vcruntime140_1.dll' | Where-Object { Test-Path "C:\Windows\System32\$_" }).Count
    appLocal = @(foreach ($d in 'C:\D2Lab\ws\outputs\d2-coop-lab\bin','C:\D2Lab\ws\work\asset-game') { 'msvcp140.dll','vcruntime140.dll','vcruntime140_1.dll' | Where-Object { Test-Path "$d\$_" } }).Count
    dll = (Get-FileHash C:\D2Lab\ws\outputs\d2-coop-lab\bin\dinput8.dll).Hash.Substring(0,16) } }
"guest before start: VC++ runtime DLLs in System32=$($pre.system32), app-local=$($pre.appLocal), dll=$($pre.dll)"
try { Stop-D2LabVMRole -Role client | Out-Null } catch { }
Invoke-D2LabVM { Remove-Item C:\D2Lab\ws\work\lab-instances\client\loader.log -ErrorAction SilentlyContinue }
$r = Start-D2LabVMRole -Role client -TimeoutSec 150
Start-Sleep 15
$s = Get-D2LabVMStatus
"guest instance: alive=$($r.alive) statusWritten=$($r.status) engineThread=$($s.engineThread) steam=$($s.steamAvailable) logic=$($s.localGameLogicState)"
Invoke-D2LabVM { $p = Get-Process Dishonored2 | Select-Object -First 1; "loaded dinput8: " + (($p.Modules | Where-Object ModuleName -eq 'dinput8.dll' | ForEach-Object FileName) -join ', '); "loaded CRT modules: " + (($p.Modules | Where-Object { $_.ModuleName -match '^(msvcp140|vcruntime140)' } | ForEach-Object ModuleName) -join ', ' | ForEach-Object { if ($_) { $_ } else { 'none' } }); Get-Content C:\D2Lab\ws\work\lab-instances\client\loader.log -TotalCount 4 }
