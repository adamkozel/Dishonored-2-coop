param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='items-per-player-v2035a',[string]$Candidate='candidate-itemsperplayer-v2035',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('corvo','emily')][string]$HostCharacter='corvo',[ValidateSet('corvo','emily')][string]$ClientCharacter='emily',[string]$Plan='pickups_fruit_banana_02_6:client,pickups_mana_potion_01_17:host',[switch]$Shared)
# 2035 (#238): story cases name:role:story (a key, note, rewire tool or map): the role takes it, and within 30 s the
# other world's copy must be gone and the other player must hold the same item too (the story pickup catch-up).
# 2033 (#238, stage 8.4.2): items per player. Both lab games load a mission and arm the product controller; the host
# turns coop_items_per_player on and the partner must follow it (control flag 8). -Plan lists cases name:role: the role
# takes the pickup natively (1.1 m off it on eight bearings, aimed with Set-NativeInteractionAim, use only while the native
# use focus is that pickup), then the other player takes its own copy the same way. Each case must show:
#  - the taker's copy left only the taker's world: the other world still has the pickup until its player takes it;
#  - each take changed only the taker's own money or inventory;
#  - no shared catalog in either world (coin-sync enabled false, itemsPerPlayer true in both).
# -Shared is the mutation twin: items per player stays off (0.9.6 shared pickups), and the take must fail.
# The lab is restored at the end (Deploy-SourceClock). Based on Test-MissionPickupFamilies-v1855.
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
function Inventory($r){for($t=0;$t-lt20;$t++){try{$i=Q $r '--player-inventory';$k=@{};foreach($x in @($i.items)){$k[$x.name]=$x.quantity};return $k}catch{Start-Sleep -Milliseconds 500}};$null}
function Money($r){
 $h=Q $r '--health' 1;$cs=Q $r '--components' 1;$i=@($cs.components|Where-Object type -eq '.?AVarkCpntInventory@@');if($i.Count-ne1-or$i[0].owner-ne$h.actor){throw 'Currency inventory ownership mismatch'}
 $o=Q $r '--object' $i[0].pointer;$w=@($o.words|Where-Object offset -eq '0x168');if($o.type-ne'.?AVarkCpntInventory@@'-or$w.Count-ne1){throw 'Currency inventory incarnation changed'}
 [long]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32)}
function Vitals($r){$t=$null;for($a=0;$a-lt10-and!$t;$a++){try{$t=@{money=(Money $r);inventory=(Inventory $r)}}catch{Start-Sleep -Milliseconds 500}};$t}
function Present($r,$name){@((Q $r '--entities').entities|Where-Object name -eq $name).Count}
# A change of the player's own goods: money, or any inventory entry's count.
function Gain($b,$a){$g=[ordered]@{money=$a.money-$b.money;items=[ordered]@{}}
 if($b.inventory-and$a.inventory){foreach($k in @($a.inventory.Keys)+@($b.inventory.Keys)|Sort-Object -Unique){$d=[int]$a.inventory[$k]-[int]$b.inventory[$k];if($d){$g.items[$k]=$d}}}
 $g.any=($g.money-ne0)-or($g.items.Count-gt0);$g}
