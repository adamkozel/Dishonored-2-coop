from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NativeUsableApi-v830.ps1').read_text().replace('native-window-api-v830','shared-window-family-v832').replace('NativeUsable830','SharedUsable832').replace('candidate-usable-state-v829','candidate-usable-sync-v831').replace('Expected829 candidate','Expected831 candidate')
s=s.replace("'coop_coin_sync off','coop_door_sync off'","'coop_usable_sync off','coop_coin_sync off','coop_door_sync off'")
s=s.replace("'-t','24'","'-t','52'")
a=s.index(' function F($r)');b=s.index(" 'PASS_NATIVE_USABLE_INDEX_MATRIX'",a)
s=s[:a]+''' function Y($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.usable-sync.json" -Raw|ConvertFrom-Json}
 function Ready($tag){
  $w=[Diagnostics.Stopwatch]::StartNew()
  do{$ok=$true;foreach($r in 'host','client'){$y=Y $r;if(!$y.enabled-or!$y.session.enabled-or$y.catalogCount-ne4-or@($y.entries|Where-Object {!$_.enabled-or!$_.ready-or!$_.revision-or$_.applied-ne$_.revision-or$_.inFlight-or$_.awaiting}).Count){$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 70}}while(!$ok-and$w.ElapsedMilliseconds-lt7000)
  foreach($r in 'host','client'){Save "$out/sync-$tag-$r.json" (Y $r)}
  if(!$ok){throw "Shared usable family not ready $tag"}
 }
 function Pair($targetName,$target,$tag){
  $w=[Diagnostics.Stopwatch]::StartNew()
  do{$ok=$true;$pair=@{};foreach($r in 'host','client'){
    $y=Y $r;$entry=@($y.entries|Where-Object name -eq $targetName);$handle=$family[$r][$targetName];$u=Q $r '--usable' $handle
    $pair[$r]=@{native=$u;channel=$entry};$samples.Add(@{role=$r;tag=$tag;state=$u})
    if(!$y.enabled-or$entry.Count-ne1-or!$entry[0].enabled-or!$entry[0].ready-or$entry[0].target-ne$target-or$entry[0].applied-ne$entry[0].revision-or$entry[0].inFlight-or$entry[0].awaiting-or$u.current-ne$target-or$u.pending-ne-1-or$u.transitioning){$ok=$false}
  };if(!$ok){Start-Sleep -Milliseconds 70}}while(!$ok-and$w.ElapsedMilliseconds-lt7000)
  Save "$out/pair-$tag.json" $pair
  if(!$ok){throw "Shared window did not converge $targetName target$target $tag"}
 }
 $family=@{}
 foreach($r in 'host','client'){
  $family[$r]=@{};foreach($e in (Q $r '--entities').entities|Where-Object name -like 'ld_rich_small_01_sndctr_*'){$family[$r][$e.name]=$e.handle}
  Save "$out/family-$r.json" $family[$r];C $r 'coop_usable_sync session'
 }
 Ready 'initial'
 # Ordinary native input in both directions; remote side receives state only.
 I 'host' 'X' 130;I 'host';Pair $name 1 'ordinary-host-open'
 foreach($r in 'host','client'){& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X -26.5714 -Y -149.123 -Z 68.65 -Evidence "$out/aim-open-$r.json"}
 I 'client' 'X' 130;I 'client';Pair $name 0 'ordinary-client-close'
 Save "$out/ordinary-host.json" (Y 'host');Save "$out/ordinary-client.json" (Y 'client')
 # Same adapter across all four catalog entries, alternating the initiating role.
 $index=0
 foreach($window in @($family.host.Keys|Sort-Object)){
  $r=if(($index%2)-eq0){'host'}else{'client'};$index++
  C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
  $watch=[Diagnostics.Stopwatch]::StartNew();do{$iso=Get-Content "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json" -Raw|ConvertFrom-Json;if(!$iso.ready){Start-Sleep -Milliseconds 40}}while(!$iso.ready-and$watch.ElapsedMilliseconds-lt3000)
  if(!$iso.ready){throw 'Family source fixture not ready'}
  C $r "coop_usable_fixture $window 1";Pair $window 1 "batch-$index-open"
  $before=@{host=(Y 'host');client=(Y 'client')};C $r 'coop_usable_sync duplicate';Start-Sleep -Milliseconds 280
  foreach($peer in 'host','client'){$now=Y $peer;$old=@($before[$peer].entries|Where-Object name -eq $window)[0];$new=@($now.entries|Where-Object name -eq $window)[0];if($new.nativeCalls-ne$old.nativeCalls-or$new.revision-ne$old.revision){throw 'Duplicate shared request caused native call or revision'}}
  C $r "coop_usable_fixture $window 0";Pair $window 0 "batch-$index-close"
 }
 Ready 'batch-complete'
 foreach($r in 'host','client'){
  Save "$out/inventory-after-$r.json" (Q $r '--player-inventory');Save "$out/food-after-$r.json" (Q $r '--player-food')
  $after=Get-Content "$out/inventory-after-$r.json" -Raw|ConvertFrom-Json
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Usable experiment altered inventory'}
  if((Q $r '--health' 1).current-ne56){throw 'Native use altered local health'}
 }
 'PASS_SHARED_USABLE_FAMILY'
'''+s[b+len(" 'PASS_NATIVE_USABLE_INDEX_MATRIX'"):]
s=s.replace("try{I $r;C $r 'coop_movement_fixture off'}","try{I $r;C $r 'coop_usable_sync off';C $r 'coop_movement_fixture off'}")
s=s.replace('Both local roles: native indexed window open/close, repeated-target no-op and opposite request during transition. Exact owned reader, finite API fixture and actual game recordings; full inventory/health/protected restoration. No shared usable network channel yet. Ordinary input reference is828d.','Shared native window family: ordinary host open/client close; all four catalog members exercised with alternating source, exact native state convergence and duplicate envelopes; full inventory/health/protected restoration. Gameplay binding limited to the observed Dunwall family, not arbitrary quest devices.')
(N/'Test-SharedUsable-v832.ps1').write_text(s)
print('Created shared native state-list family runtime test832')
