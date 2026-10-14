param([Parameter(Mandatory)][string]$Baseline,[Parameter(Mandatory)][string]$Evidence)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$name='models/interactive/notes/note_dw_frommeagan.inventoryitem'
$m=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$checks=[Collections.Generic.List[string]]::new();$armed=[Collections.Generic.List[string]]::new()
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function Q($r){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Isolated identity differs'};$raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --player-inventory;if($LASTEXITCODE){throw 'Native inventory read failed'};return($raw|ConvertFrom-Json)}
try{
 try{$owned=$m.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path -LiteralPath $Evidence){throw 'Unique note restoration evidence required'};New-Item -ItemType Directory $Evidence|Out-Null
 foreach($r in 'host','client'){
  $expected=Get-Content "$Baseline/inventory-before-$r.json" -Raw|ConvertFrom-Json;$before=Q $r
  $before|ConvertTo-Json -Depth 12|Set-Content "$Evidence/before-$r.json"
  $a=@($expected.items|Where-Object name -eq $name);$b=@($before.items|Where-Object name -eq $name)
  if($a.Count-ne1-or$b.Count-ne1-or$a[0].quantity-notin0,1-or$b[0].quantity-notin0,1){throw 'Known scalar note not found'}
  $otherBefore=@($before.items|Where-Object name -ne $name|ForEach-Object {"$($_.name):$($_.quantity)"});$otherExpected=@($expected.items|Where-Object name -ne $name|ForEach-Object {"$($_.name):$($_.quantity)"})
  if(Compare-Object $otherExpected $otherBefore){throw 'Other inventory differs; narrow note restore cannot repair it'}
  if($a[0].quantity-ne$b[0].quantity){
   $armed.Add($r);C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
   $wait=[Diagnostics.Stopwatch]::StartNew();do{$iso=Get-Content "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json" -Raw|ConvertFrom-Json;if($iso.active-and$iso.ready-and$iso.movementFixture){break};Start-Sleep -Milliseconds 50}while($wait.ElapsedMilliseconds-lt3000)
   $iso|ConvertTo-Json -Depth 4|Set-Content "$Evidence/isolation-$r.json";if(!$iso.active-or!$iso.ready-or!$iso.movementFixture){throw 'Native isolated fixture not ready'}
   C $r $(if($a[0].quantity){'coop_note_fixture restore_present'}else{'coop_note_fixture restore_absent'})
   Start-Sleep -Milliseconds 250
   Copy-Item "$root/work/lab-instances/$r/coop-status.json.note-restore.json" "$Evidence/native-receipt-$r.json"
   $receipt=Get-Content "$Evidence/native-receipt-$r.json" -Raw|ConvertFrom-Json;if(!$receipt.confirmed-or$receipt.after-ne$a[0].quantity){throw 'Native restoration unconfirmed'}
  }
  $after=Q $r;$after|ConvertTo-Json -Depth 12|Set-Content "$Evidence/after-$r.json"
  if(Compare-Object @($expected.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete inventory restoration unconfirmed'}
  $checks.Add("$r complete inventory matches preserved baseline")
 }
 @{passed=$true;checks=$checks;scope='Exact scalar note quantity via native inventory API, solely to restore captured isolated test baseline; no shared pickup/progression claim'}|ConvertTo-Json -Depth 5|Set-Content "$Evidence/result.json"
}finally{if($owned){foreach($r in $armed){C $r 'coop_movement_fixture off'};$m.ReleaseMutex()};$m.Dispose()}
