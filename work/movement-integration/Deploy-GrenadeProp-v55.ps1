$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$exe=Join-Path $root 'work/asset-game/Dishonored2.exe'
$archive=Join-Path $PSScriptRoot 'candidate-grenade-prop-v55';$identity=Get-Content "$archive/identity.json" -Raw|ConvertFrom-Json
foreach($f in Get-Content "$PSScriptRoot/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file differs'}}
foreach($r in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
 if(!$p){continue};if($p.Path-ne$exe-or[Math]::Abs(($p.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Isolated process identity changed'}
 Stop-Process -Id $p.Id;if(!$p.WaitForExit(15000)){throw 'Isolated process did not exit'}
}
& "$lab/Build-D2GrenadePropAssets.ps1" -Install *> "$PSScriptRoot/grenade-prop-install-v55.log"
$installed=Get-Content "$lab/research/grenade-prop-presentation-asset-install.json" -Raw|ConvertFrom-Json
if(!$installed.Installed-or!$installed.OriginalSteamIndicesUnchanged){throw 'Asset installation unconfirmed'}
Copy-Item "$archive/dinput8.dll" "$lab/bin/dinput8.dll" -Force;Copy-Item "$archive/d2_runtime_probe.exe" "$lab/bin/d2_runtime_probe.exe" -Force
& "$PSScriptRoot/Deploy-SourceClock.ps1"
if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$identity.dll-or(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash-ne$identity.probe){throw 'Live pins differ'}
'Grenade cosmetic prop candidate deployed with preserved asset backup'
