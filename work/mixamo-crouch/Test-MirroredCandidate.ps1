param([ValidateSet('corvo','emily')][string]$Character='corvo')
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$lab="$root/outputs/d2-coop-lab"
$source=if($Character-eq'corvo'){'host'}else{'client'}
$target=if($source-eq'host'){'client'}else{'host'}
$stride=if($Character-eq'corvo'){2.04}else{2.05}
function State($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function Mix{Get-Content "$root/work/lab-instances/$target/coop-status.json.mixamo.json" -Raw|ConvertFrom-Json}
function Cmd($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function Stance($r,$v){if((State $r).localPlayerStance-ne$v){& "$lab/Send-D2Input.ps1" -Role $r -Button B -DurationMs 150|Out-Null;Start-Sleep -Milliseconds 900}}
Cmd $target 'coop_mixamo_crouch off'
Stance $source 0;Stance $target 0
Cmd $source 'setviewpos -33 -153 68.1 90 0';Cmd $target 'setviewpos -29 -151.5 68.1 200 22'
Start-Sleep -Milliseconds 600
Stance $source 1;Cmd $target 'coop_mixamo_crouch on';Start-Sleep -Milliseconds 500
$before=State $target;$m0=Mix
if(!$m0.active-or!$m0.persistent){throw 'Session candidate failed to arm'}
& "$lab/Send-D2Input.ps1" -Role $source -LeftY 16000 -DurationMs 750|Out-Null
Start-Sleep -Milliseconds 1500 # input publication returns immediately; include the 750ms hold and interpolation/status settling
$after=State $target;$m1=Mix
$distance=[Math]::Sqrt([Math]::Pow($after.probeTargetPosition[0]-$before.probeTargetPosition[0],2)+[Math]::Pow($after.probeTargetPosition[1]-$before.probeTargetPosition[1],2))
$phase=($m1.phase-$m0.phase+1)%1
if($distance-lt.2-or[Math]::Abs($phase-$distance/$stride)-gt.015){throw "Phase/travel disagreement: $phase, $distance m"}
Start-Sleep -Milliseconds 900;$idle=Mix
if([Math]::Abs($idle.phase-$m1.phase)-gt.001){throw 'Gait kept advancing at rest'}
& "$PSScriptRoot/Capture-MixamoState.ps1" -Stage "mirror-$Character-forward" -TargetRole $target|Out-Null
Stance $source 0;Start-Sleep -Milliseconds 400;$stand0=Mix
Start-Sleep -Milliseconds 500;$stand1=Mix
if($stand0.applied-ne$stand1.applied){throw 'Donor still writing standing pose'}
Stance $source 1;Start-Sleep -Milliseconds 500;$again=Mix
if(!$again.active-or$again.applied-le$stand1.applied){throw 'Current-session crouch did not resume'}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Character=$Character;Dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;Passed=$true;DistanceMeters=$distance;PhaseAdvanced=$phase;CycleDistanceMeters=$stride;IdlePhaseChange=($idle.phase-$m1.phase);StandingWrites=($stand1.applied-$stand0.applied);ResumedWrites=($again.applied-$stand1.applied);Before=$before;After=$after;MixBefore=$m0;MixAfter=$again}|ConvertTo-Json -Depth 10|Set-Content "$PSScriptRoot/mirror-$Character-runtime.json"
[pscustomobject]@{Character=$Character;Passed=$true;Distance=$distance;Phase=$phase;StandingWrites=0;Resumed=$true}
