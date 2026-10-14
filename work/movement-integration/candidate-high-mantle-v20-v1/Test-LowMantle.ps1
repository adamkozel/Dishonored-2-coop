param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='low-mantle-v1-test',[ValidateSet('host','client')][string[]]$Sources=@('host','client'),[ValidateSet(1,2,3,19,20)][int]$BankVersion=1,[switch]$Crouched,[switch]$AuthoredFeet,[switch]$High,[switch]$Record)
if($High-and$BankVersion-ne20){throw 'High mantle requires bank20'};if($Record-and$Sources.Count-ne1){throw 'One source per recording'}
$expectedVariant=if($High){4}else{1};if($Crouched){$expectedVariant++}
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$handles=@{};$checks=[Collections.Generic.List[string]]::new();$rows=[Collections.Generic.List[object]]::new();$failure=$null;$liveDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$capture=$null;$overall=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function Status($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe failed $r $mode"}
function AwaitBank($r){$w=[Diagnostics.Stopwatch]::StartNew();do{$m=Status $r '.movement.json';if($m.active-and$m.version-eq$(if($BankVersion-ge19){$BankVersion}elseif($BankVersion-eq3){13}else{12})-and$m.weight-ge.9){return};Start-Sleep -Milliseconds 35}while($w.ElapsedMilliseconds-lt5000);throw 'Bank readiness deadline'}
function SetApproach($r){C $r $(if($High){'setviewpos -30.7 -153.6 68.1 270 0'}else{'setviewpos -32 -148.5 68.1 90 0'})}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(Status $r).probeActorHandle;I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'
  if(!(Q $r '--player-sword').equipped-or(Q $r '--health' 1).current-ne56){throw 'Clean armed fixture required'}
  $role=if($r-eq'host'){'emily'}else{'corvo'};$bankPath=if($BankVersion-eq20){"high-mantle-banks-v1/$role-high-mantle-bank.bin"}elseif($BankVersion-eq19){"crouch-turn-banks-v1/$role-crouch-turn-bank.bin"}elseif($BankVersion-eq3){"crouch-mantle-banks-v1/$role-low-mantle-bank.bin"}else{"low-mantle-banks-v$BankVersion/$role-low-mantle-bank.bin"};Copy-Item "$PSScriptRoot/$bankPath" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
  if($AuthoredFeet){C $r 'coop_movement_feet authored'}
 }
 foreach($src in $Sources){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
  C $dst $(if($High){'setviewpos -35.5 -153 68.1 340 0'}else{'setviewpos -34.5 -150 68.1 50.2 0'});SetApproach $src
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $src -Pitch 0 -Evidence "$out/$src-camera.json"
  if($Record){& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $dst -Pitch $(if($High){-8}else{0}) -Evidence "$out/observer-camera.json"}
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C $dst 'coop_movement_bank on';AwaitBank $dst
  if($Record){
   $window=(Get-Process -Id $pids[$dst]).MainWindowHandle.ToInt64();if(!$window){throw 'Missing observer window'}
   $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','45','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
   $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'};$overall=[Diagnostics.Stopwatch]::StartNew()
  }
  foreach($armed in $true,$false){
   if(!$armed){I $src X 1500;Start-Sleep -Milliseconds 3000}
   foreach($trial in 1,2){
    I $src;if((Q $src '--player-states').stance-eq1){I $src B 180;Start-Sleep -Milliseconds 650}
    SetApproach $src;Start-Sleep -Milliseconds 900
    if($Crouched){I $src B 180;Start-Sleep -Milliseconds 850};I $src A 1300 22000
    $w=[Diagnostics.Stopwatch]::StartNew();$nativeSamples=0;$ownedSamples=0;$presented=$false;$maxPhase=0;$minPhase=1;$events=[Collections.Generic.HashSet[long]]::new()
    while($w.ElapsedMilliseconds-lt3200){
     $native=Q $src '--player-states';$m=Status $dst '.movement.json';$s=Status $dst '.shadow-stream.json';$midnight=$null
     if((@($native.active|Where-Object id -eq 8).Count-and!$native.traversalSceneValid)-or(@($native.active|Where-Object id -eq 53).Count-and!$native.equipmentSceneValid)){$midnight=Q $src '--midnight'}
     $rows.Add([pscustomobject]@{source=$src;armed=$armed;trial=$trial;ms=$w.ElapsedMilliseconds;videoTime=$(if($overall){.7+$overall.Elapsed.TotalSeconds}else{$null});native=$native;movement=$m;stream=$s;midnight=$midnight;sourcePosition=(Status $src).localPosition;target=(Status $dst)})
     if(!$m.active-or!$s.armed-or!$s.ready){throw "Mantle presentation retired: $src $armed $trial $($s.result)"}
     if(@($native.active|Where-Object id -eq 8).Count){$nativeSamples++;if($native.traversalSceneValid){$ownedSamples++}}
     if($m.traversalWeight-gt.7-and$m.weight-gt.8-and$m.traversalVariant-eq$expectedVariant){$presented=$true;$maxPhase=[Math]::Max($maxPhase,$m.traversalAmount);$minPhase=[Math]::Min($minPhase,$m.traversalAmount);$null=$events.Add($m.traversalEvent)}
     Start-Sleep -Milliseconds 15
    }
    if(!$nativeSamples-or!$ownedSamples-or!$presented-or$maxPhase-lt.5-or$events.Count-ne1){throw "Low mantle missing native=$nativeSamples owned=$ownedSamples presented=$presented maxPhase=$maxPhase events=$($events.Count)"}
    $checks.Add("${src}: armed=$armed trial=$trial exact player-bound mantle drives one delayed full-body climb event")
    if($m.traversalWeight-gt.01-or$m.airWeight-gt.01-or$m.weight-lt.9){throw 'Mantle did not return to grounded gait'}
    if($Crouched-and($m.stanceMix-lt.9-or(Q $src '--player-states').stance-ne1)){throw 'Crouched mantle did not recover into native low stance'}
    $sword=Q $dst '--avatar-sword' $handles[$dst];if([bool]$sword.equipped-ne$armed){throw 'Mantle changed remote equipment'}
    if(@((Q $dst '--player-states').active|Where-Object {$_.id-eq8-or$_.id-eq2-or$_.id-eq3}).Count){throw 'Observer native player entered traversal'}
    $checks.Add("${src}: armed=$armed trial=$trial native recovery, equipment and observer first-person state preserved")
   }
  }
  I $src X 650;Start-Sleep -Milliseconds 3000;C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off';if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original files and protected saves unchanged')
 if($capture){$capture.WaitForExit(50000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Capture incomplete'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2500};if((Status $r).localPlayerStance-eq1){I $r B 180};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 13|Set-Content "$out/states.json"
 if($capture-and!$capture.HasExited){$capture.WaitForExit(50000)|Out-Null}
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=$liveDll;bankVersion=$BankVersion;high=[bool]$High;crouched=[bool]$Crouched;authoredFeet=[bool]$AuthoredFeet;recorded=[bool]$Record;sources=$Sources;scope='Observed native mantle scene for the requested sources and exit stance, drawn and holstered equipment, two repetitions; actual scene clock, ownership, recovery and health. Contact, other ledges/variants, interruption and visual acceptance remain separate.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"LOW_MANTLE_VALIDATED $($checks.Count) checks"
