from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-FoodCatalogBatch-v822.ps1').read_text().replace('shared-food-catalog-batch-v822','shared-food-recovery-v823').replace('FoodBatch822','FoodRecovery823')
a=s.index(' $plan=@(');b=s.index('\n}catch',a)
body=r'''
 $foodName=@($h.entries|Where-Object family -eq 6)[0].name
 foreach($r in 'host','client'){Save "$out/armed-$r.json" (S $r '.coin-sync.json')}
 function FoodReceipt($r){@((S $r '.coin-sync.json').entries|Where-Object name -eq $foodName)[0]}
 function AwaitFood($label){
  $watch=[Diagnostics.Stopwatch]::StartNew();do{$done=$true;foreach($r in 'host','client'){$state=S $r '.coin-sync.json';$x=FoodReceipt $r;if(!$state.enabled){throw "Food relay stopped $r $($state.result)"};if(!$x.awarded-or!$x.ack){$done=$false}};if(!$done){Start-Sleep -Milliseconds 60}}while(!$done-and$watch.ElapsedMilliseconds-lt5000)
  foreach($r in 'host','client'){Save "$out/$label-$r.json" (S $r '.coin-sync.json');Save "$out/$label-food-$r.json" (Q $r '--player-food');Save "$out/$label-entities-$r.json" (Q $r '--entities')}
  if(!$done){throw "Food did not converge $label"}
 }
 function Capture($prefix,$seconds){foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$file="$prefix$r"
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$seconds",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$file.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$file-capture.log" -RedirectStandardOutput "$out/$file-capture.stdout" -PassThru))
 }}
 Capture '' 16
 C host 'coop_potion_fixture health';$wait.Restart();do{$potion=S host '.potion-fixture.json';if($potion.active-and$potion.result-eq'native_potion_ready'){break};Start-Sleep -Milliseconds 70}while($wait.ElapsedMilliseconds-lt3000)
 if(!$potion.active-or$potion.result-ne'native_potion_ready'){throw 'Native potion fixture unavailable for asymmetric health'}
 C host 'coop_potion_fixture use_health';$wait.Restart();do{$food=Q host '--player-food';if($food.food.health-eq$food.food.healthMaximum){break};Start-Sleep -Milliseconds 80}while($wait.ElapsedMilliseconds-lt4000)
 if($food.food.health-ne$food.food.healthMaximum){throw 'Native health potion did not fill source'}
 C host 'coop_potion_fixture off';Start-Sleep -Milliseconds 150
 foreach($r in 'host','client'){Save "$out/asymmetric-before-$r.json" (Q $r '--player-food');Save "$out/potion-fixture-$r.json" (S $r '.potion-fixture.json')}
 if((Q client '--player-food').food.health-ne56){throw 'Local potion changed peer before food'}
 C host "coop_coin_sync exercise $foodName";AwaitFood 'committed';Start-Sleep -Milliseconds 800
 foreach($r in 'host','client'){
  $x=FoodReceipt $r;$before=[Math]::Max($x.foodBefore.health,$x.foodBefore.healthTarget);$after=[Math]::Max($x.foodAfter.health,$x.foodAfter.healthTarget)
  if($x.family-ne6-or$x.nativeSource-ne($r-eq'host')-or$x.exerciseCalls-ne$(if($r-eq'host'){5}else{0})-or$x.nativeCalls-ne$(if($r-eq'host'){0}else{5})-or$after-$before-ne$(if($r-eq'host'){0}else{5})){throw 'Asymmetric native food outcome differs'}
  if(@((Q $r '--entities').entities|Where-Object name -eq $foodName).Count){throw 'Food remains in native world'}
 }
 $old=S client '.coin-sync.json';C host 'coop_coin_sync duplicate';Start-Sleep -Milliseconds 700
 if((S client '.coin-sync.json').duplicates-le$old.duplicates){throw 'Duplicate food action not rejected'}
 foreach($r in 'host','client'){Save "$out/duplicate-$r.json" (S $r '.coin-sync.json');Save "$out/duplicate-food-$r.json" (Q $r '--player-food');if((Q $r '--player-food').food.health-ne$(if($r-eq'host'){70}else{61})){throw 'Food replay changed health'}}
 foreach($p in $captures){$p.WaitForExit(20000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){throw 'First capture failed'}}
'''
old=(N/'Test-PickupCatalogRecovery-v813.ps1').read_text();x=old.index(' $reloadBefore=@{}');y=old.index(" C host 'setviewpos -38.7 -155 68.1 145 0';C client",x)
reload=old[x:y]
reload=reload.replace("  C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'", """  if($cs.catalogCount-ne51-or$cs.awardedCount-or@($cs.entries|Where-Object {$_.attempted-or$_.exerciseCalls}).Count-or(Q $r '--player-food').food.health-ne56){throw 'Food catalog/checkpoint values not fresh'}
  Save "$out/reload-food-$r.json" (Q $r '--player-food')
  C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'""")
reload=reload.replace("if((Money $r).currency-ne$baseline[$r].currency-or(Target $r).Count-ne1)", "if((Money $r).currency-ne$baseline[$r].currency-or(Target $r).Count-ne1-or(Q $r '--player-food').food.health-ne56)")
body+=reload
body+=r'''
 foreach($r in 'host','client'){C $r 'setviewpos -38.7 -155 68.1 145 0'}
 Capture 'fresh-' 12
 C client "coop_coin_sync exercise $foodName";AwaitFood 'fresh';Start-Sleep -Milliseconds 900
 foreach($r in 'host','client'){
  $x=FoodReceipt $r;$food=Q $r '--player-food';Save "$out/fresh-settled-$r.json" $food
  if($food.food.health-ne61-or$x.nativeSource-ne($r-eq'client')-or$x.nativeCalls-ne$(if($r-eq'host'){5}else{0})-or$x.exerciseCalls-ne$(if($r-eq'client'){5}else{0})-or@((Q $r '--entities').entities|Where-Object name -eq $foodName).Count){throw 'Fresh food receipt retained old world or wrong source'}
 }
 $rows.Add(@{passed=$true;food=$foodName;asymmetricHealth=@(70,61);freshHealth=@(61,61);nativeApiSources=@('host','client');staleRejects=3;sharedReload=$true})
 'PASS_FOOD_RECOVERY'
'''
s=s[:a]+body+s[b:]
s=s.replace("I $r;C $r 'coop_coin_sync off'", "I $r;C $r 'coop_potion_fixture off';C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';C $r 'coop_coin_sync off'")
a=s.index("scope='Developer native API source exercise");b=s.index("'",a+7)
scope='Native food API exercise with unequal local health: full-health host gains0 and client gains5; object retired in both worlds, duplicate action rejected. One coordinated native checkpoint each rebinds51 members and fresh receipts; old food packets rejected; fresh client food gives5 each. Full baseline/protected-file restoration. Not ordinary food-input or remote-animation approval.'
s=s[:a]+"scope='"+scope+s[b:]
s=s.replace('PASS_SHARED_FOOD_BATCH','PASS_SHARED_FOOD_RECOVERY')
(N/'Test-FoodRecovery-v823.ps1').write_text(s)
print('Prepared food asymmetry and shared checkpoint recovery823')
