# Mirrors the lab into the VM so the lab scripts run there unchanged (they resolve everything
# relative to the workspace root): guest root C:\D2Lab\ws with
#   outputs/d2-coop-lab/{*.ps1, tooling/, bin/}  + app-local VC++ runtime DLLs in bin/
#   work/asset-game   physical copy of the guest's own Steam install + the lab's voidmod/master.index
#   work/lab-instances/<Role>/  seed from the host instance (saves, config, movement bank)
# plus the pinned pwsh 7 runtime at C:\D2Lab\pwsh (lab scripts need pwsh 7.4+).
# Idempotent; re-run after rebuilding the DLL/probe or changing lab scripts.
# pwsh 7 on the host, member of "Hyper-V Administrators".
param([ValidateSet('host', 'client')][string]$Role = 'client', [switch]$SkipGameCopy)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')
$lab = Join-Path $RepoRoot 'outputs/d2-coop-lab'
$pwshSrc = 'C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell'
$g = 'C:\D2Lab\ws'
$s = New-PSSession -VMName $VmName -Credential (Get-D2LabGuestCredential)
function Step($m) { Write-Host ("{0:HH:mm:ss} {1}" -f (Get-Date), $m) }
try {
    Invoke-Command -Session $s -ArgumentList $g {
        param($g)
        foreach ($d in "$g\outputs\d2-coop-lab\tooling", "$g\outputs\d2-coop-lab\bin", "$g\outputs\d2-coop-lab\research", "$g\work\lab-instances", 'C:\D2Lab\pwsh') { New-Item -ItemType Directory -Force $d | Out-Null }
    }
    # 1. pwsh 7 runtime (copied once; version-checked).
    $have = Invoke-Command -Session $s { if (Test-Path C:\D2Lab\pwsh\pwsh.exe) { & C:\D2Lab\pwsh\pwsh.exe -NoProfile -Command '$PSVersionTable.PSVersion.ToString()' } }
    $want = & "$pwshSrc\pwsh.exe" -NoProfile -Command '$PSVersionTable.PSVersion.ToString()'
    if ($have -ne $want) { Step "copying pwsh $want"; Copy-Item -ToSession $s -Recurse -Force -Path "$pwshSrc\*" -Destination 'C:\D2Lab\pwsh' }
    # 2. Lab scripts, tooling and binaries, plus the VC++ runtime the DLL/probe import (#22).
    Step 'copying lab scripts, tooling and bin'
    Get-ChildItem $lab -Filter *.ps1 -File | ForEach-Object { Copy-Item -ToSession $s -Force -Path $_.FullName -Destination "$g\outputs\d2-coop-lab\$($_.Name)" }
    Copy-Item -ToSession $s -Force -Path "$lab\tooling\*" -Destination "$g\outputs\d2-coop-lab\tooling"
    Copy-Item -ToSession $s -Force -Path "$lab\bin\*" -Destination "$g\outputs\d2-coop-lab\bin"
    $crt = 'msvcp140.dll', 'vcruntime140.dll', 'vcruntime140_1.dll' | ForEach-Object { Join-Path "$env:windir\System32" $_ }
    $crt | ForEach-Object { Copy-Item -ToSession $s -Force -Path $_ -Destination "$g\outputs\d2-coop-lab\bin" }
    # 3. Physical game copy from the guest's own Steam install (42 GB, guest-local), then the lab's mod resources.
    if (!$SkipGameCopy) {
        Step 'game copy: guest Steam install -> work\asset-game (robocopy, skips files already present)'
        $rc = Invoke-Command -Session $s -ArgumentList $g {
            param($g)
            $src = 'C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
            robocopy $src "$g\work\asset-game" /E /XJ /R:1 /W:1 /NFL /NDL /NJH /NP /XF dinput8.dll | Select-Object -Last 6
            $LASTEXITCODE
        }
        $rc | Select-Object -SkipLast 1 | ForEach-Object { Step "  $_" }
        if ([int]($rc | Select-Object -Last 1) -ge 8) { throw "robocopy failed ($($rc | Select-Object -Last 1))" }
    }
    Step 'mod resources (voidmod, master.index) and app-local VC++ runtime into the game copy'
    Get-ChildItem (Join-Path $RepoRoot 'work/asset-game/base') -File | Where-Object { $_.Name -like 'game?voidmod.*' -or $_.Name -eq 'master.index' } |
        ForEach-Object { Copy-Item -ToSession $s -Force -Path $_.FullName -Destination "$g\work\asset-game\base\$($_.Name)" }
    $crt | ForEach-Object { Copy-Item -ToSession $s -Force -Path $_ -Destination "$g\work\asset-game" }
    # 4. Instance seed for the guest role (saves, config, movement bank); never the host's live state files.
    Step "instance seed: work/lab-instances/$Role"
    $src = Join-Path $RepoRoot "work/lab-instances/$Role"
    Get-ChildItem $src -Recurse -File | Where-Object {
        $_.Name -notlike 'coop-status.json*.json' -and $_.Name -notin 'launch.json', 'command.bin', 'input.bin', 'coop-status.json' -and $_.Extension -ne '.log' -and $_.Name -notlike 'packet-*.tmp' -and $_.Name -notlike '*.tmp'
    } | ForEach-Object {
        $rel = $_.FullName.Substring($src.Length + 1)
        $dest = "$g\work\lab-instances\$Role\$rel"
        Invoke-Command -Session $s -ArgumentList (Split-Path $dest -Parent) { param($d) New-Item -ItemType Directory -Force $d | Out-Null }
        Copy-Item -ToSession $s -Force -Path $_.FullName -Destination $dest
    }
    # 5. Verify the copy matches the lab's expectations.
    $v = Invoke-Command -Session $s -ArgumentList $g, $Role {
        param($g, $r)
        $h = { param($p) if (Test-Path $p) { (Get-FileHash $p).Hash.Substring(0, 16) } else { 'missing' } }
        [pscustomobject]@{
            pwsh = (& C:\D2Lab\pwsh\pwsh.exe -NoProfile -Command '$PSVersionTable.PSVersion.ToString()')
            assetExe = & $h "$g\work\asset-game\Dishonored2.exe"
            steamApi = & $h "$g\work\asset-game\steam_api64.dll"
            masterIndex = & $h "$g\work\asset-game\base\master.index"
            voidmod = Test-Path "$g\work\asset-game\base\game1voidmod.index"
            gameCopyGB = [math]::Round(((Get-ChildItem -Recurse -File "$g\work\asset-game" | Measure-Object Length -Sum).Sum) / 1GB, 1)
            dll = & $h "$g\outputs\d2-coop-lab\bin\dinput8.dll"
            probe = & $h "$g\outputs\d2-coop-lab\bin\d2_runtime_probe.exe"
            crtInBin = @('msvcp140.dll', 'vcruntime140.dll', 'vcruntime140_1.dll' | Where-Object { Test-Path "$g\outputs\d2-coop-lab\bin\$_" }).Count
            slot81 = Test-Path "$g\work\lab-instances\$r\base\savegame\SLOT81\checkpoint.dat"
            slot82 = Test-Path "$g\work\lab-instances\$r\base\savegame\SLOT82"
            bankMB = [math]::Round((Get-Item "$g\work\lab-instances\$r\coop-status.json.movement.bin" -ErrorAction SilentlyContinue).Length / 1MB)
            freeGB = [math]::Round((Get-PSDrive C).Free / 1GB)
        }
    }
    $v | Format-List | Out-String | Write-Host
    $local = [pscustomobject]@{ dll = (Get-FileHash "$lab\bin\dinput8.dll").Hash.Substring(0, 16); probe = (Get-FileHash "$lab\bin\d2_runtime_probe.exe").Hash.Substring(0, 16); masterIndex = (Get-FileHash (Join-Path $RepoRoot 'work/asset-game/base/master.index')).Hash.Substring(0, 16) }
    if ($v.dll -ne $local.dll -or $v.probe -ne $local.probe -or $v.masterIndex -ne $local.masterIndex) { throw 'Guest binaries or master.index differ from the host lab' }
    if ($v.assetExe -ne 'B671F8249422862B') { throw "Guest game copy exe hash unexpected: $($v.assetExe)" }
    Step 'guest workspace in sync'
} finally { Remove-PSSession $s }
