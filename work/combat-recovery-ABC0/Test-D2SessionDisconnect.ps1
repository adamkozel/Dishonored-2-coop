[CmdletBinding()]
param([ValidateSet('host','client')][string]$DisconnectRole='client',
      [ValidateSet('Gameplay','Death')][string]$Stage='Gameplay')
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$survivor=if($DisconnectRole -eq 'client'){'host'}else{'client'}
$failure=$null;$before=$null;$paused=$null;$after=$null;$nativeBefore=$null;$nativeAfter=$null
function State([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json}
function Reset([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json.reset.json") -Raw|ConvertFrom-Json}
function Command([string]$role,[string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null}
$launch=@{};foreach($role in 'host','client'){$launch[$role]=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json}
function Native([string]$option) {
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch[$survivor].pid $option
    if($LASTEXITCODE){throw "Surviving game native read failed: $option"};$raw|ConvertFrom-Json
}
try {
    foreach($role in 'host','client') {
        if((State $role).localGameLogicState -ne 1){throw 'Both games must start in gameplay.'}
        if(!(Reset $role).enabled){Command $role 'coop_checkpoint_sync on';Start-Sleep -Milliseconds 300}
    }
    Start-Sleep -Milliseconds 1000
    foreach($role in 'host','client'){if(!(Reset $role).peerArm){throw 'Shared session did not arm.'}}
    if($Stage -eq 'Death') {
        Command $DisconnectRole 'coop_gameover_probe'
        $watch=[Diagnostics.Stopwatch]::StartNew()
        do {
            Start-Sleep -Milliseconds 100;$ready=$true
            foreach($role in 'host','client'){$r=Reset $role;if((State $role).localGameLogicState -ne 13 -or !$r.deathReady -or !$r.peerDeathReady){$ready=$false}}
        }while(!$ready -and $watch.ElapsedMilliseconds -lt 10000)
        if(!$ready){throw 'Both native death screens did not become ready.'}
    }
    $before=[pscustomobject]@{Bridge=(State $survivor);Reset=(Reset $survivor)}
    $victim=Get-Process -Id $launch[$DisconnectRole].pid -ErrorAction Stop
    $expected=(Resolve-Path (Join-Path $root 'work/lab-game/Dishonored2.exe')).Path
    if($victim.Path -ne $expected){throw 'Refusing to stop a non-lab process.'}
    Stop-Process -Id $victim.Id -Force
    $expectedLogic=if($Stage -eq 'Death'){13}else{2}
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do {
        Start-Sleep -Milliseconds 150;$r=Reset $survivor;$s=State $survivor
        if(!(Get-Process -Id $launch[$survivor].pid -ErrorAction SilentlyContinue)){throw 'Survivor exited.'}
        $done=$r.phase -eq 7 -and $s.localGameLogicState -eq $expectedLogic -and !$s.localPendingGameLogicState
    }while(!$done -and $watch.ElapsedMilliseconds -lt 15000)
    if(!$done){throw 'Lost session did not stop in the expected native screen.'}
    $nativeBefore=Native '--player'
    $paused=[pscustomobject]@{Bridge=$s;Reset=$r;GameOver=(Native '--gameover');Character=(Native '--character')}
    if($paused.GameOver.logic -ne $expectedLogic -or $paused.GameOver.pending){throw 'Independent native state disagrees.'}
    Command $survivor 'RestartFromCheckpoint'
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $survivor -LeftY 16000 -DurationMs 500|Out-Null
    Start-Sleep -Milliseconds 850;$nativeAfter=Native '--player';$after=[pscustomobject]@{Reset=(Reset $survivor);Bridge=(State $survivor)}
    $distance=0.0;for($i=0;$i -lt 3;$i++){$d=$nativeAfter.candidatePosition[$i]-$nativeBefore.candidatePosition[$i];$distance+=$d*$d}
    if([Math]::Sqrt($distance) -gt 0.001 -or $after.Bridge.localGameLogicState -ne $expectedLogic -or
       $after.Reset.loads -ne $before.Reset.loads -or $after.Reset.completed -ne $before.Reset.completed -or
       $paused.Character.character -ne $(if($survivor -eq 'host'){1}else{2})){throw 'Failed session moved, loaded, lost its role, or claimed completion.'}
}catch{$failure=$_.Exception.Message}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;DisconnectRole=$DisconnectRole;Survivor=$survivor;Stage=$Stage;
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;Before=$before;Paused=$paused;After=$after;NativeBefore=$nativeBefore;NativeAfter=$nativeAfter;
    Scope='Terminate one lab peer in an armed session. Gameplay stops in native pause; an existing shared GameOver stays in native GameOver. Movement and local retry cannot resume the failed shared attempt.'}|
    ConvertTo-Json -Depth 14|Set-Content (Join-Path $PSScriptRoot "research/session-disconnect-$DisconnectRole-$($Stage.ToLower())-verification.json")
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Disconnected=$DisconnectRole;Stage=$Stage;Survivor=$survivor;NativeLogic=$expectedLogic;Displacement=[Math]::Sqrt($distance)}
