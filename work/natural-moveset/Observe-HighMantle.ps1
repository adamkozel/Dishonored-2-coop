param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-high-mantle-v1',[switch]$Unarmed,[switch]$StairSide,[string]$CasesPath)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt8;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native read $r $mode"}
# Existing map placements near shelves carrying objects at z=68.49/69.07.
# Candidate approach points only: actual native physics and scene activation
# determine whether each surface is climbable. No forced player state.
$cases=@(
 @{name='west-shelf-front';x=-36.6;y=-172.1;z=68.1;yaw=180},
 @{name='west-shelf-near';x=-37.25;y=-172.1;z=68.1;yaw=180},
 @{name='bookcase-near';x=-36.7;y=-169.4;z=68.1;yaw=270},
 @{name='bookcase-right';x=-35.5;y=-169.4;z=68.1;yaw=270},
 @{name='bedroom-cabinet-east';x=-54.15;y=-165.4;z=68.1;yaw=180},
 @{name='bedroom-cabinet-north';x=-55.6;y=-163.8;z=68.1;yaw=270},
 @{name='bookcase-jump-near';x=-36.7;y=-169.85;z=68.1;yaw=270},
 @{name='west-shelf-north';x=-38.3;y=-170.25;z=68.1;yaw=270}
)
if($StairSide){$cases=@(
 @{name='rail-low-near';x=-44.3;y=-177.3;z=65.1;yaw=270},
 @{name='rail-low-far';x=-44.3;y=-176.5;z=65.1;yaw=270},
 @{name='rail-mid-near';x=-43.5;y=-177.3;z=65.1;yaw=270},
 @{name='rail-mid-far';x=-43.5;y=-176.5;z=65.1;yaw=270},
 @{name='rail-upper-near';x=-42.75;y=-177.3;z=65.1;yaw=270},
 @{name='rail-upper-far';x=-42.75;y=-176.5;z=65.1;yaw=270}
)}
if($CasesPath){$cases=@(Get-Content -LiteralPath $CasesPath -Raw|ConvertFrom-Json);if(!$cases.Count-or$cases.Count-gt12){throw 'Expected 1-12 approach cases'};foreach($case in $cases){if($case.name-notmatch'^[a-z0-9-]+$'){throw 'Invalid approach name'};foreach($field in 'x','y','z','yaw'){if($null-eq$case.$field-or![double]::IsFinite([double]$case.$field)){throw 'Invalid approach coordinate'}}}}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/camera.json"
 if($Unarmed-and(Q $Source '--player-sword').equipped){I $Source X 1500;Start-Sleep -Milliseconds 2800}
 foreach($case in $cases){
  I $Source;if((Q $Source '--player-states').stance-eq1){I $Source B 180;Start-Sleep -Milliseconds 750}
  foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
  C $Source "setviewpos $($case.x) $($case.y) $($case.z) $($case.yaw) 0";Start-Sleep -Milliseconds 1000
  if($case.crouch){I $Source B 180;Start-Sleep -Milliseconds 850;if((Q $Source '--player-states').stance-ne1){throw 'Crouched approach did not enter native low stance'}}
  $before=Q $Source '--player-states'
  $slot=Q $Source '--object' ('0x{0:x}' -f ([Convert]::ToUInt64($before.mode.Substring(2),16)+0x50))
  $stateObject=Q $Source '--object' $slot.words[0].word
  $sensorPointer=($stateObject.words|Where-Object offset -eq '0x108').word
  $sensorBefore=if($sensorPointer-and$sensorPointer-ne'0x0'){Q $Source '--object' $sensorPointer}else{$null}
  @{state=$stateObject;sensor=$sensorBefore;before=$before}|ConvertTo-Json -Depth 12|Set-Content "$out/sensor-$($case.name)-before.json"
  I $Source A 1300 22000;$w=[Diagnostics.Stopwatch]::StartNew();$sensorSaved=$false
  while($w.ElapsedMilliseconds-lt2600){
   $state=Q $Source '--player-states';$midnight=$null
   if(@($state.active|Where-Object {$_.id-eq8}).Count){$midnight=Q $Source '--midnight'}
   $rows.Add([pscustomobject]@{source=$Source;trial=$case.name;ms=$w.ElapsedMilliseconds;before=$before.position;state=$state;midnight=$midnight})
   if(!$sensorSaved-and$w.ElapsedMilliseconds-ge500){
    $stateObject=Q $Source '--object' $slot.words[0].word;$sensorPointer=($stateObject.words|Where-Object offset -eq '0x108').word
    if($sensorPointer-and$sensorPointer-ne'0x0'){Q $Source '--object' $sensorPointer|ConvertTo-Json -Depth 8|Set-Content "$out/sensor-$($case.name)-moving.json"};$sensorSaved=$true
   }
   Start-Sleep -Milliseconds 15
  }
  I $Source;if((Q $Source '--health' 1).current-ne56){throw 'Health changed'}
  $part=@($rows|Where-Object trial -eq $case.name);$kinds=@($part.state.active|Where-Object id -eq 8|ForEach-Object {$_.mantleResearch.kindd8}|Sort-Object -Unique)
  "MANTLE_OBSERVATION $($case.name) kinds=$($kinds -join ',')"
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;if((Q $r '--player-states').stance-eq1){I $r B 180;Start-Sleep -Milliseconds 650};C $r 'setviewpos -33.5 -153 68.1 0 0';if($Unarmed-and$r-eq$Source-and!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2200};C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 16|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;cases=$cases;scope='Native controls at unmodified Dunwall geometry. Read-only high-mantle scene/state research, no forced states or pose playback.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"OBSERVED_HIGH_MANTLE $($rows.Count) samples"
