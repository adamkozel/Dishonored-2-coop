param([ValidateSet('receiver-stop','source-pause','receiver-pause')][string]$Mode='receiver-stop',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='bendtime-lifecycle-v44',[switch]$Resume)
if($Resume-and$Mode-eq'receiver-stop'){throw 'Automatic resume is scoped to native menu pause'}
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory $out|Out-Null
$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$pausedRole=$null;$captures=@();$start=0
$manifest=Get-Content "$PSScriptRoot/bendtime-balanced-bank-manifest-v42.json" -Raw|ConvertFrom-Json;$bank=Join-Path $PSScriptRoot ($manifest|Where-Object role -eq 'corvo').bank
if((Get-FileHash $bank).Hash-ne($manifest|Where-Object role -eq 'corvo').sha256){throw 'Bank identity mismatch'}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$trigger=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -LeftTrigger $trigger -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode"}
function Snap($stage){$x=[pscustomobject]@{stage=$stage;seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;host=(S host);client=(S client);movement=(S client '.movement.json');stream=(S client '.shadow-stream.json');sourceStream=(S host '.shadow-stream.json');fx=(S client '.power-fx.json')};$rows.Add($x);return $x}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_bendtime_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Clean living fixture required'}
 }
 C host 'setviewpos -33.5 -153 68.1 90 0';C client 'setviewpos -30.7 -151.8 68.1 203 0'
 if((Q host '--player-states').stance-ne0){I host B;Start-Sleep -Milliseconds 700};if(!(Q host '--player-sword').equipped){I host X 0 650;Start-Sleep -Milliseconds 2300}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 15 -Evidence "$out/observer-camera.json"
 $before=Q host '--player-bendtime';if($before.quantity-or$before.equipped-or$before.powerActive){throw 'Power already owned or active'}
 Copy-Item $bank "$root/work/lab-instances/client/coop-status.json.movement.bin";C host 'coop_bendtime_fixture 20000'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$f=S host '.bendtime-fixture.json';if($f.result-eq'native_bendtime_ready'){break};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt5000);if($f.result-ne'native_bendtime_ready'){throw 'Native power not ready'}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';if($m.active-and$m.version-eq39-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 35}while($w.ElapsedMilliseconds-lt5000);if(!$m.active-or$m.version-ne39-or$m.weight-lt.9){throw 'Movement not ready'}
 if($Resume){
  $emily=$manifest|Where-Object role -eq 'emily';$emilyBank=Join-Path $PSScriptRoot $emily.bank;if((Get-FileHash $emilyBank).Hash-ne$emily.sha256){throw 'Emily bank identity mismatch'};Copy-Item $emilyBank "$root/work/lab-instances/host/coop-status.json.movement.bin";C host 'coop_movement_bank on'
  $w.Restart();do{$m=S host '.movement.json';if($m.active-and$m.version-eq39-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 35}while($w.ElapsedMilliseconds-lt5000);if(!$m.active-or$m.version-ne39-or$m.weight-lt.9){throw 'Emily presentation not ready for resume test'}
  $resumeBefore=@{};foreach($r in 'host','client'){C $r 'coop_movement_resume on';$resumeBefore[$r]=S $r '.movement-resume.json';if(!$resumeBefore[$r].enabled){throw 'Native resume not armed'}}
 }
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
 foreach($r in 'client','host'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No game window'};$suffix=if($r-eq'host'){'-source'}else{''}
  $a=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','18','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take$suffix.mp4")
  $captures+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $a -WindowStyle Hidden -RedirectStandardOutput "$out/capture$suffix.stdout" -RedirectStandardError "$out/capture$suffix.log" -PassThru
 }
 Start-Sleep -Milliseconds 700;$pre=Snap ready;I host Neutral 255 350
 $w.Restart();do{$s=Snap casting;if($s.movement.powerVariant-eq12-and$s.movement.powerWeight-gt.5){break};Start-Sleep -Milliseconds 10}while($w.ElapsedMilliseconds-lt2500)
 if($s.movement.powerVariant-ne12-or$s.movement.powerWeight-le.5){throw 'No active remote cast to interrupt'};$checks.Add('Actual source controller cast produces remote phase12 before interruption')
 if($Mode-eq'receiver-stop'){C client 'coop_shadow_stream off'}else{
  $pausedRole=if($Mode-eq'source-pause'){'host'}else{'client'};I $pausedRole Start 0 180
  $w.Restart();do{$s=Snap pausing;if((S $pausedRole).localGameLogicState-eq2){break};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt2500)
  if((S $pausedRole).localGameLogicState-ne2){throw 'Native menu pause not observed'};Start-Sleep -Milliseconds 600
  if($Resume){foreach($r in 'host','client'){if((S $r '.movement.json').active-or(S $r '.shadow-stream.json').armed-or(S $r '.movement-resume.json').phase-ne2){throw 'Menu did not suspend both movement owners'}};$checks.Add('Native pause suspends both banks and action streams before resume')}
  I $pausedRole A 0 180;$w.Restart();do{if((S $pausedRole).localGameLogicState-eq1){break};Start-Sleep -Milliseconds 35}while($w.ElapsedMilliseconds-lt4000);if((S $pausedRole).localGameLogicState-ne1){throw 'Native resume failed'};$pausedRole=$null
 }
 if($Resume){
  $w.Restart();do{$ready=$true;foreach($r in 'host','client'){$m=S $r '.movement.json';$stream=S $r '.shadow-stream.json';$resumeState=S $r '.movement-resume.json';if(!$m.active-or$m.version-ne39-or$m.weight-lt.9-or!$stream.ready-or$resumeState.phase-ne1){$ready=$false}};if($ready){break};Start-Sleep -Milliseconds 70}while($w.ElapsedMilliseconds-lt14000)
  if(!$ready){throw 'Native same-world animation resume did not finish'}
  foreach($r in 'host','client'){$a=S $r '.movement-resume.json';$t=S $r '.shadow-stream.json';if($a.resumed-le$resumeBefore[$r].resumed-or$a.checksum-ne$resumeBefore[$r].checksum-or$t.token-eq$a.priorToken-or$t.peerToken-eq$a.priorPeerToken-or(S $r).probeActorHandle-ne$handles[$r]){throw 'Resume reused old ownership or changed actor/bank'}}
  $checks.Add('Both existing avatars resume pinned banks through fresh native Steam ownership tokens');$s=Snap resumed
  Start-Sleep -Milliseconds 1200;$s=Snap settled;if(!$s.movement.active-or$s.movement.weight-lt.9-or$s.movement.powerWeight-gt.02){throw 'Resumed cast did not recover to native idle'};$checks.Add('Resumed native phase finishes once and returns to usable standing presentation')
 }else{
  $w.Restart();do{$s=Snap retired;if(!$s.stream.armed-and(!$s.movement.active-or$s.movement.weight-lt.001)){break};Start-Sleep -Milliseconds 35}while($w.ElapsedMilliseconds-lt2000)
  if($s.stream.armed-or($s.movement.active-and$s.movement.weight-ge.001)){throw 'Interruption retained body ownership'};$checks.Add('Interruption retires native Steam animation stream and body writes')
  $applied=$s.movement.applied;Start-Sleep -Milliseconds 550;$s=Snap settled
  if($s.stream.armed-or$s.movement.applied-ne$applied){throw 'Retired cast replayed or wrote another pose'};$checks.Add('No delayed cast replays or additional pose writes after retirement')
 }
 if($s.fx.started-ne$pre.fx.started-or$s.fx.active){throw 'Unrequested observer screen effect'};$checks.Add('Remote cast does not introduce private native screen effects on the observer')
 if((Q host '--player-bendtime').powerActive){I host Neutral 255 300;Start-Sleep -Milliseconds 1300}
 C host 'coop_bendtime_fixture off';$w.Restart();do{$f=S host '.bendtime-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt7000)
 $after=Q host '--player-bendtime';$after|ConvertTo-Json -Depth 6|Set-Content "$out/after.json";if($f.active-or$f.result-ne'quantity_and_selection_restored'-or$after.quantity-or$after.equipped-or$after.started-or$after.powerActive){throw 'Power cleanup incomplete'};$checks.Add('Normal controller end and native fixture cleanup restore quantity/selection')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}};$checks.Add('Both player and avatar health retained')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 original settings/save and isolated checkpoint hashes retained')
}catch{$failure=$_.Exception.Message}finally{
 if($pausedRole){try{I $pausedRole A 0 180;Start-Sleep -Milliseconds 700}catch{}}
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_resume off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'}catch{}}
 try{if((Q host '--player-bendtime').powerActive){I host Neutral 255 300;Start-Sleep -Milliseconds 1300};C host 'coop_bendtime_fixture off'}catch{}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 foreach($capture in $captures){$capture.WaitForExit(22000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure='Capture incomplete'}}
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;mode=$Mode;automaticResume=[bool]$Resume;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bankSha256=(Get-FileHash $bank).Hash;recorded=($captures.Count-eq2);scope='Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred.'}|ConvertTo-Json -Depth 7|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_BENDTIME_LIFECYCLE $Mode $($checks.Count) checks"
