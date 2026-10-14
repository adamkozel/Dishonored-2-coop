param([string]$Take='mission-coop-eotw-v1036',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
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
 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','10','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -PassThru))
 }
 Start-Sleep -Milliseconds 800
 # Host walks away and back so the client watches remote Corvo locomotion.
 & "$lab/Send-D2Input.ps1" -Role host -LeftY -32000 -DurationMs 2500|Out-Null;Start-Sleep -Milliseconds 2800
 & "$lab/Send-D2Input.ps1" -Role host -LeftY 32000 -DurationMs 2500|Out-Null;Start-Sleep -Milliseconds 2800
 & "$lab/Send-D2Input.ps1" -Role host -DurationMs 100|Out-Null
 foreach($c in $captures){if(!$c.WaitForExit(30000)-or$c.ExitCode){throw 'Recording failed'}}
 $final=@{host=(S host);client=(S client);hostPlayer=(Q host '--player');clientPlayer=(Q client '--player')};Save "$out/bridge-final.json" $final
 $d=[Math]::Sqrt([Math]::Pow($final.client.probeRenderPosition[0]-$final.hostPlayer.candidatePosition[0],2)+[Math]::Pow($final.client.probeRenderPosition[1]-$final.hostPlayer.candidatePosition[1],2))
 Save "$out/verified.json" @{passed=($d-lt1.0);clientAvatarToHostDistance=$d;scope='Co-op bring-up on the Edge of the World mission map: native campaign load for both roles, fresh transport, partner Doppelganger spawn and exact follow, remote Corvo locomotion recorded.'}
 if($d-ge1.0){throw "Client remote Corvo not following host (distance $d)"}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in $captures){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   try{& "$lab/Send-D2Input.ps1" -Role host -DurationMs 100|Out-Null}catch{}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;scope='Mission-map co-op bring-up (Edge of the World); Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
