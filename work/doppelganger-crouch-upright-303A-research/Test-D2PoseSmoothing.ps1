[CmdletBinding()]
param()
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$checks=[Collections.Generic.List[object]]::new();$failure=$null
function Cmd($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function State($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function Distance($a,$b){$v=0.;for($i=0;$i-lt3;$i++){$v+=[Math]::Pow($a[$i]-$b[$i],2)};[Math]::Sqrt($v)}
function Metrics($trace){
 $changes=[Collections.Generic.List[object]]::new();$maxError=0.;$nativeError=0.;$renderError=0.
 foreach($r in $trace.samples){
  $nativeError=[Math]::Max($nativeError,(Distance $r.native $r.target));$renderError=[Math]::Max($renderError,(Distance $r.render $r.target))
  if(!$changes.Count-or(Distance $r.render $changes[$changes.Count-1].render)-gt0.0001){$changes.Add($r)}
 }
 # Exclude the initial idle-to-motion interval; retain every gap within motion.
 $gaps=@(for($i=2;$i-lt$changes.Count;$i++){$changes[$i].t-$changes[$i-1].t})
 $sorted=@($gaps|Sort-Object)
 $duration=if($changes.Count-gt2){$changes[$changes.Count-1].t-$changes[1].t}else{0}
 $rate=if($duration-gt0){1000.*($changes.Count-2)/$duration}else{0}
 $travel=0.;for($i=1;$i-lt$changes.Count;$i++){$travel+=Distance $changes[$i].render $changes[$i-1].render}
 [pscustomobject]@{Samples=$trace.samples.Count;MovingUpdates=[Math]::Max(0,$changes.Count-1);MovingHz=$rate;Gap90Ms=$(if($sorted.Count){$sorted[[Math]::Min($sorted.Count-1,[int][Math]::Floor($sorted.Count*.9))]}else{0});Travel=$travel;NativeTargetError=$nativeError;RenderTargetError=$renderError;FinalRemoteError=(Distance $trace.samples[-1].render $trace.samples[-1].remote)}
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'}
  foreach($c in 'coop_shadow_stream off','coop_shadow_action off','coop_probe_follow exact','coop_probe_locomotion on'){Cmd $r $c}
 }
 foreach($source in 'host','client'){
  $target=if($source-eq'host'){'client'}else{'host'}
  foreach($axis in 'Horizontal','Diagonal','Vertical'){
   $pair=@{}
   foreach($mode in 'off','on'){
    Cmd $source 'setviewpos -33 -153 68.1 0 0';Cmd $target 'setviewpos -28.0921 -153.962 68.1 180 0'
    Cmd $target "coop_probe_smoothing $mode";Start-Sleep -Milliseconds 1200
    $before=State $target
    if(!$before.remoteFresh-or!$before.probeFollowExact-or[Math]::Abs($before.remotePosition[2]-66.4036484)-gt.02){throw 'Flat-floor scope missing'}
    $tracePath="$root/work/lab-instances/$target/coop-status.json.pose-cadence.json";$started=[DateTime]::UtcNow
    Cmd $target 'coop_pose_trace 3000';Start-Sleep -Milliseconds 200
    switch($axis){
     Horizontal{& "$PSScriptRoot/Send-D2Input.ps1" -Role $source -LeftX 11000 -DurationMs 1000|Out-Null}
     Diagonal{& "$PSScriptRoot/Send-D2Input.ps1" -Role $source -LeftX 8000 -LeftY 8000 -DurationMs 1000|Out-Null}
     Vertical{& "$PSScriptRoot/Send-D2Input.ps1" -Role $source -Button A -DurationMs 220|Out-Null}
    }
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do{Start-Sleep -Milliseconds 100;$done=(Test-Path $tracePath)-and(Get-Item $tracePath).LastWriteTimeUtc-gt$started}while(!$done-and$watch.ElapsedMilliseconds-lt4500)
    if(!$done){throw 'Native per-frame trace did not finish'}
    $trace=Get-Content $tracePath -Raw|ConvertFrom-Json
    if($trace.smoothing-ne($mode-eq'on')-or$trace.samples.Count-lt90){throw 'Wrong mode or insufficient frame evidence'}
    $name="pose-cadence-$source-$($axis.ToLowerInvariant())-$mode.json"
    Copy-Item -LiteralPath $tracePath -Destination "$PSScriptRoot/research/$name"
    $metrics=Metrics $trace;$after=State $target
    $pair[$mode]=[pscustomobject]@{Mode=$mode;Report=$name;Metrics=$metrics;ObserverDisplacement=(Distance $before.localPosition $after.localPosition)}
   }
   $errors=[Collections.Generic.List[string]]::new()
   if($pair.on.Metrics.MovingHz-lt$pair.off.Metrics.MovingHz*2-or$pair.on.Metrics.Gap90Ms-gt$pair.off.Metrics.Gap90Ms*.65){$errors.Add('Interpolation did not materially improve native/rendered motion cadence')}
   foreach($m in 'off','on'){
    $v=$pair[$m];if($v.Metrics.Travel-lt.25){$errors.Add("Insufficient actual movement: $m")}
    if($v.Metrics.NativeTargetError-gt.001-or$v.Metrics.RenderTargetError-gt.001-or$v.Metrics.FinalRemoteError-gt.01){$errors.Add("Native/rendered transform alignment or settling failed: $m")}
    if($v.ObserverDisplacement-gt.02){$errors.Add("Local observer moved: $m")}
   }
   $checks.Add([pscustomobject]@{Role=$source;Axis=$axis;Passed=(!$errors.Count);Failures=$errors;Off=$pair.off;On=$pair.on})
   if($errors.Count){throw "Pose smoothing failed: $source $axis"}
  }
 }
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$h=& $probe $l.pid --health 1|ConvertFrom-Json;if($LASTEXITCODE-or$h.current-ne56){throw 'Player health changed'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{& "$PSScriptRoot/Send-D2Input.ps1" -Role $r -Button Neutral -DurationMs 100|Out-Null;Cmd $r 'coop_probe_smoothing on'}catch{}}
}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;Dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;Checks=$checks;
 Scope='A/B native and render-transform frame traces under actual horizontal, diagonal and vertical player input. Transport cadence unchanged; 80ms receiver buffer plus QPC evaluation on every native command frame. Teleports >3m and outages/world changes reset history. Not internet jitter, gameplay hitbox authority, or action/root temporal alignment validation.'}|ConvertTo-Json -Depth 12|Set-Content "$PSScriptRoot/research/pose-smoothing-verification.json"
$checks|Select-Object Role,Axis,Passed,@{n='OffHz';e={$_.Off.Metrics.MovingHz}},@{n='OnHz';e={$_.On.Metrics.MovingHz}},@{n='OffGap90';e={$_.Off.Metrics.Gap90Ms}},@{n='OnGap90';e={$_.On.Metrics.Gap90Ms}}
if($failure){throw $failure}
