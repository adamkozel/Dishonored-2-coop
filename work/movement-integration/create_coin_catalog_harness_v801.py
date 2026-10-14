from pathlib import Path
R=Path(__file__).resolve().parents[2]
p=R/'work/natural-moveset/Test-SharedCoin-v795.ps1'
s=p.read_text(encoding='utf-8-sig')
s=s.replace("[string]$Take='shared-coin-host-v793',[ValidateSet('host','client')][string]$Source='host'", "[string]$Take='shared-coin-catalog-v801',[ValidateSet('batch','simultaneous')][string]$Mode='batch'")
s=s.replace('SharedCoin795','CoinCatalog801').replace('v794/identity','v800/identity').replace('Expected794','Expected800')
start=s.index(' $peer=if($Source')
end=s.index('\n}catch{$failure',start)
body=r'''
 # Test data selects native objects; there are no per-item implementation paths.
 $plans=if($Mode-eq'batch'){@(
  @{role='host';name='pickups_coin_emily_lowvalue_4';quantity=1;view='-41.65 -152.9 69.5 180 0'},
  @{role='client';name='pickups_coin_emily_midvalue_1';quantity=2;view='-37.701 -171.723 68.7971 180 0'},
  @{role='client';name='pickups_coin_emily_lowvalue_7';quantity=1;view='-37.701 -171.723 68.7971 180 0'}
 )}else{@(
  @{role='host';name='pickups_coin_emily_lowvalue_4';quantity=1;view='-41.65 -152.9 69.5 180 0'},
  @{role='client';name='pickups_coin_emily_midvalue_1';quantity=2;view='-37.701 -171.723 68.7971 180 0'}
 )}
 Save "$out/plan.json" $plans
 foreach($r in 'host','client'){C $r 'setviewpos -38.7 -155 68.1 145 0';C $r 'coop_coin_sync on'}
 $wait.Restart();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt5000);if(!$ready){throw 'Catalog handshake failed'}
 $h=S host '.coin-sync.json';$c=S client '.coin-sync.json'
 if($h.catalogCount-ne11-or$c.catalogCount-ne11-or$h.catalogId-ne$c.catalogId){throw 'Expected matching 11 coin catalog'}
 foreach($r in 'host','client'){Save "$out/armed-$r.json" (S $r '.coin-sync.json')}
 function Aim($plan,$index){
  $r=$plan.role;$target=@((Q $r '--entities').entities|Where-Object name -eq $plan.name);if($target.Count-ne1){throw "Target missing $($plan.name)"}
  C $r "setviewpos $($plan.view)";Start-Sleep -Milliseconds 600
  $st=Q $r '--player-states';if($st.stance-eq0){I $r B 200;Start-Sleep -Milliseconds 500};if((Q $r '--player-states').stance-ne1){throw 'Native crouch required'}
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X $target[0].position[0] -Y $target[0].position[1] -Z ($target[0].position[2]+.03) -Evidence "$out/aim-$index-$r.json"
  foreach($role in 'host','client'){$isolation=S $role '.observer-isolation.json';Save "$out/isolation-before-$index-$role.json" $isolation;if(!$isolation.ready-or$isolation.remainingMs-lt11000){throw 'Finite isolation insufficient for next case'}}
 }
 if($Mode-eq'simultaneous'){for($n=0;$n-lt$plans.Count;$n++){Aim $plans[$n] $n}}else{Aim $plans[0] 0}
 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$seconds=if($Mode-eq'batch'){32}else{14};$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$seconds",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 1500;$expected=0;$completed=[Collections.Generic.List[object]]::new()
 for($n=0;$n-lt$plans.Count;$n++){
  $plan=$plans[$n]
  if($Mode-eq'batch'){
   if($n){Aim $plan $n};I $plan.role X 200;$expected+=$plan.quantity;$completed.Add($plan)
  }else{
   foreach($plan in $plans){I $plan.role X 200;$expected+=$plan.quantity;$completed.Add($plan)}
  }
  $wait.Restart();do{$done=$true;foreach($r in 'host','client'){if((Money $r).currency-ne$baseline[$r].currency+$expected){$done=$false}};if(!$done){Start-Sleep -Milliseconds 60}}while(!$done-and$wait.ElapsedMilliseconds-lt7000)
  foreach($r in 'host','client'){Save "$out/after-$n-$r.json" (S $r '.coin-sync.json');Save "$out/currency-after-$n-$r.json" (Money $r)}
  if(!$done){throw "Catalog native awards did not converge at case $n"};Start-Sleep -Milliseconds 500
  foreach($r in 'host','client'){
   $state=S $r '.coin-sync.json';Save "$out/committed-$n-$r.json" $state
   if(!$state.enabled-or$state.commits-ne$completed.Count-or$state.awardedCount-ne$completed.Count){throw 'Catalog total receipt mismatch'}
   $entities=(Q $r '--entities').entities
   foreach($entry in $completed){$receipt=@($state.entries|Where-Object name -eq $entry.name)
    if($receipt.Count-ne1-or!$receipt[0].awarded-or$receipt[0].revision-ne1-or$receipt[0].ack-ne1-or@($entities|Where-Object name -eq $entry.name).Count){throw "Catalog receipt/removal mismatch $r $($entry.name)"}
    if($r-eq$entry.role){if(!$receipt[0].nativeSource-or$receipt[0].observations-ne1-or$receipt[0].nativeCalls-ne0){throw 'Expected ordinary native source'}}else{if($receipt[0].nativeSource-or$receipt[0].observations-ne0-or$receipt[0].nativeCalls-ne5){throw 'Expected one five-call native recipient'}}
   }
  }
  $rows.Add(@{mode=$Mode;completed=$completed.Count;deltaEach=$expected;passed=$true});"PASS_CATALOG_CASE $n count=$($completed.Count) delta=$expected"
  if($Mode-eq'simultaneous'){break}
 }
 $beforeDuplicate=S client '.coin-sync.json';for($n=0;$n-lt3;$n++){C host 'coop_coin_sync duplicate';Start-Sleep -Milliseconds 120};Start-Sleep -Milliseconds 500
 if((S client '.coin-sync.json').duplicates-le$beforeDuplicate.duplicates){throw 'Duplicate Steam catalog action not rejected'}
 foreach($r in 'host','client'){Save "$out/duplicate-$r.json" (S $r '.coin-sync.json');Save "$out/hook-$r.json" (S $r '.coin-hook.json');Save "$out/currency-repeat-$r.json" (Money $r);if((Money $r).currency-ne$baseline[$r].currency+$expected){throw 'Duplicate catalog credit'}}
'''
s=s[:start]+body+s[end:]
# Wait for recordings before restoring NPC activity.
a="  foreach($r in 'host','client'){try{I $r;C $r 'coop_coin_sync off';C $r 'coop_movement_fixture off'}catch{}}\n"
s=s.replace(a,'')
anchor="  if($dirty){try{"
s=s.replace(anchor,a+anchor)
s=s.replace("scope='Exact authored coin shared through native Steam: ordinary source pickup, peer five-call native sequence, one delta per wallet and duplicate envelope rejection. Full isolated checkpoint/inventory/health/coin restoration and protected file checks.'", "scope='One shared catalog of 11 ordinary native coins; data-driven mixed values and roles, independent receipts and duplicate rejection. No generic inventory/consumption claim. Full isolated restoration and protected file checks.'")
(R/'work/natural-moveset/Test-CoinCatalog-v801.ps1').write_text(s,encoding='utf-8')
print('Created generic catalog test harness')
