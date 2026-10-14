[CmdletBinding()]
param()
$ErrorActionPreference = 'Stop'
$workspaceRoot = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$buildDirectory = Join-Path $workspaceRoot 'work\build'
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$vsRoot = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $vsRoot) { throw 'Visual Studio C++ Build Tools are required.' }
$cmake = Join-Path $vsRoot 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
$nodeCommand = Get-Command node -ErrorAction SilentlyContinue
$nodePath = if ($nodeCommand) { $nodeCommand.Source } else { Join-Path $env:USERPROFILE '.cache\codex-runtimes\codex-primary-runtime\dependencies\node\bin\node.exe' }
if (-not (Test-Path -LiteralPath $nodePath)) { throw 'Node.js is needed to normalize the build environment.' }
$cleanRunner = Join-Path $PSScriptRoot 'tooling\run-clean.cjs'
& $nodePath $cleanRunner $cmake -S $PSScriptRoot -B $buildDirectory -G 'Visual Studio 17 2022' -A x64
if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed.' }
& $nodePath $cleanRunner $cmake --build $buildDirectory --config Release
if ($LASTEXITCODE -ne 0) { throw 'Build failed.' }
$bin = Join-Path $PSScriptRoot 'bin'
New-Item -ItemType Directory -Path $bin -Force | Out-Null
foreach ($file in 'dinput8.dll','d2_runtime_probe.exe','d2_proxy_smoke.exe','d2_steam_probe.exe') {
    Copy-Item -LiteralPath (Join-Path $buildDirectory "Release\$file") -Destination $bin -Force
}
& (Join-Path $bin 'd2_proxy_smoke.exe') (Join-Path $bin 'dinput8.dll')
if ($LASTEXITCODE -ne 0) { throw 'Proxy contract verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_world_identity_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'World NPC identity verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_guard_protocol_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Guard binding protocol verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_damage_protocol_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Sword damage protocol verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_guard_body_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Guard animation protocol verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_guard_weapon_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Guard equipment protocol verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_enemy_outcome_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Enemy outcome protocol verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_enemy_melee_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Enemy melee protocol verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_checkpoint_protocol_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Checkpoint protocol verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_native_skeleton_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Native skeleton verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_arm_protocol_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Arm stream protocol verification failed.' }
& (Join-Path $buildDirectory 'Release/d2_avatar_weapon_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Avatar weapon protocol verification failed.' }

& (Join-Path $buildDirectory 'Release/d2_shadow_action_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Shadow action protocol verification failed.' }

& (Join-Path $buildDirectory 'Release/d2_pose_interpolation_tests.exe')
if ($LASTEXITCODE -ne 0) { throw 'Pose interpolation verification failed.' }
