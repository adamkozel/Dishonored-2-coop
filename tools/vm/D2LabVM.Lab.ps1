# Host-side control of a lab role running INSIDE the VM (guest workspace C:\D2Lab\ws, mirrored by
# Sync-D2LabVMWorkspace.ps1). Each function runs the lab's own scripts in the guest with the guest's
# pinned pwsh 7, so the guest instance behaves exactly like a host lab instance:
#   Start-D2LabVMRole / Stop-D2LabVMRole       launch (on the visible desktop) / stop the guest instance
#   Send-D2LabVMCommand / Send-D2LabVMInput    Send-D2Command.ps1 / Send-D2Input.ps1 in the guest
#   Get-D2LabVMStatus                          coop-status.json (or a sidecar, e.g. -Sidecar online)
#   Invoke-D2LabVMProbe                        bin\d2_runtime_probe.exe <pid> <mode> [arg] in the guest
#   Invoke-D2LabVMLabScript                    any lab script by name, with arguments
# Dot-source after D2LabVM.Common.ps1.
$script:GuestWs = 'C:\D2Lab\ws'
$script:GuestLab = 'C:\D2Lab\ws\outputs\d2-coop-lab'
$script:GuestPwsh = 'C:\D2Lab\pwsh\pwsh.exe'

function Invoke-D2LabVMLabScript {
    # Runs outputs/d2-coop-lab/<Script> in the guest (PowerShell Direct session, guest pwsh 7).
    param([Parameter(Mandatory)][string]$Script, [string[]]$Arguments = @())
    Invoke-D2LabVM -ArgumentList $GuestPwsh, (Join-Path $GuestLab $Script), (, $Arguments) -ScriptBlock {
        param($pwsh, $file, $a)
        $out = & $pwsh -NoProfile -ExecutionPolicy Bypass -File $file @a 2>&1
        [pscustomobject]@{ exitCode = $LASTEXITCODE; output = ($out | ForEach-Object { "$_" }) }
    }
}

function Send-D2LabVMCommand {
    param([Parameter(Mandatory)][string]$Command, [ValidateSet('host', 'client')][string]$Role = 'client', [switch]$WaitConsumed, [int]$ReceiptTimeoutMs = 8000)
    $a = @('-Role', $Role, '-Command', $Command)
    if ($WaitConsumed) { $a += @('-WaitConsumed', '-ReceiptTimeoutMs', "$ReceiptTimeoutMs") }
    $r = Invoke-D2LabVMLabScript -Script 'Send-D2Command.ps1' -Arguments $a
    if ($r.exitCode) { throw "Guest Send-D2Command failed ($($r.exitCode)): $($r.output -join ' ')" }
    $r.output
}

function Send-D2LabVMInput {
    param([ValidateSet('host', 'client')][string]$Role = 'client', [string[]]$Button = @('Neutral'), [int]$DurationMs = 400,
        [int]$LeftX = 0, [int]$LeftY = 0, [int]$RightX = 0, [int]$RightY = 0, [int]$LeftTrigger = 0, [int]$RightTrigger = 0)
    $a = @('-Role', $Role, '-Button') + $Button + @('-DurationMs', "$DurationMs", '-LeftX', "$LeftX", '-LeftY', "$LeftY", '-RightX', "$RightX", '-RightY', "$RightY", '-LeftTrigger', "$LeftTrigger", '-RightTrigger', "$RightTrigger")
    $r = Invoke-D2LabVMLabScript -Script 'Send-D2Input.ps1' -Arguments $a
    if ($r.exitCode) { throw "Guest Send-D2Input failed ($($r.exitCode)): $($r.output -join ' ')" }
}

function Get-D2LabVMStatus {
    param([ValidateSet('host', 'client')][string]$Role = 'client', [string]$Sidecar = '')
    Invoke-D2LabVM -ArgumentList "$GuestWs\work\lab-instances\$Role\coop-status.json$(if ($Sidecar) { ".$Sidecar.json" })" -ScriptBlock {
        param($p)
        for ($i = 0; $i -lt 40; $i++) { try { return (Get-Content -Raw -LiteralPath $p -ErrorAction Stop | ConvertFrom-Json -ErrorAction Stop) } catch { Start-Sleep -Milliseconds 50 } }
        throw "Status unavailable: $p"
    }
}

