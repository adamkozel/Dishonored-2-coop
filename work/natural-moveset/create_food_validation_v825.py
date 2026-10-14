from pathlib import Path
N = Path(__file__).resolve().parent
s = (N/'Test-PickupCatalogRestoration-v817.ps1').read_text(encoding='utf-8-sig')
s = s.replace('v817', 'v825').replace('Validation817','Validation825').replace('v814','v821').replace('Expected814','Expected821')
s = s.replace('shared-pickup-stack-batch-v812', 'shared-food-catalog-batch-v822').replace('catalogCount-ne24','catalogCount-ne51').replace('catalogCount=24','catalogCount=51').replace('All 24 authored currency/ammunition/stored-potion members','All 51 authored currency/ammunition/stored-potion/food members')
s = s.replace("$inventory[$r]=Q $r '--player-inventory';", """$food=Q $r '--player-food';Save "$out/food-restored-$r.json" $food
  if($food.food.health-ne56-or$food.food.mana-ne100-or$food.food.stored){throw 'Restored food outcome differs'}
  $inventory[$r]=Q $r '--player-inventory';""")
s = s.replace("if(!$state.enabled-or$state.awardedCount", """$food=Q $r '--player-food';Save "$out/food-after-refusal-$r.json" $food
  if($food.food.health-ne56-or$food.food.mana-ne100-or$food.food.stored){throw 'Refused fixture changed restorative values'}
  if(!$state.enabled-or$state.awardedCount""")
(N/'Test-PickupCatalogRestoration-v825.ps1').write_text(s,encoding='utf-8')
