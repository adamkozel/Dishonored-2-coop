param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mission-locked-door-v1841a',[string]$Candidate='candidate-npc-combat-v1214jd',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('host','client')][string]$Source='client',[string]$Door='script_door_overseersoutpost_3rdfloor_interrogationroom_01',[string]$Key='models/interactive/keys/eotw/key_overseersoutpost_interrogationroom.inventoryitem')
# 1841 (#185): a locked door opened with its key on a mission map, from either role. Both lab games load the mission and arm
# the product controller; the source is granted the door's key (coop_grant_named, setup only), then stands around the door
# and presses use (X) until its own door unlocks or opens. Both worlds' door state (current, pending, locked) is read
# before, after each press and 5 s after. Passed when the source's door unlocked and the other world's door is unlocked
# too and in the same open state. The lab is restored at the end (Deploy-SourceClock).
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
function KeyCount($r){$i=Q $r '--player-inventory';(@($i.items|Where-Object {$_.name-eq$Key})|Select-Object -First 1).quantity}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
$peer=if($Source-eq'host'){'client'}else{'host'}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;source=$Source;door=$Door;key=$Key}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;source=$Source;door=$Door})
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'};Start-Sleep -Seconds 3
 $notes.before=[ordered]@{host=(DoorState host);client=(DoorState client)};if(!$notes.before.host-or!$notes.before.client){throw "Door $Door missing"}
 C $Source "coop_grant_named $Key";Start-Sleep -Milliseconds 1500;$notes.grant=(S $Source '.observer-camera.json').itemGrant.named;$notes.keyBefore=KeyCount $Source
 if([int]$notes.keyBefore-lt1){throw "The key was not granted ($($notes.grant.result))"}
 $hp=$notes.before[$Source].position;$tries=[Collections.Generic.List[object]]::new();$done=$false
 :search foreach($o in @(@(1.0,0),@(-1.0,0),@(0,1.0),@(0,-1.0),@(0.8,0.8),@(-0.8,-0.8),@(0.8,-0.8),@(-0.8,0.8))){$x=[double]$hp[0]+$o[0];$y=[double]$hp[1]+$o[1];$base=[Math]::Atan2([double]$hp[1]-$y,[double]$hp[0]-$x)*180/[Math]::PI
  foreach($dy in 0,-25,25,-50,50){C $Source (P $x $y ([double]$hp[2]+1.6) ($base+$dy));Start-Sleep -Milliseconds 500;I $Source X 200;Start-Sleep -Milliseconds 1200
   $s=DoorState $Source;$tries.Add([ordered]@{offset=$o;yaw=$base+$dy;locked=$s.locked;current=$s.current;pending=$s.pending})
   if(!$s.locked-or$s.current-ne0-or$s.pending-ne0){$done=$true;break search}}}
 $notes.tries=@($tries);if(!$done){throw "The source's door never unlocked or opened ($($tries.Count) presses)"}
 Start-Sleep -Seconds 5
 $notes.after=[ordered]@{host=(DoorState host);client=(DoorState client)};$notes.keyAfter=KeyCount $Source
 foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.doors.json" "$out/$r.doors.json"}catch{}}
 $a=$notes.after
 if($a[$peer].locked-ne$a[$Source].locked){throw "Lock differs: the $Source's door locked $($a[$Source].locked), the $peer's $($a[$peer].locked)"}
 if($a[$peer].current-ne$a[$Source].current){throw "Open state differs: the $Source's door $($a[$Source].current), the $peer's $($a[$peer].current)"}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="1841 (#185): a locked door opened with its key by the $Source on a mission map; both worlds' door state"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS locked door shared"
