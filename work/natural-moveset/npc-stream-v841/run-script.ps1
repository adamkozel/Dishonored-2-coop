param([string]$Take='npc-stream-v841')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$before=@{};$inventory=@{};$health=@{};$catalog=@{};$samples=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 22|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 30};throw "Native query failed $r $mode"}
function GetNpcStream($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.npc-stream.json" -Raw|ConvertFrom-Json}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-stream-v840/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Wrong candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  $before[$r]=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;Save "$out/before-$r.json" $before[$r]
  if($before[$r].localGameLogicState-ne1-or$before[$r].localPendingGameLogicState){throw 'Gameplay required'}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r];$health[$r]=Q $r '--health' 1;Save "$out/health-before-$r.json" $health[$r]
  $catalog[$r]=Q $r '--npc-authority';Save "$out/catalog-before-$r.json" $catalog[$r];if($catalog[$r].entries.Count-ne5){throw 'Expected five native targets'}
 }
 $dirty=$true;foreach($r in 'host','client'){C $r 'coop_npc_stream on'}
 $watch=[Diagnostics.Stopwatch]::StartNew();do{
  $ready=$true;$pair=@{};foreach($r in 'host','client'){$n=GetNpcStream $r;$pair[$r]=$n;if(!$n.enabled-or$n.entries.Count-ne5-or@($n.entries|Where-Object {!$_.enabled-or!$_.ready}).Count){$ready=$false}}
  $samples.Add(@{tick=[Environment]::TickCount64;phase='negotiation';pair=$pair});if(!$ready){Start-Sleep -Milliseconds 80}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt8000)
 Save "$out/negotiated.json" $pair;if(!$ready){throw 'Five NPC channels did not negotiate'}
 $watch.Restart();do{
  $pair=@{};foreach($r in 'host','client'){$pair[$r]=GetNpcStream $r;if(!$pair[$r].enabled-or@($pair[$r].entries|Where-Object {!$_.enabled-or!$_.ready}).Count){throw 'NPC channel retired during quiet streaming'}}
  $samples.Add(@{tick=[Environment]::TickCount64;phase='streaming';pair=$pair});Start-Sleep -Milliseconds 200
 }while($watch.ElapsedMilliseconds-lt10000)
 foreach($r in 'host','client'){
  $n=GetNpcStream $r;Save "$out/stream-$r.json" $n
  foreach($e in $n.entries){if($e.poses-lt30){throw 'A native body stream did not sustain samples'};if($r-eq'client'-and($e.references-ne2-or$e.roots-lt30)){throw 'Client root/AI ownership missing'}}
  foreach($actor in $catalog[$r].entries){Save "$out/$r-$($actor.name)-skeleton.json" (Q $r '--skeleton' $actor.handle)}
  Save "$out/world-$r.json" (Q $r '--world-npcs')
 }
 $client=GetNpcStream 'client';if(!$client.framesOwned-or!$client.bodyApplied){throw 'Native frame/body hooks did not apply any stream'}
 Save "$out/coverage.json" @{bodyCallbacks=$client.bodyEntries;allActorBodyCallbacks=(@($client.bodyEntries|Where-Object applied -eq 0).Count-eq0);scope='Complete negotiated root/rig delivery; callback counts separately reveal native visibility/culling coverage. Visual review and dynamic movement remain required.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  try{
   if($dirty){foreach($r in 'host','client'){C $r 'coop_npc_stream off'};Start-Sleep -Milliseconds 300}
   foreach($r in 'host','client'){
    if(!$pids.ContainsKey($r)){continue};$n=GetNpcStream $r;Save "$out/final-stream-$r.json" $n
    if($n.enabled-or@($n.entries|Where-Object {$_.references-or$_.lost}).Count){throw 'NPC ownership not released'}
    $s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;Save "$out/after-$r.json" $s
    if($s.localEpoch-ne$before[$r].localEpoch-or$s.localGameLogicState-ne1-or$s.localPendingGameLogicState){throw 'World changed'}
    $inv=Q $r '--player-inventory';Save "$out/inventory-after-$r.json" $inv
    if(($inventory[$r].items|ConvertTo-Json -Depth 15 -Compress)-ne($inv.items|ConvertTo-Json -Depth 15 -Compress)){throw 'Inventory changed'}
    $hp=Q $r '--health' 1;Save "$out/health-after-$r.json" $hp;if($hp.current-ne$health[$r].current){throw 'Health changed'}
    $c=Q $r '--npc-authority';Save "$out/catalog-after-$r.json" $c;if($c.entries.Count-ne5){throw 'Native AI catalog did not recover'}
   }
   foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}}
   Save "$out/restoration.json" @{passed=$true}
  }catch{Save "$out/restoration.json" @{passed=$false;message=$_.Exception.Message};if(!$failure){$failure=$_.Exception.Message}}
  Save "$out/samples.json" @($samples);Save "$out/result.json" @{passed=(!$failure);failure=$failure;scope='Five-actor native Steam root/body negotiation and quiet streaming, per-actor native AI ownership and release. No claim for combat/outcomes, dynamic motion, visual quality or campaign lifecycle.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
