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
    # Wait until PowerShell Direct answers. -Desktop also requires Windows setup/OOBE to be
    # finished and the autologon desktop (explorer.exe for d2lab) to be up: PowerShell Direct
    # already answers during the post-install "Installing x%" phase (first build).
    param([int]$TimeoutMin = 75, [switch]$Desktop)
    $deadline = (Get-Date).AddMinutes($TimeoutMin)
    while ((Get-Date) -lt $deadline) {
        try {
            $r = Invoke-D2LabVM {
                $s = Get-ItemProperty 'HKLM:\SYSTEM\Setup'
                [pscustomobject]@{ name = $env:COMPUTERNAME; setupDone = ($s.SystemSetupInProgress -eq 0 -and $s.OOBEInProgress -eq 0)
                    desktop = @(Get-Process explorer -IncludeUserName -ErrorAction SilentlyContinue | Where-Object UserName -like '*\d2lab').Count -gt 0 }
            }
            if ($r.name -and (!$Desktop -or ($r.setupDone -and $r.desktop))) { return $r.name }
        } catch { }
        Start-Sleep -Seconds 20
    }
    throw "Guest not ready through PowerShell Direct within $TimeoutMin min"
}

function Start-D2LabVMInteractive {
    # Start a program on the guest's signed-in console desktop (d2lab), not in PowerShell Direct's
    # hidden logon session: a one-shot interactive scheduled task (/IT) run immediately.
    param([Parameter(Mandatory)][string]$FilePath, [string]$Arguments = '', [string]$WorkingDirectory = '')
    Invoke-D2LabVM -ArgumentList $FilePath, $Arguments, $WorkingDirectory -ScriptBlock {
        param($f, $a, $w)
        $action = if ($w) { New-ScheduledTaskAction -Execute $f -Argument $a -WorkingDirectory $w } elseif ($a) { New-ScheduledTaskAction -Execute $f -Argument $a } else { New-ScheduledTaskAction -Execute $f }
        $principal = New-ScheduledTaskPrincipal -UserId "$env:COMPUTERNAME\d2lab" -LogonType Interactive -RunLevel Highest
        $name = 'D2Lab-Interactive-' + [guid]::NewGuid().ToString('N').Substring(0, 8)
        Register-ScheduledTask -TaskName $name -Action $action -Principal $principal -Settings (New-ScheduledTaskSettingsSet -AllowStartIfOnBatteries -ExecutionTimeLimit 0) | Out-Null
        Start-ScheduledTask -TaskName $name
        Start-Sleep -Seconds 2
        Unregister-ScheduledTask -TaskName $name -Confirm:$false
    }
}

