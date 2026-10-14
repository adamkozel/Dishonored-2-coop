from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
t=(N/'Test-SharedCoin-v795.ps1').read_text()
t=t.replace("[string]$Source='host')","[string]$Source='host',[ValidateSet('Simultaneous','Menu')][string]$Mode='Simultaneous')")
start=t.index(" $peer=if($Source-eq'host')");end=t.index(" foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';C $r 'coop_coin_sync on'}",start)
t=t[:start]+r'''
 $peer=if($Source-eq'host'){'client'}else{'host'}
 $sources=if($Mode-eq'Simultaneous'){@('host','client')}else{@($Source)}
 if($Mode-eq'Simultaneous'){foreach($r in 'host','client'){C $r 'coop_probe_follow off'}}else{C $peer 'setviewpos -38.7 -155 68.1 145 0'}
 foreach($r in $sources){
  C $r 'setviewpos -41.65 -152.9 69.5 180 0';Start-Sleep -Milliseconds 1200;$st=Q $r '--player-states';if($st.stance-eq0){I $r B 200;Start-Sleep -Milliseconds 700}
  if((Q $r '--player-states').stance-ne1){throw 'Expected native crouch for known coin approach'};$t=Target $r
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X $t[0].position[0] -Y $t[0].position[1] -Z ($t[0].position[2]+.03) -Evidence "$out/aim-$r.json"
  $aim=Get-Content "$out/aim-$r.json" -Raw|ConvertFrom-Json;if($aim.samples[-1].eye[2]-lt67.8){throw 'Source fell below coin vantage'}
 }
'''+t[end:]
t=t.replace("'-t','14'","'-t','18'")
t=t.replace('Start-Sleep -Milliseconds 1500;I $Source X 200',r'''Start-Sleep -Milliseconds 1500
 if($Mode-eq'Simultaneous'){I host X 200;I client X 200}else{
  I $peer Start 180;$wait.Restart();do{if((S $peer).localGameLogicState-eq2-and(S $Source '.coin-sync.json').paused-and(S $peer '.coin-sync.json').paused){break};Start-Sleep -Milliseconds 50}while($wait.ElapsedMilliseconds-lt4000)
  if((S $peer).localGameLogicState-ne2-or!(S $Source '.coin-sync.json').paused){throw 'Native peer pause did not suspend channel'}
  I $Source X 200;$wait.Restart();do{if((Money $Source).currency-eq$baseline[$Source].currency+1){break};Start-Sleep -Milliseconds 50}while($wait.ElapsedMilliseconds-lt4000)
  if((Money $Source).currency-ne$baseline[$Source].currency+1){throw 'Source native pickup failed during peer pause'}
  Start-Sleep -Milliseconds 700;$o=Q $peer '--object' $baseline[$peer].inventory;$owner=@($o.words|Where-Object offset -eq '0x10');$w=@($o.words|Where-Object offset -eq '0x168')
  if($o.type-ne'.?AVarkCpntInventory@@'-or$owner[0].word-ne$baseline[$peer].actor-or$w.Count-ne1-or(S $peer).localGameLogicState-ne2){throw 'Paused inventory identity changed'}
  $cash=[Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32;Save "$out/paused-peer-wallet.json" @{currency=$cash;inventory=$baseline[$peer].inventory;owner=$owner[0];raw=$w[0]}
  if($cash-ne$baseline[$peer].currency){throw 'Peer was credited inside native pause'}
  foreach($r in 'host','client'){Save "$out/paused-$r.json" (S $r '.coin-sync.json')}
  I $peer Start 180;$wait.Restart();do{if((S $peer).localGameLogicState-eq1){break};Start-Sleep -Milliseconds 50}while($wait.ElapsedMilliseconds-lt4000);if((S $peer).localGameLogicState-ne1){throw 'Native menu failed to resume'}
 }''')
t=t.replace("if($r-eq$Source){if(!$s.nativeSource", "if($r-eq$Source-or$Mode-eq'Simultaneous'){if(!$s.nativeSource")
t=t.replace("Save \"$out/committed-$r.json\" $s", "Save \"$out/committed-$r.json\" $s;Save \"$out/hook-$r.json\" (S $r '.coin-hook.json')")
t=t.replace('@{source=$Source;ordinarySourceObserved=$true;nativePeerCalls=5;',"@{source=$Source;mode=$Mode;ordinarySourceObserved=$true;nativePeerCalls=$(if($Mode-eq'Simultaneous'){0}else{5});")
old="  foreach($r in 'host','client'){try{I $r;C $r 'coop_coin_sync off';C $r 'coop_movement_fixture off'}catch{}}\n"
assert old in t;t=t.replace(old,'')
insert="  if($dirty){try{"
t=t.replace(insert,"  foreach($r in 'host','client'){try{I $r;C $r 'coop_coin_sync off';if((S $r).localGameLogicState-eq2){I $r Start 180;Start-Sleep -Milliseconds 400};C $r 'coop_movement_fixture off'}catch{}}\n"+insert)
t=t.replace("operation='SharedCoin795'","operation='SharedCoin796'")
(N/'Test-SharedCoinEdge-v796.ps1').write_text(t)
print('Created simultaneous/menu native Steam test796')
