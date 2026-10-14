[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
function Read-Coop([string]$Role) {
    $root=Join-Path $workspaceRoot "work\lab-instances\$Role"
    $launch=Get-Content -LiteralPath (Join-Path $root 'launch.json') -Raw | ConvertFrom-Json
    $process=Get-Process -Id $launch.pid -ErrorAction Stop
    if($process.Path -ne $launch.executable){throw "Unexpected $Role process."}
    $file=Get-Item -LiteralPath (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw "Stale $Role status: engine may be loading or stopped."}
    $status=Get-Content -LiteralPath $file.FullName -Raw | ConvertFrom-Json
    if($status.nativeInterface -ne 'SteamNetworkingSockets012' -or !$status.steamAvailable -or
       $status.connectionState -ne 3 -or !$status.remoteFresh -or !$status.localPoseAvailable -or !$status.remotePoseAvailable){throw "$Role native Steam connection or pose unavailable."}
    $status
}
function Distance($A,$B) {
    $sum=0.0;for($i=0;$i -lt $A.Count;$i++){$difference=[double]$A[$i]-[double]$B[$i];$sum+=$difference*$difference}
    [Math]::Sqrt($sum)
}
$results=@()
try {
    foreach($role in 'host','client') {
        $other=if($role -eq 'host'){'client'}else{'host'}
        $before=Read-Coop $role;$peerBefore=Read-Coop $other
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -LeftX 16000 -DurationMs 500 | Out-Null
        Start-Sleep -Milliseconds 1200
        $after=Read-Coop $role;$peerAfter=Read-Coop $other
        $movement=Distance $before.localPosition $after.localPosition
        $replicationError=Distance $after.localPosition $peerAfter.remotePosition
        $peerMovement=Distance $peerBefore.localPosition $peerAfter.localPosition
        $result=[ordered]@{Role=$role;Before=$before;After=$after;PeerBefore=$peerBefore;PeerAfter=$peerAfter;
            Movement=$movement;RemotePoseError=$replicationError;OtherLocalPlayerMovement=$peerMovement;
            Passed=($movement -gt 0.05 -and $replicationError -lt 0.02 -and $peerMovement -lt 0.02 -and
                $after.sent -gt $before.sent -and $peerAfter.received -gt $peerBefore.received -and
                $after.rejected -eq $before.rejected -and $peerAfter.rejected -eq $peerBefore.rejected)}
        $results+=[pscustomobject]$result
        if(!$result.Passed){throw "Native Steam movement transfer failed for $role."}
    }
} finally {
    foreach($role in 'host','client'){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100 | Out-Null}
    [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;Tests=$results;
        Scope='Actual SteamNetworkingSockets012 movement packets between two workspace games over loopback; does not establish remote actor control or shared world state'} |
        ConvertTo-Json -Depth 8 | Set-Content -LiteralPath (Join-Path $PSScriptRoot 'research\steam-transport-verification.json')
}
$results | Select-Object Role,Movement,RemotePoseError,OtherLocalPlayerMovement,Passed
