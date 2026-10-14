# Attaches a partition of the host GPU (RTX 5080) to the lab VM (GPU-P) and copies the host
# NVIDIA driver into the guest, which GPU-P requires (guest uses the host's driver store).
# Re-run after every host NVIDIA driver update. pwsh 7, member of "Hyper-V Administrators".
# -Share: fraction of the GPU (VRAM, compute, encode, decode) given to the VM. The first build used
# 8-10%; the VM's default-quality game then hung its slice (LiveKernelEvent 0x141) and the host
# NVIDIA driver bugchecked in its recovery (0x50 in nvlddmkm.sys, dump read 2026-10-14). 40% leaves
# the host lab instances ample room at their low settings.
param([ValidateRange(0.05, 0.9)][double]$Share = 0.4)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')

# Host driver: the DriverStore folder of the active NVIDIA display driver.
$gpu = Get-CimInstance Win32_VideoController | Where-Object Name -like 'NVIDIA*' | Select-Object -First 1
if (!$gpu) { throw 'No NVIDIA display adapter on the host' }
$umd = ($gpu.InstalledDisplayDrivers -split ',')[0].Trim()
$storeDir = Split-Path $umd -Parent
if ($storeDir -notlike "$env:windir\System32\DriverStore\FileRepository\*") { throw "Unexpected driver path: $umd" }
$folderName = Split-Path $storeDir -Leaf
Write-Host "Host GPU: $($gpu.Name), driver $($gpu.DriverVersion), store $folderName"

# GPU partition (VM must be off).
if ((Get-VM $VmName).State -ne 'Off') { Stop-VM -Name $VmName -Force }
if (!(Get-VMGpuPartitionAdapter -VMName $VmName -ErrorAction SilentlyContinue)) { Add-VMGpuPartitionAdapter -VMName $VmName }
$v = [uint64]([math]::Round($Share * 1000000000))   # partition units: 1e9 = the whole GPU
Set-VMGpuPartitionAdapter -VMName $VmName `
    -MinPartitionVRAM $v -MaxPartitionVRAM $v -OptimalPartitionVRAM $v `
    -MinPartitionEncode $v -MaxPartitionEncode $v -OptimalPartitionEncode $v `
    -MinPartitionDecode $v -MaxPartitionDecode $v -OptimalPartitionDecode $v `
    -MinPartitionCompute $v -MaxPartitionCompute $v -OptimalPartitionCompute $v
Start-VM -Name $VmName
Wait-D2LabVMReady -TimeoutMin 10 | Out-Null

# Copy the driver store folder to the guest HostDriverStore, plus NVIDIA user-mode DLLs from System32.
$session = New-PSSession -VMName $VmName -Credential (Get-D2LabGuestCredential)
try {
    $dest = "C:\Windows\System32\HostDriverStore\FileRepository\$folderName"
    Invoke-Command -Session $session { param($d) Remove-Item -Recurse -Force $d -ErrorAction SilentlyContinue; New-Item -ItemType Directory -Force $d | Out-Null } -ArgumentList $dest
    Copy-Item -ToSession $session -Recurse -Force -Path (Join-Path $storeDir '*') -Destination $dest
    Get-ChildItem "$env:windir\System32" -Filter 'nv*' -File | Where-Object { $_.Extension -in '.dll', '.exe', '.bin', '.json' } |
        ForEach-Object { Copy-Item -ToSession $session -Force -Path $_.FullName -Destination "C:\Windows\System32\$($_.Name)" -ErrorAction SilentlyContinue }
    Invoke-Command -Session $session { Restart-Computer -Force }
} finally { Remove-PSSession $session }
Start-Sleep -Seconds 15
Wait-D2LabVMReady -TimeoutMin 10 | Out-Null
$seen = Invoke-D2LabVM { Get-CimInstance Win32_VideoController | ForEach-Object { "$($_.Name) | status $($_.Status) | driver $($_.DriverVersion)" } }
Write-Host "Guest display adapters:"; $seen | ForEach-Object { Write-Host "  $_" }
if (!($seen -match 'NVIDIA')) { throw 'The guest does not see the NVIDIA partition' }
