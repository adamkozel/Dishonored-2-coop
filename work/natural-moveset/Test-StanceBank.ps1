param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='stance-bank-v1-test')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
function C($r,$text){& "$lab/Send-D2Command.ps1" -Role $r -Command $text -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -DurationMs $ms -Button $button|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function M($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.movement.json" -Raw|ConvertFrom-Json}
function Stance($r,$value){if((B $r).localPlayerStance-ne$value){I $r 0 0 180 B;Start-Sleep -Milliseconds 1100};if((B $r).localPlayerStance-ne$value){throw 'Native stance did not change'}}
$cases=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new();$failure=$null
try {
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong test process'}
  I $r;C $r 'coop_review_clip off';C $r 'coop_authoring_proof off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_mixamo_crouch off';C $r 'coop_probe_body off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat';Stance $r 0
 }
 foreach($src in 'host','client') {
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
  C $src 'setviewpos -34.3 -154.8 68.1 0 0';C $dst 'setviewpos -29 -151.5 68.1 214 11';Start-Sleep -Milliseconds 500
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C $dst 'coop_movement_bank on';Start-Sleep -Milliseconds 800
  $initial=M $dst;if(!$initial.active-or$initial.version-lt3){throw 'Standing/crouch bank did not arm'}
  $armed=[Diagnostics.Stopwatch]::StartNew()
  foreach($stance in 1,0,1,0){
   $transition=[Diagnostics.Stopwatch]::StartNew();if((B $src).localPlayerStance-ne$stance){I $src 0 0 180 B}
   do{
    $s=M $dst;$b=B $src;$t=B $dst
    $samples.Add([pscustomobject]@{source=$src;case='stance-transition';stance=$stance;time=$transition.ElapsedMilliseconds;nativeStance=$b.localPlayerStance;remoteStance=$t.remotePlayerStance;movement=$s})
    $done=$b.localPlayerStance-eq$stance-and[Math]::Abs($s.stanceMix-$stance)-le.01-and$s.weight-ge.95
    if(!$done){Start-Sleep -Milliseconds 40}
   }while(!$done-and$transition.ElapsedMilliseconds-lt2500)
   if(!$done){throw "Full stance transition failed $src stance=$stance"}
   $cases.Add([pscustomobject]@{source=$src;case='complete stance transition';stance=$stance;mix=$s.stanceMix;durationMs=$transition.ElapsedMilliseconds})
   Start-Sleep -Milliseconds 250
  }
  foreach($stance in 0,1){
   Stance $src $stance
   for($d=0;$d-lt8;$d++){
    I $src;C $src 'setviewpos -34.3 -154.8 68.1 0 0';Start-Sleep -Milliseconds 450
    $a=$d*[Math]::PI/4;$speed=if($stance){19000}else{14000};$x=[int](-[Math]::Sin($a)*$speed);$y=[int]([Math]::Cos($a)*$speed)
    $before=M $dst;I $src $x $y 1000;$w=[Diagnostics.Stopwatch]::StartNew();$peak=0;$last=$before
    while($w.ElapsedMilliseconds-lt1650){
     $m=M $dst;$b=B $src;$t=B $dst;$peak=[Math]::Max($peak,$m.speed);$last=$m
     $samples.Add([pscustomobject]@{source=$src;stance=$stance;direction=$d;time=$armed.ElapsedMilliseconds;nativeStance=$b.localPlayerStance;nativeMotion=$b.localPlayerMotion;sourcePosition=$b.localPosition;targetPosition=$t.probeRenderPosition;fresh=$t.remoteFresh;movement=$m});Start-Sleep -Milliseconds 40
    }
    I $src;$phase=$last.phase;Start-Sleep -Milliseconds 220;$settled=M $dst
    $drift=[Math]::Abs($settled.phase-$phase);$drift=[Math]::Min($drift,1-$drift)
    if(!$settled.active-or$peak-lt.1-or$settled.applied-$before.applied-lt15-or$drift-gt.0002){throw "Travel/stop failure $src stance=$stance direction=$d"}
    $cases.Add([pscustomobject]@{source=$src;case='direction and stopped cadence';stance=$stance;direction=$d;peakSpeed=$peak;idleDrift=$drift;applied=$settled.applied-$before.applied})
   }
  }
  Stance $src 0
  while($armed.ElapsedMilliseconds-lt65000){if(!(M $dst).active-or!(B $dst).remoteFresh){throw 'Continuous playback retired while healthy'};Start-Sleep -Milliseconds 250}
  $final=M $dst;if($final.applied-le$initial.applied+100){throw 'No sustained native publication'}
  $cases.Add([pscustomobject]@{source=$src;case='continuous stream exceeds old 15s/60s probe limits';durationMs=$armed.ElapsedMilliseconds;active=$final.active;applied=$final.applied})
  C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';Stance $r 0}catch{}}
 $samples|ConvertTo-Json -Depth 9|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;cases=@($cases);dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;scope='Both roles: network-controlled standing/crouched eight-direction movement, distance cadence, full-body stance blending and continuous publication. Numerical checks do not establish visual acceptance.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"STANCE_BANK_PASSED $($cases.Count) checks"
