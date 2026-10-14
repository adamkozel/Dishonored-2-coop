# Finishes a freshly installed lab VM up to the user's Steam login: waits for the autologon
# desktop, applies guest settings, attaches the GPU partition with the host NVIDIA driver, and
# installs Steam. Each step logs to work/vm-lab/build-log.txt. pwsh 7, Hyper-V Administrators.
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')
$log = Join-Path $VmLab 'build-log.txt'
function Log($m) { $line = "{0:u} {1}" -f (Get-Date), $m; Write-Host $line; Add-Content -LiteralPath $log $line }
try {
    Log "waiting for the autologon desktop"
    Wait-D2LabVMReady -TimeoutMin 45 -Desktop | Out-Null
    Log "desktop ready"
    $g = & (Join-Path $PSScriptRoot 'Initialize-D2LabGuest.ps1')
    Log "guest initialized: $($g.os) build $($g.build), user $($g.user)"
    & (Join-Path $PSScriptRoot 'Enable-D2LabVMGpu.ps1') *>&1 | ForEach-Object { Log "gpu: $_" }
    Wait-D2LabVMReady -TimeoutMin 15 -Desktop | Out-Null
    $s = & (Join-Path $PSScriptRoot 'Install-D2LabSteam.ps1')
    Log "steam installed: $($s.version), signer $($s.signer -replace ',.*$','')"
    Log "READY for the user's Steam login"
} catch {
    Log "FAILED: $($_.Exception.Message)"
    throw
}
