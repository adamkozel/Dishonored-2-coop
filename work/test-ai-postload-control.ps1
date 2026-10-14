$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
$launch=Get-Content (Join-Path $root 'work/lab-instances/client/launch.json') -Raw|ConvertFrom-Json
if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected lab process.'}
function State {Get-Content (Join-Path $root 'work/lab-instances/client/coop-status.json') -Raw|ConvertFrom-Json}
$before=State
if($before.aiHoldActive){throw 'No AI hold may be active in the control run.'}
& (Join-Path $lab 'Send-D2Command.ps1') -Role client -Command 'LoadGame 81'|Out-Null
$watch=[Diagnostics.Stopwatch]::StartNew();$changed=$false
do {
    Start-Sleep -Milliseconds 100;$s=State
    if($s.localEpoch -ne $before.localEpoch){$changed=$true}
    if($changed -and $s.localGameLogicState -eq 1 -and $s.localPendingGameLogicState -eq 0){break}
    if($watch.ElapsedMilliseconds -gt 6000 -and $s.localGameLogicState -in 8,15){& (Join-Path $lab 'Send-D2Input.ps1') -Role client -Button A -DurationMs 200|Out-Null}
}while($watch.ElapsedMilliseconds -lt 25000)
if(!$changed -or $s.localGameLogicState -ne 1){throw 'Control checkpoint did not load.'}
$catalog=& (Join-Path $lab 'bin/d2_runtime_probe.exe') $launch.pid --world-npcs
if($LASTEXITCODE){throw 'No native control catalog.'}
$guard=@(($catalog|ConvertFrom-Json).npcs|Where-Object spawnerName -eq 'ld_spawner_guard_tutorial_1')
if($guard.Count -ne 1 -or !$guard[0].unique){throw 'Control guard is not unique.'}
$samples=[Collections.Generic.List[object]]::new();$watch.Restart()
do {
    $native=& (Join-Path $lab 'bin/d2_runtime_probe.exe') $launch.pid --ai-decision $guard[0].handle
    if($LASTEXITCODE){throw 'Control AI snapshot unavailable.'}
    $s=State;$n=$native|ConvertFrom-Json
    if($s.aiHoldActive -or $s.aiHoldStarted -ne $before.aiHoldStarted -or $n.pauseReferences -ne 0 -or !$n.schedulerActive){throw 'Control run is contaminated by an AI hold.'}
    $samples.Add([pscustomobject]@{Ms=$watch.ElapsedMilliseconds;Native=$n;HoldStarted=$s.aiHoldStarted})
    Start-Sleep -Milliseconds 150
}while($watch.ElapsedMilliseconds -lt 6000)
$variants=@($samples|ForEach-Object {$_.Native.activeIndices -join ','}|Select-Object -Unique)
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $lab 'bin/dinput8.dll')).Hash;
    NoHoldApplied=$true;ObservedMembershipChanges=($variants.Count -gt 1);Variants=$variants;Samples=$samples.ToArray();
    Scope='Control observation after native checkpoint load without issuing any AI hold. Establishes whether other decision components change scheduler membership independently of this experiment.'}|
    ConvertTo-Json -Depth 8|Set-Content (Join-Path $lab 'research/ai-postload-control.json')
if(!(State).probeActorHandle){& (Join-Path $lab 'Send-D2Command.ps1') -Role client -Command 'coop_spawn_probe corvo'|Out-Null;Start-Sleep -Milliseconds 700}
foreach($role in 'host','client'){& (Join-Path $lab 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_follow on'|Out-Null;Start-Sleep -Milliseconds 200}
[pscustomobject]@{NoHoldApplied=$true;Variants=$variants;Samples=$samples.Count}
