[CmdletBinding()]
param([switch]$AssetExperiment)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$allowed=@((Join-Path $root 'work/lab-game/Dishonored2.exe'),(Join-Path $root 'work/asset-game/Dishonored2.exe')) | ForEach-Object {[IO.Path]::GetFullPath($_)}
foreach($role in 'host','client'){
 $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 $p=Get-Process -Id $launch.pid -ErrorAction SilentlyContinue
 if(!$p){continue}
 if($p.Path -notin $allowed -or $p.Path -ne $launch.executable -or [Math]::Abs(($p.StartTime.ToUniversalTime()-([DateTimeOffset]$launch.startedAt).UtcDateTime).TotalSeconds)-gt10){throw 'Unexpected game process identity.'}
 Stop-Process -Id $p.Id -Force
 if(!$p.WaitForExit(10000)){throw 'Test game did not exit.'}
}
foreach($role in 'host','client'){& "$PSScriptRoot/Start-D2Lab.ps1" -Role $role -AssetExperiment:$AssetExperiment|Out-Null}
foreach($role in 'host','client') {
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 [pscustomobject]@{Role=$role;Pid=$l.pid;Executable=$l.executable;AssetExperiment=$l.assetExperiment}
}
