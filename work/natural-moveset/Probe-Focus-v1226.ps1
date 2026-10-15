param([string]$Take = 'focus-probe-v1226', [string]$Window = 'ld_rich_small_01_sndctr_4', [string]$Probe = 'work/build/Release/d2_runtime_probe.exe')
# #3 groundwork: validates the new d2_runtime_probe --focus mode on the host lab instance. Unfocused
# baseline, then stand 1.3 m in front of a real Dunwall window, aim at it with the established aim
# script, and read --focus (must name the window). Read-only probe; the Dunwall fixture is restored.
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..')); $lab = "$root/outputs/d2-coop-lab"; $out = Join-Path $PSScriptRoot $Take
$mutex = [Threading.Mutex]::new($false, 'Local\D2CoopLab_NativeOperation_01a11a65'); $owned = $false; $failure = $null; $notes = [ordered]@{}
function Save($p, $v) { $v | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $p }
function C($r, $cmd) { & "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000 | Out-Null }
$hostPid = (Get-Content "$root/work/lab-instances/host/launch.json" -Raw | ConvertFrom-Json).pid
function Focus { $raw = & (Join-Path $root $Probe) $hostPid --focus; if ($LASTEXITCODE) { throw "probe --focus failed ($LASTEXITCODE)" }; $raw | ConvertFrom-Json }
try {
    try { $owned = $mutex.WaitOne(0) } catch [Threading.AbandonedMutexException] { $owned = $true }; if (!$owned) { throw 'Native operation active' }
    if (Test-Path $out) { throw 'Unique take required' }; New-Item -ItemType Directory $out | Out-Null; Copy-Item $PSCommandPath "$out/run-script.ps1"
    $notes.baseline = Focus
    $e = @((& "$lab/bin/d2_runtime_probe.exe" $hostPid --entities | ConvertFrom-Json).entities | Where-Object name -eq $Window)
    if ($e.Count -ne 1) { throw "Window $Window not found" }; $wp = $e[0].position
    C host ("setviewpos {0:F3} {1:F3} 67.0 0 0" -f ($wp[0] - 1.28), $wp[1]); Start-Sleep -Milliseconds 1500
    & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role host -X $wp[0] -Y $wp[1] -Z 68.0 -Evidence "$out/aim.json"
    Start-Sleep -Milliseconds 500
    $notes.aimed = Focus
    $notes.windowHandle = $e[0].handle
    if (!$notes.aimed.focused -or $notes.aimed.name -ne $Window) { throw "--focus did not name the aimed window (got '$($notes.aimed.name)')" }
} catch { $failure = $_.Exception.Message; Save "$out/failure.json" @{ message = $failure; stack = $_.ScriptStackTrace } }
finally {
    if ($owned) {
        $errors = [Collections.Generic.List[string]]::new()
        try { & "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json" | Out-Null } catch { $errors.Add($_.Exception.Message) }
        foreach ($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw | ConvertFrom-Json) { if ((Get-FileHash -LiteralPath $f.Path).Hash -ne $f.Hash) { $errors.Add('Protected file changed') } }
        Save "$out/restoration.json" @{ passed = (!$errors.Count); errors = @($errors) }; if ($errors.Count -and !$failure) { $failure = $errors -join '; ' }
        Save "$out/result.json" ([ordered]@{ passed = (!$failure); failure = $failure; notes = $notes; scope = 'd2_runtime_probe --focus: unfocused baseline, then a real Dunwall window aimed at must be named. Fixture restored.' })
        $mutex.ReleaseMutex()
    }
    $mutex.Dispose()
}
Get-Content "$out/result.json"; if ($failure) { throw $failure }
