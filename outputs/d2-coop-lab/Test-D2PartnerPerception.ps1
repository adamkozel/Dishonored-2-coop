[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$rows=[Collections.Generic.List[object]]::new();$pids=@{};$handles=@{};$failure=$null;$damage=@{}
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Probe($role,$option,$handle){
 for($n=0;$n-lt4;$n++){$raw=& $probe $pids[$role] $option $handle;if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15}
 return @{Unavailable=$LASTEXITCODE}
}
try {
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'};$pids[$role]=$launch.pid
  $catalog=(& $probe $launch.pid --world-npcs)|ConvertFrom-Json;if($LASTEXITCODE){throw 'Catalog unavailable'}
  $guard=@($catalog.npcs|Where-Object spawnerName -eq 'ld_spawner_guard_escape_1')
  if($guard.Count-ne1-or!$guard[0].unique){throw 'Unique authored escape guard required'}
  $handles[$role]=$guard[0].handle
  if((State $role).localGameLogicState-ne1){throw 'Both peers must begin in gameplay'}
  Command $role 'coop_checkpoint_sync off';Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000';Command $role 'coop_probe_follow on'
 }
 Command host 'coop_probe_follow exact';Start-Sleep -Milliseconds 500
 $initial=State host;$initialHealth=Probe host '--health' '1'
 $baseline=Probe host '--ai-targets' $handles.host
 if($baseline.Unavailable-or$baseline.combat.handle-eq$initial.probeActorHandle){throw 'Baseline guard must not already target partner'}
 Command host 'coop_partner_enemy escape 8000';Command client 'setviewpos -42.9563 -117.27 62.7 270 0'
 for($i=0;$i-lt22;$i++){
  $row=[ordered]@{At=[DateTimeOffset]::Now.ToString('o')}
  foreach($role in 'host','client'){
   $bridge=State $role
   $row[$role]=@{Bridge=$bridge;Lease=(Get-Content "$root/work/lab-instances/$role/coop-status.json.partner-ai.json" -Raw|ConvertFrom-Json);
    Targets=(Probe $role '--ai-targets' $handles[$role]);Attack=(Probe $role '--ai-attack' $handles[$role]);GuardPose=(Probe $role '--npc-pose' $handles[$role]);
    PartnerPose=(Probe $role '--npc-pose' $bridge.probeActorHandle);Health=(Probe $role '--health' '1');PartnerHealth=(Probe $role '--health' $bridge.probeActorHandle)}
  }
  $rows.Add([pscustomobject]$row);Start-Sleep -Milliseconds 200
 }
 $targeted=@($rows|Where-Object {$_.host.Targets.combat.resolved-and$_.host.Targets.combat.handle-eq$initial.probeActorHandle-and
   @($_.host.Targets.attention|Where-Object {$_.handle-eq$initial.probeActorHandle-and$_.perceived-eq2-and$_.cached-eq2}).Count-eq1-and
   $_.host.Attack.started-and$_.host.Attack.schedulerActive})
 if($targeted.Count-lt3){throw 'Insufficient independent native partner perception and attack-scheduler samples'}
 foreach($s in $targeted){if(!$s.host.Bridge.probeFollowExact-or@($s.host.Bridge.probeOffset|Where-Object {$_-ne0}).Count){throw 'Exact world coordinates required'}}
 if(@($rows|Where-Object {$_.host.Health.current-ne$initialHealth.current}).Count){throw 'Host player took unexpected damage'}
 $positive=@($rows|Where-Object {$_.client.Targets.combat.resolved-and$_.client.Targets.combat.handle-eq1})
 if(!$positive.Count){throw 'No independently resolved native player combat target positive control'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  $damage[$role]=Get-Content "$root/work/lab-instances/$role/coop-status.json.damage.json" -Raw|ConvertFrom-Json
  Command $role 'coop_damage_observe off';Command $role 'coop_partner_enemy off'
 }
 if(!$failure-and!@($damage.client.events|Where-Object {$_.handle-eq1-and$_.inflictedDamage-gt0}).Count){$failure='No native player damage positive control'}
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;
 ProbeSha256=(Get-FileHash $probe).Hash;Scope='Native escape guard perceives the remote Emily counterpart as enemy, resolves it as combat target and activates the native attack scheduler. Native player control receives real damage. This does not establish a landed hit against the partner or damage replication.';
 Baseline=$baseline;TargetedSamples=$targeted.Count;PositiveControlSamples=$positive.Count;Samples=$rows;Damage=$damage;DamageReplicated=$false}|ConvertTo-Json -Depth 20|Set-Content "$PSScriptRoot/research/partner-perception-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;PartnerTargetedSamples=$targeted.Count;NativePlayerPositiveControls=$positive.Count;PartnerHitEvents=@($damage.host.events|Where-Object handle -eq $initial.probeActorHandle).Count}
