param([string]$Take = 'product-two-account-v1314a', [string]$Zip = 'outputs/release/d2coop-v0.7.0.zip')
# Release 0.7.0 clean-PC path (#30, goal item 4) across two machines and two Steam accounts, the way players use it:
#   VM   (second account, Defender on, no exclusion over the Steam folder): the release zip is extracted into the
#        user's Downloads folder and Install.cmd runs with no arguments (it finds the game through Steam and builds
#        every game-derived file from the VM's own game). The game starts through Steam in product mode.
#   Host (main account): product instance from the lab copy (work/asset-game, the same DLL) with a temporary
#        d2coop folder (cfg, the banks and tables this PC's installer builds) and an ISOLATED profile holding the
#        fixture save; real keystrokes: title -> Continue -> gameplay -> F7 (friends-only lobby, invite overlay).
#   Join: steam://joinlobby/<app>/<lobby>/<host> in the VM (Steam's Join Game route). The VM's game follows the
#        host's mission by itself. Both players then walk (host keystrokes, VM keyboard), and F8 leaves.
#   Pass: install ok; connected both ways; the VM reaches the host's map in gameplay; both partner banks active;
#        each side sees the other move; F8 ends the session on both sides.
# Cleanup: games closed, the release UNINSTALLED from the VM with Uninstall.cmd (rollback), the host's temporary
# d2coop folder removed, protected saves checked. SteamIDs are never recorded.
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..')); $out = Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1"; . "$root/tools/vm/D2LabVM.Lab.ps1"
$game = Join-Path $root 'work/asset-game'; $data = Join-Path $game 'd2coop'; $exe = Join-Path $game 'Dishonored2.exe'
$zipPath = Join-Path $root $Zip; $leaf = [IO.Path]::GetFileNameWithoutExtension($zipPath); $vmGame = 'C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
$mutex = [Threading.Mutex]::new($false, 'Local\D2CoopLab_NativeOperation_01a11a65'); $owned = $false; $failure = $null; $proc = $null; $notes = [ordered]@{}
$installedVm = $false; $createdData = $false; $vmDownloads = $null
function Save($p, $v) { $v | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $p }
$safe = 'active', 'host', 'phase', 'result', 'connectionState', 'joinRequests', 'role', 'inviteOpened', 'lobbyMembers', 'steamReady'
function Safe($o) { if (!$o) { return $null }; $h = [ordered]@{}; foreach ($n in $safe) { if ($o.PSObject.Properties[$n]) { $h[$n] = $o.$n } }; [pscustomobject]$h }
function Json($p) { for ($i = 0; $i -lt 30; $i++) { try { return (Get-Content -LiteralPath $p -Raw -ErrorAction Stop | ConvertFrom-Json) } catch { Start-Sleep -Milliseconds 60 } }; $null }
function VmJson($p) { Invoke-D2LabVM -ArgumentList $p -ScriptBlock { param($p) for ($i = 0; $i -lt 30; $i++) { try { return (Get-Content -LiteralPath $p -Raw -ErrorAction Stop | ConvertFrom-Json) } catch { Start-Sleep -Milliseconds 60 } }; $null } }
function Pos($s) { if ($s -and $s.localPosition) { @($s.localPosition) } else { $null } }
function Dist($a, $b) { if (!$a -or !$b) { return 0 }; [math]::Sqrt([math]::Pow($a[0] - $b[0], 2) + [math]::Pow($a[1] - $b[1], 2)) }
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
function Movement($s) { if (!$s) { return $null }; [ordered]@{ active = $s.active; version = $s.version; checksum = "$($s.checksum)" } }
try {
    try { $owned = $mutex.WaitOne(0) } catch [Threading.AbandonedMutexException] { $owned = $true }; if (!$owned) { throw 'Native operation active' }
    if (Test-Path $out) { throw 'Unique take required' }; New-Item -ItemType Directory $out | Out-Null; Copy-Item $PSCommandPath "$out/run-script.ps1"
    if (Test-Path $data) { throw 'asset-game already has a d2coop folder; refusing to overwrite' }
    $notes.zip = [ordered]@{ path = $Zip; bytes = (Get-Item $zipPath).Length; sha256 = (Get-FileHash $zipPath).Hash }
    & "$root/tools/vm/Test-D2LabVM.ps1" -Repair | Out-Null; if ($LASTEXITCODE) { throw 'Lab VM not ready' }
    # --- VM: extract the release into Downloads and run Install.cmd with no arguments, as the README says.
    try { Stop-D2LabVMRole -Role client | Out-Null } catch { }
    Invoke-D2LabVM { Get-Process Dishonored2 -ErrorAction SilentlyContinue | Stop-Process -Force; Start-Sleep 2 }
    $s = New-PSSession -VMName $VmName -Credential (Get-D2LabGuestCredential)
    try {
        $vmDownloads = Invoke-Command -Session $s { $d = Join-Path $env:USERPROFILE 'Downloads'; New-Item -ItemType Directory -Force $d | Out-Null; $d }
        Invoke-Command -Session $s -ArgumentList $vmDownloads, $leaf { param($d, $l) Remove-Item -Recurse -Force (Join-Path $d $l), (Join-Path $d "$l.zip") -ErrorAction SilentlyContinue }
        Copy-Item -ToSession $s -Path $zipPath -Destination (Join-Path $vmDownloads "$leaf.zip")
        $inst = Invoke-Command -Session $s -ArgumentList $vmDownloads, $leaf, $vmGame { param($d, $l, $g)
            $mp = Get-MpComputerStatus -ErrorAction SilentlyContinue
            $exclusions = @((Get-MpPreference -ErrorAction SilentlyContinue).ExclusionPath)
            Expand-Archive -LiteralPath (Join-Path $d "$l.zip") -DestinationPath $d -Force
            $env:D2COOP_NO_PAUSE = '1'; $sw = [Diagnostics.Stopwatch]::StartNew()
            $o = & cmd.exe /c "`"$(Join-Path $d "$l\Install.cmd")`"" 2>&1; $exit = $LASTEXITCODE
            Start-Sleep -Seconds 20  # give real-time protection time to react to the new DLL
            $since = (Get-Date).AddMinutes(-15)
            [pscustomobject]@{ exit = $exit; seconds = [math]::Round($sw.Elapsed.TotalSeconds, 1); output = ($o | ForEach-Object { "$_" } | Select-Object -Last 8)
                receipt = Test-Path "$g\d2coop-install-receipt.json"; dll = Test-Path "$g\dinput8.dll"
                built = @('base\master.index', 'base\game1voidmod.index', 'd2coop\corvo-choke-bank.bin', 'd2coop\emily-choke-bank.bin', 'd2coop\native-tables.bin' | Where-Object { Test-Path (Join-Path $g $_) })
                defender = [pscustomobject]@{ realTime = [bool]$mp.RealTimeProtectionEnabled; antivirus = [bool]$mp.AntivirusEnabled; steamExcluded = [bool]($exclusions | Where-Object { $_ -and $g.StartsWith($_, [StringComparison]::OrdinalIgnoreCase) })
                    detections = @(Get-MpThreatDetection -ErrorAction SilentlyContinue | Where-Object { $_.InitialDetectionTime -gt $since } | ForEach-Object { "$($_.ThreatID) $(@($_.Resources) -join ',')" }) } } }
    } finally { Remove-PSSession $s }
    $notes.vmInstall = $inst
    if ($inst.receipt) { $installedVm = $true }
    if ($inst.exit -or !$inst.receipt -or !$inst.dll -or @($inst.built).Count -ne 5) { throw "Release install in the VM failed: $($inst.output -join ' | ')" }
    Start-D2LabVMGame
    $d = (Get-Date).AddSeconds(150); do { Start-Sleep 5; $vp = VmJson "$vmGame\d2coop\coop-status.json.product.json" } while ((Get-Date) -lt $d -and !$vp)
    if (!$vp) { throw 'VM product mode did not report (no product status)' }
    $notes.vmProductBoot = $true
    # --- Host: product instance with an isolated profile holding the fixture save, and this PC's built files.
    $prof = Join-Path $out 'profile'; $savegame = Join-Path $prof 'base/savegame'
    New-Item -ItemType Directory -Force $savegame | Out-Null
    Copy-Item -Recurse "$root/work/lab-instances/host/base/savegame/SLOT81" $savegame
    Copy-Item "$root/work/lab-instances/host/base/savegame/profile.bin" $savegame -ErrorAction SilentlyContinue
    New-Item -ItemType Directory $data | Out-Null; $createdData = $true
    "[d2coop]`nenabled=1`nsavegame=$($savegame.Replace('/','\'))`n" | Set-Content -LiteralPath (Join-Path $data 'd2coop.cfg') -Encoding ascii
    foreach ($who in 'corvo', 'emily') { Copy-Item "$root/work/release/bank-patches-v1/build-$who-c/$who-bank-rebuilt/$who-choke-bank.bin" (Join-Path $data "$who-choke-bank.bin") }
    Copy-Item "$root/work/release/native-tables-v1/native-tables.bin" (Join-Path $data 'native-tables.bin')
    $launchArgs = "+set fs_savepath `"$($prof.Replace('\','/'))`" +set r_fullscreen 0 +set r_windowWidth 944 +set r_windowHeight 540 +set r_windowPosX 480 +set r_windowPosY 300 +set win_pauseOnFocusLoss 0 +set r_renderQualityPresets 6 +set r_textureDetailsPreset 0 +set r_shadowQualityPreset 0 +set r_skipSSAO 1"
    $psi = [Diagnostics.ProcessStartInfo]::new($exe, $launchArgs); $psi.WorkingDirectory = $game; $psi.UseShellExecute = $false
    foreach ($k in @($psi.Environment.Keys)) { if ($k -like 'D2COOP_*') { $psi.Environment.Remove($k) | Out-Null } }
    $psi.Environment['SteamAppId'] = '403640'; $psi.Environment['SteamGameId'] = '403640'
    $proc = [Diagnostics.Process]::Start($psi); $notes.hostPid = $proc.Id
    $d = (Get-Date).AddSeconds(120); do { Start-Sleep 3; if ($proc.HasExited) { throw "Host product process exited ($($proc.ExitCode))" }; $hp = Json (Join-Path $data 'coop-status.json.product.json') } while ((Get-Date) -lt $d -and !$hp)
    if (!$hp) { throw 'Host product mode did not report' }
    Start-Sleep 8; $proc.Refresh(); $hwnd = $proc.MainWindowHandle
    $hs = $null
    foreach ($step in 1..12) {
        $hs = Json (Join-Path $data 'coop-status.json'); if ($hs.localGameLogicState -eq 1) { break }
        $notes["focus$step"] = Focus $hwnd; Key 0x0D; Start-Sleep -Seconds 6
    }
    $notes.hostGameplay = [ordered]@{ logic = $hs.localGameLogicState; map = $hs.localMap }
    if ($hs.localGameLogicState -ne 1) { throw "Host product instance did not reach gameplay (logic $($hs.localGameLogicState))" }
    Focus $hwnd | Out-Null; Key 0x76 200
    $d = (Get-Date).AddSeconds(40); do { Start-Sleep 2; $ho = Json (Join-Path $data 'coop-status.json.online.json') } while ((Get-Date) -lt $d -and !($ho.lobby -and "$($ho.lobby)" -ne '0'))
    $notes.hostOnlineAfterF7 = Safe $ho
    if (!$ho.lobby -or "$($ho.lobby)" -eq '0') { throw 'F7 did not create a lobby' }
    $url = "steam://joinlobby/403640/$($ho.lobby)/$($ho.self)"
    Start-D2LabVMInteractive -FilePath 'C:\Program Files (x86)\Steam\steam.exe' -Arguments $url
    $d = (Get-Date).AddSeconds(120)
    do { Start-Sleep 4; $hs = Json (Join-Path $data 'coop-status.json'); $gs = VmJson "$vmGame\d2coop\coop-status.json" } while ((Get-Date) -lt $d -and !($hs.remoteFresh -and $gs.remoteFresh))
    $notes.connected = [ordered]@{ host = [ordered]@{ connectionState = $hs.connectionState; remoteFresh = $hs.remoteFresh }; vm = [ordered]@{ connectionState = $gs.connectionState; remoteFresh = $gs.remoteFresh } }
    if (!($hs.remoteFresh -and $gs.remoteFresh)) { throw 'Product session did not connect both ways' }
    # The VM's game follows the host's mission by itself (join-follow) and both reach gameplay there.
    # The mission card after the load waits for "Press any key" (logic 15), as for any player: press Space.
    $d = (Get-Date).AddSeconds(600); $shot = 0; $notes.vmKeys = 0
    do { Start-Sleep 5; $hs = Json (Join-Path $data 'coop-status.json'); $gs = VmJson "$vmGame\d2coop\coop-status.json"
        if ($gs.localGameLogicState -eq 15) { Send-D2LabVMKey -VirtualKey 0x20; $notes.vmKeys++ }
        # The coup mission asks who to play: the partner takes Emily (right), as the README says.
        # Its confirmation ("Play as Emily Kaldwin?", No/Yes) is a menu (logic 3): Yes is on the right.
        elseif ($gs.localGameLogicState -eq 14 -or $gs.localGameLogicState -eq 3) { Start-Sleep -Milliseconds 1500; Send-D2LabVMKey -VirtualKey 0x27; Send-D2LabVMKey -VirtualKey 0x0D; $notes.vmChoice = 1 + [int]$notes.vmChoice }
        # Intro cutscenes in the host's map: hold Space to skip (allowed in the lab).
        elseif ($gs.localGameLogicState -ne 1 -and $gs.localMapId -and $gs.localMapId -eq $hs.localMapId -and !$gs.localPendingGameLogicState) { Send-D2LabVMKey -VirtualKey 0x20 -HoldMs 2500; $notes.vmSkips = 1 + [int]$notes.vmSkips }
        if (((Get-Date) - $d).TotalSeconds % 60 -gt -5 -and $shot -lt 10) { try { Save-D2LabVMScreen -Path "$out/vm-follow-$shot.png" | Out-Null; $shot++ } catch { } }
    } while ((Get-Date) -lt $d -and !($gs.localGameLogicState -eq 1 -and $gs.localMapId -and $gs.localMapId -eq $hs.localMapId -and $hs.remoteFresh -and $gs.remoteFresh))
    $notes.sameMission = [ordered]@{ hostMap = $hs.localMap; vmMap = $gs.localMap; vmLogic = $gs.localGameLogicState; same = ($gs.localMapId -eq $hs.localMapId) }
    try { Save-D2LabVMScreen -Path "$out/vm-joined.png" | Out-Null } catch { }
    if (!($gs.localGameLogicState -eq 1 -and $gs.localMapId -eq $hs.localMapId)) { throw 'The VM did not reach the host mission in gameplay' }
    # Partner presentation armed on both sides (product controller), then both players walk.
    $d = (Get-Date).AddSeconds(90)
    do { Start-Sleep 4; $hm = Json (Join-Path $data 'coop-status.json.movement.json'); $gm = VmJson "$vmGame\d2coop\coop-status.json.movement.json" } while ((Get-Date) -lt $d -and !($hm.active -and $gm.active))
    $notes.banks = [ordered]@{ host = Movement $hm; vm = Movement $gm }
    if (!($hm.active -and $gm.active)) { throw 'Partner movement banks did not become active on both sides' }
    $h0 = Json (Join-Path $data 'coop-status.json'); $g0 = VmJson "$vmGame\d2coop\coop-status.json"
    Focus $hwnd | Out-Null; Key 0x57 3000
    Send-D2LabVMKey -VirtualKey 0x57 -HoldMs 3000
    Start-Sleep 3
    $h1 = Json (Join-Path $data 'coop-status.json'); $g1 = VmJson "$vmGame\d2coop\coop-status.json"
    $notes.walk = [ordered]@{ hostMoved = [math]::Round((Dist (Pos $h0) (Pos $h1)), 2); vmMoved = [math]::Round((Dist (Pos $g0) (Pos $g1)), 2)
        hostSawVm = [math]::Round((Dist @($h0.remotePosition) @($h1.remotePosition)), 2); vmSawHost = [math]::Round((Dist @($g0.remotePosition) @($g1.remotePosition)), 2) }
    try { Save-D2LabVMScreen -Path "$out/vm-walk.png" | Out-Null } catch { }
    if ($notes.walk.hostSawVm -lt 0.5 -or $notes.walk.vmSawHost -lt 0.5) { throw 'The partners did not see each other move' }
    # F8 leaves the session.
    Focus $hwnd | Out-Null; Key 0x77 200
    $d = (Get-Date).AddSeconds(40)
    do { Start-Sleep 3; $ho = Json (Join-Path $data 'coop-status.json.online.json'); $go = VmJson "$vmGame\d2coop\coop-status.json.online.json" } while ((Get-Date) -lt $d -and ($ho.active -or $go.active))
    $notes.left = [ordered]@{ host = Safe $ho; vm = Safe $go }
    if ($ho.active) { throw 'F8 did not end the host session' }
} catch { $failure = $_.Exception.Message; Save "$out/failure.json" @{ message = $failure; stack = $_.ScriptStackTrace } }
finally {
    if ($owned) {
        $errors = [Collections.Generic.List[string]]::new()
        if ($proc -and !$proc.HasExited) { try { $proc.Kill(); $proc.WaitForExit(15000) | Out-Null } catch { $errors.Add("host stop: $($_.Exception.Message)") } }
        if ($createdData) { Start-Sleep 2; try { Remove-Item -Recurse -Force $data } catch { $errors.Add("host d2coop cleanup: $($_.Exception.Message)") } }
        try { Invoke-D2LabVM { Get-Process Dishonored2 -ErrorAction SilentlyContinue | Stop-Process -Force; Start-Sleep 3 } } catch { }
        # The VM's own follow/product/online state (no SteamIDs: only these fields).
        try { $vf = VmJson "$vmGame\d2coop\coop-status.json.follow.json"; $vo = VmJson "$vmGame\d2coop\coop-status.json.online.json"; $vs = VmJson "$vmGame\d2coop\coop-status.json"
            $notes.vmFinal = [ordered]@{ follow = $vf; online = Safe $vo; logic = $vs.localGameLogicState; pending = $vs.localPendingGameLogicState; map = $vs.localMap; fresh = $vs.remoteFresh } } catch { }
        if ($installedVm) {
            try { $un = Invoke-D2LabVM -ArgumentList $vmDownloads, $leaf, $vmGame { param($d, $l, $g)
                    $env:D2COOP_NO_PAUSE = '1'; $o = & cmd.exe /c "`"$(Join-Path $d "$l\Uninstall.cmd")`"" 2>&1
                    [pscustomobject]@{ exit = $LASTEXITCODE; receiptGone = !(Test-Path "$g\d2coop-install-receipt.json"); dllGone = !(Test-Path "$g\dinput8.dll"); dataGone = !(Test-Path "$g\d2coop"); voidmodGone = !(Test-Path "$g\base\game1voidmod.index")
                        exeHash = (Get-FileHash "$g\Dishonored2.exe").Hash.Substring(0, 16); masterHash = (Get-FileHash "$g\base\master.index").Hash.Substring(0, 16); output = ($o | ForEach-Object { "$_" } | Select-Object -Last 4) } }
                $notes.vmUninstall = $un; if ($un.exit -or !$un.receiptGone -or !$un.dllGone -or !$un.voidmodGone) { $errors.Add('VM uninstall incomplete') } } catch { $errors.Add("vm uninstall: $($_.Exception.Message)") }
        }
        foreach ($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw | ConvertFrom-Json) { if ((Get-FileHash -LiteralPath $f.Path).Hash -ne $f.Hash) { $errors.Add('Protected file changed') } }
        Save "$out/restoration.json" @{ passed = (!$errors.Count); errors = @($errors) }; if ($errors.Count -and !$failure) { $failure = $errors -join '; ' }
        Save "$out/result.json" ([ordered]@{ passed = (!$failure); failure = $failure; notes = $notes
            scope = 'Release 0.7.0 clean-PC path, two machines/two accounts: zip extracted in the VM Downloads folder, Install.cmd with no arguments (game found through Steam, files built from the VM game, Defender on), host product instance F7, VM joins via steam://joinlobby and follows the host mission, both walk and see each other, F8 leaves; then Uninstall.cmd (rollback). The zip was copied into the VM, not downloaded from GitHub (private repository).' })
        $mutex.ReleaseMutex()
    }
    $mutex.Dispose()
}
foreach ($f in Get-ChildItem $out -File -Filter *.json) { if ((Get-Content $f.FullName -Raw) -match '7656119[0-9]{10}') { throw "SteamID leaked into $($f.Name)" } }
Get-Content "$out/result.json"; if ($failure) { throw $failure }
