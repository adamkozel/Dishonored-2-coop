$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$exe=Join-Path $root 'work/asset-game/Dishonored2.exe'
if((Get-Item (Split-Path $exe)).Attributes-band[IO.FileAttributes]::ReparsePoint){throw 'Not the physical isolated game'}
$before=@(foreach($base in @("$env:USERPROFILE/Saved Games/Arkane Studios/Dishonored2/base","$root/work/lab-instances/host/base/savegame/SLOT81","$root/work/lab-instances/client/base/savegame/SLOT81")){Get-ChildItem -LiteralPath $base -File -Recurse|Get-FileHash})
$before|ConvertTo-Json -Depth 4|Set-Content "$PSScriptRoot/protected-save-hashes-before-v4.json"
foreach($role in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
 if($p.Path-ne$exe-or[Math]::Abs(($p.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Isolated game identity changed'}
 Stop-Process -Id $p.Id
}
Start-Sleep -Milliseconds 500
foreach($role in 'host','client'){
 $character=if($role-eq'host'){'emily'}else{'corvo'};$target="$root/work/lab-instances/$role/coop-status.json.movement.bin"
 Copy-Item -LiteralPath $target -Destination "$PSScriptRoot/$role-movement-before-v4.bin"
 Copy-Item -LiteralPath "$root/work/natural-moveset/sprint-banks-v1/$character-sprint-bank.bin" -Destination $target -Force
 & "$lab/Start-D2Lab.ps1" -Role $role -AssetExperiment|Out-Null
}
foreach($f in $before){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected saves changed'}}
'Sprint candidate deployed only in isolated environment'
