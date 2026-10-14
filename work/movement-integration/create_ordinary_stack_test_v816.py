from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-PickupStackBatch-v812.ps1').read_text()
e=s.index('\n')
s="param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='shared-pickup-ammo-input-v816',[ValidateSet('ammo','potion')][string]$Kind='ammo',[ValidateSet('host','client')][string]$Source='client')"+s[e:]
s=s.replace('StackBatch812','OrdinaryStack816').replace('v811','v814').replace('Expected811','Expected814')
s=s.replace("$coin='pickups_coin_emily_lowvalue_4'", "$coin=if($Kind-eq'ammo'){'pickups_ammo_bullet_box_regular'}else{'pickups_life_potion_01_9'}")
a=s.index(" foreach($r in 'host','client'){C $r 'setviewpos");b=s.index('\n}catch',a)
s=s[:a]+r'''
 $peer=if($Source-eq'host'){'client'}else{'host'};$target=(Target $Source)[0];$pos=$target.position
 C $peer 'setviewpos -38.7 -155 68.1 145 0'
 $x=$pos[0]+.2;$y=$pos[1]+1.0;$z=$pos[2]+1.7
 C $Source "setviewpos $x $y $z 270 45";Start-Sleep -Milliseconds 1000
 & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Source -X $pos[0] -Y $pos[1] -Z ($pos[2]+.03) -Evidence "$out/aim-$Source.json"
 foreach($r in 'host','client'){$iso=S $r '.observer-isolation.json';if(!$iso.ready-or$iso.remainingMs-lt12000){throw 'Finite fixture not available for ordinary input'};C $r 'coop_coin_sync on'}
 $wait.Restart();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt5000);if(!$ready){throw 'Pickup handshake failed'}
 foreach($r in 'host','client'){
  Save "$out/armed-$r.json" (S $r '.coin-sync.json');$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','14','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 800;Save "$out/source-before-input.json" (Q $Source '--player');I $Source X 200
 $wait.Restart();do{
  $done=$true;foreach($r in 'host','client'){$state=S $r '.coin-sync.json';$receipt=@($state.entries|Where-Object name -eq $coin);if(!$state.enabled){throw "Catalog stopped $r $($state.result)"};if($receipt.Count-ne1-or!$receipt[0].awarded-or!$receipt[0].ack){$done=$false}}
  if(!$done){Start-Sleep -Milliseconds 80}
 }while(!$done-and$wait.ElapsedMilliseconds-lt5000)
 foreach($r in 'host','client'){Save "$out/after-$r.json" (S $r '.coin-sync.json');Save "$out/entities-after-$r.json" (Q $r '--entities');Save "$out/inventory-after-$r.json" (Q $r '--player-inventory')}
 if(!$done){throw 'Ordinary source input did not collect the selected pickup'}
 foreach($r in 'host','client'){
  $state=S $r '.coin-sync.json';Save "$out/committed-$r.json" $state;$receipt=@($state.entries|Where-Object name -eq $coin)[0];$source=$r-eq$Source
  if($state.catalogCount-ne24-or$state.awardedCount-ne1-or$state.commits-ne1-or!$receipt.awarded-or!$receipt.ack-or$receipt.nativeSource-ne$source-or$receipt.nativeCalls-ne$(if($source){0}else{5})-or$receipt.exerciseCalls-or$receipt.observations-ne$(if($source){1}else{0})-or$receipt.accepted-le0-or(Target $r).Count){throw 'Ordinary receipt ownership or removal differs'}
  $after=Get-Content "$out/inventory-after-$r.json" -Raw|ConvertFrom-Json
  foreach($item in $inventory[$r].items){$found=@($after.items|Where-Object name -eq $item.name);$delta=if($item.name-eq$receipt.item){$receipt.accepted}else{0};if($found.Count-ne1-or$found[0].quantity-ne$item.quantity+$delta){throw 'Ordinary pickup altered unexpected inventory'}}
  if((Money $r).currency-ne$baseline[$r].currency-or(Q $r '--health' 1).current-ne56){throw 'Ordinary stack unexpectedly changed health or money'}
  Save "$out/hook-$r.json" (S $r '.coin-hook.json');$rows.Add(@{role=$r;receipt=$receipt;passed=$true})
 }
 C host 'coop_coin_sync duplicate';Start-Sleep -Milliseconds 700
 foreach($r in 'host','client'){
  $state=S $r '.coin-sync.json';Save "$out/duplicate-$r.json" $state;$before=Get-Content "$out/committed-$r.json" -Raw|ConvertFrom-Json
  if($state.nativeCalls-ne$before.nativeCalls){throw 'Ordinary duplicate applied twice'}
  $after=Q $r '--player-inventory';$before=Get-Content "$out/inventory-after-$r.json" -Raw|ConvertFrom-Json
  if(Compare-Object @($before.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Duplicate altered stack quantity'}
 }
 'PASS_ORDINARY_STACK_SOURCE'
''' +s[b:]
a=s.index("scope='Developer native API source exercise");b=s.index("'",a+7)
s=s[:a]+"scope='Ordinary controller X source input for one representative native stack family; exact installed reward observation, one Steam recipient native award/removal, no API source exercise, duplicates and unrelated inventory checks. Full baseline/protected-file restoration. Source camera and per-entry outcomes preserved.'"+s[b+1:]
s=s.replace('PASS_SHARED_PICKUP_STACK_BATCH','PASS_ORDINARY_STACK')
(N/'Test-OrdinaryStack-v816.ps1').write_text(s)
print('Prepared bounded ordinary family representative test816')
