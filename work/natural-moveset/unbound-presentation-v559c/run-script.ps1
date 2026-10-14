param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='unbound-presentation-v559')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
Copy-Item $PSCommandPath "$out/run-script.ps1"
$identity=Get-Content "$root/work/movement-integration/candidate-unbound-presentation-v558/identity.json" -Raw|ConvertFrom-Json
if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$identity.dll-or(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash-ne$identity.probe){throw 'Pinned558 candidate required'}
$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$captures=@();$chapters=[Collections.Generic.List[object]]::new();$pids=@{};$handles=@{};$probe=Join-Path $lab 'bin/d2_runtime_probe.exe'
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms|Out-Null}
function ReadStatus($r,$suffix=''){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function B($r){ReadStatus $r};function M($r){ReadStatus $r '.movement.json'};function S($r){ReadStatus $r '.shadow-stream.json'}
function Q($r,$mode,$id=0){for($a=0;$a-lt8;$a++){$raw=if($id){& $probe $pids[$r] $mode $id}else{& $probe $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 10};throw "Read $r $mode"}
function Sample($src,$dst,$trial,$ms){
 $m=M $dst;$s=S $dst;if(!$m.active-or!$s.armed-or!$s.ready){throw "Stream retired $src $trial $($s.result)"}
 if(!(ReadStatus $dst '.observer-isolation.json').ready-or!(ReadStatus $src '.observer-isolation.json').ready){throw 'Native fixture retired'}
 $row=[pscustomobject]@{source=$src;trial=$trial;ms=$ms;movement=$m;stream=$s;sourceStream=(S $src);native=(Q $src '--player-states');sword=(Q $dst '--avatar-sword' $handles[$dst]);equipment=(ReadStatus $dst '.shadow-equipment.json')};$rows.Add($row);return $row
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(B $r).probeActorHandle;I $r;C $r 'coop_shadow_unbound_probe off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'
  $role=if($r-eq'host'){'emily'}else{'corvo'}
  $path="$PSScriptRoot/shadowwalk-sneak-banks-v548/$role-shadowwalk-sneak-bank.bin"
  Copy-Item -LiteralPath $path -Destination "$root/work/lab-instances/$r/coop-status.json.movement.bin"
  if((Q $r '--health' 1).current-ne56-or!(Q $r '--player-sword').equipped){throw 'Clean sword fixture required'}
 }
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'}
  C $src 'setviewpos -33.5 -153 68.1 90 0';C $dst 'setviewpos -33.5 -150 68.1 270 0'
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $dst -Pitch 12 -Evidence "$out/camera-$src.json"
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
  Start-Sleep -Milliseconds 600;C $dst 'coop_movement_bank on'
  $readyWatch=[Diagnostics.Stopwatch]::StartNew();$pairReady=$false
  do {
   $m=M $dst;$a=S $src;$b=S $dst
   $pairReady=$m.active-and$m.version-eq59-and$m.weight-gt.9-and$a.armed-and$a.ready-and$b.armed-and$b.ready-and(B $src).probeFollowEnabled-and(B $dst).probeFollowEnabled
   if(!$pairReady){Start-Sleep -Milliseconds 50}
  } while(!$pairReady-and$readyWatch.ElapsedMilliseconds-lt8000)
  if(!$pairReady){[ordered]@{movement=$m;source=$a;peer=$b;sourceStatus=(B $src);peerStatus=(B $dst)}|ConvertTo-Json -Depth 8|Set-Content "$out/ready-failure.json";throw 'Equipment bank and paired stream did not become ready'}
  $window=(Get-Process -Id $pids[$dst]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','48','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-$src.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-$src.log" -RedirectStandardOutput "$out/capture-$src.stdout" -PassThru;$captures+=$capture
  Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Capture failed'};$overall=[Diagnostics.Stopwatch]::StartNew()
  foreach($stance in 0,1){
   if((B $src).localPlayerStance-ne$stance){I $src B 180;Start-Sleep -Milliseconds 1200}
   foreach($trial in 'holster','draw'){
    $variant=if($trial-eq'draw'){1+2*$stance}else{2+2*$stance};$held=$trial-eq'draw';$samples=[Collections.Generic.List[object]]::new()
    $chapters.Add([pscustomobject]@{source=$src;stance=$stance;action=$trial;videoTime=(.7+$overall.Elapsed.TotalSeconds)})
    $fallbackBefore=(S $src).unboundSamples
    C $src $(if($held){'coop_shadow_unbound_probe off'}else{'coop_shadow_unbound_probe 6000'})
    I $src X $(if($held){650}else{1500});$w=[Diagnostics.Stopwatch]::StartNew()
    while($w.ElapsedMilliseconds-lt3000){$samples.Add((Sample $src $dst "$trial-$stance" $w.ElapsedMilliseconds));Start-Sleep -Milliseconds 15}
    if($held){
     $active=@($samples|Where-Object {$_.movement.equipmentVariant-eq$variant-and$_.movement.equipmentWeight-gt.8-and$_.movement.weight-gt.9})
     if(!$active.Count){throw "Native authored draw did not recover $src stance $stance"}
    }else{
     $active=@($samples|Where-Object {$_.stream.remoteState-eq18-and$_.movement.weight-gt.9-and$_.sourceStream.unboundSamples-gt$fallbackBefore})
     if(!$active.Count){throw "Explicit Steam unbound fallback not observed $src stance $stance"}
     if(@($active|Where-Object {$_.movement.equipmentWeight-gt.02-or$_.movement.powerWeight-gt.02-or$_.stream.remoteVariant-or$_.stream.remotePhase}).Count){throw 'Unbound presentation retained active action weight'}
     C $src 'coop_shadow_unbound_probe off'
    }
    $eq=Q $dst '--avatar-sword' $handles[$dst];$m=M $dst
    if($eq.explicitHidden-ne(!$held)-or$eq.equipped-ne$held-or$eq.started-ne$held-or[Math]::Abs($m.armedMix-$(if($held){1}else{0}))-gt.01-or$m.equipmentWeight-gt.05){throw 'Equipment final state differs'}
    $checks.Add("${src}: stance $stance $trial $(if($held){'returns to existing native authored binding'}else{'deliberately missing binding uses explicit Steam state18 and continuous ground body'}); settles to matching native sword and visibility")
    if(!$held){
     foreach($d in @(@(0,12000),@(12000,0),@(0,-12000),@(-12000,0))){
      C $src 'setviewpos -33.5 -153 68.1 90 0';Start-Sleep -Milliseconds 300
      & "$lab/Send-D2Input.ps1" -Role $src -LeftX $d[0] -LeftY $d[1] -DurationMs 650|Out-Null
      $w=[Diagnostics.Stopwatch]::StartNew();$moved=$false
      while($w.ElapsedMilliseconds-lt1000){$row=Sample $src $dst "unarmed-$stance-$($d[0])-$($d[1])" $w.ElapsedMilliseconds;if($row.movement.speed-gt.1-and$row.movement.weight-gt.9-and$row.movement.armedMix-lt.01){$moved=$true};Start-Sleep -Milliseconds 15}
      if(!$moved){throw 'Unarmed directional movement missing'}
     }
     $checks.Add("${src}: stance $stance unarmed forward/sideways/backward retain distance-clocked authored body")
    }
   }
  }
  C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'};Start-Sleep -Milliseconds 500
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
  $restored=Q $dst '--avatar-sword' $handles[$dst];$lease=ReadStatus $dst '.shadow-equipment.json'
  if($restored.explicitHidden-or!$restored.equipped-or$lease.active-or$lease.restoring-or$lease.lost-or$lease.visibilityRefused){throw 'Visibility cleanup failed'}
  $checks.Add("${src}: original visible drawn sword restored, no visibility refusal or lost lease; health unchanged")
  $capture.WaitForExit(50000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){throw 'Capture did not finish'}
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $checks.Add('All fourteen protected settings/save hashes unchanged')
}catch{$failure=$_.Exception.Message;foreach($r in 'host','client'){try{[ordered]@{status=(B $r);movement=(M $r);stream=(S $r);fixture=(ReadStatus $r '.observer-isolation.json')}|ConvertTo-Json -Depth 8|Set-Content "$out/failure-$r.json"}catch{}}}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_shadow_unbound_probe off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 1200};if((B $r).localPlayerStance-eq1){I $r B 180};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 foreach($rec in $captures){if(!$rec.HasExited){$rec.WaitForExit(50000)|Out-Null}}
 if($pids.Count-eq2){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (B $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed after restore'}}
  $checks.Add('Both isolated checkpoints restore; all14 protected save/settings hashes retained')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 13|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;chapters=$chapters;bankManifest='shadowwalk-sneak-bank-manifest-v548.json';probe=$identity.probe;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Both-role standing/crouching missing-equipment-binding fault injection. Native X input and verified ground/sword ownership continue; explicit Steam state18 preserves existing locomotion and known draw binding recovers. Four holstered directions sampled. Does not establish complete unsupported-action coverage or visual acceptance. Native first person untouched; checkpoint/protected saves restored.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"EQUIPMENT_BANK_VALIDATED $($checks.Count) checks"
