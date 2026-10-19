param([string]$Take='sitting-survey-v1528a',[string]$Maps='campaign/edge_of_the_world/eotw.mapinfo',[int]$Samples=30,[int]$IntervalMs=2000,[string]$Candidate='candidate-npc-combat-v1214em')
# 1528 (#11 enemy_body_size_variants, seated): which NPCs the game counts as seated, per mission map, read with the
# native seated test's own input (probe --sitting, 1214em: a type-1 request in the NPC's navigation request list,
# TIProvide_ProvideTargetIsSitting 0x14082d200; chapter 100, confirmed static). Each map is loaded in both instances
# (low chaos, host Corvo, client Emily) and the host is sampled every IntervalMs for Samples samples while the players
# stay at the mission start. Read-only probes; Dunwall fixture restored.
# 1528b: the map rows are $mapRows (1528a: a list named $maps overwrote the -Maps parameter; PowerShell names ignore case).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$mapRows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='Neutral',[int]$ms=100,[int]$ly=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms -LeftY $ly|Out-Null}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMapId})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);return @{ready=$ready;log=@($log)}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='SittingSurvey1528';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 if((Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash-ne$pin.probe){throw "Expected the $Candidate probe"}
 Save "$out/identity.json" @{candidate=$Candidate;dll=$pin.dll;probe=$pin.probe;maps=$Maps;samples=$Samples;intervalMs=$IntervalMs}
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 $dirty=$true
 foreach($map in @($Maps-split';'|Where-Object {$_})){$tag=($map-replace'^campaign/','')-replace'[^a-z0-9]+','-'
  C host "campaign $map low corvo";C client "campaign $map low emily"
  $g=WaitGameplay 300000;Save "$out/$tag-load-log.json" $g.log;if(!$g.ready){$mapRows.Add([ordered]@{map=$map;loaded=$false});continue}
  $seen=@{};$trace=[Collections.Generic.List[object]]::new()
  for($k=0;$k-lt$Samples;$k++){$q=try{Q host '--sitting'}catch{$null};if(!$q){Start-Sleep -Milliseconds $IntervalMs;continue}
   $sit=@($q.npcs|Where-Object {$_.sitting});$trace.Add([ordered]@{k=$k;tick=$q.tick;npcs=@($q.npcs).Count;sitting=@($sit|ForEach-Object {$_.spawnerName})})
   foreach($n in $sit){$key=$n.spawnerName;if(!$seen.ContainsKey($key)){$seen[$key]=[ordered]@{spawnerName=$n.spawnerName;definition=$n.definition;unique=$n.unique;handle=$n.handle;first=$k;count=0;position=$n.position;requests=$n.requests}};$seen[$key].count++}
   Start-Sleep -Milliseconds $IntervalMs}
  Save "$out/$tag-trace.json" @($trace);$last=try{Q host '--sitting'}catch{$null};if($last){Save "$out/$tag-last.json" $last}
  $mapRows.Add([ordered]@{map=$map;loaded=$true;samples=$trace.Count;npcs=$(if($last){@($last.npcs).Count}else{$null});seated=@($seen.Values)})}
 Save "$out/survey.json" @($mapRows)
 if(!@($mapRows|Where-Object {$_.loaded}).Count){$failure='No map loaded'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;maps=@($mapRows);scope='Seated NPC survey with the native seated test input (type-1 navigation request), per map at the mission start; Dunwall fixture restored.'}
  [ordered]@{pid=$PID;operation='SittingSurvey1528';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json"|Select-Object -First 5;if($failure){throw $failure}