$sensor=@{};$byHandle=@{}
function FocusInit($r){$pl=Q $r '--player';$ents=(Q $r '--entities').entities;$me=@($ents|Where-Object pointer -eq $pl.player);if($me.Count-ne1){throw "Player entity not unique $r"}
 $script:byHandle[$r]=@{};foreach($e in $ents){$script:byHandle[$r][[Convert]::ToUInt32(($e.handle-replace'^0x',''),16)]=$e}
 $s=@((Q $r '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@');if($s.Count-ne1){throw "Interaction sensor not unique $r"};$script:sensor[$r]=$s[0].pointer}
function Focus($r){$o=Q $r '--object' $script:sensor[$r];$w=@($o.words|Where-Object offset -eq '0x80');if($w.Count-ne1){throw 'Sensor word missing'};$h=[uint32]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32);$e=$script:byHandle[$r][$h];[ordered]@{handle=('0x{0:x}'-f$h);name=$e.name}}
function WaitGone($r,$name,$ms){$w=[Diagnostics.Stopwatch]::StartNew();do{if(!(Present $r $name)){return $true};Start-Sleep -Milliseconds 200}while($w.ElapsedMilliseconds-lt$ms);$false}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
function Settle(){$w=[Diagnostics.Stopwatch]::StartNew();do{$ok=$true;foreach($q in 'host','client'){$st=S $q;if($st.localGameLogicState-ne1){$ok=$false;if($st.localGameLogicState-in 2,8,12){I $q B 200}}};if(!$ok){Start-Sleep -Milliseconds 1200}}while(!$ok-and$w.ElapsedMilliseconds-lt20000);$ok}
function DoPickup($r,$item,$tries){FocusInit $r;$kp=$item.position
 foreach($bearing in 0,45,90,135,180,225,270,315){$ang=$bearing*[Math]::PI/180;$sx=[double]$kp[0]+1.1*[Math]::Cos($ang);$sy=[double]$kp[1]+1.1*[Math]::Sin($ang)
  C $r (P $sx $sy ([double]$kp[2]+1.7) ($bearing+180));Start-Sleep -Milliseconds 1500
  try{& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X ([double]$kp[0]) -Y ([double]$kp[1]) -Z ([double]$kp[2]) -Evidence "$out/aim-$r-$($item.name)-$bearing.json"|Out-Null}catch{$tries.Add([ordered]@{role=$r;bearing=$bearing;aim=$_.Exception.Message});continue}
  Start-Sleep -Milliseconds 300;$f=Focus $r;$at=(Q $r '--player').candidatePosition
  if($f.handle-ne$item.handle){$tries.Add([ordered]@{role=$r;bearing=$bearing;stand=$at;focus=$f});continue}
  I $r X 200;$ok=WaitGone $r $item.name 4000;$tries.Add([ordered]@{role=$r;bearing=$bearing;stand=$at;focus=$f;goneLocally=$ok});if($ok){return $true}}
 $false}
