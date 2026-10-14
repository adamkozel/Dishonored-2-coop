$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path $PSScriptRoot -Parent
$expected=(Resolve-Path (Join-Path $workspaceRoot 'work/lab-game/Dishonored2.exe')).Path
foreach($role in 'host','client') {
    $launch=Get-Content (Join-Path $workspaceRoot "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    $gameProcess=Get-Process -Id $launch.pid -ErrorAction SilentlyContinue
    if(!$gameProcess){continue}
    if($gameProcess.Path -ne $expected){throw 'Unexpected game process.'}
    & taskkill.exe /PID $gameProcess.Id /F
    if(!$gameProcess.WaitForExit(10000)){throw 'Game did not exit.'}
}
foreach($role in 'host','client'){& (Join-Path $workspaceRoot 'outputs/d2-coop-lab/Start-D2Lab.ps1') -Role $role|Out-Null}
$readyWatch=[Diagnostics.Stopwatch]::StartNew()
do {
    Start-Sleep -Milliseconds 500
    $ready=$true
    foreach($role in 'host','client') {
        $statusFile=Join-Path $workspaceRoot "work/lab-instances/$role/coop-status.json"
        if(!(Test-Path $statusFile)){$ready=$false;continue}
        $state=Get-Content $statusFile -Raw|ConvertFrom-Json
        if($state.localGameLogicState -ne 6 -or !$state.remoteFresh -or $state.connectionState -ne 3){$ready=$false}
    }
}while(!$ready -and $readyWatch.ElapsedMilliseconds -lt 20000)
if(!$ready){throw 'Lab games did not reach connected native menus.'}
foreach($role in 'host','client') {
    $launch=Get-Content (Join-Path $workspaceRoot "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    [pscustomobject]@{Role=$role;Pid=$launch.pid;Window=(Get-Process -Id $launch.pid).MainWindowHandle}
}
