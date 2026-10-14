$ErrorActionPreference='Stop'
$lab=Join-Path (Split-Path $PSScriptRoot -Parent) 'outputs/d2-coop-lab'
$rows=[Collections.Generic.List[object]]::new()
$launch=Get-Content "$PSScriptRoot/lab-instances/host/launch.json" -Raw|ConvertFrom-Json
function State {Get-Content "$PSScriptRoot/lab-instances/host/coop-status.json" -Raw|ConvertFrom-Json}
$initial=State
try {
    foreach($role in 'host','client'){& "$lab/Send-D2Input.ps1" -Role $role -DurationMs 100|Out-Null}
    & "$lab/Send-D2Command.ps1" -Role host -Command 'coop_probe_locomotion off'|Out-Null
    Start-Sleep -Milliseconds 3000
    $before=State
    if($before.locomotionEnabled -or $before.localGameLogicState -ne 1 -or !$before.probeFollowEnabled){throw 'Idle control not ready.'}
    $until=[DateTime]::UtcNow.AddSeconds(30)
    while([DateTime]::UtcNow -lt $until){
        $s=State
        if($s.probeActorHandle -ne $initial.probeActorHandle -or $s.locomotionApplied -ne $before.locomotionApplied){throw 'Idle control changed.'}
        $raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid --animation $s.probeActorHandle
        if($LASTEXITCODE){throw 'Animation read failed.'};$a=$raw|ConvertFrom-Json
        $raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid --bones $s.probeActorHandle
        if($LASTEXITCODE){throw 'Bones read failed.'};$b=$raw|ConvertFrom-Json
        $rows.Add([pscustomobject]@{At=[DateTimeOffset]::Now.ToString('o');Animation=$a;Bones=$b;Status=$s})
        Start-Sleep -Milliseconds 100
    }
}finally {
    & "$lab/Send-D2Command.ps1" -Role host -Command 'coop_probe_locomotion on'|Out-Null
    [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;Scope='Stationary Emily counterpart, owned AI decision pause, locomotion adapter disabled, native animation untouched.';Samples=$rows.ToArray()}|ConvertTo-Json -Depth 18|Set-Content "$PSScriptRoot/idle-control-D117.json"
}
$stable=@($rows|Where-Object {$_.Bones.stable})
function Span($points){$max=0.0;foreach($a in $points){foreach($b in $points){$d=0.0;for($i=0;$i -lt 3;$i++){$d+=($a.P[$i]-$b.P[$i])*($a.P[$i]-$b.P[$i])};$max=[Math]::Max($max,[Math]::Sqrt($d))}};$max}
$feet=@(foreach($joint in 21,25){[pscustomobject]@{Joint=$joint;Span=(Span @($stable|ForEach-Object {[pscustomobject]@{P=($_.Bones.bodies|Where-Object joint -eq $joint).localPosition}}))}})
[pscustomobject]@{Samples=$rows.Count;Stable=$stable.Count;Applied=$before.locomotionApplied;RootSpan=(Span @($stable|ForEach-Object {[pscustomobject]@{P=$_.Bones.root}}));Feet=$feet;NonzeroOutput=@($rows|Where-Object {($_.Animation.modernOutputWords[0..2]|Where-Object {$_ -ne 0}).Count}).Count}|ConvertTo-Json -Depth 6
