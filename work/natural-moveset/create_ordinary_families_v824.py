from pathlib import Path

root = Path(__file__).resolve().parents[2]
p = root / 'work/natural-moveset/Test-OrdinaryStack-v816.ps1'
s = p.read_text(encoding='utf-8-sig')
s = s.replace("shared-pickup-ammo-input-v816", "shared-pickup-potion-input-v824")
s = s.replace("[ValidateSet('ammo','potion')][string]$Kind='ammo'", "[ValidateSet('ammo','potion','food')][string]$Kind='potion'")
s = s.replace("[string]$Source='client'", "[string]$Source='host'")
s = s.replace("$coin=if($Kind-eq'ammo'){'pickups_ammo_bullet_box_regular'}else{'pickups_life_potion_01_9'}", "$coin=if($Kind-eq'ammo'){'pickups_ammo_bullet_regular_1'}elseif($Kind-eq'food'){'pickups_fruit_apple_02_6'}else{'pickups_life_potion_01_9'}")
s = s.replace('OrdinaryStack816', 'OrdinaryFamilies824').replace('candidate-coin-sync-v814', 'candidate-coin-sync-v821').replace('Expected814', 'Expected821')
s = s.replace("if(!$ready){throw 'NPC fixture not ready'}", "foreach($r in 'host','client'){Save \"$out/isolation-before-approach-$r.json\" (S $r '.observer-isolation.json')}")
s = s.replace("foreach($r in 'host','client'){$iso=S $r '.observer-isolation.json';if(!$iso.ready-or$iso.remainingMs-lt12000){throw 'Finite fixture not available for ordinary input'};C $r 'coop_coin_sync on'}", """foreach($r in 'host','client'){
  Save "$out/isolation-before-input-$r.json" (S $r '.observer-isolation.json')
  $game=S $r;Save "$out/game-before-input-$r.json" $game;Save "$out/states-before-input-$r.json" (Q $r '--player-states')
  if($game.localGameLogicState-ne1-or$game.localPendingGameLogicState-ne0-or(Q $r '--health' 1).current-ne56){throw 'Ordinary interaction requires healthy native gameplay'}
  Save "$out/food-before-$r.json" (Q $r '--player-food');C $r 'coop_coin_sync on'
 }""")
s = s.replace('if($state.catalogCount-ne24', 'if($state.catalogCount-ne51')
s = s.replace('$source=$r-eq$Source', '$isSource=$r-eq$Source').replace('$receipt.nativeSource-ne$source', '$receipt.nativeSource-ne$isSource').replace('if($source)', 'if($isSource)')
s = s.replace('-or$receipt.accepted-le0', "-or($Kind-ne'food'-and$receipt.accepted-le0)")
s = s.replace("if((Money $r).currency-ne$baseline[$r].currency-or(Q $r '--health' 1).current-ne56){throw 'Ordinary stack unexpectedly changed health or money'}", """$food=Q $r '--player-food';Save "$out/food-after-$r.json" $food
  $expectedHealth=if($Kind-eq'food'){61}else{56}
  if((Money $r).currency-ne$baseline[$r].currency-or$food.food.health-ne$expectedHealth-or$food.food.mana-ne100-or$food.food.stored-ne0){throw 'Ordinary family health, mana or currency outcome differs'}
  if($Kind-eq'food'-and($receipt.family-ne6-or$receipt.foodBefore.health-ne56-or$receipt.foodAfter.healthTarget-ne61)){throw 'Ordinary food did not use validated native healing queue'}""")
s = s.replace("if($state.nativeCalls-ne$before.nativeCalls){throw 'Ordinary duplicate applied twice'}", """if($state.nativeCalls-ne$before.nativeCalls){throw 'Ordinary duplicate applied twice'}
  $food=Q $r '--player-food';Save "$out/food-duplicate-$r.json" $food
  $previous=Get-Content "$out/food-after-$r.json" -Raw|ConvertFrom-Json
  if($food.food.health-ne$previous.food.health-or$food.food.mana-ne$previous.food.mana-or$food.food.stored-ne$previous.food.stored){throw 'Duplicate altered restorative values'}""")
s = s.replace('one representative native stack family', 'one representative native pickup family')
s = s.replace('Source camera and per-entry outcomes preserved.', 'Source camera and per-entry outcomes preserved. NPC isolation is recorded as context; normal gameplay does not require the developer fixture. Family batch coverage is separate.')
(p.parent / 'Test-OrdinaryFamilies-v824.ps1').write_text(s, encoding='utf-8')
