param([Parameter(Mandatory)][ValidatePattern('^[a-z0-9-]+$')][string]$Stage,[ValidateSet('host','client')][string]$TargetRole='client')
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe"
$records=@{}
function Health($processId,$handle){
 for($i=0;$i-lt12;$i++){$raw=& $probe $processId --health $handle;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20}
 throw "Health read failed for handle $handle"
}
foreach($role in 'host','client'){
 $dir="$root/work/lab-instances/$role"
 $launch=Get-Content "$dir/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $launch.pid).Path-ne$launch.executable-or!$launch.assetExperiment){throw 'Wrong isolated process'}
 $bridge=Get-Content "$dir/coop-status.json" -Raw|ConvertFrom-Json
 $health=Health $launch.pid 1
 $partner=Health $launch.pid $bridge.probeActorHandle
 $records[$role]=[ordered]@{Bridge=$bridge;Player=$health;Partner=$partner;Mixamo=(Get-Content "$dir/coop-status.json.mixamo.json" -Raw|ConvertFrom-Json)}
 if($role-eq$TargetRole){
  $valid=$false
  for($i=0;$i-lt30;$i++){
   $raw=& $probe $launch.pid --skeleton-final $bridge.probeActorHandle $bridge.bodyPublicationAddress
   if(!$LASTEXITCODE){$rig=$raw|ConvertFrom-Json;if($rig.finalPublicationComplete){$valid=$true;break}}
   Start-Sleep -Milliseconds 20
  }
  if($valid){$raw|Set-Content "$PSScriptRoot/$Stage-rig.json"}else{throw 'No coherent final target pose'}
 }
}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Stage=$Stage;TargetRole=$TargetRole;Dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;Roles=$records}|ConvertTo-Json -Depth 14|Set-Content "$PSScriptRoot/$Stage-state.json"
$sourceRole=if($TargetRole-eq'host'){'client'}else{'host'};$target=$records[$TargetRole]
[pscustomobject]@{Stage=$Stage;MixamoActive=$target.Mixamo.active;MixamoApplied=$target.Mixamo.applied;Phase=$target.Mixamo.phase;Player=$target.Player.current;Partner=$target.Partner.current;Stance=$records[$sourceRole].Bridge.localPlayerStance}
