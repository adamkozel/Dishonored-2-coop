param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='gaits-corvo-1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Source-eq'host'){'client'}else{'host'}
function State($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function Motion{Get-Content "$root/work/lab-instances/$target/coop-status.json.movement.json" -Raw|ConvertFrom-Json}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($x=0,$y=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $Source -LeftX $x -LeftY $y -DurationMs $ms -Button $button|Out-Null}
$cases=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new();$failure=$null
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne("$root/work/asset-game/Dishonored2.exe".Replace('/','\'))){throw 'Wrong isolated process'}
  C $r 'coop_review_clip off';C $r 'coop_authoring_proof off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'
 }
 foreach($speed in 14000,32767){
  for($d=0;$d-lt8;$d++){
   I;C $target 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
   C $Source 'setviewpos -34.3 -154.8 68.1 0 0';C $target 'setviewpos -29 -151.5 68.1 214 14';Start-Sleep -Milliseconds 500
   foreach($r in 'host','client'){C $r 'coop_shadow_stream 15000'};Start-Sleep -Milliseconds 350;C $target 'coop_movement_bank 60000';Start-Sleep -Milliseconds 300
   $before=State $target;$motion=Motion
   if(!$motion.active-or$motion.gaits-ne2){throw 'Version-2 bank did not arm'}
   $a=$d*[Math]::PI/4;$x=[int](-[Math]::Sin($a)*$speed);$y=[int]([Math]::Cos($a)*$speed)
   I $x $y 950;$watch=[Diagnostics.Stopwatch]::StartNew();$part=[Collections.Generic.List[object]]::new()
   while($watch.ElapsedMilliseconds-lt1450){$row=[pscustomobject]@{direction=$d;input=$speed;ms=$watch.ElapsedMilliseconds;source=(State $Source);target=(State $target);movement=(Motion)};$part.Add($row);$samples.Add($row);Start-Sleep -Milliseconds 30}
   I;$peak=($part.movement.speed|Measure-Object -Maximum).Maximum;$mix=($part.movement.runMix|Measure-Object -Maximum).Maximum
   $applied=($part.movement.applied|Measure-Object -Maximum).Maximum-$motion.applied
   if($applied-lt10-or$peak-lt.15){throw "No useful movement for direction $d input $speed"}
   $settled=Motion;$phase=$settled.phase;Start-Sleep -Milliseconds 200;$stopped=Motion
   if([Math]::Abs($stopped.phase-$phase)-gt.0001){throw 'Feet phase advanced while stationary'}
   $cases.Add([pscustomobject]@{direction=$d;input=$speed;peakSpeed=$peak;peakRunMix=$mix;applied=$applied;idlePhaseDrift=[Math]::Abs($stopped.phase-$phase)})
  }
 }
 if(($cases.peakRunMix|Measure-Object -Maximum).Maximum-lt.8){throw 'Actual running was not exercised'}
 # Action priority is tested separately using observed native action IDs,
 # not inferred from controller button requests (Test-GaitPriority.ps1).
}catch{$failure=$_.Exception.Message}finally{
 I;foreach($r in 'host','client'){try{C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'}catch{}}
 $samples|ConvertTo-Json -Depth 8|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;target=$target;cases=@($cases);block=$block;crouch=$crouch;jump=$jump;dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;bank=(Get-FileHash "$root/work/lab-instances/$target/coop-status.json.movement.bin").Hash;visualAccepted=$false}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"NATIVE_GAITS_PASSED $($cases.Count)"
