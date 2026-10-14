$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot
$game=[IO.Path]::GetFullPath((Join-Path $root 'work/asset-game/Dishonored2.exe'))
foreach($role in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 $p=Get-Process -Id $l.pid -ErrorAction Stop
 if($p.Path-ne$game-or$l.executable-ne$game-or[Math]::Abs(($p.StartTime.ToUniversalTime()-([DateTimeOffset]$l.startedAt).UtcDateTime).TotalSeconds)-gt10){throw 'Unexpected test process'}
 Stop-Process -Id $p.Id -Force
 if(!$p.WaitForExit(10000)){throw 'Isolated process still running'}
}
& "$root/outputs/d2-coop-lab/Build-D2DoppelgangerAssets.ps1" -Install *> "$root/work/crouch-pose-survey-install.log"
foreach($role in 'host','client'){& "$root/outputs/d2-coop-lab/Start-D2Lab.ps1" -Role $role -AssetExperiment|Out-Null}
& "$root/outputs/d2-coop-lab/Restore-D2Lab.ps1" -Doppelganger
