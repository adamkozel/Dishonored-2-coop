param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='safe-codes-v2037a',[string]$Candidate='candidate-safecodes-v2037',[string]$MapInfo='campaign/institute/institute.mapinfo',[ValidateSet('corvo','emily')][string]$HostCharacter='corvo',[ValidateSet('corvo','emily')][string]$ClientCharacter='emily',[switch]$Off)
# 2037 (#239, stage 8.4.3): one safe code for both players. Both lab games load a mission with safes and arm the product
# controller; after the mission settles (30 s of gameplay) the take requires:
#  - the host found safe locks, and the partner's world has each of them with the host's Combination (safe-codes.json);
#  - the partner wrote the host's dial start positions (dialWrites or equal dials) and no safe dial differs in the world
#    diff (usables named *combinaison_numbers*);
#  - no lock, dial or text of the host is unknown to the partner.
# -Off is the mutation twin: the host runs with coop_safe_codes off (each world rolls its own codes, 0.9.6), and the
# take must fail. The lab is restored at the end (Deploy-SourceClock). Setup from Test-ItemsPerPlayer-v2033.
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
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{mapInfo=$MapInfo;off=[bool]$Off}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $pinPath="$root/work/movement-integration/$Candidate/identity.json";if(!(Test-Path $pinPath)){$pinPath="$root/work/stage8/$Candidate/identity.json"};$pin=Get-Content $pinPath -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;mapInfo=$MapInfo;off=[bool]$Off})
 if($Off){C host 'coop_safe_codes off'}
 C host "campaign $MapInfo low $HostCharacter";C client "campaign $MapInfo low $ClientCharacter"
 if(!(WaitGameplay 300000)){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'};foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}
 Start-Sleep -Seconds 30
 $sc=@{host=(S host '.safe-codes.json');client=(S client '.safe-codes.json')};Save "$out/safe-codes-host.json" $sc.host;Save "$out/safe-codes-client.json" $sc.client
 $notes.safeCodes=[ordered]@{host=[ordered]@{result=$sc.host.result;locks=$sc.host.locks;dials=$sc.host.dials;texts=$sc.host.texts;sent=$sc.host.sent;readFailures=$sc.host.readFailures}
  client=[ordered]@{result=$sc.client.result;locks=$sc.client.locks;dials=$sc.client.dials;texts=$sc.client.texts;hostLocks=$sc.client.hostLocks;codeWrites=$sc.client.codeWrites;positionWrites=$sc.client.positionWrites;dialWrites=$sc.client.dialWrites;journalWrites=$sc.client.journalWrites;unknownLocks=$sc.client.unknownLocks;unknownDials=$sc.client.unknownDials;unknownTexts=$sc.client.unknownTexts;readFailures=$sc.client.readFailures}}
 $wd=& "$root/tools/diag/Invoke-D2WorldDiff.ps1" -A host -B client -OutDir $out -Tag 'safes'
 $notes.worldDiff=[ordered]@{status=$wd.status;compared=$wd.comparedKeys;differences=$wd.differences}
 $dj=try{Get-Content "$out/diff-safes.json" -Raw|ConvertFrom-Json}catch{$null}
 $dialDiffs=@();if($dj){$dialDiffs=@($dj.categories.usables.differences|Where-Object {"$($_.key)"-match'combinaison_numbers'})};$notes.dialDifferences=@($dialDiffs|ForEach-Object {[ordered]@{key=$_.key;host=$_.host;client=$_.client}})
 $problems=[Collections.Generic.List[string]]::new()
 if(!$dj){$problems.Add('No world diff file')}
 if(!$sc.host.locks){$problems.Add("The host found no safe lock (result $($sc.host.result), read failures $($sc.host.readFailures))")}
 $hostCodes=@{};foreach($l in @($sc.host.locksDetail)){$hostCodes["$($l.entity)"]=$l.combination}
 $notes.codes=[ordered]@{host=@($sc.host.locksDetail);client=@($sc.client.locksDetail)}
 foreach($l in @($sc.client.locksDetail)){if($hostCodes.ContainsKey("$($l.entity)")-and$hostCodes["$($l.entity)"]-ne$l.combination){$problems.Add("Lock $($l.entity): host code $($hostCodes["$($l.entity)"]), partner code $($l.combination)")}}
 foreach($k in $hostCodes.Keys){if(!@($sc.client.locksDetail|Where-Object {"$($_.entity)"-eq$k}).Count){$problems.Add("Lock $k missing in the partner's status")}}
 if($dialDiffs.Count){$problems.Add("$($dialDiffs.Count) safe dials differ: $(($dialDiffs|ForEach-Object {$_.key})-join ', ')")}
 if([int]$sc.client.unknownLocks-or[int]$sc.client.unknownDials){$problems.Add("Unknown to the partner: $($sc.client.unknownLocks) locks, $($sc.client.unknownDials) dials")}
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
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="2037 (#239): one safe code for both players on $MapInfo$(if($Off){', mutation twin: safe codes off'})"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS safe codes $MapInfo"
