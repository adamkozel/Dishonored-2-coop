param([switch]$AfterMayhew)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;$lab=Join-Path $root outputs/d2-coop-lab;$probe="$lab/bin/d2_runtime_probe.exe"
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$pids=@{};$handles=@{}
function Command($role,$command){& "$lab/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Native($role,$option,$handle=''){
 $args=@($pids[$role],$option);if($handle){$args+=$handle}
 for($i=0;$i-lt6;$i++){$r=& $probe @args;if(!$LASTEXITCODE){return($r|ConvertFrom-Json)};Start-Sleep -Milliseconds 20}
 return $null
}
function Capture($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){
  $p="$root/work/lab-instances/$role/coop-status.json"
  $r[$role]=[pscustomobject]@{Bridge=(Get-Content $p -Raw|ConvertFrom-Json);Guard=(Get-Content "$p.guard.json" -Raw|ConvertFrom-Json);
   Damage=(Get-Content "$p.damage.json" -Raw|ConvertFrom-Json);Health=(Native $role '--health' $handles[$role]);PlayerHealth=(Native $role '--health' 1);Attack=(Native $role '--ai-attack' $handles[$role]);Components=(Native $role '--components' $handles[$role])}
 }
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
try{
 if(!$AfterMayhew){& "$lab/Restore-D2Lab.ps1"|Out-Null}
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{$ready=$true;foreach($role in 'host','client'){$s=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 200}}while(!$ready-and$watch.ElapsedMilliseconds-lt45000)
 if(!$ready){throw 'Native scene has not finished'}
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  $catalog=Native $role '--world-npcs';$npc=@($catalog.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($npc.Count-ne1-or!$npc[0].unique-or!$npc[0].reciprocal){throw 'Ambiguous guard'};$handles[$role]=$npc[0].handle
  Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'
  Command $role 'coop_guard_link escape 60000'
 }
 Start-Sleep -Milliseconds 500
 Command client 'coop_observer_isolation 30000'
 Command client 'setviewpos -42.9563 -117 62.7 270 0'
 Start-Sleep -Milliseconds 400
 Capture Before|Out-Null
 for($i=0;$i-lt4;$i++){
  & "$lab/Send-D2Input.ps1" -Role client -RightTrigger 255 -DurationMs 220|Out-Null
  Start-Sleep -Milliseconds 1000
  $r=Capture "Trigger-$i"
  if(!$r.client.Health-or$r.client.Health.current-le0){break}
 }
 Start-Sleep -Milliseconds 1500;Capture Settled|Out-Null
}catch{$failure=$_.Exception.Message}finally{
 [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Failure=$failure;AfterMayhew=[bool]$AfterMayhew;DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;Handles=$handles;Samples=$rows}|ConvertTo-Json -Depth 20|Set-Content "$root/work/player-strike$(if($AfterMayhew){'-after-mayhew'})-observations-BA0B.json"
 foreach($role in 'host','client'){Command $role 'coop_guard_link off';Command $role 'coop_damage_observe off'}
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0';Command client 'coop_observer_isolation off'
 & "$lab/Restore-D2Lab.ps1"|Out-Null
}
if($failure){throw $failure}
$rows|ForEach-Object{[pscustomobject]@{Stage=$_.Stage;HostHealth=$_.host.Health.current;ClientHealth=$_.client.Health.current;PlayerHealth=$_.client.PlayerHealth.current;Hits=$_.client.Damage.events.Count;GuardResult=$_.client.Guard.result}}
