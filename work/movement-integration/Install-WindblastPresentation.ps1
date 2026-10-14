$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$exe=Join-Path $root 'work/asset-game/Dishonored2.exe'
foreach($r in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
 if(!$p){continue}
 if(!$l.assetExperiment-or$p.Path-ne$exe-or[Math]::Abs(($p.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Unexpected game process'}
 Stop-Process -Id $p.Id
 if(!$p.WaitForExit(15000)){throw 'Isolated instance still running'}
}
& "$lab/Build-D2WindblastPresentationAssets.ps1" -Install
if($LASTEXITCODE){throw 'Asset installation failed'}
foreach($r in 'host','client'){& "$lab/Start-D2Lab.ps1" -Role $r -AssetExperiment|Out-Null}
& "$root/work/PrepareShadowSword.ps1"
foreach($f in(Get-Content "$PSScriptRoot/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
'Windblast presentation asset installed; clean native test world ready'
