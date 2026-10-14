[CmdletBinding()]
param([ValidateSet('host','client')][string]$DeathRole='host')
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$failure=$null;$before=$null;$result=$null;$native=$null
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function Stream($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.avatar-weapon.json" -Raw|ConvertFrom-Json}
function Sword{Get-Content "$root/work/lab-instances/client/coop-status.json.avatar-sword.json" -Raw|ConvertFrom-Json}
try{
 & "$PSScriptRoot/Test-D2PlayerArms.ps1"|Out-Null
 foreach($role in 'host','client'){Command $role 'coop_probe_follow exact';Command $role 'coop_probe_arm on';Command $role 'coop_checkpoint_sync on'}
 Start-Sleep -Milliseconds 500
 foreach($role in 'host','client'){Command $role 'coop_avatar_weapon 15000'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 100;$before=[pscustomobject]@{Host=(Stream host);Client=(Stream client);Sword=(Sword)}}while((!$before.Host.ready-or!$before.Client.ready-or!$before.Sword.active-or$before.Sword.observed-ne$before.Sword.started)-and$w.ElapsedMilliseconds-lt2500)
 if(!$before.Host.ready-or!$before.Client.ready-or!$before.Sword.active){throw 'Native equipment not active before natural death'}
 & "$PSScriptRoot/Test-D2SharedDeath.ps1" -DeathRole $DeathRole -RetryRole client -DamageTelemetry -AvatarWeaponTelemetry|Out-Null
 $result=Get-Content "$PSScriptRoot/research/shared-death-$DeathRole-health-damage-equipment-verification.json" -Raw|ConvertFrom-Json
 if(!$result.Passed-or!$result.Before.client.Equipment.Sword.active){throw 'Natural-death evidence did not begin with active equipment'}
 foreach($role in 'host','client'){
  if($result.Screens.$role.Equipment.Stream.armed-or$result.Screens.$role.Equipment.Sword.active){throw 'Native death screen retained equipment ownership'}
  if($result.After.$role.Equipment.Stream.armed-or$result.After.$role.Equipment.Sword.active-or$result.After.$role.Equipment.Sword.restoring){throw 'Checkpoint retry resurrected old equipment'}
  if($result.After.$role.Health.current-ne56){throw 'Shared retry did not restore checkpoint health'}
 }
 $end=$result.After.client.Equipment.Sword
 if($end.lost-ne$before.Sword.lost-or($end.retired-$before.Sword.retired)+($end.restored-$before.Sword.restored)-ne1){throw 'Native sword cleanup was not confirmed exactly once across death/reload'}
 $l=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json;$b=$result.After.client.Bridge
 $native=& $probe $l.pid --avatar-sword $b.probeActorHandle|ConvertFrom-Json
 if($LASTEXITCODE-or$native.equipped-or$native.started-or$native.attachedTag-ne'holster_wpn'){throw 'New-world sword was not restored holstered'}
}catch{$failure=$_.Exception.Message}finally{foreach($role in 'host','client'){Command $role 'coop_avatar_weapon off';Command $role 'coop_probe_arm off'}}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;DeathRole=$DeathRole;
 Scope='Active Steam-equipped Corvo sword across one natural player fall death, both native death screens, overlapping retry requests and shared checkpoint restoration. No sword-hit death or collision claim.';Before=$before;SharedDeath=$result;RestoredNativeSword=$native}|ConvertTo-Json -Depth 25|Set-Content "$PSScriptRoot/research/avatar-weapon-$DeathRole-death-verification.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;DeathRole=$DeathRole;Retired=$end.retired-$before.Sword.retired;Restored=$end.restored-$before.Sword.restored}
