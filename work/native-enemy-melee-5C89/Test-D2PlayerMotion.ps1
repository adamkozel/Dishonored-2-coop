[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe'
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$retries=0;$launch=@{}
function State([string]$role){
    $file=Get-Item (Join-Path $root "work/lab-instances/$role/coop-status.json")
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale status.'}
    Get-Content $file.FullName -Raw|ConvertFrom-Json
}
function Input([string]$role,[string]$button){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button $button -DurationMs 400|Out-Null}
function Native([string]$role){
    foreach($attempt in 1..6){
        $raw=& $probe $launch[$role].pid --player-states
        if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)}
        if($LASTEXITCODE -ne 52){throw 'Native probe process failed.'}
        ++$script:retries;Start-Sleep -Milliseconds 25
    }
    throw 'Native active states unavailable after bounded retries.'
}
function CheckGround([string]$role,[int]$kind){
    $other=if($role -eq 'host'){'client'}else{'host'}
    $n=Native $role;$a=State $role;$b=State $other
    if(!$n.defaultMode -or !$n.motionValid -or $n.motion -ne $kind -or $n.stance -ne $kind -or
       !$a.localPlayerMotionValid -or $a.localPlayerMotion -ne $kind -or !$b.remotePlayerMotionValid -or $b.remotePlayerMotion -ne $kind -or
       !$b.remoteFresh -or $a.poseProtocol -ne 10 -or $b.poseProtocol -ne 10 -or $a.posePacketBytes -ne 1156 -or
       !$b.localPlayerMotionValid -or $b.localPlayerMotion -ne 0){throw 'Native stable motion state did not match both Steam endpoints.'}
    [ordered]@{Native=$n;Source=$a;Peer=$b}
}
foreach($role in 'host','client'){
    $launch[$role]=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch[$role].pid).Path -ne $launch[$role].executable){throw 'Unexpected game process.'}
}
try {
    foreach($role in 'host','client') {
        $other=if($role -eq 'host'){'client'}else{'host'}
        foreach($posture in 0,1) {
            if($posture){Input $role B;Start-Sleep -Milliseconds 900}
            $before=CheckGround $role $posture;$otherBefore=Native $other
            Input $role A;$watch=[Diagnostics.Stopwatch]::StartNew();$nativeSamples=[Collections.Generic.List[object]]::new()
            do {$nativeSamples.Add((Native $role));Start-Sleep -Milliseconds 25}while($watch.ElapsedMilliseconds -lt 2000)
            Start-Sleep -Milliseconds 600;$after=CheckGround $role $posture;$otherAfter=Native $other
            $observed=@($nativeSamples|Where-Object motionValid|Select-Object -ExpandProperty motion|Sort-Object -Unique)
            $height=($nativeSamples|ForEach-Object {$_.position[2]}|Measure-Object -Maximum).Maximum-$before.Native.position[2]
            $peerJump=$after.Peer.receivedPlayerMotions[2]-$before.Peer.receivedPlayerMotions[2]
            $peerFall=$after.Peer.receivedPlayerMotions[3]-$before.Peer.receivedPlayerMotions[3]
            $otherDistance=0.0;for($i=0;$i -lt 3;++$i){$d=$otherAfter.position[$i]-$otherBefore.position[$i];$otherDistance+=$d*$d}
            if(2 -notin $observed -or 3 -notin $observed -or $peerJump -lt 1 -or $peerFall -lt 1 -or
               $height -lt 0.5 -or [Math]::Abs($after.Native.position[2]-$before.Native.position[2]) -gt 0.02 -or
               [Math]::Sqrt($otherDistance) -gt 0.001 -or $after.Peer.rejected -ne $before.Peer.rejected){throw 'Jump/fall/landing transfer failed or moved the other local player.'}
            $rows.Add([ordered]@{Role=$role;Posture=$posture;Before=$before;After=$after;NativeSamples=$nativeSamples.ToArray();
                JumpHeight=$height;ReceivedJumpPackets=$peerJump;ReceivedFallPackets=$peerFall;OtherLocalDisplacement=[Math]::Sqrt($otherDistance)})
        }
        Input $role B;Start-Sleep -Milliseconds 800
        Input $role Start;Start-Sleep -Milliseconds 900
        $a=State $role;$b=State $other;$raw=& $probe $launch[$role].pid --player-states;$refused=$LASTEXITCODE -eq 52
        if($a.localGameLogicState -ne 2 -or $a.localPlayerMotionValid -or $a.localPlayerMotion -or
           $b.remotePlayerMotionValid -or $b.remotePlayerMotion -or !$b.remoteFresh -or !$refused){throw 'Native pause did not invalidate airborne metadata.'}
        $rows.Add([ordered]@{Role=$role;Stage='Pause';NativeRefused=$refused;Source=$a;Peer=$b})
        Input $role Start;Start-Sleep -Milliseconds 900;CheckGround $role 0|Out-Null
    }
}catch{$failure=$_.Exception.Message}finally{
    foreach($role in 'host','client'){
        Input $role Neutral
        if((State $role).localGameLogicState -eq 2){Input $role Start;Start-Sleep -Milliseconds 900}
        if((State $role).localGameLogicState -eq 1 -and (Native $role).stance -eq 1){Input $role B;Start-Sleep -Milliseconds 800}
    }
    foreach($role in 'host','client'){
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_follow on'|Out-Null
        Start-Sleep -Milliseconds 300
    }
}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;ProbeSha256=(Get-FileHash $probe).Hash;
    NativeReadRetries=$retries;Cases=$rows.ToArray();
    Scope='Default-mode native motion metadata over Steam: ground, sneaking, jumping, falling, landing and pause invalidation. Received-packet counters retain brief states between status refreshes; source actions independently sampled from native state machine. This test verifies metadata; body animation has separate tests.'}|
    ConvertTo-Json -Depth 15|Set-Content (Join-Path $PSScriptRoot 'research/player-motion-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;JumpCases=@($rows|Where-Object { $_.Contains('JumpHeight') }).Count;PauseCases=2;NativeReadRetries=$retries;RemoteAnimationVerifiedByThisTest=$false}


