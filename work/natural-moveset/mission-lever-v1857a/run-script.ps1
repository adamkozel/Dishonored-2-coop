param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mission-lever-v1857a',[string]$Candidate='candidate-npc-combat-v1214jg',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('host','client')][string]$Source='client',[string]$Usable='script_lever_windturbine_1_1_1',[double]$Radius=40,[int]$WatchSeconds=8,[ValidateSet('corvo','emily')][string]$HostCharacter='corvo',[ValidateSet('corvo','emily')][string]$ClientCharacter='emily')
# 1857 (#187): a scripted lever on a mission map, from either role. Both lab games load the mission and arm the product
# controller (usable, door and NPC sessions; trigger, gameflow and script action mirrors). The entities and usables
# within -Radius of the lever and both worlds' story flag difference are read before; the source stands around the lever
# on eight bearings, aims at it natively and presses use (X) only while the use focus is the lever. -WatchSeconds later
# everything is read again. Reported per world: entities that moved, appeared or vanished and usables that changed state.
# Passed when the lever's state is equal in both worlds and every non-character change in the source's world also
# happened in the other world (same final position within 0.25 m, same usable state). The lab is restored at the end.
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
function Dist($a,$b){[Math]::Sqrt([Math]::Pow([double]$a[0]-[double]$b[0],2)+[Math]::Pow([double]$a[1]-[double]$b[1],2)+[Math]::Pow([double]$a[2]-[double]$b[2],2))}
function UsableState($q,$name){@((Q $q '--usable-catalog').usables|Where-Object name -eq $name)|Select-Object -First 1}
# Characters (players, partner presentations, NPCs) move on their own and are left out of the comparison.
function Snap($r,$at){$e=@{};foreach($x in @((Q $r '--entities').entities)){if($x.position-and(Dist $x.position $at)-le$Radius-and"$($x.type)"-notmatch'NPC|Player|Avatar'-and"$($x.name)"-notmatch'^models/characters/'){$e[$x.name]=$x.position}}
 $u=@{};foreach($x in @((Q $r '--usable-catalog').usables)){if((Dist $x.position $at)-le$Radius){$u[$x.name]=$x.current}};@{entities=$e;usables=$u}}
function Changes($a,$b){$c=[ordered]@{moved=@{};appeared=@();vanished=@();usables=@{}}
 foreach($k in $b.entities.Keys){if(!$a.entities.ContainsKey($k)){$c.appeared+=$k}elseif((Dist $a.entities[$k] $b.entities[$k])-gt0.05){$c.moved[$k]=$b.entities[$k]}}
 foreach($k in $a.entities.Keys){if(!$b.entities.ContainsKey($k)){$c.vanished+=$k}}
 foreach($k in $b.usables.Keys){if($a.usables.ContainsKey($k)-and$a.usables[$k]-ne$b.usables[$k]){$c.usables[$k]=$b.usables[$k]}};$c}
