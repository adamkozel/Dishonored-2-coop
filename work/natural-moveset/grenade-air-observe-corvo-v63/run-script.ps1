param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-grenade-actions-v54',[ValidateRange(-15,45)][int]$Pitch=0,[switch]$ValidateReader,[ValidateSet('standing','crouched')][string]$Stance='standing',[ValidateRange(0,15000)][int]$Move=0,[switch]$FacingWall)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$beforeInv=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$monitor=$null;$dirty=$false;$start=0
function C($r,$text){& "$lab/Send-D2Command.ps1" -Role $r -Command $text -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$button='Neutral',$ms=100,$lt=0,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $lt -LeftY $y|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$handle=0){for($a=0;$a-lt8;$a++){$raw=if($handle){& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode $handle}else{& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode failed"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_grenade_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $beforeInv[$r]=Q $r '--player-inventory';$beforeInv[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 if((Q $Source '--player-states').stance-ne0){I $Source B 180;Start-Sleep -Milliseconds 700}
 # Known Dunwall room positions. Normal native input performs the throw;
 # no projectile function or animation event is called by the harness.
 C $Source 'setviewpos -34.3 -154.8 68.1 0 0'
 if($FacingWall){C $Source 'setviewpos -34.3 -154.8 68.1 180 0'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch $Pitch -Evidence "$out/source-camera.json"
 $before=Q $Source '--player-grenade';if($before.quantity-or$before.equipped){throw 'Grenade already owned'};$before|ConvertTo-Json|Set-Content "$out/before.json"
 Q $Source '--entities'|ConvertTo-Json -Depth 10|Set-Content "$out/entities-before.json"
 C $Source 'coop_grenade_fixture 12000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 50;$f=S $Source '.grenade-fixture.json'}while($f.result-ne'native_grenade_ready'-and$w.ElapsedMilliseconds-lt2500)
 if($f.result-ne'native_grenade_ready'){throw 'Native grant did not become ready'}
 I $Source X 1500;Start-Sleep -Milliseconds 1900;I $Source X 650;Start-Sleep -Milliseconds 1350
 $held=Q $Source '--player-grenade';if(!$held.equipped-or$held.quantity-ne1){throw 'Native grenade presentation not selected'}
 if($Stance-eq'crouched'){I $Source B 180;Start-Sleep -Milliseconds 650}
 $nativeStance=Q $Source '--player-states'
 if($nativeStance.stance-ne$(if($Stance-eq'crouched'){1}else{0})){throw 'Requested native stance unavailable'}
 $nativeStance|ConvertTo-Json -Depth 10|Set-Content "$out/source-stance.json"
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch $Pitch -Evidence "$out/pitch-after-draw.json"
 $checks.Add('One native grenade selected and requested native stance confirmed')
 $hwnd=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();Start-Sleep -Milliseconds 500;if($capture.HasExited){throw 'Capture failed'}
 if($true){if((Q $Source '--player-grenade-scene').valid){throw 'Action reader incorrectly active before input'};$monitor=Start-Process "$root/work/build/Release/d2_runtime_probe.exe" -ArgumentList @($pids[$Source],'--watch-grenade-scenes','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/owned-scenes.ndjson" -RedirectStandardError "$out/owned-scenes.stderr" -PassThru}
 $watch=[Diagnostics.Stopwatch]::StartNew();$fired=$false;$jumped=$false;$released=$false;$jumpAt=0;$scenes=@{}
 while($watch.ElapsedMilliseconds-lt9000){
  if(!$fired-and$watch.ElapsedMilliseconds-ge350){$dirty=$true;I $Source Neutral 1300 255 $Move;$fired=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='LT held for 1300ms then native release';pitch=$Pitch;stance=$Stance;leftY=$Move})}
  if($fired-and!$jumped-and$watch.ElapsedMilliseconds-ge950){
   I $Source A 350 255 $Move;$jumped=$true;$jumpAt=$watch.ElapsedMilliseconds
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native jump while cooking; trigger still held'})
  }
  if($jumped-and!$released-and$watch.ElapsedMilliseconds-ge($jumpAt+350)){
   I $Source Neutral 100 0 $Move;$released=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native trigger release after jump; no velocity or gameplay calls'})
  }
  $midnight=Q $Source '--midnight';$player=Q $Source '--player-states';$item=$null
  try{$item=Q $Source '--player-grenade'}catch{}
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;midnight=$midnight;player=$player;item=$item;fixture=(S $Source '.grenade-fixture.json')})
  foreach($s in $midnight.instances){if($s.state-eq3-and$s.asset-match'/gadget_grenade_'){$scenes[$s.asset]=$true}}
  Start-Sleep -Milliseconds 15
 }
 if($monitor-and$ValidateReader){
  $monitor.WaitForExit(2000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native action monitor incomplete'}
  $samples=@(Get-Content "$out/owned-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json});$expected=if($Pitch-ge30){@(3,4,7,8)}else{@(3,4,5,6)}
  foreach($v in $expected){if(!@($samples|Where-Object {$_.valid-and$_.variant-eq$v}).Count){throw "Owned native action variant $v missing"}}
  if((Q $Source '--player-grenade-scene').valid){throw 'Native action reader failed to clear'}
  $checks.Add('Exact native scene reader validates cooking, release and empty recovery then clears')
  foreach($s in $samples){if($s.valid){$scenes[$s.asset]=$true}}
 }
 foreach($part in 'cooking_in','cooking_idle') {if(!$scenes.ContainsKey("midnight/gameplay/player/base/gadgets/gadget_grenade_$part.midnightscene")){throw "Native $part missing"}}
 if(!@($scenes.Keys|Where-Object {$_-match'/gadget_grenade_throw_(high|low)\.midnightscene$'}).Count){throw 'Native throw scene missing'}
 $checks.Add('Actual native cooking entry, cooking idle and throw scene observed')
 if(!$jumped-or!$released){throw 'Airborne input sequence did not execute'}
 if(!@($rows|Where-Object {@($_.player.active|Where-Object {$_.id-in@(2,3)}).Count}).Count){throw 'Native jump/fall was not observed; input is not airborne coverage'}
 $checks.Add('Native jump/fall state observed independently of controller input')
 if(!@($rows|Where-Object {$null-ne$_.item-and$_.item.quantity-eq0}).Count){throw 'No native grenade consumption observed'};$checks.Add('Native action consumed the single granted grenade')
 C $Source 'coop_grenade_fixture off';Start-Sleep -Milliseconds 1200
 foreach($r in 'host','client'){
  $inv=Q $r '--player-inventory';$inv|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($beforeInv[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($inv.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory rollback mismatch'}
  [ordered]@{player=(Q $r '--health' 1);avatar=(Q $r '--health' (S $r).probeActorHandle)}|ConvertTo-Json -Depth 6|Set-Content "$out/health-before-reload-$r.json"
 }
 $checks.Add('Both inventories restored to the pre-grant quantities')
 Q $Source '--entities'|ConvertTo-Json -Depth 10|Set-Content "$out/entities-after.json"
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_grenade_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(15000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure='Capture incomplete'}}
 if($monitor){$monitor.WaitForExit(11000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure='Native action monitor incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content "$out/states.json"
 # A native grenade changes the temporary world. Restore both isolated
 # checkpoints even if an observation/check failed. Never save this world.
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Post-reload health differs'};if((Q $r '--player-grenade').quantity){throw 'Grenade remains after reload'}};$checks.Add('Both isolated checkpoints restored with original player/avatar health and no grenades')}catch{$failure='Checkpoint restoration failed: '+$_.Exception.Message}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=$_.Exception.Message}
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;pitch=$Pitch;stance=$Stance;leftY=$Move;checks=$checks;events=$events;scenes=@($scenes.Keys);dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$root/work/build/Release/d2_runtime_probe.exe").Hash;scope='Source native one-grenade jump/cook/release observation only. Input alone does not establish airborne overlap; inspect exact state/scene rows. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_GRENADE_CONTEXT_OBSERVATION $Source $($checks.Count) checks"
