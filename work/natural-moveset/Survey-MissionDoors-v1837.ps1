param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mission-doors-v1837a',[string]$Candidate='candidate-npc-combat-v1214jd',[string[]]$Maps=@('edge_of_the_world/eotw'))
# 1837 (#185): the native doors of mission maps in both lab worlds. Both lab games load each map with the native campaign
# command (host Corvo, client Emily); once both are in gameplay, the probe's --openables (every door with its state, lock,
# block, broken and link flags) and every entity named *key* are saved per world. Read-only on the maps; the lab is
# restored at the end (Deploy-SourceClock).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$Maps=@($Maps|ForEach-Object {$_-split','}|Where-Object {$_})
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=100){try{& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}catch{}}
function S($r){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
$pids=@{};foreach($r in 'host','client'){$pids[$r]=(Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json).pid}
function Q($r,$mode){for($a=0;$a-lt8;$a++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native read failed $r $mode"}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){
    {$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}}
    5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
    12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}
    3{I $r Right 250;Start-Sleep -Milliseconds 400;I $r A 400;$last[$r]=$w.ElapsedMilliseconds}
    14{if(!$s.localPendingGameLogicState){I $r $(if($r-eq'host'){'Left'}else{'Right'}) 200;Start-Sleep -Milliseconds 400;I $r A 300;$last[$r]=$w.ElapsedMilliseconds+1000}}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$results=[ordered]@{}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 if((Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash-ne$pin.probe){throw "Probe is not $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;maps=$Maps})
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 foreach($m in $Maps){$key=($m-split'/')[-1]
  try{C host "campaign campaign/$m.mapinfo low corvo";C client "campaign campaign/$m.mapinfo low emily"
   if(!(WaitGameplay 300000)){$results[$key]=[ordered]@{failure='no gameplay'};continue}
   Start-Sleep -Seconds 4;$row=[ordered]@{}
   foreach($r in 'host','client'){$o=@((Q $r '--openables').openables);Save "$out/openables-$key-$r.json" $o
    $k=@((Q $r '--entities').entities|Where-Object {"$($_.name)"-match'key'}|ForEach-Object {[ordered]@{name=$_.name;type=$_.type;handle=$_.handle;position=$_.position}});Save "$out/keys-$key-$r.json" $k
    $row[$r]=[ordered]@{doors=$o.Count;locked=@($o|Where-Object {$_.locked}).Count;blocked=@($o|Where-Object {$_.blocked}).Count;linked=@($o|Where-Object {$_.linked}).Count;keys=$k.Count}}
   $results[$key]=$row
  }catch{$results[$key]=[ordered]@{failure=$_.Exception.Message}}
  Save "$out/summary.json" $results}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){$errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;maps=@($results.Keys);scope='1837 (#185): the native doors (with lock flags) and key entities of mission maps in both lab worlds (read-only)'})
 $mutex.Dispose()}
if($failure){throw $failure};"SURVEYED $($results.Count) maps"
