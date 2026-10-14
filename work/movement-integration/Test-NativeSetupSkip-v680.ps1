$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));Set-Location $root
$out=Join-Path $PSScriptRoot 'native-setup-skip-v680';$lab=Join-Path $root 'outputs/d2-coop-lab'
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$checks=[Collections.Generic.List[string]]::new();$pids=@{}
function S($r){for($i=0;$i-lt10;$i++){try{return(Get-Content "work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 20}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if($LASTEXITCODE){throw "Probe failed $mode"};return($raw|ConvertFrom-Json)}
function Snapshot{
 $rows=@{}
 foreach($r in 'host','client'){$s=S $r;$rows[$r]=@{state=$s;character=(Q $r '--character');inventory=(Q $r '--player-inventory');playerStates=(Q $r '--player-states');health=(Q $r '--health' 1);partnerHealth=(Q $r '--health' $s.probeActorHandle)}}
 return $rows
}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 foreach($r in 'host','client'){$l=Get-Content "work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid}
 $before=Snapshot;$before|ConvertTo-Json -Depth 15|Set-Content "$out/before-full-playback-baseline.json"
 $watch=[Diagnostics.Stopwatch]::StartNew()
 & "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/skip-receipt.json" *> "$out/reset.log"
 $elapsed=$watch.ElapsedMilliseconds;Start-Sleep -Milliseconds 1200
 $after=Snapshot;$after|ConvertTo-Json -Depth 15|Set-Content "$out/after-native-skip.json"
 $receipt=Get-Content "$out/skip-receipt.json" -Raw|ConvertFrom-Json
 if(!$receipt.skipRequested-or!$receipt.skipConfirmed){throw 'Expected native shared skip confirmation'};$checks.Add('One native hold-to-skip callback and one Steam peer application, no echo or peer input')
 foreach($r in 'host','client'){
  $b=$before[$r];$a=$after[$r]
  if($a.state.localGameLogicState-ne1-or$a.state.localPendingGameLogicState-or!$a.state.remoteFresh-or!$a.state.probeFollowEnabled-or$a.state.localEpoch-eq$b.state.localEpoch){throw 'Fresh gameplay/follow/epoch required'}
  if($a.character.character-ne$b.character.character-or$a.health.current-ne$b.health.current-or$a.partnerHealth.current-ne$b.partnerHealth.current){throw 'Role or health differs'}
  if(Compare-Object @($b.inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($a.inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Full inventory differs after native skip'}
  $checks.Add("$r same character, complete inventory and player/partner health, fresh gameplay and native follow")
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save/settings changed'}};$checks.Add('All14 protected save/settings hashes retained')
}catch{$failure=$_.Exception.Message}finally{
 if($owned){[ordered]@{passed=(!$failure);failure=$failure;checks=$checks;elapsedResetMs=$elapsed;sceneElapsedMs=$receipt.sceneElapsedMs;dll=(Get-FileHash 'work/asset-game/dinput8.dll').Hash;scope='Routine isolated Mayhew fixture setup using native UI skip and existing Steam relay. Compared to the current restored full-playback baseline; no campaign-wide equivalence claim.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json";$mutex.ReleaseMutex()};$mutex.Dispose()
}
if($failure){throw $failure};Get-Content "$out/result.json" -Raw
