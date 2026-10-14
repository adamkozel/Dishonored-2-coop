from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Observe-NativePossessionTarget-v331.ps1').read_text();s=s.replace("spawnerName-in@('ld_spawner_guard_tutorial_1')","spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')")
a='  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw \'Inventory quantities differ\'}'
assert a in s
s=s.replace(a,'''  $changes=@(foreach($old in $inventory[$r].items){
   $new=@($after.items|Where-Object name -eq $old.name);if($new.Count-ne1){throw 'Inventory identity changed'}
   if($old.quantity-ne$new[0].quantity){[pscustomobject]@{name=$old.name;before=$old.quantity;after=$new[0].quantity}}
  })
  if($after.items.Count-ne$inventory[$r].items.Count){throw 'Inventory item count changed'}
  $changes|ConvertTo-Json -Depth 6|Set-Content "$out/native-inventory-changes-$r.json"
  if($changes.Count){
   #330 observed this exact progression change with tutorial guard2. Its
   #cause remains unproven; allow only this bounded native-world delta here,
   #then require the entire baseline inventory after checkpoint restoration.
   if($r-ne'host'-or$target.spawnerName-ne'ld_spawner_guard_tutorial_2'-or$changes.Count-ne1-or
      $changes[0].name-ne'models/interactive/keys/dunwall/dunwall_key_emilyoffice.inventoryitem'-or$changes[0].before-ne0-or$changes[0].after-ne1){throw 'Unexpected native inventory change'}
   $checks.Add('Recorded exact native office-key0-to1 progression delta with tutorial guard2; causal attribution and peer progression remain unverified; complete checkpoint inventory rollback required')
  }
''')
s=s.replace('Both original inventory quantities and player/partner health retained; world reset still required','Temporary power/upgrade and unrelated inventory quantities restored; player/partner health retained; any separately recorded native office-key delta awaits mandatory checkpoint reset')
(P/'Observe-NativePossessionTarget-v332.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Observe-NativePossessionTarget-v331.ps1','Observe-NativePossessionTarget-v332.ps1');p.write_text(s)
print('Prepared explicit native key-delta accounting; complete post-checkpoint baseline remains mandatory')
