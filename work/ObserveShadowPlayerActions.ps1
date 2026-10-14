$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;Set-Location $root
$probe='outputs/d2-coop-lab/bin/d2_runtime_probe.exe';$rows=[Collections.Generic.List[object]]::new();$pids=@{}
foreach($role in 'host','client'){$l=Get-Content "work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Wrong process'};$pids[$role]=$l.pid}
function Native($r,$flag,$h=''){$args=@($pids[$r],$flag);if($h){$args+=$h};for($n=0;$n-lt6;$n++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 10};throw "Unavailable $r $flag"}
function Input($r,$b,$ms=180){& outputs/d2-coop-lab/Send-D2Input.ps1 -Role $r -Button $b -DurationMs $ms|Out-Null}
function Sample($r,$stage){$target=if($r-eq'host'){'client'}else{'host'};$s=Native $r '--player-states';$st=Get-Content "work/lab-instances/$target/coop-status.json" -Raw|ConvertFrom-Json;if($st.localGameLogicState-ne1-or!$st.probeFollowEnabled-or!$st.remoteFresh){throw 'Invalid target scope'}
 $a=Native $target '--animation' $st.probeActorHandle;$b=Native $target '--bones' $st.probeActorHandle
 $rows.Add([pscustomobject]@{Role=$r;Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Player=$s;ReceivedMotion=$st.remotePlayerMotion;ReceivedMotionValid=$st.remotePlayerMotionValid;ReceivedStance=$st.remotePlayerStance;ReceivedStanceValid=$st.remotePlayerStanceValid;Clone=$b;Controller=$a.locomotionNodes})
}
try{foreach($r in 'host','client'){
 Input $r Neutral 100;Start-Sleep -Milliseconds 400;Sample $r Idle
 & outputs/d2-coop-lab/Send-D2Input.ps1 -Role $r -RightTrigger 255 -DurationMs 220|Out-Null
 for($i=0;$i-lt24;$i++){Sample $r Attack;Start-Sleep -Milliseconds 25}
 Input $r Neutral 100;Start-Sleep -Milliseconds 500
 Input $r RightShoulder 1300;for($i=0;$i-lt20;$i++){Sample $r Block;Start-Sleep -Milliseconds 25}
 Input $r Neutral 100;Start-Sleep -Milliseconds 500
 Input $r B;Start-Sleep -Milliseconds 300;for($i=0;$i-lt8;$i++){Sample $r Crouch;Start-Sleep -Milliseconds 25}
 Input $r B;Start-Sleep -Milliseconds 500
 Input $r A;for($i=0;$i-lt20;$i++){Sample $r Jump;Start-Sleep -Milliseconds 25}
 Input $r Neutral 100;Start-Sleep -Milliseconds 1000;Sample $r Settled
 if((Native $r '--health' 1).current-ne56){throw 'Unexpected local health change'}
}}finally{
 foreach($r in 'host','client'){Input $r Neutral 100}
 [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Dll=(Get-FileHash outputs/d2-coop-lab/bin/dinput8.dll).Hash;Scope='Real native controller actions; source player-state observations and existing Steam motion/stance plus target bone/controller samples. No new action network fields or action mapping implemented.';Samples=$rows.ToArray()}|ConvertTo-Json -Depth 14|Set-Content outputs/d2-coop-lab/research/doppelganger-player-actions-observation.json
}
$rows|Group-Object Role,Stage|ForEach-Object {[pscustomobject]@{Group=$_.Name;States=(($_.Group.Player.active|ForEach-Object {"$($_.id):$($_.type)"}|Sort-Object -Unique)-join ', ')}}
