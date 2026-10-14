[CmdletBinding()]
param([ValidateSet('host','client')][string]$DeathRole='host')
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$failure=$null;$shared=$null;$native=@{}
function Command($r,$c){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function Lease($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.avatar-sword.json" -Raw|ConvertFrom-Json}
try{
 & "$PSScriptRoot/Test-D2PlayerArms.ps1"|Out-Null
 foreach($r in 'host','client'){Command $r 'coop_probe_follow exact';Command $r 'coop_probe_arm on';Command $r 'coop_checkpoint_sync on'}
 foreach($r in 'host','client'){Command $r 'coop_avatar_weapon 15000'}
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$h=Lease host;$c=Lease client}while((!$h.active-or!$c.active-or$h.observed-ne$h.started-or$c.observed-ne$c.started)-and$w.ElapsedMilliseconds-lt2500)
 if(!$h.active-or!$c.active){throw 'Both counterpart swords must be active before natural death'}
 & "$PSScriptRoot/Test-D2SharedDeath.ps1" -DeathRole $DeathRole -RetryRole client -DamageTelemetry -AvatarWeaponTelemetry|Out-Null
 $shared=Get-Content "$PSScriptRoot/research/shared-death-$DeathRole-health-damage-equipment-verification.json" -Raw|ConvertFrom-Json
 if(!$shared.Passed){throw 'Shared death test failed'}
 foreach($r in 'host','client'){
  $before=$shared.Before.$r.Equipment.Sword;$screen=$shared.Screens.$r.Equipment;$after=$shared.After.$r.Equipment
  if(!$before.active-or$screen.Stream.armed-or$screen.Sword.active-or$after.Stream.armed-or$after.Sword.active-or$after.Sword.restoring){throw "Equipment scope survived death/retry: $r"}
  if($after.Sword.lost-ne$before.lost-or($after.Sword.retired-$before.retired)+($after.Sword.restored-$before.restored)-ne1){throw "Equipment cleanup count differs: $r"}
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  $raw=& "$PSScriptRoot/bin/d2_runtime_probe.exe" $l.pid --avatar-sword $shared.After.$r.Bridge.probeActorHandle
  if($LASTEXITCODE){throw 'New-world native sword unavailable'}
  $native[$r]=$raw|ConvertFrom-Json
  if($native[$r].equipped-or$native[$r].started-or$native[$r].attachedTag-ne'holster_wpn'-or$native[$r].quantity-ne1){throw 'New-world sword did not start in native holster'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){Command $r 'coop_avatar_weapon off';Command $r 'coop_probe_arm off'}
}
[ordered]@{Time=(Get-Date).ToString('o');Passed=(!$failure);Failure=$failure;DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;DeathRole=$DeathRole;SharedDeath=$shared;NewWorldNativeSwords=$native;Scope='Both active native counterpart swords across one real fall death, both native death screens, overlapping retry and shared checkpoint recovery. Old leases clean up once; native inventories are recreated in the new world.'}|ConvertTo-Json -Depth 26|Set-Content "$PSScriptRoot/research/partner-weapon-$DeathRole-death-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;DeathRole=$DeathRole;SwordsRetiredOrRestored=2;NewWorldInventories=2}
