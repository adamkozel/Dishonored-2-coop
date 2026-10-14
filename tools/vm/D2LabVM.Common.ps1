# Shared constants and helpers for the D2 co-op second-computer VM (Hyper-V, Windows 11 guest).
# Dot-source from the other tools/vm scripts. Requires Hyper-V and membership in
# "Hyper-V Administrators" (no elevation needed afterwards).
# Never committed: work/vm-lab/ (ISO, VHDX, answer ISO, DPAPI-protected guest credential).
$script:VmName     = 'D2CoopVM'
$script:RepoRoot   = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$script:VmLab      = Join-Path $RepoRoot 'work/vm-lab'
$script:VmDisk     = Join-Path $VmLab "$VmName.vhdx"
$script:CredFile   = Join-Path $VmLab 'guest-credential.xml'   # Export-Clixml: DPAPI, this Windows user only
$script:GuestUser  = 'd2lab'

function New-D2LabGuestCredential {
    # Random 24-character password; stored only DPAPI-protected, never printed.
    if (Test-Path $CredFile) { throw "Guest credential already exists: $CredFile" }
    $alphabet = [char[]]'ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789!#%+=?@'
    $bytes = [byte[]]::new(24); [Security.Cryptography.RandomNumberGenerator]::Fill($bytes)
    $plain = -join ($bytes | ForEach-Object { $alphabet[$_ % $alphabet.Length] })
    $secure = ConvertTo-SecureString $plain -AsPlainText -Force
    [pscredential]::new($GuestUser, $secure) | Export-Clixml -LiteralPath $CredFile
    return $plain   # caller writes it into the answer file only
}

function Get-D2LabGuestCredential {
    if (!(Test-Path $CredFile)) { throw "No guest credential at $CredFile (run New-D2LabVM.ps1 first)" }
    Import-Clixml -LiteralPath $CredFile
}

function Invoke-D2LabVM {
    # Run a script block inside the guest through PowerShell Direct (VMBus; no network needed).
    param([Parameter(Mandatory)][scriptblock]$ScriptBlock, [object[]]$ArgumentList = @(), [int]$TimeoutSec = 0)
    $p = @{ VMName = $VmName; Credential = (Get-D2LabGuestCredential); ScriptBlock = $ScriptBlock; ArgumentList = $ArgumentList; ErrorAction = 'Stop' }
    Invoke-Command @p
}

function Wait-D2LabVMReady {
    # Wait until PowerShell Direct answers (guest booted, autologon account ready).
    param([int]$TimeoutMin = 75)
    $deadline = (Get-Date).AddMinutes($TimeoutMin)
    while ((Get-Date) -lt $deadline) {
        try { $n = Invoke-D2LabVM { $env:COMPUTERNAME }; if ($n) { return $n } } catch { }
        Start-Sleep -Seconds 20
    }
    throw "Guest not reachable through PowerShell Direct within $TimeoutMin min"
}

function Save-D2LabVMScreen {
    # Host-side screenshot of the VM console (no viewer window, no guest access needed):
    # Msvm_VirtualSystemManagementService.GetVirtualSystemThumbnailImage returns RGB565 pixels.
    param([Parameter(Mandatory)][string]$Path, [int]$Width = 1280, [int]$Height = 720)
    $vm = Get-CimInstance -Namespace root/virtualization/v2 -ClassName Msvm_ComputerSystem -Filter "ElementName='$VmName'"
    $settings = Get-CimAssociatedInstance -InputObject $vm -ResultClassName Msvm_VirtualSystemSettingData | Where-Object VirtualSystemType -eq 'Microsoft:Hyper-V:System:Realized'
    $svc = Get-CimInstance -Namespace root/virtualization/v2 -ClassName Msvm_VirtualSystemManagementService
    $r = Invoke-CimMethod -InputObject $svc -MethodName GetVirtualSystemThumbnailImage -Arguments @{ TargetSystem = $settings; WidthPixels = [uint16]$Width; HeightPixels = [uint16]$Height }
    if ($r.ReturnValue -ne 0 -or !$r.ImageData) { throw "Thumbnail failed ($($r.ReturnValue))" }
    Add-Type -AssemblyName System.Drawing
    $bmp = [Drawing.Bitmap]::new($Width, $Height, [Drawing.Imaging.PixelFormat]::Format16bppRgb565)
    $rect = [Drawing.Rectangle]::new(0, 0, $Width, $Height)
    $data = $bmp.LockBits($rect, [Drawing.Imaging.ImageLockMode]::WriteOnly, $bmp.PixelFormat)
    # ImageData carries 4 bytes past the pixels; copy exactly width*height*2 (stride is 4-byte aligned for even widths).
    if ($Width % 2) { throw 'Width must be even' }
    [Runtime.InteropServices.Marshal]::Copy([byte[]]$r.ImageData, 0, $data.Scan0, $Width * $Height * 2)
    $bmp.UnlockBits($data); $bmp.Save([IO.Path]::GetFullPath($Path), [Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
    Get-Item $Path
}

function Send-D2LabVMKey {
    # Type a key into the VM console (Msvm_Keyboard); used for "Press any key to boot from CD or DVD".
    param([int]$VirtualKey = 0x0D, [int]$Count = 1)
    $vm = Get-CimInstance -Namespace root/virtualization/v2 -ClassName Msvm_ComputerSystem -Filter "ElementName='$VmName'"
    $kb = Get-CimAssociatedInstance -InputObject $vm -ResultClassName Msvm_Keyboard
    for ($i = 0; $i -lt $Count; $i++) { Invoke-CimMethod -InputObject $kb -MethodName TypeKey -Arguments @{ keyCode = $VirtualKey } | Out-Null; Start-Sleep -Milliseconds 400 }
}
