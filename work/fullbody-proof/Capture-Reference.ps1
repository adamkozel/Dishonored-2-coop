param([string]$Take='clean-take')
$ErrorActionPreference='Stop'
if($Take -notmatch '^[a-z0-9-]+$'){throw 'Invalid take name'}
$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take already exists'}
[void](New-Item -ItemType Directory -Path $out)
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$source=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json
$target=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json
foreach($l in $source,$target){if(!(Get-Process -Id $l.pid).Path.Equals($l.executable)-or!$l.assetExperiment){throw 'Wrong game process'}}
$status=Get-Content "$root/work/lab-instances/client/coop-status.json" -Raw|ConvertFrom-Json
if(!$status.remoteFresh-or!$status.probeFollowEnabled-or$status.localGameLogicState-ne1){throw 'Isolated Dunwall not ready'}
$events=[Collections.Generic.List[object]]::new()
function Cmd($role,$cmd){$events.Add([pscustomobject]@{tick=[Environment]::TickCount64;role=$role;command=$cmd});& "$lab/Send-D2Command.ps1" -Role $role -Command $cmd -WaitConsumed|Out-Null}
function Input($button,$y,$duration){$events.Add([pscustomobject]@{tick=[Environment]::TickCount64;button=$button;y=$y;duration=$duration});& "$lab/Send-D2Input.ps1" -Role host -Button $button -LeftY $y -DurationMs $duration|Out-Null}
$exe=Join-Path $PSScriptRoot 'build/Release/d2_capture_pose.exe';$jobs=@()
foreach($role in 'host','client'){Cmd $role 'coop_shadow_action off';Cmd $role 'coop_probe_body off';Cmd $role 'coop_shadow_stream off';Cmd $role 'coop_probe_follow exact';Cmd $role 'coop_probe_locomotion on';Cmd $role 'coop_shadow_loco combat'}
Cmd host 'setviewpos -33 -153 68.1 270 0'
Cmd client 'setviewpos -36.2 -154.8 68.1 30 8'
Start-Sleep -Milliseconds 350
foreach($r in @(@('body',$target.pid,$status.probeActorHandle,'actor'),@('fp-body',$source.pid,1,'actor'),@('fp-arm',$source.pid,1,'arm'))){
 $args=@($r[1],$r[2],9,('"'+(Join-Path $out ($r[0]+'-reference.json'))+'"'),$r[3])
 $jobs+=Start-Process -FilePath $exe -ArgumentList $args -WindowStyle Hidden -PassThru -RedirectStandardOutput "$out/$($r[0])-capture.log" -RedirectStandardError "$out/$($r[0])-capture.err"
}
try{
 Start-Sleep -Milliseconds 800
 Input Neutral 15000 1100;Start-Sleep -Milliseconds 1500
 Input RightShoulder 0 1200;Cmd client 'coop_shadow_action parry';Start-Sleep -Milliseconds 1250
 Cmd client 'coop_shadow_action off';Cmd client 'coop_shadow_action recover';Start-Sleep -Milliseconds 1500
 Cmd client 'coop_shadow_action off';Start-Sleep -Milliseconds 800
 Input Neutral 0 100
 foreach($job in $jobs){$job.WaitForExit(15000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){throw 'Capture failed'}}
}finally{Input Neutral 0 100;Cmd client 'coop_shadow_action off';$events|ConvertTo-Json -Depth 5|Set-Content "$out/reference-events.json"}
Get-Content "$out/*-capture.log"
