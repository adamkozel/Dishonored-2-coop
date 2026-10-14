$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;$lab=Join-Path $root 'outputs/d2-coop-lab'
$probe=Join-Path $PSScriptRoot 'd2_state_probe.exe'
$rows=[Collections.Generic.List[object]]::new();$failure=$null
try {
    foreach($role in 'host','client') {
        $launch=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
        if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected process.'}
        foreach($stage in 'Standing','Crouched','StandingAgain','Jump','CrouchedAgain','CrouchedJump','ReturnStanding') {
            $button=switch($stage){'Crouched'{'B'}'StandingAgain'{'B'}'Jump'{'A'}'CrouchedAgain'{'B'}'CrouchedJump'{'A'}'ReturnStanding'{'B'}default{'Neutral'}}
            & (Join-Path $lab 'Send-D2Input.ps1') -Role $role -Button $button -DurationMs 400|Out-Null
            $samples=[Collections.Generic.List[object]]::new();$refused=0;$watch=[Diagnostics.Stopwatch]::StartNew()
            do {
                $raw=& $probe $launch.pid --player-states
                if(!$LASTEXITCODE){$samples.Add(($raw|ConvertFrom-Json))}elseif($LASTEXITCODE -eq 52){++$refused}else{throw 'Probe process failed.'}
                Start-Sleep -Milliseconds 25
            }while($watch.ElapsedMilliseconds -lt 2000)
            if(!$samples.Count){throw 'No valid active-state samples.'}
            $rows.Add([ordered]@{Role=$role;Stage=$stage;RejectedConcurrentReads=$refused;Samples=$samples.ToArray()})
        }
    }
}catch{$failure=$_.Exception.Message}finally{
    foreach($role in 'host','client') {
        & (Join-Path $lab 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null
        $l=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
        $raw=& $probe $l.pid --player-stance
        if(!$LASTEXITCODE -and ($raw|ConvertFrom-Json).current -eq 1){& (Join-Path $lab 'Send-D2Input.ps1') -Role $role -Button B -DurationMs 400|Out-Null;Start-Sleep -Milliseconds 700}
    }
}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Failure=$failure;DllSha256=(Get-FileHash (Join-Path $lab 'bin/dinput8.dll')).Hash;
    ProbeSha256=(Get-FileHash $probe).Hash;Scope='Read-only native active-state mapping during bounded ordinary controller input. No action-state networking or remote animation application.';Observations=$rows.ToArray()}|
    ConvertTo-Json -Depth 12|Set-Content (Join-Path $lab 'research/player-active-state-observations.json')
if($failure){throw $failure}
$rows|ForEach-Object {[pscustomobject]@{Role=$_.Role;Stage=$_.Stage;States=($_.Samples.active.type|Sort-Object -Unique)-join ', ';Stances=($_.Samples.stance|Sort-Object -Unique)-join ',';Samples=$_.Samples.Count;Rejected=$_.RejectedConcurrentReads}}
