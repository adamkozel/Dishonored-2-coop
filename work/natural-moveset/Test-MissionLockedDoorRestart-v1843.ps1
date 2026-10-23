param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mission-locked-door-restart-v1843a',[string]$Candidate='candidate-npc-combat-v1214jf',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('host','client')][string]$Source='client',[ValidateSet('host','client')][string]$Dying='host',[string]$Door='script_door_overseersoutpost_3rdfloor_interrogationroom_01',[string]$Key='models/interactive/keys/eotw/key_overseersoutpost_interrogationroom.inventoryitem')
# 1843 (#185): a shared key unlock across a shared checkpoint restart on a mission map. As 1842, the source unlocks the
# locked door with a granted key (setup only) and the other world's door must unlock too. Then -Dying falls to its death
# (health hold off, setviewpos 45 m up), both games restart from the last checkpoint (the mission start autosave, before
# the unlock) and the door session must renew. After the restart both worlds' door must be in the same lock and open
# state, and the renewed channel must share a second unlock (key granted again, since the restart restores the
# inventory) and the open that follows. The lab is restored at the end (Deploy-SourceClock).
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
function DoorState($r){$d=@((Q $r '--openables').openables|Where-Object {$_.name-eq$Door})|Select-Object -First 1;if(!$d){return $null};[ordered]@{current=$d.current;pending=$d.pending;locked=$d.locked;blocked=$d.blocked;linked=$d.linked;position=$d.position}}
function Channel($r){try{$d=S $r '.door-sync.json';[ordered]@{session=$d.session.result;rearms=$d.session.rearms;catalog=$d.catalogCount;entry=(@($d.entries|Where-Object {$_.name-eq$Door})|Select-Object -First 1)}}catch{$null}}
function KeyCount($r){$i=Q $r '--player-inventory';(@($i.items|Where-Object {$_.name-eq$Key})|Select-Object -First 1).quantity}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
function WaitChannels($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ch=@{host=(Channel host);client=(Channel client)};$ok=$ch.host.entry.enabled-and$ch.client.entry.enabled-and$ch.host.entry.ready-and$ch.client.entry.ready}while(!$ok-and$w.ElapsedMilliseconds-lt$limitMs);@{ok=$ok;channels=$ch}}
# Grant the key, press use around the door until the source's door unlocks, then wait for the other world's.
function Unlock($phase){$n=[ordered]@{phase=$phase;before=[ordered]@{host=(DoorState host);client=(DoorState client)}}
 C $Source "coop_grant_named $Key";Start-Sleep -Milliseconds 1500;$n.grant=(S $Source '.observer-camera.json').itemGrant.named;$n.keyBefore=KeyCount $Source
 if([int]$n.keyBefore-lt1){throw "The key was not granted ($phase, $($n.grant.result))"}
 $hp=$n.before[$Source].position;$tries=[Collections.Generic.List[object]]::new();$script:spot=$null
 :search foreach($o in @(@(1.0,0),@(-1.0,0),@(0,1.0),@(0,-1.0),@(0.8,0.8),@(-0.8,-0.8),@(0.8,-0.8),@(-0.8,0.8))){$x=[double]$hp[0]+$o[0];$y=[double]$hp[1]+$o[1];$base=[Math]::Atan2([double]$hp[1]-$y,[double]$hp[0]-$x)*180/[Math]::PI
  foreach($dy in 0,-25,25,-50,50){$view=P $x $y ([double]$hp[2]+1.6) ($base+$dy);C $Source $view;Start-Sleep -Milliseconds 500;I $Source X 200;Start-Sleep -Milliseconds 1200
   $s=DoorState $Source;$tries.Add([ordered]@{offset=$o;yaw=$base+$dy;locked=$s.locked;current=$s.current;pending=$s.pending})
   if(!$s.locked-or$s.current-ne0-or$s.pending-ne0){$script:spot=$view;$script:spotAt=@($x,$y,$base,[double]$hp[2]);break search}}}
 $n.tries=@($tries);if(!$script:spot){throw "The source's door never unlocked ($phase, $($tries.Count) presses)"}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250;$p=DoorState $peer}while($p.locked-and$w.ElapsedMilliseconds-lt5000)
 $n.unlocked=[ordered]@{host=(DoorState host);client=(DoorState client);peerUnlockMs=$w.ElapsedMilliseconds;channels=@{host=(Channel host);client=(Channel client)}}
 $script:notes[$phase]=$n;if($n.unlocked[$peer].locked){throw "The $peer's door stayed locked after the $Source's key unlock ($phase)"}}
