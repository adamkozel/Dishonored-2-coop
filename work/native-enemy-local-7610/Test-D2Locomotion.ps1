[CmdletBinding()]
param([ValidateSet('both','host','client')][string]$Target='both')
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe'
$samples=[Collections.Generic.List[object]]::new()
$results=[Collections.Generic.List[object]]::new()
$failure=$null
function Read-State([string]$role) {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected game process.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale game state.'}
    $s=Get-Content $file.FullName -Raw|ConvertFrom-Json
    if($s.localGameLogicState -ne 1 -or !$s.probeFollowEnabled -or !$s.remoteFresh -or $s.connectionState -ne 3){throw 'Connected owned gameplay avatars required.'}
    if($s.localMap -ne 'campaign/dunwall/escape/tower/dunwall_escape_tower_p'){throw 'This bounded movement route requires the opening tower map.'}
    return [pscustomobject]@{Launch=$launch;State=$s}
}
function Sample-Stage([string]$role,[string]$stage,[int]$count=18) {
    for($i=0;$i -lt $count;$i++) {
        $state=Read-State $role
        $a=& $probe $state.Launch.pid --animation $state.State.probeActorHandle
        if($LASTEXITCODE){throw "Native animation read failed: $LASTEXITCODE"}
        $animation=$a|ConvertFrom-Json
        $expectedModel=if($role -eq 'host'){'models/characters/female/emily/emily_head.md6'}else{'models/characters/tall/str/corvo/t_str_corvo_head.md6'}
        if($animation.locomotionNodes.Count -ne 1 -or $animation.locomotionNodes[0].model -ne $expectedModel){throw 'Unexpected native avatar model or locomotion node.'}
        $b=& $probe $state.Launch.pid --bones $state.State.probeActorHandle
        $boneExit=$LASTEXITCODE
        $bones=if($boneExit -eq 0){$b|ConvertFrom-Json}else{$null}
        if($bones -and $bones.bodies.Count -ne 17){throw 'Expected 17 mapped native avatar bodies.'}
        $samples.Add([pscustomobject]@{Role=$role;Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Animation=$animation;Bones=$bones;BoneProbeExit=$boneExit;Status=$state.State})
        Start-Sleep -Milliseconds 70
    }
}
function Distance($a,$b) { $sum=0.0;for($i=0;$i -lt $a.Count;$i++){$sum+=[Math]::Pow($a[$i]-$b[$i],2)};[Math]::Sqrt($sum) }
function Span($vectors) { $maximum=0.0;foreach($a in $vectors){foreach($b in $vectors){$maximum=[Math]::Max($maximum,(Distance $a.Vector $b.Vector))}};$maximum }
function Stage-Metrics([string]$role,[string]$stage) {
    $all=@($samples|Where-Object {$_.Role -eq $role -and $_.Stage -eq $stage})
    $stable=@($all|Where-Object {$null -ne $_.Bones -and $_.Bones.stable})
    $feet=@(foreach($joint in 21,25){
        $vectors=@(foreach($s in $stable){$body=@($s.Bones.bodies|Where-Object joint -eq $joint);if($body.Count -eq 1){[pscustomobject]@{Vector=$body[0].localPosition}}})
        [pscustomobject]@{Joint=$joint;Span=(Span $vectors)}
    })
    $maximumBodyError=0.0
    foreach($s in $stable){foreach($b in $s.Bones.bodies){$maximumBodyError=[Math]::Max($maximumBodyError,(Distance $b.position $b.expectedPosition))}}
    $maximumOutputSpeed=0.0
    foreach($s in $all){$v=@(foreach($word in $s.Animation.modernOutputWords[0..2]){[BitConverter]::ToSingle([BitConverter]::GetBytes([uint32]$word),0)});$maximumOutputSpeed=[Math]::Max($maximumOutputSpeed,(Distance $v @(0,0,0)))}
    [pscustomobject]@{Stage=$stage;Samples=$all.Count;StableSamples=$stable.Count;UnavailableBones=@($all|Where-Object BoneProbeExit -ne 0).Count;
        RootSpan=(Span @($stable|ForEach-Object {[pscustomobject]@{Vector=$_.Bones.root[0..2]}}));Feet=$feet;MaxBodyError=$maximumBodyError;
        MaxNativeOutputSpeed=$maximumOutputSpeed;MaxInputSpeed=($all.Status.locomotionSpeed|Measure-Object -Maximum).Maximum;
        NativeRequestMovementSamples=@($all|Where-Object {$n=$_.Animation.locomotionNodes; $n.Count -eq 1 -and (Distance $n[0].requested[0..2] @(0,0,0)) -gt 0.2}).Count}
}
try {
    $targets=if($Target -eq 'both'){@('host','client')}else{@($Target)}
    foreach($role in $targets) {
        $source=if($role -eq 'host'){'client'}else{'host'}
        $initial=(Read-State $role).State
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_locomotion on'|Out-Null
        # The opening room has only about one unit of strafe clearance. Start
        # at its right edge so the measured left pass spans the available room
        # instead of stopping at the wall before both feet have taken a step.
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $source -LeftX 20000 -DurationMs 1800|Out-Null
        Start-Sleep -Milliseconds 1900
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $source -DurationMs 100|Out-Null
        Start-Sleep -Milliseconds 3000
        if(!(Read-State $role).State.locomotionEnabled){throw 'Adapter enable not acknowledged.'}
        Sample-Stage $role Idle
        foreach($direction in -1,1) {
            $stage=if($direction -eq -1){'MovingLeft'}else{'MovingRight'}
            & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $source -LeftX ($direction*20000) -DurationMs 1800|Out-Null
            Sample-Stage $role $stage
            & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $source -DurationMs 100|Out-Null
            Start-Sleep -Milliseconds 3000
        }
        Sample-Stage $role Stopped
        $beforeOff=(Read-State $role).State
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_locomotion off'|Out-Null
        Start-Sleep -Milliseconds 3000
        $off=(Read-State $role).State
        if($off.locomotionEnabled){throw 'Adapter disable not acknowledged.'}
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $source -LeftX -20000 -DurationMs 1800|Out-Null
        Sample-Stage $role Disabled
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $source -DurationMs 100|Out-Null
        $end=(Read-State $role).State
        $metrics=@(foreach($stage in 'Idle','MovingLeft','MovingRight','Stopped','Disabled'){Stage-Metrics $role $stage})
        $failed=[Collections.Generic.List[string]]::new()
        foreach($m in $metrics){
            if($m.StableSamples -lt 12){$failed.Add("$($m.Stage): insufficient stable bone snapshots")}
            if($m.MaxBodyError -gt 0.08){$failed.Add("$($m.Stage): animated body separation exceeds 0.08")}
        }
        foreach($m in @($metrics|Where-Object Stage -in 'MovingLeft','MovingRight')) {
            if($m.RootSpan -lt 0.2 -or $m.MaxInputSpeed -lt 0.2 -or $m.NativeRequestMovementSamples -lt 3 -or $m.MaxNativeOutputSpeed -lt 0.1){$failed.Add("$($m.Stage): insufficient movement/controller response")}
            foreach($foot in $m.Feet){if($foot.Span -lt 0.12){$failed.Add("$($m.Stage): joint $($foot.Joint) has no clear gait excursion")}}
        }
        foreach($m in @($metrics|Where-Object Stage -in 'Idle','Stopped','Disabled')) {
            if($m.MaxNativeOutputSpeed -gt 0.03 -or @($m.Feet|Where-Object Span -gt 0.08).Count){$failed.Add("$($m.Stage): not settled at idle")}
        }
        if(($metrics|Where-Object Stage -eq Disabled).RootSpan -lt 0.2){$failed.Add('Disabled comparison did not move the avatar')}
        if($end.locomotionApplied -ne $off.locomotionApplied){$failed.Add('Adapter continued after disable')}
        if($end.probeActorHandle -ne $initial.probeActorHandle){$failed.Add('Owned actor changed during the test')}
        if($end.locomotionRefused -ne $initial.locomotionRefused -or $end.rejected -ne $initial.rejected){$failed.Add('New adapter or transport refusals')}
        if((Distance $initial.localPosition $end.localPosition) -gt 0.02){$failed.Add('Input moved the other local player')}
        $results.Add([pscustomobject]@{Role=$role;Character=if($role -eq 'host'){'Emily'}else{'Corvo'};Passed=($failed.Count -eq 0);Failures=$failed.ToArray();Metrics=$metrics;AppliedDuringTest=$beforeOff.locomotionApplied-$initial.locomotionApplied;AppliedAfterDisable=$end.locomotionApplied-$off.locomotionApplied})
        # Restore the explicitly tested experimental mode after the comparison.
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_locomotion on'|Out-Null
    }
    if(@($results|Where-Object {!$_.Passed}).Count){throw 'One or more native locomotion assertions failed.'}
}catch{$failure=$_.Exception.Message}finally{
    foreach($role in 'host','client'){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null}
}
[ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;
    ProbeSha256=(Get-FileHash $probe).Hash;Results=$results.ToArray();Samples=$samples.ToArray();
    Scope='Independent native locomotion node and bone/body observations, with enabled and disabled movement comparisons, for owned story avatars on the opening tower map. Normal speed. Does not verify player animation parity, jumping, powers, combat, visual quality, or shared world authority.'}|
    ConvertTo-Json -Depth 16|Set-Content (Join-Path $PSScriptRoot 'research/locomotion-verification.json')
$results|Select-Object Role,Character,Passed,Failures,AppliedDuringTest,AppliedAfterDisable
if($failure){throw $failure}
