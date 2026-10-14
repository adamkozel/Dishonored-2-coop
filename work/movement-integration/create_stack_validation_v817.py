from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-CurrencyCatalogRestoration-v807.ps1').read_text().replace('v807','v817').replace('CurrencyValidation807','PickupValidation817').replace('currency-catalog-restoration-v817','pickup-catalog-restoration-v817').replace('v804','v814').replace('Expected804','Expected814').replace('shared-currency-batch-v805','shared-pickup-stack-batch-v812').replace('catalogCount-ne19','catalogCount-ne24').replace('catalogCount=19','catalogCount=24').replace('All 19 authored currency members','All 24 authored currency/ammunition/stored-potion members')
s=s.replace("  C $r \"coop_coin_sync exercise $($prior.entries[0].name)\"", """  Save "$out/capacity-before-refusal-$r.json" (S $r '.pickup-capacity.json')
  C $r "coop_coin_sync exercise $($prior.entries[0].name)"
  C $r "coop_coin_sync capacity $($prior.entries[0].name) 10"
"""
)
s=s.replace("  if(!$state.enabled-or$state.awardedCount", """  $f=S $r '.pickup-capacity.json';$old=Get-Content "$out/capacity-before-refusal-$r.json" -Raw|ConvertFrom-Json;Save "$out/capacity-after-refusal-$r.json" $f
  if($f.active-or$f.calls-ne$old.calls-or$f.refused-ne$old.refused+1){throw 'Capacity fixture failed outside-fixture rejection'}
  $actual=Q $r '--player-inventory';Save "$out/inventory-after-refusal-$r.json" $actual
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($actual.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Developer refusal changed actual inventory'}
  if(!$state.enabled-or$state.awardedCount""")
s=s.replace('outsideFixtureNoCalls=$true;', 'outsideFixtureNoCalls=$true;capacityFixtureRefused=$true;')
s=s.replace('PASS_CURRENCY_CATALOG_RESTORATION','PASS_PICKUP_CATALOG_RESTORATION')
(N/'Test-PickupCatalogRestoration-v817.ps1').write_text(s)
print('Prepared restoration, native identity and out-of-scope fixture validation817')
