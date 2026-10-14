# Applies the lab's low-load graphics settings to Dishonored 2 inside the VM (d2lab's config):
# windowed 944x540, every quality preset at its lowest, SSAO/motion blur off, vsync on. These are
# the same values outputs/d2-coop-lab/Start-D2Lab.ps1 forces on both host lab instances.
# The game must not be running (it rewrites the file on exit). pwsh 7, Hyper-V Administrators.
# First VM launch (2026-10-14) used the defaults (1920x1080, auto quality) and the host bugchecked.
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')
$settings = [ordered]@{
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
    r_mode = 0
    r_windowWidth = 944
    r_windowHeight = 540
    r_windowPosX = 8
    r_windowPosY = 40
    r_swapInterval = 1
    win_pauseOnFocusLoss = 0
}
Invoke-D2LabVM -ArgumentList (,$settings) -ScriptBlock {
    param($settings)
    if (Get-Process Dishonored2 -ErrorAction SilentlyContinue) { throw 'Dishonored 2 is running in the VM; close it first' }
    $dir = 'C:\Users\d2lab\Saved Games\Arkane Studios\Dishonored2\base'
    New-Item -ItemType Directory -Force $dir | Out-Null
    $file = Join-Path $dir 'dishonored2Config.cfg'
    $config = if (Test-Path $file) { Get-Content -Raw $file } else { '' }
    if ((Test-Path $file) -and !(Test-Path "$file.before-lab-settings")) { Copy-Item $file "$file.before-lab-settings" }
    foreach ($k in $settings.Keys) { $config = [regex]::Replace($config, '(?m)^' + [regex]::Escape($k) + '\s+[^\r\n]*\r?\n?', '') }
    $config = $config.TrimEnd() + "`r`n" + (($settings.GetEnumerator() | ForEach-Object { $_.Key + ' "' + $_.Value + '"' }) -join "`r`n") + "`r`n"
    [IO.File]::WriteAllText($file, $config, [Text.UTF8Encoding]::new($false))
    Get-Content $file | Select-String -Pattern '^(r_fullscreen|r_windowWidth|r_windowHeight|r_renderQualityPresets|r_textureDetailsPreset|r_shadowQualityPreset|r_swapInterval) ' | ForEach-Object { $_.Line }
}
