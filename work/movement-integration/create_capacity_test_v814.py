from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
A=W/'candidate-coin-sync-v811'
p=json.loads((A/'identity.json').read_text())
assert json.loads((N/'shared-pickup-stack-batch-v812/result.json').read_text())['passed']
assert json.loads((N/'shared-pickup-recovery-v813/result.json').read_text())['passed']
p.update(runtimeVerified=True,runtimeEvidence=['work/natural-moveset/shared-pickup-stack-batch-v812','work/natural-moveset/shared-pickup-recovery-v813'],runtimeScope='24-entry currency/ammo/stored-health-potion API batch with native rewards/removal/duplicate no-ops; ordinary coin source, shared checkpoint rebind and post-reload peer input. Full baseline/protected-file restoration. Capacity boundaries and ordinary ammo/potion input still pending.')
(A/'identity.json').write_text(json.dumps(p,indent=2))
s=(W/'stage_coin_sync_v811.py').read_text().replace('v811','v814').replace("P=W/'candidate-coin-sync-v804'","P=W/'candidate-coin-sync-v811'")
s=s.replace('packet version3.', 'packet version3; finite native inventory capacity fixture for isolated edge tests.')
(W/'stage_coin_sync_v814.py').write_text(s)
s=(W/'Deploy-CoinSync-v811.ps1').read_text().replace('v811','v814').replace('Deploy811','Deploy814')
(W/'Deploy-CoinSync-v814.ps1').write_text(s)
s=(N/'Test-PickupStackBatch-v812.ps1').read_text().replace('v812','v815').replace('shared-pickup-stack-batch-v815','shared-pickup-capacity-v815').replace('StackBatch812','PickupCapacity815').replace('v811','v814').replace('Expected811','Expected814')
start=s.index(' $plan=@(');end=s.index('\n}catch',start)
s=s[:start]+r'''
 $plan=@(
  @{name='pickups_ammo_bullet_box_regular';family=4;source='host';fixtureRole='client';setup=9;recipientDelta=1},
  @{name='pickups_ammo_bullet_regular';family=4;source='host';fixtureRole='client';setup=10;recipientDelta=0},
  @{name='pickups_life_potion_01_9';family=14;source='client';fixtureRole='host';setup=10;recipientDelta=0}
 )
 Save "$out/plan.json" $plan
 foreach($r in 'host','client'){
  Save "$out/armed-$r.json" (S $r '.coin-sync.json');$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','20','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 700;$completed=0
 foreach($entry in $plan){
  C $entry.fixtureRole "coop_coin_sync capacity $($entry.name) $($entry.setup)"
  $wait.Restart();do{$f=S $entry.fixtureRole '.pickup-capacity.json';if($f.active-and$f.result-eq'native_capacity_ready'){break};Start-Sleep -Milliseconds 50}while($wait.ElapsedMilliseconds-lt2500)
  Save "$out/setup-$completed.json" $f;if(!$f.active-or$f.result-ne'native_capacity_ready'){throw 'Native capacity fixture unavailable'}
  foreach($r in 'host','client'){Save "$out/before-$completed-$r.json" (Q $r '--player-inventory')}
  C $entry.source "coop_coin_sync exercise $($entry.name)"
  $wait.Restart();do{
   $done=$true;foreach($r in 'host','client'){$state=S $r '.coin-sync.json';$receipt=@($state.entries|Where-Object name -eq $entry.name);if(!$state.enabled){throw "Catalog stopped $r $($state.result)"};if($receipt.Count-ne1-or!$receipt[0].awarded-or!$receipt[0].ack){$done=$false}}
   if(!$done){Start-Sleep -Milliseconds 50}
  }while(!$done-and$wait.ElapsedMilliseconds-lt3500)
  if(!$done){throw "Capacity case did not converge: $($entry.name)"}
  $case=@{}
  foreach($r in 'host','client'){
   $state=S $r '.coin-sync.json';Save "$out/committed-$completed-$r.json" $state
   $receipt=@($state.entries|Where-Object name -eq $entry.name)[0];$source=$r-eq$entry.source
   if(!$receipt.awarded-or!$receipt.ack-or$receipt.family-ne$entry.family-or$receipt.nativeSource-ne$source-or$receipt.nativeCalls-ne$(if($source){0}else{5})-or$receipt.exerciseCalls-ne$(if($source){5}else{0})-or$receipt.observations-ne$(if($source){1}else{0})){throw 'Wrong receipt ownership or native call count'}
   if($source){if($receipt.accepted-le0){throw 'Source did not gain native inventory'}}else{if($receipt.before-ne$entry.setup-or$receipt.after-ne10-or$receipt.accepted-ne$entry.recipientDelta){throw 'Capacity clamp or zero-grant differs'}}
   $entities=Q $r '--entities';Save "$out/entities-$completed-$r.json" $entities
   if(@($entities.entities|Where-Object name -eq $entry.name).Count){throw 'Consumed pickup remains registered'}
   $after=Q $r '--player-inventory';Save "$out/after-$completed-$r.json" $after;$before=Get-Content "$out/before-$completed-$r.json" -Raw|ConvertFrom-Json
   foreach($item in $before.items){$found=@($after.items|Where-Object name -eq $item.name);$delta=if($item.name-eq$receipt.item){$receipt.accepted}else{0};if($found.Count-ne1-or$found[0].quantity-ne$item.quantity+$delta){throw 'Capacity case changed unrelated inventory'}}
   if((Money $r).currency-ne$baseline[$r].currency-or(Q $r '--health' 1).current-ne56){throw 'Capacity test altered health or money'}
   $case[$r]=$receipt
  }
  C $entry.source "coop_coin_sync exercise $($entry.name)";C host 'coop_coin_sync duplicate';Start-Sleep -Milliseconds 600
  foreach($r in 'host','client'){
   $state=S $r '.coin-sync.json';Save "$out/duplicate-$completed-$r.json" $state;$receipt=@($state.entries|Where-Object name -eq $entry.name)[0]
   if($receipt.nativeCalls-ne$case[$r].nativeCalls-or$receipt.exerciseCalls-ne$case[$r].exerciseCalls-or$receipt.after-ne$case[$r].after){throw 'Capacity duplicate applied again'}
   $repeat=Q $r '--player-inventory';$before=Get-Content "$out/after-$completed-$r.json" -Raw|ConvertFrom-Json
   if(Compare-Object @($before.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($repeat.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Replay changed actual capacity inventory'}
  }
  C $entry.fixtureRole 'coop_coin_sync capacity off'
  $wait.Restart();do{$f=S $entry.fixtureRole '.pickup-capacity.json';if(!$f.active-and$f.result-eq'native_quantity_restored'){break};Start-Sleep -Milliseconds 50}while($wait.ElapsedMilliseconds-lt2500)
  Save "$out/restored-$completed.json" $f;if($f.active-or$f.result-ne'native_quantity_restored'){throw 'Capacity fixture did not restore original quantity'}
  $rows.Add(@{plan=$entry;receipts=$case;passed=$true});$completed++;"PASS_PICKUP_CAPACITY $completed"
 }
 foreach($r in 'host','client'){Save "$out/committed-$r.json" (S $r '.coin-sync.json');Save "$out/hook-$r.json" (S $r '.coin-hook.json')}
''' +s[end:]
s=s.replace("I $r;C $r 'coop_coin_sync off'", "I $r;C $r 'coop_coin_sync capacity off';C $r 'coop_coin_sync off'")
scope="Native API source/recipient capacity tests over Steam: partial ammo 9-to-10, full ammo and full stored health potion accept zero but retire the world object and acknowledge once; per-item native ownership, unrelated inventory/health/money, duplicates and native fixture restoration. Checkpoint cleanup restores full isolated baseline. Not ordinary ammo/potion input or cross-account online proof."
a=s.index("scope='Developer native API source exercise");b=s.index("'",a+7)
s=s[:a]+"scope='"+scope+s[b:]
s=s.replace('PASS_SHARED_PICKUP_STACK_BATCH','PASS_SHARED_PICKUP_CAPACITY')
(N/'Test-PickupCapacity-v815.ps1').write_text(s)
print('Prepared capacity archive/deploy814 and runtime matrix815')
