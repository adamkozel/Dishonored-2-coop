param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-coin-api-v790')
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
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='NativeCoinAPI790';state='running';take=$Take}
 $pin=Get-Content "$root/work/movement-integration/candidate-coin-fixture-v790/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected790 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  if((S $r).localGameLogicState-ne1){throw 'Gameplay required'};$inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r];$baseline[$r]=Money $r;Save "$out/currency-before-$r.json" $baseline[$r]
  $t=Target $r;if($t.Count-ne1-or$t[0].type-ne'.?AVarkPickup@@'-or$t[0].vtableRva-ne'0x1c5e258'){throw 'Exact coin missing'};Save "$out/target-before-$r.json" $t[0]
 }
 $dirty=$true
 foreach($r in 'host','client'){C $r 'coop_movement_fixture off';$prior=S $r '.coin-fixture.json';C $r 'coop_coin_fixture collect';Start-Sleep -Milliseconds 200;$api=S $r '.coin-fixture.json';Save "$out/native-scope-refusal-$r.json" $api;if($api.nativeCalls-ne$prior.nativeCalls-or$api.result-ne'native_scope_required'-or(Money $r).currency-ne$baseline[$r].currency){throw 'Fixture scope refusal failed'}}
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_door_sync off','coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}}
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ready=$true;foreach($r in 'host','client'){if(!(S $r '.observer-isolation.json').ready){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt3000);if(!$ready){throw 'NPC fixture not ready'}
 foreach($r in 'host','client'){
  $peer=if($r-eq'host'){'client'}else{'host'}
  foreach($q in 'host','client'){C $q 'coop_movement_fixture off';C $q 'coop_movement_fixture 60000'}
  C $peer 'setviewpos -38.7 -155 68.1 145 0';C $r 'setviewpos -41.65 -152.9 69.5 180 0'
  Start-Sleep -Milliseconds 1200;$st=Q $r '--player-states';if($st.stance-eq0){I $r B 200;Start-Sleep -Milliseconds 700};$st=Q $r '--player-states';if($st.stance-ne1){throw 'Expected native crouch for known coin approach'};$t=Target $r
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X $t[0].position[0] -Y $t[0].position[1] -Z ($t[0].position[2]+.03) -Evidence "$out/aim-$r.json"
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','14','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
  $captures.Add((Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$r],'--watch-player-states',14000) -WindowStyle Hidden -RedirectStandardOutput "$out/$r-states.json" -RedirectStandardError "$out/$r-states.log" -PassThru))
  foreach($q in 'host','client'){C $q 'coop_movement_fixture off';C $q 'coop_movement_fixture 60000'};Start-Sleep -Milliseconds 1500;$before=Money $r;$other=Money $peer
  $target=(Target $r)[0];$cs=Q $r '--components' $target.handle;$pickable=@($cs.components|Where-Object type -eq '.?AVarkCpntPickable_Simple@@');if($pickable.Count-ne1-or$pickable[0].owner-ne$target.pointer){throw 'Pickable owner mismatch'}
  Save "$out/components-$r.json" $cs;Save "$out/pickable-before-$r.json" (Q $r '--object' $pickable[0].pointer)
  $prior=S $r '.coin-fixture.json';C $r 'coop_coin_fixture collect';$wait.Restart();do{$after=Money $r;if($after.currency-eq$before.currency+1){break};Start-Sleep -Milliseconds 40}while($wait.ElapsedMilliseconds-lt5000)
  Save "$out/currency-after-$r.json" $after;Save "$out/pickable-after-$r.json" (Q $r '--object' $pickable[0].pointer);Save "$out/target-after-$r.json" (Target $r)
  if($after.currency-ne$before.currency+1-or(Money $peer).currency-ne$other.currency){throw 'Expected exactly one local coin and unchanged peer'}
  Start-Sleep -Milliseconds 1500;Save "$out/native-first-$r.json" (S $r '.coin-fixture.json');C $r 'coop_coin_fixture collect';Start-Sleep -Milliseconds 1100;$repeat=Money $r;Save "$out/currency-repeat-$r.json" $repeat;Save "$out/native-repeat-$r.json" (S $r '.coin-fixture.json');$api=S $r '.coin-fixture.json';if($api.nativeCalls-ne($prior.nativeCalls+5)-or$api.noops-ne($prior.noops+1)-or$api.result-ne'already_collected'){throw 'Native API count or duplicate refusal differs'};if($repeat.currency-ne$after.currency){throw 'Repeated input credited again'}
  $rows.Add(@{role=$r;before=$before.currency;after=$after.currency;repeat=$repeat.currency;peerUnchanged=$true;passed=$true});"PASS_NATIVE_COIN $r"
  foreach($p in $captures){$p.WaitForExit(20000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){throw 'Capture/state watch failed'}}
 }
 foreach($p in $captures){$p.WaitForExit(25000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){throw 'Capture/state watch failed'}}
}catch{$failure=$_.Exception.Message;foreach($r in 'host','client'){try{Save "$out/failure-status-$r.json" (S $r);Save "$out/failure-states-$r.json" (Q $r '--player-states');Save "$out/failure-health-$r.json" (Q $r '--health' 1)}catch{}}}finally{
 if($owned){
  foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_fixture off'}catch{}}
  foreach($p in $captures){$p.WaitForExit(25000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline $out -Evidence "$out/note-restoration"
   foreach($r in 'host','client'){$restored=Q $r '--player-inventory';Save "$out/inventory-restored-$r.json" $restored;$cash=Money $r;Save "$out/currency-restored-$r.json" $cash;if($cash.currency-ne$baseline[$r].currency-or(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"}))){throw 'Currency or inventory restore differs'};if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40-or(Target $r).Count-ne1){throw 'Health or coin restoration differs'}}
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path $out){Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=$rows;scope='Direct guarded native claim/reward/removal for exact authored coin per role. No source input or network relay. Exact currency delta, native API call count, duplicate no-op and outside-fixture refusal. Native pickedUp(true,false) entity/script event and return-variant destruction included before removal. Cleanup restores complete quantities, money, health and exact authored coin;14 protected files checked.'}}
  Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='NativeCoinAPI790';state='finished';take=$Take};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_NATIVE_COIN_API'
