param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mission-pickup-families-v1853a',[string]$Candidate='candidate-npc-combat-v1214jg',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[string]$Plan='pickups_fruit_banana_02_6:host:pickup,pickups_mana_potion_01_17:client:pickup')
# 1853 (#186): native pages are closed (B) after the Far Reach grant and before each case (in v1852a the grant left both
# worlds in a menu state, so the pull and the crouch never ran); the Far Reach stand is 3 m off and the pull passes on
# whichever shared pickup it commits.
# 1852 (#186): a case that is not shared in this load or fails is recorded and the next case runs; the take fails at
# the end if any case failed (mission-pickup-families-v1851a stopped on an item missing from that load's catalog).
# 1851 (#186): 1850 on 1214jg (Far Reach and theft rewards confirmable) with a farreach mode: the role is granted Far
# Reach with its pull upgrade (setup), stands 4 m off the pickup on eight bearings, aims at it natively and holds LT
# 1.5 s. A pickpocket stops pressing once the item left the thief's world. Each case keeps both worlds' coin-hook
# diagnostics, and the host's usable catalog is saved for the container cases.
# 1850 (#186): pickup families on a mission map, from either role. Both lab games load the mission and arm the product
# controller (shared pickups, door, usable and NPC sessions). -Plan lists cases name:role:mode (comma-separated):
#  - pickup: the role stands 1.1 m off the pickup on eight bearings, aims at it natively (Set-NativeInteractionAim) and
#    presses use (X) only while the native use focus is that pickup;
#  - pickpocket: the role crouches and stands 0.7 m off the NPC carrying the item (the nearest NPC) on eight bearings,
#    sweeps its yaw and presses use only while the use focus is that NPC.
# Each case must commit once in both worlds (shared pickup receipt awarded and acknowledged), leave both worlds (gone from
# the shared catalog and the entity list) and reach each player's own inventory: currency as money in both, stored
# items as +1 in both (each player's native capacity applies), food as each player's own native restore. Money,
# inventory and health are recorded before and after. The lab is restored at the end (Deploy-SourceClock).
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
# The inventory probe reads through the sword equipment, which a pickup animation interrupts (mission-key-pickup-door-v1848a):
# retry for up to 10 s.
function Inventory($r){for($t=0;$t-lt20;$t++){try{$i=Q $r '--player-inventory';$k=@{};foreach($x in @($i.items)){$k[$x.name]=$x.quantity};return $k}catch{Start-Sleep -Milliseconds 500}};$null}
function Money($r){
 $h=Q $r '--health' 1;$cs=Q $r '--components' 1;$i=@($cs.components|Where-Object type -eq '.?AVarkCpntInventory@@');if($i.Count-ne1-or$i[0].owner-ne$h.actor){throw 'Currency inventory ownership mismatch'}
 $o=Q $r '--object' $i[0].pointer;$w=@($o.words|Where-Object offset -eq '0x168');if($o.type-ne'.?AVarkCpntInventory@@'-or$w.Count-ne1){throw 'Currency inventory incarnation changed'}
 [long]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32)}
