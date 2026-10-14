param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-shadow-equipment-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take already exists'};New-Item -ItemType Directory -Path $out|Out-Null
$probe=Join-Path $lab 'bin/d2_runtime_probe.exe';$pids=@{};$handles=@{};$checks=[Collections.Generic.List[string]]::new();$samples=[Collections.Generic.List[object]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function ReadStatus($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 20}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($attempt=0;$attempt-lt12;$attempt++){$raw=if($id){& $probe $pids[$r] $mode $id}else{& $probe $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe $r $mode"}
function E($r){ReadStatus $r '.shadow-equipment.json'}
function WaitFor($r,$predicate,$message){$w=[Diagnostics.Stopwatch]::StartNew();do{$s=E $r;if(&$predicate $s){return $s};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt5000);throw "$r $message : $($s|ConvertTo-Json -Compress)"}
function SaveSample($r,$label){$samples.Add([pscustomobject]@{role=$r;label=$label;lease=(E $r);equipment=(Q $r '--avatar-sword' $handles[$r]);attachment=(Q $r '--avatar-sword-attachment' $handles[$r]);player=(Q $r '--player-sword')})}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(ReadStatus $r).probeActorHandle
  C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';C $r 'setviewpos -33.5 -153 68.1 0 0'
 }
 foreach($r in 'host','client'){
  SaveSample $r 'before';$baseline=E $r
  C $r 'coop_shadow_equipment 2500';$held=WaitFor $r {param($s)$s.active-and$s.observed-gt$baseline.observed} 'Native holster not observed'
  SaveSample $r 'holstered';$sword=Q $r '--avatar-sword' $handles[$r]
  if($sword.equipped-or$sword.started-or$sword.attachedTag-ne'holster_wpn'){throw 'Native holster state disagrees'}
  $checks.Add("${r}: existing clone sword natively holsters")
  C $r 'coop_shadow_equipment 2500';$dup=E $r;if($dup.started-ne$held.started-or$dup.refused-le$held.refused){throw 'Duplicate renewed lease'}
  $checks.Add("${r}: duplicate refused without renewal")
  $restored=WaitFor $r {param($s)!$s.active-and!$s.restoring-and$s.restored-gt$baseline.restored} 'Original sword not restored'
  SaveSample $r 'expired-restored';$sword=Q $r '--avatar-sword' $handles[$r]
  if(!$sword.equipped-or!$sword.started-or$sword.attachedTag-ne'right_hand_wpn'-or$restored.lost-ne$baseline.lost){throw 'Native restore failed'}
  $checks.Add("${r}: finite expiry restores original sword and identity")
  $baseline=E $r;C $r 'coop_shadow_equipment 2500';WaitFor $r {param($s)$s.active-and$s.observed-gt$baseline.observed} 'Second holster failed'|Out-Null
  C $r 'coop_shadow_equipment off';WaitFor $r {param($s)!$s.active-and!$s.restoring-and$s.restored-gt$baseline.restored} 'Cancellation restore failed'|Out-Null
  SaveSample $r 'cancel-restored';$checks.Add("${r}: cancellation restores sword")
 }
 foreach($r in 'host','client'){
  if(!(Q $r '--player-sword').equipped-or(Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Player equipment or health changed'}
  $checks.Add("${r}: local first-person sword and both health values preserved")
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'coop_shadow_equipment off';C $r 'coop_movement_fixture off'}catch{}}
 $samples|ConvertTo-Json -Depth 12|Set-Content "$out/samples.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Local 2.5-second exact-clone native holster/restore. No remote event binding or animation/contact acceptance.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};$checks
