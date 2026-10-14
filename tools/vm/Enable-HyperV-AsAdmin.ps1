#Requires -RunAsAdministrator
# ONE-TIME, run by the user in an elevated PowerShell (system settings are the user's to change):
#   1. enables Hyper-V (platform + management tools, incl. the PowerShell module);
#   2. adds the current user to "Hyper-V Administrators" (SID S-1-5-32-578), so Claude can
#      create and control the lab VM from a normal, non-elevated session.
# A restart is required afterwards.
$ErrorActionPreference = 'Stop'
$me = [Security.Principal.WindowsIdentity]::GetCurrent().Name
$feature = Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Hyper-V-All -All -NoRestart
$group = Get-LocalGroup -SID 'S-1-5-32-578'
if (!(Get-LocalGroupMember -Group $group | Where-Object Name -eq $me)) { Add-LocalGroupMember -Group $group -Member $me }
Write-Host "Hyper-V enabled (restart needed: $($feature.RestartNeeded)); $me is in '$($group.Name)'."
Write-Host 'Restart Windows now, then tell Claude "continue".'
