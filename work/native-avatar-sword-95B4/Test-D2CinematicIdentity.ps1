[CmdletBinding()]
param([ValidateSet('Ready','Intro','Choice','PostChoice','Completed')][string]$Stage='Ready',
      [ValidateSet('host','client')][string]$CinematicRole='client')
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$record=Join-Path $PSScriptRoot 'research\native-cinematic-identity-verification.json'
function Get-NativeSignature($nodes) {
    $keys=@(foreach($node in $nodes) {
        if(!$node.ownerName -or $null -eq $node.nativeNodeId){throw 'Independent native observer could not resolve an identity.'}
        [bigint]$hash=[bigint]::Parse('14695981039346656037')
        foreach($byte in [Text.Encoding]::ASCII.GetBytes($node.ownerName)){$hash=(($hash -bxor $byte)*1099511628211) -band [bigint]::Parse('18446744073709551615')}
        [pscustomobject]@{OwnerId=$hash;NodeId=[uint32]$node.nativeNodeId;OwnerName=$node.ownerName}
    })
    if(!$keys.Count){return '0'}
    $keys=@($keys|Sort-Object OwnerId,NodeId)
    $bytes=[Collections.Generic.List[byte]]::new()
    $bytes.AddRange([BitConverter]::GetBytes([uint32]$keys.Count))
    foreach($key in $keys){$bytes.AddRange([BitConverter]::GetBytes([uint64]$key.OwnerId));$bytes.AddRange([BitConverter]::GetBytes($key.NodeId))}
    [bigint]$signature=[bigint]::Parse('14695981039346656037')
    foreach($byte in $bytes){$signature=(($signature -bxor $byte)*1099511628211) -band [bigint]::Parse('18446744073709551615')}
    return ([uint64]$signature).ToString('x')
}
$states=@{}
foreach($role in 'host','client') {
    $root=Join-Path $workspaceRoot "work\lab-instances\$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    $gameProcess=Get-Process -Id $launch.pid -ErrorAction Stop
    if($gameProcess.Path -ne $launch.executable){throw 'Unexpected process path.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale bridge status.'}
    $status=Get-Content $file.FullName -Raw|ConvertFrom-Json
    if(!$status.remoteFresh -or $status.connectionState -ne 3){throw 'Native Steam peer unavailable.'}
    $native=& (Join-Path $PSScriptRoot 'bin\d2_runtime_probe.exe') $launch.pid --cinematic
    if($LASTEXITCODE){throw 'Independent native cinematic inspection failed.'}
    $native=$native|ConvertFrom-Json
    $nodes=@($native.cinematicNodes|Where-Object field30 -eq 1)
    $signature=Get-NativeSignature $nodes
    if($status.localCinematicMetadataValid -ne 1 -or $status.localCinematicNodeCount -ne $nodes.Count -or $status.localCinematicSignature -ne $signature){throw 'Packet identity differs from independent native graph observations.'}
    $states[$role]=[pscustomobject]@{Status=$status;Native=$native;ExpectedSignature=$signature}
}
foreach($role in 'host','client') {
    $other=if($role -eq 'host'){'client'}else{'host'}
    if($states[$other].Status.remoteCinematicMetadataValid -ne 1 -or
       $states[$other].Status.remoteCinematicNodeCount -ne $states[$role].Status.localCinematicNodeCount -or
       $states[$other].Status.remoteCinematicSignature -ne $states[$role].ExpectedSignature){throw 'Native Steam peer did not receive the observed cinematic identity.'}
}
$expected=switch($Stage){Intro {5};Choice {14};PostChoice {5};default {1}}
$spectator=if($CinematicRole -eq 'host'){'client'}else{'host'}
if($states[$spectator].Status.localGameLogicState -ne 1 -or $states[$spectator].Status.localCinematicNodeCount -ne 0 -or $states[$CinematicRole].Status.localGameLogicState -ne $expected){throw 'Unexpected native state for this stage.'}
if($Stage -in 'Ready','Completed') {
    if($states[$CinematicRole].Status.localCinematicNodeCount -ne 0){throw 'Cinematic observers remain after native completion.'}
}elseif($states[$CinematicRole].Status.localCinematicNodeCount -lt 1 -or $states.client.Status.probeFollowEnabled -or $states.host.Status.probeFollowEnabled){throw 'Expected an observed native cinematic with movement follow disabled.'}
$entry=[pscustomobject]@{Stage=$Stage;SourceRole=$CinematicRole;ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=$true;States=$states}
if($Stage -eq 'Ready'){$entries=@($entry)}elseif(Test-Path $record){$prior=Get-Content $record -Raw|ConvertFrom-Json;$entries=@($prior.Stages|Where-Object { $_.Stage -ne $Stage -or ($_.SourceRole ?? 'client') -ne $CinematicRole })+$entry}else{$entries=@($entry)}
[ordered]@{Passed=$true;DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin\dinput8.dll')).Hash;Stages=$entries;Scope='Bounded native cinematic observer identities and their Steam replication. Signatures are independently calculated from validated owner names and authored native graph node IDs. This does not synchronize cinematic entry, playback, skipping, or completion.'}|
    ConvertTo-Json -Depth 12|Set-Content $record
[pscustomobject]@{Stage=$Stage;SourceRole=$CinematicRole;Passed=$true;NativeNodes=$states[$CinematicRole].Status.localCinematicNodeCount;Signature=$states[$CinematicRole].ExpectedSignature}
