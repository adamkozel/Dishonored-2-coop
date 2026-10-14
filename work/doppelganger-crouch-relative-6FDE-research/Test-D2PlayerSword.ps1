[CmdletBinding()]
param()
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$pids=@{}
function Input($role,$button,$duration=200){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button $button -DurationMs $duration|Out-Null}
function Native($role){for($i=0;$i-lt8;$i++){$raw=& $probe $pids[$role] --player-sword;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw 'Native player sword unavailable'}
function Capture($role,$stage){$s=Native $role;$samples.Add([pscustomobject]@{Role=$role;Stage=$stage;Native=$s});return $s}
try{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  & "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command 'coop_probe_arm off' -WaitConsumed|Out-Null
 }
 foreach($role in 'host','client'){
  $a=Capture $role Drawn;$expected=if($role-eq'host'){1}else{2}
  if(!$a.equipped-or!$a.started-or$a.joint-ne59-or$a.character-ne$expected-or!$a.armHandle-or$a.tag-ne'weapon_attach_tag'){throw 'Expected native drawn sword'}
  Input $role X 1500;Start-Sleep -Milliseconds 2200;$h=Capture $role Holstered
  if($h.equipped-or$h.started-or$h.armHandle-or$h.parent-ne$h.actor-or$h.joint-ne2-or$h.tag-ne'sword_attach_tag'-or$h.weaponHandle-ne$a.weaponHandle){throw 'Native holster identity mismatch'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -RightTrigger 255 -DurationMs 220|Out-Null
  Start-Sleep -Milliseconds 1400;$d=Capture $role Redrawn
  if(!$d.equipped-or!$d.started-or$d.weaponHandle-ne$a.weaponHandle-or$d.armHandle-ne$a.armHandle){throw 'Native redraw identity mismatch'}
  Input $role Start;Start-Sleep -Milliseconds 400;$p=Capture $role Paused
  if($p.logic-ne2-or!$p.equipped-or$p.weaponHandle-ne$a.weaponHandle){throw 'Read-only sword identity failed in pause'}
  Input $role A;Start-Sleep -Milliseconds 350;$r=Capture $role Resumed
  if($r.logic-ne1-or!$r.equipped-or$r.weaponHandle-ne$a.weaponHandle){throw 'Sword identity changed on resume'}
  $health=& $probe $pids[$role] --health 1|ConvertFrom-Json;if($LASTEXITCODE-or$health.current-ne56){throw 'Sword observation changed real player health'}
  $checks.Add("$role native draw/holster/redraw uses one persistent weapon, reciprocal right-arm/body attachments and matching slot/start state; identity survives pause/resume")
 }
}catch{$failure=$_.Exception.Message}finally{foreach($role in 'host','client'){Input $role Neutral}}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Read-only validation of actual local player sword ownership and draw state under normal game controls in both roles. No remote equipment application.';Checks=$checks;Samples=$samples}|ConvertTo-Json -Depth 12|Set-Content "$PSScriptRoot/research/player-sword-verification.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$samples.Count}
