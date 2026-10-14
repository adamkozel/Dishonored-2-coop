param([switch]$Execute)
$ErrorActionPreference = 'Stop'
$root = 'C:\Users\adako\Documents\Codex\2026-10-08\i-x20'
$appPath = 'C:\Program Files\WindowsApps\OpenAI.Codex_26.930.7945.0_x64__2p2nqsd0c76g0\app\ChatGPT.exe'
$mainId = 11160
$expectedCreated = [datetime]'2026-10-08T09:17:42.156876+02:00'
$all = @(Get-CimInstance Win32_Process)
$main = $all | Where-Object ProcessId -eq $mainId
if (!$main -or $main.ExecutablePath -ne $appPath -or [math]::Abs(($main.CreationDate - $expectedCreated).TotalSeconds) -gt 1) { throw 'The original app identity changed; restart cancelled.' }
if (!(Test-Path -LiteralPath $appPath -PathType Leaf)) { throw 'App executable is missing.' }
$descendants = [System.Collections.Generic.HashSet[uint32]]::new()
[void]$descendants.Add($mainId)
do {
    $changed = $false
    foreach ($entry in $all) {
        if ($descendants.Contains([uint32]$entry.ParentProcessId) -and $descendants.Add([uint32]$entry.ProcessId)) { $changed = $true }
    }
} while ($changed)
$targets = @($all | Where-Object {
    $descendants.Contains([uint32]$_.ProcessId) -and (
        ($_.Name -eq 'ChatGPT.exe' -and $_.ExecutablePath -eq $appPath) -or
        ($_.Name -eq 'codex.exe' -and $_.ParentProcessId -eq $mainId -and $_.CommandLine -match ' app-server ')
    )
})
$plan = [ordered]@{
    PreparedAt = (Get-Date).ToString('o')
    Targets = @($targets | Select-Object ProcessId,Name,ExecutablePath,CreationDate)
    GamesBefore = @($all | Where-Object Name -eq 'Dishonored2.exe' | Select-Object ProcessId,ExecutablePath)
    StateFile = "$root\work\current-task-state.json"
    GhidraSave = "$root\outputs\d2-coop-lab\analysis\ghidra\pre-codex-restart-save.json"
    Reason = 'User requested Codex restart to load registered d2-ghidra MCP tools.'
}
$plan | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath "$root\work\codex-restart-plan.json" -Encoding utf8
if (!$Execute) { $plan | ConvertTo-Json -Depth 5; return }
$logPath = "$root\work\codex-restart.log"
try {
    "$(Get-Date -Format o) Restart helper ready; waiting 20 seconds for the final chat response." | Set-Content -LiteralPath $logPath
    Start-Sleep -Seconds 20
    foreach ($entry in $targets) {
        $current = Get-CimInstance Win32_Process -Filter "ProcessId=$($entry.ProcessId)"
        if ($current -and $current.ExecutablePath -eq $entry.ExecutablePath -and $current.CreationDate -eq $entry.CreationDate) {
            Stop-Process -Id $entry.ProcessId -Force -ErrorAction SilentlyContinue
        }
    }
    Start-Sleep -Seconds 3
    Start-Process -FilePath $appPath -WorkingDirectory (Split-Path $appPath) -WindowStyle Normal
    "$(Get-Date -Format o) Codex launch requested." | Add-Content -LiteralPath $logPath
    Start-Sleep -Seconds 8
    $after = @(Get-CimInstance Win32_Process)
    [ordered]@{
        CheckedAt = (Get-Date).ToString('o')
        App = @($after | Where-Object { $_.ExecutablePath -eq $appPath -and $_.CommandLine -notmatch '--type=' } | Select-Object ProcessId,CreationDate)
        Games = @($after | Where-Object Name -eq 'Dishonored2.exe' | Select-Object ProcessId,ExecutablePath)
    } | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath "$root\work\codex-restart-result.json" -Encoding utf8
} catch {
    "$(Get-Date -Format o) ERROR: $($_.Exception.Message)" | Add-Content -LiteralPath $logPath
    throw
}
