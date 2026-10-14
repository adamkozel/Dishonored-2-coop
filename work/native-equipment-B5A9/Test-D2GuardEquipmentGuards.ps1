[CmdletBinding()]
param([switch]$DisconnectHost)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$checks=[Collections.Generic.List[string]]::new();$samples=[Collections.Generic.List[object]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'}
 $p="$root/work/lab-instances/$role/coop-status.json"
 if(([DateTime]::UtcNow-(Get-Item $p).LastWriteTimeUtc).TotalSeconds-gt1.5){throw 'Stale status'}
 [pscustomobject]@{Launch=$l;Bridge=(Get-Content $p -Raw|ConvertFrom-Json);Guard=(Get-Content "$p.guard.json" -Raw|ConvertFrom-Json);Equipment=(Get-Content "$p.guard-equipment.json" -Raw|ConvertFrom-Json)}
}
function Native($role,$kind,$handle){
 $s=State $role
 for($i=0;$i-lt8;$i++){$r=& $probe $s.Launch.pid $kind $handle;if(!$LASTEXITCODE){return($r|ConvertFrom-Json)};Start-Sleep -Milliseconds 15}
 throw "Native read failed: $role $kind"
}
function Sample($stage,$hostPresent=$true){
 $c=State client
 $r=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Client=$c;Host=$(if($hostPresent){State host}else{$null});
  Native=(Native client '--sword-equipment' $script:handle);Attack=(Native client '--ai-attack' $script:handle);Health=(Native client '--health' '1')}
 $samples.Add($r);return $r
}
function WaitPlayable($role){
 $w=[Diagnostics.Stopwatch]::StartNew();$last=0
 do{
  $s=State $role
  if($s.Bridge.localGameLogicState-eq1-and!$s.Bridge.localPendingGameLogicState){return}
  if($w.ElapsedMilliseconds-gt6000-and$s.Bridge.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-$last-gt900){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null;$last=$w.ElapsedMilliseconds}
  Start-Sleep -Milliseconds 150
 }while($w.ElapsedMilliseconds-lt25000)
 throw 'Native reload did not finish'
}
function Arm {
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
 foreach($role in 'host','client'){Command $role 'coop_guard_equipment off';Command $role 'coop_guard_link off';Command $role 'coop_guard_link escape 60000'}
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 80;$h=State host;$c=State client}while(($h.Guard.phase-ne7-or$c.Guard.phase-ne7)-and$w.ElapsedMilliseconds-lt2500)
 if($h.Guard.phase-ne7-or$c.Guard.phase-ne7){throw 'Guard binding unavailable'}
 $script:handle='0x{0:x}'-f$c.Guard.clientHandle
 $before=Sample Holstered
 if($before.Native.equipped-or$before.Native.started-or$before.Native.attachedTag-ne'holster_wpn'){throw 'Initial equipment not holstered'}
 Command client 'coop_guard_equipment 2000';Start-Sleep -Milliseconds 250
 $active=Sample Armed
 if(!$active.Native.equipped-or!$active.Native.started-or$active.Native.attachedTag-ne'right_hand_wpn'-or!$active.Client.Equipment.active-or$active.Client.Equipment.observed-ne$before.Client.Equipment.observed+1-or$active.Attack.schedulerActive){throw 'No independently confirmed native draw before invalidation'}
 return $active
}
function Restored($before,$after){
 $s=$after.Client.Equipment
 if($s.active-or$s.restoring-or$s.lost-ne$before.Client.Equipment.lost-or$s.restored-ne$before.Client.Equipment.restored+1-or
    $after.Native.equipped-or$after.Native.started-or$after.Native.attachedTag-ne'holster_wpn'-or$after.Health.current-ne56){throw "Equipment restoration failed: $($after.Stage)"}
 if($after.Client.Guard.phase-or$after.Client.Guard.heldReferences){throw 'Guard ownership survived invalidation'}
}
try{
 foreach($role in 'client','host'){
  $active=Arm
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Start -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 650;$paused=Sample "Paused-$role"
  if((State $role).Bridge.localGameLogicState-ne2-or$paused.Client.Equipment.active-or$paused.Client.Guard.phase){throw 'Native pause retained active equipment ownership'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 650;$resumed=Sample "Resumed-$role";Restored $active $resumed
  $checks.Add("$role pause cancels equipment ownership and native holster restoration is confirmed after resume")
 }
 $active=Arm;Command host 'coop_guard_stall 1000';Start-Sleep -Milliseconds 850;$stale=Sample Stale;Restored $active $stale
 if(!$stale.Client.Bridge.remoteFresh-or$stale.Client.Guard.result-ne'pose_stale'){throw 'No independent pose freshness invalidation'}
 $checks.Add('Expired guard poses restore the native sword while ordinary Steam traffic remains fresh')

 $active=Arm;Command client 'LoadGame 81';Start-Sleep -Milliseconds 650;$loading=State client
 $samples.Add([pscustomobject]@{Stage='Loading';Client=$loading;Host=(State host)})
 if($loading.Equipment.active-or$loading.Guard.phase-or$loading.Guard.heldReferences){throw 'Reload retained equipment or guard ownership'}
 WaitPlayable client;$loaded=Sample Loaded
 if($loaded.Client.Bridge.localEpoch-eq$active.Client.Bridge.localEpoch-or$loaded.Client.Equipment.active-or$loaded.Client.Equipment.restoring-or$loaded.Client.Equipment.lost-ne$active.Client.Equipment.lost-or
    $loaded.Client.Equipment.restored+$loaded.Client.Equipment.retired-ne$active.Client.Equipment.restored+$active.Client.Equipment.retired+1-or$loaded.Native.equipped-or$loaded.Native.started){throw 'Old world equipment was not restored or explicitly retired'}
 $rebound=Arm
 if($rebound.Client.Guard.binding-eq$active.Client.Guard.binding-or$rebound.Client.Guard.request-eq$active.Client.Guard.request){throw 'Replacement reused old equipment binding'}
 Command client 'coop_guard_link off';Start-Sleep -Milliseconds 400;$released=Sample ReboundReleased;Restored $rebound $released
 $checks.Add('Reload ends the old equipment ownership and a replacement world requires a new binding and request')

 if($DisconnectHost){
  $active=Arm;$p=Get-Process -Id $active.Host.Launch.pid
  if($p.Path-ne$active.Host.Launch.executable){throw 'Unexpected host process'}
  Stop-Process -Id $p.Id -Force
  if(!$p.WaitForExit(10000)){throw 'Host did not exit'}
  Start-Sleep -Milliseconds 1200;$lost=Sample Disconnected $false;Restored $active $lost
  if($lost.Client.Bridge.remoteFresh){throw 'Peer still fresh'}
  $checks.Add('Actual host termination restores the native sword and releases both guard AI references')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(Get-Process -Id $l.pid -ErrorAction SilentlyContinue){try{Command $role 'coop_guard_equipment off';Command $role 'coop_guard_link off'}catch{if(!$failure){$failure=$_.Exception.Message}}}
 }
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Local fixed sword experiment lifetime. Actual native equip must be observed before each invalidation; world retirement is distinct from a confirmed restore.';DisconnectHost=[bool]$DisconnectHost;Checks=$checks;Samples=$samples}|ConvertTo-Json -Depth 15|Set-Content "$PSScriptRoot/research/guard-equipment-guards-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count;DisconnectHost=[bool]$DisconnectHost}
