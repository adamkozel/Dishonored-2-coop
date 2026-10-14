$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$lab=Join-Path $root 'outputs/d2-coop-lab'
$game=Join-Path $root 'work/asset-game'
if((Get-Item $game).Attributes-band[IO.FileAttributes]::ReparsePoint){throw 'Not isolated physical game'}
foreach($f in (Get-Content "$PSScriptRoot/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){
 if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}
}
$candidate=Join-Path $PSScriptRoot 'candidate-fetal-release-v35'
$identity=Get-Content "$candidate/identity.json" -Raw|ConvertFrom-Json
if((Get-FileHash "$candidate/dinput8.dll").Hash-ne$identity.dll){throw 'Candidate changed'}
foreach($role in 'host','client'){
 $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 $p=Get-Process -Id $launch.pid -ErrorAction Stop
 if($p.Path-ne(Join-Path $game 'Dishonored2.exe')-or[Math]::Abs(($p.StartTime-([DateTime]$launch.startedAt)).TotalSeconds)-gt5){throw 'Game identity changed'}
 Stop-Process -Id $p.Id
 if(!$p.WaitForExit(15000)){throw 'Isolated process did not exit'}
}
& "$lab/Build-D2CarryFetalAssets.ps1" -Install
Copy-Item -LiteralPath "$candidate/dinput8.dll" -Destination "$lab/bin/dinput8.dll" -Force
& "$PSScriptRoot/Deploy-SourceClock.ps1"
if((Get-FileHash "$game/dinput8.dll").Hash-ne$identity.dll){throw 'Live candidate mismatch'}
