$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $out="$PSScriptRoot/door-reload-failure-v738";$pin=Get-Content "$PSScriptRoot/candidate-door-catalog-v728/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Unexpected candidate'}
 foreach($r in 'host','client'){
  Copy-Item "$root/work/lab-instances/$r/engine.log" "$out/$r-engine.log"
  Copy-Item "$root/work/lab-instances/$r/loader.log" "$out/$r-loader.log"
 }
 & "$PSScriptRoot/Deploy-SourceClock.ps1" *> "$out/restart-restoration.log"
 if(!$?){throw 'Restart recovery failed'}
 $checks=@()
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  $raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --player-inventory;if($LASTEXITCODE){throw 'Inventory read failed'}
  $after=$raw|ConvertFrom-Json;$before=Get-Content "$root/work/natural-moveset/native-door-reload-client-v738/inventory-before-$r.json" -Raw|ConvertFrom-Json
  $after|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-restored-$r.json"
  if(Compare-Object @($before.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Recovered inventory mismatch'}
  $s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json
  foreach($h in @(@{handle=1;health=56},@{handle=$s.probeActorHandle;health=40})){
   $raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --health $h.handle;if($LASTEXITCODE-or($raw|ConvertFrom-Json).current-ne$h.health){throw 'Recovered health mismatch'}
  }
  $checks+=@{role=$r;pid=$l.pid;inventory=$true;health=$true;fresh=$s.remoteFresh;gameState=$s.localGameLogicState}
 }
 [ordered]@{passed=$true;at=(Get-Date).ToString('o');dll=$pin.dll;checks=$checks;protectedFiles=14;method='Restart exact isolated PIDs after failed shared-reset boundary; restore SLOT81 and native sword fixture; original files unchanged'}|ConvertTo-Json -Depth 5|Set-Content "$out/recovery.json"
 'Recovered both isolated games; inventories, health and protected hashes verified'
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
