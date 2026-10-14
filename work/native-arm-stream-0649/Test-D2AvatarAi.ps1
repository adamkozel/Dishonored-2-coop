[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$failure=$null;$rows=[Collections.Generic.List[object]]::new();$launch=@{}
foreach($role in 'host','client'){$launch[$role]=Get-Content (Join-Path $root "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json}
function State([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json") -Raw|ConvertFrom-Json}
function Reset([string]$role){Get-Content (Join-Path $root "work/lab-instances/$role/coop-status.json.reset.json") -Raw|ConvertFrom-Json}
function Command([string]$role,[string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null}
function Check([string]$role,[string]$stage,[bool]$held) {
    $s=State $role;$r=Reset $role
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch[$role].pid --ai-decision $s.probeActorHandle
    if($LASTEXITCODE){throw "$role native decision unavailable at $stage"};$a=$raw|ConvertFrom-Json
    $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch[$role].pid --ai-idle $s.probeActorHandle
    if($LASTEXITCODE){throw "$role native idle-break unavailable at $stage"};$idle=$raw|ConvertFrom-Json
    $rows.Add([pscustomobject]@{Role=$role;Stage=$stage;Bridge=$s;Reset=$r;Ai=$a;Idle=$idle})
    # Native pause stops scheduler servicing. Its own pending-resume state (3)
    # can retain inactive membership after our reference reaches zero; the
    # resumed-and-rebound check below proves servicing and acquisition work.
    $deferredResume=!$held -and $stage -eq 'native_pause' -and $s.localGameLogicState -eq 2 -and $a.pauseState -eq 3
    if($r.avatarAiHeld -ne $held -or $a.pauseReferences -ne $(if($held){1}else{0}) -or (!$deferredResume -and $a.schedulerActive -eq $held) -or
       $r.avatarAiLost -or $r.avatarAiRefused -or $r.avatarAiAcquired-$r.avatarAiReleased -ne $(if($held){1}else{0})){
        throw "$role decision reference/scheduler mismatch at $stage"
    }
    if($r.avatarIdleOwned -ne $held -or [bool]($idle.idleAutoPlay -band 1) -eq $held -or
       ($held -and $idle.idleState -eq 3) -or $r.avatarIdleLost -or $r.avatarIdleRefused -or
       $r.avatarIdleAcquired-$r.avatarIdleReleased -ne $(if($held){1}else{0})) {
        throw "$role idle-break autoplay ownership mismatch at $stage"
    }
    $r
}
try {
    foreach($role in 'host','client') {
        if((State $role).localGameLogicState -ne 1 -or !(State $role).probeFollowEnabled){throw 'Both owned counterparts must start following in gameplay.'}
        $before=Check $role 'initial' $true
        Command $role 'coop_probe_follow on';Start-Sleep -Milliseconds 400
        $duplicate=Check $role 'duplicate_on' $true
        if($duplicate.avatarAiAcquired -ne $before.avatarAiAcquired -or $duplicate.avatarIdleAcquired -ne $before.avatarIdleAcquired){throw 'Duplicate follow acquired another reference or autoplay control.'}
        Command $role 'coop_probe_follow off';Start-Sleep -Milliseconds 450
        Check $role 'explicit_off' $false|Out-Null
        Command $role 'coop_probe_follow on';Start-Sleep -Milliseconds 450
        Check $role 'rebound' $true|Out-Null
    }
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role host -Button Start -DurationMs 150|Out-Null
    Start-Sleep -Milliseconds 700
    if((State host).localGameLogicState -ne 2){throw 'Host did not enter native pause.'}
    foreach($role in 'host','client'){Check $role 'native_pause' $false|Out-Null}
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role host -Button Start -DurationMs 150|Out-Null
    Start-Sleep -Milliseconds 700
    foreach($role in 'host','client'){
        Command $role 'coop_probe_follow on';Start-Sleep -Milliseconds 450
        Check $role 'resumed_and_rebound' $true|Out-Null
    }
}catch{$failure=$_.Exception.Message}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;Samples=$rows;
    Scope='Owned counterpart AIDecision: one native pause reference and independent scheduler exclusion. Owned idle-break: native autoplay disabled while following, restored on release; its own pause references are not borrowed. No duplicate acquisition, balanced release/reacquisition and both native-pause boundaries. Ordinary world NPC AI is not controlled by this feature.'}|
    ConvertTo-Json -Depth 12|Set-Content (Join-Path $PSScriptRoot 'research/avatar-ai-ownership-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;NativeSamples=$rows.Count;LostReferences=0}
