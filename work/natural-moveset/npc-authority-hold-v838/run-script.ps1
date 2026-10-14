param([string]$Take='npc-authority-hold-v838')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$epochs=@{};$catalog=@{};$inventory=@{};$baseline=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 24|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function Q($r,$mode,$arg=0){for($a=0;$a-lt6;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Read failed $r $mode"}
function S($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function H{Get-Content "$root/work/lab-instances/client/coop-status.json.npc-authority.json" -Raw|ConvertFrom-Json}
function WaitHold($active,$ready,$tag){
 $watch=[Diagnostics.Stopwatch]::StartNew();do{$h=H;if($h.active-eq$active-and$h.ready-eq$ready){Save "$out/hold-$tag.json" $h;return $h};Start-Sleep -Milliseconds 60}while($watch.ElapsedMilliseconds-lt16000)
 Save "$out/hold-$tag.json" $h;throw "Unexpected hold state $tag"
}
function AiSnapshot($r,$tag){
 $all=@();foreach($n in $catalog[$r].entries){foreach($kind in 'decision','reflex','attack'){$v=Q $r "--ai-$kind" $n.handle;$all+=@{name=$n.name;kind=$kind;value=$v}}}
 Save "$out/ai-$r-$tag.json" $all;return ,$all
}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-authority-v837/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected837 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  $s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){throw 'Gameplay required'};$epochs[$r]=$s.localEpoch;Save "$out/before-$r.json" $s
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
  Save "$out/health-before-$r.json" (Q $r '--health' 1)
  $catalog[$r]=Q $r '--npc-authority';Save "$out/catalog-before-$r.json" $catalog[$r]
  if($catalog[$r].entries.Count-ne5-or@($catalog[$r].entries|Where-Object maximum -eq 50).Count-ne1){throw 'Expected five active NPCs including different health profile'}
  $baseline[$r]=AiSnapshot $r 'before'
 }
 if(Compare-Object @($catalog.host.entries|ForEach-Object {$_.name+'|'+$_.definition}) @($catalog.client.entries|ForEach-Object {$_.name+'|'+$_.definition})){throw 'Peer authored catalogs differ'}
 # Record script-held living actors independently. Their flags and scheduler
 # membership must remain untouched while only eligible client actors pause.
 $script=@();$world=Q 'client' '--world-npcs'
 foreach($n in $world.npcs|Where-Object {$_.unique-and$_.spawnerName-like 'ld_spawner_vista*'}){
  foreach($k in 'reflex','attack'){$script+=@{handle=$n.handle;name=$n.spawnerName;kind=$k;before=(Q 'client' "--ai-$k" $n.handle)}}
 }
 Save "$out/script-before.json" $script
 $dirty=$true;C 'client' 'coop_npc_hold 12000';$h=WaitHold $true $true 'ready'
 if($h.actors-ne5-or$h.acquired-ne10-or$h.lost-or$h.released){throw 'Five-actor ownership did not acquire exactly ten references'}
 C 'client' 'coop_npc_hold 12000'
 $held=AiSnapshot 'client' 'held'
 foreach($a in $held){if($a.kind-eq'attack'){if($a.value.started-or$a.value.schedulerActive){throw 'Client attack started'}}elseif(!$a.value.started-or$a.value.schedulerActive-or$a.value.pauseReferences-ne1-or$a.value.pauseState-ne2){throw 'Client component not exclusively paused'}}
 $hostDuring=AiSnapshot 'host' 'client-held'
 foreach($a in $hostDuring|Where-Object kind -ne 'attack'){if(!$a.value.started-or!$a.value.schedulerActive-or$a.value.pauseReferences){throw 'Host authority unexpectedly paused'}}
 foreach($a in $script){$a.after=Q 'client' "--ai-$($a.kind)" $a.handle;if($a.before.flags-ne$a.after.flags-or$a.before.schedulerActive-ne$a.after.schedulerActive){throw 'Script-held component was disturbed'}}
 Save "$out/script-during.json" $script
 $h=H;Save "$out/hold-after-observations.json" $h
 if(!$h.active-or!$h.ready-or$h.acquired-ne10-or$h.refused-ne1){throw 'Duplicate arm altered ownership or batch expired before observations'}
 # Let the bounded lease expire by itself; then verify native scheduler recovery.
 $h=WaitHold $false $false 'expired';if($h.result-ne'expired'-or$h.released-ne10-or$h.lost-or$h.retired){throw 'Natural release failed'}
 Start-Sleep -Milliseconds 200
 foreach($r in 'host','client'){
  $after=AiSnapshot $r 'after';foreach($a in $after|Where-Object kind -ne 'attack'){if(!$a.value.started-or!$a.value.schedulerActive-or$a.value.pauseReferences-or$a.value.pauseState){throw 'Native scheduler did not recover'}}
  $cat=Q $r '--npc-authority';Save "$out/catalog-after-$r.json" $cat
  if(Compare-Object @($catalog[$r].entries|ForEach-Object {$_.handle.ToString()+'|'+($_.aiGenerations-join',')}) @($cat.entries|ForEach-Object {$_.handle.ToString()+'|'+($_.aiGenerations-join',')})){throw 'NPC component incarnation changed'}
 }
 C 'client' 'coop_npc_hold 12000';$h=WaitHold $true $true 'reacquired';C 'client' 'coop_npc_hold off';$h=WaitHold $false $false 'cancelled'
 if($h.result-ne'cancelled'-or$h.acquired-ne10-or$h.released-ne10-or$h.lost){throw 'Explicit cancellation failed'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  try{
   if($dirty){C 'client' 'coop_npc_hold off';Start-Sleep -Milliseconds 250}
   foreach($r in 'host','client'){
    if(!$pids.ContainsKey($r)){continue};$s=S $r;Save "$out/after-$r.json" $s
    if($s.localEpoch-ne$epochs[$r]-or$s.localGameLogicState-ne1-or$s.localPendingGameLogicState){throw 'World changed during ownership proof'}
    $inv=Q $r '--player-inventory';Save "$out/inventory-after-$r.json" $inv
    if(($inventory[$r].items|ConvertTo-Json -Depth 15 -Compress)-ne($inv.items|ConvertTo-Json -Depth 15 -Compress)){throw 'Inventory changed'}
    $hp=Q $r '--health' 1;Save "$out/health-after-$r.json" $hp;$before=Get-Content "$out/health-before-$r.json" -Raw|ConvertFrom-Json;if($hp.current-ne$before.current){throw 'Player health changed'}
   }
   foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
   Save "$out/restoration.json" @{passed=$true;nativeHold=(H)}
  }catch{Save "$out/restoration.json" @{passed=$false;message=$_.Exception.Message};if(!$failure){$failure=$_.Exception.Message}}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;scope='General native authored-NPC binding and client-side batch pause ownership, natural expiry, duplicate arm, explicit cancellation, script/host preservation. No new root/body/combat transport.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
