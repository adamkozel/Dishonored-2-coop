[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$rows=[Collections.Generic.List[object]]::new();$failure=$null
function State([string]$role='client'){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json}
function Trace([string]$role='client'){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json.loading.json") -Raw|ConvertFrom-Json}
function Command([string]$role,[string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null}
try {
    $initial=State;$hostBefore=State host;$before=Trace
    if($initial.localGameLogicState -ne 1 -or $initial.localPendingGameLogicState -or $hostBefore.localGameLogicState -ne 1){throw 'Start with both Dunwall fixtures in gameplay.'}
    $launch=Get-Content (Join-Path $root 'work/lab-instances/client/launch.json') -Raw|ConvertFrom-Json
    $hostRefused=(Trace host).probe.refused
    Command host 'coop_loading_probe 2000';Start-Sleep -Milliseconds 350
    if((Trace host).probe.refused -ne $hostRefused+1){throw 'Host probe was not refused.'}
    Command client 'coop_loading_probe 2000';Start-Sleep -Milliseconds 350
    $armed=Trace
    if(!$armed.probe.armed){throw 'Client probe did not arm.'}
    Command client 'coop_loading_probe 2000';Start-Sleep -Milliseconds 350
    if((Trace).probe.refused -ne $armed.probe.refused+1){throw 'Duplicate arm was not refused.'}
    Command client 'LoadGame 81'
    $watch=[Diagnostics.Stopwatch]::StartNew();$inputSent=$false;$lastPress=0
    do {
        Start-Sleep -Milliseconds 100
        $trace=Trace;$s=State;$hostSample=State host
        $row=[ordered]@{Ms=$watch.ElapsedMilliseconds;Probe=$trace.probe;State=$s;Host=$hostSample}
        if($trace.probe.held) {
            $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch.pid --loading-world $trace.probe.game
            if($LASTEXITCODE){throw 'Independent native loading-state read failed.'}
            $row.NativeLogic=$raw|ConvertFrom-Json
            # Status publication is periodic. An independent read can cross
            # the automatic release after reading the last held status file.
            $row.ReleaseBoundaryRead=$row.NativeLogic.tick -ge $trace.probe.startedAt+2000
            if(!$row.ReleaseBoundaryRead -and ($row.NativeLogic.logic -ne 15 -or $row.NativeLogic.pending)){
                $rows.Add([pscustomobject]$row);throw 'Held world reached another native logic state before release.'
            }
            if(!$inputSent){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -LeftY 16000 -DurationMs 650|Out-Null;$inputSent=$true}
        } elseif($watch.ElapsedMilliseconds -gt 6000 -and $watch.ElapsedMilliseconds-$lastPress -gt 600 -and $s.localGameLogicState -in 8,15 -and $trace.probe.released -eq $before.probe.released) {
            & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -Button A -DurationMs 150|Out-Null
            $lastPress=$watch.ElapsedMilliseconds
        }
        $rows.Add([pscustomobject]$row)
    }while($watch.ElapsedMilliseconds -lt 25000 -and !($trace.probe.released -gt $before.probe.released -and $s.localGameLogicState -eq 1 -and !$s.localPendingGameLogicState))
    $held=@($rows|Where-Object {$_.Probe.held -and !$_.ReleaseBoundaryRead})
    if(!$inputSent -or $held.Count -lt 3 -or $trace.probe.started -ne $before.probe.started+1 -or $trace.probe.released -ne $before.probe.released+1 -or $trace.probe.invalidated -ne $before.probe.invalidated){throw 'Expected one bounded hold and automatic release.'}
    if($trace.probe.releasedAt-$trace.probe.startedAt -lt 2000 -or $trace.probe.releasedAt-$trace.probe.startedAt -gt 3000 -or $trace.probe.maxDistance -gt 0.001){throw 'Hold duration or stationary-player check failed.'}
    foreach($r in $held){for($i=0;$i -lt 3;$i++){if([Math]::Abs($r.NativeLogic.position[$i]-$held[0].NativeLogic.position[$i]) -gt 0.001){throw 'Independent read found movement during the loading hold.'}}}
    if($held[-1].Host.received -le $held[0].Host.received -or @($rows|Where-Object {$_.Host.localEpoch -ne $hostBefore.localEpoch -or $_.Host.localGameLogicState -ne 1}).Count){throw 'Host world or Steam transport did not remain active.'}
    if($s.localGameLogicState -ne 1 -or $s.localPendingGameLogicState){throw 'Client failed to resume gameplay.'}
}catch{$failure=$_.Exception.Message}finally {
    try {& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -DurationMs 100|Out-Null}catch {if(!$failure){$failure=$_.Exception.Message}}
}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;
    Initial=$initial;Before=$before;Armed=$armed;FinalTrace=$trace;Samples=$rows;
    Scope='Explicit client-only two-second hold before native loading-to-gameplay switch. Independent state reads, expiring movement input, duplicate/host refusal and continuing Steam traffic. No role conversion or shared reset.'}|
    ConvertTo-Json -Depth 14|Set-Content (Join-Path $PSScriptRoot 'research/native-loading-hold-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;HeldSamples=$held.Count;HeldMs=$trace.probe.releasedAt-$trace.probe.startedAt;MaxMovement=$trace.probe.maxDistance}
