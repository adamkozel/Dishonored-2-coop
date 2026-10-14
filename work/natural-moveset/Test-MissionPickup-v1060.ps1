param([string]$Take='mission-pickup-v1060',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Shared pickup sync on a real mission map (candidate1059, map-agnostic coin
# catalog). Both roles load Edge of the World through the native `campaign`
# command; a read-only census (--coin-catalog) must agree on both peers; the
# shared catalog handshake runs; alternating roles exercise native pickups
# (currency, plus food/ammunition when present) through the installed reward
# observer and Steam commits. Verified: receipts on both peers, object removal
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
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1059/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected candidate1059'}
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
 if($census.host.digest-ne$census.client.digest-or$census.host.eligible-ne$census.client.eligible-or$census.host.ambiguous-or!$census.host.eligible){throw "Mission census differs or is unusable: host $($census.host.eligible)/$($census.host.ambiguous) client $($census.client.eligible)"}
 foreach($r in 'host','client'){C $r 'coop_coin_sync on'}
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ok=$true;foreach($r in 'host','client'){if(!(S $r '.coin-sync.json').ready){$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 60}}while(!$ok-and$wait.ElapsedMilliseconds-lt8000)
 foreach($r in 'host','client'){Save "$out/armed-$r.json" (S $r '.coin-sync.json')}
 if(!$ok){throw "Mission catalog handshake failed: host $((S host '.coin-sync.json').result) client $((S client '.coin-sync.json').result)"}
 $h=S host '.coin-sync.json';$c=S client '.coin-sync.json'
 if($h.catalogCount-ne$census.host.eligible-or$c.catalogCount-ne$h.catalogCount-or$h.catalogId-ne$c.catalogId){throw 'Shared catalog differs from census or between peers'}
 $currency=@($h.entries|Where-Object family -eq 1);$other=@($h.entries|Where-Object {$_.family-in4,6})
 $plan=[Collections.Generic.List[object]]::new()
 if($currency.Count){$plan.Add([pscustomobject]@{name=$currency[0].name;quantity=$currency[0].quantity;family=1;role='host'})}
 if($currency.Count-gt1){$plan.Add([pscustomobject]@{name=$currency[1].name;quantity=$currency[1].quantity;family=1;role='client'})}
 if($other.Count){$plan.Add([pscustomobject]@{name=$other[0].name;quantity=$other[0].quantity;family=$other[0].family;role='host'})}
 if(!$plan.Count){throw 'No exercisable mission pickups'};Save "$out/plan.json" @($plan)
 $money=@{};foreach($r in 'host','client'){$money[$r]=Money $r;Save "$out/money-before-$r.json" $money[$r];Save "$out/inventory-before-$r.json" (Q $r '--player-inventory')}
 Capture 'pickups' (4+4*$plan.Count);Start-Sleep -Milliseconds 800
 $expected=0;$k=0
 foreach($entry in $plan){
  C $entry.role "coop_coin_sync exercise $($entry.name)"
  $wait.Restart();do{$done=$true;foreach($r in 'host','client'){$state=S $r '.coin-sync.json';$receipt=@($state.entries|Where-Object name -eq $entry.name);if(!$state.enabled){throw "Catalog stopped $r $($state.result)"};if($receipt.Count-ne1-or!$receipt[0].awarded-or!$receipt[0].ack){$done=$false}};if(!$done){Start-Sleep -Milliseconds 60}}while(!$done-and$wait.ElapsedMilliseconds-lt3500)
  foreach($r in 'host','client'){Save "$out/case-$k-$r.json" (S $r '.coin-sync.json')}
  if(!$done){throw "Mission pickup did not converge: $($entry.name)"}
  if($entry.family-eq1){$expected+=$entry.quantity}
  $removed=$true;foreach($r in 'host','client'){if(@((Q $r '--coin-catalog').items|Where-Object name -eq $entry.name).Count){$removed=$false}}
  $rows.Add(@{name=$entry.name;family=$entry.family;quantity=$entry.quantity;role=$entry.role;converged=$true;removedBothPeers=$removed});$k++
  if(!$removed){throw "Picked object still present: $($entry.name)"}
  Start-Sleep -Milliseconds 1500
 }
 foreach($c in @($captures)){if(!$c.WaitForExit(40000)-or$c.ExitCode){throw 'Recording failed'}};$captures.Clear()
 foreach($r in 'host','client'){$now=Money $r;Save "$out/money-after-$r.json" $now;Save "$out/inventory-after-$r.json" (Q $r '--player-inventory');Save "$out/committed-$r.json" (S $r '.coin-sync.json')
  if($now-ne$money[$r]+$expected){throw "Shared currency outcome differs on ${r}: $($money[$r]) + $expected -> $now"}}
 Save "$out/verified.json" @{passed=$true;cases=@($rows);catalog=$h.catalogCount;expectedCurrency=$expected;scope="Candidate1059: shared pickup catalog ($($h.catalogCount) entries) agreed by both peers on Edge of the World; alternating native pickup exercises converge with receipts, removal on both peers and shared currency."}
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
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Shared pickup sync on Edge of the World (candidate1059); Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
