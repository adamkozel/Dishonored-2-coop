[CmdletBinding()]
param([switch]$StreamArm)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function State($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.avatar-sword.json" -Raw|ConvertFrom-Json}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $arguments=@($pids[$role],$option);if($handle){$arguments+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @arguments;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native avatar sword read unavailable: $role $option"
}
function Capture($stage){
 $row=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');State=(State client);Bridge=(Bridge client);Sword=(Native client '--avatar-sword' $handle);Attachment=$(if((Bridge client).localGameLogicState-eq1){Native client '--avatar-sword-attachment' $handle}else{$null})}
 $samples.Add($row);return $row
}
function Arm {
 $before=State client;Command client 'coop_avatar_sword 2000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 70;$s=State client}while((!$s.active-or$s.observed-le$before.observed)-and$w.ElapsedMilliseconds-lt1000)
 if(!$s.active-or$s.started-ne$before.started+1-or$s.observed-ne$before.observed+1){throw "Corvo's native sword did not equip: $($s.result)"}
 return $s
}
function Restored($stage){
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 90;$s=State client}while(($s.active-or$s.restoring)-and$w.ElapsedMilliseconds-lt3500)
 $r=Capture $stage
 if($s.active-or$s.restoring-or$s.lost-ne$before.State.lost-or$r.Sword.equipped-or$r.Sword.started-or$r.Sword.attachedTag-ne'holster_wpn'-or$r.Sword.weaponHandle-ne$before.Sword.weaponHandle){throw 'Native Corvo holster restoration failed'}
 return $r
}
try{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  Command $role 'coop_avatar_sword off';Command $role 'coop_probe_arm off';Command $role 'coop_probe_body off';Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'
 }
 Command host 'setviewpos -33 -153 68.1 90 0';Command client 'setviewpos -33 -148 68.1 270 8'
 foreach($role in 'host','client'){Command $role 'coop_probe_follow exact'}
 Start-Sleep -Milliseconds 500;$handle=(Bridge client).probeActorHandle;$before=Capture Before
 $hostBefore=State host;Command host 'coop_avatar_sword 2000';Start-Sleep -Milliseconds 250;$hostRefused=State host
 if($hostRefused.active-or$hostRefused.started-ne$hostBefore.started-or$hostRefused.refused-le$hostBefore.refused){throw 'Emily instance accepted the Corvo-only equipment experiment'}
 $checks.Add('Corvo-only equipment scope refuses the other owned character')
 if($StreamArm){foreach($role in 'host','client'){Command $role 'coop_probe_arm on'};Start-Sleep -Milliseconds 500}
 $armed=Arm
 if($StreamArm){& "$PSScriptRoot/Send-D2Input.ps1" -Role host -RightTrigger 255 -DurationMs 220|Out-Null}
 for($i=0;$i-lt10;$i++){
  Capture Drawn|Out-Null
  if($i-eq1){Command client 'coop_avatar_sword 2000';$dup=State client;if($dup.started-ne$armed.started-or$dup.remainingMs-ge$armed.remainingMs){throw 'Duplicate equipment command renewed its fixed deadline'}}
  Start-Sleep -Milliseconds 60
 }
 $expired=Restored Expired
 if($expired.State.restored-ne$before.State.restored+1-or$expired.State.released-ne$before.State.released+1){throw 'Expiry did not perform one native holster restoration'}
 $drawn=@($samples|Where-Object {$_.Stage-eq'Drawn'-and$_.Sword.equipped-and$_.Sword.started-and$_.Sword.attachedTag-eq'right_hand_wpn'})
 if($drawn.Count-lt4){throw 'Too few confirmed native hand attachments'}
 $checks.Add('Native equip starts the existing sword and attaches it to the hand; fixed expiry returns the same entity to its holster without changing inventory quantity')
 $again=Arm;Command client 'coop_avatar_sword off';$cancelled=Restored Cancelled
 $checks.Add('Explicit cancellation restores the original native holster')
 $pausedArm=Arm;& "$PSScriptRoot/Send-D2Input.ps1" -Role client -Button Start -DurationMs 200|Out-Null
 Start-Sleep -Milliseconds 500;$paused=Capture Paused
 if((Bridge client).localGameLogicState-ne2-or$paused.State.active){throw 'Native pause retained equipment ownership'}
 & "$PSScriptRoot/Send-D2Input.ps1" -Role client -Button A -DurationMs 200|Out-Null
 $resumed=Restored Resumed
 $checks.Add('Native pause cancels the lease, and deferred native cleanup confirms the holster after resume')
 foreach($role in 'host','client'){
  if((Native $role '--health' 1).current-ne56){throw 'Equipment experiment damaged a real player'}
  $damage=Get-Content "$root/work/lab-instances/$role/coop-status.json.damage.json" -Raw|ConvertFrom-Json
  if(@($damage.events).Count){throw 'Equipment-only experiment unexpectedly caused native damage'}
 }
 $checks.Add('Both real players retain checkpoint health and no native damage event is introduced')
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){Command $role 'coop_avatar_sword off';Command $role 'coop_probe_arm off';Command $role 'coop_damage_observe off'}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;StreamArm=[bool]$StreamArm;
 Scope='Local two-second equip/restore of the owned Corvo NPC existing tutorial sword, optionally alongside independently streamed right-arm motion. No remote equipment request, Emily weapon or damage replication claim. Render attachment requires independent verification.';Checks=$checks;Samples=$samples}|ConvertTo-Json -Depth 22|Set-Content "$PSScriptRoot/research/avatar-sword$(if($StreamArm){'-arms'})-observations.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$samples.Count}
