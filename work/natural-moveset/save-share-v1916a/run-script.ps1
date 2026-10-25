param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='save-share-v1916a',[string]$Candidate='candidate-npc-combat-v1214km')
# 1916 (#225): does a reader that holds game.details without delete sharing lose the native save? The lab host (its own
# savegame folder, the Dunwall fixture) requests native autosaves with the lab fixture coop_autosave_probe force (1214km)
# while this script holds every slot's game.details open: first nothing held, then held without delete sharing (the
# reader of 1214kl and earlier), then held with delete sharing, then nothing held twice (both rotation slots written
# again). Each phase lists the slot folders (a listing opens no file) before and after: a landed save rewrites the slot's
# game.details, a lost one leaves a fresh game.details.temp. Passes when both unheld saves before and after land and the
# save held without delete sharing is lost. Writes only the lab host's autosave rotation slots; the restore relaunches the
# lab pair (Deploy-SourceClock) as the other takes do.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{phases=@()}
$saves="$root/work/lab-instances/host/base/savegame"
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
function C($cmd){& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/host/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $suffix"}
function Listing{$h=@{};foreach($d in Get-ChildItem -LiteralPath $saves -Directory -Filter 'SLOT*'){$f=@{};foreach($x in Get-ChildItem -LiteralPath $d.FullName -File){$f[$x.Name]=$x.LastWriteTimeUtc.Ticks};$h[$d.Name]=$f};$h}
function Changed($a,$b){@($b.Keys|Where-Object {$s=$_;$x=$a[$s];$y=$b[$s];!$x-or@($y.Keys|Where-Object {$x[$_]-ne$y[$_]}).Count}|Sort-Object)}
function Phase($name,$share){
 $before=Listing;$start=[DateTime]::UtcNow.Ticks;$handles=[Collections.Generic.List[object]]::new()
 $r0=S '.reset.json';$p0=$r0.autosavePair
 try{
  if($share){foreach($s in $before.Keys){$p=Join-Path $saves "$s/game.details";if(Test-Path -LiteralPath $p){$handles.Add([IO.File]::Open($p,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]$share))}}}
  C 'coop_autosave_probe force'
  $w=[Diagnostics.Stopwatch]::StartNew();$seenAt=$null
  do{Start-Sleep -Milliseconds 250;$now=Listing;$c=Changed $before $now;if($c.Count-and!$seenAt){$seenAt=$w.ElapsedMilliseconds}}while($w.ElapsedMilliseconds-lt15000-and(!$seenAt-or$w.ElapsedMilliseconds-$seenAt-lt6000))
 }finally{foreach($h in $handles){$h.Dispose()}}
 Start-Sleep 2;$after=Listing;$r1=S '.reset.json';$p1=$r1.autosavePair
 $slots=Changed $before $after;$rows=@()
 foreach($s in $slots){$x=$before[$s];$y=$after[$s];if(!$x){$x=@{}}
  $rows+=[ordered]@{slot=$s;checkpoint=($y['checkpoint.dat']-ne$x['checkpoint.dat']);details=($y['game.details']-ne$x['game.details']);temp=([bool]$y['game.details.temp']-and[int64]$y['game.details.temp']-ge$start)}}
 $o=[ordered]@{phase=$name;share="$share";held=$handles.Count;probeCalls=[int64]$p1.probeCalls-[int64]$p0.probeCalls;probeRefused=[int64]$p1.probeRefused-[int64]$p0.probeRefused
  slots=$rows;landed=[bool]@($rows|Where-Object {$_.details-and!$_.temp}).Count;lost=[bool]@($rows|Where-Object {$_.temp-and!$_.details}).Count
  nativeSlotBefore=$r0.nativeAutosaveSlot;nativeSlot=$r1.nativeAutosaveSlot;saveRoot=$r1.saveRoot;detailsReads=$r1.autosaveDetailsReads;detailsBusy=$r1.autosaveDetailsBusy}
 $notes.phases+=@($o);Save "$out/phase-$name.json" $o;$o}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 $s=S;if($s.localGameLogicState-ne1){throw 'Host lab instance not in gameplay'}
 $r=S '.reset.json';if($r.saveRoot-ne'savegame'){throw "Host save root is $($r.saveRoot), expected its own savegame"}
 $notes.candidate=$Candidate;$notes.map=$s.localMap
 $a=Phase 'control-before' $null
 $b=Phase 'held-without-delete-sharing' 'Read, Write'
 $c=Phase 'held-with-delete-sharing' 'Read, Write, Delete'
 $d=Phase 'control-after-1' $null
 $e=Phase 'control-after-2' $null
 foreach($x in $a,$b,$c,$d,$e){if($x.probeCalls-lt1){throw "The autosave probe was refused in phase $($x.phase)"}}
 if(!$a.landed){throw 'The unheld save before did not land'}
 if(!$b.lost){throw 'The save held without delete sharing was not lost'}
 if(!$d.landed-or!$e.landed){throw 'The unheld saves after did not land'}
}catch{$failure=$_.Exception.Message}
finally{
 $errors=[Collections.Generic.List[string]]::new()
 if($owned){$mutex.ReleaseMutex();$owned=$false
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$x=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($x.localGameLogicState-ne1-or!$x.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}}
 $mutex.Dispose()
 if(Test-Path $out){Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope='Native save and a held game.details (#225, 1214km, lab host on its own savegame, Dunwall fixture): unheld, held without and with delete sharing, unheld twice'})}}
Get-Content "$out/result.json";if($failure){throw $failure}
