param([string]$Take='swim-exit-v1054',[double]$ExitLane=5.5,[double]$EntryAlong=0,[switch]$FaceDock,[int]$JumpFrom=6,[ValidateRange(0,70)][double]$CameraPitch=12,[ValidateSet('host','client')][string]$Source='client',[double]$ChainX=-41.3809,[double]$ChainY=73.1098,[double]$ChainZ=17.9602,[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Native water exit discovery (Edge of the World via the native `campaign`
# command; host Corvo, client Emily). v1052: the dock wall is too high (1051),
# so the source swims down the canal lane toward the stone steps in 1 s
# forward pulses (jump added after six pulses) until it stands on land;
# v1053 parameterizes entry (-EntryAlong, -FaceDock) and jump onset to reach
# ledges such as the moored boat (native swim mantle); v1054 polls the source
# player states through the mantle (traversal reader stage, mantle geometry)
# and saves both roles' action-stream status afterwards, while the read-only probe watches its
# native player states; the observer films the dock edge from a held view
# over the water and samples its remote presentation. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$samplers=[Collections.Generic.List[object]]::new()
$Observer=if($Source-eq'host'){'client'}else{'host'};$sourceCharacter=if($Source-eq'host'){'corvo'}else{'emily'}
function Save($p,$v){$v|ConvertTo-Json -Depth 14|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100,[int]$ly=0){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};if($ly){$a.LeftY=$ly};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Frame($r,$name){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();& (Get-Command ffmpeg).Source -hide_banner -loglevel error -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30,hwdownload,format=bgra" -frames:v 1 -y "$out/$name.png"|Out-Null}
function Capture($name,$seconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$name-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$name-$r-capture.log" -PassThru))}}
function Sampler($name,$ms){$pwsh=(Get-Process -Id $PID).Path;$p=Start-Process $pwsh -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Sample-StatusFile-v1023.ps1",'-Path',"$root/work/lab-instances/$Observer/coop-status.json.movement.json",'-Out',"$out/$name-observer-movement.ndjson",'-DurationMs',$ms) -WindowStyle Hidden -PassThru;$samplers.Add($p);$p}
function BridgeSampler($name,$ms){$pwsh=(Get-Process -Id $PID).Path;$p=Start-Process $pwsh -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Sample-StatusFile-v1023.ps1",'-Path',"$root/work/lab-instances/$Observer/coop-status.json",'-Out',"$out/$name-observer-bridge.ndjson",'-DurationMs',$ms,'-IntervalMs',100) -WindowStyle Hidden -PassThru;$samplers.Add($p);$p}
function Rows($name){@(Get-Content "$out/$name-observer-movement.ndjson"|ForEach-Object {($_|ConvertFrom-Json).status})}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid}
 $bankEntry=Get-Content "$PSScriptRoot/climb-bank-manifest-v1042.json" -Raw|ConvertFrom-Json|Where-Object role -eq $sourceCharacter
 if((Get-FileHash "$root/work/lab-instances/$Observer/coop-status.json.movement.bin").Hash-ne$bankEntry.sha256){throw "Observer $Observer lacks the $sourceCharacter bank70"}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;bank=$bankEntry.sha256;mapInfo=$MapInfo;source=$Source;observer=$Observer}
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
 if(!$ready){Frame host 'stuck-host';Frame client 'stuck-client';throw 'Both instances did not reach mission gameplay'}
 Start-Sleep -Seconds 2
 if(!(S host).remoteFresh-or!(S client).remoteFresh){throw 'Transport not fresh on mission map'}
 C host 'coop_spawn_probe shadow_emily';Start-Sleep -Milliseconds 1500;C client 'coop_spawn_probe shadow_corvo';Start-Sleep -Milliseconds 1500
 foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'}
 $sp=Q $Source '--player';$o=$sp.nativeViewOrientation;$n=[Math]::Sqrt($o[0]*$o[0]+$o[1]*$o[1]);$fx=$o[0]/$n;$fy=$o[1]/$n
 # Observer stands on the dock 2.5 m in front of the source spawn.
 C $Observer ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f($sp.candidatePosition[0]+2.5*$fx),($sp.candidatePosition[1]+2.5*$fy),($sp.candidatePosition[2]+1.7),([Math]::Atan2(-$fy,-$fx)*180/[Math]::PI));Start-Sleep -Seconds 2
 foreach($r in 'host','client'){$st=S $r;if(!$st.probeActorHandle-or!$st.probeFollowEnabled-or!$st.remoteFresh){throw "Partner presentation not active on $r"}}
 C $Observer 'coop_movement_bank on';$bw=[Diagnostics.Stopwatch]::StartNew();do{$m=S $Observer '.movement.json';if($m.active-and$m.version-eq70){break};Start-Sleep -Milliseconds 40}while($bw.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne70){throw 'Observer bank70 preload failed'}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 1500
 foreach($r in 'host','client'){Save "$out/shadow-stream-start-$r.json" (S $r '.shadow-stream.json')}
 if((S $Observer '.shadow-stream.json').result-ne'ready'-or(S $Source '.shadow-stream.json').result-ne'ready'){throw 'Action stream not ready on mission map'}
 $px=-$fy;$py=$fx;$s0=$sp.candidatePosition
 $wx=$s0[0]+$ExitLane*$px+$EntryAlong*$fx;$wy=$s0[1]+$ExitLane*$py+$EntryAlong*$fy;$yaw=if($FaceDock){[Math]::Atan2(-$py,-$px)*180/[Math]::PI}else{[Math]::Atan2($fy,$fx)*180/[Math]::PI}
 C $Source ("setviewpos {0:F3} {1:F3} 1.0 {2:F2} 0"-f$wx,$wy,$yaw);Start-Sleep -Milliseconds 1800
 $swimState=Q $Source '--player-states';Save "$out/source-in-water.json" $swimState
 if($swimState.defaultMode-or!@($swimState.active|Where-Object {$_.id-eq0}).Count){throw 'Source did not enter native swimming'}
 # Observer on the dock looking down the canal lane.
 $tx=if($FaceDock){$wx-2*$px}else{$wx+10*$fx};$ty=if($FaceDock){$wy-2*$py}else{$wy+10*$fy}
 $oyaw=[Math]::Atan2($ty-($s0[1]+2.5*$fy),$tx-($s0[0]+2.5*$fx))*180/[Math]::PI
 C $Observer ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f($s0[0]+2.5*$fx),($s0[1]+2.5*$fy),($s0[2]+1.7),$oyaw);Start-Sleep -Milliseconds 900
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Observer -Pitch $CameraPitch -Evidence "$out/observer-camera.json" -Quick|Out-Null
 Save "$out/observer-view.json" (Q $Observer '--player')
 $watch=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$Source],'--watch-player-states','15000') -RedirectStandardOutput "$out/source-states.json" -WindowStyle Hidden -PassThru
 $null=BridgeSampler 'exit' 15000;$smp=Sampler 'exit' 15000;Capture 'exit' 15
 Start-Sleep -Milliseconds 800
 $attempts=[Collections.Generic.List[object]]::new()
 for($k=0;$k-lt11;$k++){
  if($k-ge$JumpFrom){I $Source A 700 32000;$poll=[Collections.Generic.List[object]]::new();$pw=[Diagnostics.Stopwatch]::StartNew()
   while($pw.ElapsedMilliseconds-lt1800){try{$q=Q $Source '--player-states';$poll.Add(@{ms=$pw.ElapsedMilliseconds;states=$q})}catch{};Start-Sleep -Milliseconds 30}
   Save "$out/mantle-poll-$k.json" @($poll)}else{I $Source '' 1000 32000;Start-Sleep -Milliseconds 850}
  $st=Q $Source '--player-states';$pl=Q $Source '--player';$attempts.Add(@{pulse=$k;jump=($k-ge$JumpFrom);defaultMode=$st.defaultMode;active=$st.active;stance=$st.stance;position=$pl.candidatePosition})
  if($st.defaultMode-and$pl.candidatePosition[2]-gt.3){break}
  if(!$st.defaultMode-and!@($st.active|Where-Object {$_.vtableRva-eq'0x1c7dc68'}).Count){Save "$out/non-swim-state-$k.json" $st}
 }
 Save "$out/exit-attempts.json" @($attempts)
 foreach($r in 'host','client'){Save "$out/stream-after-exit-$r.json" (S $r '.shadow-stream.json')}
 I $Source '' 800 32000;Start-Sleep -Milliseconds 1000;I $Source
 if(!$watch.WaitForExit(20000)){Stop-Process -Id $watch.Id}
 if(!$smp.WaitForExit(20000)){Stop-Process -Id $smp.Id}
 foreach($c in @($captures)){if(!$c.WaitForExit(30000)-or$c.ExitCode){throw 'Exit recording failed'}};$captures.Clear()
 $final=Q $Source '--player';$last=$attempts[$attempts.Count-1]
 $exited=$last.defaultMode-and$final.candidatePosition[2]-gt.3
 $rows=Rows 'exit'
 Save "$out/exit-results.json" @{exited=$exited;finalPosition=$final.candidatePosition;attempts=$attempts.Count;observer=@{samples=$rows.Count;swimming=@($rows|Where-Object {$_.swimWeight-gt.5}).Count;traversal=@($rows|Where-Object {$_.traversalWeight-gt.5}).Count;traversalVariants=@($rows|Where-Object {$_.traversalWeight-gt.5}|ForEach-Object {$_.traversalVariant}|Sort-Object -Unique)}}
 Save "$out/verified.json" @{passed=$exited;scope='Native water exit discovery: source swims to the dock edge and exits with jump+forward; native states watched; observer presentation sampled (no new binding).'}
 if(!$exited){throw 'Source did not exit the water onto the dock'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)+@($samplers)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r}catch{};foreach($cmd in 'coop_observer_camera off','coop_movement_bank off','coop_shadow_stream off'){try{C $r $cmd}catch{}}}
   # A native modal (tutorial/readable) blocks fixture restore; ordinary back input dismisses it.
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;source=$Source;scope='Native water exit discovery in Edge of the World; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
