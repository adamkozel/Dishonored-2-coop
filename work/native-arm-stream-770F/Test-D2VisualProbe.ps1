[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin\d2_runtime_probe.exe'
function Read-State([string]$Role) {
    $root=Join-Path $workspaceRoot "work\lab-instances\$Role"
    $launch=Get-Content -LiteralPath (Join-Path $root 'launch.json') -Raw | ConvertFrom-Json
    $process=Get-Process -Id $launch.pid -ErrorAction Stop
    if($process.Path -ne $launch.executable){throw 'Unexpected game target.'}
    $file=Get-Item -LiteralPath (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale engine status.'}
    $status=Get-Content -LiteralPath $file.FullName -Raw | ConvertFrom-Json
    if(!$status.remoteFresh -or !$status.probeFollowEnabled -or $status.connectionState -ne 3){throw 'Enable both native visual probes before this test.'}
    $json=& $probe $launch.pid --entities
    if($LASTEXITCODE -ne 0){throw 'Native read-only entity inspection failed.'}
    $entities=($json | ConvertFrom-Json).entities
    $expectedHandle='0x'+([uint32]$status.probeActorHandle).ToString('x')
    $actors=@($entities | Where-Object { $_.handle -eq $expectedHandle -and $_.name -match '^d2coop_probe_(corvo|emily)(?:_renamed_\d+)?$' })
    if($actors.Count -ne 1 -or !$actors[0].renderPosition){throw 'Expected one independent native probe with a readable render transform.'}
    [pscustomobject]@{Status=$status;Actor=$actors[0]}
}
function Distance($A,$B) {
    $sum=0.0;for($i=0;$i -lt 3;$i++){$d=[double]$A[$i]-[double]$B[$i];$sum+=$d*$d};[Math]::Sqrt($sum)
}
$results=@()
try {
    foreach($role in 'host','client') {
        $other=if($role -eq 'host'){'client'}else{'host'}
        $before=Read-State $role;$peerBefore=Read-State $other
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -LeftX -16000 -DurationMs 500 | Out-Null
        Start-Sleep -Milliseconds 1200
        $after=Read-State $role;$peerAfter=Read-State $other
        $expected=0..2 | ForEach-Object {[double]$peerAfter.Status.remotePosition[$_]+[double]$peerAfter.Status.probeOffset[$_]}
        $movement=Distance $before.Status.localPosition $after.Status.localPosition
        $renderMovement=Distance $peerBefore.Actor.renderPosition $peerAfter.Actor.renderPosition
        $error=Distance $expected $peerAfter.Actor.renderPosition
        $physicsMovement=Distance $peerBefore.Actor.position $peerAfter.Actor.position
        $actorError=Distance $expected $peerAfter.Actor.position
        $result=[ordered]@{Role=$role;Before=$before;After=$after;PeerBefore=$peerBefore;PeerAfter=$peerAfter;
            LocalMovement=$movement;OtherNativeRenderMovement=$renderMovement;NativeRenderError=$error;OtherActorCachedMovement=$physicsMovement;
            NativeActorError=$actorError;
            Passed=($movement -gt 0.05 -and $renderMovement -gt 0.05 -and $error -lt 0.02 -and $physicsMovement -gt 0.05 -and $actorError -lt 0.02)}
        $results+=[pscustomobject]$result
        if(!$result.Passed){throw "Native actor-follow experiment failed for $role."}
    }
} finally {
    foreach($role in 'host','client'){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100 | Out-Null}
    [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Tests=$results;
        Scope='Independent read-only game memory verifies native cached actor and render transforms follow Steam packets with a lab offset. This checks movement persistence, not playable co-op or collision authority.'} |
        ConvertTo-Json -Depth 8 | Set-Content -LiteralPath (Join-Path $PSScriptRoot 'research\visual-probe-verification.json')
}
$results | Select-Object Role,LocalMovement,OtherNativeRenderMovement,NativeRenderError,OtherActorCachedMovement,Passed
