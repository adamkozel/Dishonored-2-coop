param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-restorative-state-v820')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'}
  $cs=& "$lab/bin/d2_runtime_probe.exe" $l.pid --components 1;if($LASTEXITCODE){throw 'Components unavailable'};$components=$cs|ConvertFrom-Json;Save "$out/components-$role.json" $components
  foreach($entry in $components.components|Where-Object {$_.type-match 'arkCpnt(Health|Mana|Inventory|Attributes)'}){
   $raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --object $entry.pointer;if($LASTEXITCODE){throw 'Object unavailable'};Save "$out/$role-$($entry.type.Trim('.?@')).json" ($raw|ConvertFrom-Json)
  }
  $h=& "$lab/bin/d2_runtime_probe.exe" $l.pid --health 1;if($LASTEXITCODE){throw 'Health unavailable'};Save "$out/health-$role.json" ($h|ConvertFrom-Json)
  Save "$out/launch-$role.json" $l
 }
 Save "$out/result.json" @{passed=$true;readOnly=$true;scope='Exact current isolated player health/mana/inventory component snapshots before food adapter; no native calls or memory writes.'}
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
'PASS_RESTORATIVE_READ'
