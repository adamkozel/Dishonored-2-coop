param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='jindosh-v2045a',[string]$Candidate='candidate-jindosh-v2045',[string]$MapInfo='campaign/dust/dust.mapinfo',[ValidateSet('corvo','emily')][string]$HostCharacter='corvo',[ValidateSet('corvo','emily')][string]$ClientCharacter='emily',[switch]$Off)
# 2045 (#239, stage 8.4.3): one Jindosh lock answer for both players (jindosh_mirror.h, candidate jindosh-v2045). Both lab
# games load The Dust District and arm the product controller; after 45 s of gameplay the take requires:
#  - the host found the lock (ld_enigma_v2_kiscule) and its five answer lists are a rolled answer, sent at least 3 times;
#  - the partner applied them: its five lists equal the host's, its riddle (journal list 15, what the player reads) equals
#    the host's riddle, no journal list had another length, and it wrote at least one value (each world rolls its own);
#  - the world diff stays equal.
# -Off is the mutation twin: the host runs with coop_safe_codes off (each world keeps its own riddle, 0.9.6), and the take
# must fail. The lab is restored at the end (Deploy-SourceClock). Setup from Test-SafeCodes-v2039.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=100){try{& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}catch{}}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   switch($s.localGameLogicState){{$_-in 8,15}{if($w.ElapsedMilliseconds-gt5000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}};5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds};12{I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);$ready}
function Flat($lists){@($lists|ForEach-Object {$_})-join ','}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;off=[bool]$Off}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pinPath="$root/work/stage8/$Candidate/identity.json";$pin=Get-Content $pinPath -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;off=[bool]$Off})
 if($Off){C host 'coop_safe_codes off'}
 C host "campaign $MapInfo low $HostCharacter";C client "campaign $MapInfo low $ClientCharacter"
 if(!(WaitGameplay 420000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 Start-Sleep -Seconds 45
 $j=@{host=(S host '.jindosh.json');client=(S client '.jindosh.json')};Save "$out/jindosh-host.json" $j.host;Save "$out/jindosh-client.json" $j.client
 $notes.jindosh=[ordered]@{
  host=[ordered]@{result=$j.host.result;lockFound=$j.host.lockFound;seats=$j.host.seats;localValid=$j.host.localValid;sent=$j.host.sent;readFailures=$j.host.readFailures;lists=(Flat $j.host.local);riddle=(Flat $j.host.riddle)}
  client=[ordered]@{result=$j.client.result;lockFound=$j.client.lockFound;seats=$j.client.seats;received=$j.client.received;refused=$j.client.refused;listWrites=$j.client.listWrites;seatWrites=$j.client.seatWrites;journalWrites=$j.client.journalWrites;notReady=$j.client.notReady;journalMismatch=$j.client.journalMismatch;readFailures=$j.client.readFailures;lists=(Flat $j.client.local);hostLists=(Flat $j.client.host);riddle=(Flat $j.client.riddle)}}
 $wd=& "$root/tools/diag/Invoke-D2WorldDiff.ps1" -A host -B client -OutDir $out -Tag 'jindosh'
 $notes.worldDiff=[ordered]@{status=$wd.status;compared=$wd.comparedKeys;differences=$wd.differences}
 $h=$notes.jindosh.host;$c=$notes.jindosh.client;$problems=[Collections.Generic.List[string]]::new()
 if(!$j.host.lockFound-or!$j.host.localValid){$problems.Add("The host has no rolled lock (result $($h.result), seats $($h.seats))")}
 if($h.lists-ne$c.lists){$problems.Add("Answers differ: host [$($h.lists)], partner [$($c.lists)]")}
 if(!$h.riddle-or$h.riddle-ne$c.riddle){$problems.Add("Riddles differ: host [$($h.riddle)], partner [$($c.riddle)]")}
 if([int]$c.journalMismatch){$problems.Add("$($c.journalMismatch) partner journal lists had another length")}
 if(!$Off){
  if([int]$h.sent-lt3-or$c.result-ne'applied'){$problems.Add("Host sent $($h.sent), partner result $($c.result)")}
  if(!([int]$c.listWrites+[int]$c.journalWrites)){$problems.Add('The partner wrote nothing (its own roll equal to the host''s is not credible)')}
  if([int]$c.seatWrites-lt1){$problems.Add('No seat copy was written')}}
 if($wd.status-ne'EQUAL'){$problems.Add("World diff $($wd.status) ($($wd.differences) differences)")}
 $notes.problems=@($problems)
 if($problems.Count){throw ($problems-join '; ')}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{C $r 'coop_player_health_hold off';C $r 'coop_product_auto off'}catch{}};try{C host 'coop_safe_codes on'}catch{}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  $mutex.ReleaseMutex()}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="2045 (#239): one Jindosh lock answer for both players on $MapInfo$(if($Off){', mutation twin: safe codes off'})"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS Jindosh $MapInfo"
