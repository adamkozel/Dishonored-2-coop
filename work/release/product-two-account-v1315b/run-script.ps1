param([string]$Take = 'product-two-account-v1315a', [string]$Zip = 'outputs/release/d2coop-v0.7.0.zip', [string]$Url = '')
# Release 0.7.0 clean-PC path (#30, goal item 4) across two machines and two Steam accounts.
#   VM   (second account, Defender on, no exclusion over the Steam folder), exactly as a player: the release zip is
#        downloaded inside the VM from GitHub (-Url) or copied in (-Zip), extracted into Downloads, Install.cmd runs
#        with no arguments; the game starts through Steam in product mode and waits in its main menu.
#   Host (main account): the lab host instance (lab mode, same DLL, Dunwall fixture in gameplay), driven only through
#        its command file and simulated controller, so nothing types on this PC's desktop (1314e/f: real keystrokes
#        to a host product instance stopped reaching the game once the PC was in use). It hosts a friends-only lobby
#        without the invite overlay (coop_session host_quiet), publishes its mission (coop_follow_host on) and runs
#        the product controller (coop_product_auto on). F7 hosting itself is covered by take 1225.
#   Join: steam://joinlobby/<app>/<lobby>/<host> in the VM (Steam's Join Game route). The VM's game follows the host's
#        mission by itself; the harness answers the native mission card and character choice (Emily) like a player.
#   Play: both partner banks active; the host walks (simulated controller), the VM walks (Hyper-V keyboard); each
#        side sees the other move. The VM then presses F8 (the release's leave key).
# Cleanup: the VM game closed and the release UNINSTALLED with Uninstall.cmd (rollback); the lab host's session,
# follow and product controller turned off and the lab loopback reconnected; protected saves checked. SteamIDs are
# never recorded.
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..')); $out = Join-Path $PSScriptRoot $Take; $lab = Join-Path $root 'outputs/d2-coop-lab'
. "$root/tools/vm/D2LabVM.Common.ps1"; . "$root/tools/vm/D2LabVM.Lab.ps1"
$hostDir = Join-Path $root 'work/lab-instances/host'
$leaf = if ($Url) { [IO.Path]::GetFileNameWithoutExtension(([uri]$Url).Segments[-1]) } else { [IO.Path]::GetFileNameWithoutExtension((Join-Path $root $Zip)) }
$vmGame = 'C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
$mutex = [Threading.Mutex]::new($false, 'Local\D2CoopLab_NativeOperation_01a11a65'); $owned = $false; $failure = $null; $notes = [ordered]@{}
$installedVm = $false; $hostArmed = $false; $vmDownloads = $null
function Save($p, $v) { $v | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $p }
$safe = 'active', 'host', 'phase', 'result', 'connectionState', 'joinRequests', 'role', 'inviteOpened', 'lobbyMembers', 'steamReady'
function Safe($o) { if (!$o) { return $null }; $h = [ordered]@{}; foreach ($n in $safe) { if ($o.PSObject.Properties[$n]) { $h[$n] = $o.$n } }; [pscustomobject]$h }
function Json($p) { for ($i = 0; $i -lt 30; $i++) { try { return (Get-Content -LiteralPath $p -Raw -ErrorAction Stop | ConvertFrom-Json) } catch { Start-Sleep -Milliseconds 60 } }; $null }
function VmJson($p) { Invoke-D2LabVM -ArgumentList $p -ScriptBlock { param($p) for ($i = 0; $i -lt 30; $i++) { try { return (Get-Content -LiteralPath $p -Raw -ErrorAction Stop | ConvertFrom-Json) } catch { Start-Sleep -Milliseconds 60 } }; $null } }
function Pos($s) { if ($s -and $s.localPosition) { @($s.localPosition) } else { $null } }
function Dist($a, $b) { if (!$a -or !$b) { return 0 }; [math]::Sqrt([math]::Pow($a[0] - $b[0], 2) + [math]::Pow($a[1] - $b[1], 2)) }
function C($text) { & "$lab/Send-D2Command.ps1" -Role host -Command $text -WaitConsumed | Out-Null }
function Movement($s) { if (!$s) { return $null }; [ordered]@{ active = $s.active; version = $s.version; checksum = "$($s.checksum)" } }
try {
    try { $owned = $mutex.WaitOne(0) } catch [Threading.AbandonedMutexException] { $owned = $true }; if (!$owned) { throw 'Native operation active' }
    if (Test-Path $out) { throw 'Unique take required' }; New-Item -ItemType Directory $out | Out-Null; Copy-Item $PSCommandPath "$out/run-script.ps1"
    $hs = Json "$hostDir/coop-status.json"
    $notes.labBefore = [ordered]@{ logic = $hs.localGameLogicState; map = $hs.localMap; fresh = $hs.remoteFresh; dll = (Get-FileHash "$root/work/asset-game/dinput8.dll").Hash }
    if ($hs.localGameLogicState -ne 1) { throw 'The lab host is not in gameplay' }
    & "$root/tools/vm/Test-D2LabVM.ps1" -Repair | Out-Null; if ($LASTEXITCODE) { throw 'Lab VM not ready' }
    # --- VM: get the release (download from GitHub, or copy), extract into Downloads, run Install.cmd with no arguments.
    try { Stop-D2LabVMRole -Role client | Out-Null } catch { }
    Invoke-D2LabVM { Get-Process Dishonored2 -ErrorAction SilentlyContinue | Stop-Process -Force; Start-Sleep 2 }
    $s = New-PSSession -VMName $VmName -Credential (Get-D2LabGuestCredential)
    try {
        $vmDownloads = Invoke-Command -Session $s { $d = Join-Path $env:USERPROFILE 'Downloads'; New-Item -ItemType Directory -Force $d | Out-Null; $d }
        Invoke-Command -Session $s -ArgumentList $vmDownloads, $leaf { param($d, $l) Remove-Item -Recurse -Force (Join-Path $d $l), (Join-Path $d "$l.zip") -ErrorAction SilentlyContinue }
        if ($Url) {
            $notes.download = Invoke-Command -Session $s -ArgumentList $vmDownloads, $leaf, $Url { param($d, $l, $u)
                $ProgressPreference = 'SilentlyContinue'; $sw = [Diagnostics.Stopwatch]::StartNew()
                Invoke-WebRequest -Uri $u -OutFile (Join-Path $d "$l.zip") -UseBasicParsing
                [pscustomobject]@{ url = $u; seconds = [math]::Round($sw.Elapsed.TotalSeconds, 1); bytes = (Get-Item (Join-Path $d "$l.zip")).Length; sha256 = (Get-FileHash (Join-Path $d "$l.zip")).Hash } }
        } else {
            Copy-Item -ToSession $s -Path (Join-Path $root $Zip) -Destination (Join-Path $vmDownloads "$leaf.zip")
            $notes.copied = [ordered]@{ zip = $Zip; sha256 = (Get-FileHash (Join-Path $root $Zip)).Hash }
        }
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
    # --- Host: the lab host instance hosts (no overlay), publishes its mission and runs the product controller.
    $hostArmed = $true; C 'coop_product_auto on'; C 'coop_follow_host on'; C 'coop_session host_quiet'
    $d = (Get-Date).AddSeconds(40); do { Start-Sleep 2; $ho = Json "$hostDir/coop-status.json.online.json" } while ((Get-Date) -lt $d -and !($ho.lobby -and "$($ho.lobby)" -ne '0'))
    $notes.hostOnline = Safe $ho
    if (!$ho.lobby -or "$($ho.lobby)" -eq '0') { throw 'The lab host did not create a lobby' }
    Start-D2LabVMInteractive -FilePath 'C:\Program Files (x86)\Steam\steam.exe' -Arguments "steam://joinlobby/403640/$($ho.lobby)/$($ho.self)"
    $d = (Get-Date).AddSeconds(120)
    do { Start-Sleep 4; $hs = Json "$hostDir/coop-status.json"; $gs = VmJson "$vmGame\d2coop\coop-status.json" } while ((Get-Date) -lt $d -and !($hs.remoteFresh -and $gs.remoteFresh -and $gs.connectionState -eq 3))
    $notes.connected = [ordered]@{ host = [ordered]@{ connectionState = $hs.connectionState; remoteFresh = $hs.remoteFresh }; vm = [ordered]@{ connectionState = $gs.connectionState; remoteFresh = $gs.remoteFresh } }
    if (!($hs.remoteFresh -and $gs.remoteFresh)) { throw 'The session did not connect both ways' }
    # The VM follows the host's mission by itself. Like a player it presses a key at the mission card (logic 15) and
    # takes Emily at the coup mission's character choice and its confirmation (logic 14, then a menu, logic 3).
    $d = (Get-Date).AddSeconds(600); $shot = 0; $notes.vmKeys = 0; $notes.vmChoice = 0; $notes.vmSkips = 0; $nextShot = Get-Date
    do { Start-Sleep 5; $hs = Json "$hostDir/coop-status.json"; $gs = VmJson "$vmGame\d2coop\coop-status.json"
        $sameMap = $gs.localMapId -and $gs.localMapId -eq $hs.localMapId
        if ($gs.localGameLogicState -eq 15) { Send-D2LabVMKey -VirtualKey 0x20; $notes.vmKeys++ }
        elseif ($sameMap -and ($gs.localGameLogicState -eq 14 -or $gs.localGameLogicState -eq 3)) { Start-Sleep -Milliseconds 1500; Send-D2LabVMKey -VirtualKey 0x27; Send-D2LabVMKey -VirtualKey 0x0D; $notes.vmChoice++ }
        elseif ($sameMap -and $gs.localGameLogicState -ne 1 -and !$gs.localPendingGameLogicState) { Send-D2LabVMKey -VirtualKey 0x20 -HoldMs 2500; $notes.vmSkips++ }
        if ((Get-Date) -ge $nextShot -and $shot -lt 12) { try { Save-D2LabVMScreen -Path "$out/vm-follow-$shot.png" | Out-Null; $shot++ } catch { }; $nextShot = (Get-Date).AddSeconds(45) }
    } while ((Get-Date) -lt $d -and !($gs.localGameLogicState -eq 1 -and $sameMap -and $hs.remoteFresh -and $gs.remoteFresh))
    $notes.sameMission = [ordered]@{ hostMap = $hs.localMap; vmMap = $gs.localMap; vmLogic = $gs.localGameLogicState; same = [bool]$sameMap }
    try { Save-D2LabVMScreen -Path "$out/vm-joined.png" | Out-Null } catch { }
    if (!($gs.localGameLogicState -eq 1 -and $sameMap)) { throw 'The VM did not reach the host mission in gameplay' }
    # Partner presentation armed on both sides, then both players walk.
    $d = (Get-Date).AddSeconds(120)
    do { Start-Sleep 4; $hm = Json "$hostDir/coop-status.json.movement.json"; $gm = VmJson "$vmGame\d2coop\coop-status.json.movement.json" } while ((Get-Date) -lt $d -and !($hm.active -and $gm.active))
    $notes.banks = [ordered]@{ host = Movement $hm; vm = Movement $gm }
    if (!($hm.active -and $gm.active)) { throw 'Partner movement banks did not become active on both sides' }
    $h0 = Json "$hostDir/coop-status.json"; $g0 = VmJson "$vmGame\d2coop\coop-status.json"
    & "$lab/Send-D2Input.ps1" -Role host -LeftY 32000 -DurationMs 3000 | Out-Null
    Send-D2LabVMKey -VirtualKey 0x57 -HoldMs 3000
    Start-Sleep 3
    $h1 = Json "$hostDir/coop-status.json"; $g1 = VmJson "$vmGame\d2coop\coop-status.json"
    $notes.walk = [ordered]@{ hostMoved = [math]::Round((Dist (Pos $h0) (Pos $h1)), 2); vmMoved = [math]::Round((Dist (Pos $g0) (Pos $g1)), 2)
        hostSawVm = [math]::Round((Dist @($h0.remotePosition) @($h1.remotePosition)), 2); vmSawHost = [math]::Round((Dist @($g0.remotePosition) @($g1.remotePosition)), 2) }
    try { Save-D2LabVMScreen -Path "$out/vm-walk.png" | Out-Null } catch { }
    if ($notes.walk.hostSawVm -lt 0.5 -or $notes.walk.vmSawHost -lt 0.5) { throw 'The partners did not see each other move' }
    # F8 in the VM: the release's leave key.
    # Held, not tapped: the product hotkeys poll the key state (1315a's instant tap was missed).
    Send-D2LabVMKey -VirtualKey 0x77 -HoldMs 400
    $d = (Get-Date).AddSeconds(40)
    do { Start-Sleep 3; $go = VmJson "$vmGame\d2coop\coop-status.json.online.json" } while ((Get-Date) -lt $d -and $go.active)
    $notes.vmLeft = Safe $go
    if ($go.active) { throw 'F8 did not end the VM session' }
} catch { $failure = $_.Exception.Message; Save "$out/failure.json" @{ message = $failure; stack = $_.ScriptStackTrace } }
finally {
    if ($owned) {
        $errors = [Collections.Generic.List[string]]::new()
        try { $vf = VmJson "$vmGame\d2coop\coop-status.json.follow.json"; $vo = VmJson "$vmGame\d2coop\coop-status.json.online.json"; $vs = VmJson "$vmGame\d2coop\coop-status.json"
            $notes.vmFinal = [ordered]@{ follow = $vf; online = Safe $vo; logic = $vs.localGameLogicState; pending = $vs.localPendingGameLogicState; map = $vs.localMap; fresh = $vs.remoteFresh } } catch { }
        try { Invoke-D2LabVM { Get-Process Dishonored2 -ErrorAction SilentlyContinue | Stop-Process -Force; Start-Sleep 3 } } catch { }
        if ($installedVm) {
            try { $un = Invoke-D2LabVM -ArgumentList $vmDownloads, $leaf, $vmGame { param($d, $l, $g)
                    $env:D2COOP_NO_PAUSE = '1'; $o = & cmd.exe /c "`"$(Join-Path $d "$l\Uninstall.cmd")`"" 2>&1
                    [pscustomobject]@{ exit = $LASTEXITCODE; receiptGone = !(Test-Path "$g\d2coop-install-receipt.json"); dllGone = !(Test-Path "$g\dinput8.dll"); dataGone = !(Test-Path "$g\d2coop"); voidmodGone = !(Test-Path "$g\base\game1voidmod.index")
                        exeHash = (Get-FileHash "$g\Dishonored2.exe").Hash.Substring(0, 16); masterHash = (Get-FileHash "$g\base\master.index").Hash.Substring(0, 16); output = ($o | ForEach-Object { "$_" } | Select-Object -Last 4) } }
                $notes.vmUninstall = $un; if ($un.exit -or !$un.receiptGone -or !$un.dllGone -or !$un.voidmodGone) { $errors.Add('VM uninstall incomplete') } } catch { $errors.Add("vm uninstall: $($_.Exception.Message)") }
        }
        if ($hostArmed) {
            try { C 'coop_session off'; C 'coop_follow_host off'; C 'coop_product_auto off' } catch { $errors.Add("host disarm: $($_.Exception.Message)") }
            $d = (Get-Date).AddSeconds(90); do { Start-Sleep 3; $hs = Json "$hostDir/coop-status.json"; $cs = Json "$root/work/lab-instances/client/coop-status.json" } while ((Get-Date) -lt $d -and !($hs.remoteFresh -and $cs.remoteFresh))
            $notes.labAfter = [ordered]@{ host = "logic=$($hs.localGameLogicState) fresh=$($hs.remoteFresh)"; client = "logic=$($cs.localGameLogicState) fresh=$($cs.remoteFresh)" }
            if (!($hs.remoteFresh -and $cs.remoteFresh)) { $errors.Add('Lab loopback did not reconnect') }
        }
        foreach ($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw | ConvertFrom-Json) { if ((Get-FileHash -LiteralPath $f.Path).Hash -ne $f.Hash) { $errors.Add('Protected file changed') } }
        Save "$out/restoration.json" @{ passed = (!$errors.Count); errors = @($errors) }; if ($errors.Count -and !$failure) { $failure = $errors -join '; ' }
        Save "$out/result.json" ([ordered]@{ passed = (!$failure); failure = $failure; notes = $notes
            scope = 'Release 0.7.0 clean-PC path, two machines/two accounts: release obtained in the VM (download from GitHub or copied zip), extracted in Downloads, Install.cmd with no arguments (game found through Steam, files built from the VM game, Defender on); lab host (same DLL) hosts a lobby; the VM joins via steam://joinlobby, follows the host mission, both walk and see each other, the VM leaves with F8; then Uninstall.cmd (rollback) and the lab restored.' })
        $mutex.ReleaseMutex()
    }
    $mutex.Dispose()
}
foreach ($f in Get-ChildItem $out -File -Filter *.json) { if ((Get-Content $f.FullName -Raw) -match '7656119[0-9]{10}') { throw "SteamID leaked into $($f.Name)" } }
Get-Content "$out/result.json"; if ($failure) { throw $failure }
