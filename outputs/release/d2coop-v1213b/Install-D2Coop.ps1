param([Parameter(Mandatory)][string]$GameDirectory)
# Installs the co-op payload into a Dishonored 2 folder chosen by the user.
# Refuses unless the executable and steam_api64.dll are the exact supported
# builds and master.index is the original (or this package's own). Backs up
# every file it replaces and writes a receipt used by Uninstall-D2Coop.ps1.
$ErrorActionPreference='Stop'
$package=$PSScriptRoot;$manifest=Get-Content (Join-Path $package 'manifest.json') -Raw|ConvertFrom-Json
$game=(Resolve-Path -LiteralPath $GameDirectory).Path
function Get-Sha256Hash($p){(Get-FileHash -LiteralPath $p -Algorithm SHA256).Hash}
if((Get-Sha256Hash (Join-Path $game 'Dishonored2.exe'))-ne$manifest.requiredExecutableSha256){throw 'Unsupported Dishonored2.exe build; nothing changed.'}
if((Get-Sha256Hash (Join-Path $game 'steam_api64.dll'))-ne$manifest.requiredSteamApiSha256){throw 'Unexpected steam_api64.dll; nothing changed.'}
$receiptPath=Join-Path $game 'd2coop-install-receipt.json'
if(Test-Path -LiteralPath $receiptPath){throw 'Co-op is already installed here (receipt present); uninstall first.'}
$master=Join-Path $game 'base/master.index';$modMaster=@($manifest.files|Where-Object path -eq 'base/master.index')[0].sha256
if((Get-Sha256Hash $master)-ne$manifest.originalMasterIndexSha256){throw 'base/master.index is not the original; restore the original (verify files in Steam) first.'}
foreach($f in $manifest.files){if((Get-Sha256Hash (Join-Path $package "files/$($f.path)"))-ne$f.sha256){throw "Package file corrupted: $($f.path)"}}
$stamp=Get-Date -Format 'yyyyMMdd-HHmmss';$backup=Join-Path $game "d2coop-backup/$stamp"
New-Item -ItemType Directory -Force $backup|Out-Null
$backedUp=[Collections.Generic.List[object]]::new();$added=[Collections.Generic.List[string]]::new()
foreach($f in $manifest.files){
  $target=Join-Path $game $f.path
  if(Test-Path -LiteralPath $target){$dest=Join-Path $backup $f.path;New-Item -ItemType Directory -Force (Split-Path $dest)|Out-Null;Copy-Item -LiteralPath $target $dest;$backedUp.Add(@{path=$f.path;sha256=(Get-Sha256Hash $target)})}
  else{$added.Add($f.path)}
}
foreach($f in $manifest.files){$dest=Join-Path $game $f.path;New-Item -ItemType Directory -Force (Split-Path $dest)|Out-Null;Copy-Item -LiteralPath (Join-Path $package "files/$($f.path)") $dest -Force}
foreach($f in $manifest.files){if((Get-Sha256Hash (Join-Path $game $f.path))-ne$f.sha256){throw "Installed file mismatch: $($f.path) (run Uninstall-D2Coop.ps1 to roll back)"}}
@{installed=(Get-Date).ToString('o');candidate=$manifest.candidate;backup="d2coop-backup/$stamp";backedUp=@($backedUp);added=@($added);files=@($manifest.files)}|ConvertTo-Json -Depth 6|Set-Content -LiteralPath $receiptPath -Encoding utf8
"Installed $($manifest.candidate): $($manifest.files.Count) files; backup in d2coop-backup/$stamp"
