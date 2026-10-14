[CmdletBinding()]
param([ValidateSet('host','client')][string]$DisconnectRole='client',
      [ValidateSet('Held','Loading')][string]$Stage='Held')
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$survivor=if($DisconnectRole -eq 'client'){'host'}else{'client'}
$failure=$null;$paused=$null;$held=$null;$nativeBefore=$null;$nativeAfter=$null;$interrupted=$null
function State([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json}
function Reset([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json.reset.json") -Raw|ConvertFrom-Json}
function Command([string]$role,[string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null}
$launch=@{};foreach($role in 'host','client'){$launch[$role]=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json}
function Native {
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch[$survivor].pid --loading-world $script:held.game
    if($LASTEXITCODE){throw 'Surviving game native read failed.'};$raw|ConvertFrom-Json
}
try {
    foreach($role in 'host','client') {
        if((State $role).localGameLogicState -ne 1){throw 'Both games must start in gameplay.'}
        if(!(Reset $role).enabled){Command $role 'coop_checkpoint_sync on';Start-Sleep -Milliseconds 250}
        Command $role $(if($role -eq $DisconnectRole){'coop_checkpoint_delay 10000'}else{'coop_checkpoint_delay 0'})
    }
    Start-Sleep -Milliseconds 700
    $before=Reset $survivor;Command $survivor 'RestartFromCheckpoint'
    $watch=[Diagnostics.Stopwatch]::StartNew();$lastPress=@{host=0;client=0}
    do {
        Start-Sleep -Milliseconds 100;$both=$true
        foreach($role in 'host','client') {
            $r=Reset $role;$s=State $role
            if($r.phase -eq 7){throw "Early failure: $role $($r.result)"}
            if(!$r.held){$both=$false}
            if(!$r.held -and $watch.ElapsedMilliseconds -gt 6000 -and $s.localGameLogicState -in 8,15 -and $watch.ElapsedMilliseconds-$lastPress[$role] -gt 700) {
                & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button A -DurationMs 100|Out-Null;$lastPress[$role]=$watch.ElapsedMilliseconds
            }
        }
        $early=Reset $survivor
        $inLoading=$early.phase -eq 4 -and $early.expectBoundary -and !$early.held -and $early.loads -eq $before.loads+1
        if($Stage -eq 'Loading' -and $inLoading){break}
    }while(!$both -and $watch.ElapsedMilliseconds -lt 30000)
    if($Stage -eq 'Held') {
        if(!$both){throw 'Both games did not enter their native loading holds.'}
        $script:held=Reset $survivor;$nativeBefore=Native
    }elseif(!$inLoading){throw 'Could not observe the asynchronous load before its both-ready hold.'}
    $interrupted=[pscustomobject]@{Reset=(Reset $survivor);Bridge=(State $survivor)}
    $victim=Get-Process -Id $launch[$DisconnectRole].pid -ErrorAction Stop
    $expected=(Resolve-Path (Join-Path $root 'work/lab-game/Dishonored2.exe')).Path
    if($victim.Path -ne $expected){throw 'Refusing to stop a non-lab process.'}
    Stop-Process -Id $victim.Id -Force
    $watch.Restart();$lastContinue=0
    do {
        Start-Sleep -Milliseconds 150;$r=Reset $survivor;$s=State $survivor
        $alive=Get-Process -Id $launch[$survivor].pid -ErrorAction SilentlyContinue
        if(!$alive){throw 'Surviving game exited during failed-load recovery.'}
        if($Stage -eq 'Loading' -and !$r.held -and $watch.ElapsedMilliseconds -gt 6000 -and
           $s.localGameLogicState -in 8,15 -and $watch.ElapsedMilliseconds-$lastContinue -gt 700) {
            & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $survivor -Button A -DurationMs 100|Out-Null
            $lastContinue=$watch.ElapsedMilliseconds
        }
        $done=$r.phase -eq 7 -and !$r.held -and $s.localGameLogicState -eq 2 -and !$s.localPendingGameLogicState
    }while(!$done -and $watch.ElapsedMilliseconds -lt 18000)
    if(!$done){throw 'Disconnect did not resolve to the native pause screen.'}
    if($Stage -eq 'Loading'){$script:held=$r;$nativeBefore=Native}
    $paused=[pscustomobject]@{Reset=$r;Bridge=$s;Native=(Native)}
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $survivor -LeftY 16000 -DurationMs 500|Out-Null
    Start-Sleep -Milliseconds 800;$nativeAfter=Native
    $distance=0.0;for($i=0;$i -lt 3;$i++){$delta=$nativeAfter.position[$i]-$nativeBefore.position[$i];$distance+=$delta*$delta}
    if($nativeAfter.logic -ne 2 -or $nativeAfter.pending -or $nativeAfter.character -ne $(if($survivor -eq 'host'){1}else{2}) -or
       [Math]::Sqrt($distance) -gt 0.001 -or $r.completed -ne $before.completed -or $r.loads -ne $before.loads+1){throw 'Failed reset resumed, moved, lost its role, or claimed completion.'}
}catch{$failure=$_.Exception.Message}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;DisconnectRole=$DisconnectRole;Survivor=$survivor;Stage=$Stage;Interrupted=$interrupted;
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;Before=$before;Held=$held;NativeBefore=$nativeBefore;Paused=$paused;NativeAfter=$nativeAfter;
    Scope='Terminate one isolated lab peer during a native load or both-ready hold. Survivor must finish into native pause with its assigned role and no completed shared reset. Held case compares position across interruption; loading case verifies no movement after paused completion.'}|
    ConvertTo-Json -Depth 14|Set-Content (Join-Path $PSScriptRoot "research/shared-checkpoint-disconnect-$DisconnectRole$(if($Stage -eq 'Loading'){'-loading'})-verification.json")
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Disconnected=$DisconnectRole;Survivor=$survivor;NativeLogic=$nativeAfter.logic;Displacement=[Math]::Sqrt($distance)}
