param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='gait-priority-1',[switch]$AuthoredAir,[switch]$AuthoredCombat)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$records=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$pids=@{};$failure=$null;$paused=$null
$liveDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function M($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.movement.json" -Raw|ConvertFrom-Json}
function WaitBankReady($r){$watch=[Diagnostics.Stopwatch]::StartNew();do{$m=M $r;if($m.active-and$m.weight-ge.9){return};Start-Sleep -Milliseconds 35}while($watch.ElapsedMilliseconds-lt5000);throw 'Bank did not become ready within its asynchronous load deadline'}
function Native($r,$opt,$h=''){$args=@($pids[$r],$opt);if($h){$args+=$h};for($i=0;$i-lt8;$i++){$raw=& "$lab/bin/d2_runtime_probe.exe" @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Read-only probe failed $r $opt"}
function Arm {
 foreach($r in 'host','client'){C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'}
 foreach($r in 'host','client'){C $r $(if($AuthoredCombat){'coop_shadow_stream on'}else{'coop_shadow_stream 15000'})};Start-Sleep -Milliseconds 500
}
function Capture($src,$dst,$stage){$a=[pscustomobject]@{source=$src;stage=$stage;native=(Native $src '--player-states');bridge=(B $src);target=(B $dst);movement=(M $dst);action=(Get-Content "$root/work/lab-instances/$dst/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)};$records.Add($a);return $a}
function Slash($src,$dst,$motion){
 & "$lab/Send-D2Input.ps1" -Role $src -RightTrigger 255 -DurationMs 180|Out-Null
 $observed=$false;$w=[Diagnostics.Stopwatch]::StartNew()
 do{$s=Capture $src $dst "slash-motion-$motion";if(@($s.native.active|Where-Object id -eq 39).Count-and$s.bridge.localPlayerMotionValid-and$s.bridge.localPlayerMotion-eq$motion-and$(if($AuthoredCombat){$s.movement.combatWeight-gt.6-and$s.movement.weight-gt.8}else{$s.movement.weight-eq0})){$observed=$true};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt1100)
 if(!$observed){throw "Native slash lost movement or bank priority: $src motion=$motion"}
 $checks.Add("$src Slash retains native motion=$motion and selects the intended combat presentation (authored=$AuthoredCombat)")
 Start-Sleep -Milliseconds 500
}
try{
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid;I $r;C $r 'coop_movement_bank off'}
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
  C $src 'setviewpos -33.5 -153 68.1 0 0';C $dst 'setviewpos -30.5 -149 68.1 233.13 0';Arm;C $dst 'coop_movement_bank 60000';WaitBankReady $dst
  if((M $dst).weight-lt.9){throw 'Bank did not become ready'}
  Slash $src $dst 0
  I $src RightShoulder 1500;Start-Sleep -Milliseconds 550;$s=Capture $src $dst 'standing-block'
  if(!@($s.native.active|Where-Object id -eq 44).Count-or!$s.bridge.localPlayerMotionValid-or$s.bridge.localPlayerMotion-ne0-or$(if($AuthoredCombat){$s.movement.weight-lt.8-or$s.movement.combatWeight-lt.6-or$s.action.actionActive}else{$s.movement.weight-ne0-or!$s.action.actionActive})){throw 'Standing block did not preserve motion and yield to native action'}
  $checks.Add("$src standing Block retains motion=0 and selects the intended guard presentation (authored=$AuthoredCombat)")
  I $src;Start-Sleep -Milliseconds 1700
  if((M $dst).weight-lt.9){throw 'Bank did not recover after block'}
  I $src B 180;Start-Sleep -Milliseconds 650
  Slash $src $dst 1
  I $src RightShoulder 1500;Start-Sleep -Milliseconds 550;$s=Capture $src $dst 'crouched-block'
  if(!@($s.native.active|Where-Object id -eq 44).Count-or!$s.bridge.localPlayerMotionValid-or$s.bridge.localPlayerMotion-ne1-or$(if($AuthoredCombat){$s.movement.weight-lt.8-or$s.movement.combatWeight-lt.6}else{$s.movement.weight-ne0})){throw 'Crouched block lost native locomotion identity'}
  $checks.Add("$src crouched Block retains motion=1; standing bank does not override crouch")
  I $src;Start-Sleep -Milliseconds 1000;I $src B 180;Start-Sleep -Milliseconds 750
  C $dst 'coop_movement_bank off';Arm;C $dst 'coop_movement_bank 60000';WaitBankReady $dst
  I $src A 180;$jump=@();for($i=0;$i-lt12;$i++){$jump+=Capture $src $dst 'jump';Start-Sleep -Milliseconds 30}
  if($AuthoredAir){
   if(!@($jump|Where-Object {$_.bridge.localPlayerMotionValid-and$_.bridge.localPlayerMotion-in2,3-and$_.movement.airState-in1,2-and$_.movement.airWeight-gt.6-and!$_.action.actionActive}).Count){throw 'Native airborne state did not select authored airborne motion'}
   $checks.Add("$src native jump/fall selects authored airborne motion without competing clone action")
  }else{
   if(!@($jump|Where-Object {$_.bridge.localPlayerMotionValid-and$_.bridge.localPlayerMotion-in2,3-and$_.movement.weight-eq0}).Count){throw 'Native airborne state did not suppress bank'}
   $checks.Add("$src native jump/fall suppresses the standing gait bank")
  }
  Start-Sleep -Milliseconds 1600
  C $dst 'coop_movement_bank off';Arm;C $dst 'coop_movement_bank 60000';WaitBankReady $dst
  if($AuthoredCombat){I $src RightShoulder 3000;Start-Sleep -Milliseconds 750}
  I $src Start 180;$paused=$src;Start-Sleep -Milliseconds 650;$a=M $dst
  if((B $src).localGameLogicState-ne2-or$a.active){throw 'Native pause did not retire bank'}
  C $dst 'coop_movement_bank 60000';Start-Sleep -Milliseconds 350;$b=M $dst
  if($b.active-or$b.applied-ne$a.applied){throw 'Paused rearm or mutation accepted'}
  I $src A 180;Start-Sleep -Milliseconds 650;$paused=$null
  if((M $dst).active){throw 'Resume replayed retired bank'}
  $checks.Add("$src pause retires bank, rejects rearm, and resume does not replay it")
 }
 foreach($r in 'host','client'){
  if((Native $r '--health' 1).current-ne56-or(Native $r '--health' (B $r).probeActorHandle).current-ne40){throw 'Health changed'}
 }
 $checks.Add('Both players retain native checkpoint health 56; both independent avatars retain 40')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 if($paused-and(B $paused).localGameLogicState-eq2){I $paused A 180;Start-Sleep -Milliseconds 500}
 $records|ConvertTo-Json -Depth 15|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=@($checks);dll=$liveDll;scope='Native Block/movement coexistence, action/stance/airborne priority and pause for the experimental bank; no new combat outcomes or whole-family visual acceptance.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count}
if($failure){throw $failure}

