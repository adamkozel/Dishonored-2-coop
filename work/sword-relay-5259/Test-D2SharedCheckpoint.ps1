[CmdletBinding()]
param([ValidateSet('host','client')][string]$RequestRole='client',
      [ValidateSet('host','client')][string]$DelayRole='client',
      [switch]$Duplicate)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$samples=[Collections.Generic.List[object]]::new();$failure=$null;$rows=@();$before=@{};$pids=@{}
function State([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json}
function Reset([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json.reset.json") -Raw|ConvertFrom-Json}
function Command([string]$role,[string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null}
function Native([string]$role,[string]$option,[string]$address='') {
    $args=@($pids[$role],$option);if($address){$args+=$address}
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') @args
    if($LASTEXITCODE){throw "Native read failed for $role $option"};$raw|ConvertFrom-Json
}
try {
    foreach($role in 'host','client') {
        $pids[$role]=(Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json).pid
        if((State $role).localGameLogicState -ne 1){throw "$role must begin in gameplay."}
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null
        if(!(Reset $role).enabled){Command $role 'coop_checkpoint_sync on';Start-Sleep -Milliseconds 250}
        Command $role $(if($role -eq $DelayRole){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})
    }
    Start-Sleep -Milliseconds 700
    foreach($role in 'host','client') {
        $before[$role]=Reset $role
        if(!$before[$role].enabled -or !$before[$role].peerArm -or $before[$role].phase){throw "$role did not arm a fresh pair."}
    }
    if($before.host.digest -ne $before.client.digest){throw 'Different fixture digests.'}
    Command $RequestRole 'RestartFromCheckpoint'
    if($Duplicate) {
        Command $(if($RequestRole -eq 'host'){'client'}else{'host'}) 'RestartFromMemoryCheckpoint'
        Start-Sleep -Milliseconds 150;Command $RequestRole 'LoadGame 81'
    }
    $watch=[Diagnostics.Stopwatch]::StartNew();$lastPress=@{host=0;client=0}
    do {
        $done=$true
        foreach($role in 'host','client') {
            $r=Reset $role;$s=State $role
            if($r.phase -eq 7){throw "$role reset failed: $($r.result)"}
            if($r.held) {
                $native=Native $role '--loading-world' $r.game
                $samples.Add([pscustomobject]@{Role=$role;Trace=$r;Native=$native})
            }
            if($r.completed -ne $before[$role].completed+1 -or $s.localGameLogicState -ne 1 -or $s.localPendingGameLogicState){$done=$false}
            if(!$r.held -and $watch.ElapsedMilliseconds -gt 6000 -and $s.localGameLogicState -in 8,15 -and $watch.ElapsedMilliseconds-$lastPress[$role] -gt 700) {
                & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button A -DurationMs 100|Out-Null;$lastPress[$role]=$watch.ElapsedMilliseconds
            }
        }
        if(!$done){Start-Sleep -Milliseconds 100}
    }while(!$done -and $watch.ElapsedMilliseconds -lt 40000)
    if(!$done){throw 'Both peers did not complete the transaction.'}
    Start-Sleep -Milliseconds 2500
    $rows=@(foreach($role in 'host','client') {
        $r=Reset $role;$c=Native $role '--character';$player=Native $role '--player';$npcs=Native $role '--world-npcs'
        if($r.loads -ne $before[$role].loads+1 -or $r.completed -ne $before[$role].completed+1 -or $r.aborted -ne $before[$role].aborted){throw "$role did not perform exactly one successful load."}
        $expected=if($role -eq 'host'){1}else{2}
        if($c.character -ne $expected -or $c.queued -or $r.maxDistance -gt 0.001){throw "$role role or loading position mismatch."}
        $bridge=State $role;$avatars=@((Native $role '--entities').entities|Where-Object name -match '^d2coop_probe_')
        if($r.avatarRequested -and (!$r.avatarReady -or $r.avatarPending -or !$bridge.probeFollowEnabled -or
           $r.avatarRestored -ne $before[$role].avatarRestored+1 -or $avatars.Count -ne 1 -or
           $avatars[0].name -ne $(if($role -eq 'host'){'d2coop_probe_emily'}else{'d2coop_probe_corvo'}) -or
           $avatars[0].handle -ne ('0x'+([uint32]$bridge.probeActorHandle).ToString('x')))){throw "$role did not restore exactly one following counterpart."}
        [pscustomobject]@{Role=$role;Reset=$r;Character=$c;Player=$player;Npcs=$npcs;Bridge=$bridge;Avatars=$avatars}
    })
    $h=$rows[0].Reset;$c=$rows[1].Reset
    if($h.transaction -ne $c.transaction -or $h.hostReady -ne $c.hostReady -or $h.clientReady -ne $c.clientReady){throw 'Transaction or ready tokens disagree.'}
    if($h.releasedAt -lt $c.readyAt -or $c.releasedAt -lt $h.readyAt){throw 'Gameplay released before both ready messages.'}
    foreach($sample in $samples) {
        $end=($rows|Where-Object Role -eq $sample.Role).Reset.releasedAt
        if($sample.Native.tick -ge $sample.Trace.heldAt -and $sample.Native.tick -lt $end -and
           ($sample.Native.logic -ne 15 -or $sample.Native.pending -or $sample.Native.queued -or $sample.Native.character -ne $(if($sample.Role -eq 'host'){1}else{2}))){throw 'Independent read found incorrect role/state inside the loading hold.'}
    }
    if($samples.Count -lt 3){throw 'Insufficient independent loading samples.'}
    $hostNpcs=($rows[0].Npcs.npcs|ForEach-Object {"$($_.spawnerName)/$($_.definition)"}|Sort-Object)-join ';'
    $clientNpcs=($rows[1].Npcs.npcs|ForEach-Object {"$($_.spawnerName)/$($_.definition)"}|Sort-Object)-join ';'
    if($hostNpcs -ne $clientNpcs){throw 'Authored NPC identities differ after the shared checkpoint.'}
    if($Duplicate -and (($h.duplicates-$before.host.duplicates)+($c.duplicates-$before.client.duplicates)) -lt 1){throw 'No duplicate callback was observed.'}
}catch{$failure=$_.Exception.Message}
$suffix=if($Duplicate){'duplicates'}else{"$RequestRole-delay-$DelayRole"}
$report=[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;RequestRole=$RequestRole;DelayRole=$DelayRole;Duplicate=[bool]$Duplicate;
    Scope='Steam-coordinated fixed pre-staged Dunwall SLOT82, one native load per peer, both-ready loading hold, preserved Corvo/Emily roles and restoration of previously enabled counterpart presentation. No save transfer; this test does not cause death.';
    Before=$before;After=$rows;Samples=$samples}
$report|ConvertTo-Json -Depth 18|Set-Content (Join-Path $PSScriptRoot "research/shared-checkpoint-$suffix-verification.json")
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;RequestRole=$RequestRole;DelayRole=$DelayRole;Duplicate=[bool]$Duplicate;Transaction=$rows[0].Reset.transaction;NativeSamples=$samples.Count;HostHeldMs=$h.releasedAt-$h.heldAt;ClientHeldMs=$c.releasedAt-$c.heldAt}
