param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mission-key-pickup-door-v1848a',[string]$Candidate='candidate-npc-combat-v1214jf',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('host','client')][string]$Source='client',[string]$Door='script_door_streets_hypathiaapartment',[string]$KeyEntity='pickups_key_srteets_aftersquare_hypati')
# 1848 (#185): the pickup is proven by the key entity leaving the source's world (the inventory probe refused every read
# after the native pickup in mission-key-pickup-door-v1845a/v1847a; its error text is kept now), and the door's native key
# check (0x14074bea0) proves the key is held when the source's door unlocks.
# 1847 (#185): 1845 with the inventory read retried for 3 s (it failed right after the pickup press in
# mission-key-pickup-door-v1845a, while the native inventory changed) and the pickup tries kept on failure.
# 1845 (#185): a key picked up natively by either player, then its locked door, on a mission map. Both lab games load the
# mission and arm the product controller (door session, shared pickups); the take waits until both door channels for
# the door are live. The source stands around the key's pickup (eight bearings, 1.1 m off), aims at it natively
# (Set-NativeInteractionAim) and presses use (X) only while the native use focus is that pickup. The pickup must reach
# the source's inventory (a new models/interactive/keys item); its entity in both worlds is recorded. Then, as 1844, the
# source presses use around the door until its door unlocks, the other world's door must unlock within 5 s, and the
# source opens it (yaw sweep): both doors must be open 5 s later. No grant or fixture control after the setup.
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
function DoorState($r){$d=@((Q $r '--openables').openables|Where-Object {$_.name-eq$Door})|Select-Object -First 1;if(!$d){return $null};[ordered]@{current=$d.current;pending=$d.pending;locked=$d.locked;blocked=$d.blocked;linked=$d.linked;position=$d.position}}
function Channel($r){try{$d=S $r '.door-sync.json';[ordered]@{session=$d.session.result;catalog=$d.catalogCount;entry=(@($d.entries|Where-Object {$_.name-eq$Door})|Select-Object -First 1)}}catch{$null}}
function Keys($r){$i=$null;for($t=0;$t-lt10-and!$i;$t++){try{$i=Q $r '--player-inventory'}catch{Start-Sleep -Milliseconds 300}};if(!$i){throw "Native read failed $r --player-inventory (3 s)"};$k=@{};foreach($x in @($i.items|Where-Object {"$($_.name)"-like'models/interactive/keys/*'})){$k[$x.name]=$x.quantity};$k}
function KeysTry($r){try{Keys $r}catch{$raw=(& "$lab/bin/d2_runtime_probe.exe" $pids[$r] '--player-inventory' 2>&1|Out-String);@{error=$raw.Trim()}}}
function KeyEntities($r){@((Q $r '--entities').entities|Where-Object {$_.name-eq$KeyEntity}|ForEach-Object {[ordered]@{handle=$_.handle;position=$_.position}})}
# 1133 (confirmed runtime): arkCpntPlayerSensorInteraction+0x80 high dword is the focused entity handle (0x3fff.... none).
$sensor=@{};function FocusInit($r){$pl=Q $r '--player';$me=@((Q $r '--entities').entities|Where-Object pointer -eq $pl.player);if($me.Count-ne1){throw "Player entity not unique $r"}
 $s=@((Q $r '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@');if($s.Count-ne1){throw "Interaction sensor not unique $r"};$script:sensor[$r]=$s[0].pointer}
function Focus($r){$o=Q $r '--object' $script:sensor[$r];$w=@($o.words|Where-Object offset -eq '0x80');if($w.Count-ne1){throw 'Sensor word missing'};'0x{0:x}'-f([uint32]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32))}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
$peer=if($Source-eq'host'){'client'}else{'host'}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;source=$Source;door=$Door;keyEntity=$KeyEntity}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;source=$Source;door=$Door;keyEntity=$KeyEntity})
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ch=@{host=(Channel host);client=(Channel client)};$ok=$ch.host.entry.enabled-and$ch.client.entry.enabled-and$ch.host.entry.ready-and$ch.client.entry.ready}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 $notes.channelsBefore=$ch;if(!$ok){throw "The door channel for $Door is not live in both worlds"}
 $notes.before=[ordered]@{host=(DoorState host);client=(DoorState client);keysHost=(KeysTry host);keysClient=(KeysTry client);keyEntityHost=@(KeyEntities host);keyEntityClient=@(KeyEntities client)}
 if(!$notes.before.host.locked-or!$notes.before.client.locked){throw 'The door is not locked in both worlds at the start'}
 $key=@(KeyEntities $Source);if($key.Count-ne1){throw "Key pickup $KeyEntity not unique in the $Source's world ($($key.Count))"}
 # The native pickup: stand 1.1 m off on eight bearings, aim at the pickup, press use only while it has the use focus.
 FocusInit $Source;$kp=$key[0].position;$keysBefore=if($Source-eq'host'){$notes.before.keysHost}else{$notes.before.keysClient};$tries=[Collections.Generic.List[object]]::new();$notes.pickTries=$tries;$picked=$null
 :pick foreach($bearing in 0,45,90,135,180,225,270,315){$ang=$bearing*[Math]::PI/180;$sx=[double]$kp[0]+1.1*[Math]::Cos($ang);$sy=[double]$kp[1]+1.1*[Math]::Sin($ang)
  C $Source (P $sx $sy ([double]$kp[2]+1.7) ($bearing+180));Start-Sleep -Milliseconds 1500
  try{& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Source -X ([double]$kp[0]) -Y ([double]$kp[1]) -Z ([double]$kp[2]) -Evidence "$out/aim-$bearing.json"|Out-Null}catch{$tries.Add([ordered]@{bearing=$bearing;aim=$_.Exception.Message});continue}
  Start-Sleep -Milliseconds 300;$f=Focus $Source;$at=(Q $Source '--player').candidatePosition
  if($f-ne$key[0].handle){$tries.Add([ordered]@{bearing=$bearing;stand=$at;focus=$f});continue}
  I $Source X 200;Start-Sleep -Milliseconds 1500;$left=@(KeyEntities $Source).Count;$tries.Add([ordered]@{bearing=$bearing;stand=$at;focus=$f;keyEntitiesLeft=$left})
  if(!$left){$picked=$KeyEntity;break pick}}
 $notes.pickTries=@($tries);if(!$picked){throw "The $Source never picked up $KeyEntity ($($tries.Count) stands)"}
 Start-Sleep -Seconds 3
 $notes.picked=[ordered]@{item=$picked;keysSource=(KeysTry $Source);keysPeer=(KeysTry $peer);keyEntityHost=@(KeyEntities host);keyEntityClient=@(KeyEntities client)}
 # The door: press use around it until the source's door unlocks (as 1844).
 $hp=$notes.before[$Source].position;$tries=[Collections.Generic.List[object]]::new();$spotAt=$null
 :search foreach($o in @(@(1.0,0),@(-1.0,0),@(0,1.0),@(0,-1.0),@(0.8,0.8),@(-0.8,-0.8),@(0.8,-0.8),@(-0.8,0.8))){$x=[double]$hp[0]+$o[0];$y=[double]$hp[1]+$o[1];$base=[Math]::Atan2([double]$hp[1]-$y,[double]$hp[0]-$x)*180/[Math]::PI
  foreach($dy in 0,-25,25,-50,50){C $Source (P $x $y ([double]$hp[2]+1.6) ($base+$dy));Start-Sleep -Milliseconds 500;I $Source X 200;Start-Sleep -Milliseconds 1200
   $s=DoorState $Source;$tries.Add([ordered]@{phase='unlock';offset=$o;yaw=$base+$dy;locked=$s.locked;current=$s.current;pending=$s.pending})
   if(!$s.locked-or$s.current-ne0-or$s.pending-ne0){$spotAt=@($x,$y,$base);break search}}}
 $notes.unlockTries=@($tries);if(!$spotAt){throw "The source's door never unlocked ($($tries.Count) presses)"}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250;$p=DoorState $peer}while($p.locked-and$w.ElapsedMilliseconds-lt5000)
 $notes.unlocked=[ordered]@{host=(DoorState host);client=(DoorState client);peerUnlockMs=$w.ElapsedMilliseconds;channels=@{host=(Channel host);client=(Channel client)}}
 if($notes.unlocked[$peer].locked){throw "The $peer's door stayed locked after the $Source's key unlock"}
 $opened=$false;$tries=[Collections.Generic.List[object]]::new();Start-Sleep -Seconds 2
 :open for($round=0;$round-lt2;$round++){foreach($dy in 0,-25,25,-50,50){C $Source (P $spotAt[0] $spotAt[1] ([double]$hp[2]+1.6) ($spotAt[2]+$dy));Start-Sleep -Milliseconds 500;I $Source X 200;Start-Sleep -Milliseconds 1500
  $s=DoorState $Source;$tries.Add([ordered]@{phase='open';yaw=$spotAt[2]+$dy;locked=$s.locked;current=$s.current;pending=$s.pending});if($s.current-ne0-or$s.pending-ne0){$opened=$true;break open}}}
 $notes.openTries=@($tries);if(!$opened){throw "The source's unlocked door never opened ($($tries.Count) presses)"}
 Start-Sleep -Seconds 5
 $notes.after=[ordered]@{host=(DoorState host);client=(DoorState client);channels=@{host=(Channel host);client=(Channel client)};keysSource=(KeysTry $Source);keysPeer=(KeysTry $peer)}
 foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.door-sync.json" "$out/$r.door-sync.json"}catch{}}
 $a=$notes.after
 if($a.host.locked-or$a.client.locked){throw "Lock after opening: host $($a.host.locked), client $($a.client.locked)"}
 if($a.host.current-ne2-or$a.client.current-ne2){throw "Open state differs: host $($a.host.current), client $($a.client.current)"}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="1848 (#185): $KeyEntity picked up natively by the $Source, then $Door unlocked and opened by the $Source; the pickup, the unlock and the open state in both worlds"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS key pickup and locked door shared"
