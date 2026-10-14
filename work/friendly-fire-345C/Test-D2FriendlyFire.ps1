[CmdletBinding()]
param([switch]$StrikesOnly)
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Cmd($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function State($role,$suffix=''){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Health($role,$handle){for($i=0;$i-lt8;$i++){$raw=& $probe $pids[$role] --health $handle;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Health unavailable: $role $handle"}
function Sample($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){$s=State $role;$r[$role]=[pscustomobject]@{Bridge=$s;Filter=(State $role '.damage.json');Probe=(State $role '.friendly-fire.json');Player=(Health $role 1);Partner=(Health $role $s.probeActorHandle)}}
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
function StableHealth($row){foreach($role in 'host','client'){if($row.$role.Player.current-ne56-or$row.$role.Partner.current-ne40){throw "Friendly-fire changed $role health at $($row.Stage)"}}}
try{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable-or!$l.assetExperiment){throw 'Expected isolated asset experiment process'};$pids[$role]=$l.pid
  Cmd $role 'coop_probe_follow exact';Cmd $role 'coop_probe_locomotion on';Cmd $role 'coop_damage_observe off';Cmd $role 'coop_damage_observe 60000'
 }
 Cmd host 'setviewpos -33 -153 68.1 90 0';Cmd client 'setviewpos -28.0921 -153.962 68.1 169 0';Start-Sleep -Milliseconds 700
 $initial=Sample Initial;StableHealth $initial
 foreach($role in 'host','client'){if(!$initial.$role.Filter.ownedAvatarValid-or!$initial.$role.Filter.friendlyFireFilterEnabled){throw 'Owned avatar damage protection unavailable'}}
 if(!$StrikesOnly){
  foreach($pausedRole in 'host','client'){
   & "$PSScriptRoot/Send-D2Input.ps1" -Role $pausedRole -Button Start -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 500
   foreach($role in 'host','client'){Cmd $role 'coop_friendly_fire_probe'};Start-Sleep -Milliseconds 350
   $paused=Sample "Paused-$pausedRole";StableHealth $paused
   if($paused.$pausedRole.Bridge.localGameLogicState-ne2){throw 'Pause stimulus did not pause'}
   foreach($role in 'host','client'){if($paused.$role.Probe.applied-ne$initial.$role.Probe.applied){throw 'Test damage calls ran while pair was paused'}}
   & "$PSScriptRoot/Send-D2Input.ps1" -Role $pausedRole -Button A -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 650
  }
  foreach($role in 'host','client'){
   Cmd $role 'coop_probe_follow exact';Cmd $role 'coop_probe_locomotion on'
   Cmd $role 'coop_friendly_fire_probe';Start-Sleep -Milliseconds 450
   $hit=Sample "Native-$role";StableHealth $hit
   if($hit.$role.Probe.result-ne'passed'-or$hit.$role.Probe.applied-ne$initial.$role.Probe.applied+1-or$hit.$role.Probe.cases.Count-ne6-or$hit.$role.Probe.thread-ne$hit.$role.Bridge.engineThread){throw "Native friendly-fire probe failed for $role : $($hit.$role.Probe.result)"}
   if(@($hit.$role.Probe.cases|Where-Object {!$_.blocked-or$_.inflicted-ne0}).Count){throw 'A native friendly hit was not suppressed'}
   if($hit.$role.Filter.friendlyFireLocalBlocked-lt$initial.$role.Filter.friendlyFireLocalBlocked+3-or$hit.$role.Filter.friendlyFirePartnerBlocked-lt$initial.$role.Filter.friendlyFirePartnerBlocked+3){throw 'Independent filter counters missing'}
   Cmd $role 'coop_friendly_fire_probe';Start-Sleep -Milliseconds 300;$duplicate=Sample "Duplicate-$role";StableHealth $duplicate
   if($duplicate.$role.Probe.applied-ne$hit.$role.Probe.applied-or$duplicate.$role.Probe.refused-ne$hit.$role.Probe.refused+1-or$duplicate.$role.Filter.friendlyFireLocalBlocked-ne$hit.$role.Filter.friendlyFireLocalBlocked-or$duplicate.$role.Filter.friendlyFirePartnerBlocked-ne$hit.$role.Filter.friendlyFirePartnerBlocked){throw 'Duplicate probe did not refuse'}
   $checks.Add("${role}: six actual native entry calls blocked (actor and sword ownership in both directions, plus assassination); health unchanged; duplicate refused")
  }
  $checks.Add('Either paused player prevents developer probe calls in both instances')
 }else{
  foreach($role in 'host','client'){
   $peer=if($role-eq'host'){'client'}else{'host'}
   Cmd $role 'coop_probe_follow exact';Cmd $peer 'coop_probe_follow exact'
   Cmd $role 'setviewpos -33 -153 68.1 90 0';Cmd $peer 'setviewpos -33 -151.7 68.1 270 0';Start-Sleep -Milliseconds 900
   $before=Sample "BeforeStrikes-$role";StableHealth $before
   for($n=0;$n-lt3;$n++){
    & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -RightTrigger 255 -DurationMs 220|Out-Null
    Start-Sleep -Milliseconds 850;$s=Sample "Strike-$role-$n";StableHealth $s
   }
   if($s.$role.Filter.friendlyFireLocalBlocked-le$before.$role.Filter.friendlyFireLocalBlocked){throw "No confirmed sword contact against partner in $role; a miss is not a pass"}
   $checks.Add("${role}: real sword controls produced native player-to-partner contact; both player and clone health unchanged")
  }
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){try{& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Neutral -DurationMs 100|Out-Null;Cmd $role 'coop_damage_observe off'}catch{}}
}
$suffix=if($StrikesOnly){'-strikes'}else{'-native'}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Scope='Exact owned Doppelganger health filter; no god mode. Native constructor calls and real control contact are recorded separately. Unattributed indirect hazards and every power/projectile are not established by this test.';Checks=$checks;Samples=$rows}|ConvertTo-Json -Depth 18|Set-Content "$PSScriptRoot/research/friendly-fire$suffix.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$rows.Count}