function Other($r){if($r-eq'host'){'client'}else{'host'}}
$cases=@($Plan-split','|Where-Object {$_}|ForEach-Object {$p=$_-split':';[ordered]@{name=$p[0];role=$p[1];mode=$(if($p.Count-gt2){$p[2]}else{'own'})}})
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;plan=$cases;shared=[bool]$Shared};$rows=[Collections.Generic.List[object]]::new();$notes.cases=$rows
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pinPath="$root/work/movement-integration/$Candidate/identity.json";if(!(Test-Path $pinPath)){$pinPath="$root/work/stage8/$Candidate/identity.json"};$pin=Get-Content $pinPath -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;plan=$cases;shared=[bool]$Shared})
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host $(if($Shared){'coop_items_per_player off'}else{'coop_items_per_player on'})
 C host "campaign $MapInfo low $HostCharacter";C client "campaign $MapInfo low $ClientCharacter"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 # Shared pickups arm within 60 s when items per player is off; per player, neither world may arm them.
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$cs=@{host=(S host '.coin-sync.json');client=(S client '.coin-sync.json')}}while(!($cs.host.ready-and$cs.client.ready)-and$w.ElapsedMilliseconds-lt$(if($Shared){60000}else{20000}))
 $notes.coinSync=@{host=[ordered]@{itemsPerPlayer=$cs.host.itemsPerPlayer;enabled=$cs.host.enabled;ready=$cs.host.ready;result=$cs.host.result;refusals=$cs.host.itemsPerPlayerRefusals};client=[ordered]@{itemsPerPlayer=$cs.client.itemsPerPlayer;enabled=$cs.client.enabled;ready=$cs.client.ready;result=$cs.client.result;refusals=$cs.client.itemsPerPlayerRefusals}}
 $caseFailures=[Collections.Generic.List[string]]::new()
 if(!$cs.host.itemsPerPlayer-or!$cs.client.itemsPerPlayer){$caseFailures.Add("Items per player not on in both games (host $($cs.host.itemsPerPlayer), partner $($cs.client.itemsPerPlayer))")}
 if($cs.host.enabled-or$cs.client.enabled){$caseFailures.Add("Shared pickups armed (host $($cs.host.enabled), partner $($cs.client.enabled))")}
 foreach($case in $cases){$r=$case.role;$o=Other $r;$row=[ordered]@{case=$case};$rows.Add($row)
  try{if(!(Settle)){throw 'Both worlds not in gameplay before the case'}
  $item=@((Q $r '--entities').entities|Where-Object name -eq $case.name)|Select-Object -First 1;if(!$item){throw "Item $($case.name) not in the $r world"}
  $row.item=[ordered]@{handle=$item.handle;position=$item.position;type=$item.type};$row.presentBefore=@{host=(Present host $case.name);client=(Present client $case.name)}
  if(!$row.presentBefore.host-or!$row.presentBefore.client){throw "Item $($case.name) is not in both worlds before the case"}
  $tries=[Collections.Generic.List[object]]::new();$row.tries=$tries
  $v0=@{host=(Vitals host);client=(Vitals client)}
  if(!(DoPickup $r $item $tries)){throw "The $r never took $($case.name) ($($tries.Count) tries)"}
  Start-Sleep -Seconds 4;[void](Settle)
  $v1=@{host=(Vitals host);client=(Vitals client)};$row.presentAfterFirst=@{host=(Present host $case.name);client=(Present client $case.name)}
  $row.first=[ordered]@{taker=$r;takerGain=(Gain $v0[$r] $v1[$r]);otherGain=(Gain $v0[$o] $v1[$o])}
  if(!$row.first.takerGain.any){throw "The $r gained nothing from $($case.name)"}
  if($case.mode-ne'story'-and$row.first.otherGain.any){throw "The $o gained from the $r's take of $($case.name): money $($row.first.otherGain.money), items $(($row.first.otherGain.items|ConvertTo-Json -Compress))"}
  if($row.presentAfterFirst[$r]){throw "$($case.name) still in the $r world after its take"}
  if($case.mode-ne'story'-and!$row.presentAfterFirst[$o]){throw "$($case.name) left the $o world when the $r took it"}
  if($case.mode-eq'story'){
   $item=$row.first.takerGain.items.Keys|Select-Object -First 1;if(!$item){throw "The $r gained no inventory item from story pickup $($case.name)"}
   $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 1000;$gone=!(Present $o $case.name)}while(!$gone-and$w.ElapsedMilliseconds-lt30000)
   [void](Settle);$v2=@{host=(Vitals host);client=(Vitals client)};$row.story=[ordered]@{item=$item;goneMs=$w.ElapsedMilliseconds;gone=$gone;otherGain=(Gain $v1[$o] $v2[$o])}
   if(!$gone){throw "Story pickup $($case.name) still in the $o world 30 s after the $r took it"}
   if([int]$row.story.otherGain.items[$item]-lt1){throw "The $o did not receive $item when the $r took $($case.name)"}
   $row.passed=$true;continue}
  # The other player now takes its own copy.
  $oItem=@((Q $o '--entities').entities|Where-Object name -eq $case.name)|Select-Object -First 1
  if(!(DoPickup $o $oItem $tries)){throw "The $o never took its own $($case.name)"}
  Start-Sleep -Seconds 4;[void](Settle)
  $v2=@{host=(Vitals host);client=(Vitals client)};$row.presentAfterSecond=@{host=(Present host $case.name);client=(Present client $case.name)}
  $row.second=[ordered]@{taker=$o;takerGain=(Gain $v1[$o] $v2[$o]);otherGain=(Gain $v1[$r] $v2[$r])}
  if(!$row.second.takerGain.any){throw "The $o gained nothing from its own $($case.name)"}
  if($row.second.otherGain.any){throw "The $r gained from the $o's take of $($case.name)"}
  if($row.presentAfterSecond.host-or$row.presentAfterSecond.client){throw "$($case.name) still present after both takes"}
  $row.passed=$true
  }catch{$row.failure=$_.Exception.Message;$caseFailures.Add("$($case.name): $($_.Exception.Message)")}
 }
 if($caseFailures.Count){throw ($caseFailures-join '; ')}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.coin-sync.json" "$out/$r.coin-sync.json"}catch{};try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.pickup-catchup.json" "$out/$r.pickup-catchup.json"}catch{};try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}}
  try{C host 'coop_items_per_player off'}catch{}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="2035 (#238): items per player, story items to both on a mission map ($Plan$(if($Shared){', mutation twin: shared pickups'})); each player takes each item once in their own world"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS items per player $Plan"
