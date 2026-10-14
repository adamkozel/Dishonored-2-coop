param([ValidateSet('host','client')][string]$Target='host',[string]$Label='first',[int]$Count=20,[int]$MoveX=20000,[switch]$RequireEnabled)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
$source=if($Target -eq 'host'){'client'}else{'host'}
$launch=Get-Content (Join-Path $root "work/lab-instances/$Target/launch.json") -Raw|ConvertFrom-Json
if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected game process.'}
$statusPath=Join-Path $root "work/lab-instances/$Target/coop-status.json"
$before=Get-Content $statusPath -Raw|ConvertFrom-Json
if(!$before.probeFollowEnabled -or !$before.remoteFresh -or $before.localGameLogicState -ne 1){throw 'Connected owned gameplay avatar required.'}
if($RequireEnabled -and !$before.locomotionEnabled){throw 'The animation adapter was not enabled.'}
$samples=[Collections.Generic.List[object]]::new()
$failure=$null
try {
    foreach($stage in 'Idle','Moving','Stopped') {
        if($stage -eq 'Moving'){& (Join-Path $lab 'Send-D2Input.ps1') -Role $source -LeftX $MoveX -DurationMs 4000|Out-Null}
        if($stage -eq 'Stopped'){& (Join-Path $lab 'Send-D2Input.ps1') -Role $source -DurationMs 100|Out-Null;Start-Sleep -Milliseconds 800}
        for($i=0;$i -lt $Count;$i++) {
            $animation=& (Join-Path $lab 'bin/d2_runtime_probe.exe') $launch.pid --animation $before.probeActorHandle
            if($LASTEXITCODE){throw "Native animation probe failed: $LASTEXITCODE"}
            $bones=& (Join-Path $lab 'bin/d2_runtime_probe.exe') $launch.pid --bones $before.probeActorHandle
            $boneExit=$LASTEXITCODE
            $nativeBones=if($boneExit -eq 0){$bones|ConvertFrom-Json}else{$null}
            $samples.Add([pscustomobject]@{Stage=$stage;Time=[DateTimeOffset]::Now.ToString('o');Animation=($animation|ConvertFrom-Json);Bones=$nativeBones;BoneProbeExit=$boneExit;Status=(Get-Content $statusPath -Raw|ConvertFrom-Json)})
            Start-Sleep -Milliseconds 70
        }
    }
}catch{$failure=$_.Exception.Message}finally {& (Join-Path $lab 'Send-D2Input.ps1') -Role $source -DurationMs 100|Out-Null}
$path=Join-Path $root "work/locomotion-$Label-$Target.json"
[ordered]@{Failure=$failure;DllSha256=(Get-FileHash (Join-Path $lab 'bin/dinput8.dll')).Hash;Target=$Target;Samples=$samples.ToArray()}|ConvertTo-Json -Depth 14|Set-Content $path
if($failure){throw $failure}
$samples|Group-Object Stage|ForEach-Object {[pscustomobject]@{Stage=$_.Name;Samples=$_.Count;MaximumInputSpeed=($_.Group.Status.locomotionSpeed|Measure-Object -Maximum).Maximum;Applied=($_.Group.Status.locomotionApplied|Measure-Object -Maximum).Maximum;Refused=($_.Group.Status.locomotionRefused|Measure-Object -Maximum).Maximum}}
