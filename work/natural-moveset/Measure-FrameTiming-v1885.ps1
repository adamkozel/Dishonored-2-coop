param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='frame-timing-v1885a',[string]$Candidate='candidate-npc-combat-v1214jx',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[int]$Seconds=120)
# 1885 (#195): frame pacing with and without the mod's services. Both lab games load the mission (host Corvo, partner
# Emily) with the health hold. Phase "on": product mode armed (every co-op service the product runs). Phase "off": the
# product controller and every sync service turned off (the lab session stays connected). Each phase settles 15 s, then
# frame-timing.json (1214jx) is read at its start and after $Seconds, with native-service-timing.json at its end. The
# result keeps, per role and phase, the frame-time percentiles of the last window, spikes over 25/50/100 ms per minute
# over the phase, the mod's per-frame time, and how often each scope was the top scope of a spike. The lab is restored
# at the end (Deploy-SourceClock). Two lab games share this PC's CPU and GPU: compare the phases, not the absolute values.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=100){try{& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}catch{}}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
function Phase($name){
 Start-Sleep -Seconds 15;$a=@{};foreach($r in 'host','client'){$a[$r]=S $r '.frame-timing.json'};Start-Sleep -Seconds $Seconds
 $result=[ordered]@{phase=$name}
 foreach($r in 'host','client'){$b=S $r '.frame-timing.json';$st=S $r '.native-service-timing.json';Save "$out/$name-$r-frame-timing.json" $b;Save "$out/$name-$r-service-timing.json" $st
  $minutes=$Seconds/60.0;$tops=@{};foreach($s in @($b.spikes|Where-Object {$_.tick-gt$a[$r].tick})){$k=if($s.top){$s.top}else{'(none)'};$tops[$k]=1+[int]$tops[$k]}
  $result[$r]=[ordered]@{framesPerSecond=[Math]::Round(($b.frames-$a[$r].frames)/[double]$Seconds,1);p50=$b.window.p50;p90=$b.window.p90;p99=$b.window.p99;max=$b.window.max
   spikes25PerMinute=[Math]::Round(($b.spikes25-$a[$r].spikes25)/$minutes,2);spikes50PerMinute=[Math]::Round(($b.spikes50-$a[$r].spikes50)/$minutes,2);spikes100PerMinute=[Math]::Round(($b.spikes100-$a[$r].spikes100)/$minutes,2)
   modP50=$b.window.modP50;modP99=$b.window.modP99;modMax=$b.window.modMax;modShare=[Math]::Round(($b.modTotalMs-$a[$r].modTotalMs)/(10.0*$Seconds),2)
   recentSpikeTops=$tops;slowestStages=@($st.stages|Sort-Object maximumMs -Descending|Select-Object -First 8|ForEach-Object {"$($_.name) max $([Math]::Round($_.maximumMs,1)) ms, mean $([Math]::Round($_.totalMs/[Math]::Max(1,$_.count),3)) ms"})}}
 $notes.phases+=@($result);Save "$out/phases.json" @($notes.phases);$result}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;seconds=$Seconds;phases=@()}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;seconds=$Seconds})
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 $notes.on=Phase 'on'
 foreach($r in 'host','client'){C $r 'coop_product_auto off';foreach($cmd in 'coop_checkpoint_sync off','coop_door_sync off','coop_usable_sync off','coop_coin_sync off','coop_npc_stream off','coop_trigger_mirror off','coop_breakable_sync off','coop_prop_rest off','coop_use_mirror off'){try{C $r $cmd}catch{$notes.refused+=@("$r $cmd")}}}
 $notes.off=Phase 'off'
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 $mutex.Dispose()
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope='Frame pacing of both lab games on a mission with the co-op services on, then off; lab restored.'})
 if($failure){throw $failure}
}
