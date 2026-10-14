from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/natural-moveset'
old=(W/'Test-NativeCoinAPI-v790.ps1').read_text()
prefix=old[:old.index('\ntry{\n try{$owned')]
prefix=prefix.replace("[string]$Take='native-coin-api-v790')","[string]$Take='shared-coin-host-v793',[ValidateSet('host','client')][string]$Source='host')")
body=r'''
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='SharedCoin793';state='running';take=$Take}
 $pin=Get-Content "$root/work/movement-integration/candidate-coin-sync-v792/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected792 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  if((S $r).localGameLogicState-ne1){throw 'Gameplay required'};$inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r];$baseline[$r]=Money $r;Save "$out/currency-before-$r.json" $baseline[$r]
  $t=Target $r;if($t.Count-ne1){throw 'Exact coin missing'};Save "$out/target-before-$r.json" $t[0]
 }
 $dirty=$true
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_coin_sync off','coop_checkpoint_sync off','coop_door_sync off','coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}}
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.observer-isolation.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt3000);if(!$ready){throw 'NPC fixture not ready'}
 $peer=if($Source-eq'host'){'client'}else{'host'}
 C $peer 'setviewpos -38.7 -155 68.1 145 0';C $Source 'setviewpos -41.65 -152.9 69.5 180 0'
 Start-Sleep -Milliseconds 1200;$st=Q $Source '--player-states';if($st.stance-eq0){I $Source B 200;Start-Sleep -Milliseconds 700};$st=Q $Source '--player-states';if($st.stance-ne1){throw 'Expected native crouch for known coin approach'};$t=Target $Source
 & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Source -X $t[0].position[0] -Y $t[0].position[1] -Z ($t[0].position[2]+.03) -Evidence "$out/aim-$Source.json"
 $aim=Get-Content "$out/aim-$Source.json" -Raw|ConvertFrom-Json;if($aim.samples[-1].eye[2]-lt67.8){throw 'Source fell below coin vantage'}
 foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';C $r 'coop_coin_sync on'}
 $wait.Restart();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt5000);if(!$ready){throw 'Shared coin handshake failed'}
 foreach($r in 'host','client'){
  Save "$out/armed-$r.json" (S $r '.coin-sync.json')
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','14','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 1500;I $Source X 200
 $wait.Restart();do{$done=$true;foreach($r in 'host','client'){if((Money $r).currency-ne$baseline[$r].currency+1){$done=$false}};if(!$done){Start-Sleep -Milliseconds 60}}while(!$done-and$wait.ElapsedMilliseconds-lt7000)
 foreach($r in 'host','client'){Save "$out/after-$r.json" (S $r '.coin-sync.json');Save "$out/currency-after-$r.json" (Money $r);Save "$out/target-after-$r.json" (Target $r)}
 if(!$done){throw 'Shared native coin award did not converge'}
 Start-Sleep -Milliseconds 1000
 foreach($r in 'host','client'){
  $s=S $r '.coin-sync.json';Save "$out/committed-$r.json" $s
  if(!$s.enabled-or!$s.awarded-or$s.revision-ne1-or$s.ack-ne1-or$s.commits-ne1-or(Target $r).Count){throw "Shared receipt/remove mismatch $r"}
  if($r-eq$Source){if(!$s.nativeSource-or$s.observations-ne1-or$s.nativeCalls-ne0){throw 'Source must be ordinary native pickup'}}else{if($s.nativeSource-or$s.observations-ne0-or$s.nativeCalls-ne5){throw 'Peer must apply one complete five-call native sequence'}}
 }
 $beforeDuplicate=S client '.coin-sync.json';for($n=0;$n-lt3;$n++){C host 'coop_coin_sync duplicate';Start-Sleep -Milliseconds 120}
 Start-Sleep -Milliseconds 1300;$dup=S client '.coin-sync.json';if($dup.duplicates-le$beforeDuplicate.duplicates){throw 'Steam duplicate envelope was not rejected'}
 foreach($r in 'host','client'){Save "$out/duplicate-$r.json" (S $r '.coin-sync.json');Save "$out/currency-repeat-$r.json" (Money $r);if((Money $r).currency-ne$baseline[$r].currency+1){throw 'Duplicate packet credited twice'}}
 $rows.Add(@{source=$Source;ordinarySourceObserved=$true;nativePeerCalls=5;deltaEach=1;duplicatePacketsRejected=$true;passed=$true});"PASS_SHARED_COIN $Source"
}catch{$failure=$_.Exception.Message;foreach($r in 'host','client'){try{Save "$out/failure-status-$r.json" (S $r);Save "$out/failure-channel-$r.json" (S $r '.coin-sync.json');Save "$out/failure-states-$r.json" (Q $r '--player-states')}catch{}}}finally{
'''
suffix=old[old.index('\n if($owned){\n',old.index('}finally{')):]
suffix=suffix.replace("try{I $r;C $r 'coop_movement_fixture off'}", "try{I $r;C $r 'coop_coin_sync off';C $r 'coop_movement_fixture off'}")
suffix=suffix.replace('NativeCoinAPI790','SharedCoin793').replace('PASS_NATIVE_COIN_API','PASS_SHARED_COIN')
start=suffix.index("scope='");end=suffix.index("'",start+7)
suffix=suffix[:start]+"scope='Exact authored coin shared through native Steam: ordinary source pickup, peer five-call native sequence, one delta per wallet and duplicate envelope rejection. Full isolated checkpoint/inventory/health/coin restoration and protected file checks."+suffix[end:]
(W/'Test-SharedCoin-v793.ps1').write_text(prefix+body+suffix)
print('Created native Steam pickup harness793')
