from pathlib import Path
R=Path(__file__).resolve().parents[2]
W=R/'work/movement-integration'
N=R/'work/natural-moveset'
s=(W/'stage_coin_sync_v804.py').read_text()
s=s.replace('v804','v811').replace("P=W/'candidate-coin-sync-v800'", "P=W/'candidate-coin-sync-v804'")
s=s.replace('Bounded ordinary coin catalog with authored names and declared quantities, independent receipts, serialized native wallet transactions, host commits, menu retention and checkpoint rebind.', 'Bounded shared currency/ammo/stored-potion catalog with native capacity-aware outcomes, per-item receipts, serialized native reward transactions, Steam host commits and checkpoint rebind; packet version3.')
(W/'stage_coin_sync_v811.py').write_text(s)
s=(W/'Deploy-CoinSync-v804.ps1').read_text().replace('v804','v811').replace('Deploy804','Deploy811')
(W/'Deploy-CoinSync-v811.ps1').write_text(s)
s=(N/'Test-CurrencyBatch-v805.ps1').read_text().replace('v805','v812').replace('CurrencyBatch805','StackBatch812').replace('v804','v811').replace('Expected804','Expected811')
s=s.replace('shared-currency-batch-v812','shared-pickup-stack-batch-v812')
s=s.replace('catalogCount-ne19','catalogCount-ne24').replace('matching 19 currency pickup catalog','matching 24 currency/ammo/stored-potion catalog')
s=s.replace('quantity=$_.quantity;role=', 'quantity=$_.quantity;family=$_.family;role=')
s=s.replace('$expected+=$entry.quantity;', 'if($entry.family-eq1){$expected+=$entry.quantity};')
s=s.replace('quantity=$entry.quantity;role=$entry.role;', 'quantity=$entry.quantity;family=$entry.family;role=$entry.role;')
s=s.replace('PASS_CURRENCY_BATCH','PASS_PICKUP_BATCH').replace('awardedCount-ne19','awardedCount-ne24').replace('commits-ne19','commits-ne24')
s=s.replace('-or$receipt.after-$receipt.before-ne$entry.quantity', '-or$receipt.family-ne$entry.family-or($entry.family-eq1-and$receipt.accepted-ne$entry.quantity)-or($entry.family-ne1-and($receipt.accepted-le0-or$receipt.after-gt$receipt.maximum))')
s=s.replace("Save \"$out/currency-after-$r.json\" (Money $r);", "Save \"$out/currency-after-$r.json\" (Money $r);Save \"$out/inventory-after-$r.json\" (Q $r '--player-inventory');Save \"$out/health-after-$r.json\" (Q $r '--health' 1);")
s=s.replace("Save \"$out/currency-repeat-$r.json\" (Money $r)", "Save \"$out/currency-repeat-$r.json\" (Money $r);Save \"$out/inventory-repeat-$r.json\" (Q $r '--player-inventory')\n  $afterInv=Get-Content \"$out/inventory-after-$r.json\" -Raw|ConvertFrom-Json;$repeatInv=Get-Content \"$out/inventory-repeat-$r.json\" -Raw|ConvertFrom-Json\n  if(Compare-Object @($afterInv.items|ForEach-Object {\"$($_.name):$($_.quantity)\"}) @($repeatInv.items|ForEach-Object {\"$($_.name):$($_.quantity)\"})){throw 'Replay altered inventory'}")
s=s.replace("  $entities=(Q $r '--entities').entities", """  $entities=(Q $r '--entities').entities
  $actual=Get-Content "$out/inventory-after-$r.json" -Raw|ConvertFrom-Json
  foreach($item in $inventory[$r].items){
   $matching=@($state.entries|Where-Object {$_.family-ne1-and$_.item-eq$item.name});$delta=0;if($matching.Count){$delta=($matching.accepted|Measure-Object -Sum).Sum}
   $found=@($actual.items|Where-Object name -eq $item.name);if($found.Count-ne1-or$found[0].quantity-ne$item.quantity+$delta){throw "Inventory outcome differs $r $($item.name)"}
  }
  if((Q $r '--health' 1).current-ne56){throw 'Stored potion unexpectedly changed health'}""")
s=s.replace('the entire 19-entry currency catalog','the entire 24-entry currency/ammunition/stored-potion catalog')
s=s.replace('per-entry deltas, native events, removal, duplicate no-ops and full restoration','per-entry observed native grants, bounded capacity, unchanged unrelated inventory/health, native events, removal, duplicate no-ops and full restoration')
s=s.replace('or generic consumables.','full/partial capacity behavior, or immediate food restoration.')
s=s.replace("'PASS_SHARED_COIN'", "'PASS_SHARED_PICKUP_STACK_BATCH'")
(N/'Test-PickupStackBatch-v812.ps1').write_text(s)
print('Prepared candidate811 deployment and mixed-family batch812')
