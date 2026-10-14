param([string]$Take='swim-coop-eotw-v1038',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Co-op bring-up outside the Dunwall fixture: both isolated instances load the
# same campaign mission through the native developer `campaign` command (host
# Corvo, client Emily), pass native load screens/intro, then the existing
# transport, partner Doppelganger spawn and exact follow are verified on the
# new map with recordings. Restores the Dunwall fixture afterwards.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 14|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function Frame($r,$name){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();& (Get-Command ffmpeg).Source -hide_banner -loglevel error -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30,hwdownload,format=bgra" -frames:v 1 -y "$out/$name.png"|Out-Null}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;mapInfo=$MapInfo;chaos=$Chaos}
 $dirty=$true
 C host "campaign $MapInfo $Chaos corvo";C client "campaign $MapInfo $Chaos emily"
 $w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{
  Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;remoteFresh=$s.remoteFresh;conn=$s.connectionState})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){& "$lab/Send-D2Input.ps1" -Role $r -Button A -DurationMs 300|Out-Null;$last[$r]=$w.ElapsedMilliseconds}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt240000)
 Save "$out/load-log.json" @($log)
 if(!$ready){Frame host 'stuck-host';Frame client 'stuck-client';throw 'Both instances did not reach mission gameplay'}
 Start-Sleep -Seconds 2
 $before=@{host=(S host);client=(S client)};Save "$out/bridge-before-spawn.json" $before
 foreach($r in 'host','client'){$c=Q $r '--character';Save "$out/character-$r.json" $c}
 if(!$before.host.remoteFresh-or!$before.client.remoteFresh){throw 'Transport not fresh on mission map'}
 C host 'coop_spawn_probe shadow_emily';Start-Sleep -Milliseconds 1500;C client 'coop_spawn_probe shadow_corvo';Start-Sleep -Milliseconds 1500
 foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'}
 # Client stands 2.5 m beside the host on the same dock, facing it.
 $hp=Q host '--player';$o=$hp.nativeViewOrientation;$n=[Math]::Sqrt($o[0]*$o[0]+$o[1]*$o[1]);$fx=$o[0]/$n;$fy=$o[1]/$n
 $cx=$hp.candidatePosition[0]+2.5*$fx;$cy=$hp.candidatePosition[1]+2.5*$fy;$cyaw=[Math]::Atan2(-$fy,-$fx)*180/[Math]::PI
 C client ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$cx,$cy,($hp.candidatePosition[2]+1.7),$cyaw);Start-Sleep -Seconds 3
 $after=@{host=(S host);client=(S client)};Save "$out/bridge-after-spawn.json" $after
 foreach($r in 'host','client'){if(!$after[$r].probeActorHandle-or!$after[$r].probeFollowEnabled-or!$after[$r].remoteFresh){throw "Partner presentation not active on $r"}}
 $bankEntry=Get-Content "$PSScriptRoot/swim-bank-manifest-v1037.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo'
 if((Get-FileHash "$root/work/lab-instances/client/coop-status.json.movement.bin").Hash-ne$bankEntry.sha256){throw 'Client Corvo bank69 not installed'}
 C client 'coop_movement_bank on';$bw=[Diagnostics.Stopwatch]::StartNew();do{$m=S client '.movement.json';if($m.active-and$m.version-eq69){break};Start-Sleep -Milliseconds 40}while($bw.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne69){throw 'Client bank69 preload failed'}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
 Start-Sleep -Milliseconds 1500
 foreach($r in 'host','client'){Save "$out/shadow-stream-start-$r.json" (S $r '.shadow-stream.json');Save "$out/bridge-stream-start-$r.json" (S $r)}
 # Host enters the canal on the +X side of the spawn dock (native1035).
 $o=$hp.nativeViewOrientation;$n=[Math]::Sqrt($o[0]*$o[0]+$o[1]*$o[1]);$fx=$o[0]/$n;$fy=$o[1]/$n
 $wx=$hp.candidatePosition[0]+4*(-$fy);$wy=$hp.candidatePosition[1]+4*$fx;$yaw=[Math]::Atan2($fy,$fx)*180/[Math]::PI
 $cx=$hp.candidatePosition[0]-0.5*(-$fy)-1.0*$fx;$cy=$hp.candidatePosition[1]-0.5*$fx-1.0*$fy
 # Aim at the middle of the host swim path (forward along the dock).
 $tx=$wx+3*$fx;$ty=$wy+3*$fy
 C client ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} -18"-f$cx,$cy,($hp.candidatePosition[2]+1.7),([Math]::Atan2($ty-$cy,$tx-$cx)*180/[Math]::PI))
 # Client watches from the dock edge facing the water.
  $pwsh=(Get-Process -Id $PID).Path
 $sampler=Start-Process $pwsh -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Sample-StatusFile-v1023.ps1",'-Path',"$root/work/lab-instances/client/coop-status.json.movement.json",'-Out',"$out/client-movement.ndjson",'-DurationMs',16000) -WindowStyle Hidden -PassThru
 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -PassThru))
 }
 Start-Sleep -Milliseconds 800
 C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$wx,$wy,1.0,$yaw);Start-Sleep -Milliseconds 1800
 $swimState=Q host '--player-states';Save "$out/host-in-water.json" $swimState
 & "$lab/Send-D2Input.ps1" -Role host -LeftY 32000 -DurationMs 3000|Out-Null;Start-Sleep -Milliseconds 3200
 & "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 1500|Out-Null;Start-Sleep -Milliseconds 1700
 & "$lab/Send-D2Input.ps1" -Role host -Button A -DurationMs 1500|Out-Null;Start-Sleep -Milliseconds 1700
 & "$lab/Send-D2Input.ps1" -Role host -LeftY -32000 -DurationMs 2500|Out-Null;Start-Sleep -Milliseconds 2700
 & "$lab/Send-D2Input.ps1" -Role host -DurationMs 100|Out-Null
 if(!$sampler.WaitForExit(20000)){Stop-Process -Id $sampler.Id}
 foreach($c in $captures){if(!$c.WaitForExit(30000)-or$c.ExitCode){throw 'Recording failed'}}
 $final=@{host=(S host);client=(S client);hostPlayer=(Q host '--player');clientPlayer=(Q client '--player')};Save "$out/bridge-final.json" $final
 $rows=@(Get-Content "$out/client-movement.ndjson"|ForEach-Object {($_|ConvertFrom-Json).status})
 $sw=@{samples=$rows.Count;swimming=@($rows|Where-Object {$_.swimWeight-gt.5}).Count;moving=@($rows|Where-Object {$_.swimWeight-gt.5-and$_.swimMove-gt.5}).Count;sinking=@($rows|Where-Object {$_.swimWeight-gt.5-and$_.swimSink-gt.3}).Count;rising=@($rows|Where-Object {$_.swimWeight-gt.5-and$_.swimRise-gt.3}).Count;maxSwimSamples=($rows|Measure-Object swimSamples -Maximum).Maximum}
 Save "$out/swim-results.json" @{hostSwimState=@{defaultMode=$swimState.defaultMode;active=$swimState.active;stance=$swimState.stance};client=$sw}
 if($swimState.defaultMode-or!$sw.swimming-or!$sw.moving){throw 'Remote Corvo body did not present native swimming on the client'}
 Save "$out/verified.json" @{passed=$true;swim=$sw;scope='Candidate1037: host native swimming (state22, protocol71) presented on the client remote Corvo body with bank69 tread/stroke/dive/surface blends in the Edge of the World mission; root/depth network-owned.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in $captures){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   try{& "$lab/Send-D2Input.ps1" -Role host -DurationMs 100|Out-Null}catch{}
   foreach($r in 'host','client'){foreach($cmd in 'coop_movement_bank off','coop_shadow_stream off'){try{C $r $cmd}catch{}}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;scope='Remote native swimming presentation in Edge of the World; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
