$ErrorActionPreference='Stop'
$lab=Join-Path $PWD 'outputs/d2-coop-lab'
$probe=Join-Path $lab 'bin/d2_runtime_probe.exe'
function Cmd($role,$command){& "$lab/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
foreach($r in 'host','client'){
 foreach($command in 'coop_checkpoint_sync off','coop_shadow_stream off','coop_shadow_action off','coop_shadow_loco off','coop_probe_arm off','coop_probe_body off','coop_probe_follow exact','coop_probe_locomotion on'){Cmd $r $command}
}
Cmd host 'setviewpos -33 -153 68.1 90 0'
Cmd client 'setviewpos -33 -148 68.1 270 0'
Start-Sleep -Seconds 3
$rows=@(foreach($role in 'host','client'){
 $dir="work/lab-instances/$role"
 $launch=Get-Content "$dir/launch.json" -Raw|ConvertFrom-Json
 $process=Get-Process -Id $launch.pid
 if($process.Path-ne$launch.executable){throw 'Unexpected game process'}
 $file=Get-Item "$dir/coop-status.json"
 if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds-gt1.5){throw 'Stale status'}
 $s=Get-Content $file.FullName -Raw|ConvertFrom-Json
 $stream=Get-Content "$dir/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json
 $reset=Get-Content "$dir/coop-status.json.reset.json" -Raw|ConvertFrom-Json
 $health=& $probe $launch.pid --health 1|ConvertFrom-Json
 if($LASTEXITCODE){throw 'Cannot observe player health'}
 $entities=& $probe $launch.pid --entities|ConvertFrom-Json
 if($LASTEXITCODE){throw 'Cannot observe owned entities'}
 $clone=@($entities.entities|Where-Object name -match '^d2coop_probe_')
 $character=if($role-eq'host'){'emily'}else{'corvo'}
 if($clone.Count-ne1-or$clone[0].definitionName-ne"models/characters/def/d2coop/doppelganger_$character.def"){throw 'Wrong avatar'}
 if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState-or!$s.remoteFresh-or$s.connectionState-ne3-or!$s.probeFollowExact-or!$s.probeFollowEnabled-or!$s.locomotionEnabled-or$s.bodyStreamEnabled-or$stream.armed-or$stream.actionActive-or$reset.enabled-or$health.current-ne56){throw 'Fixture cleanup failed'}
 [pscustomobject]@{Role=$role;Pid=$launch.pid;Epoch=$s.localEpoch;Handle=$s.probeActorHandle;Follow=$s.probeFollowEnabled;Exact=$s.probeFollowExact;Fresh=$s.remoteFresh;BodyStream=$s.bodyStreamEnabled;PlayerHealth=$health.current;ShadowStreamArmed=$stream.armed;NativeInterface=$s.nativeInterface;Locomotion=$s.locomotionEnabled;Position=$s.localPosition;Avatar=$clone[0].definitionName;AiHeld=$reset.avatarAiHeld;IdleOwned=$reset.avatarIdleOwned;WindowHandle=$process.MainWindowHandle.ToInt64()}
})
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;Passed=$true;Rows=$rows}|ConvertTo-Json -Depth 6|Set-Content "$lab/research/doppelganger-final-fixture-ADB4.json"
$rows|ConvertTo-Json -Depth 4
