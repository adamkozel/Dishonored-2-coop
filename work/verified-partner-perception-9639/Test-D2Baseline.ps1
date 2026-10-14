[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$backup=(Join-Path $root 'work/baseline-backup/Dishonored2/base').Replace('/','\').TrimEnd('\')+'\'
$original=Join-Path $env:USERPROFILE 'Saved Games/Arkane Studios/Dishonored2/base'
$manifest=Get-Content (Join-Path $root 'work/baseline-hashes.json') -Raw|ConvertFrom-Json
$files=foreach($entry in $manifest){
    if(!$entry.Path.StartsWith($backup,[StringComparison]::OrdinalIgnoreCase)){throw 'Unexpected baseline manifest path.'}
    $relative=$entry.Path.Substring($backup.Length)
    [pscustomobject]@{File=$relative;Matches=((Get-FileHash (Join-Path $original $relative)).Hash -eq $entry.Hash)}
}
$installed='C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
$result=[ordered]@{
    ObservedAt=[DateTimeOffset]::Now.ToString('o');Files=@($files)
    InstalledExecutableSha256=(Get-FileHash (Join-Path $installed 'Dishonored2.exe')).Hash
    OriginalSteamDllSha256=(Get-FileHash (Join-Path $installed 'steam_api64.dll')).Hash
    StagedSteamDllSha256=(Get-FileHash (Join-Path $root 'work/lab-game/steam_api64.dll')).Hash
    InstalledDinput8Present=(Test-Path (Join-Path $installed 'dinput8.dll'))
    LoaderSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash
    StagedLoaderSha256=(Get-FileHash (Join-Path $root 'work/lab-game/dinput8.dll')).Hash
}
$result.Passed=(@($files|Where-Object {!$_.Matches}).Count -eq 0 -and !$result.InstalledDinput8Present -and
    $result.InstalledExecutableSha256 -eq 'B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180' -and
    $result.OriginalSteamDllSha256 -eq '71AF8666392ACAD2080A816B9F1196FE9CCB950E1DB8A549F8312C4D231F55A8' -and
    $result.OriginalSteamDllSha256 -eq $result.StagedSteamDllSha256 -and $result.LoaderSha256 -eq $result.StagedLoaderSha256)
$result|ConvertTo-Json -Depth 5|Set-Content (Join-Path $PSScriptRoot 'research/baseline-integrity-verification.json')
if(!$result.Passed){throw 'Baseline or loader staging integrity check failed.'}
[pscustomobject]@{Passed=$true;OriginalFilesUnchanged=$files.Count;InstalledGameUnmodified=$true;StagedLoaderMatchesBuild=$true}
