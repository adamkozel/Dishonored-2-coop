$ErrorActionPreference='Stop'
$lab=Join-Path $PSScriptRoot '../outputs/d2-coop-lab';$probe="$lab/bin/d2_runtime_probe.exe";$rows=[Collections.Generic.List[object]]::new();$pids=@{}
function Command($role,$command){& "$lab/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
foreach($role in 'host','client'){$pids[$role]=(Get-Content "$PSScriptRoot/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json).pid}
function Probe($role,$option,$handle){$raw=& $probe $pids[$role] $option $handle;if($LASTEXITCODE){return @{Unavailable=$LASTEXITCODE}};return ($raw|ConvertFrom-Json)}
try{
 foreach($role in 'host','client'){Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000';Command $role 'coop_probe_follow on'}
 Command host 'coop_partner_enemy 8000'
 Command client 'setviewpos -42.5 -151.2 68.1 270 0'
 for($i=0;$i-lt20;$i++){
  $r=[ordered]@{At=(Get-Date).ToString('o')}
  foreach($role in 'host','client'){
   $bridge=Get-Content "$PSScriptRoot/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
   $r[$role]=@{Bridge=$bridge;Lease=(Get-Content "$PSScriptRoot/lab-instances/$role/coop-status.json.partner-ai.json" -Raw|ConvertFrom-Json);Targets=(Probe $role '--ai-targets' '0xb570773');Attack=(Probe $role '--ai-attack' '0xb570773');Health=(Probe $role '--health' '1');PartnerHealth=(Probe $role '--health' $bridge.probeActorHandle)}
  }
  $rows.Add([pscustomobject]$r);Start-Sleep -Milliseconds 200
 }
}finally{
 $result=[ordered]@{ObservedAt=(Get-Date).ToString('o');DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;Rows=$rows}
 foreach($role in 'host','client'){$result["$role`Damage"]=Get-Content "$PSScriptRoot/lab-instances/$role/coop-status.json.damage.json" -Raw|ConvertFrom-Json;Command $role 'coop_damage_observe off';Command $role 'coop_partner_enemy off'}
 $result|ConvertTo-Json -Depth 20|Set-Content "$PSScriptRoot/partner-perception-first-capture.json"
 & "$lab/Restore-D2Lab.ps1"|Out-Null
}
$rows|ForEach-Object{[pscustomobject]@{HostTarget=$_.host.Targets.combat.handle;HostAttention=($_.host.Targets.attention.handle -join ',');HostAttack=$_.host.Attack.flags;ClientTarget=$_.client.Targets.combat.handle;ClientHealth=$_.client.Health.current;HostLease=$_.host.Lease.result}}|Format-Table
