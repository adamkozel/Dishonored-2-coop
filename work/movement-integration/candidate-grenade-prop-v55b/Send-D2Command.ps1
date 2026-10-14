[CmdletBinding()]
param([Parameter(Mandatory)][ValidateSet('host','client')][string]$Role,[Parameter(Mandatory)][string]$Command,[switch]$WaitConsumed,[ValidateRange(100,9000)][int]$ReceiptTimeoutMs=2500)
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'tooling/Publish-D2Packet.ps1')
if($Command.Length -gt 510 -or $Command -match '[^\x20-\x7e]|;' -or $Command -notmatch '^(getviewpos|where|Ansel_PlayerVisible|LoadGame|listDecls|debugEntityByName|trigger|teleport|setviewpos|find|listModels|coop_authoring_proof|coop_movement_bank|coop_movement_resume|coop_movement_feet|coop_review_clip|coop_probe_smoothing|coop_pose_trace|coop_shadow_stream|coop_shadow_stream_duplicate|coop_shadow_action|coop_shadow_pose|coop_shadow_crouch|coop_shadow_damage_probe|coop_friendly_fire_probe|coop_mixamo_crouch|coop_shadow_loco|coop_decl_probe|coop_spawn_probe|coop_probe_follow|coop_probe_trace_pose|coop_probe_trace_transform|coop_probe_locomotion|coop_body_probe|coop_arm_probe|coop_probe_arm|coop_probe_arm_space|coop_arm_duplicate|coop_shadow_equipment|coop_avatar_sword|coop_avatar_weapon|coop_avatar_weapon_duplicate|coop_probe_body|coop_cinematic_sync|coop_cinematic_hold|coop_cinematic_barrier|coop_world_observe|coop_damage_observe|coop_damage_sample|coop_enemy_damage|coop_enemy_outcome|coop_enemy_duplicate|coop_enemy_melee|coop_enemy_melee_duplicate|coop_sword_link|coop_sword_duplicate|coop_world_hold_ai|coop_guard_link|coop_guard_body|coop_guard_equipment|coop_guard_weapon|coop_observer_isolation|coop_movement_fixture|coop_power_fixture|coop_darkvision_fixture|coop_windblast_fixture|coop_bendtime_fixture|coop_ranged_fixture|coop_grenade_fixture|coop_grenade_prop|coop_ranged_prop|coop_agility_fixture|coop_carry_attachment|coop_power_fx|coop_source_isolation|coop_guard_health|coop_partner_enemy|coop_guard_nudge|coop_guard_stall|coop_character_probe|coop_loading_probe|coop_gameover_probe|coop_gameover_retry|coop_checkpoint_sync|coop_checkpoint_delay|RestartFromCheckpoint|RestartFromMemoryCheckpoint)( |$)') {throw 'Command is outside the bounded native research command set.'}
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
    }while(!$consumed -and $watch.ElapsedMilliseconds -lt $ReceiptTimeoutMs -and [DateTime]::UtcNow -lt $expiresUtc)
    if(!$consumed){throw "Receipt unconfirmed for $Role command sequence $sequence; it may still execute before its original expiry."}
}
[pscustomobject]@{Role=$Role;Sequence=$sequence;Command=$Command;Consumed=$consumed}
















