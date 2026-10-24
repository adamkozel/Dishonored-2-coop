param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mission-simultaneous-v1870a',[string]$Candidate='candidate-npc-combat-v1214jn',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('deaths','autosaves','manual')][string]$Case='deaths',[ValidateSet('host','client')][string]$ManualSaver='client')
# 1870 (#188): the saves are made at the docks-apartment stands of Test-MissionAutosavePair-v1156 (eye height 14.55);
# the take waits for each world's expected save count (autosaves: 2 each; manual: 1 each, the manual save is not an
# autosave) before the settle check, walks both players back to their mission-start positions and drops the host from
# 45 m above its own (mission-simultaneous-v1869b: the settle check passed at 0 saves, and the moved players stood over
# the harbour water, where a fall does not kill).
# 1869 (#188): simultaneous checkpoint and save requests on a mission map in product mode. Both lab games load the
# mission and arm the product controller (checkpoint sync in mission mode, paired autosaves); the take waits until both
# worlds' checkpoint sync is armed with the mission autosave. Then -Case:
#  - deaths: both players fall to their deaths at the same moment (setviewpos 45 m up, sent back to back) and both press
#    restart on their GameOver together;
#  - autosaves: both worlds issue a native autosave at the same moment (coop_autosave_probe, back to back), each
#    standing at its own spot; once the pair has settled, the host falls and both restart;
#  - manual: -ManualSaver saves through the pause menu (Save Game, first slot) while the other world issues a native
#    autosave; once the pair has settled, the host falls and both restart.
# Passed when exactly one transaction ran in each world (loads +1, completed +1, none aborted), the paired autosave
# counters settled equal (nothing pending) before the restart, and (autosaves, manual) each player came back within
# 1.5 m of where it stood at its last autosave. Health hold is off only for the falls. Lab restored at the end.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$inv=[Globalization.CultureInfo]::InvariantCulture
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=100){try{& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}catch{}}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
$pids=@{};foreach($r in 'host','client'){$pids[$r]=(Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json).pid}
function Q($r,$mode){for($a=0;$a-lt8;$a++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native read failed $r $mode"}
function P($x,$y,$z,$yaw){'setviewpos {0} {1} {2} {3} 0' -f ([double]$x).ToString('F3',$inv),([double]$y).ToString('F3',$inv),([double]$z).ToString('F3',$inv),([double]$yaw).ToString('F2',$inv)}
function Dist($a,$b){[Math]::Sqrt([Math]::Pow([double]$a[0]-[double]$b[0],2)+[Math]::Pow([double]$a[1]-[double]$b[1],2)+[Math]::Pow([double]$a[2]-[double]$b[2],2))}
function Reset($r){$x=S $r '.reset.json';$y=$x.autosavePair;[ordered]@{phase=$x.phase;result=$x.result;loads=$x.loads;completed=$x.completed;aborted=$x.aborted;requests=$x.requests;duplicates=$x.duplicates;
 saves=$y.saves;originated=$y.originated;mirrored=$y.mirrored;pending=$y.pending;pair=$y.result;slot=$y.slot;deathReady=$x.deathReady;peerDeathReady=$x.peerDeathReady;mission=$x.mission}}
function Pos($r){(Q $r '--player').candidatePosition}
function Settled(){$h=Reset host;$c=Reset client;$h.saves-eq$c.saves-and!$h.pending-and!$c.pending-and($h.originated+$h.mirrored)-eq$h.saves-and($c.originated+$c.mirrored)-eq$c.saves-and$h.originated-eq$c.mirrored-and$c.originated-eq$h.mirrored}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
# Both worlds to GameOver and back: returns the restart time; $press decides who presses restart (both, or the one that fell).
function DeathAndRestart([string[]]$fallers,[string[]]$pressers){
 $base=@{};foreach($r in 'host','client'){$base[$r]=@{epoch=(S $r).localEpoch;reset=(Reset $r)}}
 foreach($r in 'host','client'){C $r 'coop_player_health_hold off'};Start-Sleep -Milliseconds 400
 $cmds=@{};foreach($r in $fallers){$p=Pos $r;$cmds[$r]=P $p[0] $p[1] ([double]$p[2]+45) 0}
 foreach($r in $fallers){C $r $cmds[$r]}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250}while(!((S host).localGameLogicState-eq13-and(S client).localGameLogicState-eq13)-and$w.ElapsedMilliseconds-lt25000)
 $script:notes.deathScreensMs=$w.ElapsedMilliseconds;if(!((S host).localGameLogicState-eq13-and(S client).localGameLogicState-eq13)){throw 'Shared death screens not reached'}
 $script:notes.atDeath=@{host=(Reset host);client=(Reset client)}
 Start-Sleep -Seconds 2;foreach($r in $pressers){I $r A 200}
 $w=[Diagnostics.Stopwatch]::StartNew();$lastA=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 300;foreach($r in 'host','client'){$st=S $r;if($st.localGameLogicState-in8,13,15-and$w.ElapsedMilliseconds-$lastA[$r]-gt2500-and$w.ElapsedMilliseconds-gt6000){I $r A 300;$lastA[$r]=$w.ElapsedMilliseconds}}
  $done=(S host).localEpoch-ne$base.host.epoch-and(S client).localEpoch-ne$base.client.epoch-and(S host).localGameLogicState-eq1-and(S client).localGameLogicState-eq1-and!(S host).localPendingGameLogicState-and!(S client).localPendingGameLogicState}while(!$done-and$w.ElapsedMilliseconds-lt120000)
 $script:notes.restartMs=$w.ElapsedMilliseconds;if(!$done){throw 'The shared restart did not complete'}
 Start-Sleep -Seconds 4;foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 $after=@{host=(Reset host);client=(Reset client)};$script:notes.afterRestart=$after
 foreach($r in 'host','client'){$d=@{loads=$after[$r].loads-$base[$r].reset.loads;completed=$after[$r].completed-$base[$r].reset.completed;aborted=$after[$r].aborted-$base[$r].reset.aborted}
  $script:notes["transaction$r"]=$d;if($d.loads-ne1-or$d.completed-ne1-or$d.aborted){throw "The $r world ran $($d.loads) loads, $($d.completed) completions, $($d.aborted) aborts"}}}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;case=$Case;manualSaver=$ManualSaver}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;case=$Case;manualSaver=$ManualSaver})
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.reset.json').result-eq'armed'-and(S client '.reset.json').result-eq'armed'-and(Settled)}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 $notes.armed=@{host=(Reset host);client=(Reset client)};if(!$ok){throw 'Mission checkpoint sync not armed and settled in both worlds'}
 switch($Case){
  'deaths'{DeathAndRestart @('host','client') @('host','client')}
  default{
   # Each player stands at its own docks-apartment stand (proven floor, v1156), then the saves.
   $start=@{host=(Pos host);client=(Pos client)};$notes.start=$start
   $stands=@{host=@(-6.758,142.542,14.555);client=@(-3.436,140.039,14.550)}
   $spots=@{};foreach($r in 'host','client'){C $r (P $stands[$r][0] $stands[$r][1] $stands[$r][2] 90)};Start-Sleep -Seconds 2
   foreach($r in 'host','client'){$spots[$r]=Pos $r};$notes.spots=$spots
   $before=@{host=(Reset host);client=(Reset client)};$notes.beforeSaves=$before
   if($Case-eq'autosaves'){C host 'coop_autosave_probe';C client 'coop_autosave_probe'}
   else{$other=if($ManualSaver-eq'host'){'client'}else{'host'};$m=$ManualSaver
    # The manual save through the pause menu (Save Game, the second entry; the first slot), with the other world's
    # native autosave issued while the saver's pause menu is open.
    I $m Start 180;$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250}while((S $m).localGameLogicState-ne2-and$w.ElapsedMilliseconds-lt8000)
    foreach($k in 1..8){I $m Up 120;Start-Sleep -Milliseconds 250};I $m Down 200;Start-Sleep -Milliseconds 600;I $m A 250;Start-Sleep -Seconds 2
    C $other 'coop_autosave_probe'
    I $m A 250;Start-Sleep -Seconds 3;if((S $m).localGameLogicState-eq3){I $m A 250;Start-Sleep -Seconds 3}
    $notes.manualLogic=(S $m).localGameLogicState
    foreach($k in 1..4){if((S $m).localGameLogicState-eq1){break};I $m B 250;Start-Sleep -Milliseconds 1500}
    if((S $m).localGameLogicState-ne1){throw "The manual saver did not return to gameplay ($((S $m).localGameLogicState))"}}
   $expected=if($Case-eq'autosaves'){2}else{1}
   $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$h=Reset host;$c=Reset client;$ok=[int]$h.saves-ge$expected-and[int]$c.saves-ge$expected-and(Settled)}while(!$ok-and$w.ElapsedMilliseconds-lt30000)
   $notes.settled=@{ms=$w.ElapsedMilliseconds;host=(Reset host);client=(Reset client)};if(!$ok){throw 'The paired autosaves did not settle'}
   foreach($r in 'host','client'){C $r (P ([double]$start[$r][0]) ([double]$start[$r][1]) ([double]$start[$r][2]+1.7) 0)};Start-Sleep -Seconds 2
   DeathAndRestart @('host') @('host','client')
   $back=@{};foreach($r in 'host','client'){$p=Pos $r;$back[$r]=[ordered]@{position=$p;fromSpot=(Dist $p $spots[$r])}};$notes.back=$back
   foreach($r in 'host','client'){if($back[$r].fromSpot-gt1.5){throw "The $r came back $([Math]::Round($back[$r].fromSpot,2)) m from its saved spot"}}}
 }
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.reset.json" "$out/$r.reset-final.json"}catch{};try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="1870 (#188): simultaneous checkpoint and save requests ($Case) on a mission map in product mode"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS simultaneous $Case"