function Start-D2LabVMHidden {
    # Like Start-D2LabVMInteractive, but through wscript (no console, window style 0), so the
    # started process never takes focus from the game. A powershell.exe start stole focus and
    # Dishonored 2 paused (first FPS measurement, 2026-10-14).
    # The command line goes into a guest .cmd file: quotes passed through wscript's own argument
    # parsing were mangled (Start-D2Lab never ran, 2026-10-15).
    param([Parameter(Mandatory)][string]$CommandLine)
    $vbs = 'C:\D2Lab\run-hidden.vbs'
    $cmd = 'C:\D2Lab\run-' + [guid]::NewGuid().ToString('N').Substring(0, 8) + '.cmd'
    Invoke-D2LabVM -ArgumentList $vbs, $cmd, $CommandLine -ScriptBlock {
        param($v, $c, $line)
        Set-Content -LiteralPath $v -Encoding ASCII -Value 'CreateObject("WScript.Shell").Run WScript.Arguments(0), 0, False'
        Set-Content -LiteralPath $c -Encoding ASCII -Value "@echo off`r`n$line`r`ndel `"%~f0`""
    }
    Start-D2LabVMInteractive -FilePath 'wscript.exe' -Arguments "$vbs $cmd"
}

function Start-D2LabVMGame {
    # Launches Dishonored 2 in the VM through Steam (second account) on the visible desktop, with
    # the lab's low-load settings also passed as +set arguments, as Start-D2Lab does for the host
    # instances: the config file alone did not stop the game pausing on focus loss.
    param([switch]$ShowFps, [ValidateRange(0, 4)][int]$SwapInterval = 1)
    $set = [ordered]@{ com_showLoadingScreen = 0; r_renderQualityPresets = 6; r_maxDynamicDecalsPreset = 0; r_textureDetailsPreset = 0; r_viewDistPreset = 0
        r_modelsQualityPreset = 0; r_oceanQualityPreset = 0; r_shadowQualityPreset = 0; r_downsampleQualityPreset = 0; r_AntiAliasingPreset = 0
        r_skipCameraMotionBlur = 1; r_skipSSAO = 1; r_fullscreen = 0; r_windowWidth = 944; r_windowHeight = 540; r_swapInterval = $SwapInterval; win_pauseOnFocusLoss = 0 }
    if ($ShowFps) { $set.com_showFPS = 1 }
    $args = '-applaunch 403640 ' + (($set.GetEnumerator() | ForEach-Object { "+set $($_.Key) $($_.Value)" }) -join ' ')
    if (Invoke-D2LabVM { [bool](Get-Process Dishonored2 -ErrorAction SilentlyContinue) }) { throw 'Dishonored 2 is already running in the VM' }
    Start-D2LabVMInteractive -FilePath 'C:\Program Files (x86)\Steam\steam.exe' -Arguments $args
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
    # Type a key into the VM console (Msvm_Keyboard), e.g. "Press any key to boot from CD or DVD".
    # -HoldMs presses and holds (walking, hold-to-skip), then releases.
    param([int]$VirtualKey = 0x0D, [int]$Count = 1, [int]$HoldMs = 0)
    $vm = Get-CimInstance -Namespace root/virtualization/v2 -ClassName Msvm_ComputerSystem -Filter "ElementName='$VmName'"
    $kb = Get-CimAssociatedInstance -InputObject $vm -ResultClassName Msvm_Keyboard
    for ($i = 0; $i -lt $Count; $i++) {
        if ($HoldMs -gt 0) {
            Invoke-CimMethod -InputObject $kb -MethodName PressKey -Arguments @{ keyCode = $VirtualKey } | Out-Null
            Start-Sleep -Milliseconds $HoldMs
            Invoke-CimMethod -InputObject $kb -MethodName ReleaseKey -Arguments @{ keyCode = $VirtualKey } | Out-Null
        } else { Invoke-CimMethod -InputObject $kb -MethodName TypeKey -Arguments @{ keyCode = $VirtualKey } | Out-Null }
        Start-Sleep -Milliseconds 400
    }
}

function Get-D2LabVMFps {
    # Records the VM game window for -Seconds (guest/Measure-D2Fps.ps1 on the interactive desktop)
    # and returns delivered-frame statistics from the clip's frame timestamps.
    # -During runs on the host while the capture records (e.g. holding W to walk).
    param([string]$Name = 'fps', [int]$Seconds = 10, [scriptblock]$During)
    $guestScript = Join-Path $PSScriptRoot 'guest/Measure-D2Fps.ps1'
    $s = New-PSSession -VMName $VmName -Credential (Get-D2LabGuestCredential)
    try { Copy-Item -ToSession $s -Path $guestScript -Destination 'C:\D2Lab\Measure-D2Fps.ps1' -Force } finally { Remove-PSSession $s }
    Start-D2LabVMHidden -CommandLine "powershell.exe -NoProfile -ExecutionPolicy Bypass -WindowStyle Hidden -File C:\D2Lab\Measure-D2Fps.ps1 -Name $Name -Seconds $Seconds"
    $sw = [Diagnostics.Stopwatch]::StartNew()
    if ($During) { & $During }
    $left = ($Seconds + 4) - $sw.Elapsed.TotalSeconds; if ($left -gt 0) { Start-Sleep -Seconds ([math]::Ceiling($left)) }
    Invoke-D2LabVM -ArgumentList $Name -ScriptBlock {
        param($n)
        $done = "C:\D2Lab\fps\$n.done"
        for ($i = 0; $i -lt 30 -and !(Test-Path $done); $i++) { Start-Sleep -Seconds 1 }
        $status = if (Test-Path $done) { (Get-Content $done) } else { 'no result' }
        $mp4 = "C:\D2Lab\fps\$n.mp4"
        if (!(Test-Path $mp4)) { return [pscustomobject]@{ status = $status; log = (Get-Content "C:\D2Lab\fps\$n.log" -ErrorAction SilentlyContinue) -join ' ' } }
        $t = & C:\D2Lab\tools\ffprobe.exe -v error -select_streams v:0 -show_entries packet=pts_time -of csv=p=0 $mp4 | ForEach-Object { [double]$_ } | Sort-Object
        $d = for ($i = 1; $i -lt $t.Count; $i++) { ($t[$i] - $t[$i - 1]) * 1000 }
        $span = $t[-1] - $t[0]; $sorted = $d | Sort-Object
        [pscustomobject]@{
            status = $status; frames = $t.Count; seconds = [math]::Round($span, 2)
            avgFps = [math]::Round(($t.Count - 1) / $span, 1)
            medianFrameMs = [math]::Round($sorted[[int]($sorted.Count / 2)], 1)
            p99FrameMs = [math]::Round($sorted[[int]($sorted.Count * 0.99)], 1)
            maxFrameMs = [math]::Round($sorted[-1], 1)
            onePercentLowFps = [math]::Round(1000 / (($sorted | Select-Object -Last ([math]::Max(1, [int]($sorted.Count / 100)))) | Measure-Object -Average).Average, 1)
            hitches50ms = @($d | Where-Object { $_ -gt 50 }).Count
        }
    }
}
