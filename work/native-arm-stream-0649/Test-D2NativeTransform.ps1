[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
function Read-Status([string]$Role) {
    $file=Get-Item -LiteralPath (Join-Path $workspaceRoot "work\lab-instances\$Role\coop-status.json")
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale engine status.'}
    $status=Get-Content -LiteralPath $file.FullName -Raw|ConvertFrom-Json
    if(!$status.localPoseAvailable -or !$status.remotePoseAvailable -or !$status.remoteFresh -or $status.connectionState -ne 3){throw 'Loaded Steam peers required.'}
    $status
}
function Distance($A,$B,[int]$Count=3) {
    $sum=0.0;for($i=0;$i -lt $Count;$i++){$d=[double]$A[$i]-[double]$B[$i];$sum+=$d*$d};[Math]::Sqrt($sum)
}
$results=@()
try {
    foreach($role in 'host','client') {
        $other=if($role -eq 'host'){'client'}else{'host'}
        $initial=Read-Status $other
        if(!$initial.probeActorHandle){throw 'Native probe clone required.'}
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $other -Command 'coop_probe_follow on'|Out-Null
        Start-Sleep -Milliseconds 400
        $request=& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $other -Command 'coop_probe_trace_transform'
        Start-Sleep -Milliseconds 200
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -LeftX 16000 -DurationMs 500|Out-Null
        Start-Sleep -Milliseconds 900
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -RightX 15000 -DurationMs 400|Out-Null
        Start-Sleep -Milliseconds 3400
        $status=Read-Status $other
        $trace=Get-Content -LiteralPath (Join-Path $workspaceRoot "work\lab-instances\$other\coop-status.json.transform.json") -Raw|ConvertFrom-Json
        if($trace.command -ne $request.Sequence -or $trace.handle -ne $initial.probeActorHandle -or $status.probeActorHandle -ne $trace.handle){throw 'Trace did not match this request and clone.'}
        $maxError=0.0;$moved=0;$turned=0;$deferred=0;$bodyCount=0
        foreach($sample in $trace.samples) {
            if($sample.deferred){$deferred++}
            if((Distance $sample.before $sample.after) -gt 0.0001){$moved++}
            if((Distance $sample.before[3..11] $sample.after[3..11] 9) -gt 0.0001){$turned++}
            if($sample.bodiesBefore.Count -ne 17 -or $sample.bodiesAfter.Count -ne 17){throw 'Expected 17 native collision bodies.'}
            foreach($body in $sample.bodiesBefore) {
                $match=@($sample.bodiesAfter|Where-Object id -eq $body.id)
                if($match.Count -ne 1){throw 'Native body identity changed inside transform call.'}
                # Recover the body's local coordinates from the old orthogonal
                # basis, then transform them by the new pose independently.
                $relative=0..2|ForEach-Object {[double]$body.position[$_]-[double]$sample.before[$_]}
                $local=@(for($axis=0;$axis -lt 3;$axis++) {
                    $value=0.0;for($j=0;$j -lt 3;$j++){$value+=$relative[$j]*[double]$sample.before[3+$axis*3+$j]};$value
                })
                $expected=@(for($j=0;$j -lt 3;$j++) {
                    $value=[double]$sample.after[$j];for($axis=0;$axis -lt 3;$axis++){$value+=$local[$axis]*[double]$sample.after[3+$axis*3+$j]};$value
                })
                $maxError=[Math]::Max($maxError,(Distance $expected $match[0].position));$bodyCount++
            }
        }
        $passed=$moved -ge 5 -and $turned -ge 5 -and $deferred -eq 0 -and $maxError -lt 0.003
        $results+=[pscustomobject]@{SourceRole=$role;TraceRole=$other;Samples=$trace.samples.Count;MovementSamples=$moved;RotationSamples=$turned;
            DeferredSamples=$deferred;BodyObservations=$bodyCount;MaxImmediateBodyTransformError=$maxError;Passed=$passed;Trace=$trace}
        if(!$passed){throw "Native immediate transform check failed for $role."}
    }
}finally {
    foreach($role in 'host','client') {
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_follow off'|Out-Null
    }
    [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Tests=$results;
        Scope='Positions of 17 native physics bodies sampled immediately before and after the native entity transform call, checked against independently calculated translation/rotation. Does not establish later animation, collision behavior, interaction authority, or playable co-op.'}|
        ConvertTo-Json -Depth 12|Set-Content -LiteralPath (Join-Path $PSScriptRoot 'research\native-transform-verification.json')
}
$results|Select-Object SourceRole,TraceRole,Samples,MovementSamples,RotationSamples,DeferredSamples,BodyObservations,MaxImmediateBodyTransformError,Passed
