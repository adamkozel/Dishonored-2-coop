$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
$probe=Join-Path $PSScriptRoot 'd2_stance_probe.exe'
$rows=[Collections.Generic.List[object]]::new()
foreach($role in 'host','client') {
    $launch=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    $p=Get-Process -Id $launch.pid -ErrorAction Stop
    if($p.Path -ne $launch.executable){throw 'Unexpected lab process.'}
    $state=Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json
    if($state.localGameLogicState -ne 1 -or $state.localPendingGameLogicState){throw 'Native gameplay required.'}
    $native=& $probe $launch.pid --character
    if($LASTEXITCODE){throw 'Character unavailable.'}
    $character=$native|ConvertFrom-Json
    foreach($stage in 'Standing','ButtonB','ButtonBAgain','ButtonA','Settled') {
        $button=switch($stage){'ButtonB'{'B'}'ButtonBAgain'{'B'}'ButtonA'{'A'}default{'Neutral'}}
        & (Join-Path $lab 'Send-D2Input.ps1') -Role $role -Button $button -DurationMs 400|Out-Null
        $watch=[Diagnostics.Stopwatch]::StartNew();$refused=0;$samples=[Collections.Generic.List[object]]::new()
        do {
            $raw=& $probe $launch.pid --player-stance
            if($LASTEXITCODE){++$refused}else{$samples.Add(($raw|ConvertFrom-Json))}
            Start-Sleep -Milliseconds 65
        }while($watch.ElapsedMilliseconds -lt 2000)
        if(!$samples.Count){throw 'No valid stance observations.'}
        $rows.Add([ordered]@{Role=$role;Character=$character.character;Stage=$stage;RefusedReads=$refused;Samples=$samples.ToArray()})
    }
}
$record=[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $lab 'bin/dinput8.dll')).Hash;
    ProbeSha256=(Get-FileHash $probe).Hash;Scope='Read-only player physics samples during ordinary bounded controller input. Raw enum mapping research; no remote stance application.';Observations=$rows.ToArray()}
$record|ConvertTo-Json -Depth 12|Set-Content (Join-Path $lab 'research/player-stance-observations.json')
$rows|ForEach-Object {[pscustomobject]@{Role=$_.Role;Stage=$_.Stage;Values=($_.Samples.current|Sort-Object -Unique)-join ',';Samples=$_.Samples.Count;Refused=$_.RefusedReads;MinZ=($_.Samples|ForEach-Object {$_.position[2]}|Measure-Object -Minimum).Minimum}}

