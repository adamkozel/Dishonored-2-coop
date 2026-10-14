[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$samples=[Collections.Generic.List[object]]::new();$failure=$null;$restoreFailure=$null
function State([string]$role='client'){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json}
function Trace {Get-Content (Join-Path $root 'work/lab-instances/client/coop-status.json.loading.json') -Raw|ConvertFrom-Json}
function Replacement {Get-Content (Join-Path $root 'work/lab-instances/client/coop-status.json.replacement.json') -Raw|ConvertFrom-Json}
function Command([string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role client -Command $text|Out-Null}
function Native([string]$option){
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $script:clientPid $option
    if($LASTEXITCODE){throw "Native read failed: $option"};$raw|ConvertFrom-Json
}
function Health {
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $script:clientPid --health 1
    if($LASTEXITCODE){throw 'Independent player health unavailable'};$raw|ConvertFrom-Json
}
function Load([int]$slot){
    $epoch=(State).localEpoch;Command "LoadGame $slot";$watch=[Diagnostics.Stopwatch]::StartNew();$lastPress=0
    do {
        Start-Sleep -Milliseconds 200;$s=State
        if($s.localEpoch -ne $epoch -and $s.localGameLogicState -eq 1 -and !$s.localPendingGameLogicState){Start-Sleep -Milliseconds 600;return}
        if($watch.ElapsedMilliseconds -gt 6000 -and $watch.ElapsedMilliseconds-$lastPress -gt 600 -and $s.localGameLogicState -in 8,15){
            & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -Button A -DurationMs 150|Out-Null;$lastPress=$watch.ElapsedMilliseconds
        }
    }while($watch.ElapsedMilliseconds -lt 25000)
    throw 'Native checkpoint load did not complete.'
}
function Distance($a,$b){$sum=0.0;for($i=0;$i -lt 3;$i++){$d=[double]$a[$i]-[double]$b[$i];$sum+=$d*$d};[Math]::Sqrt($sum)}
function NpcIdentity($snapshot){($snapshot.npcs|ForEach-Object {"$($_.spawnerName)/$($_.definition)"}|Sort-Object) -join ';'}
try {
    $script:clientPid=(Get-Content (Join-Path $root 'work/lab-instances/client/launch.json') -Raw|ConvertFrom-Json).pid
    $source=Join-Path $root 'work/lab-instances/host/base/savegame/SLOT81/checkpoint.dat'
    $copy=Join-Path $root 'work/lab-instances/client/base/savegame/SLOT82/checkpoint.dat'
    $sourceHash=(Get-FileHash $source).Hash
    if((Get-FileHash $copy).Hash -ne $sourceHash){throw 'Client slot 82 must contain the unchanged Corvo checkpoint; restart after staging it.'}
    $hostBefore=State host
    Load 82
    $baseline=[pscustomobject]@{Character=(Native --character);Player=(Native --player);Npcs=(Native --world-npcs);Health=(Health)}
    if($baseline.Character.character -ne 1){throw 'Unmodified checkpoint did not load Corvo.'}
    Load 81
    $before=Trace;Command 'coop_loading_probe emily';Start-Sleep -Milliseconds 350
    if(!(Trace).probe.armed){throw 'Role restoration probe did not arm.'}
    Command 'LoadGame 82';$watch=[Diagnostics.Stopwatch]::StartNew();$lastPress=0
    do {
        Start-Sleep -Milliseconds 100;$trace=Trace;$state=State
        if($trace.probe.held) {
            $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $script:clientPid --loading-world $trace.probe.game
            if($LASTEXITCODE){throw 'Independent loaded-world character read failed.'}
            $native=$raw|ConvertFrom-Json
            $sample=[pscustomobject]@{At=$watch.ElapsedMilliseconds;Native=$native;Hold=$trace.probe;Restore=(Replacement).restore;Host=(State host)}
            $samples.Add($sample)
            if($native.tick -lt $trace.probe.startedAt+2000 -and ($native.logic -ne 15 -or $native.pending -or $native.character -ne 2 -or $native.queued -or $native.definition -ne 'models/characters/player/emily/body.def')){throw 'Emily was not fully restored before gameplay release.'}
        } elseif($watch.ElapsedMilliseconds -gt 6000 -and $watch.ElapsedMilliseconds-$lastPress -gt 600 -and $state.localGameLogicState -in 8,15 -and $trace.probe.released -eq $before.probe.released) {
            & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -Button A -DurationMs 150|Out-Null;$lastPress=$watch.ElapsedMilliseconds
        }
    }while($watch.ElapsedMilliseconds -lt 25000 -and !($trace.probe.released -gt $before.probe.released -and $state.localGameLogicState -eq 1 -and !$state.localPendingGameLogicState))
    $replacement=Replacement;$r=$replacement.restore
    $held=@($samples|Where-Object {$_.Native.tick -lt $_.Hold.startedAt+2000})
    if($held.Count -lt 3 -or !$r.completed -or $r.beforeCharacter -ne 1 -or $r.afterCharacter -ne 2 -or $r.beforeLogic -ne 15 -or $r.afterLogic -ne 15 -or $r.queueAfter -or $r.completedAt -gt $trace.probe.startedAt){throw 'Native role replacement did not finish before the loading barrier.'}
    if(!$r.healthRestored-or$r.checkpointHealth-ne$baseline.Health.current-or$r.restoredHealth-ne$baseline.Health.current){throw 'Checkpoint health was not restored before the loading barrier.'}
    if($trace.probe.started -ne $before.probe.started+1 -or $trace.probe.released -ne $before.probe.released+1 -or $trace.probe.maxDistance -gt 0.001){throw 'Expected one stationary loading barrier and release.'}
    Start-Sleep -Milliseconds 700
    $after=[pscustomobject]@{Character=(Native --character);Player=(Native --player);Npcs=(Native --world-npcs);Health=(Health)}
    if($after.Health.current-ne$baseline.Health.current-or$after.Health.maximum-ne$baseline.Health.maximum){throw 'Character replacement changed checkpoint health.'}
    if($after.Character.character -ne 2 -or $after.Character.queued -or (Distance $baseline.Player.candidatePosition $after.Player.candidatePosition) -gt 0.01){throw 'Final role or checkpoint position mismatch.'}
    if((NpcIdentity $baseline.Npcs) -ne (NpcIdentity $after.Npcs)){throw 'Authored NPC identities changed during pre-gameplay restoration.'}
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -LeftY 16000 -DurationMs 650|Out-Null
    Start-Sleep -Milliseconds 900
    $moved=Native --player;$movement=Distance $after.Player.candidatePosition $moved.candidatePosition
    if($movement -lt 0.15){throw 'Restored Emily did not respond to native input.'}
    $hostAfter=State host
    if($hostAfter.localEpoch -ne $hostBefore.localEpoch -or $hostAfter.localGameLogicState -ne 1){throw 'Host world changed during client-only experiment.'}
    if((Get-FileHash $copy).Hash -ne $sourceHash -or (Get-FileHash $source).Hash -ne $sourceHash){throw 'Checkpoint source bytes changed.'}
}catch{$failure=$_.Exception.Message}finally {
    try {
        # A failed assertion can occur inside the two-second hold. Let that
        # load finish before issuing another one, so its epoch is unambiguous.
        $settle=[Diagnostics.Stopwatch]::StartNew()
        while((Trace).probe.held -and $settle.ElapsedMilliseconds -lt 3500){Start-Sleep -Milliseconds 200}
        Load 81;$restored=Native --character;if($restored.character -ne 2){throw 'Original Emily fixture was not restored.'}
    }catch{$restoreFailure=$_.Exception.Message}
}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure -and !$restoreFailure);Failure=$failure;RestoreFailure=$restoreFailure;
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;SourceCheckpointSha256=$sourceHash;
    Baseline=$baseline;Samples=$samples;Replacement=$replacement;FinalTrace=$trace;After=$after;MovementDistance=$movement;Restored=$restored;
    Scope='Client loads unchanged host checkpoint. At the fully loaded native boundary, the character-choice API and native replacement processor restore Emily inside the native scoped-game context. Checks role/body before gameplay, a two-second hold, position, NPC identities and movement. No shared reset, death propagation, quest/inventory/power compatibility or network both-ready transaction.'}|
    ConvertTo-Json -Depth 16|Set-Content (Join-Path $PSScriptRoot 'research/native-loading-character-verification.json')
if($failure){throw $failure};if($restoreFailure){throw $restoreFailure}
[pscustomobject]@{Passed=$true;HeldSamples=$held.Count;MovementDistance=$movement;RestoredBeforeGameplay=$r.completed}
