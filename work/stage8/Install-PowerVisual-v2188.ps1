$ErrorActionPreference='Stop'
# 2188 (#261): installs the cosmetic fish and Deadly Shade puppet decls (Build-D2PowerVisualAssets-v2187.ps1 -Install) into
# the lab game work/asset-game only. Both isolated lab instances run from that copy and lock its resources, so they are
# stopped first, with the same identity checks as Deploy-SourceClock.ps1; Deploy-SourceClock.ps1 starts them again
# afterwards. The Steam install is only read (the builder checks its indices unchanged).
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$exe=Join-Path $root 'work/asset-game/Dishonored2.exe'
foreach($role in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
 if(!$p){continue}
 if($p.Path-ne$exe-or[Math]::Abs(($p.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Game identity changed'}
 Stop-Process -Id $p.Id
 if(!$p.WaitForExit(15000)){throw 'Isolated process did not fully exit'}
}
Start-Sleep -Seconds 2
& "$lab/Build-D2PowerVisualAssets-v2187.ps1" -Install
