param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='npc-sleep-knockout-v1251',[string]$Candidate='candidate-npc-combat-v1213s')
# 1251 (#46): does the game's own sleep_no_reaction_damage knock an NPC out like a choke (probe 1250: health
# 40/40 -> 10/10, actor flag 0x20, AI trees destroyed)? The host stands 3 m in front of the Dunwall upper-floor
# tutorial guard 2 (AI held by the movement fixture), records its window, and sends the lab-only
# coop_npc_combat knockout_test once (1213s applyNativeKnockout, attacker = local player). Recorded: the test
# result file and the guard's readouts before and after. Pass: result "unconscious" with flag 0x20 and health
# > 0, matching the native choke knockout. Fixture restored afterwards.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$inventory=@{};$inv=[Globalization.CultureInfo]::InvariantCulture
function Save($p,$v){$v|ConvertTo-Json -Depth 20|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r$suffix"}
function Q($r,$mode,$arg=0){for($a=0;$a-lt6;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){try{return($raw|ConvertFrom-Json)}catch{return @{raw="$raw"}}};Start-Sleep -Milliseconds 20};return @{failed=$mode;exit=$LASTEXITCODE}}
function F($v){([double]$v).ToString('F5',$inv)}
function Readouts($h){[pscustomobject][ordered]@{world=@((Q host '--world-npcs').npcs|Where-Object {$_.handle-eq$h})|Select-Object -First 1;health=(Q host '--health' $h);decision=(Q host '--ai-decision' $h);reflex=(Q host '--ai-reflex' $h);idle=(Q host '--ai-idle' $h);animation=(Q host '--animation' $h)}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='NpcSleepKnockout1251';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;$dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r;if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"};$inventory[$r]=Q $r '--player-inventory'}
 Save "$out/identity.json" @{candidate=$Candidate;dll=$dll}
 $dirty=$true
 foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
 C client 'setviewpos -33 -153 68.1 90 0'
 $g=@((Q host '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'});if($g.Count-ne1){throw 'Guard absent'};$g=$g[0];$h=[uint32]$g.handle
 $fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$px=[double]$g.position[0]+3*$fx;$py=[double]$g.position[1]+3*$fy;$yaw=[Math]::Atan2(-$fy,-$fx)*180/[Math]::PI
 C host ('setviewpos {0} {1} 68.1 {2} 0' -f (F $px),(F $py),$yaw.ToString('F4',$inv));Start-Sleep -Milliseconds 900
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 12 -Quick|Out-Null
 Save "$out/before.json" (Readouts $h)
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $cap=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','8','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/host-knockout.mp4") -WindowStyle Hidden -RedirectStandardError "$out/host-capture.log" -RedirectStandardOutput "$out/host-capture.stdout" -PassThru
 Start-Sleep -Milliseconds 1500;C host 'coop_npc_combat knockout_test';$w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$k=try{S host '.npc-knockout.json'}catch{$null}}while((!$k-or$k.result-eq'idle')-and$w.ElapsedMilliseconds-lt5000)
 Save "$out/knockout.json" $k
 $samples=[Collections.Generic.List[object]]::new();for($i=0;$i-lt8;$i++){$samples.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;health=(Q host '--health' $h);world=@((Q host '--world-npcs').npcs|Where-Object {$_.handle-eq$h})|Select-Object -First 1});Start-Sleep -Milliseconds 400}
 Save "$out/samples.json" @($samples)
 if(!$cap.WaitForExit(20000)){Stop-Process -Id $cap.Id}
 Save "$out/after.json" (Readouts $h)
 $result=[ordered]@{knockout=$k}
 if(!$k-or$k.result-ne'unconscious'){throw "Knockout test result: $($k.result)"}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){try{foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off'}
    & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
    & "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration" *> "$out/note-restoration.log"
    foreach($r in 'host','client'){$restored=Q $r '--player-inventory';if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){$errors.Add("Inventory differs ($r)")};if((Q $r '--health' 1).current-ne56){$errors.Add("Health differs ($r)")}}
   }catch{$errors.Add($_.Exception.Message)}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}}catch{$errors.Add($_.Exception.Message)}
  foreach($r in 'host','client'){try{if((S $r).localGameLogicState-ne1){$errors.Add("$r not back in gameplay")}}catch{$errors.Add("$r status unavailable")}}
  if(Test-Path -LiteralPath $out){Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
   Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;scope='Native sleep_no_reaction_damage knockout of the Dunwall upper-floor guard 2 on the host (lab test command); readouts and host video.'})}
  [ordered]@{pid=$PID;operation='NpcSleepKnockout1251';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_NPC_SLEEP_KNOCKOUT'
