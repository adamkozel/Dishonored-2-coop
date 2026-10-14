[CmdletBinding()]
param([ValidateSet('All','ReloadHost','ReloadClient','DeathHost','DeathClient')][string]$Case='All')
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe'
$cases=if($Case-eq'All'){@('ReloadHost','ReloadClient','DeathHost','DeathClient')}else{@($Case)}
$checks=[Collections.Generic.List[object]]::new();$failure=$null
function Cmd($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function Json($r,$suffix=''){Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($r,$flag,$handle=''){
 $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'}
 $args=@($l.pid,$flag);if($handle){$args+=$handle}
 for($i=0;$i-lt6;$i++){$v=& $probe @args;if(!$LASTEXITCODE){return($v|ConvertFrom-Json)};Start-Sleep -Milliseconds 25}
 throw "Native observation failed: $r $flag"
}
function Observe($r,[bool]$after){
 $b=Json $r;$reset=Json $r '.reset.json';$stream=Json $r '.shadow-stream.json'
 $entities=Native $r '--entities'
 $avatars=@($entities.entities|Where-Object name -match '^d2coop_probe_')
 $name=if($r-eq'host'){'emily'}else{'corvo'}
 if($avatars.Count-ne1-or$avatars[0].definitionName-ne"models/characters/def/d2coop/doppelganger_$name.def"){throw "Wrong or duplicated clone for $r"}
 $a=Native $r '--animation' $b.probeActorHandle
 $sword=Native $r '--avatar-sword-attachment' $b.probeActorHandle
 $health=Native $r '--health' $b.probeActorHandle
 if($b.localGameLogicState-ne1-or!$b.probeFollowEnabled-or!$b.probeFollowExact-or!$b.remoteFresh-or$b.bodyStreamEnabled-or
    $a.locomotionNodes.Count-ne1-or$a.locomotionNodes[0].model-ne"models/characters/d2coop/shadow_$name.md6"-or
    $a.locomotionNodes[0].controller-eq'0x0'-or$health.current-ne40-or$stream.armed-or$stream.actionActive){throw "Clone ownership/model/equipment/health cleanup failed: $r"}
 if($after-and($reset.avatarProfile-ne'doppelganger'-or!$reset.avatarExact-or!$reset.avatarReady-or$reset.avatarPending-or!$reset.avatarAiHeld-or!$reset.avatarIdleOwned)){throw "Clone recovery readiness failed: $r"}
 [pscustomobject]@{Role=$r;Bridge=$b;Reset=$reset;Stream=$stream;Avatar=$avatars[0];Animation=$a;Sword=$sword;Health=$health}
}
try{
 foreach($caseName in $cases){
  foreach($r in 'host','client'){
   Cmd $r 'coop_checkpoint_sync off';Cmd $r 'coop_shadow_stream off';Cmd $r 'coop_shadow_action off'
   Cmd $r 'coop_probe_follow exact';Cmd $r 'coop_probe_locomotion on'
  }
  Start-Sleep -Milliseconds 350
  $before=@(Observe host $false;Observe client $false)
  switch($caseName){
   'ReloadHost'{& "$PSScriptRoot/Test-D2SharedCheckpoint.ps1" -RequestRole host -DelayRole client -Duplicate|Out-Null;$report='shared-checkpoint-duplicates-verification.json'}
   'ReloadClient'{& "$PSScriptRoot/Test-D2SharedCheckpoint.ps1" -RequestRole client -DelayRole host|Out-Null;$report='shared-checkpoint-client-delay-host-verification.json'}
   'DeathHost'{& "$PSScriptRoot/Test-D2SharedDeath.ps1" -DeathRole host -RetryRole client -HealthTelemetry|Out-Null;$report='shared-death-host-health-verification.json'}
   'DeathClient'{& "$PSScriptRoot/Test-D2SharedDeath.ps1" -DeathRole client -RetryRole host -HealthTelemetry|Out-Null;$report='shared-death-client-health-verification.json'}
  }
  $native=Get-Content "$PSScriptRoot/research/$report" -Raw|ConvertFrom-Json
  if(!$native.Passed){throw 'Underlying shared-recovery verification failed'}
  $after=@(Observe host $true;Observe client $true)
  for($i=0;$i-lt2;$i++){if($after[$i].Bridge.localEpoch-eq$before[$i].Bridge.localEpoch){throw 'Recovery reused old world epoch'}}
  $copy="doppelganger-recovery-$($caseName.ToLowerInvariant())-native.json"
  Copy-Item -LiteralPath "$PSScriptRoot/research/$report" -Destination "$PSScriptRoot/research/$copy"
  $checks.Add([pscustomobject]@{Case=$caseName;Passed=$true;NativeReport=$copy;Before=$before;After=$after})
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{Cmd $r 'coop_checkpoint_sync off';Cmd $r 'coop_shadow_stream off'}catch{}}
}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;Dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;Cases=$checks;
 Scope='Fixed canonical Dunwall SLOT82. Either reload and either natural player death/native retry; both original death bindings, one shared load, both-ready role gate, exactly one owned native clone/model with AI/idle held and initialized sword after recovery. Does not verify arbitrary campaign checkpoints or cinematic rearming.'}|ConvertTo-Json -Depth 22|Set-Content "$PSScriptRoot/research/doppelganger-shared-recovery-verification.json"
$checks|Select-Object Case,Passed,NativeReport
if($failure){throw $failure}
