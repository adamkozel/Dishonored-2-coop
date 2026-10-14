param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='keyhole-corvo-pause-v24-v5',[ValidateSet('host','client')][string]$Source='host',[switch]$CrouchedStart,[switch]$Record)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$dst=if($Source-eq'host'){'client'}else{'host'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$paused=$false;$capture=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe $r $mode"}
function Snap($label){foreach($r in 'host','client'){$rows.Add([pscustomobject]@{label=$label;role=$r;bridge=(S $r);movement=(S $r '.movement.json');resume=(S $r '.movement-resume.json');stream=(S $r '.shadow-stream.json');native=(Q $r '--player-states');equipment=(Q $r '--avatar-sword' $handles[$r])})}}
function AwaitLive {
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{$ok=$true;foreach($r in 'host','client'){$m=S $r '.movement.json';$s=S $r '.shadow-stream.json';if(!$m.active-or$m.version-ne24-or$m.weight-lt.9-or!$s.ready){$ok=$false}};if($ok){return};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt14000)
 Snap 'readiness-failed';throw 'Movement did not resume'
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle
  I $r;foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_feet authored'){C $r $c}
  C $r 'setviewpos -33.5 -153 68.1 0 0'
  if((Q $r '--health' 1).current-ne56){throw 'Baseline health changed'}
  if((Q $r '--player-states').stance-eq1){I $r B 100;Start-Sleep -Milliseconds 650}
  if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2300}
  $role=if($r-eq'host'){'emily'}else{'corvo'};Copy-Item "$PSScriptRoot/keyhole-banks-v2/$role-keyhole-bank.bin" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
 }
 C $dst 'setviewpos -32.5 -151 68.1 217 0';C $Source 'setviewpos -35.9 -153.55 68.1 180 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json"
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $dst -Pitch 10 -Evidence "$out/observer-camera.json"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;foreach($r in 'host','client'){C $r 'coop_movement_bank on'};AwaitLive
 foreach($r in 'host','client'){C $r 'coop_movement_resume on'}
 if($Record){
  $window=(Get-Process -Id $pids[$dst]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','24','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 }
 if($CrouchedStart){I $Source B 100;Start-Sleep -Milliseconds 700}
 I $Source X 1000;Start-Sleep -Milliseconds 3300;Snap 'before-pause'
 $before=@{};foreach($r in 'host','client'){$before[$r]=S $r '.movement-resume.json';if(!$before[$r].enabled){throw 'Resume was not armed'}}
 $native=Q $Source '--player-states';$m=S $dst '.movement.json'
 if(!$native.keyholeSceneValid-or$native.keyholeScene.variant-ne8-or$m.traversalVariant-ne8-or$m.traversalWeight-lt.9){throw 'Native and remote keyhole hold required'}
 $handle=$native.keyholeScene.handle;$clock=$native.keyholeScene.timeMs
 I $Source Start 180;$paused=$true;Start-Sleep -Milliseconds 1000;Snap 'paused'
 foreach($r in 'host','client'){if((S $r '.movement.json').active-or(S $r '.shadow-stream.json').armed-or(S $r '.movement-resume.json').phase-ne2){throw 'Pause retained movement ownership'}}
 $checks.Add('Native pause retires both movement banks and Steam action streams')
 I $Source A 180;$paused=$false;AwaitLive;Start-Sleep -Milliseconds 400;Snap 'resumed'
 foreach($r in 'host','client'){$resume=S $r '.movement-resume.json';$stream=S $r '.shadow-stream.json';if($resume.phase-ne1-or$resume.resumed-le$before[$r].resumed-or$resume.checksum-ne$before[$r].checksum-or$stream.token-eq$resume.priorToken-or$stream.peerToken-eq$resume.priorPeerToken-or(S $r).probeActorHandle-ne$handles[$r]){throw 'Resume changed actor/bank or reused old action tokens'}}
 $native=Q $Source '--player-states';$m=S $dst '.movement.json'
 if(!$native.keyholeSceneValid-or$native.keyholeScene.handle-ne$handle-or$native.keyholeScene.variant-ne8-or$native.keyholeScene.timeMs-lt$clock-or$m.traversalVariant-ne8-or$m.traversalWeight-lt.9-or$m.presentedDrawn){throw 'Live keyhole hold did not resume from native evidence'}
 $checks.Add('Same native keyhole hold resumes through fresh Steam tokens and pinned banks')
 I $Source B 100;Start-Sleep -Milliseconds 3300;Snap 'recovered'
 $m=S $dst '.movement.json';if(!(Q $Source '--player-sword').equipped-or!(Q $dst '--avatar-sword' $handles[$dst]).equipped-or$m.traversalWeight-gt.02-or(Q $Source '--player-states').stance-ne[int][bool]$CrouchedStart){throw 'Exit equipment/stance recovery failed'}
 $checks.Add('Native exit restores original stance and both swords after pause')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}};$checks.Add('Player and avatar health preserved')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Protected files unchanged')
}catch{$failure=$_.Exception.Message}finally{
 if($paused){try{I $Source A 180;Start-Sleep -Milliseconds 800}catch{}}
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_resume off';if(@((Q $r '--player-states').active|Where-Object id -eq 24).Count){I $r B 100;Start-Sleep -Milliseconds 1800};C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((Q $r '--player-states').stance-eq1){I $r B 100;Start-Sleep -Milliseconds 650};C $r 'setviewpos -33.5 -153 68.1 0 0';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2300};C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 14|Set-Content -Encoding utf8 "$out/states.json"
 if($capture){$capture.WaitForExit(30000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Capture incomplete'}}
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;source=$Source;crouchedStart=[bool]$CrouchedStart;recorded=[bool]$Record;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Same-world native menu pause/resume during a keyhole hold, fresh Steam tokens, native scene continuity and stance/equipment recovery. Death/load/disconnect are separate.'}|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"KEYHOLE_PAUSE_VALIDATED $($checks.Count) checks"
