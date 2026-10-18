param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='high-mantle-probe-v1327a',[string[]]$Roles=@('client','host'),[switch]$Crouched)
# 1327 (#11): why does the source not classify Emily's high mantle (take 1326a: client source, read stage 6, 12 holds)
# while Corvo's is classified? Each role mantles onto the safe-room canopy (Test-LowMantle -High approach) and the
# native traversal reader research (stage, refusing line, kind, next, asset, duration, bindings, arms) is polled
# about every 15 ms. Movement only: the source is put back on the lab spot (and stood up) after each trial.
$Roles=@($Roles|ForEach-Object {$_-split','}|Where-Object {$_});$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$pids=@{}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function Q($r){for($i=0;$i-lt8;$i++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] '--player-states';if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native read failed $r"}
$rows=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$pids[$r]=$l.pid}
 foreach($r in $Roles){
  C $r 'setviewpos -30.7 -153.6 68.1 270 0';Start-Sleep -Milliseconds 900
  if($Crouched){& "$lab/Send-D2Input.ps1" -Role $r -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 850}
  & "$lab/Send-D2Input.ps1" -Role $r -Button A -DurationMs 1300 -LeftY 22000|Out-Null;$w=[Diagnostics.Stopwatch]::StartNew()
  while($w.ElapsedMilliseconds-lt2500){$s=Q $r
   $rows.Add([ordered]@{role=$r;ms=$w.ElapsedMilliseconds;active=@($s.active|ForEach-Object {$_.id});stance=$s.stance;position=$s.position;valid=$s.traversalSceneValid;stage=$s.traversalReaderStage;research=$s.traversalResearch})}
  C $r 'setviewpos -33 -153 68.1 90 0';Start-Sleep -Milliseconds 600;if((Q $r).stance-eq1){& "$lab/Send-D2Input.ps1" -Role $r -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 700}
 }
}finally{
 $rows|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
foreach($r in $Roles){$m=@($rows|Where-Object {$_.role-eq$r-and@($_.active)-contains8})
 "$r mantle samples $($m.Count), valid $(@($m|Where-Object valid).Count); lines $(@($m|ForEach-Object {$_.research.line})|Sort-Object -Unique) next $(@($m|ForEach-Object {$_.research.next})|Sort-Object -Unique) kind $(@($m|ForEach-Object {$_.research.kind})|Sort-Object -Unique) duration $(@($m|ForEach-Object {$_.research.duration})|Sort-Object -Unique) asset $(@($m|ForEach-Object {$_.research.asset})|Sort-Object -Unique) expected $(@($m|ForEach-Object {$_.research.expected})|Sort-Object -Unique) active $(@($m|ForEach-Object {$_.active-join'+'})|Sort-Object -Unique)"}
