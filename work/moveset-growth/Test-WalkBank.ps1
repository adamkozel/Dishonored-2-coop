param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='walk-test-1',[ValidateSet('host','client','both')][string]$Source='both',[switch]$AwaitRecording)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path -LiteralPath $out){throw 'Take already exists'};New-Item -ItemType Directory -Path $out|Out-Null
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function M($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.movement.json" -Raw|ConvertFrom-Json}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -DurationMs $ms|Out-Null}
function Arm {
 foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
 Start-Sleep -Milliseconds 150
 foreach($r in 'host','client'){C $r 'coop_shadow_stream 15000'}
 Start-Sleep -Milliseconds 500
 foreach($r in 'host','client'){$s=Get-Content "$root/work/lab-instances/$r/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json;if(!$s.ready){throw 'Action stream did not agree'}}
}
$records=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
$directions=@(@('forward',0,11000,1),@('forward-left',-7800,7800,2),@('left',-11000,0,3),@('back-left',-7800,-7800,4),@('back',0,-11000,5),@('back-right',7800,-7800,6),@('right',11000,0,7),@('forward-right',7800,7800,8))
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'}
  I $r;C $r 'coop_movement_bank off';C $r 'coop_authoring_proof off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'
 }
 $roles=if($Source-eq'both'){@('host','client')}else{@($Source)}
 foreach($src in $roles){
  $dst=if($src-eq'host'){'client'}else{'host'}
  C $src 'setviewpos -33 -154.8 68.1 0 0';C $dst 'setviewpos -28 -151.5 68.1 214 11';Start-Sleep -Milliseconds 700
  if($AwaitRecording){$deadline=[DateTime]::UtcNow.AddSeconds(45);while(!(Test-Path "$out/record.ready")){if([DateTime]::UtcNow-gt$deadline){throw 'Recorder not ready'};Start-Sleep -Milliseconds 50}}
  Arm;C $dst 'coop_movement_bank 60000';Start-Sleep -Milliseconds 600
  if(!(M $dst).active-or(M $dst).weight-lt.9){throw 'Walk bank not applying ready pose'}
  for($case=0;$case-lt$directions.Count;$case++){
   $d=$directions[$case];if($case%2-eq0-and$case){Arm}
   I $src;C $src 'setviewpos -33 -154.8 68.1 0 0';Start-Sleep -Milliseconds 500
   $before=B $src;$start=[Environment]::TickCount64
   I $src $d[1] $d[2] 1800
   $watch=[Diagnostics.Stopwatch]::StartNew()
   do{
    $records.Add([pscustomobject]@{source=$src;direction=$d[0];stage='moving';tick=[Environment]::TickCount64;elapsed=$watch.ElapsedMilliseconds;sourceState=(B $src);targetState=(B $dst);movement=(M $dst)})
    Start-Sleep -Milliseconds 55
   }while($watch.ElapsedMilliseconds-lt1800)
   I $src;Start-Sleep -Milliseconds 600;$settled=M $dst;$pos=B $src;Start-Sleep -Milliseconds 350;$held=M $dst
   $records.Add([pscustomobject]@{source=$src;direction=$d[0];stage='stopped';tick=[Environment]::TickCount64;sourceState=$pos;targetState=(B $dst);movement=$held})
   $moving=@($records|Where-Object {$_.source-eq$src-and$_.direction-eq$d[0]-and$_.stage-eq'moving'-and$_.elapsed-gt400})
   if(!@($moving|Where-Object {$_.movement.weight-gt.9-and$_.movement.speed-gt.1-and$_.movement.applied-gt$settled.applied-2000}).Count){throw "No authored movement $src $($d[0])"}
   $travel=[Math]::Sqrt([Math]::Pow($pos.localPosition[0]-$before.localPosition[0],2)+[Math]::Pow($pos.localPosition[1]-$before.localPosition[1],2))
   if($travel-lt.1){throw "Native controller failed to move $src $($d[0])"}
   if([Math]::Abs($held.phase-$settled.phase)-gt.0001){throw 'Walking phase advanced after rest'}
   if(!$held.active){throw 'Bank unexpectedly retired'}
   $checks.Add("$src $($d[0]): native movement, authored pose publication and no gait advance at rest; travel=$travel")
  }
  C $dst 'coop_movement_bank off';Start-Sleep -Milliseconds 300;$a=M $dst;Start-Sleep -Milliseconds 350;$b=M $dst
  if($a.active-or$b.applied-ne$a.applied){throw 'Bank writes continued after cancellation'};$checks.Add("$dst cancellation stops authored pose writes")
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'}catch{}}
 $records|ConvertTo-Json -Depth 8|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=@($checks);dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;at=[DateTimeOffset]::Now.ToString('o');scope='Experimental eight-direction walking only. Actual native inputs and native Steam root stream; separate generated pose banks. No running/sprinting, crouch, traversal, combat replacement or full moveset claim.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count;Records=$records.Count}
if($failure){throw $failure}
