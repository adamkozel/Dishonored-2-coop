# Gives the lab VM a game controller so the lab's simulated controller works there (#24): Dishonored 2
# ignores XInput state unless a controller device exists, and the guest has none.
#   1. builds tools/vm/guest/vigem/{vigem_setup,vigem_pad}.cpp with the local MSVC (static CRT)
#   2. copies the signed ViGEmBus driver package from the HOST's DriverStore into the guest (no download)
#   3. registers the bus device (vigem_setup, idempotent)
#   4. registers a logon task that plugs one virtual Xbox 360 pad (vigem_pad) and starts it now
# The lab proxy's XInputGetState hook still supplies the controller state; the pad only has to exist.
# Start (or restart) the game AFTER the pad exists: the game enumerates controllers at launch.
# pwsh 7, member of "Hyper-V Administrators".
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')
$bin = Join-Path $VmLab 'vigem'; New-Item -ItemType Directory -Force $bin | Out-Null
$src = Join-Path $PSScriptRoot 'guest/vigem'
if (!(Test-Path "$bin/vigem_setup.exe") -or !(Test-Path "$bin/vigem_pad.exe") -or
    (Get-Item "$src/vigem_pad.cpp").LastWriteTime -gt (Get-Item "$bin/vigem_pad.exe").LastWriteTime) {
    $vcvars = 'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat'
    $cmd = Join-Path $bin 'build.cmd'
    Set-Content -Encoding ASCII $cmd @"
@echo off
call "$vcvars" >nul
cd /d "$bin"
cl /nologo /EHsc /O2 /MT /W4 "$src\vigem_setup.cpp" setupapi.lib newdev.lib /Fe:vigem_setup.exe || exit /b 1
cl /nologo /EHsc /O2 /MT /W4 "$src\vigem_pad.cpp" setupapi.lib /Fe:vigem_pad.exe || exit /b 1
"@
    & cmd /c $cmd | Out-Null; if ($LASTEXITCODE) { throw 'Building the ViGEm tools failed' }
}
$pkg = (Get-ChildItem "$env:windir\System32\DriverStore\FileRepository" -Directory -Filter 'vigembus.inf_amd64_*' | Sort-Object LastWriteTime -Descending | Select-Object -First 1).FullName
if (!$pkg) { throw 'No ViGEmBus driver package in the host DriverStore' }
$s = New-PSSession -VMName $VmName -Credential (Get-D2LabGuestCredential)
try {
    Invoke-Command -Session $s { New-Item -ItemType Directory -Force C:\D2Lab\vigem\driver | Out-Null }
    Copy-Item -ToSession $s -Force -Path "$pkg\*" -Destination 'C:\D2Lab\vigem\driver'
    # Copy a tool only when it differs (vigem_pad.exe is locked while the pad is plugged).
    foreach ($exe in 'vigem_setup.exe', 'vigem_pad.exe') {
        $want = (Get-FileHash "$bin\$exe").Hash
        $have = Invoke-Command -Session $s -ArgumentList $exe { param($e) $p = "C:\D2Lab\vigem\$e"; if (Test-Path $p) { (Get-FileHash $p).Hash } }
        if ($have -ne $want) {
            if ($exe -eq 'vigem_pad.exe') { Invoke-Command -Session $s { Get-Process vigem_pad -ErrorAction SilentlyContinue | Stop-Process -Force; Start-Sleep 1 } }
            Copy-Item -ToSession $s -Force -Path "$bin\$exe" -Destination 'C:\D2Lab\vigem'
        }
    }
    Invoke-Command -Session $s {
        if ((Get-AuthenticodeSignature C:\D2Lab\vigem\driver\ViGEmBus.cat).Status -ne 'Valid') { throw 'ViGEmBus catalog signature not valid' }
        $o = & C:\D2Lab\vigem\vigem_setup.exe C:\D2Lab\vigem\driver\ViGEmBus.inf; if ($LASTEXITCODE) { throw "vigem_setup: $o" }; $o
        # Replug the pad at every sign-in of the autologon account (hidden, no console window).
        Set-Content -Encoding ASCII C:\D2Lab\vigem\pad-hidden.vbs 'CreateObject("WScript.Shell").Run "cmd /c C:\D2Lab\vigem\vigem_pad.exe > C:\D2Lab\vigem\pad.log 2>&1", 0, False'
        $a = New-ScheduledTaskAction -Execute 'wscript.exe' -Argument 'C:\D2Lab\vigem\pad-hidden.vbs'
        $t = New-ScheduledTaskTrigger -AtLogOn -User "$env:COMPUTERNAME\d2lab"
        $p = New-ScheduledTaskPrincipal -UserId "$env:COMPUTERNAME\d2lab" -LogonType Interactive -RunLevel Highest
        Register-ScheduledTask -TaskName 'D2Lab-VirtualPad' -Action $a -Trigger $t -Principal $p -Settings (New-ScheduledTaskSettingsSet -AllowStartIfOnBatteries -ExecutionTimeLimit 0) -Force | Out-Null
        if (!(Get-Process vigem_pad -ErrorAction SilentlyContinue)) { Start-ScheduledTask -TaskName 'D2Lab-VirtualPad'; Start-Sleep -Seconds 4 }
        "pad process running: " + [bool](Get-Process vigem_pad -ErrorAction SilentlyContinue)
    }
} finally { Remove-PSSession $s }
