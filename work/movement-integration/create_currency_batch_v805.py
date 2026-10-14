from pathlib import Path
R=Path(__file__).resolve().parents[2]
s=(R/'work/natural-moveset/Test-CoinCatalog-v801.ps1').read_text(encoding='utf-8-sig')
s=s.replace("[string]$Take='shared-coin-catalog-v801',[ValidateSet('batch','simultaneous')][string]$Mode='batch'", "[string]$Take='shared-currency-batch-v805'")
s=s.replace('CoinCatalog801','CurrencyBatch805').replace('v800/identity','v804/identity').replace('Expected800','Expected804')
start=s.index(' # Test data selects')
end=s.index('\n}catch{$failure',start)
body=r'''
 foreach($r in 'host','client'){C $r 'setviewpos -38.7 -155 68.1 145 0';C $r 'coop_coin_sync on'}
 $wait.Restart();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt5000);if(!$ready){throw 'Catalog handshake failed'}
 $h=S host '.coin-sync.json';$c=S client '.coin-sync.json';if($h.catalogCount-ne19-or$c.catalogCount-ne19-or$h.catalogId-ne$c.catalogId){throw 'Expected matching 19 currency pickup catalog'}
 $plan=@($h.entries|ForEach-Object {$i=[array]::IndexOf($h.entries,$_);[pscustomobject]@{name=$_.name;item=$_.item;quantity=$_.quantity;role=$(if($i%2){'client'}else{'host'})}})
 Save "$out/plan.json" $plan
 foreach($r in 'host','client'){
  Save "$out/armed-$r.json" (S $r '.coin-sync.json');$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','32','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 1000;$expected=0;$completed=0
 foreach($entry in $plan){
  foreach($r in 'host','client'){$iso=S $r '.observer-isolation.json';if(!$iso.ready-or$iso.remainingMs-lt8000){throw 'Finite fixture unavailable before batch source request'}}
  C $entry.role "coop_coin_sync exercise $($entry.name)"
  $wait.Restart();do{
   $done=$true;foreach($r in 'host','client'){$state=S $r '.coin-sync.json';$receipt=@($state.entries|Where-Object name -eq $entry.name);if(!$state.enabled){throw "Catalog stopped $r $($state.result)"};if($receipt.Count-ne1-or!$receipt[0].awarded-or!$receipt[0].ack){$done=$false}}
   if(!$done){Start-Sleep -Milliseconds 60}
  }while(!$done-and$wait.ElapsedMilliseconds-lt3500)
  foreach($r in 'host','client'){Save "$out/case-$completed-$r.json" (S $r '.coin-sync.json')}
  if(!$done){throw "Batch native source did not converge: $($entry.name)"}
  $expected+=$entry.quantity;$completed++;$rows.Add(@{name=$entry.name;quantity=$entry.quantity;role=$entry.role;source='isolated native API exercise';passed=$true});"PASS_CURRENCY_BATCH $completed value=$($entry.quantity)"
 }
 foreach($r in 'host','client'){
  $state=S $r '.coin-sync.json';Save "$out/committed-$r.json" $state;Save "$out/currency-after-$r.json" (Money $r);Save "$out/entities-after-$r.json" (Q $r '--entities')
  if(!$state.enabled-or$state.awardedCount-ne19-or$state.commits-ne19-or(Money $r).currency-ne$baseline[$r].currency+$expected){throw 'Batch total differs'}
  $entities=(Q $r '--entities').entities
  foreach($entry in $plan){$receipt=@($state.entries|Where-Object name -eq $entry.name)[0];$source=$r-eq$entry.role
   if(!$receipt.awarded-or!$receipt.ack-or$receipt.nativeSource-ne$source-or$receipt.observations-ne$(if($source){1}else{0})-or$receipt.nativeCalls-ne$(if($source){0}else{5})-or$receipt.exerciseCalls-ne$(if($source){5}else{0})-or$receipt.after-$receipt.before-ne$entry.quantity-or@($entities|Where-Object name -eq $entry.name).Count){throw "Per-entry receipt or removal differs $r $($entry.name)"}
  }
 }
 foreach($entry in $plan){C $entry.role "coop_coin_sync exercise $($entry.name)"}
 $beforeDuplicate=S client '.coin-sync.json';for($n=0;$n-lt3;$n++){C host 'coop_coin_sync duplicate';Start-Sleep -Milliseconds 120};Start-Sleep -Milliseconds 800
 if((S client '.coin-sync.json').duplicates-le$beforeDuplicate.duplicates){throw 'Steam replay not rejected'}
 foreach($r in 'host','client'){
  $state=S $r '.coin-sync.json';Save "$out/duplicate-$r.json" $state;Save "$out/hook-$r.json" (S $r '.coin-hook.json');Save "$out/currency-repeat-$r.json" (Money $r)
  $before=Get-Content "$out/committed-$r.json" -Raw|ConvertFrom-Json
  if((Money $r).currency-ne$baseline[$r].currency+$expected-or$state.nativeCalls-ne$before.nativeCalls-or($state.entries.exerciseCalls|Measure-Object -Sum).Sum-ne($before.entries.exerciseCalls|Measure-Object -Sum).Sum){throw 'Duplicate request or packet applied again'}
 }
'''
s=s[:start]+body+s[end:]
s=s.replace("scope='One shared catalog of 19 ordinary native currency pickups; data-driven mixed values and roles, independent receipts and duplicate rejection. No generic inventory/consumption claim. Full isolated restoration and protected file checks.'", "scope='Developer native API source exercise across the entire 19-entry currency catalog, alternating roles. Traverses installed reward observer and native Steam commits; validates per-entry deltas, native events, removal, duplicate no-ops and full restoration. Does not establish ordinary player reachability, container sharing, pickup-animation quality, or generic consumables.'")
(R/'work/natural-moveset/Test-CurrencyBatch-v805.ps1').write_text(s,encoding='utf-8')
print('Created data-driven native currency catalog batch test')
