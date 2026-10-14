[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe'
$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
$nativeReadRetries=0
function State([string]$role){
    $file=Get-Item (Join-Path $root "work/lab-instances/$role/coop-status.json")
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale lab status.'}
    Get-Content $file.FullName -Raw|ConvertFrom-Json
}
function Input([string]$role,[string]$button){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button $button -DurationMs 400|Out-Null}
$launch=@{};foreach($role in 'host','client'){
    $launch[$role]=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch[$role].pid).Path -ne $launch[$role].executable){throw 'Unexpected lab process.'}
}
function Native([string]$role){
    foreach($attempt in 1..6){
        $raw=& $probe $launch[$role].pid --player-stance
        if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)}
        if($LASTEXITCODE -ne 50){throw 'Native stance probe process failed.'}
        ++$script:nativeReadRetries;Start-Sleep -Milliseconds 25
    }
    throw 'Native stance snapshot unavailable after bounded read retries.'
}
function Verify([string]$role,[string]$stage,[int]$expected){
    $other=if($role -eq 'host'){'client'}else{'host'}
    foreach($iteration in 1..5){
        $native=Native $role;$a=State $role;$b=State $other;$otherNative=Native $other
        if($native.current -ne $expected -or !$a.localPlayerStanceValid -or $a.localPlayerStance -ne $expected -or
           !$b.remotePlayerStanceValid -or $b.remotePlayerStance -ne $expected -or !$b.remoteFresh -or
           $a.poseProtocol -notin @(8,9,10,11) -or $b.poseProtocol -ne $a.poseProtocol -or $a.posePacketBytes -ne $(if($a.poseProtocol -eq 11){1164}elseif($a.poseProtocol -eq 10){1156}elseif($a.poseProtocol -eq 9){192}else{184}) -or
           $b.localPlayerStance -ne 0 -or !$b.localPlayerStanceValid -or $otherNative.current -ne 0){throw "Stance transfer failed: $role $stage"}
        $samples.Add([ordered]@{Role=$role;Stage=$stage;Native=$native;Local=$a;Peer=$b;OtherNative=$otherNative})
        Start-Sleep -Milliseconds 125
    }
}
try {
    foreach($role in 'host','client') {
        if((Native $role).current -ne 0){throw 'Both players must begin standing.'}
        Verify $role Standing 0
        Input $role B;Start-Sleep -Milliseconds 900;Verify $role Crouched 1
        Input $role Start;Start-Sleep -Milliseconds 900
        $a=State $role;$other=if($role -eq 'host'){'client'}else{'host'};$b=State $other
        $raw=& $probe $launch[$role].pid --player-stance;$nativeRefused=$LASTEXITCODE -eq 50
        if($a.localGameLogicState -ne 2 -or $a.localPlayerStanceValid -or $a.localPlayerStance -or
           $b.remotePlayerStanceValid -or $b.remotePlayerStance -or !$b.remoteFresh -or !$nativeRefused){throw 'Paused player exposed usable stance metadata.'}
        $samples.Add([ordered]@{Role=$role;Stage='Paused';Local=$a;Peer=$b;NativeReadRefused=$nativeRefused})
        Input $role Start;Start-Sleep -Milliseconds 900;Verify $role ResumedCrouched 1
        Input $role B;Start-Sleep -Milliseconds 900;Verify $role StandingAgain 0
        $before=Native $role;Input $role A;$jump=[Collections.Generic.List[object]]::new()
        foreach($iteration in 1..15){$n=Native $role;$jump.Add($n);Start-Sleep -Milliseconds 65}
        Start-Sleep -Milliseconds 900;Verify $role Landed 0
        $height=($jump|ForEach-Object {$_.position[2]}|Measure-Object -Maximum).Maximum-$before.position[2]
        if($height -lt 0.5 -or @($jump|Where-Object current -ne 0).Count){throw 'Jump did not verify the distinct physical stance semantics.'}
        $samples.Add([ordered]@{Role=$role;Stage='Jump';Height=$height;Native=$jump.ToArray()})
        $checks.Add("$role native standing/crouching matches peer packet, leaves other player standing, clears in native pause, returns after resume; jump remains a separate signal")
    }
}catch{$failure=$_.Exception.Message}finally{
    foreach($role in 'host','client'){
        Input $role Neutral
        $s=State $role
        if($s.localGameLogicState -eq 2){Input $role Start;Start-Sleep -Milliseconds 800}
        if((State $role).localGameLogicState -eq 1){
            if((Native $role).current -eq 1){Input $role B;Start-Sleep -Milliseconds 700}
        }
    }
    foreach($role in 'host','client'){
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_follow on'|Out-Null
        Start-Sleep -Milliseconds 300
    }
}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;ProbeSha256=(Get-FileHash $probe).Hash;
    Checks=$checks.ToArray();Samples=$samples.ToArray();NativeReadRetries=$nativeReadRetries;
    Scope='Native Steam transfer of actual player physics standing/crouch metadata, gated by gameplay and lifetime checks. Does not apply remote crouch animation or replicate jump/combat behavior.'}|
    ConvertTo-Json -Depth 15|Set-Content (Join-Path $PSScriptRoot 'research/player-stance-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$samples.Count;RemoteAnimationImplemented=$false}

