from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeCancel-v227.ps1').read_text(encoding='utf-8-sig')
key=" $initialCatalog=Q client '--world-npcs'"
insert=''' # Suspending AI decisions does not stop an existing native navigation animation.
 $settleRows=[Collections.Generic.List[object]]::new();$previousGuard=$null;$stableGuard=0;$settleClock=[Diagnostics.Stopwatch]::StartNew()
 do{
  $catalog=Q client '--world-npcs'
  $guard=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'})
  if($guard.Count-ne1){throw 'Exact guard missing during settling'}
  $same=$null-ne$previousGuard-and$previousGuard.handle-eq$guard[0].handle
  if($same){for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$guard[0].position[$k]-[double]$previousGuard.position[$k])-gt.01){$same=$false}}}
  $stableGuard=if($same){$stableGuard+1}else{0};$previousGuard=$guard[0]
  $settleRows.Add([pscustomobject]@{ms=$settleClock.ElapsedMilliseconds;stableSamples=$stableGuard;target=$guard[0]})
  if($stableGuard-ge3){break};Start-Sleep -Milliseconds 220
 }while($settleClock.ElapsedMilliseconds-lt7500)
 $settleRows|ConvertTo-Json -Depth 9|Set-Content "$out/native-guard-settling.json"
 if($stableGuard-lt3){throw 'Native first guard still moving after bounded settling'}
'''
assert key in s;s=s.replace(key,insert+key,1)
s=s.replace(" $cancelBefore=Q client '--domino-links';",''' & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 45 -Evidence "$out/cancel-empty-aim.json" -Quick
 $cancelBefore=Q client '--domino-links';''',1)
s=s.replace(" if(!@($cancelState.active|Where-Object id -eq72).Count){throw 'Native Domino aim state not active before cancel'}",''' if(!@($cancelState.active|Where-Object id -eq72).Count){throw 'Native Domino aim state not active before cancel'}
 $aimLinks=Q client '--domino-links';$aimLinks|ConvertTo-Json -Depth 10|Set-Content "$out/links-while-cancel-aiming.json"
 if(!$aimLinks.valid-or$aimLinks.targets-ne2-or$aimLinks.connections.Count-ne2){throw 'Aiming changed the native links before cancellation input'}''',1)
(P/'Test-DominoNativeCancel-v228.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig').replace('Test-DominoNativeCancel-v227.ps1','Test-DominoNativeCancel-v228.ps1');runner.write_text(s,encoding='utf-8')
