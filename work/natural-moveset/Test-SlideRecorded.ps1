param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='slide-corvo-v28-v1',[ValidateSet('crouch','stand','jump')][string]$Exit='crouch',[switch]$Holstered,[ValidateSet(1,2)][int]$Layout=2)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$peer=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$recorders=@();$start=0
$bank="$PSScriptRoot/balanced-crouch-power-banks-v3/$character-balanced-crouch-power-bank.bin"
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native read $r $mode"}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 if($Layout-eq1){C $Source 'setviewpos -35 -153 68.1 0 0';C $peer 'setviewpos -29 -146 68.1 238 0'}
 else{C $Source 'setviewpos -35 -153 68.1 0 0';C $peer 'setviewpos -35.5 -150 68.1 305 0'}
 if((Q $Source '--player-states').stance-eq1){I $Source B 100;Start-Sleep -Milliseconds 700}
 if((Q $Source '--player-sword').equipped-eq[bool]$Holstered){$duration=if($Holstered){1500}else{650};I $Source X $duration;Start-Sleep -Milliseconds 2300}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json"
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 12 -Evidence "$out/observer-camera.json"
 Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $peer 'coop_movement_bank on'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$m=S $peer '.movement.json';if($m.active-and$m.version-eq28-and$m.weight-gt.9-and$m.presentedDrawn-ne[bool]$Holstered){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne28-or$m.weight-lt.9-or$m.presentedDrawn-eq[bool]$Holstered){throw 'Slide context bank unavailable'};$checks.Add('Exact bank and native Steam stream ready in requested sword state')
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
 foreach($r in @($peer,$Source)){
  $window=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$window){throw 'No game window'};$suffix=if($r-eq$peer){''}else{'-source'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','9','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take$suffix.mp4")
  $recorders+=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture$suffix.log" -RedirectStandardOutput "$out/capture$suffix.stdout" -PassThru
 }
 Start-Sleep -Milliseconds 700;if(@($recorders|Where-Object HasExited).Count){throw 'Recorder failed'}
 $watch=[Diagnostics.Stopwatch]::StartNew();$stage=0;$seenNative=$false;$seenRemote=$false;$seenRecovery=$false;$seenAir=$false;$exitSent=$false
 while($watch.ElapsedMilliseconds-lt7000){
  $ms=$watch.ElapsedMilliseconds
  if(!$stage-and$ms-ge500){I $Source LeftThumb 1000 32767;$stage=1;$sprintTime=$watch.ElapsedMilliseconds;$events.Add(@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native sprint input'})}
  if($stage-eq1-and$ms-$sprintTime-ge350){I $Source B 500 32767;$stage=2;$events.Add(@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native crouch while sprinting'})}
  $native=Q $Source '--player-states';$m=S $peer '.movement.json';$stream=S $peer '.shadow-stream.json'
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;native=$native;movement=$m;stream=$stream;source=(S $Source);target=(S $peer)})
  $seenNative=$seenNative-or($native.active.id-contains7-and$native.stance-eq5);$seenRemote=$seenRemote-or($stream.remoteState-eq6-and$m.slideState-eq1-and$m.slideWeight-gt.8-and$m.weight-gt.8);$seenRecovery=$seenRecovery-or$m.slideState-eq2
  if($Exit-eq'jump'-and!$exitSent-and$seenRemote){I $Source A 180;$exitSent=$true;$events.Add(@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native jump interrupts slide; neutral stick avoids requesting a furniture vault'})}
  if($Exit-eq'stand'-and!$exitSent-and$seenRecovery-and$ms-ge3000-and$native.stance-eq1){I $Source B 100;$exitSent=$true;$events.Add(@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native stand after slide recovery'})}
  $seenAir=$seenAir-or($exitSent-and$Exit-eq'jump'-and$m.airWeight-gt.6-and$m.airState-in@(1,2))
  if(!$m.active-or!$stream.armed-or!$stream.ready){throw 'Slide lost presentation ownership'};Start-Sleep -Milliseconds 15
 }
 if(!$seenNative-or!$seenRemote){throw 'Native slide did not drive the remote whole body'};$checks.Add('Native sprint-to-slide state drives remote whole-body slide through Steam')
 if($Exit-eq'jump'){if(!$seenAir){throw 'Jump interruption not observed'};$checks.Add('Native jump interrupts slide and retains airborne body presentation')}else{if(!$seenRecovery){throw 'Slide recovery absent'};$checks.Add('Native slide completion selects authored recovery')}
 $expected=if($Exit-eq'crouch'){1}else{0};if($native.stance-ne$expected-or$m.slideState-or$m.slideWeight-gt.01-or$m.airWeight-gt.02-or$m.weight-lt.9-or[Math]::Abs($m.stanceMix-$expected)-gt.03){throw "Wrong final stance or recovery: native=$($native.stance), slide=$($m.slideState), air=$($m.airWeight), stance=$($m.stanceMix)"};$checks.Add('Requested end stance and fully owned native-driven gait recovered')
 if((Q $Source '--player-sword').equipped-eq[bool]$Holstered-or(Q $peer '--avatar-sword' $handles[$peer]).equipped-eq[bool]$Holstered){throw 'Sword state changed'};$checks.Add('Source first-person and native remote sword retain requested equipment state')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}};$checks.Add('Both players and avatars retain native health')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected files changed'}};$checks.Add('Protected original files and saves unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((Q $r '--player-states').stance-eq1){I $r B 100;Start-Sleep -Milliseconds 700};C $r 'setviewpos -33 -153 68.1 90 0';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2300};C $r 'coop_movement_fixture off'}catch{}}
 foreach($capture in $recorders){$capture.WaitForExit(14000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Capture incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;character=$character;exit=$Exit;holstered=[bool]$Holstered;layout=$Layout;checks=$checks;events=$events;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bankSha256=(Get-FileHash $bank).Hash;recorded=($recorders.Count-eq2);scope='Native sprint-to-slide in isolated Dunwall with one explicit equipment and recovery/interruption context. Source POV and remote full-body recording. No combat/victim contact, slopes, low ceilings or arbitrary interruption acceptance.'}|ConvertTo-Json -Depth 7|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"PASS_SLIDE_RECORDING $Source $Exit $($checks.Count) checks"
