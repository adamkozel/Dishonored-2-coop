$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$lab=Join-Path $root 'outputs/d2-coop-lab'
$checks=[Collections.Generic.List[string]]::new();$events=[Collections.Generic.List[object]]::new()
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function I($r,$b){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs 180|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function S($r='client'){Get-Content "$root/work/lab-instances/$r/coop-status.json.authoring.json" -Raw|ConvertFrom-Json}
function Ready{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Unexpected game process'}
  if((B $r).localGameLogicState-ne1){throw 'Native gameplay not ready'}
  C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'
 }
 Start-Sleep -Milliseconds 400
}
function Arm{
 C client 'coop_authoring_proof hold_guard';Start-Sleep -Milliseconds 850
 $s=S;if(!$s.active){throw 'Known valid proof not accepted'}
 return $s
}
$clip=Join-Path $root 'work/lab-instances/client/coop-status.json.authoring.bin'
$good=[IO.File]::ReadAllBytes($clip);$failure=$null;$paused=$null
try{
 Ready;C client 'coop_authoring_proof off'
 $a=Arm;C client 'coop_authoring_proof off';Start-Sleep -Milliseconds 350;$b=S
 Start-Sleep -Milliseconds 400;$c=S
 if($b.active-or$c.active-or$b.applied-ne$c.applied){throw 'Cancellation continued writing'}
 $checks.Add('Explicit cancellation retires the clip and stops pose writes');$events.Add(@{stage='cancel';before=$a;after=$b;settled=$c})
 C host 'coop_authoring_proof hold_guard';Start-Sleep -Milliseconds 250
 if((S host).active){throw 'Wrong role accepted Corvo proof'}
 $checks.Add('Host/remote Emily scope refuses the Corvo-only proof')
 $bad=$good.Clone();$bad[0]=0;[IO.File]::WriteAllBytes($clip,$bad)
 C client 'coop_authoring_proof hold_guard';Start-Sleep -Milliseconds 250
 if((S).active){throw 'Invalid clip accepted'}
 [IO.File]::WriteAllBytes($clip,$good)
 $checks.Add('Invalid clip magic is refused; exact working clip restored')
 foreach($r in 'host','client'){
  Ready;$a=Arm;I $r Start;$paused=$r;Start-Sleep -Milliseconds 850;$b=S
  if((B $r).localGameLogicState-ne2-or$b.active){throw 'Native pause failed to cancel proof'}
  C client 'coop_authoring_proof hold_guard';Start-Sleep -Milliseconds 350;$c=S
  if($c.active-or$c.applied-ne$b.applied){throw 'Paused rearm or mutation accepted'}
  I $r A;Start-Sleep -Milliseconds 600
  if((B $r).localGameLogicState-ne1){throw 'Native resume failed'}
  $paused=$null;$d=S
  if($d.active-or$d.applied-ne$c.applied){throw 'Resume replayed retired proof'}
  $checks.Add("$r native pause cancels the clip, refuses rearm, and resume does not replay it")
  $events.Add(@{stage="pause-$r";before=$a;paused=$b;refused=$c;resumed=$d})
 }
 Ready
}catch{$failure=$_.Exception.Message}finally{
 [IO.File]::WriteAllBytes($clip,$good)
 foreach($r in 'host','client'){try{I $r Neutral;C $r 'coop_authoring_proof off'}catch{}}
 if($paused-and(B $paused).localGameLogicState-eq2){I $paused A;Start-Sleep -Milliseconds 500}
}
$report=[ordered]@{passed=(!$failure);failure=$failure;at=[DateTimeOffset]::Now.ToString('o');dllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;clipSha256=(Get-FileHash $clip).Hash;checks=@($checks);events=@($events);scope='One bounded Corvo authoring proof. No checkpoint reload, stale network, combat arbitration or full moveset coverage claim.'}
$report|ConvertTo-Json -Depth 10|Set-Content "$PSScriptRoot/safety-verification.json"
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count}
if($failure){throw $failure}
