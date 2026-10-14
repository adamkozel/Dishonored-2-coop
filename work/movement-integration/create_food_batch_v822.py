from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-PickupStackBatch-v812.ps1').read_text().replace('v812','v822').replace('shared-pickup-stack-batch-v822','shared-food-catalog-batch-v822').replace('StackBatch812','FoodBatch822').replace('v811','v821').replace('Expected811','Expected821').replace('catalogCount-ne24','catalogCount-ne51').replace('matching 24 currency/ammo/stored-potion catalog','matching 51 currency/ammo/potion/food catalog').replace('awardedCount-ne24','awardedCount-ne51').replace('commits-ne24','commits-ne51')
s=s.replace("'-t','32'", "'-t','42'")
s=s.replace("$baseline[$r]=Money $r;", "$baseline[$r]=Money $r;Save \"$out/food-before-$r.json\" (Q $r '--player-food');")
s=s.replace("Save \"$out/health-after-$r.json\" (Q $r '--health' 1);", "Save \"$out/health-after-$r.json\" (Q $r '--health' 1);Save \"$out/food-after-$r.json\" (Q $r '--player-food');")
s=s.replace("  if((Q $r '--health' 1).current-ne56){throw 'Stored potion unexpectedly changed health'}", """  $food=Q $r '--player-food';$beforeFood=Get-Content "$out/food-before-$r.json" -Raw|ConvertFrom-Json
  if($food.food.health-ne$beforeFood.food.healthMaximum-or$food.food.mana-ne$beforeFood.food.mana-or$food.food.stored){throw 'Final native food healing/mana/storage outcome differs'}
  $heals=@($state.entries|Where-Object family -eq 6);$total=0;$zero=0
  foreach($h in $heals){
   $before=[Math]::Max($h.foodBefore.health,$h.foodBefore.healthTarget);$after=[Math]::Max($h.foodAfter.health,$h.foodAfter.healthTarget);$expectedHeal=[Math]::Min($before+$h.foodBefore.healthGrant,$h.foodBefore.healthMaximum)
   if([Math]::Abs($after-$expectedHeal)-gt.01-or$h.foodAfter.stored-ne$h.foodBefore.stored-or$h.foodAfter.mana-ne$h.foodBefore.mana){throw 'Per-food native queued outcome differs'}
   $total+=$after-$before;if($after-eq$before){$zero++}
  }
  if($heals.Count-ne27-or$total-ne$beforeFood.food.healthMaximum-$beforeFood.food.health-or$zero-ne24){throw 'Food family saturation coverage differs'}
  Save "$out/healing-audit-$r.json" @{foodCount=$heals.Count;healing=$total;fullHealthNoOps=$zero;final=$food.food}""")
s=s.replace("($entry.family-ne1-and($receipt.accepted-le0-or$receipt.after-gt$receipt.maximum))", "($entry.family-in4,14-and($receipt.accepted-le0-or$receipt.after-gt$receipt.maximum))")
s=s.replace("Save \"$out/inventory-repeat-$r.json\" (Q $r '--player-inventory')", "Save \"$out/inventory-repeat-$r.json\" (Q $r '--player-inventory');Save \"$out/food-repeat-$r.json\" (Q $r '--player-food')")
s=s.replace('the entire 24-entry currency/ammunition/stored-potion catalog','the entire 51-entry currency/ammunition/stored-potion/food catalog')
s=s.replace('unchanged unrelated inventory/health','native queued healing to maximum, full-health zero-heal food, unchanged unrelated inventory/mana')
s=s.replace('full/partial capacity behavior, or immediate food restoration.','ordinary food/ammo/potion reachability or animation quality; native health food and full-health outcomes are covered.')
s=s.replace('PASS_SHARED_PICKUP_STACK_BATCH','PASS_SHARED_FOOD_BATCH')
(N/'Test-FoodCatalogBatch-v822.ps1').write_text(s)
print('Prepared51-item food family/native Steam batch822')
