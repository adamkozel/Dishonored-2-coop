[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin\d2_runtime_probe.exe'
$roles=@{}
foreach($role in 'host','client') {
    $launch=Get-Content -LiteralPath (Join-Path $workspaceRoot "work\lab-instances\$role\launch.json") -Raw | ConvertFrom-Json
    $process=Get-Process -Id $launch.pid -ErrorAction Stop
    if($process.Path -ne $launch.executable) {throw "Recorded $role process differs."}
    $roles[$role]=$launch.pid
}
function Read-Player([string]$Role) {
    $json=& $probe $roles[$Role] --player
    if($LASTEXITCODE -ne 0) {throw "Cannot read $Role gameplay state. Enter its isolated baseline first."}
    $state=$json | ConvertFrom-Json
    if(!$state.candidatePosition -or !$state.candidateOrientation) {throw "No local player in $Role."}
    $state
}
function Distance($A,$B) {
    $sum=0.0
    for($i=0;$i -lt $A.Count;$i++) {$sum+=([double]$A[$i]-[double]$B[$i])*([double]$A[$i]-[double]$B[$i])}
    [Math]::Sqrt($sum)
}
$results=@()
try {
    foreach($role in 'host','client') {
        $other=if($role -eq 'host') {'client'}else{'host'}
        $before=Read-Player $role
        $otherBefore=Read-Player $other
        & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -RightX 15000 -DurationMs 400 | Out-Null
        Start-Sleep -Milliseconds 900
        $after=Read-Player $role
        $otherAfter=Read-Player $other
        Start-Sleep -Milliseconds 700
        $released=Read-Player $role
        $rotation=Distance $before.candidateOrientation $after.candidateOrientation
        $drift=Distance $after.candidateOrientation $released.candidateOrientation
        $otherMove=Distance $otherBefore.candidatePosition $otherAfter.candidatePosition
        $result=[ordered]@{Role=$role;Before=$before;After=$after;Released=$released;OtherBefore=$otherBefore;OtherAfter=$otherAfter;RotationChange=$rotation;PostExpiryRotationChange=$drift;OtherPlayerDisplacement=$otherMove;Passed=($rotation -gt 0.02 -and $drift -lt 0.01 -and $otherMove -lt 0.01)}
        $results+=[pscustomobject]$result
        if(!$result.Passed) {throw "Controller routing/release check failed for $role."}
    }
} finally {
    foreach($role in 'host','client') {& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100 | Out-Null}
    [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Tests=$results;Scope='Native controller routing and expiry in the isolated loaded scene; no co-op replication'} | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath (Join-Path $PSScriptRoot 'research\autonomous-input-verification.json')
}
$results | Select-Object Role,RotationChange,PostExpiryRotationChange,OtherPlayerDisplacement,Passed
