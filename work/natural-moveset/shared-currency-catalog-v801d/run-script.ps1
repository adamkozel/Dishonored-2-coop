param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='shared-coin-catalog-v801',[ValidateSet('batch','simultaneous')][string]$Mode='batch')
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
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='CoinCatalog801';state='running';take=$Take}
 $pin=Get-Content "$root/work/movement-integration/candidate-coin-sync-v800/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected800 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  if((S $r).localGameLogicState-ne1){throw 'Gameplay required'};$inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r];$baseline[$r]=Money $r;Save "$out/currency-before-$r.json" $baseline[$r]
  $t=Target $r;if($t.Count-ne1){throw 'Exact coin missing'};Save "$out/target-before-$r.json" $t[0]
 }
 $dirty=$true
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_coin_sync off','coop_checkpoint_sync off','coop_door_sync off','coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}}
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.observer-isolation.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt3000);if(!$ready){throw 'NPC fixture not ready'}

 # Test data selects native objects; there are no per-item implementation paths.
 $plans=if($Mode-eq'batch'){@(
  @{role='host';name='pickups_coin_emily_lowvalue_4';quantity=1;view='-41.65 -152.9 69.5 158 59'},
  @{role='client';name='pickups_pouch_small_01';quantity=10;view='-32.6 -158.11 68.55 0 50'},
  @{role='client';name='pickups_coin_emily_lowvalue_7';quantity=1;view='-37.701 -171.723 68.7971 180 0'}
 )}else{@(
  @{role='host';name='pickups_coin_emily_lowvalue_4';quantity=1;view='-41.65 -152.9 69.5 158 59'},
  @{role='client';name='pickups_pouch_small_01';quantity=10;view='-32.6 -158.11 68.55 0 50'}
 )}
 Save "$out/plan.json" $plans
 foreach($r in 'host','client'){C $r 'setviewpos -38.7 -155 68.1 145 0';C $r 'coop_coin_sync on'}
 $wait.Restart();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt5000);if(!$ready){throw 'Catalog handshake failed'}
 $h=S host '.coin-sync.json';$c=S client '.coin-sync.json'
 if($h.catalogCount-ne19-or$c.catalogCount-ne19-or$h.catalogId-ne$c.catalogId){throw 'Expected matching 19 currency pickup catalog'}
 foreach($r in 'host','client'){Save "$out/armed-$r.json" (S $r '.coin-sync.json')}
 function Aim($plan,$index){
  $r=$plan.role;$target=@((Q $r '--entities').entities|Where-Object name -eq $plan.name);if($target.Count-ne1){throw "Target missing $($plan.name)"}
  C $r "setviewpos $($plan.view)";Start-Sleep -Milliseconds 600
  $st=Q $r '--player-states';if($st.stance-eq0){I $r B 200;Start-Sleep -Milliseconds 500};if((Q $r '--player-states').stance-ne1){throw 'Native crouch required'}
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X $target[0].position[0] -Y $target[0].position[1] -Z ($target[0].position[2]+.03) -Evidence "$out/aim-$index-$r.json"
  foreach($role in 'host','client'){$isolation=S $role '.observer-isolation.json';Save "$out/isolation-before-$index-$role.json" $isolation;if(!$isolation.ready-or$isolation.remainingMs-lt11000){throw 'Finite isolation insufficient for next case'}}
 }
 if($Mode-eq'simultaneous'){for($n=0;$n-lt$plans.Count;$n++){Aim $plans[$n] $n}}else{Aim $plans[0] 0}
 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$seconds=if($Mode-eq'batch'){32}else{14};$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$seconds",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 1500;$expected=0;$completed=[Collections.Generic.List[object]]::new()
 for($n=0;$n-lt$plans.Count;$n++){
  $plan=$plans[$n]
  if($Mode-eq'batch'){
   if($n){Aim $plan $n};I $plan.role X 200;$expected+=$plan.quantity;$completed.Add($plan)
  }else{
   foreach($plan in $plans){I $plan.role X 200;$expected+=$plan.quantity;$completed.Add($plan)}
  }
  $wait.Restart();do{$done=$true;foreach($r in 'host','client'){if((Money $r).currency-ne$baseline[$r].currency+$expected){$done=$false}};if(!$done){Start-Sleep -Milliseconds 60}}while(!$done-and$wait.ElapsedMilliseconds-lt7000)
  foreach($r in 'host','client'){Save "$out/after-$n-$r.json" (S $r '.coin-sync.json');Save "$out/currency-after-$n-$r.json" (Money $r)}
  if(!$done){throw "Catalog native awards did not converge at case $n"};Start-Sleep -Milliseconds 500
  foreach($r in 'host','client'){
   $state=S $r '.coin-sync.json';Save "$out/committed-$n-$r.json" $state
   if(!$state.enabled-or$state.commits-ne$completed.Count-or$state.awardedCount-ne$completed.Count){throw 'Catalog total receipt mismatch'}
   $entities=(Q $r '--entities').entities
   foreach($entry in $completed){$receipt=@($state.entries|Where-Object name -eq $entry.name)
    if($receipt.Count-ne1-or!$receipt[0].awarded-or$receipt[0].revision-ne1-or$receipt[0].ack-ne1-or@($entities|Where-Object name -eq $entry.name).Count){throw "Catalog receipt/removal mismatch $r $($entry.name)"}
    if($r-eq$entry.role){if(!$receipt[0].nativeSource-or$receipt[0].observations-ne1-or$receipt[0].nativeCalls-ne0){throw 'Expected ordinary native source'}}else{if($receipt[0].nativeSource-or$receipt[0].observations-ne0-or$receipt[0].nativeCalls-ne5){throw 'Expected one five-call native recipient'}}
   }
  }
  $rows.Add(@{mode=$Mode;completed=$completed.Count;deltaEach=$expected;passed=$true});"PASS_CATALOG_CASE $n count=$($completed.Count) delta=$expected"
  if($Mode-eq'simultaneous'){break}
 }
 $beforeDuplicate=S client '.coin-sync.json';for($n=0;$n-lt3;$n++){C host 'coop_coin_sync duplicate';Start-Sleep -Milliseconds 120};Start-Sleep -Milliseconds 500
 if((S client '.coin-sync.json').duplicates-le$beforeDuplicate.duplicates){throw 'Duplicate Steam catalog action not rejected'}
 foreach($r in 'host','client'){Save "$out/duplicate-$r.json" (S $r '.coin-sync.json');Save "$out/hook-$r.json" (S $r '.coin-hook.json');Save "$out/currency-repeat-$r.json" (Money $r);if((Money $r).currency-ne$baseline[$r].currency+$expected){throw 'Duplicate catalog credit'}}

}catch{$failure=$_.Exception.Message;foreach($r in 'host','client'){try{Save "$out/failure-status-$r.json" (S $r);Save "$out/failure-channel-$r.json" (S $r '.coin-sync.json');Save "$out/failure-states-$r.json" (Q $r '--player-states')}catch{}}}finally{

 if($owned){
  foreach($p in $captures){$p.WaitForExit(25000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
  foreach($r in 'host','client'){try{I $r;C $r 'coop_coin_sync off';C $r 'coop_movement_fixture off'}catch{}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline $out -Evidence "$out/note-restoration"
   foreach($r in 'host','client'){$restored=Q $r '--player-inventory';Save "$out/inventory-restored-$r.json" $restored;$cash=Money $r;Save "$out/currency-restored-$r.json" $cash;if($cash.currency-ne$baseline[$r].currency-or(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"}))){throw 'Currency or inventory restore differs'};if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40-or(Target $r).Count-ne1){throw 'Health or coin restoration differs'}}
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path $out){Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=$rows;scope='One shared catalog of 19 ordinary native currency pickups; data-driven mixed values and roles, independent receipts and duplicate rejection. No generic inventory/consumption claim. Full isolated restoration and protected file checks.'}}
  Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='CoinCatalog801';state='finished';take=$Take};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_SHARED_COIN'




