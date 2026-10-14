[CmdletBinding()]
param([switch]$DisconnectHost)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role){
 $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected lab process'}
 $path="$root/work/lab-instances/$role/coop-status.json"
 if(([DateTime]::UtcNow-(Get-Item $path).LastWriteTimeUtc).TotalSeconds-gt1.5){throw 'Stale native bridge status'}
 [pscustomobject]@{Launch=$launch;Bridge=(Get-Content $path -Raw|ConvertFrom-Json);Guard=(Get-Content "$path.guard.json" -Raw|ConvertFrom-Json);
  Body=((Get-Content "$path.guard-body.json" -Raw|ConvertFrom-Json)|Select-Object enabled,active,sent,received,rejected,applied,refused,sequence,appliedSequence,remainingMs)}
}
function NativeAi($kind){
 $s=State client;$handle='0x{0:x}'-f$s.Guard.clientHandle
 if(!$s.Guard.clientHandle){$handle=$script:guardHandle}
 for($i=0;$i-lt6;$i++){$raw=& $probe $s.Launch.pid $kind $handle;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20}
 throw 'Native AI read unavailable'
}
function Sample($stage,$hostPresent=$true){
 $row=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=$(if($hostPresent){State host}else{$null});Client=(State client);
  Decision=(NativeAi '--ai-decision');Reflex=(NativeAi '--ai-reflex');Attack=(NativeAi '--ai-attack')}
 $samples.Add($row);return $row
}
function Released($row,$queued=$false){
 if($row.Client.Guard.phase-or$row.Client.Guard.heldReferences-or$row.Client.Body.active-or$row.Client.Guard.frameOwned){throw "Control survived $($row.Stage)"}
 foreach($ai in $row.Decision,$row.Reflex){
  if($ai.pauseReferences-or(!$ai.schedulerActive-and!($queued-and$ai.pauseState-eq3))){throw "AI ownership survived $($row.Stage)"}
 }
}
function ResetFixture {
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
 foreach($role in 'host','client'){Command $role 'coop_guard_body off';Command $role 'coop_guard_link off'}
}
function WaitPlayable($role){
 $watch=[Diagnostics.Stopwatch]::StartNew();$last=0
 do{
  $s=State $role
  if($s.Bridge.localGameLogicState-eq1-and!$s.Bridge.localPendingGameLogicState){return}
  if($watch.ElapsedMilliseconds-gt6000-and$s.Bridge.localGameLogicState-in8,15-and$watch.ElapsedMilliseconds-$last-gt900){
   & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null;$last=$watch.ElapsedMilliseconds
  }
  Start-Sleep -Milliseconds 150
 }while($watch.ElapsedMilliseconds-lt25000)
 throw 'Native reload did not return to gameplay'
}
function Bind($mode='escape 60000'){
 foreach($role in 'host','client'){Command $role "coop_guard_link $mode"}
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$h=State host;$c=State client}while(($h.Guard.phase-ne7-or$c.Guard.phase-ne7)-and$watch.ElapsedMilliseconds-lt2500)
 if($h.Guard.phase-ne7-or$c.Guard.phase-ne7){throw 'Guard binding unavailable'}
 $script:guardHandle='0x{0:x}'-f$c.Guard.clientHandle
}
function VisibleBody {
 $before=(State client).Body.applied
 Command client 'setviewpos -42.9563 -112 62.7 270 0'
 foreach($role in 'host','client'){Command $role 'coop_guard_body on'}
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$c=State client}while((!$c.Body.active-or$c.Body.applied-le$before)-and$watch.ElapsedMilliseconds-lt1500)
 if(!$c.Body.active-or$c.Body.applied-le$before){throw 'No actual native animation application before lifetime test'}
 $s=Sample Active
 if($s.Client.Guard.heldReferences-ne2-or$s.Attack.schedulerActive){throw 'Native guard ownership invalid'}
 # Return the observer to the safe room before testing lease lifetime. The
 # initial visible application is separately required; culling is not a pass.
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0'
 return $s
}
try{
 ResetFixture;Bind
 $before=Sample Disabled
 Command host 'coop_guard_body on';Start-Sleep -Milliseconds 650;$hostOnly=Sample HostOnly
 if($hostOnly.Client.Body.active-or$hostOnly.Client.Body.applied-ne$before.Client.Body.applied-or$hostOnly.Client.Body.received-le$before.Client.Body.received){throw 'Client opt-in was not enforced'}
 Command host 'coop_guard_body off';Start-Sleep -Milliseconds 400;Command client 'coop_guard_body on';Start-Sleep -Milliseconds 400;$clientOnly=Sample ClientOnly
 if($clientOnly.Client.Body.active-or$clientOnly.Client.Body.applied-ne$before.Client.Body.applied){throw 'Client alone created a body lease'}
 $checks.Add('Both local animation opt-ins required despite an active guard binding')
 VisibleBody|Out-Null;Command client 'coop_guard_body off';Start-Sleep -Milliseconds 450;$disabled=Sample ClientDisabled
 if($disabled.Client.Body.active-or$disabled.Client.Guard.phase-ne7-or$disabled.Client.Guard.heldReferences-ne2){throw 'Local body disable disturbed guard ownership or retained pose'}
 $checks.Add('Local animation disable ends pose lease while the guard binding remains owned')

 ResetFixture;Bind '8000'
 foreach($role in 'host','client'){Command $role 'coop_guard_body on'}
 $before=Sample WrongModeBefore;Start-Sleep -Milliseconds 650;$after=Sample WrongModeAfter
 if($after.Client.Body.active-or$after.Client.Body.applied-ne$before.Client.Body.applied-or$after.Host.Body.sent-ne$before.Host.Body.sent){throw 'Inactive guard mode accepted combat animation'}
 $checks.Add('Inactive tutorial guard mode cannot send or apply combat guard animation')

 ResetFixture;Bind;$active=VisibleBody
 Command host 'coop_guard_stall 1000';Start-Sleep -Milliseconds 850;$stalled=Sample PoseStalled;Released $stalled
 if(!$stalled.Client.Bridge.remoteFresh-or$stalled.Client.Guard.result-ne'pose_stale'){throw 'Pose freshness was not independent of Steam traffic'}
 Start-Sleep -Milliseconds 350;if((State client).Body.applied-ne$stalled.Client.Body.applied){throw 'Stale body continued applying'}
 $checks.Add('Root pose expiry cancels body control while ordinary Steam packets remain fresh')

 foreach($role in 'client','host'){
  ResetFixture;Bind;$active=VisibleBody
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Start -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 650;$paused=Sample "Paused-$role";Released $paused ($role-eq'client')
  if((State $role).Bridge.localGameLogicState-ne2-or$paused.Host.Guard.phase){throw 'Native pause did not invalidate both bindings'}
  Start-Sleep -Milliseconds 350;if((State client).Body.applied-ne$paused.Client.Body.applied){throw 'Body application continued while paused'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 650;$resumed=Sample "Resumed-$role";Released $resumed
  if((State $role).Bridge.localGameLogicState-ne1-or$resumed.Client.Body.active){throw 'Resume reused an old animation lease'}
  $checks.Add("$role native pause releases AI references and body control; resume does not revive the old lease")
 }

 ResetFixture;Bind;$active=VisibleBody
 Command client 'LoadGame 81';Start-Sleep -Milliseconds 650;$loading=State client
 $samples.Add([pscustomobject]@{Stage='Loading';Client=$loading;Host=(State host)})
 if($loading.Guard.phase-or$loading.Guard.heldReferences-or$loading.Body.active-or$loading.Guard.released-ne$active.Client.Guard.released+2-or$loading.Guard.releaseLost-ne$active.Client.Guard.releaseLost){throw 'Reload retained guard animation or lost native AI references'}
 WaitPlayable client;ResetFixture;Bind;$rebound=VisibleBody
 if($rebound.Client.Bridge.localEpoch-eq$active.Client.Bridge.localEpoch-or$rebound.Client.Guard.binding-eq$active.Client.Guard.binding-or$rebound.Client.Guard.request-eq$active.Client.Guard.request){throw 'Replacement world reused the old animation binding'}
 $checks.Add('Reload releases controls before replacement and new gameplay requires a new epoch, request and binding')

 if($DisconnectHost){
  ResetFixture;Bind;$active=VisibleBody
  $p=Get-Process -Id $active.Host.Launch.pid
  if($p.Path-ne$active.Host.Launch.executable){throw 'Host process identity changed'}
  Stop-Process -Id $p.Id -Force
  if(!$p.WaitForExit(10000)){throw 'Host did not exit'}
  Start-Sleep -Milliseconds 1500;$lost=Sample HostDisconnected $false;Released $lost
  if($lost.Client.Bridge.remoteFresh-or$lost.Client.Guard.released-ne$active.Client.Guard.released+2-or$lost.Client.Guard.releaseLost-ne$active.Client.Guard.releaseLost){throw 'Peer loss retained native control'}
  Start-Sleep -Milliseconds 350;if((State client).Body.applied-ne$lost.Client.Body.applied){throw 'Body kept applying after peer loss'}
  $checks.Add('Actual host shutdown releases both native AI references and prevents further body application')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(Get-Process -Id $l.pid -ErrorAction SilentlyContinue){
   try{Command $role 'coop_guard_body off';Command $role 'coop_guard_link off'}catch{if(!$failure){$failure=$_.Exception.Message}}
  }
 }
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Fixed native escape-guard body lease lifetime, with independently confirmed application before each invalidation. No general NPC authority or combat/equipment completeness claim.';
 DisconnectHost=[bool]$DisconnectHost;Checks=$checks;Samples=$samples}|ConvertTo-Json -Depth 16|Set-Content "$PSScriptRoot/research/guard-body-guards-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count;DisconnectHost=[bool]$DisconnectHost}
