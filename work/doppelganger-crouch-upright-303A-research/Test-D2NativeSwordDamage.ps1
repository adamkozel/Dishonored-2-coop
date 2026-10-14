[CmdletBinding()]
param([switch]$Back)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$pids=@{};$rows=[Collections.Generic.List[object]]::new();$failure=$null
$command=if($Back){'coop_damage_sample sword_back'}else{'coop_damage_sample sword'}
$expectedDamage=if($Back){5}else{15};$expectedHealth=56-$expectedDamage
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Sample($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){
  $raw=& $probe $pids[$role] --health 1;if($LASTEXITCODE){throw 'Native health unavailable'}
  $r[$role]=@{Health=($raw|ConvertFrom-Json);Status=(Get-Content "$root/work/lab-instances/$role/coop-status.json.sword-sample.json" -Raw|ConvertFrom-Json);Damage=(Get-Content "$root/work/lab-instances/$role/coop-status.json.damage.json" -Raw|ConvertFrom-Json);Bridge=(Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json)}
 }
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
try {
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'};$pids[$role]=$launch.pid
  Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'
 }
 $initial=Sample Initial;if($initial.host.Health.current-ne56-or$initial.client.Health.current-ne56){throw 'Expected unmodified Dunwall health'}
 Command host $command;Start-Sleep -Milliseconds 400;$wrong=Sample WrongRole
 if($wrong.host.Health.current-ne56-or$wrong.host.Status.applied-ne$initial.host.Status.applied-or$wrong.host.Status.refused-le$initial.host.Status.refused){throw 'Host authority guard failed'}
 & "$PSScriptRoot/Send-D2Input.ps1" -Role client -Button Start -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 500
 Command client $command;Start-Sleep -Milliseconds 300;$paused=Sample Paused
 if($paused.client.Bridge.localGameLogicState-ne2-or$paused.client.Health.current-ne56-or$paused.client.Status.applied-ne$initial.client.Status.applied){throw 'Paused player accepted damage'}
 & "$PSScriptRoot/Send-D2Input.ps1" -Role client -Button A -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 600
 Command client $command;Start-Sleep -Milliseconds 500;$hit=Sample Hit
 if($hit.client.Status.applied-ne$initial.client.Status.applied+1-or$hit.client.Status.thread-ne$hit.client.Bridge.engineThread-or$hit.client.Health.current-ne$expectedHealth-or$hit.host.Health.current-ne56){throw 'Native sword result differs from expected directional difficulty multiplier'}
 $events=@($hit.client.Damage.events|Where-Object handle -eq 1)
 if($events.Count-ne1-or$events[0].damageAmount-ne20-or$events[0].damageType-ne'80'-or$events[0].inflictedDamage-ne$expectedDamage-or$events[0].before-ne56-or$events[0].after-ne$expectedHealth-or!$events[0].afterValid){throw 'Independent native callback evidence missing'}
 Command client $command;Start-Sleep -Milliseconds 400;$duplicate=Sample Duplicate
 if($duplicate.client.Health.current-ne$expectedHealth-or$duplicate.client.Status.applied-ne$hit.client.Status.applied-or$duplicate.client.Damage.events.Count-ne$hit.client.Damage.events.Count){throw 'Duplicate caused damage'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){Command $role 'coop_damage_observe off'}
}
$suffix=if($Back){'-back'}else{''}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='One explicit local client-only sword damage reconstruction through native declaration and parameter/result constructors and native health damage entry. Actual authored guard attacker, invalid inflictor, no network relay. Wrong role, pause and duplicate refusal.';Back=[bool]$Back;ExpectedDamage=$expectedDamage;Samples=$rows}|ConvertTo-Json -Depth 18|Set-Content "$PSScriptRoot/research/native-sword-damage$suffix-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;NativeDamage=$expectedDamage;NetworkDamage=$false}
