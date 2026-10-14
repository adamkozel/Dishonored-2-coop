from pathlib import Path
R=Path(__file__).resolve().parents[2]
s=(R/'work/natural-moveset/Test-CurrencyBatch-v805.ps1').read_text(encoding='utf-8-sig')
s=s[:s.index('\ntry{')].replace("[string]$Take='shared-currency-batch-v805'", "[string]$Take='currency-catalog-restoration-v807'")
s+=r'''
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='CurrencyValidation807';state='running';take=$Take}
 $pin=Get-Content "$root/work/movement-integration/candidate-coin-sync-v804/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected804 candidate'}
 $prior=Get-Content "$PSScriptRoot/shared-currency-batch-v805/armed-host.json" -Raw|ConvertFrom-Json
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  $st=S $r;$h=Q $r '--health' 1;$a=Q $r '--health' $st.probeActorHandle;$char=Q $r '--character';$iso=S $r '.observer-isolation.json'
  if($st.localGameLogicState-ne1-or$st.localPendingGameLogicState-or!$st.probeFollowEnabled-or$iso.active-or$h.current-ne56-or$a.current-ne40-or$char.character-ne$(if($r-eq'host'){1}else{2})){throw 'Final native baseline differs'}
  $inventory[$r]=Q $r '--player-inventory';$before=Get-Content "$PSScriptRoot/shared-currency-batch-v805/inventory-before-$r.json" -Raw|ConvertFrom-Json
  if(Compare-Object @($before.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Full inventory baseline differs'}
  $baseline[$r]=Money $r;if($baseline[$r].currency){throw 'Currency baseline differs'}
  Save "$out/baseline-$r.json" @{launch=$l;health=$h;avatar=$a;character=$char;inventory=$inventory[$r];currency=$baseline[$r];isolation=$iso;bridge=$st}
  C $r 'coop_coin_sync on'
 }
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 60}}while(!$ready-and$wait.ElapsedMilliseconds-lt5000);if(!$ready){throw 'Restored catalog handshake failed'}
 foreach($r in 'host','client'){
  $state=S $r '.coin-sync.json';Save "$out/catalog-restored-$r.json" $state
  if($state.catalogCount-ne19-or$state.catalogId-ne$prior.catalogId-or$state.awardedCount-or$state.nativeCalls-or$state.commits-or@($state.entries|Where-Object {$_.attempted-or$_.exerciseCalls}).Count){throw 'Restored catalog reused receipts or changed authored members'}
  C $r "coop_coin_sync exercise $($prior.entries[0].name)"
 }
 Start-Sleep -Milliseconds 700
 foreach($r in 'host','client'){
  $state=S $r '.coin-sync.json';Save "$out/outside-fixture-$r.json" $state;Save "$out/currency-after-refusal-$r.json" (Money $r)
  if(!$state.enabled-or$state.awardedCount-or$state.nativeCalls-or$state.commits-or@($state.entries|Where-Object {$_.attempted-or$_.exerciseCalls}).Count-or(Money $r).currency){throw 'Developer exercise mutated without finite fixture'}
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $page=Invoke-WebRequest 'http://127.0.0.1:8769/review.html' -TimeoutSec 15;if($page.StatusCode-ne200){throw 'Review dashboard unavailable'}
 Save "$out/runtime.json" @{passed=$true;dashboardHttp=$page.StatusCode;dashboardBytes=$page.RawContentLength;catalogCount=19;catalogId=$prior.catalogId;money=0;fullInventoryRestored=$true;health=@(56,40);roles=@(1,2);outsideFixtureNoCalls=$true;protectedFiles=14}
}catch{$failure=$_.Exception.Message}finally{
 if($owned){foreach($r in 'host','client'){try{C $r 'coop_coin_sync off';Save "$out/disabled-$r.json" (S $r '.coin-sync.json')}catch{$failure=($failure+'; disable failed').TrimStart('; ')}}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;scope='All 19 authored currency members rebound with empty receipts after restored checkpoint; complete baseline and outside-fixture zero-call exercise refusal; native roles, health, follow, released fixture and protected files checked.'}
  Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='CurrencyValidation807';state='finished';take=$Take};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_CURRENCY_CATALOG_RESTORATION'
'''
(R/'work/natural-moveset/Test-CurrencyCatalogRestoration-v807.ps1').write_text(s,encoding='utf-8')
print('Created catalog restoration and scope validation')
