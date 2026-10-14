# Prepares the guest for unattended lab use. Runs on the host; executes inside the VM through
# PowerShell Direct. Idempotent. pwsh 7, member of "Hyper-V Administrators".
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')
Invoke-D2LabVM {
    $ErrorActionPreference = 'Stop'
    # Never sleep, lock or blank the console session the game renders in.
    powercfg /setactive SCHEME_MIN | Out-Null
    'standby-timeout-ac', 'monitor-timeout-ac', 'hibernate-timeout-ac', 'disk-timeout-ac' | ForEach-Object { powercfg /change $_ 0 }
    New-Item -Force 'HKLM:\SOFTWARE\Policies\Microsoft\Windows\Personalization' | Out-Null
    Set-ItemProperty 'HKLM:\SOFTWARE\Policies\Microsoft\Windows\Personalization' NoLockScreen 1 -Type DWord
    Set-ItemProperty 'HKCU:\Control Panel\Desktop' ScreenSaveActive 0
    # Windows Update may download, but never reboots while someone is signed in (the lab account always is).
    New-Item -Force 'HKLM:\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate\AU' | Out-Null
    Set-ItemProperty 'HKLM:\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate\AU' NoAutoRebootWithLoggedOnUsers 1 -Type DWord
    Set-ItemProperty 'HKLM:\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate\AU' AUOptions 3 -Type DWord   # download, notify to install
    # Game-friendly defaults: no Game DVR overlay prompts, visible file extensions for tooling.
    Set-ItemProperty 'HKCU:\System\GameConfigStore' GameDVR_Enabled 0 -ErrorAction SilentlyContinue
    Set-ItemProperty 'HKCU:\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced' HideFileExt 0
    New-Item -ItemType Directory -Force 'C:\D2Lab' | Out-Null   # lab tooling, captures and instance state live here
    [pscustomobject]@{ computer = $env:COMPUTERNAME; os = (Get-CimInstance Win32_OperatingSystem).Caption; build = [Environment]::OSVersion.Version.ToString(); user = $env:USERNAME }
}
