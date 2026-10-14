param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='partner-takedown-exclusion-host-v388',[ValidateSet('host','client')][string]$Source='host')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$capture=$null
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new()
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='PartnerTargetingObserve';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
 if($dll-ne'440B0F9FD8BE7D24D22B62DE0BD7FB43C2E9C85FE6AB3551DA9A988AA273C6CD'){throw 'Expected archived387 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true
 foreach($source in @($Source)){
  $peer=if($source-eq'host'){'client'}else{'host'}
  foreach($case in @(@{name='rear-choke';yaw=90;stance=0;distance=1.15})){
   I $source;I $peer
   $state=Q $source '--player-states';if($state.stance-ne$case.stance){I $source B 180;Start-Sleep -Milliseconds 500}
   C $source 'setviewpos -33.5 -153 68.1 90 0';C $peer "setviewpos -33.5 $(-153+$case.distance) 68.1 $($case.yaw) 0"
   Start-Sleep -Milliseconds 800
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$out/$source-$($case.name)-camera.json" -Quick
   $components=Q $source '--components' 1
   $sensors=@($components.components|Where-Object {$_.type-match'(?i)(PlayerSensor.*(Assassination|Interaction|Use|Grab)|PlayerFocus)'})
   if(!@($sensors|Where-Object type -eq '.?AVarkCpntPlayerSensorGlobalAssassination@@').Count){throw 'Global assassination sensor not found'}
   $key="$source-$($case.name)";$hwnd=(Get-Process -Id $pids[$source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
   $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','8','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$key.mp4")
   $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$key-capture.log" -RedirectStandardOutput "$out/$key-capture.stdout" -PassThru
   $filterBefore=Get-Content "$root/work/lab-instances/$source/coop-status.json.assassination-partner-filter.json" -Raw|ConvertFrom-Json;if(!$filterBefore.installed){throw "Takedown filter missing"}
   $watch=[Diagnostics.Stopwatch]::StartNew();$samples=[Collections.Generic.List[object]]::new();$sent=$false
   while($watch.ElapsedMilliseconds-lt7000){
    if(!$sent-and$watch.ElapsedMilliseconds-ge1100){I $source RightShoulder 2000;$sent=$true}
    $objects=@(foreach($c in $sensors){[pscustomobject]@{type=$c.type;pointer=$c.pointer;object=(Q $source '--object' $c.pointer)}})
    $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;player=(Q $source '--player-states');source=(S $source);peer=(S $peer);sensors=$objects;midnight=(Q $source '--midnight')})
    Start-Sleep -Milliseconds 80
   }
   $capture.WaitForExit(6000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){throw 'Native capture failed'};$capture=$null
   $samples|ConvertTo-Json -Depth 17|Set-Content "$out/$key-observations.json"
   $filterAfter=Get-Content "$root/work/lab-instances/$source/coop-status.json.assassination-partner-filter.json" -Raw|ConvertFrom-Json
   [ordered]@{before=$filterBefore;after=$filterAfter}|ConvertTo-Json -Depth 6|Set-Content "$out/filter.json"
   if(!$filterAfter.installed-or$filterAfter.blocked-le$filterBefore.blocked-or$filterAfter.readFailed-ne$filterBefore.readFailed){throw 'Exact partner eligibility filter did not reject consistently'}
   if(@($samples.player.active|Where-Object {$_.id-eq41}).Count){throw 'Partner choke state41 still entered'}
   $paired=@($samples.midnight.instances|Where-Object {$_.state-eq3-and$_.asset-match'(?i)(/choke/|assassin)'})
   if($paired.Count){throw 'Partner paired takedown scene still entered'}
   $checks.Add('Native partner choke input rejected before state41 or paired takedown scene; exact partner filter actively rejected with no ownership read refusal')

   $rows.Add([pscustomobject]@{source=$source;case=$case;key=$key;count=$samples.Count;sensors=@($sensors.type);partnerHandle=(S $source).probeActorHandle})
   foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed during neutral targeting observation'}}
  }
 }
 $checks.Add('Native RightShoulder held2000ms while behind exact partner; targeting/state/scene and native video observations collected. Outcome requires analysis.')
}catch{$failure=$_.Exception.Message}finally{
 if($owned){
  foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_fixture off'}catch{}}
  if($capture){$capture.WaitForExit(10000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture incomplete').TrimStart('; ')}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
   foreach($r in 'host','client'){
    $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-checkpoint-restored-$r.json"
    if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete checkpoint inventory differs'}
    if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}
   };$checks.Add('Both checkpoint inventories and player/partner health restored')
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path -LiteralPath $out){[ordered]@{passed=(!$failure);failure=$failure;dll=$dll;checks=$checks;cases=$rows;scope='Scoped partner rear choke exclusion test on candidate387. Native input must produce active exact-identity rejection, no choke state41 or paired scene, and unchanged health. Other takedowns, contexts and ordinary NPC control remain separate; visual review required. Normal native movement collision intentionally retained.'}|ConvertTo-Json -Depth 10|Set-Content "$out/result.json"}
  [ordered]@{pid=$PID;operation='PartnerTargetingObserve';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_PARTNER_TARGETING_OBSERVATION $($checks.Count) checks"
