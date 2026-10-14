param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='shared-coin-host-v793',[ValidateSet('host','client')][string]$Source='host')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$inventory=@{};$baseline=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new();$coin='pickups_coin_emily_lowvalue_4'
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 20}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Read failed $r $mode"}
function Money($r){
 $h=Q $r '--health' 1;if($h.logic-ne1){throw 'Currency requires native gameplay'}
 $cs=Q $r '--components' 1;$i=@($cs.components|Where-Object type -eq '.?AVarkCpntInventory@@');if($i.Count-ne1-or$i[0].owner-ne$h.actor){throw 'Currency inventory ownership mismatch'}
 $o=Q $r '--object' $i[0].pointer;$owner=@($o.words|Where-Object offset -eq '0x10');$w=@($o.words|Where-Object offset -eq '0x168')
 if($o.type-ne'.?AVarkCpntInventory@@'-or$owner.Count-ne1-or$owner[0].word-ne$h.actor-or$w.Count-ne1-or(Q $r '--health' 1).actor-ne$h.actor){throw 'Currency inventory incarnation changed'}
 $n=[Convert]::ToUInt64($w[0].word.Substring(2),16);return @{currency=[long]($n-shr32);inventory=$i[0].pointer;raw=$w[0];actor=$h.actor}
}
function Target($r){@((Q $r '--entities').entities|Where-Object name -eq $coin)}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='CurrencyCatalogRecovery802';state='running';take=$Take}
 $pin=Get-Content "$root/work/movement-integration/candidate-coin-sync-v800/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected800 candidate'}
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
 foreach($r in 'host','client'){$iso=S $r '.observer-isolation.json';Save "$out/isolation-armed-$r.json" $iso;if(!$iso.ready-or$iso.remainingMs-lt18000){throw 'Finite NPC fixture unavailable before coin arm'};C $r 'coop_coin_sync on'}
 $wait.Restart();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt5000);if(!$ready){throw 'Shared coin handshake failed'}
 foreach($r in 'host','client'){
  Save "$out/armed-$r.json" (S $r '.coin-sync.json')
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','14','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 1500;Save "$out/source-before-input.json" (Q $Source '--player');Save "$out/isolation-before-input.json" (S $Source '.observer-isolation.json');if(!(S $Source '.observer-isolation.json').ready){throw 'NPC fixture lost before source input'};I $Source X 200
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
  $iso=S $r '.observer-isolation.json';if(!$iso.ready-or$iso.remainingMs-lt16000){throw 'Finite NPC fixture unavailable for fresh capture'};$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','14','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/fresh-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/fresh-$r-capture.log" -RedirectStandardOutput "$out/fresh-$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 900;I client X 200;$wait.Restart();do{$done=$true;foreach($r in 'host','client'){if((Money $r).currency-ne$baseline[$r].currency+1-or!(S $r '.coin-sync.json').ack){$done=$false}};if(!$done){Start-Sleep -Milliseconds 60}}while(!$done-and$wait.ElapsedMilliseconds-lt7000);if(!$done){throw 'Fresh post-recovery pickup failed'}
 foreach($r in 'host','client'){
  $cs=S $r '.coin-sync.json';Save "$out/fresh-$r.json" $cs;Save "$out/hook-$r.json" (S $r '.coin-hook.json')
  if(!$cs.enabled-or$cs.commits-ne$reloadBefore[$r].coin.commits+1-or$cs.nativeCalls-ne$(if($r-eq'host'){5}else{0})-or$cs.nativeSource-ne($r-eq'client')-or(Target $r).Count){throw 'Fresh post-recovery receipt differs'}
 }
 $rows.Add(@{source=$Source;ordinarySourceObserved=$true;nativePeerCalls=5;deltaEach=1;duplicatePacketsRejected=$true;sharedReloadsEach=1;automaticallyRebound=$true;staleWorldPacketsRejected=3;freshPostReloadClientPickup=$true;passed=$true});'PASS_COIN_RECOVERY'

}catch{$failure=$_.Exception.Message;foreach($r in 'host','client'){try{Save "$out/failure-status-$r.json" (S $r);Save "$out/failure-channel-$r.json" (S $r '.coin-sync.json');Save "$out/failure-states-$r.json" (Q $r '--player-states')}catch{}}}finally{

 if($owned){
  foreach($p in $captures){$p.WaitForExit(25000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
  foreach($r in 'host','client'){try{I $r;C $r 'coop_coin_sync off';C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';C $r 'coop_movement_fixture off'}catch{}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline $out -Evidence "$out/note-restoration"
   foreach($r in 'host','client'){$restored=Q $r '--player-inventory';Save "$out/inventory-restored-$r.json" $restored;$cash=Money $r;Save "$out/currency-restored-$r.json" $cash;if($cash.currency-ne$baseline[$r].currency-or(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"}))){throw 'Currency or inventory restore differs'};if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40-or(Target $r).Count-ne1){throw 'Health or coin restoration differs'}}
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path $out){Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=$rows;scope='Exact authored coin shared through native Steam: ordinary source pickup, peer five-call native sequence, one delta per wallet and duplicate envelope rejection. Full isolated checkpoint/inventory/health/coin restoration and protected file checks.'}}
  Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='CurrencyCatalogRecovery802';state='finished';take=$Take};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_SHARED_COIN'



