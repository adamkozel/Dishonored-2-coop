# Creates the D2 co-op lab VM and installs Windows 11 Pro unattended. Ends at a logged-in
# desktop reachable through PowerShell Direct. Run under pwsh 7 as a member of
# "Hyper-V Administrators" (elevation not required).
#   pwsh -File tools/vm/New-D2LabVM.ps1 -IsoPath work/vm-lab/Windows11_Client_x64_en-us_26300_9457.iso
param(
    [Parameter(Mandatory)][string]$IsoPath,
    [int]$ProcessorCount = 6,
    [long]$MemoryBytes = 16GB,
    [long]$DiskBytes = 220GB,
    [string]$SwitchName = 'Default Switch'
)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')
$IsoPath = (Resolve-Path $IsoPath).Path
if (Get-VM -Name $VmName -ErrorAction SilentlyContinue) { throw "VM $VmName already exists" }
New-Item -ItemType Directory -Force $VmLab | Out-Null
$log = Join-Path $VmLab 'build-log.txt'
function Log($m) { $line = "{0:u} {1}" -f (Get-Date), $m; Write-Host $line; Add-Content -LiteralPath $log $line }

# 1. Answer ISO with a fresh random guest password (only in work/vm-lab, deleted after setup).
$answerDir = Join-Path $VmLab 'answer'; $answerIso = Join-Path $VmLab 'answer.iso'
New-Item -ItemType Directory -Force $answerDir | Out-Null
$plain = New-D2LabGuestCredential
$xml = (Get-Content -Raw (Join-Path $PSScriptRoot 'autounattend.template.xml')).Replace('__USER__', $GuestUser).Replace('__PASSWORD__', [Security.SecurityElement]::Escape($plain))
[IO.File]::WriteAllText((Join-Path $answerDir 'autounattend.xml'), $xml, [Text.UTF8Encoding]::new($false))
Remove-Variable plain, xml
& "$env:windir\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -File (Join-Path $PSScriptRoot 'New-AnswerIso.ps1') -Source $answerDir -Destination $answerIso | Out-Null
Remove-Item -Recurse -Force $answerDir
Log "answer ISO built"

# 2. Generation 2 VM: fixed memory, Secure Boot, vTPM, checkpoints off, GPU-P-ready MMIO.
New-VM -Name $VmName -Generation 2 -MemoryStartupBytes $MemoryBytes -NewVHDPath $VmDisk -NewVHDSizeBytes $DiskBytes -SwitchName $SwitchName -Path $VmLab | Out-Null
Set-VMProcessor -VMName $VmName -Count $ProcessorCount
Set-VMMemory -VMName $VmName -DynamicMemoryEnabled $false
Set-VM -Name $VmName -CheckpointType Disabled -AutomaticStopAction ShutDown -AutomaticStartAction Nothing `
    -GuestControlledCacheTypes $true -LowMemoryMappedIoSpace 3GB -HighMemoryMappedIoSpace 33GB
Set-VMFirmware -VMName $VmName -EnableSecureBoot On -SecureBootTemplate MicrosoftWindows
try { Set-VMKeyProtector -VMName $VmName -NewLocalKeyProtector; Enable-VMTPM -VMName $VmName; Log "vTPM enabled" }
catch { Log "vTPM unavailable ($($_.Exception.Message)); the answer file bypasses the TPM check" }
Enable-VMIntegrationService -VMName $VmName -Name 'Guest Service Interface'
Set-VMVideo -VMName $VmName -ResolutionType Single -HorizontalResolution 1920 -VerticalResolution 1080 -ErrorAction SilentlyContinue
$dvd = Add-VMDvdDrive -VMName $VmName -Path $IsoPath -Passthru
Add-VMDvdDrive -VMName $VmName -Path $answerIso
Set-VMFirmware -VMName $VmName -FirstBootDevice $dvd
Log "VM created ($ProcessorCount vCPU, $([math]::Round($MemoryBytes/1GB)) GB, $([math]::Round($DiskBytes/1GB)) GB disk)"

# 3. Boot; answer "Press any key to boot from CD or DVD" (shown ~5 s) by typing Enter for 8 s only.
#    Longer reaches Setup's window, whose focused Cancel button then asks "quit?" (seen in the first build).
Start-VM -Name $VmName
for ($i = 0; $i -lt 8; $i++) { Start-Sleep -Seconds 1; try { Send-D2LabVMKey -VirtualKey 0x0D } catch { } }
Log "setup started; waiting for the unattended install and autologon"

# 4. Wait for the installed guest (several reboots), then detach the install media.
$name = Wait-D2LabVMReady -TimeoutMin 90 -Desktop
Log "guest reachable via PowerShell Direct as $name"
Get-VMDvdDrive -VMName $VmName | Remove-VMDvdDrive
Remove-Item -Force $answerIso
Log "install media detached, answer ISO deleted"
