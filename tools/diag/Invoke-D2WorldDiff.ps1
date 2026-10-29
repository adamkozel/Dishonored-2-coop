param(
    [ValidateSet('host', 'client', 'vm')][string]$A = 'host',
    [ValidateSet('host', 'client', 'vm')][string]$B = 'client',
    [Parameter(Mandatory)][string]$OutDir,
    [string]$Tag = 'diff',
    [int]$Count = 3,
    [int]$IntervalSec = 3)
# Stage 8.2 (#235): the difference detector in one call, for any lab or VM take. Takes $Count `--world-digest`
# snapshots of each side ($IntervalSec apart; the stable-world gate needs 3), writes them to
# <OutDir>/digest-<Tag>-<side>.jsonl, compares them with tools/diag/d2_world_diff.py into <OutDir>/diff-<Tag>.json,
# and returns the comparison. 'vm' is the lab VM's client instance (two-account sessions), read through
# Invoke-D2LabVMProbe. Read-only for the games. Names are game-derived: keep the output local.
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$probe = "$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe"
$py = 'C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe'
if ($A -eq 'vm' -or $B -eq 'vm') { . "$root/tools/vm/D2LabVM.Common.ps1"; . "$root/tools/vm/D2LabVM.Lab.ps1" }
function Snapshot($side) {
    if ($side -eq 'vm') { return (Invoke-D2LabVMProbe -Mode '--world-digest' -Role client | ConvertTo-Json -Compress -Depth 20) }
    $l = Get-Content "$root/work/lab-instances/$side/launch.json" -Raw | ConvertFrom-Json
    $raw = & $probe $l.pid '--world-digest'
    if ($LASTEXITCODE) { throw "World digest failed ($side, $LASTEXITCODE)" }
    $raw
}
New-Item -ItemType Directory -Force $OutDir | Out-Null
$files = @{}
foreach ($side in $A, $B) {
    $files[$side] = Join-Path $OutDir "digest-$Tag-$side.jsonl"
    if (Test-Path $files[$side]) { throw "Digest file exists (evidence is never rewritten): $($files[$side])" }
}
foreach ($k in 1..$Count) {
    foreach ($side in $A, $B) { Add-Content -LiteralPath $files[$side] -Value (Snapshot $side) }
    if ($k -lt $Count) { Start-Sleep -Seconds $IntervalSec }
}
$json = Join-Path $OutDir "diff-$Tag.json"
& $py -I "$root/tools/diag/d2_world_diff.py" $files[$A] $files[$B] --out $json | Set-Content (Join-Path $OutDir "diff-$Tag.txt")
Get-Content $json -Raw | ConvertFrom-Json
