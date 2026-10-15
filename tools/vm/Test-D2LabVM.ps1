# Health check for the lab VM before (overnight) use. -Repair fixes what can be fixed unattended:
# starts the VM, re-attaches the GPU partition after a host NVIDIA driver change, starts Steam,
# re-syncs changed lab binaries/scripts. Exit code 0 = ready; 1 = not ready (a reason is printed;
# a lost Steam login is a Needs-you item). pwsh 7, member of "Hyper-V Administrators".
param([switch]$Repair)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')
$problems = [Collections.Generic.List[string]]::new()
function Check($name, $ok, $detail) { $mark = if ($ok) { 'ok  ' } else { 'FAIL' }; Write-Host ("{0} {1,-22} {2}" -f $mark, $name, $detail); if (!$ok) { $problems.Add($name) } }

$vm = Get-VM $VmName
if ($vm.State -ne 'Running' -and $Repair) { Start-VM $VmName; Start-Sleep 5; $vm = Get-VM $VmName }
Check 'vm running' ($vm.State -eq 'Running') "$($vm.State), autostart $($vm.AutomaticStartAction)"
if ($vm.State -ne 'Running') { exit 1 }
try { Wait-D2LabVMReady -TimeoutMin 8 -Desktop | Out-Null; $desk = $true } catch { $desk = $false }
Check 'guest desktop' $desk 'autologon d2lab, PowerShell Direct'
if (!$desk) { exit 1 }

# GPU partition: the guest must see the NVIDIA adapter and carry the host's current driver store folder.
$hostGpu = Get-CimInstance Win32_VideoController | Where-Object Name -like 'NVIDIA*' | Select-Object -First 1
$hostStore = Split-Path (Split-Path (($hostGpu.InstalledDisplayDrivers -split ',')[0].Trim()) -Parent) -Leaf
$g = Invoke-D2LabVM -ArgumentList $hostStore -ScriptBlock {
    param($store)
    [pscustomobject]@{ nvidia = [bool](Get-CimInstance Win32_VideoController | Where-Object { $_.Name -like 'NVIDIA*' -and $_.Status -eq 'OK' })
        storeMatch = Test-Path "C:\Windows\System32\HostDriverStore\FileRepository\$store" }
}
if ((!$g.nvidia -or !$g.storeMatch) -and $Repair) { & (Join-Path $PSScriptRoot 'Enable-D2LabVMGpu.ps1') | Out-Host; Wait-D2LabVMReady -TimeoutMin 8 -Desktop | Out-Null
    $g = Invoke-D2LabVM -ArgumentList $hostStore -ScriptBlock { param($store) [pscustomobject]@{ nvidia = [bool](Get-CimInstance Win32_VideoController | Where-Object { $_.Name -like 'NVIDIA*' -and $_.Status -eq 'OK' }); storeMatch = Test-Path "C:\Windows\System32\HostDriverStore\FileRepository\$store" } } }
Check 'gpu partition' ($g.nvidia -and $g.storeMatch) "guest NVIDIA OK=$($g.nvidia), host driver $($hostGpu.DriverVersion) present in guest=$($g.storeMatch), share $([math]::Round((Get-VMGpuPartitionAdapter -VMName $VmName).MaxPartitionVRAM / 1e7))%"

# Steam: running and signed in (ActiveUser non-zero). The account id itself is never printed.
$st = { Invoke-D2LabVM { [pscustomobject]@{ running = [bool](Get-Process steam -ErrorAction SilentlyContinue); signedIn = ((Get-ItemProperty 'HKCU:\Software\Valve\Steam\ActiveProcess' -ErrorAction SilentlyContinue).ActiveUser -gt 0) } } }
$s = & $st
if (!$s.running -and $Repair) { Start-D2LabVMInteractive -FilePath 'C:\Program Files (x86)\Steam\steam.exe'; Start-Sleep 60; $s = & $st }
Check 'steam' ($s.running -and $s.signedIn) "running=$($s.running) signedIn=$($s.signedIn)$(if ($s.running -and !$s.signedIn) { ' (login lost: Needs-you)' })"

# Guest workspace in sync with the host lab (binaries and scripts).
$lab = Join-Path $RepoRoot 'outputs/d2-coop-lab'
$hostHash = @{ dll = (Get-FileHash "$lab/bin/dinput8.dll").Hash; probe = (Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash; startLab = (Get-FileHash "$lab/Start-D2Lab.ps1").Hash }
$w = { Invoke-D2LabVM { $f = 'C:\D2Lab\ws\outputs\d2-coop-lab'; $h = { param($p) if (Test-Path $p) { (Get-FileHash $p).Hash } }
    [pscustomobject]@{ dll = & $h "$f\bin\dinput8.dll"; probe = & $h "$f\bin\d2_runtime_probe.exe"; startLab = & $h "$f\Start-D2Lab.ps1"; pwsh = Test-Path 'C:\D2Lab\pwsh\pwsh.exe'
        game = Test-Path 'C:\D2Lab\ws\work\asset-game\base\game1voidmod.index'; freeGB = [math]::Round((Get-PSDrive C).Free / 1GB) } } }
$ws = & $w
$inSync = $ws.dll -eq $hostHash.dll -and $ws.probe -eq $hostHash.probe -and $ws.startLab -eq $hostHash.startLab -and $ws.pwsh -and $ws.game
if (!$inSync -and $Repair) { & (Join-Path $PSScriptRoot 'Sync-D2LabVMWorkspace.ps1') -SkipGameCopy:($ws.game) | Out-Host; $ws = & $w
    $inSync = $ws.dll -eq $hostHash.dll -and $ws.probe -eq $hostHash.probe -and $ws.startLab -eq $hostHash.startLab -and $ws.pwsh -and $ws.game }
Check 'guest workspace' $inSync "dll/probe/scripts match host=$($ws.dll -eq $hostHash.dll -and $ws.probe -eq $hostHash.probe -and $ws.startLab -eq $hostHash.startLab), game copy=$($ws.game), pwsh=$($ws.pwsh)"
Check 'guest disk' ($ws.freeGB -ge 20) "$($ws.freeGB) GB free"

# Virtual controller (#24): the game ignores the lab's simulated controller without a controller device.
$pad = { Invoke-D2LabVM { [pscustomobject]@{ process = [bool](Get-Process vigem_pad -ErrorAction SilentlyContinue)
    device = [bool](Get-PnpDevice -PresentOnly -ErrorAction SilentlyContinue | Where-Object FriendlyName -like 'Xbox 360 Controller*') } } }
$pd = & $pad
if (!($pd.process -and $pd.device) -and $Repair) { & (Join-Path $PSScriptRoot 'Enable-D2LabVMPad.ps1') | Out-Host; $pd = & $pad }
Check 'virtual pad' ($pd.process -and $pd.device) "vigem_pad running=$($pd.process), Xbox 360 controller present=$($pd.device) (start the game after it)"
Check 'time sync' ((Get-VMIntegrationService -VMName $VmName -Name 'Time Synchronization').Enabled) 'Hyper-V time synchronization (packet expiry clocks)'
if ($problems.Count) { Write-Host "NOT READY: $($problems -join ', ')"; exit 1 }
Write-Host 'READY'
