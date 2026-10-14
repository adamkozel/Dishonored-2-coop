from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
t=(N/'Test-SharedCoin-v795.ps1').read_text()
start=t.index(' $rows.Add(@{source=$Source;');end=t.index('\n}catch{',start)
t=t[:start]+r'''
 foreach($p in $captures){$p.WaitForExit(25000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){throw 'First capture failed'}}
 $reloadBefore=@{}
 foreach($r in 'host','client'){C $r 'coop_checkpoint_sync on';C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})}
 $wait.Restart();do{$h=S host '.reset.json';$c=S client '.reset.json';if($h.enabled-and$c.enabled-and$h.peerArm-and$c.peerArm-and!$h.phase-and!$c.phase-and$h.digest-eq$c.digest){break};Start-Sleep -Milliseconds 50}while($wait.ElapsedMilliseconds-lt5000)
 if(!$h.peerArm-or!$c.peerArm-or$h.digest-ne$c.digest){throw 'Checkpoint pair failed to arm'}
 foreach($r in 'host','client'){
  $reloadBefore[$r]=@{reset=(S $r '.reset.json');coin=(S $r '.coin-sync.json');bridge=(S $r);loading=(S $r '.loading.json')};Save "$out/reload-before-$r.json" $reloadBefore[$r]
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','30','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/reload-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/reload-$r-capture.log" -RedirectStandardOutput "$out/reload-$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 800;C host 'RestartFromCheckpoint';C client 'RestartFromMemoryCheckpoint'
 $wait.Restart();$lastPress=@{host=0;client=0};$samples=[Collections.Generic.List[object]]::new()
 do{
  $ready=$true;$pair=@{}
  foreach($r in 'host','client'){
   $rs=S $r '.reset.json';$br=S $r;$cs=S $r '.coin-sync.json';$pair[$r]=@{reset=$rs;coin=$cs;bridge=$br}
   if($rs.phase-eq7){throw "Shared recovery failed $r $($rs.result)"}
   if($rs.completed-ne$reloadBefore[$r].reset.completed+1-or$br.localGameLogicState-ne1-or$br.localPendingGameLogicState-or!$rs.avatarReady-or$rs.avatarPending-or!$cs.enabled-or!$cs.ready-or$cs.rearms-ne$reloadBefore[$r].coin.rearms+1){$ready=$false}
   if(!$rs.held-and$wait.ElapsedMilliseconds-gt6000-and$br.localGameLogicState-in8,15-and$wait.ElapsedMilliseconds-$lastPress[$r]-gt700){I $r A 100;$lastPress[$r]=$wait.ElapsedMilliseconds}
  }
  $samples.Add(@{ms=$wait.ElapsedMilliseconds;pair=$pair});if(!$ready){Start-Sleep -Milliseconds 80}
 }while(!$ready-and$wait.ElapsedMilliseconds-lt50000)
 Save "$out/reload-samples.json" $samples;if(!$ready){throw 'Coin channel did not automatically rebind after shared checkpoint'}
 foreach($r in 'host','client'){
  $rs=S $r '.reset.json';$cs=S $r '.coin-sync.json';Save "$out/reload-after-$r.json" @{reset=$rs;coin=$cs;currency=(Money $r);character=(Q $r '--character');target=(Target $r)}
  if($rs.loads-ne$reloadBefore[$r].reset.loads+1-or$rs.aborted-ne$reloadBefore[$r].reset.aborted-or$rs.arm-ne$reloadBefore[$r].reset.arm-or$cs.arm-le$reloadBefore[$r].coin.arm-or$cs.epoch-eq$reloadBefore[$r].coin.epoch-or$cs.awarded-or$cs.attempted-or$cs.revision-or$cs.nativeCalls-or$cs.reloads-ne$reloadBefore[$r].coin.reloads+1-or(Money $r).currency-ne$baseline[$r].currency-or(Target $r).Count-ne1){throw "Fresh coin world reused receipt or failed restoration $r"}
  $sw=S $r '.loading.json';$events=@($sw.events|Where-Object sequence -gt $reloadBefore[$r].loading.switches);Save "$out/loading-after-$r.json" $sw
  if($sw.switches-$reloadBefore[$r].loading.switches-gt32-or@($events|Where-Object {$_.caller-eq'0x787a55'-and$_.to-eq15}).Count-ne1-or@($events|Where-Object {$_.caller-eq'0x7879cd'-and$_.to-eq1}).Count-ne1){throw 'Expected one actual native load entry/finish'}
  C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
 }
 $old=S client '.coin-sync.json';for($n=0;$n-lt3;$n++){C host 'coop_coin_sync stale';Start-Sleep -Milliseconds 120};Start-Sleep -Milliseconds 1300
 if((S client '.coin-sync.json').rejected-lt$old.rejected+3){throw 'Retired-world Steam packet not rejected'}
 foreach($r in 'host','client'){Save "$out/stale-$r.json" (S $r '.coin-sync.json');if((Money $r).currency-ne$baseline[$r].currency-or(Target $r).Count-ne1){throw 'Old coin packet altered new checkpoint'}}
 foreach($p in $captures){$p.WaitForExit(40000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){throw 'Reload capture failed'}}
 C host 'setviewpos -38.7 -155 68.1 145 0';C client 'setviewpos -41.65 -152.9 69.5 180 0';Start-Sleep -Milliseconds 1200
 if((Q client '--player-states').stance-eq0){I client B 200;Start-Sleep -Milliseconds 700};$target=(Target client)[0]
 & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role client -X $target.position[0] -Y $target.position[1] -Z ($target.position[2]+.03) -Evidence "$out/fresh-aim-client.json"
 $aim=Get-Content "$out/fresh-aim-client.json" -Raw|ConvertFrom-Json;if($aim.samples[-1].eye[2]-lt67.8){throw 'Fresh pickup vantage below coin'}
 foreach($r in 'host','client'){
  C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','14','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/fresh-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/fresh-$r-capture.log" -RedirectStandardOutput "$out/fresh-$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 900;I client X 200;$wait.Restart();do{$done=$true;foreach($r in 'host','client'){if((Money $r).currency-ne$baseline[$r].currency+1-or!(S $r '.coin-sync.json').ack){$done=$false}};if(!$done){Start-Sleep -Milliseconds 60}}while(!$done-and$wait.ElapsedMilliseconds-lt7000);if(!$done){throw 'Fresh post-recovery pickup failed'}
 foreach($r in 'host','client'){
  $cs=S $r '.coin-sync.json';Save "$out/fresh-$r.json" $cs;Save "$out/hook-$r.json" (S $r '.coin-hook.json')
  if(!$cs.enabled-or$cs.commits-ne$reloadBefore[$r].coin.commits+1-or$cs.nativeCalls-ne$(if($r-eq'host'){5}else{0})-or$cs.nativeSource-ne($r-eq'client')-or(Target $r).Count){throw 'Fresh post-recovery receipt differs'}
 }
 $rows.Add(@{source=$Source;ordinarySourceObserved=$true;nativePeerCalls=5;deltaEach=1;duplicatePacketsRejected=$true;sharedReloadsEach=1;automaticallyRebound=$true;staleWorldPacketsRejected=3;freshPostReloadClientPickup=$true;passed=$true});'PASS_COIN_RECOVERY'
'''+t[end:]
old="  foreach($r in 'host','client'){try{I $r;C $r 'coop_coin_sync off';C $r 'coop_movement_fixture off'}catch{}}\n"
assert old in t;t=t.replace(old,'')
t=t.replace("  if($dirty){try{","  foreach($r in 'host','client'){try{I $r;C $r 'coop_coin_sync off';C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';C $r 'coop_movement_fixture off'}catch{}}\n  if($dirty){try{")
t=t.replace("operation='SharedCoin795'","operation='SharedCoin797'")
(N/'Test-SharedCoinRecovery-v797.ps1').write_text(t)
print('Created coin shared-reload/stale replay/fresh-world pickup harness797')
