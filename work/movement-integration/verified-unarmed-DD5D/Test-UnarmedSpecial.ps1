param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='unarmed-special-v1-test',[ValidateSet('both','host','client')][string]$OnlyRole='both')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$handles=@{};$checks=[Collections.Generic.List[string]]::new();$rows=[Collections.Generic.List[object]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$x=0,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftX $x -LeftY $y|Out-Null}
function Status($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe failed $r $mode"}
function Collect($src,$dst,$label,$ms){
 $samples=[Collections.Generic.List[object]]::new();$watch=[Diagnostics.Stopwatch]::StartNew()
 while($watch.ElapsedMilliseconds-lt$ms){
  $m=Status $dst '.movement.json';$s=Status $dst '.shadow-stream.json'
  if(!$m.active-or$m.version-ne10-or!$s.ready-or!$s.armed){throw "Presentation retired during $label"}
  if(!(Status $src '.observer-isolation.json').ready-or!(Status $dst '.observer-isolation.json').ready){throw 'Fixture retired'}
  $base=Status $src;$row=[pscustomobject]@{source=$src;label=$label;ms=$watch.ElapsedMilliseconds;movement=$m;stream=$s;nativeMotion=$base.localPlayerMotion;sourcePosition=$base.localPosition;observerPosition=(Status $dst).localPosition};$samples.Add($row);$rows.Add($row);Start-Sleep -Milliseconds 35
 }
 $sword=Q $dst '--avatar-sword' $handles[$dst]
 if($sword.equipped-or$sword.started){throw "Weaponless action $label redrew remote sword"}
 return $samples
}
function ResetPosition($src){
 if(@((Q $src '--player-states').active|Where-Object id -eq 18).Count){I $src Neutral 650 0 -22000;Start-Sleep -Milliseconds 900}
 I $src;C $src 'setviewpos -33.5 -153 68.1 0 0';Start-Sleep -Milliseconds 750
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(Status $r).probeActorHandle;I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'
  if(!(Q $r '--player-sword').equipped-or(Q $r '--health' 1).current-ne56){throw 'Clean armed fixture required'}
  $role=if($r-eq'host'){'emily'}else{'corvo'};Copy-Item "$PSScriptRoot/unarmed-special-banks-v1/$role-unarmed-special-bank.bin" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
 }
 foreach($src in $(if($OnlyRole-eq'both'){@('host','client')}else{@($OnlyRole)})){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
  C $src 'setviewpos -33.5 -153 68.1 0 0';C $dst 'setviewpos -30.5 -149 68.1 233.13 0'
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
  Start-Sleep -Milliseconds 500;C $dst 'coop_movement_bank on';Start-Sleep -Milliseconds 1100
  I $src X 1500;Start-Sleep -Milliseconds 3000
  foreach($stance in 0,1){
   if((Status $src).localPlayerStance-ne$stance){I $src B 180;Start-Sleep -Milliseconds 1000}
   I $src A 180;$s=Collect $src $dst "jump-$stance" 2200
   foreach($phase in 1,2,3){if(!@($s|Where-Object {$_.movement.airState-eq$phase-and$_.movement.airWeight-gt.5-and$_.movement.armedMix-lt.01-and$_.movement.weight-gt.9}).Count){throw "Missing unarmed jump phase $phase stance $stance"}}
   $checks.Add("${src}: unarmed stance $stance native jump/rise/fall/landing uses hands-free body; sword stays holstered")
   foreach($side in -32767,32767){
    I $src Y 650 $side 0;$s=Collect $src $dst "lean-$stance-$side" 1100
    if(!@($s|Where-Object {$_.movement.leanWeight-gt.8-and[Math]::Abs($_.movement.leanAmount)-gt.8-and$_.movement.armedMix-lt.01}).Count){throw 'Missing weaponless lean'}
    I $src Neutral 250 0 -16000;Start-Sleep -Milliseconds 550;ResetPosition $src
   }
   $checks.Add("${src}: unarmed stance $stance left/right lean and native exit preserve empty equipment")
  }
  if((Status $src).localPlayerStance-eq1){I $src B 180;Start-Sleep -Milliseconds 900}
  foreach($x in -14000,0,14000){
   ResetPosition $src;I $src LeftThumb 1000 $x 28000;$s=Collect $src $dst "sprint-$x" 1600
   if(!@($s|Where-Object {$_.movement.sprintMix-gt.85-and$_.movement.speed-gt2-and$_.movement.armedMix-lt.01-and$_.movement.weight-gt.9}).Count){throw 'Missing weaponless sprint'}
  }
  $checks.Add("${src}: unarmed forward/diagonal sprint uses separate donor arms and distance-clocked stride")
  # Reuse the native slide fixture's proven lane, away from the observer's
  # capsule. The longer filming lane intersects furniture in Emily's world.
  C $dst 'setviewpos -31 -146 68.1 270 8';C $src 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 800;I $src LeftThumb 1000 0 32767;Start-Sleep -Milliseconds 350;I $src B 500 0 32767
  $s=Collect $src $dst 'slide' 4300
  if(!@($s|Where-Object {$_.movement.slideState-eq1-and$_.movement.slideWeight-gt.8-and$_.movement.armedMix-lt.01}).Count-or!@($s|Where-Object {$_.movement.slideState-eq2}).Count){throw 'Missing unarmed slide/recovery'}
  $checks.Add("${src}: native unarmed slide and crouched recovery retain holstered sword")
  ResetPosition $src;if((Status $src).localPlayerStance-eq1){I $src B 180;Start-Sleep -Milliseconds 900};I $src X 650
  $watch=[Diagnostics.Stopwatch]::StartNew();$settled=$false
  while($watch.ElapsedMilliseconds-lt3500){
   $m=Status $dst '.movement.json';$stream=Status $dst '.shadow-stream.json';$native=Q $src '--player-states';$sword=Q $dst '--avatar-sword' $handles[$dst]
   $rows.Add([pscustomobject]@{source=$src;label='draw-recovery';ms=$watch.ElapsedMilliseconds;movement=$m;stream=$stream;native=$native;sword=$sword})
   if(!$m.active-or!$stream.ready){throw 'Armed recovery presentation retired'}
   if($m.armedMix-ge.99-and$sword.equipped-and$sword.started-and$m.equipmentWeight-lt.01){$settled=$true;break};Start-Sleep -Milliseconds 35
  }
  if(!$settled){throw 'Armed recovery missing'}
  $checks.Add("${src}: native draw restores the armed full-body path")
  C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off';if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 1400};if((Status $r).localPlayerStance-eq1){I $r B 180};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 10|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Native unarmed sprint/lean/slide/jump inputs over Steam, hands-free whole-body variants, equipment and health recovery. Visual quality and full movement completion remain separate.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"UNARMED_SPECIAL_VALIDATED $($checks.Count) checks"
