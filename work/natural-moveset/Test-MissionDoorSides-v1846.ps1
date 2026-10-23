param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mission-door-sides-v1846a',[string]$Candidate='candidate-npc-combat-v1214jf',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[string]$Door='script_door_overseersoutpost_3rdfloor_interrogationroom_01',[string]$Key='models/interactive/keys/eotw/key_overseersoutpost_interrogationroom.inventoryitem')
# 1846 (#185): door side corrections on a mission map. Both lab games load the mission and arm the product controller;
# the take waits until both door channels for the door are live. The host is granted the door's key (setup only) and
# unlocks it; the client's door must unlock too (1214jf). Then the two players stand on opposite sides of the door: for
# each pair of opposite stands around the hinge, each player sweeps its yaw until the native use focus is the door (no
# press). With both focused, both press use (X) back to back, so each world opens its door from its own side. 6 s later
# both doors must be open and both channels converged on the host's order (applied = revision, which requires the same
# open side); side corrections and native calls are recorded. The lab is restored at the end (Deploy-SourceClock).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$inv=[Globalization.CultureInfo]::InvariantCulture
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=100){try{& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}catch{}}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
$pids=@{};foreach($r in 'host','client'){$pids[$r]=(Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json).pid}
function Q($r,$mode,$arg=$null){for($a=0;$a-lt8;$a++){$raw=if($null-ne$arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native read failed $r $mode"}
function P($x,$y,$z,$yaw){'setviewpos {0} {1} {2} {3} 0' -f ([double]$x).ToString('F3',$inv),([double]$y).ToString('F3',$inv),([double]$z).ToString('F3',$inv),([double]$yaw).ToString('F2',$inv)}
function DoorState($r){$d=@((Q $r '--openables').openables|Where-Object {$_.name-eq$Door})|Select-Object -First 1;if(!$d){return $null};[ordered]@{handle=$d.handle;current=$d.current;pending=$d.pending;locked=$d.locked;position=$d.position}}
function Channel($r){try{$d=S $r '.door-sync.json';[ordered]@{session=$d.session.result;catalog=$d.catalogCount;entry=(@($d.entries|Where-Object {$_.name-eq$Door})|Select-Object -First 1)}}catch{$null}}
function KeyCount($r){$i=Q $r '--player-inventory';(@($i.items|Where-Object {$_.name-eq$Key})|Select-Object -First 1).quantity}
# 1133 (confirmed runtime): arkCpntPlayerSensorInteraction+0x80 high dword is the focused entity handle (0x3fff.... none).
$sensor=@{};$byHandle=@{};function FocusInit($r){$pl=Q $r '--player';$ents=(Q $r '--entities').entities;$me=@($ents|Where-Object pointer -eq $pl.player);$script:byHandle[$r]=@{};foreach($e in $ents){$script:byHandle[$r][[Convert]::ToUInt32(($e.handle-replace'^0x',''),16)]=$e};if($me.Count-ne1){throw "Player entity not unique $r"}
 $s=@((Q $r '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@');if($s.Count-ne1){throw "Interaction sensor not unique $r"};$script:sensor[$r]=$s[0].pointer}
function Focus($r){$o=Q $r '--object' $script:sensor[$r];$w=@($o.words|Where-Object offset -eq '0x80');if($w.Count-ne1){throw 'Sensor word missing'};[uint32]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32)}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
# The focus may report the door actor or its spawned visual child (named after the door).
function DoorFocus($r,$f,$handle){$e=$script:byHandle[$r][$f];$f-eq$handle-or($e-and"$($e.name)"-like"*$Door*")}
# Place $r at the stand and sweep its yaw until the native use focus is the door; returns the yaw or $null.
function FocusDoor($r,$sx,$sy,$z,$handle){$base=[Math]::Atan2([double]$hp[1]-$sy,[double]$hp[0]-$sx)*180/[Math]::PI
 foreach($dy in 0,20,-20,40,-40,60,-60){C $r (P $sx $sy $z ($base+$dy));Start-Sleep -Milliseconds 700;$f=Focus $r;$e=$script:byHandle[$r][$f];$script:focusTries.Add([ordered]@{role=$r;stand=@($sx,$sy);yaw=$base+$dy;focus=('0x{0:x}'-f$f);name=$e.name;type=$e.type})
  if(DoorFocus $r $f $handle){return $base+$dy}};$null}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;door=$Door;key=$Key};$focusTries=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;door=$Door})
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ch=@{host=(Channel host);client=(Channel client)};$ok=$ch.host.entry.enabled-and$ch.client.entry.enabled-and$ch.host.entry.ready-and$ch.client.entry.ready}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 $notes.channelsBefore=$ch;if(!$ok){throw "The door channel for $Door is not live in both worlds"}
 $notes.before=[ordered]@{host=(DoorState host);client=(DoorState client)};$hp=$notes.before.host.position
 # Setup: the host's key unlock, shared by 1214jf.
 C host "coop_grant_named $Key";Start-Sleep -Milliseconds 1500;if([int](KeyCount host)-lt1){throw 'The key was not granted'}
 $tries=[Collections.Generic.List[object]]::new();$done=$false
 :search foreach($o in @(@(-1.0,0),@(1.0,0),@(0,1.0),@(0,-1.0))){$x=[double]$hp[0]+$o[0];$y=[double]$hp[1]+$o[1];$base=[Math]::Atan2([double]$hp[1]-$y,[double]$hp[0]-$x)*180/[Math]::PI
  foreach($dy in 0,-25,25,-50,50){C host (P $x $y ([double]$hp[2]+1.6) ($base+$dy));Start-Sleep -Milliseconds 500;I host X 200;Start-Sleep -Milliseconds 1200
   $s=DoorState host;$tries.Add([ordered]@{offset=$o;yaw=$base+$dy;locked=$s.locked;current=$s.current})
   if(!$s.locked-or$s.current-ne0-or$s.pending-ne0){$done=$true;break search}}}
 $notes.unlockTries=@($tries);if(!$done){throw 'The host never unlocked the door'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250}while((DoorState client).locked-and$w.ElapsedMilliseconds-lt5000)
 $notes.unlocked=[ordered]@{host=(DoorState host);client=(DoorState client)};if($notes.unlocked.client.locked){throw 'The client door stayed locked'}
 if($notes.unlocked.host.current-ne0-or$notes.unlocked.client.current-ne0){throw 'The door opened during the unlock setup'}
 # Opposite stands: each player focuses the door from its side without pressing.
 foreach($r in 'host','client'){FocusInit $r};$handles=@{host=[Convert]::ToUInt32(([string]$notes.before.host.handle),10);client=[Convert]::ToUInt32(([string]$notes.before.client.handle),10)}
 $z=[double]$hp[2]+1.6;$pair=$null
 foreach($pr in @(@(@(0,1.1),@(0,-1.1)),@(@(1.1,0),@(-1.1,0)),@(@(0.5,1.1),@(0.5,-1.1)),@(@(-0.5,1.1),@(-0.5,-1.1)),@(@(1.1,0.5),@(-1.1,0.5)),@(@(1.1,-0.5),@(-1.1,-0.5)))){
  $ah=@([double]$hp[0]+$pr[0][0],[double]$hp[1]+$pr[0][1]);$ac=@([double]$hp[0]+$pr[1][0],[double]$hp[1]+$pr[1][1])
  $yh=FocusDoor host $ah[0] $ah[1] $z $handles.host;if($null-eq$yh){continue}
  $yc=FocusDoor client $ac[0] $ac[1] $z $handles.client;if($null-eq$yc){continue}
  if(!(DoorFocus host (Focus host) $handles.host)-or!(DoorFocus client (Focus client) $handles.client)){continue};$pair=[ordered]@{host=@($ah[0],$ah[1],$yh);client=@($ac[0],$ac[1],$yc)};break}
 $notes.focusTries=@($focusTries);if(!$pair){throw 'No pair of opposite stands focused the door for both players'}
 $notes.pair=$pair;$notes.channelsAtPress=@{host=(Channel host);client=(Channel client)}
 I host X 200;I client X 200
 $samples=[Collections.Generic.List[object]]::new();$w=[Diagnostics.Stopwatch]::StartNew()
 do{$samples.Add([ordered]@{ms=$w.ElapsedMilliseconds;host=(DoorState host);client=(DoorState client)});Start-Sleep -Milliseconds 300}while($w.ElapsedMilliseconds-lt6000)
 $notes.samples=@($samples)
 $notes.after=[ordered]@{host=(DoorState host);client=(DoorState client);channels=@{host=(Channel host);client=(Channel client)}}
 foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.door-sync.json" "$out/$r.door-sync.json"}catch{}}
 $a=$notes.after;$eh=$a.channels.host.entry;$ec=$a.channels.client.entry
 if(!$eh.enabled-or!$ec.enabled){throw "A door channel retired: host $($eh.result), client $($ec.result)"}
 if($a.host.current-ne$a.client.current){throw "Open state differs: host $($a.host.current), client $($a.client.current)"}
 if($eh.applied-ne$eh.revision-or$ec.applied-ne$ec.revision-or$eh.revision-ne$ec.revision){throw "Channels not converged: host $($eh.applied)/$($eh.revision), client $($ec.applied)/$($ec.revision)"}
 if(($samples|Where-Object {$_.host.current-ne0-or$_.client.current-ne0}).Count-eq0){throw 'Neither door moved after the presses'}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope='1846 (#185): both players use the same mission door from opposite sides back to back; both worlds converge on the host order (side corrections recorded)'})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS mission door sides converged"
