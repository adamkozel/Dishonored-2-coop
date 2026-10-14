$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $workspaceRoot 'outputs/d2-coop-lab'
function Pair([string]$button,[int]$duration,[int]$wait) {
    foreach($role in 'host','client'){& (Join-Path $lab 'Send-D2Input.ps1') -Role $role -Button $button -DurationMs $duration|Out-Null}
    Start-Sleep -Milliseconds $wait
}
function State([string]$role) {
    Get-Content (Join-Path $workspaceRoot "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json
}
# Setup route for fresh lab processes at the observed native title screen.
# Slot 02 is the disposable test slot; user saves live outside these isolated roots.
foreach($role in 'host','client'){if((State $role).localGameLogicState -ne 6){throw 'Fresh native title/menu state required.'}}
Pair A 400 1200
Pair A 150 800
Pair Down 150 350
Pair A 150 700
Pair Down 150 350
Pair A 150 500
Pair Right 150 350
Pair A 150 600
Pair Up 150 350
Pair A 150 600
Pair A 150 5000
Pair A 400 2200
Pair X 2500 5500
foreach($role in 'host','client'){& (Join-Path $lab 'Send-D2Command.ps1') -Role $role -Command 'coop_cinematic_sync on'|Out-Null}
Start-Sleep -Milliseconds 700
& (Join-Path $lab 'Test-D2SharedSkip.ps1') -SourceRole host -Destination Choice
Start-Sleep -Milliseconds 1800
& (Join-Path $lab 'Send-D2Input.ps1') -Role host -LeftX -25000 -DurationMs 650|Out-Null
Start-Sleep -Milliseconds 1500
foreach($role in 'host','client') {
    for($attempt=0;$attempt -lt 3 -and (State $role).localGameLogicState -eq 14;$attempt++) {
        # The character selector accepts the native interact event (X), which
        # reaches 0x6818b0 even when a menu-confirm A pulse is ignored.
        & (Join-Path $lab 'Send-D2Input.ps1') -Role $role -Button X -DurationMs 250|Out-Null
        Start-Sleep -Milliseconds 700
    }
    if((State $role).localGameLogicState -ne 3){throw "$role did not open its native confirmation dialog."}
    & (Join-Path $lab 'Send-D2Command.ps1') -Role $role -Command 'coop_cinematic_barrier on'|Out-Null
}
Start-Sleep -Milliseconds 500
'Both native confirmation dialogs prepared using analog character selection; the rendezvous test checks actual Corvo/Emily branches.'
