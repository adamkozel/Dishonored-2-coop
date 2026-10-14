$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out="$PSScriptRoot/heavy-fixture-recovery-v631"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$checks=[Collections.Generic.List[string]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 New-Item -ItemType Directory -Path $out -ErrorAction Stop|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){Copy-Item "$root/work/lab-instances/$r/coop-status.json" "$out/before-$r.json"}
 & "$root/work/PrepareShadowSword.ps1" *> "$out/restore.log"
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne"$root\work\asset-game\Dishonored2.exe"){throw 'Wrong isolated process'}
  $s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json
  if(!$s.probeActorHandle-or$s.localGameLogicState-ne1-or!$s.remoteFresh){throw 'Fresh active partner fixture absent'}
  $raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --player-inventory;if($LASTEXITCODE){throw 'Inventory read failed'}
  $after=$raw|ConvertFrom-Json;$raw|Set-Content "$out/inventory-$r.json"
  $before=Get-Content "$root/work/natural-moveset/native-heavy-emily-v630/inventory-before-$r.json" -Raw|ConvertFrom-Json
  if(Compare-Object @($before.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Restored inventory differs'}
  $raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --health 1;if($LASTEXITCODE-or($raw|ConvertFrom-Json).current-ne56){throw 'Player health differs'}
  $raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --health $s.probeActorHandle;if($LASTEXITCODE-or($raw|ConvertFrom-Json).current-ne40){throw 'Partner health differs'}
  $s|ConvertTo-Json -Depth 8|Set-Content "$out/after-$r.json"
  $checks.Add("$r fresh partner, full inventory and player/partner health restored")
 }
 foreach($f in(Get-Content "$PSScriptRoot/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $checks.Add('All14 protected save/settings hashes retained')
}catch{$failure=$_.Exception.Message}finally{
 if($owned){[ordered]@{at=(Get-Date).ToString('o');passed=(!$failure);failure=$failure;checks=$checks;scope='Recovery after630 cleanup failed with absent host avatar. Original630 failure preserved; this is separate full restoration evidence.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json";$mutex.ReleaseMutex()};$mutex.Dispose()
}
if($failure){throw $failure};'PASS_HEAVY_FIXTURE_RECOVERY'
