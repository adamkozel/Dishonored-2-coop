[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
function State([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json}
function Reset([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json.reset.json") -Raw|ConvertFrom-Json}
function Command([string]$role,[string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null;Start-Sleep -Milliseconds 350}
$fixture=Join-Path $root 'work/lab-instances/client/base/savegame/SLOT82/game.details'
$saved=[IO.File]::ReadAllBytes($fixture);$before=@{};$after=@{};$failure=$null
try {
    foreach($role in 'host','client') {
        if((State $role).localGameLogicState -ne 1){throw 'Both games must start in gameplay.'}
        Command $role 'coop_checkpoint_sync off'
        if((Reset $role).enabled){throw 'An active reset prevents the fixture mismatch test.'}
    }
    # Only the pre-staged experimental copy changes, never SLOT81 or Steam.
    # No native load may be dispatched while these bytes differ.
    $changed=[byte[]]::new($saved.Length+1);$saved.CopyTo($changed,0);$changed[-1]=10
    [IO.File]::WriteAllBytes($fixture,$changed)
    foreach($role in 'host','client'){Command $role 'coop_checkpoint_sync on'}
    Start-Sleep -Milliseconds 700
    foreach($role in 'host','client'){$before[$role]=[pscustomobject]@{Reset=(Reset $role);Bridge=(State $role)}}
    if($before.host.Reset.digest -eq $before.client.Reset.digest){throw 'Hash mismatch was not represented.'}
    foreach($role in 'host','client'){Command $role 'RestartFromCheckpoint'}
    Start-Sleep -Milliseconds 1200
    foreach($role in 'host','client') {
        $after[$role]=[pscustomobject]@{Reset=(Reset $role);Bridge=(State $role)}
        if($after[$role].Reset.loads -ne $before[$role].Reset.loads -or $after[$role].Reset.phase -or
           $after[$role].Bridge.localEpoch -ne $before[$role].Bridge.localEpoch -or $after[$role].Bridge.localGameLogicState -ne 1 -or
           $after[$role].Reset.rejected -le $before[$role].Reset.rejected){throw "$role accepted or failed to reject the mismatched reset."}
    }
}catch{$failure=$_.Exception.Message}finally {
    [IO.File]::WriteAllBytes($fixture,$saved)
    foreach($role in 'host','client'){Command $role 'coop_checkpoint_sync off'}
}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;
    Scope='Different pre-staged checkpoint metadata hashes refuse both native reload requests before either world changes; experimental metadata restored byte-for-byte.';
    Before=$before;After=$after;RestoredMetadataSha256=(Get-FileHash $fixture).Hash}|ConvertTo-Json -Depth 12|
    Set-Content (Join-Path $PSScriptRoot 'research/shared-checkpoint-mismatch-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;HostLoads=$after.host.Reset.loads;ClientLoads=$after.client.Reset.loads;RefusedBoth=$true}
