param([string]$Take = 'two-account-session-v1222')
# Stage 2 two-account validation across two machines (host lab instance on this PC = main Steam
# account, client lab instance in the lab VM = second account), with the host's LOCAL client lab
# instance still connected over loopback (the #23 case 1213e fixes):
#   1. host coop_session host -> friends-only lobby; VM coop_session join -> Steam P2P both sides
#   2. movement syncs both ways
#   3. disconnect: VM coop_session off -> the host releases the partner slot
#   4. reconnect: VM joins the same lobby again -> connected, fresh, no stale ownership
# Restores: sessions off, VM instance stopped, host lab pair relaunched with the Dunwall fixture.
# Online sidecars contain SteamIDs: only whitelisted fields are ever recorded.
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..')); $lab = "$root/outputs/d2-coop-lab"; $out = Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1"; . "$root/tools/vm/D2LabVM.Lab.ps1"
$mutex = [Threading.Mutex]::new($false, 'Local\D2CoopLab_NativeOperation_01a11a65'); $owned = $false; $failure = $null; $notes = [ordered]@{}
function Save($p, $v) { $v | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $p }
function HS { for ($i = 0; $i -lt 40; $i++) { try { return (Get-Content "$root/work/lab-instances/host/coop-status.json" -Raw | ConvertFrom-Json) } catch { Start-Sleep -Milliseconds 50 } } }
function HO { try { Get-Content "$root/work/lab-instances/host/coop-status.json.online.json" -Raw | ConvertFrom-Json } catch { $null } }
$safe = 'active', 'host', 'phase', 'result', 'connectionState', 'joinRequests', 'role', 'inviteOpened', 'lobbyMembers'
function Safe($o) { if (!$o) { return $null }; $h = [ordered]@{}; foreach ($n in $safe) { if ($o.PSObject.Properties[$n]) { $h[$n] = $o.$n } }; [pscustomobject]$h }
function Pair($tag) { $hs = HS; $gs = Get-D2LabVMStatus
    [ordered]@{ tag = $tag; at = (Get-Date).ToString('o'); hostOnline = Safe (HO); guestOnline = Safe (Get-D2LabVMStatus -Sidecar online)
        host = [ordered]@{ connectionState = $hs.connectionState; remoteFresh = $hs.remoteFresh; sent = $hs.sent; received = $hs.received }
        guest = [ordered]@{ connectionState = $gs.connectionState; remoteFresh = $gs.remoteFresh; sent = $gs.sent; received = $gs.received } } }
