param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='grip-closeup-corvo-v1',[switch]$Candidate)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$peer=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null
$bank=if($Candidate){"$PSScriptRoot/crouch-balance-banks-v1/$character-crouch-balance-bank.bin"}else{"$PSScriptRoot/windblast-context-banks-v1/$character-windblast-context-bank.bin"}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$y=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native read $r $mode"}
function Pose($label){for($a=0;$a-lt20;$a++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$peer] --skeleton-final $handles[$peer] (S $peer).bodyPublicationAddress;if(!$LASTEXITCODE){$pose=$raw|ConvertFrom-Json;if($pose.stable-and$pose.finalPublicationComplete){$raw|Set-Content "$out/pose-$label.json";return}}};throw "No stable final skeleton $label"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_power_fixture off','coop_windblast_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Living fixture required'}
  if((Q $r '--player-states').stance){I $r 0 100 B;Start-Sleep -Milliseconds 700}
  if(!(Q $r '--player-sword').equipped){I $r 0 650 X;Start-Sleep -Milliseconds 2300}
 }
 C $Source 'setviewpos -33.5 -153 68.1 90 0';C $peer 'setviewpos -31.8 -152.78 68.1 180 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 25 -Evidence "$out/camera.json"
 Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $peer 'coop_movement_bank on'
 $watch=[Diagnostics.Stopwatch]::StartNew();do{$m=S $peer '.movement.json';if($m.active-and$m.version-eq28-and$m.weight-gt.99){break};Start-Sleep -Milliseconds 40}while($watch.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne28-or$m.weight-lt.99){throw 'Movement bank unavailable'};$checks.Add('Exact comparison bank ready through native Steam stream')
 Pose 'standing'
 $window=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();if(!$window){throw 'No game window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','5','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 Start-Sleep -Milliseconds 1500;Pose 'standing-rendered';(Q $peer '--avatar-sword-attachment' $handles[$peer])|ConvertTo-Json -Depth 12|Set-Content -Encoding utf8 "$out/attachment.json"
 $checks.Add('Stable final native grip and rendered sword attachment captured beside a close game recording')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
 if(!(Q $Source '--player-sword').equipped-or!(Q $peer '--avatar-sword' $handles[$peer]).equipped){throw 'Sword selection changed'};$checks.Add('Native health and drawn swords preserved')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original files and saves unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((S $r).localPlayerStance-eq1){I $r 0 100 B;Start-Sleep -Milliseconds 700};C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(15000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Capture incomplete'}}
 $rows|ConvertTo-Json -Depth 15|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;character=$character;candidate=[bool]$Candidate;checks=$checks;bankSha256=(Get-FileHash $bank).Hash;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;visualAccepted=$false;scope='Close camera view of standing drawn sword grip, with stable final native joint cache and independent rendered attachment. Partial-body diagnostic view, not full-movement acceptance.'}|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"PASS_GRIP_CLOSEUP $character $($checks.Count) checks"
