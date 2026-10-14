from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'Observe-Ranged.ps1').read_text(encoding='utf-8-sig')
s=s.replace("'native-ranged-corvo-gun-v1'","'native-ranged-corvo-gun-actions-v1'")
s=s.replace('coop_ranged_fixture $Kind','coop_ranged_fixture ${Kind}_loaded')
s=s.replace("'-t','12'","'-t','18'").replace('$times=@(1000,5000)','$times=@(1000,4000,7000,10000,13000,15000)')
s=s.replace('$sceneCount=0','$sceneCount=0;$fireCount=0;$emptyCount=0;$scenes=[Collections.Generic.HashSet[string]]::new()')
s=s.replace('$watch.ElapsedMilliseconds-lt9500','$watch.ElapsedMilliseconds-lt16300')
old="I $Source 255 250;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native LT press 250ms';index=$next});$next++"
new="if($next-eq3){I $Source 0 1500 X;$action='Native X hold 1500ms'}elseif($next-eq4){I $Source 0 650 X;$action='Native X draw 650ms'}else{I $Source 255 250;$action='Native LT press 250ms'};$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action=$action;index=$next});$next++"
assert old in s;s=s.replace(old,new)
s=s.replace("$native=Q $Source '--player-states';$midnight=Q $Source '--midnight'","$native=Q $Source '--player-states';$midnight=Q $Source '--midnight';$ranged=$null;try{$ranged=Q $Source \"--player-$Kind\"}catch{};foreach($scene in $midnight.instances){if($scene.asset-match'/guns/'){$null=$scenes.Add(\"$($scene.asset)|$($scene.subscene)|$($scene.sceneDurationMs)\");if($scene.asset-match('/'+$Kind+'_fire')){$fireCount++};if($scene.asset-match('/'+$Kind+'_ooa')){$emptyCount++}}}")
s=s.replace('player=$native;midnight=$midnight;component=', 'player=$native;ranged=$ranged;midnight=$midnight;component=')
s=s.replace("if(!$sceneCount){throw 'No native ranged fire/out-of-ammo scene observed'};", "if(!$fireCount-or!$emptyCount){throw 'Native fire and empty action sequence incomplete'};")
s=s.replace("$checks.Add('Normal controller casting produced native ranged fire/out-of-ammo scene observations')", "$checks.Add('Two native shots and a subsequent native empty trigger action observed');$scenes|Sort-Object|Set-Content -Encoding utf8 \"$out/scenes.txt\"")
s=s.replace("fixture grants no ammunition", "fixture restores its two-round ammunition grant")
s=s.replace("scope='Native ranged weapon investigation only;", "scope='Two-round native ranged weapon/action investigation only;")
s=s.replace('No saves or original assets modified.', 'Native projectile and impact state requires an isolated checkpoint reload after this test. No saves or original assets modified.')
s=s.replace("$checks.Add('Exact native requested character/ranged equipment acquired and selected')",'''# Inventory acquisition alone does not enter the native left-arm equipment
 # presentation. Use ordinary native holster/draw input before recording.
 I $Source 0 1500 X;$w.Restart();do{Start-Sleep -Milliseconds 100;$weapon=Q $Source "--player-$Kind";$sword=Q $Source '--player-sword'}while(($weapon.equipped-or$sword.equipped)-and$w.ElapsedMilliseconds-lt5000)
 if($weapon.equipped-or$sword.equipped){throw 'Initial native holster incomplete'}
 I $Source 0 650 X;$w.Restart();do{Start-Sleep -Milliseconds 100;$weapon=Q $Source "--player-$Kind";$sword=Q $Source '--player-sword'}while((!$weapon.equipped-or!$sword.equipped)-and$w.ElapsedMilliseconds-lt5000)
 if(!$weapon.equipped-or!$sword.equipped){throw 'Initial native draw incomplete'}
 Start-Sleep -Milliseconds 1200
 $checks.Add('Exact native ranged equipment acquired; normal controller holster/draw initializes native arm presentation')''')
s=s.replace("$checks.Add('Two native shots and a subsequent native empty trigger action observed');",'''$checks.Add('Two native shots and a subsequent native empty trigger action observed');
 $expectedVariants=if($Kind-eq'gun'){@(1,3,4,5,6)}else{@(2,7,8,9,10)}
 $observedVariants=@($rows|Where-Object {$_.player.rangedSceneValid}|ForEach-Object {$_.player.rangedScene.variant}|Sort-Object -Unique)
 foreach($variant in $expectedVariants){if($variant-notin$observedVariants){throw "Owned ranged scene reader missed variant $variant"}}
 $checks.Add('Owned native scene reader validates fire, reload, empty, equip and unequip identities and clocks');''')
(p/'Observe-RangedActions.ps1').write_text(s,encoding='utf-8')
print('Wrote bounded two-round action observation harness')

