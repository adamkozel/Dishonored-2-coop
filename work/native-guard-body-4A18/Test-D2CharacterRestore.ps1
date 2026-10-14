[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$restoreFailure=$null
function State { Get-Content (Join-Path $root 'work/lab-instances/client/coop-status.json') -Raw|ConvertFrom-Json }
function Command([string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role client -Command $text|Out-Null}
function Native([string]$option) {
    $launch=Get-Content (Join-Path $root 'work/lab-instances/client/launch.json') -Raw|ConvertFrom-Json
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch.pid $option
    if($LASTEXITCODE){throw "Native read failed: $option, code $LASTEXITCODE"}
    $raw|ConvertFrom-Json
}
function Snapshot([string]$stage) {
    $r=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');State=(State);Character=(Native --character);Player=(Native --player);Npcs=(Native --world-npcs)}
    $rows.Add($r);$r
}
function Load([int]$slot) {
    $epoch=(State).localEpoch;Command "LoadGame $slot";$watch=[Diagnostics.Stopwatch]::StartNew()
    do {
        Start-Sleep -Milliseconds 200;$s=State
        if($s.localEpoch -ne $epoch -and $s.localGameLogicState -eq 1 -and !$s.localPendingGameLogicState){return}
        if($watch.ElapsedMilliseconds -gt 6000 -and $s.localGameLogicState -in 8,15){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -Button A -DurationMs 200|Out-Null}
    }while($watch.ElapsedMilliseconds -lt 25000)
    throw 'Native load did not complete.'
}
function Distance($a,$b){$sum=0.0;for($i=0;$i -lt 3;$i++){$sum+=([double]$a[$i]-[double]$b[$i])*([double]$a[$i]-[double]$b[$i])};[Math]::Sqrt($sum)}
try {
    $source=Join-Path $root 'work/lab-instances/host/base/savegame/SLOT81/checkpoint.dat'
    $copy=Join-Path $root 'work/lab-instances/client/base/savegame/SLOT82/checkpoint.dat'
    $sourceHash=(Get-FileHash $source).Hash
    if((Get-FileHash $copy).Hash -ne $sourceHash){throw 'Client slot 82 must contain the unchanged host checkpoint.'}
    Load 82;Start-Sleep -Milliseconds 750
    $before=Snapshot CorvoCheckpointLoaded
    if($before.Character.character -ne 1 -or $before.Character.definition -ne 'models/characters/player/corvo/body.def'){throw 'Expected actual Corvo from the host save.'}
    Command 'coop_character_probe emily';Start-Sleep -Milliseconds 1200
    $after=Snapshot NativeEmilyRequested
    if($after.Character.character -ne 2 -or $after.Character.queued -or $after.Character.definition -ne 'models/characters/player/emily/body.def'){throw 'Native character conversion did not complete.'}
    if($before.State.localMapId -ne $after.State.localMapId -or (Distance $before.Player.candidatePosition $after.Player.candidatePosition) -gt 0.2){throw 'Conversion changed map or player position.'}
    $beforeIdentity=($before.Npcs.npcs|ForEach-Object {"$($_.handle)/$($_.spawnerName)/$($_.definition)"}|Sort-Object) -join ';'
    $afterIdentity=($after.Npcs.npcs|ForEach-Object {"$($_.handle)/$($_.spawnerName)/$($_.definition)"}|Sort-Object) -join ';'
    if($beforeIdentity -ne $afterIdentity){throw 'Authored NPC identity set changed during character conversion.'}
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -LeftY 16000 -DurationMs 700|Out-Null
    Start-Sleep -Milliseconds 1000
    $moved=Snapshot EmilyInput
    $movement=Distance $after.Player.candidatePosition $moved.Player.candidatePosition
    if($movement -lt 0.15 -or $moved.Character.character -ne 2){throw 'Converted Emily did not respond to native movement input.'}
    Command 'coop_character_probe corvo';Start-Sleep -Milliseconds 1200
    $back=Snapshot NativeCorvoRequested
    if($back.Character.character -ne 1 -or $back.Character.queued -or $back.Character.definition -ne 'models/characters/player/corvo/body.def'){throw 'Reverse native conversion failed.'}
}catch{$failure=$_.Exception.Message}finally {
    try {Load 81;$restored=Snapshot OriginalEmilyRestored;if($restored.Character.character -ne 2){throw 'Emily restoration failed.'}}catch{$restoreFailure=$_.Exception.Message}
}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure -and !$restoreFailure);Failure=$failure;RestoreFailure=$restoreFailure;
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;SourceCheckpointSha256=$sourceHash;MovementDistance=$movement;Samples=$rows;
    Scope='Unchanged host checkpoint loaded in client, then native character-choice API requests Emily. Checks native role/body definition, position, authored NPC identity set, native movement and reverse conversion. No automatic shared reset, powers, quests, inventory compatibility or both-ready barrier proven.'}|
    ConvertTo-Json -Depth 16|Set-Content (Join-Path $PSScriptRoot 'research/native-character-restore-verification.json')
if($failure){throw $failure};if($restoreFailure){throw $restoreFailure}
[pscustomobject]@{Passed=$true;MovementDistance=$movement;Samples=$rows.Count}
