$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$expected=Join-Path $root 'work/asset-game/Dishonored2.exe'
$observed=@(Get-CimInstance Win32_Process -Filter "name = 'Dishonored2.exe'")
foreach($role in 'host','client') {
    $profile=(Join-Path $root "work/lab-instances/$role").Replace('\','/')
    $matches=@($observed|Where-Object {$_.ExecutablePath -eq $expected -and $_.CommandLine.Contains($profile)})
    if($matches.Count -ne 1){throw "Expected exactly one actual isolated $role process"}
    $process=Get-Process -Id $matches[0].ProcessId
    $file=Join-Path $root "work/lab-instances/$role/launch.json"
    Copy-Item -LiteralPath $file -Destination (Join-Path $PSScriptRoot "$role-sandbox-launch.json")
    $launch=Get-Content -LiteralPath $file -Raw|ConvertFrom-Json
    $launch.pid=$process.Id;$launch.startedAt=$process.StartTime.ToString('o')
    $launch|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $file
    [pscustomobject]@{Role=$role;Pid=$process.Id;Responding=$process.Responding;StatusUpdated=(Get-Item (Join-Path $root "work/lab-instances/$role/coop-status.json")).LastWriteTime}
}
