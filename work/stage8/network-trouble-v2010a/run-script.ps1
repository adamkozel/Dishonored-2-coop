param([string]$Take='network-trouble-v2007a',[string]$Candidate='candidate-control-v2007',[string[]]$Profiles=@('lan','good_wifi','bad_wifi','mobile_tether','transatlantic','hell'),[int]$PhaseSeconds=40,[int]$BlackoutMs=5000,[int]$Seed=8306)
# Stage 8.3 (#236): network trouble with the control channel (2007: v2006 plus each game's control status per phase and a
# version-mismatch case at the end: the client restarted announcing control protocol 99 must be refused by the host).
# Stage 8.3 (#236): network trouble. coop-anything's chaos relay (vendored, MIT) sits on the lab's loopback leg: it
# listens on 31427 and forwards to the host's 31426; the client is restarted with -PeerPort 31427 (D2COOP_LAB_PEER_PORT)
# so all its traffic crosses the relay. The fixture script brings both games into Dunwall gameplay, then for each profile: $PhaseSeconds of play, the
# relay's own stats (proof the impairment happened), transport freshness, and the difference detector. Then a
# $BlackoutMs two-way partition: the time until both transports are fresh again and the worlds compare equal.
# Each game dumps its flight recorder at the end. The lab is redeployed (Deploy-SourceClock) afterwards.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$rows=[Collections.Generic.List[object]]::new();$relay=$null
$py='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe';$chaos="$lab/third_party/coop-anything/chaos/chaos.py"
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Rpc($method,$arg='{}'){$raw=& $py -I $chaos call --rpc-port 31428 $method $arg;if($LASTEXITCODE){throw "Relay call $method failed"};$raw|ConvertFrom-Json}
function Pids(){foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$script:pids[$r]=$l.pid}}
function Ctl($r){try{Get-Content "$root/work/lab-instances/$r/coop-status.json.control.json" -Raw|ConvertFrom-Json}catch{$null}}
function Fresh(){(S host).remoteFresh-and(S client).remoteFresh}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$PSScriptRoot/$Candidate/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$lab/bin/dinput8.dll").Hash-ne$pin.dll){throw "Expected the $Candidate DLL staged in bin"}
 Save "$out/identity.json" @{dll=$pin.dll;candidate=$pin.candidate;map='dunwall fixture';profiles=$Profiles;phaseSeconds=$PhaseSeconds;blackoutMs=$BlackoutMs;seed=$Seed}
 $dirty=$true
 # The relay first, then both games: the host on 31426, the client through the relay.
 $relay=Start-Process $py -ArgumentList @('-I',$chaos,'relay','--target','127.0.0.1:31426','--listen','31427','--rpc-port','31428','--profile','lan','--seed',[string]$Seed,'--log',"$out/relay.jsonl",'--info-file',"$out/relay-info.json") -WindowStyle Hidden -PassThru -RedirectStandardError "$out/relay-stderr.txt" -RedirectStandardOutput "$out/relay-stdout.txt"
 $w=[Diagnostics.Stopwatch]::StartNew();while(!(Test-Path "$out/relay-info.json")-and$w.ElapsedMilliseconds-lt10000){Start-Sleep -Milliseconds 200};if(!(Test-Path "$out/relay-info.json")){throw 'Relay did not start'}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
  if($p){if($p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};Stop-Process -Id $p.Id;if(!$p.WaitForExit(15000)){throw 'Game did not exit'}}}
 Start-Sleep -Milliseconds 500
 & "$lab/Start-D2Lab.ps1" -Role host -AssetExperiment|Out-Null
 & "$lab/Start-D2Lab.ps1" -Role client -AssetExperiment -PeerPort 31427|Out-Null
 Pids;Save "$out/pids.json" $pids
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 1000;$ok=$true;foreach($r in 'host','client'){try{$s=S $r;if($s.connectionState-ne3){$ok=$false}}catch{$ok=$false}}}while(!$ok-and$w.ElapsedMilliseconds-lt180000)
 if(!$ok){throw 'The client did not connect through the relay'}
 $st=Rpc 'stats';Save "$out/relay-connect-stats.json" $st
 # From a fresh start the fixture script brings both games into Dunwall gameplay (campaign from the main menu did not,
 # take network-trouble-v2006a).
 & "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/prepared-setup.json"|Out-Null
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ready=$true;foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}}}while(!$ready-and$w.ElapsedMilliseconds-lt120000)
 if(!$ready){throw 'Both instances did not reach gameplay after the fixture'}
 foreach($r in 'host','client'){I $r;C $r 'coop_npc_stream on'}
 Start-Sleep -Seconds 8
 if(!(Fresh)){throw 'Transport not fresh before the first profile'}
 foreach($profile in $Profiles){
  $before=Rpc 'stats';Rpc 'set_profile' ('{"profile":"'+$profile+'"}')|Out-Null
  $samples=[Collections.Generic.List[object]]::new();$w=[Diagnostics.Stopwatch]::StartNew()
  while($w.ElapsedMilliseconds-lt($PhaseSeconds*1000)){$h=S host;$c=S client;$samples.Add(@{ms=$w.ElapsedMilliseconds;hostFresh=$h.remoteFresh;clientFresh=$c.remoteFresh;hostRejected=$h.rejected;clientRejected=$c.rejected});Start-Sleep -Milliseconds 1000}
  $after=Rpc 'stats';Rpc 'set_profile' '{"profile":"lan"}'|Out-Null;Start-Sleep -Seconds 6
  $diff=& "$root/tools/diag/Invoke-D2WorldDiff.ps1" -OutDir $out -Tag "after-$profile"
  $fresh=@($samples|Where-Object {$_.hostFresh-and$_.clientFresh}).Count
  $row=@{profile=$profile;hostControl=(Ctl host);clientControl=(Ctl client);relayBefore=$before;relayAfter=$after;freshSamples=$fresh;samples=$samples.Count;diffStatus=$diff.status;compared=$diff.comparedKeys;differences=$diff.differences}
  $rows.Add($row);Save "$out/phase-$profile.json" $row}
 # The blackout: both directions cut for $BlackoutMs, then the time until both transports are fresh and the worlds match.
 Rpc 'disconnect' ('{"duration_ms":'+$BlackoutMs+'}')|Out-Null
 $w=[Diagnostics.Stopwatch]::StartNew();Start-Sleep -Milliseconds $BlackoutMs
 do{Start-Sleep -Milliseconds 250}while(!(Fresh)-and$w.ElapsedMilliseconds-lt120000)
 $freshMs=$w.ElapsedMilliseconds-$BlackoutMs
 $diff=& "$root/tools/diag/Invoke-D2WorldDiff.ps1" -OutDir $out -Tag 'after-blackout'
 $rows.Add(@{profile='blackout';blackoutMs=$BlackoutMs;freshAfterMs=$freshMs;diffStatus=$diff.status;compared=$diff.comparedKeys;differences=$diff.differences;hostConnection=(S host).connectionState;clientConnection=(S client).connectionState;hostControl=(Ctl host);clientControl=(Ctl client)})
 Save "$out/relay-final-stats.json" (Rpc 'stats')
 foreach($r in 'host','client'){C $r 'coop_flight dump'}
 Start-Sleep -Seconds 2
 foreach($r in 'host','client'){$d=Get-ChildItem "$root/work/lab-instances/$r" -Filter '*.cofr'|Sort-Object LastWriteTime|Select-Object -Last 1;if($d){Copy-Item $d.FullName "$out/$r.cofr"}}
 # Version mismatch: restart only the client, announcing control protocol 99. The host must refuse it with a reason.
 $l=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
 if($p){Stop-Process -Id $p.Id;if(!$p.WaitForExit(15000)){throw 'Client did not exit'}}
 & "$lab/Start-D2Lab.ps1" -Role client -AssetExperiment -PeerPort 31427 -TestControlProtocol 99|Out-Null;Pids
 $w=[Diagnostics.Stopwatch]::StartNew();$seen=$null
 do{Start-Sleep -Milliseconds 1000;$h=Ctl host;if($h-and"$($h.result)"-like'version_mismatch*'){$seen=$h}}while(!$seen-and$w.ElapsedMilliseconds-lt120000)
 $rows.Add(@{profile='version-mismatch';refusedAfterMs=$w.ElapsedMilliseconds;hostControl=$seen;clientControl=(Ctl client);hostConnection=(S host).connectionState})
 if(!$seen){throw 'The host did not refuse the mismatched client'}
 Save "$out/verified.json" @{measured=$true;phases=@($rows).Count;scope='Network-trouble profiles and a blackout on the lab leg; differences and freshness are findings.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  try{if($relay-and!$relay.HasExited){try{Rpc 'shutdown'|Out-Null}catch{};if(!$relay.WaitForExit(5000)){Stop-Process -Id $relay.Id}}}catch{$errors.Add("relay: $($_.Exception.Message)")}
  if($dirty){try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("redeploy: $($_.Exception.Message)")}}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){throw 'Final gameplay/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;phases=@($rows);scope='Network trouble on the lab leg through the coop-anything chaos relay; lab redeployed.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
