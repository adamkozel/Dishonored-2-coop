param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='pickup-catalog-restoration-v825')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$inventory=@{};$baseline=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new();$coin='pickups_coin_emily_lowvalue_4'
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 20}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Read failed $r $mode"}
function Money($r){
 $h=Q $r '--health' 1;if($h.logic-ne1){throw 'Currency requires native gameplay'}
 $cs=Q $r '--components' 1;$i=@($cs.components|Where-Object type -eq '.?AVarkCpntInventory@@');if($i.Count-ne1-or$i[0].owner-ne$h.actor){throw 'Currency inventory ownership mismatch'}
 $o=Q $r '--object' $i[0].pointer;$owner=@($o.words|Where-Object offset -eq '0x10');$w=@($o.words|Where-Object offset -eq '0x168')
 if($o.type-ne'.?AVarkCpntInventory@@'-or$owner.Count-ne1-or$owner[0].word-ne$h.actor-or$w.Count-ne1-or(Q $r '--health' 1).actor-ne$h.actor){throw 'Currency inventory incarnation changed'}
 $n=[Convert]::ToUInt64($w[0].word.Substring(2),16);return @{currency=[long]($n-shr32);inventory=$i[0].pointer;raw=$w[0];actor=$h.actor}
}
function Target($r){@((Q $r '--entities').entities|Where-Object name -eq $coin)}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='PickupValidation825';state='running';take=$Take}
 $pin=Get-Content "$root/work/movement-integration/candidate-coin-sync-v821/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected821 candidate'}
 $prior=Get-Content "$PSScriptRoot/shared-food-catalog-batch-v822/armed-host.json" -Raw|ConvertFrom-Json
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  $st=S $r;$h=Q $r '--health' 1;$a=Q $r '--health' $st.probeActorHandle;$char=Q $r '--character';$iso=S $r '.observer-isolation.json'
  if($st.localGameLogicState-ne1-or$st.localPendingGameLogicState-or!$st.probeFollowEnabled-or$iso.active-or$h.current-ne56-or$a.current-ne40-or$char.character-ne$(if($r-eq'host'){1}else{2})){throw 'Final native baseline differs'}
  $food=Q $r '--player-food';Save "$out/food-restored-$r.json" $food
  if($food.food.health-ne56-or$food.food.mana-ne100-or$food.food.stored){throw 'Restored food outcome differs'}
  $inventory[$r]=Q $r '--player-inventory';$before=Get-Content "$PSScriptRoot/shared-food-catalog-batch-v822/inventory-before-$r.json" -Raw|ConvertFrom-Json
  if(Compare-Object @($before.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Full inventory baseline differs'}
  $baseline[$r]=Money $r;if($baseline[$r].currency){throw 'Currency baseline differs'}
  Save "$out/baseline-$r.json" @{launch=$l;health=$h;avatar=$a;character=$char;inventory=$inventory[$r];currency=$baseline[$r];isolation=$iso;bridge=$st}
  C $r 'coop_coin_sync on'
 }
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 60}}while(!$ready-and$wait.ElapsedMilliseconds-lt5000);if(!$ready){throw 'Restored catalog handshake failed'}
 foreach($r in 'host','client'){
  $state=S $r '.coin-sync.json';Save "$out/catalog-restored-$r.json" $state
  if($state.catalogCount-ne51-or$state.catalogId-ne$prior.catalogId-or$state.awardedCount-or$state.nativeCalls-or$state.commits-or@($state.entries|Where-Object {$_.attempted-or$_.exerciseCalls}).Count){throw 'Restored catalog reused receipts or changed authored members'}
  Save "$out/capacity-before-refusal-$r.json" (S $r '.pickup-capacity.json')
  C $r "coop_coin_sync exercise $($prior.entries[0].name)"
  C $r "coop_coin_sync capacity $($prior.entries[0].name) 10"

 }
 Start-Sleep -Milliseconds 700
 foreach($r in 'host','client'){
  $state=S $r '.coin-sync.json';Save "$out/outside-fixture-$r.json" $state;Save "$out/currency-after-refusal-$r.json" (Money $r)
  $f=S $r '.pickup-capacity.json';$old=Get-Content "$out/capacity-before-refusal-$r.json" -Raw|ConvertFrom-Json;Save "$out/capacity-after-refusal-$r.json" $f
  if($f.active-or$f.calls-ne$old.calls-or$f.refused-ne$old.refused+1){throw 'Capacity fixture failed outside-fixture rejection'}
  $actual=Q $r '--player-inventory';Save "$out/inventory-after-refusal-$r.json" $actual
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($actual.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Developer refusal changed actual inventory'}
  $food=Q $r '--player-food';Save "$out/food-after-refusal-$r.json" $food
  if($food.food.health-ne56-or$food.food.mana-ne100-or$food.food.stored){throw 'Refused fixture changed restorative values'}
  if(!$state.enabled-or$state.awardedCount-or$state.nativeCalls-or$state.commits-or@($state.entries|Where-Object {$_.attempted-or$_.exerciseCalls}).Count-or(Money $r).currency){throw 'Developer exercise mutated without finite fixture'}
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $page=Invoke-WebRequest 'http://127.0.0.1:8769/review.html' -TimeoutSec 15;if($page.StatusCode-ne200){throw 'Review dashboard unavailable'}
 Save "$out/runtime.json" @{passed=$true;dashboardHttp=$page.StatusCode;dashboardBytes=$page.RawContentLength;catalogCount=51;catalogId=$prior.catalogId;money=0;fullInventoryRestored=$true;health=@(56,40);roles=@(1,2);outsideFixtureNoCalls=$true;capacityFixtureRefused=$true;protectedFiles=14}
}catch{$failure=$_.Exception.Message}finally{
 if($owned){foreach($r in 'host','client'){try{C $r 'coop_coin_sync off';Save "$out/disabled-$r.json" (S $r '.coin-sync.json')}catch{$failure=($failure+'; disable failed').TrimStart('; ')}}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;scope='All 51 authored currency/ammunition/stored-potion/food members rebound with empty receipts after restored checkpoint; complete baseline and outside-fixture zero-call exercise refusal; native roles, health, follow, released fixture and protected files checked.'}
  Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='PickupValidation825';state='finished';take=$Take};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_PICKUP_CATALOG_RESTORATION'
