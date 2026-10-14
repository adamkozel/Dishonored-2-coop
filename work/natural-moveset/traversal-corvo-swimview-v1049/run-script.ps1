param([string]$Take='traversal-coop-v1049',[ValidateSet('swim','climb','both')][string]$Modes='both',[ValidateRange(0,70)][double]$SwimPitch=50,[double]$SwimLane=5.5,[double]$CameraHeight=9.3,[ValidateSet('host','client')][string]$Source='client',[double]$ChainX=-41.3809,[double]$ChainY=73.1098,[double]$ChainZ=17.9602,[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Role-parametric remote traversal presentation in a real mission (Edge of the
# World via the native `campaign` command; host Corvo, client Emily). The
# source performs native swimming (state22) then native chain climbing
# (state23) with ordinary input; the observer renders the remote body with
# its installed bank70 and samples presentation. v1047: the swim observer
# stands at the spawn, yaws at the swim lane and pitches down with native look
# input so the remote swimmer is framed; v1048 also samples the observer
# bridge (remote/render position) and its native view to locate the body;
# v1049 moves the lane clear of the moored boat and films it from a held
# overhead view above the dock edge; observer camera films the climb. Dunwall fixture restored afterwards.
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
 # ---- Swim: canal on the +X side of the spawn dock (native1035), lane clear of the moored boat ----
 $px=-$fy;$py=$fx;$wx=$sp.candidatePosition[0]+$SwimLane*$px;$wy=$sp.candidatePosition[1]+$SwimLane*$py;$yaw=[Math]::Atan2($fy,$fx)*180/[Math]::PI
 if($Modes-ne'climb'){
 C $Source ("setviewpos {0:F3} {1:F3} 1.0 {2:F2} 0"-f$wx,$wy,$yaw);Start-Sleep -Milliseconds 1800
 $swimState=Q $Source '--player-states';Save "$out/source-in-water.json" $swimState
 # Observer faces the lane from the spawn and pitches down (native look input;
 # setviewpos preserves pitch), then is placed above the dock edge and the
 # view is held before the body falls; the body returns to the spawn.
 $vyaw=[Math]::Atan2($py,$px)*180/[Math]::PI
 C $Observer ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$sp.candidatePosition[0],$sp.candidatePosition[1],($sp.candidatePosition[2]+1.7),$vyaw);Start-Sleep -Milliseconds 900
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Observer -Pitch $SwimPitch -Evidence "$out/swim-observer-camera.json" -Quick|Out-Null
 C $Observer ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f($wx-4.5*$px+3.5*$fx),($wy-4.5*$py+3.5*$fy),$CameraHeight,$vyaw);Start-Sleep -Milliseconds 120
 C $Observer 'coop_observer_camera hold';Start-Sleep -Milliseconds 300
 C $Observer ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$sp.candidatePosition[0],$sp.candidatePosition[1],($sp.candidatePosition[2]+1.7),$vyaw);Start-Sleep -Milliseconds 900
 Save "$out/swim-observer-view.json" (Q $Observer '--player')
 $null=BridgeSampler 'swim' 15000;$smp=Sampler 'swim' 15000;Capture 'swim' 15
 Start-Sleep -Milliseconds 1500
 I $Source '' 2200 32000;Start-Sleep -Milliseconds 2400
 I $Source B 1500;Start-Sleep -Milliseconds 1700
 I $Source A 1500;Start-Sleep -Milliseconds 1700
 I $Source '' 2000 -32000;Start-Sleep -Milliseconds 2200
 I $Source
 if(!$smp.WaitForExit(20000)){Stop-Process -Id $smp.Id}
 foreach($c in @($captures)){if(!$c.WaitForExit(30000)-or$c.ExitCode){throw 'Swim recording failed'}};$captures.Clear()
 $rows=Rows 'swim'
 $sw=@{samples=$rows.Count;swimming=@($rows|Where-Object {$_.swimWeight-gt.5}).Count;moving=@($rows|Where-Object {$_.swimWeight-gt.5-and$_.swimMove-gt.5}).Count;sinking=@($rows|Where-Object {$_.swimWeight-gt.5-and$_.swimSink-gt.3}).Count;rising=@($rows|Where-Object {$_.swimWeight-gt.5-and$_.swimRise-gt.3}).Count}
 Save "$out/swim-results.json" @{source=@{defaultMode=$swimState.defaultMode;active=$swimState.active;stance=$swimState.stance};observer=$sw}
 C $Observer 'coop_observer_camera off'
}
 # ---- Climb: chain1 (native1041b); observer camera held facing it ----