$peer=if($Source-eq'host'){'client'}else{'host'};$other=if($Dying-eq'host'){'client'}else{'host'}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;source=$Source;dying=$Dying;door=$Door;key=$Key}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;source=$Source;dying=$Dying;door=$Door})
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 $c=WaitChannels 60000;$notes.channelsBefore=$c.channels;if(!$c.ok){throw "The door channel for $Door is not live in both worlds"}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.reset.json').result-eq'armed'-and(S client '.reset.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 $notes.checkpointArmed=@{host=(S host '.reset.json').result;client=(S client '.reset.json').result};if(!$ok){throw 'Mission checkpoint sync not armed in both worlds'}
 Unlock 'first'
 # The shared death and restart.
 $base=@{};foreach($r in 'host','client'){$base[$r]=@{epoch=(S $r).localEpoch;completed=(S $r '.reset.json').completed;rearms=(Channel $r).rearms}}
 foreach($r in 'host','client'){C $r 'coop_player_health_hold off'};Start-Sleep -Milliseconds 500
 $pos=(Q $Dying '--player').candidatePosition;C $Dying ("setviewpos {0} {1} {2} 0 0" -f ([double]$pos[0]).ToString('F3',$inv),([double]$pos[1]).ToString('F3',$inv),([double]$pos[2]+45).ToString('F3',$inv))
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250}while(!((S host).localGameLogicState-eq13-and(S client).localGameLogicState-eq13)-and$w.ElapsedMilliseconds-lt25000)
 $notes.deathScreensMs=$w.ElapsedMilliseconds;if(!((S host).localGameLogicState-eq13-and(S client).localGameLogicState-eq13)){throw "Shared death screens not reached ($Dying fell)"}
 Start-Sleep -Seconds 2;I $Dying A 200;Start-Sleep -Milliseconds 300;I $other A 200
 $w=[Diagnostics.Stopwatch]::StartNew();$lastA=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 300;foreach($r in 'host','client'){$st=S $r;if($st.localGameLogicState-in8,13,15-and$w.ElapsedMilliseconds-$lastA[$r]-gt2500-and$w.ElapsedMilliseconds-gt6000){I $r A 300;$lastA[$r]=$w.ElapsedMilliseconds}}
  $done=(S host).localEpoch-ne$base.host.epoch-and(S client).localEpoch-ne$base.client.epoch-and(S host).localGameLogicState-eq1-and(S client).localGameLogicState-eq1-and!(S host).localPendingGameLogicState-and!(S client).localPendingGameLogicState}while(!$done-and$w.ElapsedMilliseconds-lt120000)
 $notes.restartMs=$w.ElapsedMilliseconds;if(!$done){throw 'The shared checkpoint restart did not complete'}
 foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(Channel host).rearms-gt$base.host.rearms-and(Channel client).rearms-gt$base.client.rearms}while(!$ok-and$w.ElapsedMilliseconds-lt45000)
 $c=WaitChannels 30000;$notes.renewed=[ordered]@{rearms=@{host=(Channel host).rearms;client=(Channel client).rearms};completed=@{host=(S host '.reset.json').completed;client=(S client '.reset.json').completed};channels=$c.channels;doors=[ordered]@{host=(DoorState host);client=(DoorState client)}}
 if(!$ok-or!$c.ok){throw "The door session did not renew with the door live in both worlds (rearms $($notes.renewed.rearms.host)/$($notes.renewed.rearms.client))"}
 $d=$notes.renewed.doors;if($d.host.locked-ne$d.client.locked-or$d.host.current-ne$d.client.current){throw "Door differs after the restart: host locked $($d.host.locked) current $($d.host.current), client locked $($d.client.locked) current $($d.client.current)"}
 if(!$d[$Source].locked){throw 'The restart did not restore the locked door (the checkpoint is after the unlock)'}
 Unlock 'second'
 # As 1844: the open presses sweep the same yaws, 2 s after the unlock.
 $opened=$false;$tries=[Collections.Generic.List[object]]::new();$at=$script:spotAt;Start-Sleep -Seconds 2
 :open for($round=0;$round-lt2;$round++){foreach($dy in 0,-25,25,-50,50){C $Source (P $at[0] $at[1] ($at[3]+1.6) ($at[2]+$dy));Start-Sleep -Milliseconds 500;I $Source X 200;Start-Sleep -Milliseconds 1500
  $s=DoorState $Source;$tries.Add([ordered]@{yaw=$at[2]+$dy;locked=$s.locked;current=$s.current;pending=$s.pending});if($s.current-ne0-or$s.pending-ne0){$opened=$true;break open}}}
 $notes.openTries=@($tries);if(!$opened){throw "The source's unlocked door never opened ($($tries.Count) presses)"}
 Start-Sleep -Seconds 5
 $notes.after=[ordered]@{host=(DoorState host);client=(DoorState client);channels=@{host=(Channel host);client=(Channel client)}}
 $a=$notes.after
 if($a.host.locked-or$a.client.locked){throw "Lock after opening: host $($a.host.locked), client $($a.client.locked)"}
 if($a.host.current-ne2-or$a.client.current-ne2){throw "Open state after the restart: host $($a.host.current), client $($a.client.current)"}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.door-sync.json" "$out/$r.door-sync-final.json"}catch{};try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.reset.json" "$out/$r.reset-final.json"}catch{};try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="1843 (#185): a shared key unlock by the $Source, a shared checkpoint restart ($Dying fell), the renewed door channel and a second shared unlock and open"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS locked door across a shared restart"
