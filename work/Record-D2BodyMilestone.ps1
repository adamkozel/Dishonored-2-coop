$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;$lab=Join-Path $root 'outputs/d2-coop-lab';$research=Join-Path $lab 'research'
$hash=(Get-FileHash "$lab/bin/dinput8.dll").Hash
$required=@('body-stream-first-verification.json','body-stream-verification.json','body-stream-guards-verification.json',
 'avatar-ai-ownership-verification.json','player-motion-verification.json','player-stance-verification.json','steam-transport-verification.json',
 'shared-checkpoint-host-delay-host-verification.json','shared-checkpoint-client-delay-client-verification.json',
 'shared-death-host-verification.json','shared-death-client-verification.json','locomotion-verification.json','baseline-integrity-verification.json','lab-restoration-verification.json')
$reports=@(foreach($name in $required){
 $r=Get-Content "$research/$name" -Raw|ConvertFrom-Json
 $build=if($name -eq 'baseline-integrity-verification.json'){$r.LoaderSha256}else{$r.DllSha256}
 if($build -ne $hash){throw "Wrong report build: $name"}
 if($name -eq 'steam-transport-verification.json'){if($r.Tests.Count -ne 2 -or @($r.Tests|Where-Object {!$_.Passed}).Count){throw 'Steam report did not pass'}}elseif(!$r.Passed){throw "Report did not pass: $name"}
 [ordered]@{Report=$name;Passed=$true;Sha256=(Get-FileHash "$research/$name").Hash;DllSha256=$build;ObservedAt=$r.ObservedAt}
})
$live=@(foreach($role in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 $s=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
 $a=Get-Content "$root/work/lab-instances/$role/coop-status.json.reset.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $l.pid).Path -ne $l.executable -or $s.localGameLogicState -ne 1 -or !$s.probeFollowEnabled -or !$s.bodyStreamEnabled -or !$s.remoteFresh -or $s.rejected -or $a.avatarAiLost -or $a.avatarIdleLost){throw 'Unexpected current lab state'}
 [ordered]@{Role=$role;Pid=$l.pid;Epoch=$s.localEpoch;Handle=$s.probeActorHandle;BodyStream=$s.bodyStreamEnabled;BodyApplied=$s.bodyStreamApplied;Follow=$s.probeFollowEnabled;Fresh=$s.remoteFresh}
})
$summary=[ordered]@{Passed=$true;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=$hash;ProbeSha256=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;PoseProtocol=10;PacketBytes=1156;Reports=$reports;Live=$live;
 Scope='Experimental actual-player body poses over native Steam; 17 mapped joints on Corvo/Emily counterparts, crouch/crouch-walk/jumps, native standing gait, 250ms freshness, both pauses and actual peer loss. Verified again after both shared reloads and deaths. Unmapped native arm motion and target bone lengths preserved.';
 NativeUiAdded=$false;VisualAppearanceReviewed=$false;CombatReplicated=$false;WorldAuthority=$false;OnlineTwoAccountsVerified=$false;PlayableCoop=$false;
 PhysicsNote='Clean idle baseline already shows about 0.01182 maximum Emily body/joint separation with zero body-stream applications. The local crouch experiment reached a similar Emily error and about 0.00281 for Corvo. Existing independent animated-body tolerance is unchanged; shared hit detection is not established.';
 PriorMovement='milestones/6ec7/player-motion-summary.json';PriorBroadRecovery='milestones/7623/shared-recovery-summary.json'}
$folder=Join-Path $research ('milestones/'+$hash.Substring(0,4).ToLower());New-Item -ItemType Directory -Path $folder -Force|Out-Null
foreach($name in $required){Copy-Item "$research/$name" $folder}
$summary|ConvertTo-Json -Depth 18|Set-Content "$folder/body-stream-summary.json"
$summary|ConvertTo-Json -Depth 18|Set-Content "$research/body-stream-summary.json"
$backup=Join-Path $root ('work/verified-body-stream-'+$hash.Substring(0,4).ToLower());New-Item -ItemType Directory -Path $backup -Force|Out-Null
Copy-Item "$lab/src" $backup -Recurse;Copy-Item "$lab/bin/dinput8.dll","$lab/bin/d2_runtime_probe.exe","$lab/CMakeLists.txt","$lab/Build-D2Lab.ps1" $backup
Copy-Item $folder "$backup/reports" -Recurse
Copy-Item "$root/work/body-stream-observations.json","$root/work/body-stream-28E6-first.json","$root/work/body-physics-idle-baseline.json" $backup
foreach($path in "$root/work/current-task-state.json","$research/verification.json"){
 $s=Get-Content $path -Raw|ConvertFrom-Json
 $s|Add-Member -NotePropertyName PreviousVerifiedLoaderSha256 -NotePropertyValue $s.LoaderSha256 -Force
 $s.LoaderSha256=$hash
 $s|Add-Member -NotePropertyName BodyStream -NotePropertyValue $summary -Force
 $s|Add-Member -NotePropertyName CurrentBuildReports -NotePropertyValue $required -Force
 $s|Add-Member -NotePropertyName VerificationAttribution -NotePropertyValue 'Body stream and 14 targeted reports hash-checked for 28E6. Previous movement 6EC7 and broad recovery 7623 remain separate immutable evidence. No complete co-op claim.' -Force
 if($s.PSObject.Properties.Name -contains 'WorkInProgress'){
  $s.UpdatedAt=$summary.ObservedAt;$s.Live=$live;$s.WorkInProgress.UpdatedAt=$summary.ObservedAt;$s.WorkInProgress.CandidateDll=$hash
  $s.WorkInProgress.PendingTest='No active game test. Body streaming and targeted recovery regressions passed. Next: read-only native health observer and validation; do not mutate health until native semantics and ownership are established.'
  $s.WorkInProgress.Scope=$summary.Scope
  $s.Next='Investigate native health/damage and shared enemy authority. Remote torso/leg pose support is now experimental and verified in the Dunwall fixture. Full combat, arms/weapons/powers, shared interactions/progression, native session UI and real online remain. Continue without milestone pause.'
 }
 $s|ConvertTo-Json -Depth 35|Set-Content $path
}
[pscustomobject]@{Passed=$true;DllSha256=$hash;Reports=$reports.Count;Archive=$folder}
