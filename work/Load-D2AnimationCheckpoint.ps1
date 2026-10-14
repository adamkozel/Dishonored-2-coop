$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
foreach($role in 'host','client') {
    $s=Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json
    if($s.localGameLogicState -ne 6){throw 'Fresh native menus required.'}
    & (Join-Path $lab 'Send-D2Input.ps1') -Role $role -Button A -DurationMs 400|Out-Null
}
Start-Sleep -Milliseconds 1200
foreach($role in 'host','client'){& (Join-Path $lab 'Send-D2Command.ps1') -Role $role -Command 'LoadGame 81'|Out-Null}
Start-Sleep -Milliseconds 5000
for($attempt=0;$attempt -lt 10;$attempt++) {
    $ready=$true
    foreach($role in 'host','client') {
        $s=Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json
        if($s.localGameLogicState -ne 1){$ready=$false;& (Join-Path $lab 'Send-D2Input.ps1') -Role $role -Button A -DurationMs 400|Out-Null}
    }
    if($ready){break}
    Start-Sleep -Milliseconds 1000
}
if(!$ready){throw 'Native isolated checkpoint did not finish loading.'}
foreach($role in 'host','client') {
    $character=if($role -eq 'host'){'emily'}else{'corvo'}
    & (Join-Path $lab 'Send-D2Command.ps1') -Role $role -Command "coop_spawn_probe $character"|Out-Null
}
Start-Sleep -Milliseconds 800
foreach($role in 'host','client'){& (Join-Path $lab 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_follow on'|Out-Null}
Start-Sleep -Milliseconds 500
foreach($role in 'host','client') {
    $s=Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json
    if(!$s.probeFollowEnabled){throw "$role did not enable the independent owned avatar."}
}
'Both isolated checkpoints and owned avatars ready.'
