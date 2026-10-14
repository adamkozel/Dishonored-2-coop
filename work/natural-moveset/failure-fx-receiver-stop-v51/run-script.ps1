param([ValidateSet('receiver-stop','source-pause','receiver-pause')][string]$Mode='receiver-stop',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='failure-fx-lifecycle-v51',[switch]$Resume)
if($Resume-and$Mode-eq'receiver-stop'){throw 'Resume is scoped to native pause'}
$captures=@();$trace=$null;$pausedRole=$null;$Crouched=$false;$Holstered=$false;$FrontView=$true;$TriggerMs=250;$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
Copy-Item -LiteralPath $PSCommandPath -Destination "$out/run-script.ps1"
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$start=0
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$trigger=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $trigger|ForEach-Object {$script:lastInputSequence=$_.Sequence}}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
function S($r,$suffix=''){return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_power_fixture off','coop_darkvision_fixture off','coop_bendtime_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 C host 'setviewpos -33.5 -153 68.1 90 0';C client 'setviewpos -30.7 -151.8 68.1 203 0'
 if($FrontView){C client 'setviewpos -33.5 -150 68.1 270 0'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 15 -Evidence "$out/observer-camera.json"
 if((Q host '--player-states').stance-ne[int][bool]$Crouched){I host 0 100 B;Start-Sleep -Milliseconds 700}
 if((Q host '--player-sword').equipped-eq[bool]$Holstered){$duration=if($Holstered){1500}else{650};I host 0 $duration X;Start-Sleep -Milliseconds 2300}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 0 -Evidence "$out/camera.json"
 $before=Q host '--player-bendtime';$inventory=Q host '--player-inventory';$peerBefore=Q client '--player-inventory'
 $before|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/before.json";$inventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Native BendTime already owned or active'}
 C host 'coop_bendtime_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew();do{$s=S host '.bendtime-fixture.json';if($s.result-eq'native_bendtime_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($s.result-ne'native_bendtime_ready'){throw "BendTime did not equip: $($s.result)"}
 $power=Q host '--player-bendtime';$power|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'BendTime acquisition unconfirmed'}
 $checks.Add('Exact native Corvo BendTime equipment acquired and selected')
 $entry=Get-Content "$PSScriptRoot/outofmana-context-bank-manifest-v46.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo';$bank=Join-Path $PSScriptRoot $entry.bank
 if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank identity mismatch'}
 Copy-Item $bank "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';if($m.active-and$m.version-eq41-and$m.weight-gt.9-and$m.presentedDrawn-ne[bool]$Holstered-and[Math]::Abs($m.stanceMix-[int][bool]$Crouched)-lt.02){break};Start-Sleep -Milliseconds 35}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne41-or$m.weight-lt.9-or$m.presentedDrawn-eq[bool]$Holstered-or[Math]::Abs($m.stanceMix-[int][bool]$Crouched)-ge.02){throw 'Native v41 stance/equipment body not ready'}
 $checks.Add('Exact v41 stance/equipment bank active on the native Steam remote avatar')
 $avatar=(S client).probeActorHandle;$fxBefore=S client '.power-fx.json'
 if($Resume){
  $emily=Get-Content "$PSScriptRoot/outofmana-context-bank-manifest-v46.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'emily';$eb=Join-Path $PSScriptRoot $emily.bank
  if((Get-FileHash $eb).Hash-ne$emily.sha256){throw 'Emily bank mismatch'};Copy-Item $eb "$root/work/lab-instances/host/coop-status.json.movement.bin";C host 'coop_movement_bank on'
  $w.Restart();do{$m=S host '.movement.json';if($m.active-and$m.version-eq41-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
  if(!$m.active-or$m.version-ne41-or$m.weight-lt.9){throw 'Both banks required for recovery'}
  $priorTokens=@{};foreach($r in 'host','client'){C $r 'coop_movement_resume on';$priorTokens[$r]=(S $r '.shadow-stream.json').token}
 }
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
 foreach($r in 'host','client'){
 $window=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$window){throw 'No game window'};$suffix=if($r-eq'host'){'-source'}else{''}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','20','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take$suffix.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture$suffix.log" -RedirectStandardOutput "$out/capture$suffix.stdout" -PassThru
 $captures+=$capture
 }
 Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}

 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$times=@(1000,3000,6000);$failed=$false
 while($watch.ElapsedMilliseconds-lt8500){
  if($next-lt$times.Count-and$watch.ElapsedMilliseconds-ge$times[$next]){
   if($next-eq2){$mana=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-before-failure.json"}
   I host 255 $TriggerMs;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native LT press';index=$next});$next++
  }
  $native=Q host '--player-states';$fx=S client '.power-fx.json';$m=S client '.movement.json'
  $rows.Add([pscustomobject]@{stage='casting';videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;native=$native;fx=$fx;movement=$m;stream=(S client '.shadow-stream.json')})
  if($native.powerSceneValid-and$native.powerScene.variant-eq15){$failed=$true}
  if($next-eq3-and$failed-and$fx.started-eq($fxBefore.started+1)-and$fx.active-eq1){break}
  Start-Sleep -Milliseconds 15
 }
 if(!$failed-or$fx.started-ne($fxBefore.started+1)-or$fx.active-ne1-or@($fx.effects|Where-Object {$_.kind-eq11-and$_.owner-eq$avatar-and$_.record-ne'0x0'}).Count-ne1){throw 'Owned native failed scene and live automatic hand effect required'}
 if((Q host '--player-bendtime').powerActive){throw 'Failure activated native power'}
 $checks.Add('One exact native failure scene produces one owned live hand effect before interruption')
 $interruptionAt=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;$beforeInterrupt=$fx
 if($Mode-eq'receiver-stop'){C client 'coop_shadow_stream off'}else{
  $pausedRole=if($Mode-eq'source-pause'){'host'}else{'client'};I $pausedRole 0 180 Start
  $w.Restart();do{if((S $pausedRole).localGameLogicState-eq2){break};Start-Sleep -Milliseconds 25}while($w.ElapsedMilliseconds-lt2500)
  if((S $pausedRole).localGameLogicState-ne2){throw 'Native pause not reached'}
  $rows.Add([pscustomobject]@{stage='paused';videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;fx=(S client '.power-fx.json');movement=(S client '.movement.json');stream=(S client '.shadow-stream.json')})
  if($Resume){
   $w.Restart();do{$suspended=$true;foreach($r in 'host','client'){if((S $r '.movement.json').active-or(S $r '.shadow-stream.json').armed-or(S $r '.movement-resume.json').phase-ne2){$suspended=$false}};if($suspended){break};Start-Sleep -Milliseconds 25}while($w.ElapsedMilliseconds-lt1200)
   if(!$suspended){throw 'Pause did not suspend both owners'}
  }
  Start-Sleep -Milliseconds 500;I $pausedRole 0 180 A
  $w.Restart();do{if((S $pausedRole).localGameLogicState-eq1){break};Start-Sleep -Milliseconds 25}while($w.ElapsedMilliseconds-lt4000)
  if((S $pausedRole).localGameLogicState-ne1){throw 'Native menu resume failed'};$pausedRole=$null
 }
 $w.Restart();do{$fx=S client '.power-fx.json';if(!$fx.active){break};Start-Sleep -Milliseconds 25}while($w.ElapsedMilliseconds-lt1800)
 if($fx.active-or$fx.started-ne$beforeInterrupt.started-or$fx.stopped-ne($beforeInterrupt.stopped+1)){throw 'Owned effect did not stop exactly once after interruption'}
 $checks.Add('Interruption stops the owned effect exactly once; observer pause defers native cleanup until resume')
 if($Resume){
  $w.Restart();do{$ready=$true;foreach($r in 'host','client'){$m=S $r '.movement.json';$s=S $r '.shadow-stream.json';if(!$m.active-or$m.version-ne41-or$m.weight-lt.9-or!$s.ready-or$s.token-eq$priorTokens[$r]-or(S $r '.movement-resume.json').phase-ne1){$ready=$false}};if($ready){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt14000)
  if(!$ready){throw 'Pinned bodies did not resume through fresh tokens'};$checks.Add('Both pinned banks resume through fresh Steam tokens after native menu resume')
 }else{
  if((S client '.shadow-stream.json').armed-or(S client '.movement.json').weight-gt.001){throw 'Retired stream retained body ownership'};$checks.Add('Stopped stream retires body ownership')
 }
 Start-Sleep -Milliseconds 1800;$fx=S client '.power-fx.json'
 if($fx.active-or$fx.started-ne$beforeInterrupt.started){throw 'Failure effect replayed after retirement'}
 $rows.Add([pscustomobject]@{stage='settled';videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;fx=$fx;movement=(S client '.movement.json');stream=(S client '.shadow-stream.json')})
 $checks.Add('No delayed failure effect restarts after retirement or fresh-token recovery')
 I host;C host 'coop_bendtime_fixture off';$w.Restart();do{$f=S host '.bendtime-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt7000)
 $after=Q host '--player-bendtime';if($f.active-or$f.result-ne'quantity_and_selection_restored'-or$after.quantity-or$after.equipped-or$after.started-or$after.powerActive){throw 'Power cleanup incomplete'}
 $checks.Add('Inactive native power fixture restores quantity and selection')
 $afterInventory=Q host '--player-inventory';$peerAfter=Q client '--player-inventory'
 if(Compare-Object @($inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($afterInventory.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Source inventory changed'}
 if(Compare-Object @($peerBefore.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($peerAfter.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Peer inventory changed'}
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed'}}
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $checks.Add('Both inventories, player/avatar health and fourteen protected hashes retained')
}catch{$failure=$_.Exception.Message}finally{
 if($pausedRole){try{I $pausedRole 0 180 A;Start-Sleep -Milliseconds 700}catch{}}
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_resume off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_bendtime_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 foreach($rec in $captures){$rec.WaitForExit(25000)|Out-Null;if(!$rec.HasExited-or$rec.ExitCode){$failure='Capture incomplete'}}
 $rows|ConvertTo-Json -Depth 15|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;mode=$Mode;automaticResume=[bool]$Resume;interruptionAt=$interruptionAt;checks=$checks;events=$events;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bankSha256=(Get-FileHash $bank).Hash;scope='Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open.'}|ConvertTo-Json -Depth 7|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_FAILURE_FX_LIFECYCLE $Mode $($checks.Count) checks"
