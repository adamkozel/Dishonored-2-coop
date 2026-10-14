[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$pids=@{};$rows=[Collections.Generic.List[object]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Link($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.sword-link.json" -Raw|ConvertFrom-Json}
function Sample($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){
  $raw=& $probe $pids[$role] --health 1;if($LASTEXITCODE){throw 'Native health unavailable'}
  $r[$role]=@{Health=($raw|ConvertFrom-Json);Link=(Link $role);Bridge=(Bridge $role);Damage=(Get-Content "$root/work/lab-instances/$role/coop-status.json.damage.json" -Raw|ConvertFrom-Json);Enemy=(Get-Content "$root/work/lab-instances/$role/coop-status.json.partner-ai.json" -Raw|ConvertFrom-Json)}
 }
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
try {
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'};$pids[$role]=$launch.pid
  Command $role 'coop_sword_link off';Command $role 'coop_partner_enemy off';Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000';Command $role 'coop_probe_follow on'
 }
 # Use the existing offset follow fixture to leave Emily near a host guard
 # while her actual client stays in the safe starting room. This isolates the
 # source of damage; this offset is only for the experiment, not gameplay.
 Command client 'setviewpos -42.9563 -117.27 62.7 270 0'
 Command host 'coop_probe_follow exact';Start-Sleep -Milliseconds 150
 Command host 'coop_probe_follow off'
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0'
 Start-Sleep -Milliseconds 400;Command host 'coop_probe_follow on';Start-Sleep -Milliseconds 300
 $initial=Sample Initial
 if($initial.host.Health.current-ne56-or$initial.client.Health.current-ne56){throw 'Setup received local damage'}
 if([Math]::Abs($initial.host.Bridge.probeRenderPosition[1]+117.27)-gt.1-or[math]::Abs($initial.client.Bridge.localPosition[1]+153.962)-gt.1){throw 'Isolated partner placement failed'}
 Command client 'coop_sword_link 15000';Start-Sleep -Milliseconds 400;$single=Sample ClientOnly
 if($single.client.Link.ready-or$single.host.Link.armed-or$single.client.Link.applied-ne$initial.client.Link.applied){throw 'One-sided arm authorized damage'}
 Command host 'coop_sword_link 15000'
 $watch=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 100;$h=Link host;$c=Link client}while((!$h.ready-or!$c.ready)-and$watch.ElapsedMilliseconds-lt2000)
 if(!$h.ready-or!$c.ready-or$h.hostArm-ne$c.hostArm-or$h.clientArm-ne$c.clientArm){throw 'Fresh two-sided handshake failed'}
 $armed=Sample Armed;Command host 'coop_partner_enemy escape 8000'
 $watch.Restart();do{
  Start-Sleep -Milliseconds 200;$hit=Sample Observing
 }while($hit.client.Link.applied-eq$initial.client.Link.applied-and$watch.ElapsedMilliseconds-lt9000)
 if($hit.client.Link.applied-ne$initial.client.Link.applied+1-or$hit.host.Link.captured-ne$initial.host.Link.captured+1){throw 'No single native hit relayed'}
 if($hit.host.Health.current-ne56-or$hit.client.Health.current-notin41,51-or$hit.client.Link.thread-ne$hit.client.Bridge.engineThread){throw 'Unexpected native player damage or thread'}
 $source=@($hit.host.Damage.events|Where-Object {$_.handle-eq$initial.host.Bridge.probeActorHandle-and$_.damageAmount-eq20-and$_.damageType-eq'80'})
 $target=@($hit.client.Damage.events|Where-Object handle -eq 1)
 if(!$source.Count-or$target.Count-ne1-or$target[0].before-ne56-or$target[0].after-ne$hit.client.Health.current-or$target[0].inflictedDamage-ne56-$hit.client.Health.current){throw 'Independent source and receiving callback evidence failed'}
 if($target[0].attackerHandle-ne$hit.host.Enemy.guardHandle){throw 'Authored attacker mapping failed in this fixture'}
 Command host 'coop_sword_duplicate';Start-Sleep -Milliseconds 400;$duplicate=Sample Duplicate
 if($duplicate.client.Link.applied-ne$hit.client.Link.applied-or$duplicate.client.Health.current-ne$hit.client.Health.current-or$duplicate.client.Link.rejected-le$hit.client.Link.rejected){throw 'Duplicate hit was not rejected'}
 Command host 'coop_sword_link off';Start-Sleep -Milliseconds 400;$off=Sample Cancelled
 if($off.host.Link.armed-or$off.client.Link.armed){throw 'Cancellation failed to reach both peers'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){Command $role 'coop_sword_link off';Command $role 'coop_partner_enemy off';Command $role 'coop_damage_observe off'}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='One actual host escape-guard sword hit on the remote Emily body, reconstructed as native client-player damage over Steam. Both sides explicitly arm; one event only, original attacker resolved locally, no inflictor. Client held in safe starting room using the temporary offset fixture. Not general combat, native hit reactions, lethal damage, client guard suppression or world authority.';Samples=$rows}|ConvertTo-Json -Depth 20|Set-Content "$PSScriptRoot/research/sword-relay-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;NativeDamage=$hit.client.Link.inflicted;SteamHitRelayed=$true;DuplicatesRejected=$true}