function Invoke-D2LabVMProbe {
    param([Parameter(Mandatory)][string]$Mode, [string]$Arg = '', [ValidateSet('host', 'client')][string]$Role = 'client')
    Invoke-D2LabVM -ArgumentList $GuestWs, $GuestLab, $Role, $Mode, $Arg -ScriptBlock {
        param($ws, $lab, $role, $mode, $arg)
        $l = Get-Content -Raw "$ws\work\lab-instances\$role\launch.json" | ConvertFrom-Json
        $raw = if ($arg) { & "$lab\bin\d2_runtime_probe.exe" $l.pid $mode $arg } else { & "$lab\bin\d2_runtime_probe.exe" $l.pid $mode }
        if ($LASTEXITCODE) { throw "probe $mode failed ($LASTEXITCODE)" }
        $raw | ConvertFrom-Json
    }
}

function Start-D2LabVMRole {
    # Launches the guest instance with Start-D2Lab.ps1 -AssetExperiment on the guest's visible desktop
    # (the game must not start in PowerShell Direct's hidden session), then waits for its status file.
    param([ValidateSet('host', 'client')][string]$Role = 'client', [int]$TimeoutSec = 120)
    $log = "C:\D2Lab\start-$Role.log"
    Invoke-D2LabVM -ArgumentList $GuestWs, $Role, $log -ScriptBlock {
        param($ws, $role, $log)
        $l = "$ws\work\lab-instances\$role\launch.json"
        if (Test-Path $l) { $j = Get-Content -Raw $l | ConvertFrom-Json; if (Get-Process -Id $j.pid -ErrorAction SilentlyContinue | Where-Object Path -eq $j.executable) { throw "Guest $role instance already running (pid $($j.pid))" } }
        Remove-Item $log, "$ws\work\lab-instances\$role\coop-status.json" -ErrorAction SilentlyContinue
    }
    Start-D2LabVMHidden -CommandLine "`"$GuestPwsh`" -NoProfile -ExecutionPolicy Bypass -File `"$GuestLab\Start-D2Lab.ps1`" -Role $Role -AssetExperiment > `"$log`" 2>&1"
    $deadline = (Get-Date).AddSeconds($TimeoutSec)
    while ((Get-Date) -lt $deadline) {
        Start-Sleep -Seconds 3
        $st = Invoke-D2LabVM -ArgumentList $GuestWs, $Role, $log -ScriptBlock {
            param($ws, $role, $log)
            $l = "$ws\work\lab-instances\$role\launch.json"; $s = "$ws\work\lab-instances\$role\coop-status.json"
            $alive = $false; if (Test-Path $l) { $j = Get-Content -Raw $l | ConvertFrom-Json; $alive = [bool](Get-Process -Id $j.pid -ErrorAction SilentlyContinue) }
            [pscustomobject]@{ alive = $alive; status = (Test-Path $s); log = ((Get-Content $log -ErrorAction SilentlyContinue) -join ' ') }
        }
        if ($st.alive -and $st.status) { return $st }
        if ($st.log -match 'Exception|throw|error' -and !$st.alive) { throw "Guest Start-D2Lab failed: $($st.log)" }
    }
    throw "Guest $Role instance did not come up within $TimeoutSec s: $($st.log)"
}

function Stop-D2LabVMRole {
    param([ValidateSet('host', 'client')][string]$Role = 'client')
    Invoke-D2LabVM -ArgumentList $GuestWs, $Role -ScriptBlock {
        param($ws, $role)
        $l = "$ws\work\lab-instances\$role\launch.json"
        if (!(Test-Path $l)) { return 'no launch.json' }
        $j = Get-Content -Raw $l | ConvertFrom-Json
        $p = Get-Process -Id $j.pid -ErrorAction SilentlyContinue | Where-Object Path -eq $j.executable
        if ($p) { $p | Stop-Process -Force; "stopped pid $($j.pid)" } else { 'not running' }
    }
}
