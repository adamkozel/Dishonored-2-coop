param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateSet('health','mana')][string]$Kind='health',[ValidateSet('None','B','X','Y','NativeHealth','NativeMana')][string]$WheelButton='None',[switch]$Network,[ValidateRange(0,15000)][int]$Move=0,[switch]$ValidateStatistics,[switch]$DrainMana)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$beforeInv=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$monitor=$null;$dirty=$false
$peer=if($Source-eq'host'){'client'}else{'host'};$peerCapture=$null;$tokens=@{};$moved=$false
$kindId=if($Kind-eq'health'){1}else{2};$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$root/work/build/Release/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string[]]$buttons=@('Neutral'),$ms=100,$ly=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $buttons -DurationMs $ms -LeftY $ly|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
function OptionalState($r){$raw=& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] '--player-states';$code=$LASTEXITCODE;if($code){return [pscustomobject]@{readValid=$false;exitCode=$code;scope='Diagnostic read refused; no gameplay state inferred'}};return($raw|ConvertFrom-Json)}
try{
 if($DrainMana-and($Kind-ne'mana'-or$WheelButton-notin@('X','NativeMana')-or!$ValidateStatistics)){throw 'Mana preparation requires native mana use and statistic validation'}
 if($WheelButton-eq'NativeHealth'-and$Kind-ne'health'){throw 'Health callback requires health potion'}
 if($WheelButton-eq'NativeMana'-and($Kind-ne'mana'-or!$DrainMana)){throw 'Mana callback requires mana and verified native expenditure'}
 if($Network-and($WheelButton-notin@('NativeHealth','NativeMana')-or!$Move)){throw 'Network continuity test requires native callback and walking input'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_grenade_fixture off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $beforeInv[$r]=Q $r '--player-inventory';$beforeInv[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 C $Source 'setviewpos -34.3 -154.8 68.1 0 0'
 if((Q $Source '--player-states').stance-ne0){I $Source B 180;Start-Sleep -Milliseconds 650}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json" -Quick
 if($Network){
  C $peer 'setviewpos -30.5 -151.9 68.1 217 0'
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 0 -Evidence "$out/observer-camera.json" -Quick
  foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'}
  $character=if($Source-eq'host'){'corvo'}else{'emily'}
  $entry=Get-Content "$PSScriptRoot/crouched-grenade-bank-manifest-v62.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
  $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank identity differs'}
  Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force;C $peer 'coop_movement_bank on';$bw=[Diagnostics.Stopwatch]::StartNew()
  do{$m=S $peer '.movement.json';if($m.active-and$m.version-eq44){break};Start-Sleep -Milliseconds 45}while($bw.ElapsedMilliseconds-lt5000)
  if(!$m.active-or$m.version-ne44){throw 'Candidate bank preload failed'}
 }
 if($DrainMana){
  if($Source-eq'host'){
  $power=Q $Source '--player-bendtime';if($power.quantity-or$power.equipped-or$power.started-or$power.powerActive){throw 'BendTime preparation requires unowned power'}
  $manaFull=& "$PSScriptRoot/Read-NativeMana.ps1" -Role $Source -Evidence "$out/mana-before-power.json"
  $dirty=$true;C $Source 'coop_bendtime_fixture 20000';$pw=[Diagnostics.Stopwatch]::StartNew()
  do{$pf=S $Source '.bendtime-fixture.json';if($pf.result-eq'native_bendtime_ready'){break};Start-Sleep -Milliseconds 60}while($pw.ElapsedMilliseconds-lt6000)
  if($pf.result-ne'native_bendtime_ready'){throw 'Native BendTime preparation did not equip'}
  & "$lab/Send-D2Input.ps1" -Role $Source -Button Neutral -LeftTrigger 255 -DurationMs 250|Out-Null
  Start-Sleep -Milliseconds 2000;$power=Q $Source '--player-bendtime';$power|ConvertTo-Json -Depth 7|Set-Content "$out/power-active.json"
  $manaSpent=& "$PSScriptRoot/Read-NativeMana.ps1" -Role $Source -Evidence "$out/mana-after-power.json"
  if(!$power.powerActive-or$manaSpent.currentCandidate-ge$manaFull.currentCandidate){throw 'Controller activation did not prove native mana expenditure'}
  & "$lab/Send-D2Input.ps1" -Role $Source -Button Neutral -LeftTrigger 255 -DurationMs 250|Out-Null
  Start-Sleep -Milliseconds 1000;I $Source;C $Source 'coop_bendtime_fixture off';$pw.Restart()
  do{$pf=S $Source '.bendtime-fixture.json';if(!$pf.active){break};Start-Sleep -Milliseconds 70}while($pw.ElapsedMilliseconds-lt7000)
  $power=Q $Source '--player-bendtime';$power|ConvertTo-Json -Depth 7|Set-Content "$out/power-restored.json"
  if($pf.active-or$pf.result-ne'quantity_and_selection_restored'-or$power.quantity-or$power.equipped-or$power.started-or$power.powerActive){throw 'Native power did not completely retire before potion grant'}
  $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role $Source -Evidence "$out/mana-before-potion.json"
  if($manaBefore.currentCandidate-ge$manaFull.currentCandidate){throw 'Mana deficit expired before potion experiment'}
  $checks.Add('Native controller BendTime spends mana; toggle and native inventory cleanup retire power completely before potion grant, without resource writes')
  }else{
   $power=Q $Source '--player-darkvision';if($power.quantity-or$power.equipped-or$power.started-or$power.managerStarted){throw 'Dark Vision preparation requires unowned inactive power'}
   $manaFull=& "$PSScriptRoot/Read-NativeMana.ps1" -Role $Source -Evidence "$out/mana-before-power.json"
   $dirty=$true;C $Source 'coop_darkvision_fixture 20000';$pw=[Diagnostics.Stopwatch]::StartNew()
   do{$pf=S $Source '.darkvision-fixture.json';if($pf.result-eq'native_darkvision_ready'){break};Start-Sleep -Milliseconds 60}while($pw.ElapsedMilliseconds-lt6000)
   if($pf.result-ne'native_darkvision_ready'){throw 'Native Dark Vision preparation did not equip'}
   $visionStarted=$false
   for($cast=0;$cast-lt3;$cast++){
    & "$lab/Send-D2Input.ps1" -Role $Source -Button Neutral -LeftTrigger 255 -DurationMs 250|Out-Null
    Start-Sleep -Milliseconds 3000
    $power=Q $Source '--player-darkvision';$power|ConvertTo-Json -Depth 7|Set-Content "$out/power-cast-$cast.json"
    if($power.managerStarted){$visionStarted=$true};if(!$power.equipped-or!$power.started){throw 'Native Dark Vision equipment retired during preparation'}
   }
   $manaSpent=& "$PSScriptRoot/Read-NativeMana.ps1" -Role $Source -Evidence "$out/mana-after-power.json"
   if(!$visionStarted-or$manaSpent.currentCandidate-ge$manaFull.currentCandidate){throw 'Controller casts did not prove native Dark Vision mana expenditure'}
   I $Source;C $Source 'coop_darkvision_fixture off';$pw.Restart()
   do{$pf=S $Source '.darkvision-fixture.json';if(!$pf.active){break};Start-Sleep -Milliseconds 80}while($pw.ElapsedMilliseconds-lt32000)
   $power=Q $Source '--player-darkvision';$power|ConvertTo-Json -Depth 7|Set-Content "$out/power-restored.json"
   if($pf.active-or$pf.result-ne'quantity_selection_and_effect_inactive'-or$power.quantity-or$power.equipped-or$power.started-or$power.managerStarted){throw 'Native Dark Vision did not retire before potion grant'}
   $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role $Source -Evidence "$out/mana-before-potion.json"
   if($manaBefore.currentCandidate-ge$manaFull.currentCandidate){throw 'Mana deficit expired before potion experiment'}
   foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000'}
   $checks.Add('Native Emily Dark Vision casts spend mana; native effect expires and inventory retires before potion grant, without resource writes')
  }
 }
 $before=Q $Source '--player-potion' $kindId;$before|ConvertTo-Json -Depth 6|Set-Content "$out/potion-before.json"
 if(!$before.valid-or$before.quantity){throw 'Requested native scalar potion must be unowned'}
 if($ValidateStatistics-and!$before.statisticsValid){throw 'Native consumption statistics unavailable before grant'}
 $dirty=$true;C $Source "coop_potion_fixture $Kind";$wait=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 50;$f=S $Source '.potion-fixture.json'}while($f.result-ne'native_potion_ready'-and$wait.ElapsedMilliseconds-lt2500)
 if($f.result-ne'native_potion_ready'){throw 'Native finite potion grant did not become ready'}
 $f|ConvertTo-Json|Set-Content "$out/fixture-ready.json";$held=Q $Source '--player-potion' $kindId
 if(!$held.valid-or$held.quantity-ne1){throw 'Native potion quantity differs'};$checks.Add('Exactly one scalar native potion granted without equipping or invoking consumption')
 if($ValidateStatistics){if(!$held.statisticsValid-or(Compare-Object $before.statisticsA $held.statisticsA)-or(Compare-Object $before.statisticsB $held.statisticsB)){throw 'Grant changed consumption statistics'};$held|ConvertTo-Json -Depth 7|Set-Content "$out/potion-granted.json"}
 if($Network){
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};$tw=[Diagnostics.Stopwatch]::StartNew()
  do{foreach($r in 'host','client'){$tokens[$r]=S $r '.shadow-stream.json'};if($tokens.host.ready-and$tokens.client.ready){break};Start-Sleep -Milliseconds 40}while($tw.ElapsedMilliseconds-lt2500)
  if(!$tokens.host.ready-or!$tokens.client.ready){throw 'Native Steam stream handshake failed'}
  $tokens|ConvertTo-Json -Depth 8|Set-Content "$out/stream-before.json"
 }
 $hwnd=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','9','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 if($Network){$ph=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();if(!$ph){throw 'No receiver window'};$pa=@($args|ForEach-Object {if($_-like'gfxcapture=*'){"gfxcapture=hwnd=${ph}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"}elseif($_-eq"$PSScriptRoot/$Take.mp4"){"$PSScriptRoot/$Take-receiver.mp4"}else{$_}});$peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $pa -WindowStyle Hidden -RedirectStandardError "$out/capture-receiver.log" -RedirectStandardOutput "$out/capture-receiver.stdout" -PassThru}
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();Start-Sleep -Milliseconds 500;if($capture.HasExited){throw 'Capture failed'}
 $monitor=Start-Process "$root/work/build/Release/d2_runtime_probe.exe" -ArgumentList @($pids[$Source],'--watch-potion-states',$kindId,'8000') -WindowStyle Hidden -RedirectStandardOutput "$out/potion-states.ndjson" -RedirectStandardError "$out/potion-states.stderr" -PassThru
 $watch=[Diagnostics.Stopwatch]::StartNew();$opened=$false;$pressed=$false
 while($watch.ElapsedMilliseconds-lt7500){
  if($Move-and!$moved-and$watch.ElapsedMilliseconds-ge900){I $Source Neutral 2500 $Move;$moved=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native forward walking through consumption';leftY=$Move})}
  if($WheelButton-notin@('NativeHealth','NativeMana')-and!$opened-and$watch.ElapsedMilliseconds-ge350){I $Source LeftShoulder 3500;$opened=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LeftShoulder item wheel'})}
  if($WheelButton-ne'None'-and!$pressed-and$watch.ElapsedMilliseconds-ge1700){if($WheelButton-eq'NativeHealth'){C $Source 'coop_potion_fixture use_health'}elseif($WheelButton-eq'NativeMana'){C $Source 'coop_potion_fixture use_mana'}else{I $Source @('LeftShoulder',$WheelButton) 250};$pressed=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input=if($WheelButton-in@('NativeHealth','NativeMana')){"Exact native $Kind-consume routine from engine thread; no keyboard input"}else{"Native item wheel plus $WheelButton"}})}
  $scene=Q $Source '--midnight';$rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;status=(S $Source);player=(OptionalState $Source);scenes=@($scene.instances|Where-Object {$_.state-eq3});fixture=(S $Source '.potion-fixture.json');movement=$(if($Network){S $peer '.movement.json'});stream=$(if($Network){S $peer '.shadow-stream.json'});sourceStream=$(if($Network){S $Source '.shadow-stream.json'});receiver=$(if($Network){S $peer})})
  Start-Sleep -Milliseconds 50
 }
 I $Source;Start-Sleep -Milliseconds 300;$after=Q $Source '--player-potion' $kindId;$after|ConvertTo-Json -Depth 6|Set-Content "$out/potion-after.json"
 if(!$after.valid){throw 'Native final potion read unavailable'}
 if($ValidateStatistics){
  if(!$after.statisticsValid){throw 'Native consumption statistics unavailable after action'}
  for($k=0;$k-lt2;$k++){$delta=if($WheelButton-ne'None'-and$k-eq($kindId-1)){1}else{0};foreach($key in 'statisticsA','statisticsB'){if($after.$key[$k]-$before.$key[$k]-ne$delta){throw "Native statistic $key kind$k does not match consumption versus rollback"}}}
  $checks.Add($(if($WheelButton-eq'None'){'Native wheel-only grant/rollback leaves both consumption statistic arrays unchanged'}else{'Both native consumption statistic arrays independently distinguish exactly one use from grant/rollback; other potion kind unchanged'}))
 }
 if($WheelButton-eq'None'){if($after.quantity-ne0-or$after.health-ne$before.health-or(S $Source '.potion-fixture.json').result-ne'native_quantity_restored'){throw 'Wheel-only native fixture rollback was not confirmed'};$checks.Add('Wheel-only pause retires the fixture and restores quantity on resume without healing')}
 else{if(!$pressed-or$after.quantity-ne0){throw 'Native consumption did not spend the granted potion'};$checks.Add($(if($WheelButton-in@('NativeHealth','NativeMana')){'Exact native callback consumed one potion in normal gameplay'}else{'Native wheel input consumed one potion'}));if($Kind-eq'health'){if(!$after.healthValid-or$after.health-le$before.health){throw 'Native health did not increase'};$checks.Add('Native health increases after consumption without a health setter')}}
 if($DrainMana){
  $manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role $Source -Evidence "$out/mana-after-potion.json"
  if($manaAfter.currentCandidate-le$manaBefore.currentCandidate-or$after.health-ne$before.health){throw 'Mana gain or unchanged native health not confirmed'}
  $checks.Add('Native mana increases with exactly one mana-consumption statistic increment and unchanged health')
 }
 if($Network){
  foreach($row in $rows){foreach($r in 'host','client'){$ss=if($r-eq$Source){$row.sourceStream}else{$row.stream};if(!$ss.ready-or!$ss.armed-or$ss.token-ne$tokens[$r].token-or$ss.peerToken-ne$tokens[$r].peerToken){throw 'Native consumption retired the Steam stream'}};if(!$row.movement.active-or$row.movement.version-ne44){throw 'Remote body bank stopped'}}
  if(!$moved-or!@($rows|Where-Object {$_.ms-ge1500-and$_.ms-le3400-and$_.movement.speed-gt.1}).Count-or!@($rows|Where-Object {$_.ms-ge5500-and$_.movement.speed-lt.05}).Count){throw 'Remote walking or native stopping absent'}
  if($rows[-1].movement.applied-$rows[0].movement.applied-lt100){throw 'Remote body updates did not progress'}
  if(@($rows|Where-Object {$_.movement.powerWeight-gt.01-or$_.movement.grenadeWeight-gt.01-or$_.movement.traversalWeight-gt.01-or$_.movement.combatWeight-gt.01}).Count){throw 'Unexpected action overlay during native potion use'}
  $checks.Add('Remote walking and stopping persist through native consumption with same Steam tokens, continuous bank updates and no invented body action')
  [ordered]@{source=(Q $Source '--health' 1);receiverPlayer=(Q $peer '--health' 1);receiverAvatar=(Q $peer '--health' (S $peer).probeActorHandle)}|ConvertTo-Json -Depth 8|Set-Content "$out/health-scope.json"
  if((Q $peer '--health' 1).current-ne56-or(Q $peer '--health' (S $peer).probeActorHandle).current-ne40){throw 'Unexpected remote health mutation'}
  $checks.Add('Remote player and cosmetic avatar health remain unchanged by source potion consumption')
 }
 C $Source 'coop_potion_fixture off';Start-Sleep -Milliseconds 700
 foreach($r in 'host','client'){
  $inv=Q $r '--player-inventory';$inv|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($beforeInv[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($inv.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory rollback differs'}
 }
 $checks.Add('Both inventories restore exactly after finite native grant')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_potion_fixture off';C $r 'coop_bendtime_fixture off';C $r 'coop_darkvision_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(12000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($peerCapture){$peerCapture.WaitForExit(12000)|Out-Null;if(!$peerCapture.HasExited-or$peerCapture.ExitCode){$failure=($failure+'; receiver capture failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(10000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; monitor failed').TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 15|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'};$p=Q $r '--player-potion' $kindId;if(!$p.valid-or$p.quantity){throw 'Potion remains after reload'}};$checks.Add('Both native isolated checkpoints restore health and unowned potion state')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;kind=$Kind;wheelButton=$WheelButton;networkContinuity=[bool]$Network;manaPreparation=[bool]$DrainMana;statisticsValidated=[bool]$ValidateStatistics;leftY=$Move;checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;finalDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope=$(if($Network){'Existing remote body walking/stopping continuity through source native potion consumption, over Steam. No dedicated drink gesture, remote drink audio or remote resource synchronization is implemented; all outcomes reset through isolated checkpoints.'}else{'Source-only native potion and item-wheel observation. No remote animation or shared healing is claimed; all native effects are reset through isolated checkpoints.'})}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_POTION_OBSERVATION $Source $($checks.Count) checks"
