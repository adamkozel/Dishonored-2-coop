param([string]$Candidate='candidate-npc-combat-v884',[string]$Take='npc-combat-handshake-v885')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null
function Save($p,$v){$v|ConvertTo-Json -Depth 22|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Snap($name){$pair=@{};foreach($r in 'host','client'){$pair[$r]=@{combat=(S $r '.npc-combat.json');stream=(S $r '.npc-stream.json');bridge=(S $r)}};Save "$out/$name.json" $pair;return $pair}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Wrong candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'}
  if((S $r).localGameLogicState-ne1-or(S $r '.npc-stream.json').enabled){throw 'Restored gameplay with stream off required'}
 }
 $dirty=$true;foreach($r in 'host','client'){C $r 'coop_npc_stream on'}
 $watch=[Diagnostics.Stopwatch]::StartNew();do{
  $ready=$true;foreach($r in 'host','client'){$n=S $r '.npc-stream.json';if(!$n.enabled-or!$n.entries.Count-or@($n.entries|Where-Object {$_.enabled-and!$_.ready}).Count){$ready=$false}}
  if(!$ready){Start-Sleep -Milliseconds 80}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt8000)
 Snap 'stream-ready'|Out-Null;if(!$ready){throw 'Native stream not ready'}
 for($round=0;$round-lt4;$round++){
  $first=if($round%2){'client'}else{'host'};$second=if($first-eq'host'){'client'}else{'host'}
  C $first 'coop_npc_combat on';Start-Sleep -Milliseconds 750;C $second 'coop_npc_combat on'
  $watch.Restart();do{$ready=$true;foreach($r in 'host','client'){$n=S $r '.npc-combat.json';if(!$n.enabled-or$n.result-ne'active_native_melee'){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 80}}while(!$ready-and$watch.ElapsedMilliseconds-lt6000)
  $pair=Snap "round-$round";if(!$ready){throw "Handshake round $round failed"}
  foreach($r in 'host','client'){if($pair[$r].combat.committed-or$pair[$r].combat.applied-or$pair[$r].combat.deaths){throw 'Negotiation mutated combat outcomes'};C $r 'coop_npc_combat off'}
  Start-Sleep -Milliseconds 350
 }
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Snap 'failure-state'|Out-Null}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){foreach($r in 'host','client'){foreach($cmd in 'coop_npc_combat off','coop_npc_stream off'){try{C $r $cmd}catch{$errors.Add($_.Exception.Message)}}}}
  foreach($r in 'host','client'){try{$s=S $r;$n=S $r '.npc-stream.json';Save "$out/final-$r.json" @{bridge=$s;stream=$n;combat=(S $r '.npc-combat.json')};if($s.localGameLogicState-ne1-or!$s.remoteFresh-or$n.enabled-or@($n.entries|Where-Object {$_.references-or$_.lost}).Count){throw 'Final gameplay or ownership not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add("Protected file changed $($f.Path)")}}
  Save "$out/restoration.json" @{passed=($errors.Count-eq0);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;candidate=$pin.dll;scope='Four no-input negotiations with staggered starts alternating host/client first; no combat actions or world transitions.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
