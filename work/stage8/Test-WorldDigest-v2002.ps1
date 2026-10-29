param([string]$Take='world-digest-v2002a',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[string]$HostCharacter='corvo',[string]$ClientCharacter='emily',[int]$SettleSeconds=10)
# Stage 8.2 (#235): the difference detector on a mission map. Both roles load the map (product NPC stream on), the
# world settles, then each game gives three --world-digest snapshots 3 s apart (the stable-world gate) and
# tools/diag/d2_world_diff.py compares them. The mode is asserted first: two live peers, one map, both in gameplay.
# A run with nothing compared is SKIP, never a pass. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$diff=$null
$py='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe'
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Digest($tag){
 foreach($k in 1..3){foreach($r in 'host','client'){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] '--world-digest';if($LASTEXITCODE){throw "World digest failed ($r)"};Add-Content -LiteralPath "$out/digest-$tag-$r.jsonl" -Value $raw}
  if($k-lt3){Start-Sleep -Seconds 3}}
 & $py -I "$root/tools/diag/d2_world_diff.py" "$out/digest-$tag-host.jsonl" "$out/digest-$tag-client.jsonl" --out "$out/diff-$tag.json"|Set-Content "$out/diff-$tag.txt"
 Get-Content "$out/diff-$tag.json" -Raw|ConvertFrom-Json}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$PSScriptRoot/candidate-probe-digest-v2002/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected the 1214kq DLL'}
 if((Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash-ne$pin.probe){throw 'Expected the v2002 probe'}
 Save "$out/identity.json" @{dll=$pin.dll;dllCandidate=$pin.dllCandidate;probe=$pin.probe;mapInfo=$MapInfo;chaos=$Chaos;host=$HostCharacter;client=$ClientCharacter;pids=$pids}
 $dirty=$true
 C host "campaign $MapInfo $Chaos $HostCharacter";C client "campaign $MapInfo $Chaos $ClientCharacter"
 $w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{
  Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt240000)
 Save "$out/load-log.json" @($log)
 if(!$ready){throw 'Both instances did not reach mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_npc_stream on'}
 Start-Sleep -Seconds $SettleSeconds
 # Mode assertion: two live peers, one map, gameplay in both (connected is not live: transport must be fresh).
 $mode=@{};foreach($r in 'host','client'){$s=S $r;$mode[$r]=@{logic=$s.localGameLogicState;remoteFresh=$s.remoteFresh;map=(Q $r '--world').mapName}}
 Save "$out/mode.json" $mode
 if(!$mode.host.remoteFresh-or!$mode.client.remoteFresh){throw 'Transport not fresh'};if($mode.host.map-ne$mode.client.map){throw 'Peers on different maps'}
 $diff=Digest 'load'
 if($diff.status-eq'SKIP'){throw "Digest comparison skipped: $($diff.reason)"}
 Save "$out/verified.json" @{measured=$true;status=$diff.status;compared=$diff.comparedKeys;differences=$diff.differences;scope='Difference detector on a mission map after load; differences are findings, not failures of the take.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_npc_stream off'}catch{}}
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;status=$(if($diff){$diff.status}else{'none'});compared=$(if($diff){$diff.comparedKeys}else{0});differences=$(if($diff){$diff.differences}else{0});scope="Difference detector after loading $MapInfo ($HostCharacter host, $ClientCharacter client); Dunwall fixture restored."}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