function WaitConnected($sec) { $d = (Get-Date).AddSeconds($sec); do { Start-Sleep 3; $hs = HS; $gs = Get-D2LabVMStatus } while ((Get-Date) -lt $d -and !($hs.remoteFresh -and $gs.remoteFresh -and $gs.connectionState -eq 3)); return ($hs.remoteFresh -and $gs.remoteFresh -and $gs.connectionState -eq 3) }
function Dist($a, $b) { [math]::Round([math]::Sqrt([math]::Pow($b[0] - $a[0], 2) + [math]::Pow($b[1] - $a[1], 2)), 2) }
try {
    try { $owned = $mutex.WaitOne(0) } catch [Threading.AbandonedMutexException] { $owned = $true }; if (!$owned) { throw 'Native operation active' }
    if (Test-Path $out) { throw 'Unique take required' }; New-Item -ItemType Directory $out | Out-Null; Copy-Item $PSCommandPath "$out/run-script.ps1"
    foreach ($r in 'host', 'client') { $s = Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw | ConvertFrom-Json; if ($s.localGameLogicState -ne 1 -or !$s.remoteFresh) { throw "Host lab pair not in gameplay ($r)" } }
    $notes.localClientConnectedBefore = (HS).remoteFresh
    # VM: health, same DLL, client instance in the Dunwall fixture (VM keyboard drives menus; #24).
    & "$root/tools/vm/Test-D2LabVM.ps1" -Repair | Out-Null; if ($LASTEXITCODE) { throw 'Lab VM not ready' }
    try { Stop-D2LabVMRole -Role client | Out-Null } catch { }
    Start-D2LabVMRole -Role client | Out-Null
    $d = (Get-Date).AddSeconds(90); do { Start-Sleep 3; $gs = Get-D2LabVMStatus } while ((Get-Date) -lt $d -and $gs.localGameLogicState -ne 6)
    Send-D2LabVMKey 0x0D; Start-Sleep 3
    Send-D2LabVMCommand -Command 'LoadGame 81' -WaitConsumed | Out-Null
    $d = (Get-Date).AddSeconds(120); do { Start-Sleep 3; $gs = Get-D2LabVMStatus; if ($gs.localGameLogicState -in 13, 8, 15) { Send-D2LabVMKey 0x0D } } while ((Get-Date) -lt $d -and $gs.localGameLogicState -ne 1)
    if ($gs.localGameLogicState -ne 1) { throw 'VM instance did not reach gameplay' }
    $notes.identity = [ordered]@{ hostDll = (Get-FileHash "$lab/bin/dinput8.dll").Hash; guestDllMatches = ((Invoke-D2LabVM { (Get-FileHash C:\D2Lab\ws\outputs\d2-coop-lab\bin\dinput8.dll).Hash }) -eq (Get-FileHash "$lab/bin/dinput8.dll").Hash) }
    # 1. Host (with its local client still connected) opens the lobby; VM joins.
    & "$lab/Send-D2Command.ps1" -Role host -Command 'coop_session host' -WaitConsumed -ReceiptTimeoutMs 8000 | Out-Null
    $d = (Get-Date).AddSeconds(30); do { Start-Sleep 2; $ho = HO } while ((Get-Date) -lt $d -and !($ho.lobby -and "$($ho.lobby)" -ne '0'))
    if (!$ho.lobby -or "$($ho.lobby)" -eq '0') { throw 'Host did not create a lobby' }
    $lobby = "$($ho.lobby)"
    Send-D2LabVMCommand -Command "coop_session join $lobby" -WaitConsumed | Out-Null
    $notes.join = [ordered]@{ connected = (WaitConnected 60); state = Pair 'join' }
    if (!$notes.join.connected) { throw 'Join with the host local client present did not connect (#23)' }
    # 2. Movement both ways.
    $b = (HS).remotePosition; Send-D2LabVMKey -VirtualKey 0x57 -HoldMs 1500; Start-Sleep 2; $a = (HS).remotePosition
    $gb = (Get-D2LabVMStatus).remotePosition; & "$lab/Send-D2Input.ps1" -Role host -Button Neutral -LeftY 30000 -DurationMs 1500 | Out-Null; Start-Sleep 2; $ga = (Get-D2LabVMStatus).remotePosition
    $notes.movement = [ordered]@{ hostSeesVmMove_m = (Dist $b $a); vmSeesHostMove_m = (Dist $gb $ga) }
    if ($notes.movement.hostSeesVmMove_m -lt 0.5 -or $notes.movement.vmSeesHostMove_m -lt 0.5) { throw 'Movement did not sync both ways' }
    # 3. Disconnect: the VM leaves; the host must release the partner slot.
    Send-D2LabVMCommand -Command 'coop_session off' -WaitConsumed | Out-Null
    $d = (Get-Date).AddSeconds(40); do { Start-Sleep 2; $hs = HS } while ((Get-Date) -lt $d -and ($hs.connection -or $hs.remoteFresh))
    $notes.disconnect = [ordered]@{ hostReleased = (!$hs.connection -and !$hs.remoteFresh); state = Pair 'after-leave' }
    if (!$notes.disconnect.hostReleased) { throw 'Host kept the departed partner (stale ownership)' }
    # 4. Reconnect: the VM joins the same lobby again.
    Send-D2LabVMCommand -Command "coop_session join $lobby" -WaitConsumed | Out-Null
    $notes.reconnect = [ordered]@{ connected = (WaitConnected 60); state = Pair 'rejoin' }
    if (!$notes.reconnect.connected) { throw 'Rejoin did not connect' }
    $b = (HS).remotePosition; Send-D2LabVMKey -VirtualKey 0x53 -HoldMs 1500; Start-Sleep 2; $a = (HS).remotePosition
    $notes.reconnect.hostSeesVmMove_m = Dist $b $a
    if ($notes.reconnect.hostSeesVmMove_m -lt 0.5) { throw 'Movement did not sync after reconnect' }
    try { Save-D2LabVMScreen -Path "$out/vm-after-rejoin.png" | Out-Null } catch { }
} catch { $failure = $_.Exception.Message; Save "$out/failure.json" @{ message = $failure; stack = $_.ScriptStackTrace } }
finally {
    if ($owned) {
        $errors = [Collections.Generic.List[string]]::new()
        try { Send-D2LabVMCommand -Command 'coop_session off' -WaitConsumed | Out-Null } catch { }
        try { & "$lab/Send-D2Command.ps1" -Role host -Command 'coop_session off' -WaitConsumed -ReceiptTimeoutMs 8000 | Out-Null } catch { }
        try { Stop-D2LabVMRole -Role client | Out-Null } catch { $errors.Add("vm stop: $($_.Exception.Message)") }
        $mutex.ReleaseMutex(); $owned = $false
        try { & "$root/work/movement-integration/Deploy-SourceClock.ps1" | Out-Null } catch { $errors.Add("restore: $($_.Exception.Message)") }
        foreach ($r in 'host', 'client') { try { $s = Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw | ConvertFrom-Json; if ($s.localGameLogicState -ne 1 -or !$s.remoteFresh) { $errors.Add("$r not restored") } } catch { $errors.Add("$r status") } }
        foreach ($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw | ConvertFrom-Json) { if ((Get-FileHash -LiteralPath $f.Path).Hash -ne $f.Hash) { $errors.Add('Protected file changed') } }
        Save "$out/restoration.json" @{ passed = (!$errors.Count); errors = @($errors) }; if ($errors.Count -and !$failure) { $failure = $errors -join '; ' }
        Save "$out/result.json" ([ordered]@{ passed = (!$failure); failure = $failure; notes = $notes
            scope = 'Two-account two-machine session with the host local lab client connected (#23 on 1213e), plus disconnect/reconnect without stale ownership (#12). Host lab instance = main account; VM lab instance = second account; Steam friends-only lobby + P2P.' })
    }
    $mutex.Dispose()
}
if ((Get-Content "$out/result.json" -Raw) -match '7656119[0-9]{10}') { throw 'SteamID leaked into the result' }
Get-Content "$out/result.json"; if ($failure) { throw $failure }
