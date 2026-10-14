param([switch]$Prepared,[int[]]$Slots=@(82,83))
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
$source=Join-Path $root 'work/lab-instances/host/base/savegame/SLOT81'
$client=Join-Path $root 'work/lab-instances/client'
$rows=[Collections.Generic.List[object]]::new()
foreach($slot in $Slots) {
    $dest=Join-Path $client "base/savegame/SLOT$slot"
    if(!$Prepared -and (Test-Path $dest)){throw "Research slot $slot already exists; refusing overwrite."}
}
function Load([int]$slot) {
    $old=Get-Content (Join-Path $client 'coop-status.json') -Raw|ConvertFrom-Json
    & (Join-Path $lab 'Send-D2Command.ps1') -Role client -Command "LoadGame $slot"|Out-Null
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do {
        Start-Sleep -Milliseconds 200
        $s=Get-Content (Join-Path $client 'coop-status.json') -Raw|ConvertFrom-Json
        if($s.localEpoch -ne $old.localEpoch -and $s.localGameLogicState -eq 1 -and !$s.localPendingGameLogicState){return $s}
        if($watch.ElapsedMilliseconds -gt 6000 -and $s.localGameLogicState -in 8,15){& (Join-Path $lab 'Send-D2Input.ps1') -Role client -Button A -DurationMs 200|Out-Null}
    }while($watch.ElapsedMilliseconds -lt 25000)
    throw 'Checkpoint did not load.'
}
$errorText=$null
try {
    foreach($slot in $Slots) {
        $dest=Join-Path $client "base/savegame/SLOT$slot"
        if(!$Prepared){Copy-Item -LiteralPath $source -Destination $dest -Recurse}
        $details=Join-Path $dest 'game.details'
        if(!$Prepared -and $slot -eq 83) {
            $text=[IO.File]::ReadAllText($details)
            if($text -notmatch '(?m)^character=1\r?$'){throw 'Expected host Corvo metadata.'}
            [IO.File]::WriteAllText($details,($text -replace '(?m)^character=1(?=\r?$)','character=2'),[Text.UTF8Encoding]::new($false))
        }
        if((Get-FileHash (Join-Path $dest 'checkpoint.dat')).Hash -ne (Get-FileHash (Join-Path $source 'checkpoint.dat')).Hash){throw 'Prepared slot differs from host fixture.'}
        $s=Load $slot
        $launch=Get-Content (Join-Path $client 'launch.json') -Raw|ConvertFrom-Json
        $raw=& (Join-Path $lab 'bin/d2_runtime_probe.exe') $launch.pid --entities
        if($LASTEXITCODE){throw 'Independent entity read failed.'}
        $players=@(($raw|ConvertFrom-Json).entities|Where-Object {$_.type -eq '.?AVarkPlayer@@' -and $_.name -eq 'player1' -and $_.handle -eq '0x1'})
        if($players.Count -ne 1){throw 'Expected one native local player.'}
        $rows.Add([pscustomobject]@{Slot=$slot;MetadataCharacter=$(if($slot -eq 82){1}else{2});Player=$players[0];Epoch=$s.localEpoch;
            SaveHash=(Get-FileHash (Join-Path $dest 'checkpoint.dat')).Hash;SourceHash=(Get-FileHash (Join-Path $source 'checkpoint.dat')).Hash;
            Trace=(Get-Content (Join-Path $client 'coop-status.json.checkpoint.json') -Raw|ConvertFrom-Json)})
    }
}catch{$errorText=$_.Exception.Message}finally {
    $restoreError=$null;$restored=$null
    try{$restored=Load 81}catch{$restoreError=$_.Exception.Message}
    & (Join-Path $lab 'Send-D2Input.ps1') -Role client -DurationMs 100|Out-Null
}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Failure=$errorText;RestoreFailure=$restoreError;DllSha256=(Get-FileHash (Join-Path $lab 'bin/dinput8.dll')).Hash;
    Trials=$rows.ToArray();RestoredClientEpoch=$restored.localEpoch;
    Scope='Copy of the host checkpoint into disposable client slots. Only game.details character metadata differs in slot 83. Original client slot 81 restored. Tests native character selection when loading identical checkpoint bytes; does not implement a shared save or mutate the binary checkpoint.'}|
    ConvertTo-Json -Depth 10|Set-Content (Join-Path $lab 'research/checkpoint-character-compatibility.json')
if($errorText){throw $errorText}
$rows|Select-Object Slot,MetadataCharacter,@{n='NativePlayerDefinition';e={$_.Player.definitionName}}
