#Requires -RunAsAdministrator
# ONE-TIME, run by the user in an elevated PowerShell (security settings are the user's to change).
# Microsoft Defender's machine-learning heuristic flags the co-op proxy DLL (dinput8.dll: an import-
# hooking game proxy) as Trojan:Win32/Bearfoos.A!ml and quarantines every fresh build, which breaks
# lab deploys and fixture restores (2026-10-12 and 2026-10-15, #25). This excludes ONLY:
#   1. this project folder on the host;
#   2. C:\D2Lab inside the lab VM D2CoopVM (via PowerShell Direct, using the lab's own guest credential).
# Nothing else changes; real-time protection stays on everywhere else. Undo with Remove-MpPreference.
$ErrorActionPreference = 'Stop'
$project = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
Add-MpPreference -ExclusionPath $project
Write-Host "Host: Defender excludes $project"
$cred = Join-Path $project 'work\vm-lab\guest-credential.xml'
if ((Get-Command Get-VM -ErrorAction SilentlyContinue) -and (Get-VM D2CoopVM -ErrorAction SilentlyContinue).State -eq 'Running' -and (Test-Path $cred)) {
    Invoke-Command -VMName D2CoopVM -Credential (Import-Clixml $cred) { Add-MpPreference -ExclusionPath 'C:\D2Lab'; "VM: Defender excludes C:\D2Lab" }
} else { Write-Host 'VM: not running or not set up; run this again when it is, for the VM exclusion.' }
Write-Host 'Done. Tell Claude "defender done".'