function Vitals($r){$t=$null;for($a=0;$a-lt10-and!$t;$a++){try{$t=@{money=(Money $r);health=(Q $r '--health' 1).current;inventory=(Inventory $r)}}catch{Start-Sleep -Milliseconds 500}};$t}
function Entry($r,$name){@((S $r '.coin-sync.json').entries|Where-Object name -eq $name)|Select-Object -First 1}
function Present($r,$name){[ordered]@{catalog=@((Q $r '--coin-catalog').items|Where-Object name -eq $name).Count;entity=@((Q $r '--entities').entities|Where-Object name -eq $name).Count}}
# 1133 (confirmed runtime): arkCpntPlayerSensorInteraction+0x80 high dword is the focused entity handle (0x3fff.... none).
$sensor=@{};$byHandle=@{}
function FocusInit($r){$pl=Q $r '--player';$ents=(Q $r '--entities').entities;$me=@($ents|Where-Object pointer -eq $pl.player);if($me.Count-ne1){throw "Player entity not unique $r"}
 $script:byHandle[$r]=@{};foreach($e in $ents){$script:byHandle[$r][[Convert]::ToUInt32(($e.handle-replace'^0x',''),16)]=$e}
 $s=@((Q $r '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@');if($s.Count-ne1){throw "Interaction sensor not unique $r"};$script:sensor[$r]=$s[0].pointer}
function Focus($r){$o=Q $r '--object' $script:sensor[$r];$w=@($o.words|Where-Object offset -eq '0x80');if($w.Count-ne1){throw 'Sensor word missing'};$h=[uint32]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32);$e=$script:byHandle[$r][$h];[ordered]@{handle=('0x{0:x}'-f$h);name=$e.name}}
function Crouched($r){try{6 -in @((Q $r '--player-states').active|ForEach-Object {$_.id})}catch{$null}}
function Crouch($r){for($a=0;$a-lt8;$a++){$c=Crouched $r;if($c){return $true};if($c-eq$false){I $r B 150};Start-Sleep -Milliseconds 900};$false}
function WaitReceipt($name,$ms){$w=[Diagnostics.Stopwatch]::StartNew();do{$ok=$true;foreach($q in 'host','client'){$e=Entry $q $name;if(!$e-or!$e.awarded-or!$e.ack){$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 100}}while(!$ok-and$w.ElapsedMilliseconds-lt$ms);$ok}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
function DoPickup($r,$item,$tries){FocusInit $r;$kp=$item.position
 foreach($bearing in 0,45,90,135,180,225,270,315){$ang=$bearing*[Math]::PI/180;$sx=[double]$kp[0]+1.1*[Math]::Cos($ang);$sy=[double]$kp[1]+1.1*[Math]::Sin($ang)
  C $r (P $sx $sy ([double]$kp[2]+1.7) ($bearing+180));Start-Sleep -Milliseconds 1500
  try{& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X ([double]$kp[0]) -Y ([double]$kp[1]) -Z ([double]$kp[2]) -Evidence "$out/aim-$($item.name)-$bearing.json"|Out-Null}catch{$tries.Add([ordered]@{bearing=$bearing;aim=$_.Exception.Message});continue}
  Start-Sleep -Milliseconds 300;$f=Focus $r;$at=(Q $r '--player').candidatePosition
  if($f.handle-ne$item.handle){$tries.Add([ordered]@{bearing=$bearing;stand=$at;focus=$f});continue}
  I $r X 200;$ok=WaitReceipt $item.name 4000;$tries.Add([ordered]@{bearing=$bearing;stand=$at;focus=$f;committed=$ok});if($ok){return $true}}
 $false}
function DoPickpocket($r,$item,$tries){FocusInit $r
 $npc=@((Q $r '--entities').entities|Where-Object {"$($_.type)"-match'arkNPC'}|Sort-Object {[Math]::Sqrt([Math]::Pow($_.position[0]-$item.position[0],2)+[Math]::Pow($_.position[1]-$item.position[1],2))})|Select-Object -First 1
 if(!$npc){return $false};$script:pickNpc=$npc.name
 foreach($bearing in 0,45,90,135,180,225,270,315){$ang=$bearing*[Math]::PI/180;$q=@((Q $r '--entities').entities|Where-Object name -eq $npc.name)|Select-Object -First 1;if(!$q){return $false};$np=$q.position
  $sx=[double]$np[0]+0.7*[Math]::Cos($ang);$sy=[double]$np[1]+0.7*[Math]::Sin($ang)
  foreach($dy in 0,-20,20){C $r (P $sx $sy ([double]$np[2]+1.0) ($bearing+180+$dy));Start-Sleep -Milliseconds 600;if(!(Crouch $r)){$tries.Add([ordered]@{bearing=$bearing;crouch=$false});continue}
   $f=Focus $r;$tries.Add([ordered]@{bearing=$bearing;yaw=$bearing+180+$dy;npc=$npc.name;npcAt=$np;focus=$f})
   if($f.name-ne$npc.name){continue}
   I $r X 200;$ok=WaitReceipt $item.name 4000;$gone=!@((Q $r '--entities').entities|Where-Object name -eq $item.name).Count;$tries[$tries.Count-1].committed=$ok;$tries[$tries.Count-1].goneLocally=$gone
   if($ok){return $true};if($gone){return $false}}}
 $false}
function Settle(){$w=[Diagnostics.Stopwatch]::StartNew();do{$ok=$true;foreach($q in 'host','client'){$st=S $q;if($st.localGameLogicState-ne1){$ok=$false;if($st.localGameLogicState-in 2,8,12){I $q B 200}}};if(!$ok){Start-Sleep -Milliseconds 1200}}while(!$ok-and$w.ElapsedMilliseconds-lt20000);$ok}
function Awarded($q){@((S $q '.coin-sync.json').entries|Where-Object {$_.awarded}|ForEach-Object {$_.name})}
function DoFarReach($r,$item,$tries){$kp=$item.position
 foreach($u in 'components/characters/player/base/attributes/grapple_pull_objects.inventoryitem','models/powers/grapple/grapple.inventoryitem'){C $r "coop_grant_named $u";Start-Sleep -Milliseconds 800;[void](Settle)}
 foreach($bearing in 0,45,90,135,180,225,270,315){$ang=$bearing*[Math]::PI/180;$sx=[double]$kp[0]+3.0*[Math]::Cos($ang);$sy=[double]$kp[1]+3.0*[Math]::Sin($ang);$was=@{host=(Awarded host);client=(Awarded client)}
  C $r (P $sx $sy ([double]$kp[2]+1.7) ($bearing+180));Start-Sleep -Milliseconds 1500
  try{& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X ([double]$kp[0]) -Y ([double]$kp[1]) -Z ([double]$kp[2]) -Evidence "$out/aim-farreach-$($item.name)-$bearing.json"|Out-Null}catch{$tries.Add([ordered]@{bearing=$bearing;aim=$_.Exception.Message});continue}
  $at=(Q $r '--player').candidatePosition;& "$lab/Send-D2Input.ps1" -Role $r -LeftTrigger 255 -DurationMs 1500|Out-Null
  $ok=WaitReceipt $item.name 5000;$gone=!@((Q $r '--entities').entities|Where-Object name -eq $item.name).Count
  $new=@{host=@(Awarded host|Where-Object {$was.host-notcontains$_});client=@(Awarded client|Where-Object {$was.client-notcontains$_})};[void](Settle)
  $tries.Add([ordered]@{bearing=$bearing;stand=$at;committed=$ok;goneLocally=$gone;newlyCommitted=$new})
  if($ok){return $true}
  $other=@($new.host|Where-Object {$new.client-contains$_});if($other.Count){$script:pulledInstead=$other[0];return $true}
  if($gone){return $false}}
 $false}
$cases=@($Plan-split','|Where-Object {$_}|ForEach-Object {$p=$_-split':';[ordered]@{name=$p[0];role=$p[1];mode=$p[2]}})
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;plan=$cases};$rows=[Collections.Generic.List[object]]::new();$notes.cases=$rows
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;plan=$cases})
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.coin-sync.json').ready-and(S client '.coin-sync.json').ready}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 $notes.catalog=@{host=(S host '.coin-sync.json').catalogCount;client=(S client '.coin-sync.json').catalogCount};if(!$ok){throw 'Shared pickups not ready in both worlds'}
 try{Save "$out/usable-catalog-host.json" (Q host '--usable-catalog')}catch{}
 $caseFailures=[Collections.Generic.List[string]]::new()
 foreach($case in $cases){$r=$case.role;$row=[ordered]@{case=$case};$rows.Add($row);$script:pickNpc=$null;$script:pulledInstead=$null
  try{if(!(Settle)){throw 'Both worlds not in gameplay before the case'}
  $item=@((Q $r '--entities').entities|Where-Object name -eq $case.name)|Select-Object -First 1;if(!$item){throw "Item $($case.name) not in the $r world"}
  $row.item=[ordered]@{handle=$item.handle;position=$item.position;type=$item.type};$row.entryBefore=@{host=(Entry host $case.name);client=(Entry client $case.name)}
  if(!$row.entryBefore.host-or!$row.entryBefore.client){throw "Item $($case.name) is not a shared pickup in both worlds"}
  $row.before=@{host=(Vitals host);client=(Vitals client)};$row.presentBefore=@{host=(Present host $case.name);client=(Present client $case.name)}
  $tries=[Collections.Generic.List[object]]::new();$row.tries=$tries
  $done=switch($case.mode){'pickpocket'{DoPickpocket $r $item $tries};'farreach'{DoFarReach $r $item $tries};default{DoPickup $r $item $tries}};$row.npc=$script:pickNpc
  foreach($q in 'host','client'){try{Copy-Item "$root/work/lab-instances/$q/coop-status.json.coin-hook.json" "$out/case-$($rows.Count)-$q.coin-hook.json"}catch{}};$row.hook=@{host=(S host '.coin-hook.json');client=(S client '.coin-hook.json')};$row.coinResult=@{host=(S host '.coin-sync.json').result;client=(S client '.coin-sync.json').result}
  if(!$done){throw "The $r never took $($case.name) ($($tries.Count) tries)"}
  if($case.mode-eq'pickpocket'){foreach($q in 'host','client'){if((Crouched $q)){I $q B 150}}}
  if($script:pulledInstead){$row.pulledInstead=$script:pulledInstead;$case=[ordered]@{name=$script:pulledInstead;role=$r;mode=$case.mode};$row.case=$case}
  Start-Sleep -Seconds 3
  $row.after=@{host=(Vitals host);client=(Vitals client)};$row.presentAfter=@{host=(Present host $case.name);client=(Present client $case.name)};$row.entryAfter=@{host=(Entry host $case.name);client=(Entry client $case.name)}
  foreach($q in 'host','client'){if($row.presentAfter[$q].catalog-or$row.presentAfter[$q].entity){throw "$($case.name) still in the $q world"}}
  $e=$row.entryAfter.host;$path=$e.item;$row.family=$e.family
  foreach($q in 'host','client'){$b=$row.before[$q];$a=$row.after[$q];$x=$row.entryAfter[$q]
   $countBefore=if($b.inventory){$b.inventory[$path]}else{'unread'};$countAfter=if($a.inventory){$a.inventory[$path]}else{'unread'}
   $row["outcome$q"]=[ordered]@{money=$a.money-$b.money;health=@($b.health,$a.health);item=@($countBefore,$countAfter);accepted=$x.accepted;nativeBefore=$x.before;nativeAfter=$x.after}
   if($e.family-eq1-and($a.money-$b.money)-ne[long]$x.accepted){throw "Currency differs on ${q}: money $($b.money) to $($a.money), accepted $($x.accepted)"}
   if(($e.family -in 4,14)-and$b.inventory-and$a.inventory-and[int]$countAfter-ne([int]$countBefore+[int]$x.accepted)){throw "Stored item differs on ${q}: $path $($b.inventory[$path]) to $($a.inventory[$path]), accepted $($x.accepted)"}}
   $row.passed=$true
  }catch{$row.failure=$_.Exception.Message;$caseFailures.Add("$($case.name): $($_.Exception.Message)")}
 }
 if($caseFailures.Count){throw ($caseFailures-join '; ')}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.coin-sync.json" "$out/$r.coin-sync.json"}catch{};try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="1853 (#186): pickup families on a mission map ($Plan); receipts, removal from both worlds and each player's inventory"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS pickup families $Plan"
