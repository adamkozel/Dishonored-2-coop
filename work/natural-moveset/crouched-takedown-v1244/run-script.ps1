param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='crouched-takedown-v1244',[string[]]$Sources=@('host','client'),[string[]]$Kinds=@('choke','lethal'))
# 1244 (#11, crouched_takedown): native observation only. The partner reader accepts a rear choke from standing
# (0) or choke stance (12) and a lethal fast-back from standing; a takedown started from CROUCH was never
# observed. For each source role and kind, the source crouches 0.9 m behind the Dunwall upper-floor tutorial
# guard 2 (AI held by the movement fixture) and uses ordinary input: RightShoulder held (choke to finish) or
# RightTrigger (lethal). Both --watch-midnight and --watch-player-states run through the action; the source
# window is recorded. Pass: a choke/assassination player scene is observed in every case. The checkpoint
# fixture (PrepareShadowSword) is restored after every case.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$inv=[Globalization.CultureInfo]::InvariantCulture
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
function F($v){([double]$v).ToString('F5',$inv)}
function Guard($r){$g=@((Q $r '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'});if($g.Count-ne1){throw 'Exact upper-floor guard2 absent'};$g[0]}
function Restore{& "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"
 foreach($r in 'host','client'){$restored=Q $r '--player-inventory'
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw "Checkpoint inventory differs ($r)"}
  if((Q $r '--health' 1).current-ne56){throw "Restored player health differs ($r)"}}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='CrouchedTakedown1244';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-$r.json"
 }
 [ordered]@{dll=$dll;sources=$Sources;kinds=$Kinds}|ConvertTo-Json|Set-Content "$out/identity.json"
 foreach($source in $Sources){
  $peer=if($source-eq'host'){'client'}else{'host'}
  foreach($kind in $Kinds){
   $key="$source-$kind";$row=[ordered]@{source=$source;kind=$kind;passed=$false;failure=$null}
   try{
    $dirty=$true
    foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
    C $peer 'setviewpos -28.5 -158 68.1 90 0'
    $g=Guard $source;$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1]
    if([Math]::Abs($fx*$fx+$fy*$fy-1)-gt.01){throw 'Guard forward not horizontal'}
    $px=[double]$g.position[0]-.9*$fx;$py=[double]$g.position[1]-.9*$fy;$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
    C $source ('setviewpos {0} {1} 68.1 {2} 0' -f (F $px),(F $py),$look.ToString('F4',$inv));Start-Sleep -Milliseconds 800
    $st=Q $source '--player-states';if($st.stance-ne1){I $source B 180;Start-Sleep -Milliseconds 700;$st=Q $source '--player-states'}
    $row.stanceBefore=$st.stance;$row.statesBefore=$st
    if($st.stance-ne1){throw "Source not crouched (stance $($st.stance))"}
    $g2=Guard $source;$row.guardBefore=@{position=$g2.position;orientation=$g2.orientation}
    $hwnd=(Get-Process -Id $pids[$source]).MainWindowHandle.ToInt64()
    $cap=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','11','-an','-c:v','libx264','-preset','veryfast','-crf','20','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$key-source.mp4") -WindowStyle Hidden -RedirectStandardError "$out/$key-capture.log" -RedirectStandardOutput "$out/$key-capture.stdout" -PassThru
    $scenes=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$source],'--watch-midnight','9000') -WindowStyle Hidden -RedirectStandardOutput "$out/$key-scenes.ndjson" -RedirectStandardError "$out/$key-scenes.stderr" -PassThru
    $states=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$source],'--watch-player-states','9000') -WindowStyle Hidden -RedirectStandardOutput "$out/$key-states.json" -RedirectStandardError "$out/$key-states.stderr" -PassThru
    Start-Sleep -Milliseconds 600
    if($kind-eq'choke'){I $source RightShoulder 6500}else{& "$lab/Send-D2Input.ps1" -Role $source -RightTrigger 255 -DurationMs 250|Out-Null}
    foreach($p in $scenes,$states){if(!$p.WaitForExit(15000)){Stop-Process -Id $p.Id;throw 'Native watch did not finish'}}
    if(!$cap.WaitForExit(20000)){Stop-Process -Id $cap.Id}
    $found=@{};$players=@{}
    foreach($line in Get-Content "$out/$key-scenes.ndjson"){try{$o=$line|ConvertFrom-Json}catch{continue}
     foreach($x in @($o.instances)){if("$($x.asset)"-match'/player/.*/(choke|assassination)/'){$found["$($x.asset)|$($x.subscene)|$($x.sceneDurationMs)"]=1}}}
    $sw=Get-Content "$out/$key-states.json" -Raw|ConvertFrom-Json
    foreach($rec in @($sw.records)){if($rec.valid){$players["stance $($rec.stance) / active $((@($rec.active)|ForEach-Object {$_.id}) -join ',')"]=1}}
    $row.scenes=@($found.Keys|Sort-Object);$row.playerStates=@($players.Keys|Sort-Object);$row.guardAfter=(Q $source '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'}|Select-Object -First 1 position,unique
    $row.passed=[bool]$row.scenes.Count
    if(!$row.passed){$row.failure='No choke/assassination player scene observed'}
   }catch{$row.failure=$_.Exception.Message}
   try{Restore;$row.restored=$true}catch{$row.restored=$false;$row.failure=(("$($row.failure); restore: "+$_.Exception.Message)).TrimStart('; ')}
   $rows.Add([pscustomobject]$row);[pscustomobject]$row|ConvertTo-Json -Depth 8|Set-Content "$out/$key.json"
   if(!$row.restored){throw "Fixture restore failed after $key"}
  }
 }
 $dirty=$false
}catch{$failure=$_.Exception.Message;Set-Content "$out/failure.json" (@{message=$failure;stack=$_.ScriptStackTrace}|ConvertTo-Json)}finally{
 if($owned){
  foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_fixture off'}catch{}}
  if($dirty){try{Restore}catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  $errors=[Collections.Generic.List[string]]::new()
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}}catch{$errors.Add($_.Exception.Message)}
  foreach($r in 'host','client'){try{if((S $r).localGameLogicState-ne1){$errors.Add("$r not back in gameplay")}}catch{$errors.Add("$r status unavailable")}}
  if(Test-Path -LiteralPath $out){
   @{passed=(!$errors.Count);errors=@($errors)}|ConvertTo-Json|Set-Content "$out/restoration.json"
   $bad=@($rows|Where-Object {!$_.passed});if(!$failure-and$bad.Count){$failure="$($bad.Count) case(s) without a takedown scene"}
   if($errors.Count-and!$failure){$failure=$errors-join '; '}
   [ordered]@{passed=(!$failure);failure=$failure;dll=$dll;cases=@($rows);scope='Native observation of rear choke and lethal takedowns started from crouch (Dunwall upper-floor tutorial guard 2), both source roles; ordinary controller input only; no partner presentation claims.'}|ConvertTo-Json -Depth 10|Set-Content "$out/result.json"
  }
  [ordered]@{pid=$PID;operation='CrouchedTakedown1244';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_CROUCHED_TAKEDOWN_PROBE $($rows.Count) cases"
