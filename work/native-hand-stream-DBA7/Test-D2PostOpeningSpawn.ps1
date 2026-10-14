[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe'
function Snapshot([string]$role) {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected game process.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale game status.'}
    $status=Get-Content $file.FullName -Raw|ConvertFrom-Json
    if($status.localGameLogicState -ne 1 -or !$status.localPoseAvailable -or !$status.remoteFresh){throw 'Connected native gameplay required.'}
    $native=& $probe $launch.pid --entities
    if($LASTEXITCODE){throw 'Independent native entity read failed.'}
    [pscustomobject]@{Status=$status;Entities=($native|ConvertFrom-Json).entities}
}
$tests=@();$failure=$null
try {
    foreach($role in 'host','client') {
        $character=if($role -eq 'host'){'emily'}else{'corvo'}
        $expected=if($character -eq 'emily'){'models/characters/def/story/emily_kaldwin.def'}else{'models/characters/def/story/corvo_attano.def'}
        $before=Snapshot $role
        if($before.Status.probeActorHandle -or @($before.Entities|Where-Object definitionName -eq $expected).Count){throw 'This test requires the original character NPC to have been removed and no existing probe.'}
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command "coop_spawn_probe $character"|Out-Null
        Start-Sleep -Milliseconds 1000
        $after=Snapshot $role
        $handle='0x'+([uint32]$after.Status.probeActorHandle).ToString('x')
        $actors=@($after.Entities|Where-Object handle -eq $handle)
        $spawnerName="ld_spawner_${character}_audience"
        $spawnerBefore=@($before.Entities|Where-Object name -eq $spawnerName)
        $spawnerAfter=@($after.Entities|Where-Object name -eq $spawnerName)
        $passed=$actors.Count -eq 1 -and $actors[0].definitionName -eq $expected -and
            $actors[0].name -match "^d2coop_probe_$character" -and $actors[0].type -eq '.?AVarkNPC@@' -and
            $actors[0].physicsBodies.Count -eq 17 -and $actors[0].renderPosition.Count -eq 3 -and
            $spawnerBefore.Count -eq 1 -and $spawnerAfter.Count -eq 1 -and $spawnerBefore[0].handle -eq $spawnerAfter[0].handle
        $tests+=[pscustomobject]@{Role=$role;Character=$character;OriginalCharacterCount=0;Actor=$actors;SpawnerBefore=$spawnerBefore;SpawnerAfter=$spawnerAfter;Passed=$passed}
        if(!$passed){throw "Post-opening $character spawn failed."}
        # Duplicate requests must keep the same owned actor, with no extra NPC.
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command "coop_spawn_probe $character"|Out-Null
        Start-Sleep -Milliseconds 400
        $duplicate=Snapshot $role
        if($duplicate.Status.probeActorHandle -ne $after.Status.probeActorHandle -or @($duplicate.Entities|Where-Object name -match '^d2coop_probe_').Count -ne 1){throw 'Duplicate request created or replaced an avatar.'}
    }
}catch{$failure=$_.Exception.Message}
$record=Join-Path $PSScriptRoot 'research/post-opening-spawn-verification.json'
[ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;Tests=$tests;Scope='Native independent Corvo/Emily NPC creation after removal of the original character NPCs, retained spawner identity, 17 native bodies and duplicate-request refusal. Does not establish remote animation, collision authority or playable co-op.'}|ConvertTo-Json -Depth 10|Set-Content $record
if($failure){throw $failure}
$tests|Select-Object Role,Character,OriginalCharacterCount,Passed
