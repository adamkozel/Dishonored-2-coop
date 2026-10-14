$ErrorActionPreference='Stop'
$workspaceRoot=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vsRoot=& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$cmake=Join-Path $vsRoot 'Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$node=(Get-Command node).Source
$runner=Join-Path $workspaceRoot 'outputs/d2-coop-lab/tooling/run-clean.cjs'
& $node $runner $cmake -S $PSScriptRoot -B "$PSScriptRoot/build" -G 'Visual Studio 17 2022' -A x64
if($LASTEXITCODE){throw 'Configure failed'}
& $node $runner $cmake --build "$PSScriptRoot/build" --config Release
if($LASTEXITCODE){throw 'Build failed'}
