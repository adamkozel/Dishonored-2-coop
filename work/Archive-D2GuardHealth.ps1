$ErrorActionPreference='Stop'
$lab=Join-Path $PSScriptRoot '../outputs/d2-coop-lab'
$hash=(Get-FileHash "$lab/bin/dinput8.dll").Hash
if($hash-ne'42BEAD3DF5FDCAE248E332F84F3CBAA82C63587FF604424B3A761DA345F629FA'){throw 'Unexpected build'}
$dest=Join-Path $lab 'research/milestones/42be'
$backup=Join-Path $PSScriptRoot 'verified-guard-health-42BE'
New-Item -ItemType Directory -Path $dest,$backup -Force|Out-Null
Copy-Item "$lab/src","$lab/bin" -Destination $backup -Recurse -Force
Copy-Item "$lab/*.ps1","$lab/CMakeLists.txt" -Destination $backup -Force
$names=@('guard-health','guard-link','world-observation','shared-death-host-health-damage','shared-death-client-health-damage','body-stream','baseline-integrity','lab-restoration')
$reports=@(foreach($name in $names){
 $file="$name-verification.json"
 $r=Get-Content "$lab/research/$file" -Raw|ConvertFrom-Json
 $reportHash=if($r.DllSha256){$r.DllSha256}else{$r.LoaderSha256}
 if(!$r.Passed-or$reportHash-ne$hash){throw "Wrong evidence: $file"}
 Copy-Item "$lab/research/$file" $dest -Force
 [ordered]@{Report=$file;Passed=$r.Passed;ObservedAt=$r.ObservedAt;Sha256=(Get-FileHash "$dest/$file").Hash}
})
Copy-Item "$PSScriptRoot/body-stream-observations.json","$PSScriptRoot/body-stream-42BE-regression-failed.json","$PSScriptRoot/body-stream-42BE-latency-diagnostic.json" $backup -Force
$summary=[ordered]@{ObservedAt=(Get-Date).ToString('o');DllSha256=$hash;ProbeSha256=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;Passed=$true;Reports=$reports;Scope='One authored inactive guard: explicitly armed native health 40 to 35, host/client isolation and lifetime guards; read-only world health; both natural fall deaths and coordinated retries; body stream and baseline integrity.';BodyFixtureCorrection='Four transition captures precede stationary crouch sampling. First pre-fix sample crossed the native transition. Rotation tolerance unchanged; failed raw data preserved.';PlayableCoop=$false;GeneralCombatReplication=$false;OnlineTwoAccounts=$false;SourceBackup='work/verified-guard-health-42BE'}
$summary|ConvertTo-Json -Depth 8|Set-Content "$dest/guard-health-summary.json"
$state=Get-Content "$PSScriptRoot/current-task-state.json" -Raw|ConvertFrom-Json
$state.UpdatedAt=(Get-Date).ToString('o')
$state.Next='Continue native faction/perception and counterpart combat research. Current42BE eight targeted reports archived; no active tests.'
$state.WorkInProgress.CandidateDll=$hash
$state.WorkInProgress.PendingTest='No active test. Guard health, world observation, both natural damage/death tests, body stream and baseline integrity passed.'
$state.WorkInProgress.Next=$state.Next
$state|Add-Member -NotePropertyName GuardHealthMilestone -NotePropertyValue $summary -Force
$state.Live=@(foreach($role in @('host','client')){$launch=Get-Content "$PSScriptRoot/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$status=Get-Content "$PSScriptRoot/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json;[ordered]@{Role=$role;Pid=$launch.pid;Epoch=$status.localEpoch;Handle=$status.probeActorHandle;Follow=$status.probeFollowEnabled;Fresh=$status.remoteFresh;BodyStream=$status.bodyStreamEnabled}})
$state|ConvertTo-Json -Depth 30|Set-Content "$PSScriptRoot/current-task-state.json"
$summary|ConvertTo-Json -Depth 8
