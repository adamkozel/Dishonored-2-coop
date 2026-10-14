param([string]$Take='mission-pickup-v1062',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Shared pickup sync on a real mission map (candidate1059, map-agnostic coin
# catalog). Both roles load Edge of the World through the native `campaign`
# command; a read-only census (--coin-catalog) must agree on both peers; the
# shared catalog handshake runs; alternating roles exercise native pickups
# (currency, plus food/ammunition when present) through the installed reward
# observer and Steam commits. v1061 (candidate1059b): the shared catalog is the
# per-entry intersection of both character worlds, and pickups use ordinary
# native interaction (stand at the item, aim with native look input, press X).
# v1062 (1059c): intersection from one reliable identity-list exchange; both
# peers must report the same shared digest and count. Verified: receipts on both peers, object removal
# on both, shared currency outcome. Dunwall fixture restored afterwards.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new();$dunwall=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Money($r){
 $h=Q $r '--health' 1;if($h.logic-ne1){throw 'Currency requires native gameplay'}
 $cs=Q $r '--components' 1;$i=@($cs.components|Where-Object type -eq '.?AVarkCpntInventory@@');if($i.Count-ne1-or$i[0].owner-ne$h.actor){throw 'Currency inventory ownership mismatch'}
 $o=Q $r '--object' $i[0].pointer;$owner=@($o.words|Where-Object offset -eq '0x10');$w=@($o.words|Where-Object offset -eq '0x168')
 if($o.type-ne'.?AVarkCpntInventory@@'-or$owner.Count-ne1-or$owner[0].word-ne$h.actor-or$w.Count-ne1){throw 'Currency inventory incarnation changed'}
 $n=[Convert]::ToUInt64($w[0].word.Substring(2),16);return [long]($n-shr32)
}
function Capture($name,$seconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$name-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$name-$r-capture.log" -PassThru))}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1059c/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected candidate1059c'}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  if((S $r).localGameLogicState-ne1){throw 'Gameplay required'};$dunwall[$r]=@{money=(Money $r);inventory=(Q $r '--player-inventory')};Save "$out/dunwall-before-$r.json" $dunwall[$r]}
 Save "$out/identity.json" @{dll=$pin.dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;mapInfo=$MapInfo}
 $dirty=$true
 C host "campaign $MapInfo $Chaos corvo";C client "campaign $MapInfo $Chaos emily"
 $w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{
  Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt240000)
 Save "$out/load-log.json" @($log)
 if(!$ready){throw 'Both instances did not reach mission gameplay'}
 Start-Sleep -Seconds 2
 if(!(S host).remoteFresh-or!(S client).remoteFresh){throw 'Transport not fresh on mission map'}
 # Read-only census on both peers: identical native catalogs are required.
 $census=@{};foreach($r in 'host','client'){$census[$r]=Q $r '--coin-catalog';Save "$out/census-$r.json" $census[$r]}
 $hn=@($census.host.items|ForEach-Object name);$cn=@($census.client.items|ForEach-Object name);$common=@($hn|Where-Object {$cn-contains$_})
 Save "$out/census-intersection.json" @{host=$census.host.eligible;client=$census.client.eligible;common=$common.Count;hostOnly=@($hn|Where-Object {$cn-notcontains$_});clientOnly=@($cn|Where-Object {$hn-notcontains$_})}
 if($census.host.ambiguous-or$census.client.ambiguous-or!$common.Count){throw 'Mission census unusable'}
 foreach($r in 'host','client'){C $r 'coop_coin_sync on'}
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ok=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 60}}while(!$ok-and$wait.ElapsedMilliseconds-lt8000)
 foreach($r in 'host','client'){Save "$out/armed-$r.json" (S $r '.coin-sync.json')}
 if(!$ok){throw "Mission catalog handshake failed: host $((S host '.coin-sync.json').result) client $((S client '.coin-sync.json').result)"}
 $h=S host '.coin-sync.json';$c=S client '.coin-sync.json'
 if(!$h.settled-or!$c.settled-or$h.sharedDigest-ne$c.sharedDigest-or$h.catalogCount-ne$c.catalogCount-or$h.catalogCount-lt[Math]::Floor(.9*$common.Count)-or$h.catalogCount-gt$common.Count+5){throw "Shared intersection differs: host $($h.catalogCount)/$($h.localOnly)/$($h.sharedDigest) client $($c.catalogCount)/$($c.localOnly)/$($c.sharedDigest) census common $($common.Count)"}
 $sharedNames=@($h.entries|ForEach-Object name)
 $spawn=@{};foreach($r in 'host','client'){$spawn[$r]=(Q $r '--player').candidatePosition}
 $items=@($census.host.items|Where-Object {$sharedNames-contains$_.name})
 function Dist($i){$dx=$i.position[0]-$spawn.host[0];$dy=$i.position[1]-$spawn.host[1];$dz=$i.position[2]-$spawn.host[2];[Math]::Sqrt($dx*$dx+$dy*$dy+$dz*$dz)}
 $currency=@($items|Where-Object type -eq 1|Sort-Object {Dist $_});$ammo=@($items|Where-Object type -eq 4|Sort-Object {Dist $_})
 $plan=[Collections.Generic.List[object]]::new()
 if($currency.Count){$plan.Add([pscustomobject]@{name=$currency[0].name;quantity=$currency[0].quantity;family=1;role='host';position=$currency[0].position})}
 if($currency.Count-gt1){$plan.Add([pscustomobject]@{name=$currency[1].name;quantity=$currency[1].quantity;family=1;role='client';position=$currency[1].position})}
 if($ammo.Count){$plan.Add([pscustomobject]@{name=$ammo[0].name;quantity=$ammo[0].quantity;family=4;role='host';position=$ammo[0].position})}
 if(!$plan.Count){throw 'No reachable shared pickups'};Save "$out/plan.json" @($plan)
 $money=@{};foreach($r in 'host','client'){$money[$r]=Money $r;Save "$out/money-before-$r.json" $money[$r];Save "$out/inventory-before-$r.json" (Q $r '--player-inventory')}
 $expected=0;$k=0
 foreach($entry in $plan){
  $r=$entry.role;$p=$entry.position;$sp=$spawn[$r];$dx=$sp[0]-$p[0];$dy=$sp[1]-$p[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001);$ux=$dx/$n;$uy=$dy/$n
  $yaw=[Math]::Atan2(-$uy,-$ux)*180/[Math]::PI
  C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f($p[0]+1.1*$ux),($p[1]+1.1*$uy),($p[2]+1.7),$yaw);Start-Sleep -Milliseconds 1300
  $v=Q $r '--player';$eye=$v.nativeViewPosition;$hd=[Math]::Sqrt(($p[0]-$eye[0])*($p[0]-$eye[0])+($p[1]-$eye[1])*($p[1]-$eye[1]));$pitch=[Math]::Max(-60,[Math]::Min(60,[Math]::Atan2($eye[2]-$p[2]-.05,$hd)*180/[Math]::PI))
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $r -Pitch $pitch -Evidence "$out/aim-$k.json" -Quick|Out-Null
  Save "$out/approach-$k.json" @{view=(Q $r '--player');item=$entry;pitch=$pitch}
  Capture "pickup-$k" 6;Start-Sleep -Milliseconds 800
  $done=$false
  for($attempt=0;$attempt-lt2-and!$done;$attempt++){
   I $r X (200+400*$attempt)
   $wait=[Diagnostics.Stopwatch]::StartNew();do{$done=$true;foreach($q in 'host','client'){$state=S $q '.coin-sync.json';$receipt=@($state.entries|Where-Object name -eq $entry.name);if(!$state.enabled){throw "Catalog stopped $q $($state.result)"};if($receipt.Count-ne1-or!$receipt[0].awarded-or!$receipt[0].ack){$done=$false}};if(!$done){Start-Sleep -Milliseconds 60}}while(!$done-and$wait.ElapsedMilliseconds-lt3000)
  }
  foreach($c in @($captures)){if(!$c.WaitForExit(30000)-or$c.ExitCode){throw 'Recording failed'}};$captures.Clear()
  foreach($q in 'host','client'){Save "$out/case-$k-$q.json" (S $q '.coin-sync.json')}
  $removed=$true;foreach($q in 'host','client'){if(@((Q $q '--coin-catalog').items|Where-Object name -eq $entry.name).Count){$removed=$false}}
  $rows.Add(@{name=$entry.name;family=$entry.family;quantity=$entry.quantity;role=$r;converged=$done;removedBothPeers=$removed});$k++
  if(!$done){throw "Ordinary pickup did not converge: $($entry.name) by $r"}
  if(!$removed){throw "Picked object still present: $($entry.name)"}
  if($entry.family-eq1){$expected+=$entry.quantity}
 }
 foreach($r in 'host','client'){$now=Money $r;Save "$out/money-after-$r.json" $now;Save "$out/inventory-after-$r.json" (Q $r '--player-inventory');Save "$out/committed-$r.json" (S $r '.coin-sync.json')
  if($now-ne$money[$r]+$expected){throw "Shared currency outcome differs on ${r}: $($money[$r]) + $expected -> $now"}}
 Save "$out/verified.json" @{passed=$true;cases=@($rows);catalog=$h.catalogCount;expectedCurrency=$expected;scope="Candidate1059c: shared pickup intersection ($($h.catalogCount) entries; host-only $($h.localOnly), client-only $($c.localOnly) left local) on Edge of the World; ordinary native pickups by alternating roles converge with receipts, removal on both peers and shared currency."}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};foreach($r in 'host','client'){try{Save "$out/failure-channel-$r.json" (S $r '.coin-sync.json')}catch{}}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(40000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_coin_sync off'}catch{}}
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp;money=(Money $r)};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'};if($dunwall[$r]-and(Money $r)-ne$dunwall[$r].money){throw 'Dunwall currency not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Shared pickup intersection with ordinary native pickups on Edge of the World (candidate1059c); Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
