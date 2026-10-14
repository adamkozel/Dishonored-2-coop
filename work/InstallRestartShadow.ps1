$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
Set-Location -LiteralPath $root
$allowed=[IO.Path]::GetFullPath((Join-Path $root 'work/asset-game/Dishonored2.exe'))
foreach($role in 'host','client'){
 $l=Get-Content "work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 $p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
 if(!$p){continue}
 if($p.Path -ne $allowed -or $p.Path -ne $l.executable -or [Math]::Abs(($p.StartTime.ToUniversalTime()-([DateTimeOffset]$l.startedAt).UtcDateTime).TotalSeconds)-gt10){throw 'Unexpected process'}
 Stop-Process -Id $p.Id -Force
 if(!$p.WaitForExit(10000)){throw 'Still running'}
}
& 'C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe' -NoProfile -File outputs/d2-coop-lab/Build-D2DoppelgangerAssets.ps1 -Install > work/doppelganger-action-install.json
if($LASTEXITCODE){throw 'Asset install failed'}
& outputs/d2-coop-lab/Restart-D2Lab.ps1 -AssetExperiment
Start-Sleep -Seconds 6
& outputs/d2-coop-lab/Restore-D2Lab.ps1 -Doppelganger
