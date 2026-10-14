[CmdletBinding()]
param(
    [ValidateSet('host','client')][string]$Role = 'host',
    [string]$GameDirectory = 'C:\Program Files (x86)\Steam\steamapps\common\Dishonored2',
    [switch]$PrepareOnly
)
$ErrorActionPreference = 'Stop'
$workspaceRoot = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$labGameDirectory = Join-Path $workspaceRoot 'work\lab-game'
$profilesRoot = Join-Path $workspaceRoot 'work\lab-instances'
$executable = Join-Path $GameDirectory 'Dishonored2.exe'
$expectedHash = 'B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
if ((Get-FileHash -LiteralPath $executable -Algorithm SHA256).Hash -ne $expectedHash) {
    throw 'Game executable differs from the inspected Steam build. Reinspect before testing.'
}
$priorLaunchFile = Join-Path $profilesRoot "$Role\launch.json"
if (-not $PrepareOnly -and (Test-Path -LiteralPath $priorLaunchFile)) {
    $priorLaunch = Get-Content -LiteralPath $priorLaunchFile -Raw | ConvertFrom-Json
    if ($priorLaunch.pid) {
        $priorProcess = Get-Process -Id $priorLaunch.pid -ErrorAction SilentlyContinue
        if ($priorProcess -and $priorProcess.Path -eq $priorLaunch.executable -and $priorProcess.StartTime.ToUniversalTime() -ge ([DateTimeOffset]$priorLaunch.startedAt).UtcDateTime.AddSeconds(-5)) {
            throw "The $Role test instance is still running (PID $($priorLaunch.pid))."
        }
    }
}
$instanceRoot = Join-Path $profilesRoot $Role
$baseDirectory = Join-Path $instanceRoot 'base'
New-Item -ItemType Directory -Path $baseDirectory -Force | Out-Null
$baseline = Join-Path $env:USERPROFILE 'Saved Games\Arkane Studios\Dishonored2\base'
# Copy once. Subsequent launches preserve each test instance's progress.
$seedMarker = Join-Path $instanceRoot 'seeded.txt'
if (-not (Test-Path -LiteralPath $seedMarker)) {
    if (Test-Path -LiteralPath $baseline) {
        foreach ($item in (Get-ChildItem -LiteralPath $baseline -Force)) {
            if ($item.Name -eq 'steam_autocloud.vdf') { continue }
            Copy-Item -LiteralPath $item.FullName -Destination $baseDirectory -Recurse
        }
    }
    Set-Content -LiteralPath $seedMarker -Value 'Copied baseline; isolated runtime path must still be verified.'
}
# Copy only the executable and root dependencies; read large assets from the
# installed game via a workspace junction. Never install this DLL into Steam.
if (-not (Test-Path -LiteralPath $labGameDirectory)) {
    New-Item -ItemType Directory -Path $labGameDirectory | Out-Null
    foreach ($gameFile in (Get-ChildItem -LiteralPath $GameDirectory -File)) {
        if ($gameFile.Extension -in '.exe','.dll' -and $gameFile.Name -ne 'dinput8.dll') {
            Copy-Item -LiteralPath $gameFile.FullName -Destination $labGameDirectory
        }
    }
}
$assetLink = Join-Path $labGameDirectory 'base'
$assetDirectory = Join-Path $GameDirectory 'base'
if (-not (Test-Path -LiteralPath $assetLink)) {
    New-Item -ItemType Junction -Path $assetLink -Target $assetDirectory | Out-Null
} elseif ((Get-Item -LiteralPath $assetLink).Target -ne $assetDirectory) {
    throw 'Workspace asset junction points to an unexpected directory.'
}
$proxySource = Join-Path $PSScriptRoot 'bin\dinput8.dll'
if (-not (Test-Path -LiteralPath $proxySource)) { throw 'Lab loader has not been built.' }
$proxyDestination = Join-Path $labGameDirectory 'dinput8.dll'
if (-not (Test-Path -LiteralPath $proxyDestination)) {
    Copy-Item -LiteralPath $proxySource -Destination $proxyDestination
} elseif ((Get-FileHash -LiteralPath $proxyDestination).Hash -ne (Get-FileHash -LiteralPath $proxySource).Hash) {
    if (Get-Process -Name Dishonored2 -ErrorAction SilentlyContinue | Where-Object Path -eq (Join-Path $labGameDirectory 'Dishonored2.exe')) { throw 'Close both lab instances before updating the loader.' }
    Copy-Item -LiteralPath $proxySource -Destination $proxyDestination -Force
}
$executable = Join-Path $labGameDirectory 'Dishonored2.exe'
if ((Get-FileHash -LiteralPath $executable -Algorithm SHA256).Hash -ne $expectedHash) { throw 'Workspace executable hash differs from the inspected build.' }
$savePathArgument = $instanceRoot.Replace('\','/')
$testSettings = [ordered]@{
    com_showLoadingScreen = 0
    r_renderQualityPresets = 6
    r_maxDynamicDecalsPreset = 0
    r_textureDetailsPreset = 0
    r_viewDistPreset = 0
    r_modelsQualityPreset = 0
    r_oceanQualityPreset = 0
    r_shadowQualityPreset = 0
    r_downsampleQualityPreset = 0
    r_AntiAliasingPreset = 0
    r_skipCameraMotionBlur = 1
    r_skipSSAO = 1
    r_fullscreen = 0
    r_windowWidth = 944
    r_windowHeight = 540
    r_windowPosX = $(if ($Role -eq 'host') { 8 } else { 968 })
    r_windowPosY = 40
    win_pauseOnFocusLoss = 0
    in_controlInactiveWindow = 1
}
$configFile = Join-Path $baseDirectory 'dishonored2Config.cfg'
$config = if (Test-Path -LiteralPath $configFile) { Get-Content -LiteralPath $configFile -Raw } else { '' }
foreach ($setting in $testSettings.GetEnumerator()) {
    $pattern = '(?m)^' + [regex]::Escape($setting.Key) + '\s+[^\r\n]*\r?\n?'
    $config = [regex]::Replace($config, $pattern, '')
}
$config = $config.TrimEnd() + "`r`n" + (($testSettings.GetEnumerator() | ForEach-Object { $_.Key + ' "' + $_.Value + '"' }) -join "`r`n") + "`r`n"
Set-Content -LiteralPath $configFile -Value $config -NoNewline -Encoding utf8
$argumentLine = '+com_showLoadingScreen 0 +set fs_savepath "' + $savePathArgument + '"'
foreach ($setting in $testSettings.GetEnumerator()) { $argumentLine += ' +set ' + $setting.Key + ' ' + $setting.Value }
$launch = [ordered]@{
    role = $Role
    executable = $executable
    sha256 = $expectedHash
    savePath = $instanceRoot
    arguments = $argumentLine
    testSettings = $testSettings
    note = 'Independent single-player instances only; no multiplayer implementation yet.'
}
if (-not $PrepareOnly) {
    $staleCommand = Join-Path $instanceRoot 'command.bin'
    if(Test-Path -LiteralPath $staleCommand){Remove-Item -LiteralPath $staleCommand}
    # These are interactive, windowed game instances requested by the user.
    $gameProcess = Start-Process -FilePath $executable -WorkingDirectory $labGameDirectory -ArgumentList $argumentLine -Environment @{SteamAppId='403640';SteamGameId='403640';D2COOP_LAB_ROLE=$Role;D2COOP_LAB_POLICY='1';D2COOP_LAB_INPUT=(Join-Path $instanceRoot 'input.bin');D2COOP_LAB_LOG=(Join-Path $instanceRoot 'loader.log');D2COOP_ENGINE_BRIDGE='1';D2COOP_CHECKPOINT_DIR=(Join-Path $instanceRoot 'base/savegame/SLOT82');D2COOP_COMMAND_FILE=(Join-Path $instanceRoot 'command.bin');D2COOP_STATUS_FILE=(Join-Path $instanceRoot 'coop-status.json');D2COOP_ENGINE_LOG=(Join-Path $instanceRoot 'engine.log')} -WindowStyle Normal -PassThru
    $launch.pid = $gameProcess.Id
    $launch.startedAt = [DateTimeOffset]::Now.ToString('o')
}
$launch | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $instanceRoot 'launch.json')
$launch | ConvertTo-Json
