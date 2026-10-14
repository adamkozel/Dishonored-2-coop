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
    if(!$status.remoteFresh -or $status.connectionState -ne 3){throw 'Steam peer unavailable.'}
    $json=& $probe $launch.pid --entities
    if($LASTEXITCODE -ne 0){throw 'Entity inspection failed.'}
    $entities=($json | ConvertFrom-Json).entities
    $expectedHandle='0x'+([uint32]$status.probeActorHandle).ToString('x')
    $actors=@($entities | Where-Object { $_.handle -eq $expectedHandle -and $_.name -match '^d2coop_probe_(corvo|emily)(?:_renamed_\d+)?$' })
    if($actors.Count -ne 1 -or !$actors[0].physicsBodies){throw 'Expected one clone with native physics bodies.'}
    $player=& $probe $launch.pid --player
    if($LASTEXITCODE -ne 0){throw 'Player inspection failed.'}
    [pscustomobject]@{Status=$status;Actor=$actors[0];Player=($player|ConvertFrom-Json);
        Story=@($entities|Where-Object { $_.definitionName -match '/story/(corvo_attano|emily_kaldwin)\.def$' -and $_.name -notmatch '^d2coop_probe_' })}
}
function Distance($A,$B,[int]$Count=3) {
    $sum=0.0;for($i=0;$i -lt $Count;$i++){$d=[double]$A[$i]-[double]$B[$i];$sum+=$d*$d};[Math]::Sqrt($sum)
}
$results=@()
try {
    foreach($role in 'host','client'){
        $other=if($role -eq 'host'){'client'}else{'host'}
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $other -Command 'coop_probe_follow on' | Out-Null
        Start-Sleep -Milliseconds 400
        $idleBefore=Read-State $other
        Start-Sleep -Milliseconds 1200
        $before=Read-State $role;$peerBefore=Read-State $other
        # Native idle animation moves the individual skeleton bodies even when
        # the entity root is stationary. Measure that separately before input.
        $idleDelta=0..2|ForEach-Object {[double]$peerBefore.Actor.position[$_]-[double]$idleBefore.Actor.position[$_]}
        $idleErrors=@(foreach($body in $idleBefore.Actor.physicsBodies){
            $match=@($peerBefore.Actor.physicsBodies|Where-Object id -eq $body.id)
            if($match.Count -ne 1){throw 'Physics body identity changed while stationary.'}
            $expected=0..2|ForEach-Object {[double]$body.position[$_]+$idleDelta[$_]}
            Distance $expected $match[0].position
        })
        $maxIdleBodyMotion=($idleErrors|Measure-Object -Maximum).Maximum
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -LeftX 16000 -DurationMs 500 | Out-Null
        Start-Sleep -Milliseconds 1200
        $after=Read-State $role;$peerAfter=Read-State $other
        $delta=0..2|ForEach-Object {[double]$peerAfter.Actor.position[$_]-[double]$peerBefore.Actor.position[$_]}
        $bodyErrors=@(foreach($body in $peerBefore.Actor.physicsBodies){
            $match=@($peerAfter.Actor.physicsBodies|Where-Object id -eq $body.id)
            if($match.Count -ne 1){throw 'Physics body identity changed.'}
            $expected=0..2|ForEach-Object {[double]$body.position[$_]+$delta[$_]}
            Distance $expected $match[0].position
        })
        $maxBodyError=($bodyErrors|Measure-Object -Maximum).Maximum
        $movement=Distance $before.Player.candidatePosition $after.Player.candidatePosition
        # The allowance comes from the measured idle motion, with a hard cap
        # of one quarter of the translated distance. A body left behind fails.
        $bodyErrorBudget=[Math]::Min($movement*0.25,[Math]::Max(0.03,$maxIdleBodyMotion*2+0.01))
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -RightX 15000 -DurationMs 400 | Out-Null
        Start-Sleep -Milliseconds 1200
        $turned=Read-State $role;$peerTurned=Read-State $other
        $turn=Distance $after.Player.candidateOrientation $turned.Player.candidateOrientation 9
        $actorRotationError=Distance $turned.Player.candidateOrientation $peerTurned.Actor.orientation 9
        $renderRotationError=Distance $turned.Player.candidateOrientation $peerTurned.Actor.renderOrientation 9
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $other -Command 'coop_probe_follow off' | Out-Null
        Start-Sleep -Milliseconds 400
        $stopped=Read-State $other
        Start-Sleep -Milliseconds 2200
        $persisted=Read-State $other
        $drift=Distance $stopped.Actor.position $persisted.Actor.position
        $renderDrift=Distance $stopped.Actor.renderPosition $persisted.Actor.renderPosition
        $storyDrift=0.0
        foreach($story in $peerBefore.Story){
            $match=@($persisted.Story|Where-Object handle -eq $story.handle)
            if($match.Count -ne 1){throw 'Original story actor identity changed.'}
            $storyDrift=[Math]::Max($storyDrift,(Distance $story.position $match[0].position))
        }
        $result=[ordered]@{Role=$role;LocalMovement=$movement;PhysicsBodyCount=$bodyErrors.Count;MaxBodyDeltaError=$maxBodyError;
            MaxStationaryBodyMotion=$maxIdleBodyMotion;BodyErrorBudget=$bodyErrorBudget;IdleBefore=$idleBefore;
            LocalTurn=$turn;ActorRotationError=$actorRotationError;RenderRotationError=$renderRotationError;
            PersistenceSeconds=2.2;ActorDrift=$drift;RenderDrift=$renderDrift;OriginalStoryActorDrift=$storyDrift;
            PeerBefore=$peerBefore;PeerAfter=$peerAfter;PeerTurned=$peerTurned;Stopped=$stopped;Persisted=$persisted;
            Passed=($movement -gt 0.05 -and $maxBodyError -lt $bodyErrorBudget -and $turn -gt 0.05 -and $actorRotationError -lt 0.01 -and
                $renderRotationError -lt 0.01 -and $drift -lt 0.02 -and $renderDrift -lt 0.02 -and $storyDrift -lt 0.02)}
        $results+=[pscustomobject]$result
        if(!$result.Passed){throw "Native actor physics/rotation/persistence check failed for $role."}
    }
}finally{
    foreach($role in 'host','client'){
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100 | Out-Null
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_follow off' | Out-Null
    }
    [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Tests=$results;
        Scope='Native clone translation, Havok body translation, yaw rotation, persistence with follow disabled, and original story actor preservation. Does not establish animation, shared world, interaction authority, or playable co-op.'}|
        ConvertTo-Json -Depth 10|Set-Content -LiteralPath (Join-Path $PSScriptRoot 'research\actor-pose-verification.json')
}
$results|Select-Object Role,LocalMovement,PhysicsBodyCount,MaxBodyDeltaError,LocalTurn,ActorRotationError,ActorDrift,OriginalStoryActorDrift,Passed
