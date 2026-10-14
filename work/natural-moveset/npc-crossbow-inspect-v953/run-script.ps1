param([ValidateSet('host','client')][string]$Source='host',[string]$Take='npc-combat-reload-v953')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$handles=@{};$captures=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 22|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 30};throw "Native query failed $r $mode"}
function Snap($name){$pair=@{};foreach($r in 'host','client'){$pair[$r]=@{combat=(S $r '.npc-combat.json');stream=(S $r '.npc-stream.json');health=(Q $r '--health' $handles[$r]);player=(Q $r '--health' 1);bridge=(S $r);damage=(S $r '.damage.json')}};Save "$out/$name.json" $pair;return $pair}
function Retreat($r){C $r 'setviewpos -28.0921 -153.962 68.1 270 0'}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v948/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Wrong candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}
  Save "$out/inventory-before-$r.json" (Q $r '--player-inventory');Save "$out/health-before-$r.json" (Q $r '--health' 1)
  $world=Q $r '--world-npcs';Save "$out/world-before-$r.json" $world
  $target=@($world.npcs|Where-Object spawnerName -eq 'ld_spawner_guard_escape_1');if($target.Count-ne1-or!$target[0].unique-or!$target[0].reciprocal){throw 'Target identity unavailable'}
  $handles[$r]=[Convert]::ToUInt32($target[0].handle.Replace('0x',''),16)
 }
 $dirty=$true
 # Ordinary sword hits need native alert combat context. Let each local AI
 # see its player, retreat, then transfer only its inactive attack scheduler.
 foreach($r in 'host','client'){
  C $r 'coop_damage_observe 60000';C $r 'setviewpos -42.9563 -122 62.7 90 0'
  $watch=[Diagnostics.Stopwatch]::StartNew();do{$a=Q $r '--ai-targets' $handles[$r];if($a.combat.resolved-and$a.combat.handle-eq1){break};Start-Sleep -Milliseconds 80}while($watch.ElapsedMilliseconds-lt3500)
  Save "$out/awareness-$r.json" $a;Retreat $r
  if(!$a.combat.resolved-or$a.combat.handle-ne1){throw "Native alert context unavailable $r"}
 }
 $watch.Restart();do{$idle=$true;foreach($r in 'host','client'){$a=Q $r '--ai-attack' $handles[$r];if($a.schedulerActive){$idle=$false}};if(!$idle){Start-Sleep -Milliseconds 100}}while(!$idle-and$watch.ElapsedMilliseconds-lt6000)
 if(!$idle){throw 'Native attack scheduler did not settle'}
 foreach($r in 'host','client'){C $r 'coop_npc_stream on'}
 $watch.Restart();do{
  $ready=$true;foreach($r in 'host','client'){$n=S $r '.npc-stream.json';$entry=@($n.entries|Where-Object handle -eq $handles[$r]);if(!$n.enabled-or$entry.Count-ne1-or!$entry[0].ready){$ready=$false}}
  if(!$ready){Start-Sleep -Milliseconds 100}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt8000)
 Snap 'stream-ready'|Out-Null;if(!$ready){throw 'Target did not enter generic NPC stream'}
 foreach($r in 'host','client'){C $r 'coop_npc_combat on'}
 $watch.Restart();do{$ready=$true;foreach($r in 'host','client'){$n=S $r '.npc-combat.json';if(!$n.enabled-or$n.result-ne'active_native_melee'){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 100}}while(!$ready-and$watch.ElapsedMilliseconds-lt7000)
 $before=Snap 'combat-ready';if(!$ready){throw 'Combat negotiation failed'}
 C host 'coop_npc_combat fixture40000';$watch.Restart();do{$fx=S host '.npc-combat.json';if($fx.fixture.ready){break};Start-Sleep -Milliseconds 80}while($watch.ElapsedMilliseconds-lt2500)
 Save "$out/source-fixture.json" $fx;if(!$fx.fixture.ready){throw "Damage fixture failed $($fx.fixture.result)"}
 C $Source 'coop_ranged_fixture crossbow_loaded';$watch.Restart();do{$f=S $Source '.ranged-fixture.json';if($f.result-eq'native_ranged_ready'){break};Start-Sleep -Milliseconds 60}while($watch.ElapsedMilliseconds-lt5000);if($f.result-ne'native_ranged_ready'){throw 'Ranged acquisition failed'}
 & "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 1500|Out-Null;Start-Sleep -Milliseconds 1800
 & "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 650|Out-Null;Start-Sleep -Milliseconds 1800
 Save "$out/weapon.json" (Q $Source '--player-crossbow')
 $rig=Q host '--npc-pose' $handles.host;$t=$rig.root;Save "$out/target-before.json" $rig
 $x=$t[0]-3*$t[3];$y=$t[1]-3*$t[4];$z=$t[2]+1.7;$yaw=[Math]::Atan2($t[4],$t[3])*180/[Math]::PI
 C $Source ("setviewpos {0} {1} {2} {3} 13.1"-f$x,$y,$z,$yaw);Start-Sleep -Milliseconds 350
 & "$lab/Send-D2Input.ps1" -Role $Source -LeftTrigger 255 -DurationMs 200|Out-Null
 Start-Sleep -Milliseconds 1200
 $after=Snap 'ranged-callbacks'
 $history=@($after[$Source].combat.sourceHistory|Where-Object nonce -eq $before[$Source].combat.nonce)
 if(!$history.Count){throw 'No native crossbow callback captured'}
 Save "$out/source-trace.json" $history
 $i=0;foreach($e in $history){$bytes=[Convert]::FromHexString($e.rawInfo);$ptr='0x'+[BitConverter]::ToUInt64($bytes,0).ToString('x');Save "$out/declaration-$i.json" (Q $Source '--object' $ptr);$i++}
 Save "$out/verified.json" @{scope='Diagnostic source identity only. Unsupported ranged profile is an expected rejection, not replication acceptance.';source=$Source;captures=$history.Count;result=$after[$Source].combat.result}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Snap 'failure-state'|Out-Null;foreach($r in 'host','client'){foreach($ai in 'decision','reflex'){Save "$out/failure-$r-$ai.json" (Q $r "--ai-$ai" $handles[$r])}}}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){foreach($r in 'host','client'){foreach($cmd in 'coop_ranged_fixture off','coop_npc_combat fixture_off','coop_npc_combat off','coop_npc_stream off','coop_checkpoint_sync off','coop_checkpoint_delay 0','coop_damage_observe off'){try{C $r $cmd}catch{$errors.Add("${r} $cmd $($_.Exception.Message)")}}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($p in $captures){try{if(!$p.WaitForExit(20000)){throw 'Capture did not finish'};if($p.ExitCode){throw 'Capture failed'}}catch{$errors.Add($_.Exception.Message)}}
  foreach($r in 'host','client'){try{
   $s=S $r;Save "$out/final-$r.json" $s;Save "$out/final-combat-$r.json" (S $r '.npc-combat.json');$n=S $r '.npc-stream.json';Save "$out/final-stream-$r.json" $n
   if($s.localGameLogicState-ne1-or!$s.remoteFresh-or$n.enabled-or@($n.entries|Where-Object {$_.references-or$_.lost}).Count){throw 'Final gameplay or ownership not restored'}
   $hp=Q $r '--health' 1;Save "$out/final-player-health-$r.json" $hp;if($hp.current-ne56){throw 'Player health not restored'};if((S $r '.npc-combat.json').fixture.lost){throw 'Fixture AI reference lost'}
  }catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add("Protected file changed $($f.Path)")}}
  Save "$out/restoration.json" @{passed=($errors.Count-eq0);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;candidate=$pin.dll;source=$Source;scope='Read-only source snapshots from genuine native crossbow callbacks; no ranged replication acceptance.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}






