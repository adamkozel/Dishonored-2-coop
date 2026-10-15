param([string]$Take = 'product-two-account-v1225', [string]$Package = 'outputs/release/d2coop-v1213f')
# Stage 2/#12 product path across two machines and two Steam accounts, the way players use it:
#   VM   (second account): release package installed into its own Steam game with the package's
#        installer; game launched through Steam -> product mode, main menu.
#   Host (main account):   product instance from the lab copy (work/asset-game) with a temporary
#        d2coop.cfg and an ISOLATED profile holding the fixture save; real keystrokes to its window:
#        title -> Continue -> loading -> F7 (host: friends-only lobby + invite overlay).
#   Join: steam://joinlobby/<app>/<lobby>/<host> in the VM (Steam's "Join Game" route).
#   Pass: both product sessions connected (phase 5, state 3) and fresh.
# Cleanup: both games closed, the package UNINSTALLED from the VM with its own uninstaller (rollback),
# the temporary host d2coop folder removed, protected saves checked. SteamIDs are never recorded.
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..')); $out = Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1"; . "$root/tools/vm/D2LabVM.Lab.ps1"
$game = Join-Path $root 'work/asset-game'; $data = Join-Path $game 'd2coop'; $exe = Join-Path $game 'Dishonored2.exe'
$pkg = Join-Path $root $Package; $vmGame = 'C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
$mutex = [Threading.Mutex]::new($false, 'Local\D2CoopLab_NativeOperation_01a11a65'); $owned = $false; $failure = $null; $proc = $null; $notes = [ordered]@{}
$installedVm = $false; $createdData = $false
function Save($p, $v) { $v | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $p }
$safe = 'active', 'host', 'phase', 'result', 'connectionState', 'joinRequests', 'role', 'inviteOpened', 'lobbyMembers', 'steamReady'
function Safe($o) { if (!$o) { return $null }; $h = [ordered]@{}; foreach ($n in $safe) { if ($o.PSObject.Properties[$n]) { $h[$n] = $o.$n } }; [pscustomobject]$h }
function Json($p) { for ($i = 0; $i -lt 30; $i++) { try { return (Get-Content -LiteralPath $p -Raw -ErrorAction Stop | ConvertFrom-Json) } catch { Start-Sleep -Milliseconds 60 } }; $null }
function VmJson($p) { Invoke-D2LabVM -ArgumentList $p -ScriptBlock { param($p) for ($i = 0; $i -lt 30; $i++) { try { return (Get-Content -LiteralPath $p -Raw -ErrorAction Stop | ConvertFrom-Json) } catch { Start-Sleep -Milliseconds 60 } }; $null } }
Add-Type -Namespace D2 -Name Win -MemberDefinition @'
[DllImport("user32.dll")] public static extern bool SetForegroundWindow(System.IntPtr h);
[DllImport("user32.dll")] public static extern bool ShowWindow(System.IntPtr h, int c);
[DllImport("user32.dll")] public static extern System.IntPtr GetForegroundWindow();
[DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(System.IntPtr h, out uint p);
[DllImport("user32.dll")] public static extern bool AttachThreadInput(uint a, uint b, bool f);
[DllImport("kernel32.dll")] public static extern uint GetCurrentThreadId();
[DllImport("user32.dll")] public static extern void keybd_event(byte vk, byte scan, uint flags, System.UIntPtr extra);
[DllImport("user32.dll")] public static extern uint MapVirtualKey(uint code, uint type);
'@
function Focus($hwnd) {
    $fg = [D2.Win]::GetForegroundWindow(); $pid0 = 0; $t = [D2.Win]::GetWindowThreadProcessId($fg, [ref]$pid0); $me = [D2.Win]::GetCurrentThreadId()
    [D2.Win]::AttachThreadInput($me, $t, $true) | Out-Null; [D2.Win]::ShowWindow($hwnd, 9) | Out-Null; [D2.Win]::SetForegroundWindow($hwnd) | Out-Null; [D2.Win]::AttachThreadInput($me, $t, $false) | Out-Null
    Start-Sleep -Milliseconds 300; return ([D2.Win]::GetForegroundWindow() -eq $hwnd)
}
function Key($vk, $holdMs = 120) { $sc = [byte][D2.Win]::MapVirtualKey($vk, 0); [D2.Win]::keybd_event([byte]$vk, $sc, 0, [UIntPtr]::Zero); Start-Sleep -Milliseconds $holdMs; [D2.Win]::keybd_event([byte]$vk, $sc, 2, [UIntPtr]::Zero); Start-Sleep -Milliseconds 400 }
try {
    try { $owned = $mutex.WaitOne(0) } catch [Threading.AbandonedMutexException] { $owned = $true }; if (!$owned) { throw 'Native operation active' }
    if (Test-Path $out) { throw 'Unique take required' }; New-Item -ItemType Directory $out | Out-Null; Copy-Item $PSCommandPath "$out/run-script.ps1"
    if (Test-Path $data) { throw 'asset-game already has a d2coop folder; refusing to overwrite' }
    $manifest = Get-Content "$pkg/manifest.json" -Raw | ConvertFrom-Json
    $notes.package = [ordered]@{ path = $Package; candidate = $manifest.candidate; dll = ($manifest.files | Where-Object path -eq 'dinput8.dll').sha256 }
    & "$root/tools/vm/Test-D2LabVM.ps1" -Repair | Out-Null; if ($LASTEXITCODE) { throw 'Lab VM not ready' }
    # --- VM: install the package into its own Steam game with the package's installer, launch through Steam.
    try { Stop-D2LabVMRole -Role client | Out-Null } catch { }
    Invoke-D2LabVM { Get-Process Dishonored2 -ErrorAction SilentlyContinue | Stop-Process -Force; Start-Sleep 2 }
    $s = New-PSSession -VMName $VmName -Credential (Get-D2LabGuestCredential)
    try {
        Invoke-Command -Session $s { Remove-Item -Recurse -Force C:\D2Lab\release -ErrorAction SilentlyContinue; New-Item -ItemType Directory -Force C:\D2Lab\release | Out-Null }
        Copy-Item -ToSession $s -Recurse -Force -Path $pkg -Destination 'C:\D2Lab\release'
        $inst = Invoke-Command -Session $s -ArgumentList (Split-Path $pkg -Leaf), $vmGame { param($leaf, $g)
            $o = & C:\D2Lab\pwsh\pwsh.exe -NoProfile -ExecutionPolicy Bypass -File "C:\D2Lab\release\$leaf\Install-D2Coop.ps1" -GameDirectory $g 2>&1
            [pscustomobject]@{ exit = $LASTEXITCODE; output = ($o | ForEach-Object { "$_" } | Select-Object -Last 6); receipt = Test-Path "$g\d2coop-install-receipt.json"; cfg = Test-Path "$g\d2coop\d2coop.cfg" } }
    } finally { Remove-PSSession $s }
    $notes.vmInstall = $inst
    if ($inst.exit -or !$inst.receipt) { throw "Package install in the VM failed: $($inst.output -join ' | ')" }
    $installedVm = $true
    Start-D2LabVMGame
    $d = (Get-Date).AddSeconds(150); do { Start-Sleep 5; $vp = VmJson "$vmGame\d2coop\coop-status.json.product.json" } while ((Get-Date) -lt $d -and !$vp)
    if (!$vp) { throw 'VM product mode did not report (no product status)' }
    $notes.vmProductBoot = [ordered]@{ productStatus = $true; files = @(Invoke-D2LabVM -ArgumentList $vmGame { param($g) Get-ChildItem "$g\d2coop" -File | ForEach-Object Name }) }
    # --- Host: product instance with an isolated profile holding the fixture save.
    $prof = Join-Path $out 'profile'; $savegame = Join-Path $prof 'base/savegame'
    New-Item -ItemType Directory -Force $savegame | Out-Null
    Copy-Item -Recurse "$root/work/lab-instances/host/base/savegame/SLOT81" $savegame
    Copy-Item "$root/work/lab-instances/host/base/savegame/profile.bin" $savegame -ErrorAction SilentlyContinue
    New-Item -ItemType Directory $data | Out-Null; $createdData = $true
    "[d2coop]`nenabled=1`nsavegame=$($savegame.Replace('/','\'))`n" | Set-Content -LiteralPath (Join-Path $data 'd2coop.cfg') -Encoding ascii
    foreach ($who in 'corvo', 'emily') { Copy-Item (Join-Path $pkg "files/d2coop/$who-choke-bank.bin") (Join-Path $data "$who-choke-bank.bin") }
    $launchArgs = "+set fs_savepath `"$($prof.Replace('\','/'))`" +set r_fullscreen 0 +set r_windowWidth 944 +set r_windowHeight 540 +set r_windowPosX 480 +set r_windowPosY 300 +set win_pauseOnFocusLoss 0 +set r_renderQualityPresets 6 +set r_textureDetailsPreset 0 +set r_shadowQualityPreset 0 +set r_skipSSAO 1"
    $psi = [Diagnostics.ProcessStartInfo]::new($exe, $launchArgs); $psi.WorkingDirectory = $game; $psi.UseShellExecute = $false
    foreach ($k in @($psi.Environment.Keys)) { if ($k -like 'D2COOP_*') { $psi.Environment.Remove($k) | Out-Null } }
    $psi.Environment['SteamAppId'] = '403640'; $psi.Environment['SteamGameId'] = '403640'
    $proc = [Diagnostics.Process]::Start($psi); $notes.hostPid = $proc.Id
    $d = (Get-Date).AddSeconds(120); do { Start-Sleep 3; if ($proc.HasExited) { throw "Host product process exited ($($proc.ExitCode))" }; $hp = Json (Join-Path $data 'coop-status.json.product.json') } while ((Get-Date) -lt $d -and !$hp)
    if (!$hp) { throw 'Host product mode did not report' }
    Start-Sleep 8; $proc.Refresh(); $hwnd = $proc.MainWindowHandle
    # Title -> main menu -> Continue -> loading screen -> gameplay, with real keystrokes.
    $hs = $null
    foreach ($step in 1..12) {
        $hs = Json (Join-Path $data 'coop-status.json'); if ($hs.localGameLogicState -eq 1) { break }
        $notes["focus$step"] = Focus $hwnd; Key 0x0D; Start-Sleep -Seconds 6
    }
    $notes.hostGameplay = [ordered]@{ logic = $hs.localGameLogicState; map = $hs.localMap }
    if ($hs.localGameLogicState -ne 1) { throw "Host product instance did not reach gameplay (logic $($hs.localGameLogicState))" }
    # F7: host.
    Focus $hwnd | Out-Null; Key 0x76 200
    $d = (Get-Date).AddSeconds(40); do { Start-Sleep 2; $ho = Json (Join-Path $data 'coop-status.json.online.json') } while ((Get-Date) -lt $d -and !($ho.lobby -and "$($ho.lobby)" -ne '0'))
    $notes.hostOnlineAfterF7 = Safe $ho
    if (!$ho.lobby -or "$($ho.lobby)" -eq '0') { throw 'F7 did not create a lobby' }
    # Join from the VM through Steam's own join route (same as Join Game / invite accept).
    $url = "steam://joinlobby/403640/$($ho.lobby)/$($ho.self)"
    Start-D2LabVMInteractive -FilePath 'C:\Program Files (x86)\Steam\steam.exe' -Arguments $url
    $d = (Get-Date).AddSeconds(120)
    do { Start-Sleep 4; $hs = Json (Join-Path $data 'coop-status.json'); $gs = VmJson "$vmGame\d2coop\coop-status.json" } while ((Get-Date) -lt $d -and !($hs.remoteFresh -and $gs.remoteFresh))
    $notes.session = [ordered]@{ hostOnline = Safe (Json (Join-Path $data 'coop-status.json.online.json')); vmOnline = Safe (VmJson "$vmGame\d2coop\coop-status.json.online.json")
        host = [ordered]@{ logic = $hs.localGameLogicState; connectionState = $hs.connectionState; remoteFresh = $hs.remoteFresh; map = $hs.localMap }
        vm = [ordered]@{ logic = $gs.localGameLogicState; connectionState = $gs.connectionState; remoteFresh = $gs.remoteFresh; map = $gs.localMap } }
    try { Save-D2LabVMScreen -Path "$out/vm-session.png" | Out-Null } catch { }
    if (!($hs.remoteFresh -and $gs.remoteFresh)) { throw 'Product session did not connect both ways' }
} catch { $failure = $_.Exception.Message; Save "$out/failure.json" @{ message = $failure; stack = $_.ScriptStackTrace } }
finally {
    if ($owned) {
        $errors = [Collections.Generic.List[string]]::new()
        if ($proc -and !$proc.HasExited) { try { $proc.Kill(); $proc.WaitForExit(15000) | Out-Null } catch { $errors.Add("host stop: $($_.Exception.Message)") } }
        if ($createdData) { Start-Sleep 2; try { Remove-Item -Recurse -Force $data } catch { $errors.Add("host d2coop cleanup: $($_.Exception.Message)") } }
        try { Invoke-D2LabVM { Get-Process Dishonored2 -ErrorAction SilentlyContinue | Stop-Process -Force; Start-Sleep 3 } } catch { }
        if ($installedVm) {
            try { $un = Invoke-D2LabVM -ArgumentList (Split-Path $pkg -Leaf), $vmGame { param($leaf, $g)
                    $o = & C:\D2Lab\pwsh\pwsh.exe -NoProfile -ExecutionPolicy Bypass -File "C:\D2Lab\release\$leaf\Uninstall-D2Coop.ps1" -GameDirectory $g 2>&1
                    [pscustomobject]@{ exit = $LASTEXITCODE; receiptGone = !(Test-Path "$g\d2coop-install-receipt.json"); dllGone = !(Test-Path "$g\dinput8.dll"); exeHash = (Get-FileHash "$g\Dishonored2.exe").Hash.Substring(0, 16); output = ($o | ForEach-Object { "$_" } | Select-Object -Last 4) } }
                $notes.vmUninstall = $un; if ($un.exit -or !$un.receiptGone -or !$un.dllGone) { $errors.Add('VM uninstall incomplete') } } catch { $errors.Add("vm uninstall: $($_.Exception.Message)") }
        }
        foreach ($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw | ConvertFrom-Json) { if ((Get-FileHash -LiteralPath $f.Path).Hash -ne $f.Hash) { $errors.Add('Protected file changed') } }
        foreach ($r in 'host', 'client') { try { $st = Json "$root/work/lab-instances/$r/coop-status.json"; $notes["lab$r"] = "logic=$($st.localGameLogicState) fresh=$($st.remoteFresh)" } catch { } }
        Save "$out/restoration.json" @{ passed = (!$errors.Count); errors = @($errors) }; if ($errors.Count -and !$failure) { $failure = $errors -join '; ' }
        Save "$out/result.json" ([ordered]@{ passed = (!$failure); failure = $failure; notes = $notes
            scope = 'Product path, two machines/two accounts: package installed in the VM Steam game with its installer, host product instance (isolated profile) F7, VM joins via steam://joinlobby; then VM uninstall (rollback).' })
        $mutex.ReleaseMutex()
    }
    $mutex.Dispose()
}
foreach ($f in Get-ChildItem $out -File -Filter *.json) { if ((Get-Content $f.FullName -Raw) -match '7656119[0-9]{10}') { throw "SteamID leaked into $($f.Name)" } }
Get-Content "$out/result.json"; if ($failure) { throw $failure }
