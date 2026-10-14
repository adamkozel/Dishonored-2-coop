param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-traversal-fixtures-v1',[switch]$VaultOnly,[switch]$SensorTrigger,[switch]$Unarmed,[ValidateRange(1,6)][int]$Repetitions=2)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt8;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native read $r $mode"}
# Native declaration requires speed >=3.65 m/s, obstacle height .55..1.25 m,
# depth <=1.8 m and two metres of clearance behind. These are candidate paths,
# not assumed vaults. No geometry or player-state fields are changed.
$cases=@(
 @{name='desk-clear-front';x=-30.2;y=-151.5;yaw=90;sprint=520},
 @{name='desk-clear-back';x=-30.2;y=-142;yaw=270;sprint=380},
 @{name='desk-side';x=-26.9;y=-146.6;yaw=180;sprint=180},
 @{name='shelf-high';x=-35.3;y=-148.3;yaw=180;sprint=0},
 @{name='bookcase-high';x=-36.7;y=-168.5;yaw=270;sprint=0}
)
if($VaultOnly){$cases=@(
 @{name='front-sprint-jump';x=-30.7;y=-150.5;yaw=90;sprint=260},
 @{name='front-right-sprint-jump';x=-29;y=-150.5;yaw=90;sprint=300},
 @{name='back-sprint-jump';x=-31;y=-143;yaw=270;sprint=200},
 @{name='front-sprint-only';x=-30.7;y=-150.5;yaw=90;sprint=0;automatic=$true},
 @{name='back-sprint-only';x=-31;y=-143;yaw=270;sprint=0;automatic=$true}
)}
if($SensorTrigger){$cases=@(
 @{name='front-sensor-trigger-a';x=-30.7;y=-150.5;yaw=90;sprint=0;automatic=$true},
 @{name='front-sensor-trigger-b';x=-30.7;y=-150.5;yaw=90;sprint=0;automatic=$true}
)}
if($SensorTrigger){$cases=@(for($i=1;$i-le$Repetitions;$i++){@{name="front-sensor-trigger-$i";x=-30.7;y=-150.5;yaw=90;sprint=0;automatic=$true}})}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/camera.json"
 if($Unarmed){I $Source X 1500;Start-Sleep -Milliseconds 2800;if((Q $Source '--player-sword').equipped){throw 'Sword did not holster'}}
 foreach($case in $cases){
  I $Source;if((Q $Source '--player-states').stance-eq1){I $Source B 180;Start-Sleep -Milliseconds 650}
  C $Source "setviewpos $($case.x) $($case.y) 68.1 $($case.yaw) 0";Start-Sleep -Milliseconds 800
  if($case.sprint){I $Source LeftThumb 1800 32767;Start-Sleep -Milliseconds $case.sprint}
  $buttons=if($case.automatic){@('LeftThumb')}elseif($case.sprint){@('A','LeftThumb')}else{@('A')}
  I $Source $buttons 1500 32767;$watch=[Diagnostics.Stopwatch]::StartNew();$triggered=$false
  while($watch.ElapsedMilliseconds-lt2600){
   $state=Q $Source '--player-states';$midnight=$null
   if($SensorTrigger-and!$triggered){$sensor=$state.vaultSensorResearch;$vel=$sensor.velocityCandidate33c
    if($sensor.front38-eq1-and$sensor.clearF0-eq1-and($vel[0]*$vel[0]+$vel[1]*$vel[1])-ge($sensor.minSpeed70*$sensor.minSpeed70)){
     I $Source @('A','LeftThumb') 1000 32767;$triggered=$true
    }
   }
   if(@($state.active|Where-Object {$_.id-eq8-or$_.id-eq9-or$_.id-eq53}).Count){$midnight=Q $Source '--midnight'}
   $rows.Add([pscustomobject]@{source=$Source;trial=$case.name;ms=$watch.ElapsedMilliseconds;triggered=$triggered;state=$state;midnight=$midnight});Start-Sleep -Milliseconds 15
  }
  if((Q $Source '--health' 1).current-ne56){throw 'Health changed'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;if((Q $r '--player-states').stance-eq1){I $r B 180;Start-Sleep -Milliseconds 650};C $r 'setviewpos -33.5 -153 68.1 0 0';if($Unarmed-and$r-eq$Source-and!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2200};C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 15|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;cases=$cases;scope='Ordinary native input at existing Dunwall desk and shelves. Read-only scene/state observations. No authored pose or geometry writes.'}|ConvertTo-Json -Depth 4|Set-Content "$out/result.json"
}
if($failure){throw $failure};"OBSERVED_TRAVERSAL_FIXTURES $($rows.Count) samples"
