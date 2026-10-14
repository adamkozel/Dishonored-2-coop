[CmdletBinding()]
param([Parameter(Mandatory)][ValidateSet('host','client')][string]$Role,[Parameter(Mandatory)][string]$Command,[switch]$WaitConsumed)
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'tooling/Publish-D2Packet.ps1')
if($Command.Length -gt 510 -or $Command -match '[^\x20-\x7e]|;' -or $Command -notmatch '^(getviewpos|where|Ansel_PlayerVisible|LoadGame|listDecls|debugEntityByName|trigger|teleport|setviewpos|find|listModels|coop_spawn_probe|coop_probe_follow|coop_probe_trace_pose|coop_probe_trace_transform|coop_probe_locomotion|coop_body_probe|coop_probe_body|coop_cinematic_sync|coop_cinematic_hold|coop_cinematic_barrier|coop_world_observe|coop_damage_observe|coop_damage_sample|coop_world_hold_ai|coop_guard_link|coop_guard_health|coop_partner_enemy|coop_guard_nudge|coop_guard_stall|coop_character_probe|coop_loading_probe|coop_gameover_probe|coop_gameover_retry|coop_checkpoint_sync|coop_checkpoint_delay|RestartFromCheckpoint|RestartFromMemoryCheckpoint)( |$)') {throw 'Command is outside the bounded native research command set.'}
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$instanceRoot=Join-Path $workspaceRoot "work\lab-instances\$Role"
$launch=Get-Content -LiteralPath (Join-Path $instanceRoot 'launch.json') -Raw | ConvertFrom-Json
$process=Get-Process -Id $launch.pid -ErrorAction Stop
if($process.Path -ne $launch.executable){throw 'The recorded lab process no longer matches.'}
$sequence=[uint32]([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()%4294967296L)
if(!$sequence){$sequence=1}
$bytes=[byte[]]::new(528)
[BitConverter]::GetBytes([uint32]0x43433244).CopyTo($bytes,0)
[BitConverter]::GetBytes($sequence).CopyTo($bytes,4)
$expiresUtc=[DateTime]::UtcNow.AddSeconds(10)
[BitConverter]::GetBytes([uint64]$expiresUtc.ToFileTimeUtc()).CopyTo($bytes,8)
[Text.Encoding]::ASCII.GetBytes($Command).CopyTo($bytes,16)
Publish-D2Packet -Path (Join-Path $instanceRoot 'command.bin') -Bytes $bytes -ExpiresUtc $expiresUtc
$consumed=$false
if($WaitConsumed){
    # The research mailbox holds one command. Explicitly wait between tightly
    # spaced commands so a later publication cannot overwrite an unread one.
    # Receipt confirms consumption, not success of the native operation.
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do {
        $statusPath=Join-Path $instanceRoot 'coop-status.json'
        if(Test-Path $statusPath){$status=Get-Content $statusPath -Raw|ConvertFrom-Json;$consumed=$status.lastCommand -eq $sequence}
        if(!$consumed){Start-Sleep -Milliseconds 25}
    }while(!$consumed -and $watch.ElapsedMilliseconds -lt 2500)
    if(!$consumed){throw "Receipt unconfirmed for $Role command sequence $sequence; it may still execute before its original expiry."}
}
[pscustomobject]@{Role=$Role;Sequence=$sequence;Command=$Command;Consumed=$consumed}