$sensor=@{};$byHandle=@{}
function FocusInit($r){$pl=Q $r '--player';$ents=(Q $r '--entities').entities;$me=@($ents|Where-Object pointer -eq $pl.player);if($me.Count-ne1){throw "Player entity not unique $r"}
 $script:byHandle[$r]=@{};foreach($e in $ents){$script:byHandle[$r][[Convert]::ToUInt32(($e.handle-replace'^0x',''),16)]=$e}
 $s=@((Q $r '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@');if($s.Count-ne1){throw "Interaction sensor not unique $r"};$script:sensor[$r]=$s[0].pointer}
function Focus($r){$o=Q $r '--object' $script:sensor[$r];$w=@($o.words|Where-Object offset -eq '0x80');if($w.Count-ne1){throw 'Sensor word missing'};$h=[uint32]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32);$e=$script:byHandle[$r][$h];[ordered]@{handle=('0x{0:x}'-f$h);name=$e.name}}
function Settle(){$w=[Diagnostics.Stopwatch]::StartNew();do{$ok=$true;foreach($q in 'host','client'){$st=S $q;if($st.localGameLogicState-ne1){$ok=$false;if($st.localGameLogicState-in 2,8,12){I $q B 200}}};if(!$ok){Start-Sleep -Milliseconds 1200}}while(!$ok-and$w.ElapsedMilliseconds-lt20000);$ok}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
$peer=if($Source-eq'host'){'client'}else{'host'}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;source=$Source;usable=$Usable;radius=$Radius;pairing="$HostCharacter-$ClientCharacter"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;source=$Source;usable=$Usable;pairing="$HostCharacter-$ClientCharacter"})
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host "campaign $MapInfo low $HostCharacter";C client "campaign $MapInfo low $ClientCharacter"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'};Start-Sleep -Seconds 5
 $u=UsableState $Source $Usable;if(!$u){throw "Usable $Usable not in the $Source world"};$up=$u.position
 $notes.channel=@{host=@((S host '.usable-sync.json').entries|Where-Object name -eq $Usable)|Select-Object -First 1;client=@((S client '.usable-sync.json').entries|Where-Object name -eq $Usable)|Select-Object -First 1}
 $before=@{host=(Snap host $up);client=(Snap client $up)};Save "$out/snap-before.json" $before
 try{& python -I "$root/tools/diag/d2_storyflags_diff.py" --out "$out/flags-before.json"|Out-Null}catch{}
 # The native use: stand 1.1 m off on eight bearings, aim at the lever, press only while it has the use focus.
 FocusInit $Source;$from=$u.current;$tries=[Collections.Generic.List[object]]::new();$notes.tries=$tries
 :use foreach($bearing in 0,45,90,135,180,225,270,315){$ang=$bearing*[Math]::PI/180;$sx=[double]$up[0]+1.1*[Math]::Cos($ang);$sy=[double]$up[1]+1.1*[Math]::Sin($ang)
  C $Source (P $sx $sy ([double]$up[2]+1.7) ($bearing+180));Start-Sleep -Milliseconds 1500
  foreach($h in 0.0,0.15,-0.15){try{& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Source -X ([double]$up[0]) -Y ([double]$up[1]) -Z ([double]$up[2]+$h) -Evidence "$out/aim-$bearing-$h.json"|Out-Null}catch{$tries.Add([ordered]@{bearing=$bearing;height=$h;aim=$_.Exception.Message});continue}
   Start-Sleep -Milliseconds 300;$f=Focus $Source;$at=(Q $Source '--player').candidatePosition
   if("$($f.name)"-ne$Usable){$tries.Add([ordered]@{bearing=$bearing;height=$h;stand=$at;focus=$f});continue}
   I $Source X 200;Start-Sleep -Milliseconds 2500;$now=UsableState $Source $Usable;$tries.Add([ordered]@{bearing=$bearing;height=$h;stand=$at;focus=$f;current=$now.current})
   if($now.current-ne$from){break use}}}
 if((UsableState $Source $Usable).current-eq$from){throw "The $Source never used $Usable ($($tries.Count) tries)"}
 # Watch both worlds; the source may stay near the lever.
 Start-Sleep -Seconds $WatchSeconds;[void](Settle)
 $after=@{host=(Snap host $up);client=(Snap client $up)};Save "$out/snap-after.json" $after
 try{& python -I "$root/tools/diag/d2_storyflags_diff.py" --out "$out/flags-after.json"|Out-Null}catch{}
 $ch=@{host=(Changes $before.host $after.host);client=(Changes $before.client $after.client)};$notes.changes=$ch
 $notes.lever=@{host=(UsableState host $Usable).current;client=(UsableState client $Usable).current;from=$from}
 $missing=[Collections.Generic.List[object]]::new()
 foreach($k in $ch[$Source].moved.Keys){$p=$ch[$peer].moved[$k];if(!$p-or(Dist $p $ch[$Source].moved[$k])-gt0.25){$missing.Add([ordered]@{entity=$k;source=$ch[$Source].moved[$k];peer=$after[$peer].entities[$k]})}}
 foreach($k in $ch[$Source].appeared){if($ch[$peer].appeared-notcontains$k){$missing.Add([ordered]@{appeared=$k})}}
 foreach($k in $ch[$Source].vanished){if($ch[$peer].vanished-notcontains$k){$missing.Add([ordered]@{vanished=$k})}}
 foreach($k in $ch[$Source].usables.Keys){if($after[$peer].usables[$k]-ne$after[$Source].usables[$k]){$missing.Add([ordered]@{usable=$k;source=$after[$Source].usables[$k];peer=$after[$peer].usables[$k]})}}
 $notes.unshared=@($missing)
 if($notes.lever.host-ne$notes.lever.client){throw "Lever state differs: host $($notes.lever.host), client $($notes.lever.client)"}
 if($missing.Count){throw "$($missing.Count) changes in the $Source's world did not happen in the $peer's"}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.usable-sync.json" "$out/$r.usable-sync.json"}catch{};try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="1857 (#187): the scripted lever $Usable used by the $Source on $MapInfo; the lever state and its consequences within $Radius m in both worlds"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS lever $Usable shared with its consequences"
