$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$exe=Join-Path $root 'work/asset-game/Dishonored2.exe'
if((Get-Item (Split-Path $exe)).Attributes-band[IO.FileAttributes]::ReparsePoint){throw 'Not physical isolated game'}
$before=Get-Content "$PSScriptRoot/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json
foreach($f in $before){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save differs'}}
foreach($role in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
 if(!$p){continue} # A crashed isolated instance has nothing left to stop.
 if($p.Path-ne$exe-or[Math]::Abs(($p.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Game identity changed'}
 Stop-Process -Id $p.Id
 if(!$p.WaitForExit(15000)){throw 'Isolated process did not fully exit; loader remains protected'}
}
Start-Sleep -Milliseconds 500
foreach($role in 'host','client'){& "$lab/Start-D2Lab.ps1" -Role $role -AssetExperiment|Out-Null}
& "$root/work/PrepareShadowSword.ps1"
foreach($f in $before){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}}
'Source clock candidate ready; protected saves unchanged'
