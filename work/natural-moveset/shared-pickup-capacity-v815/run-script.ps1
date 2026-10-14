param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='shared-pickup-capacity-v815')
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
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='PickupCapacity815';state='running';take=$Take}
 $pin=Get-Content "$root/work/movement-integration/candidate-coin-sync-v814/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected814 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  if((S $r).localGameLogicState-ne1){throw 'Gameplay required'};$inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r];$baseline[$r]=Money $r;Save "$out/currency-before-$r.json" $baseline[$r]
  $t=Target $r;if($t.Count-ne1){throw 'Exact coin missing'};Save "$out/target-before-$r.json" $t[0]
 }
 $dirty=$true
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_coin_sync off','coop_checkpoint_sync off','coop_door_sync off','coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}}
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.observer-isolation.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt3000);if(!$ready){throw 'NPC fixture not ready'}


 foreach($r in 'host','client'){C $r 'setviewpos -38.7 -155 68.1 145 0';C $r 'coop_coin_sync on'}
 $wait.Restart();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt5000);if(!$ready){throw 'Catalog handshake failed'}
 $h=S host '.coin-sync.json';$c=S client '.coin-sync.json';if($h.catalogCount-ne24-or$c.catalogCount-ne24-or$h.catalogId-ne$c.catalogId){throw 'Expected matching 24 currency/ammo/stored-potion catalog'}

 $plan=@(
  @{name='pickups_ammo_bullet_box_regular';family=4;source='host';fixtureRole='client';setup=9;recipientDelta=1},
  @{name='pickups_ammo_bullet_regular';family=4;source='host';fixtureRole='client';setup=10;recipientDelta=0},
  @{name='pickups_life_potion_01_9';family=14;source='client';fixtureRole='host';setup=10;recipientDelta=0}
 )
 Save "$out/plan.json" $plan
 foreach($r in 'host','client'){
  Save "$out/armed-$r.json" (S $r '.coin-sync.json');$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','20','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 700;$completed=0
 foreach($entry in $plan){
  C $entry.fixtureRole "coop_coin_sync capacity $($entry.name) $($entry.setup)"
  $wait.Restart();do{$f=S $entry.fixtureRole '.pickup-capacity.json';if($f.active-and$f.result-eq'native_capacity_ready'){break};Start-Sleep -Milliseconds 50}while($wait.ElapsedMilliseconds-lt2500)
  Save "$out/setup-$completed.json" $f;if(!$f.active-or$f.result-ne'native_capacity_ready'){throw 'Native capacity fixture unavailable'}
  foreach($r in 'host','client'){Save "$out/before-$completed-$r.json" (Q $r '--player-inventory')}
  C $entry.source "coop_coin_sync exercise $($entry.name)"
  $wait.Restart();do{
   $done=$true;foreach($r in 'host','client'){$state=S $r '.coin-sync.json';$receipt=@($state.entries|Where-Object name -eq $entry.name);if(!$state.enabled){throw "Catalog stopped $r $($state.result)"};if($receipt.Count-ne1-or!$receipt[0].awarded-or!$receipt[0].ack){$done=$false}}
   if(!$done){Start-Sleep -Milliseconds 50}
  }while(!$done-and$wait.ElapsedMilliseconds-lt3500)
  if(!$done){throw "Capacity case did not converge: $($entry.name)"}
  $case=@{}
  foreach($r in 'host','client'){
   $state=S $r '.coin-sync.json';Save "$out/committed-$completed-$r.json" $state
   $receipt=@($state.entries|Where-Object name -eq $entry.name)[0];$source=$r-eq$entry.source
   if(!$receipt.awarded-or!$receipt.ack-or$receipt.family-ne$entry.family-or$receipt.nativeSource-ne$source-or$receipt.nativeCalls-ne$(if($source){0}else{5})-or$receipt.exerciseCalls-ne$(if($source){5}else{0})-or$receipt.observations-ne$(if($source){1}else{0})){throw 'Wrong receipt ownership or native call count'}
   if($source){if($receipt.accepted-le0){throw 'Source did not gain native inventory'}}else{if($receipt.before-ne$entry.setup-or$receipt.after-ne10-or$receipt.accepted-ne$entry.recipientDelta){throw 'Capacity clamp or zero-grant differs'}}
   $entities=Q $r '--entities';Save "$out/entities-$completed-$r.json" $entities
   if(@($entities.entities|Where-Object name -eq $entry.name).Count){throw 'Consumed pickup remains registered'}
   $after=Q $r '--player-inventory';Save "$out/after-$completed-$r.json" $after;$before=Get-Content "$out/before-$completed-$r.json" -Raw|ConvertFrom-Json
   foreach($item in $before.items){$found=@($after.items|Where-Object name -eq $item.name);$delta=if($item.name-eq$receipt.item){$receipt.accepted}else{0};if($found.Count-ne1-or$found[0].quantity-ne$item.quantity+$delta){throw 'Capacity case changed unrelated inventory'}}
   if((Money $r).currency-ne$baseline[$r].currency-or(Q $r '--health' 1).current-ne56){throw 'Capacity test altered health or money'}
   $case[$r]=$receipt
  }
  C $entry.source "coop_coin_sync exercise $($entry.name)";C host 'coop_coin_sync duplicate';Start-Sleep -Milliseconds 600
  foreach($r in 'host','client'){
   $state=S $r '.coin-sync.json';Save "$out/duplicate-$completed-$r.json" $state;$receipt=@($state.entries|Where-Object name -eq $entry.name)[0]
   if($receipt.nativeCalls-ne$case[$r].nativeCalls-or$receipt.exerciseCalls-ne$case[$r].exerciseCalls-or$receipt.after-ne$case[$r].after){throw 'Capacity duplicate applied again'}
   $repeat=Q $r '--player-inventory';$before=Get-Content "$out/after-$completed-$r.json" -Raw|ConvertFrom-Json
   if(Compare-Object @($before.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($repeat.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Replay changed actual capacity inventory'}
  }
  C $entry.fixtureRole 'coop_coin_sync capacity off'
  $wait.Restart();do{$f=S $entry.fixtureRole '.pickup-capacity.json';if(!$f.active-and$f.result-eq'native_quantity_restored'){break};Start-Sleep -Milliseconds 50}while($wait.ElapsedMilliseconds-lt2500)
  Save "$out/restored-$completed.json" $f;if($f.active-or$f.result-ne'native_quantity_restored'){throw 'Capacity fixture did not restore original quantity'}
  $rows.Add(@{plan=$entry;receipts=$case;passed=$true});$completed++;"PASS_PICKUP_CAPACITY $completed"
 }
 foreach($r in 'host','client'){Save "$out/committed-$r.json" (S $r '.coin-sync.json');Save "$out/hook-$r.json" (S $r '.coin-hook.json')}

}catch{$failure=$_.Exception.Message;foreach($r in 'host','client'){try{Save "$out/failure-status-$r.json" (S $r);Save "$out/failure-channel-$r.json" (S $r '.coin-sync.json');Save "$out/failure-states-$r.json" (Q $r '--player-states')}catch{}}}finally{

 if($owned){
  foreach($p in $captures){$p.WaitForExit(25000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
  foreach($r in 'host','client'){try{I $r;C $r 'coop_coin_sync capacity off';C $r 'coop_coin_sync off';C $r 'coop_movement_fixture off'}catch{}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline $out -Evidence "$out/note-restoration"
   foreach($r in 'host','client'){$restored=Q $r '--player-inventory';Save "$out/inventory-restored-$r.json" $restored;$cash=Money $r;Save "$out/currency-restored-$r.json" $cash;if($cash.currency-ne$baseline[$r].currency-or(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"}))){throw 'Currency or inventory restore differs'};if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40-or(Target $r).Count-ne1){throw 'Health or coin restoration differs'}}
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path $out){Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=$rows;scope='Native API source/recipient capacity tests over Steam: partial ammo 9-to-10, full ammo and full stored health potion accept zero but retire the world object and acknowledge once; per-item native ownership, unrelated inventory/health/money, duplicates and native fixture restoration. Checkpoint cleanup restores full isolated baseline. Not ordinary ammo/potion input or cross-account online proof.'}}
  Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='PickupCapacity815';state='finished';take=$Take};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_SHARED_PICKUP_CAPACITY'




