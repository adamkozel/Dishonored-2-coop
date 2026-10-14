[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe'
$observations=[Collections.Generic.List[object]]::new();$summary=@();$failure=$null
function Read-Bones([string]$role,[string]$stage) {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected game process.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale game state.'}
    $status=Get-Content $file.FullName -Raw|ConvertFrom-Json
    if($status.localGameLogicState -ne 1 -or !$status.probeFollowEnabled -or !$status.remoteFresh){throw 'Both avatars must be following connected gameplay poses.'}
    $out=& $probe $launch.pid --bones $status.probeActorHandle
    if($LASTEXITCODE){throw "Native bone inspection failed ($LASTEXITCODE)."}
    $native=$out|ConvertFrom-Json
    if($native.bodies.Count -ne 17){throw 'Expected 17 mapped native bodies.'}
    $errors=@(foreach($body in $native.bodies){
        $sum=0.0;for($i=0;$i -lt 3;$i++){$sum+=[Math]::Pow($body.position[$i]-$body.expectedPosition[$i],2)}
        [pscustomobject]@{Id=$body.id;Joint=$body.joint;Error=[Math]::Sqrt($sum)}
    })
    $sample=[pscustomobject]@{Role=$role;Stage=$stage;Native=$native;Errors=$errors;MaxError=($errors.Error|Measure-Object -Maximum).Maximum;At=[DateTimeOffset]::Now.ToString('o')}
    $observations.Add($sample);return $sample
}
try {
    foreach($source in 'host','client') {
        $target=if($source -eq 'host'){'client'}else{'host'}
        $before=Read-Bones $target Idle
        for($i=0;$i -lt 11;$i++){Start-Sleep -Milliseconds 80;Read-Bones $target Idle|Out-Null}
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $source -LeftX 16000 -DurationMs 1600|Out-Null
        for($i=0;$i -lt 16;$i++){Start-Sleep -Milliseconds 80;Read-Bones $target Translation|Out-Null}
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $source -DurationMs 100|Out-Null
        Start-Sleep -Milliseconds 300
        $translated=Read-Bones $target Translated
        $distance=0.0;for($i=0;$i -lt 3;$i++){$distance+=[Math]::Pow($translated.Native.root[$i]-$before.Native.root[$i],2)};$distance=[Math]::Sqrt($distance)
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $source -RightX 12000 -DurationMs 500|Out-Null
        for($i=0;$i -lt 12;$i++){Start-Sleep -Milliseconds 80;Read-Bones $target Rotation|Out-Null}
        $end=Read-Bones $target Final
        $turn=0.0;for($i=3;$i -lt 12;$i++){$turn+=[Math]::Pow($end.Native.root[$i]-$translated.Native.root[$i],2)};$turn=[Math]::Sqrt($turn)
        $samples=@($observations|Where-Object Role -eq $target)
        $coherent=@($samples|Where-Object {$_.Native.stable})
        # External sampling spans native phases; 0.08 game units bounds their
        # observed positional separation. This is separate from hit detection.
        $maximum=($coherent.MaxError|Measure-Object -Maximum).Maximum
        $stagesReady=$true
        foreach($stage in 'Idle','Translation','Rotation'){if(@($coherent|Where-Object Stage -eq $stage).Count -lt 8){$stagesReady=$false}}
        $passed=$stagesReady -and $maximum -le 0.08 -and $distance -gt 0.2 -and $turn -gt 0.05
        $summary+=[pscustomobject]@{Source=$source;AvatarRole=$target;Samples=$samples.Count;CoherentSamples=$coherent.Count;BodyObservations=$coherent.Count*17;MaxAnimatedBodyError=$maximum;RootMovement=$distance;RootRotation=$turn;Passed=$passed}
        if(!$passed){throw "Animated body alignment failed for $target."}
    }
}catch{$failure=$_.Exception.Message}finally{
    foreach($role in 'host','client'){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null}
}
[ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;ProbeSha256=(Get-FileHash $probe).Hash;Tolerance=0.08;Summary=$summary;Samples=$observations.ToArray();Scope='Independent reconstruction of native current animated joint positions and body binding offsets versus actual Havok bodies during idle, Steam-driven translation and rotation. Snapshot races are reported and excluded. Does not establish hit detection, replicated animation or collision authority; the historical rigid-body test is preserved.'}|ConvertTo-Json -Depth 12|Set-Content (Join-Path $PSScriptRoot 'research/animated-body-verification.json')
if($failure){throw $failure}
$summary
