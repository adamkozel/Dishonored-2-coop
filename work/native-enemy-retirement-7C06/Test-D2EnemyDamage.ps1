[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role,$suffix=''){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $args=@($pids[$role],$option);if($handle){$args+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Unavailable $role $option"
}
function Capture($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){
  $r[$role]=[pscustomobject]@{Bridge=(State $role);Sample=(State $role '.enemy-sample.json');Guard=(State $role '.guard.json');Damage=(State $role '.damage.json');
   Health=(Native $role '--health' $handles[$role]);Player=(Native $role '--health' 1);Decision=(Native $role '--ai-decision' $handles[$role]);Reflex=(Native $role '--ai-reflex' $handles[$role]);
   Components=(Native $role '--components' $handles[$role]);Skeleton=(Native $role '--skeleton' $handles[$role])}
 }
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
try{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  $cat=Native $role '--world-npcs';$n=@($cat.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Ambiguous guard'};$handles[$role]=$n[0].handle
  Command $role 'coop_guard_link off';Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'
 }
 $before=Capture Initial
 foreach($role in 'host','client'){Command $role 'coop_enemy_damage assassination'}
 Start-Sleep -Milliseconds 300;$refused=Capture UnarmedRefused
 foreach($role in 'host','client'){if($refused.$role.Sample.applied-ne$before.$role.Sample.applied-or$refused.$role.Health.current-ne40-or$refused.$role.Sample.refused-ne$before.$role.Sample.refused+1){throw 'Unarmed native mutation was not refused'}}
 $checks.Add('Unarmed calls do not damage either guard')
 foreach($role in 'host','client'){Command $role 'coop_guard_link escape 60000'}
 Start-Sleep -Milliseconds 600;$armed=Capture Armed
 if($armed.client.Guard.phase-ne7-or$armed.client.Guard.heldReferences-ne2){throw 'Native guard ownership unavailable'}
 Command host 'coop_enemy_damage assassination';Start-Sleep -Milliseconds 200
 if((State host '.enemy-sample.json').applied-ne$before.host.Sample.applied){throw 'Host local experiment was not refused'}
 $checks.Add('Host cannot invoke the client-only death experiment')
 Command client 'coop_observer_isolation 30000'
 Command client 'setviewpos -42.9563 -112 62.7 270 0'
 Start-Sleep -Milliseconds 400
 Command client 'coop_enemy_damage assassination'
 foreach($delay in 400,1500,3000,6000){Start-Sleep -Milliseconds $delay;Capture "After-$delay"|Out-Null}
 $after=$rows[$rows.Count-1]
 if($after.client.Sample.applied-ne$before.client.Sample.applied+1-or$after.client.Sample.before-ne40-or$after.client.Sample.after-ne0-or$after.client.Sample.inflicted-ne40-or$after.client.Health.current-ne0){throw 'Native lethal damage was not confirmed'}
 if($after.host.Health.current-ne40-or$after.host.Damage.events.Count-or$after.client.Player.current-ne56-or$after.host.Player.current-ne56){throw 'Damage escaped the selected local guard'}
 foreach($ai in $after.client.Decision,$after.client.Reflex){if($ai.schedulerActive-or$ai.pauseReferences-or$ai.started){throw 'Native dead guard kept decision/reflex scheduling'}}
 if($after.client.Guard.phase-or$after.client.Guard.heldReferences-or$after.client.Guard.releaseLost-ne$armed.client.Guard.releaseLost-or$after.client.Guard.released-ne$armed.client.Guard.released+2){throw 'Native death left guard ownership behind'}
 $hits=@($after.client.Damage.events|Where-Object handle -eq ([Convert]::ToUInt32($handles.client.Substring(2),16)))
 if($hits.Count-ne1-or$hits[0].before-ne40-or!$hits[0].afterValid-or$hits[0].after-ne0-or$hits[0].damageType-ne'8c'-or$hits[0].attackerHandle-ne1){throw 'Independent native damage record differs'}
 $checks.Add('One canonical native damage call kills only the selected guard and leaves both players and host guard unchanged')
 $checks.Add('Guard ownership is released before death and native decision/reflex components stop without retained pause references')
 Command client 'coop_enemy_damage assassination';Start-Sleep -Milliseconds 300;$duplicate=Capture Duplicate
 if($duplicate.client.Sample.applied-ne$after.client.Sample.applied-or$duplicate.client.Sample.refused-ne$after.client.Sample.refused+1-or$duplicate.client.Damage.events.Count-ne$after.client.Damage.events.Count){throw 'Repeated local death call was not refused'}
 $checks.Add('Repeated native death experiment is refused without another damage event')
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){Command $role 'coop_guard_link off';Command $role 'coop_damage_observe off'}
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0';Command client 'coop_observer_isolation off'
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Client-only canonical native assassination damage experiment on one bound guard; no wire damage, kill animation, corpse physics or campaign progression synchronization claim.';Checks=$checks;Samples=$rows}|ConvertTo-Json -Depth 24|Set-Content "$PSScriptRoot/research/native-enemy-damage-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count}