if($Modes-ne'swim'){
 $lx=$ChainX-1.2;$ly=$ChainY;$lz=$ChainZ-2.22
 C $Observer ("setviewpos {0:F3} {1:F3} {2:F3} 0 5"-f($ChainX-4.5),($ChainY-1.5),($lz+1.9));Start-Sleep -Milliseconds 400
 C $Observer 'coop_observer_camera hold';Start-Sleep -Milliseconds 300
 C $Observer ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$sp.candidatePosition[0],$sp.candidatePosition[1],($sp.candidatePosition[2]+1.7))
 $smp=Sampler 'climb' 16000;Capture 'climb' 16
 Start-Sleep -Milliseconds 800
 C $Source ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$lx,$ly,($ChainZ+0.5));Start-Sleep -Milliseconds 1500
 $land=Q $Source '--player';C $Source ("setviewpos {0:F3} {1:F3} {2:F3} 0 20"-f$land.candidatePosition[0],$land.candidatePosition[1],($land.candidatePosition[2]+1.7));Start-Sleep -Milliseconds 500
 I $Source X 600;Start-Sleep -Milliseconds 1200
 $chainState=Q $Source '--player-states';Save "$out/source-on-chain.json" $chainState
 I $Source '' 3000 32000;Start-Sleep -Milliseconds 3200
 I $Source '' 3000 -32000;Start-Sleep -Milliseconds 3200
 I $Source A 300;Start-Sleep -Milliseconds 1500
 I $Source
 if(!$smp.WaitForExit(20000)){Stop-Process -Id $smp.Id}
 foreach($c in @($captures)){if(!$c.WaitForExit(30000)-or$c.ExitCode){throw 'Climb recording failed'}};$captures.Clear()
 $rows=Rows 'climb'
 $cl=@{samples=$rows.Count;climbing=@($rows|Where-Object {$_.climbWeight-gt.5}).Count;descending=@($rows|Where-Object {$_.climbWeight-gt.5-and$_.climbDescend-gt.5}).Count;ascending=@($rows|Where-Object {$_.climbWeight-gt.5-and$_.climbDescend-lt.5}).Count;phases=@($rows|Where-Object {$_.climbWeight-gt.5}|ForEach-Object {[Math]::Round($_.climbPhase,2)}|Sort-Object -Unique).Count}
 Save "$out/climb-results.json" @{source=@{defaultMode=$chainState.defaultMode;active=$chainState.active;stance=$chainState.stance};observer=$cl}
}
 $swimOk=($Modes-eq'climb')-or(!$swimState.defaultMode-and@($swimState.active|Where-Object {$_.id-eq0}).Count-and$sw.swimming-gt20-and$sw.moving-gt5)
 $climbOk=($Modes-eq'swim')-or(!$chainState.defaultMode-and@($chainState.active|Where-Object {$_.id-eq29}).Count-and$cl.climbing-gt20-and$cl.phases-ge5)
 Save "$out/verified.json" @{passed=($swimOk-and$climbOk);swim=$swimOk;climb=$climbOk;source=$Source;modes=$Modes;scope="Candidate1042 role parity ($Modes): $sourceCharacter ($Source) native swimming (state22) and chain climbing (state23) presented on the $Observer remote body with bank70 in Edge of the World."}
 if(!$swimOk){throw "Remote $sourceCharacter swimming not presented on $Observer"};if(!$climbOk){throw "Remote $sourceCharacter climbing not presented on $Observer"}
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
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;source=$Source;scope='Role-parametric remote swimming/climbing in Edge of the World; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
