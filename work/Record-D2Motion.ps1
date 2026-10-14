$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;$lab=Join-Path $root 'outputs/d2-coop-lab';$research=Join-Path $lab 'research'
$hash=(Get-FileHash "$lab/bin/dinput8.dll").Hash
$required=@('player-motion-verification.json','player-stance-verification.json','avatar-ai-ownership-verification.json',
 'steam-transport-verification.json','shared-checkpoint-host-delay-host-verification.json','shared-checkpoint-client-delay-client-verification.json',
 'shared-death-host-verification.json','shared-death-client-verification.json','locomotion-verification.json',
 'locomotion-guards-verification.json','avatar-idle-disconnect-verification.json','lab-restoration-verification.json','baseline-integrity-verification.json')
$rows=@(foreach($name in $required){
 $file=Join-Path $research $name;$r=Get-Content $file -Raw|ConvertFrom-Json
 $build=if($name -eq 'baseline-integrity-verification.json'){$r.LoaderSha256}else{$r.DllSha256}
 if($build -ne $hash){throw "Wrong build: $name"}
 if($name -eq 'steam-transport-verification.json'){if($r.Tests.Count -ne 2 -or @($r.Tests|Where-Object {!$_.Passed}).Count){throw 'Steam regression incomplete'}}elseif(!$r.Passed){throw "Failed report: $name"}
 [ordered]@{Report=$name;Passed=$true;DllSha256=$hash;ObservedAt=$r.ObservedAt;Sha256=(Get-FileHash $file).Hash}
})
$live=@(foreach($role in 'host','client'){
 $s=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
 $r=Get-Content "$root/work/lab-instances/$role/coop-status.json.reset.json" -Raw|ConvertFrom-Json
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $l.pid).Path -ne $l.executable -or $s.localGameLogicState -ne 1 -or !$s.probeFollowEnabled -or !$s.remoteFresh -or
 !$r.avatarIdleOwned -or $r.avatarIdleLost -or $r.avatarIdleRefused -or $r.avatarIdleAcquired-$r.avatarIdleReleased -ne 1){throw 'Final restored state is not valid'}
 [ordered]@{Role=$role;Pid=$l.pid;Epoch=$s.localEpoch;Follow=$s.probeFollowEnabled;IdleOwned=$r.avatarIdleOwned;IdleAcquired=$r.avatarIdleAcquired;IdleReleased=$r.avatarIdleReleased;IdleLost=$r.avatarIdleLost}
})
$summary=[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=$true;DllSha256=$hash;PoseProtocol=9;PacketBytes=192;Reports=$rows;Live=$live;
 Scope='Read-only actual player stance and verified default-mode ground/sneak/jump/fall states transported over native Steam. Standing and crouched jumps in both directions, native pause invalidation, both shared reloads and natural deaths, counterpart walking, owned native idle-break autoplay restoration, and actual peer-loss cleanup passed.';
 RemoteCrouchJumpAnimation=$false;NativeIdleAutoplayControl=$true;PlayableCoop=$false;SharedWorldAuthority=$false;TwoAccountOnlineVerified=$false;
 PriorBroadRecovery='milestones/7623/shared-recovery-summary.json';PriorStance='milestones/0247/player-stance-summary.json';
 KnownFailureFixed='D117 Emily idle foot-motion failure coincided with full native Midnight animation weight. Native idle-break autoplay control fixed the original unchanged locomotion assertions; failed evidence retained.'}
$folder=Join-Path $research ('milestones/'+$hash.Substring(0,4).ToLower());New-Item -ItemType Directory -Path $folder -Force|Out-Null
foreach($name in $required){Copy-Item (Join-Path $research $name) $folder}
$summary|ConvertTo-Json -Depth 15|Set-Content "$folder/player-motion-summary.json"
$summary|ConvertTo-Json -Depth 15|Set-Content "$research/player-motion-summary.json"
$failed=Get-Content "$root/work/locomotion-D117-idle-failure.json" -Raw|ConvertFrom-Json
[ordered]@{Passed=$false;DllSha256=$failed.DllSha256;ObservedAt=$failed.ObservedAt;Failure=$failed.Failure;Results=$failed.Results;Scope='Condensed original failure; full raw observations retained in work/locomotion-D117-idle-failure.json.'}|ConvertTo-Json -Depth 10|Set-Content "$research/locomotion-idle-break-failure.json"
$backup=Join-Path $root ('work/verified-player-motion-'+$hash.Substring(0,4).ToLower());New-Item -ItemType Directory -Path $backup -Force|Out-Null
Copy-Item "$lab/src" $backup -Recurse;Copy-Item "$lab/bin/dinput8.dll","$lab/bin/d2_runtime_probe.exe" $backup;Copy-Item $folder "$backup/reports" -Recurse
foreach($path in "$root/work/current-task-state.json","$research/verification.json"){
 $s=Get-Content $path -Raw|ConvertFrom-Json
 $s|Add-Member -NotePropertyName PreviousVerifiedLoaderSha256 -NotePropertyValue $s.LoaderSha256 -Force
 $s.LoaderSha256=$hash
 $s|Add-Member -NotePropertyName PlayerMotion -NotePropertyValue $summary -Force
 $s|Add-Member -NotePropertyName CurrentBuildReports -NotePropertyValue $required -Force
 $s|Add-Member -NotePropertyName VerificationAttribution -NotePropertyValue 'PlayerMotion reports are hash-checked for 6EC7. Broader fixed-fixture recovery remains separately attributed to 7623. Other feature records retain historical build hashes. No playable co-op claim.' -Force
 if($s.PSObject.Properties.Name -contains 'WorkInProgress'){
  $s.UpdatedAt=$summary.ObservedAt;$s.Live=$live;$s.WorkInProgress.CandidateDll=$hash;$s.WorkInProgress.UpdatedAt=$summary.ObservedAt
  $s.WorkInProgress.Scope=$summary.Scope;$s.WorkInProgress.PendingTest='Current build movement, idle ownership, shared recovery and peer-loss regressions passed. Begin native skeleton reader validation; no active game mutation test.'
  $s.Next='Continue native player-to-counterpart animation mapping. Native action state transport is verified but remote crouch/jump poses are not implemented. Then continue world/combat/interaction authority and native session UI. No milestone pause.'
 }
 $s|ConvertTo-Json -Depth 35|Set-Content $path
}
[pscustomobject]@{Passed=$true;DllSha256=$hash;Reports=$rows.Count;Archive=$folder;PlayableCoop=$false}
