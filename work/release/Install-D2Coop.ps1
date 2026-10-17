param([string]$GameDirectory)
# Installs the co-op mod into a Dishonored 2 folder (found through Steam when not given).
# Works in Windows PowerShell 5.1 and PowerShell 7. The package ships no game files: every game-derived file
# (base/master.index, base/game*voidmod.*, d2coop/*-choke-bank.bin, d2coop/native-tables.bin) is built here
# by bin/d2coop_setup.exe from this game folder's own files, then checked. Refuses unless the executable and
# steam_api64.dll are the exact supported builds and master.index is the original. Backs up every file it
# replaces and writes a receipt used by Uninstall-D2Coop.ps1. Nothing in the game folder changes until every
# file has been built and checked.
$ErrorActionPreference='Stop'
$package=$PSScriptRoot;$manifest=Get-Content (Join-Path $package 'manifest.json') -Raw|ConvertFrom-Json
function Get-Sha256Hash($p){(Get-FileHash -LiteralPath $p -Algorithm SHA256).Hash}
function Find-Dishonored2{
  # Steam's library folders list every library; Dishonored 2 is app 403640.
  $steam=$null;foreach($k in 'HKCU:\Software\Valve\Steam','HKLM:\SOFTWARE\WOW6432Node\Valve\Steam','HKLM:\SOFTWARE\Valve\Steam'){
    $v=Get-ItemProperty -Path $k -ErrorAction SilentlyContinue;if($v){if($v.SteamPath){$steam=$v.SteamPath}elseif($v.InstallPath){$steam=$v.InstallPath}};if($steam){break}}
  if(!$steam){throw 'Steam was not found. Run: Install.cmd "<your Dishonored2 folder>"'}
  $libraries=@($steam.Replace('/','\'))
  $vdf=Join-Path $steam 'steamapps\libraryfolders.vdf'
  if(Test-Path -LiteralPath $vdf){foreach($m in [regex]::Matches((Get-Content -LiteralPath $vdf -Raw),'"path"\s+"([^"]+)"')){$libraries+=$m.Groups[1].Value.Replace('\\','\')}}
  foreach($l in $libraries|Select-Object -Unique){$d=Join-Path $l 'steamapps\common\Dishonored2';if(Test-Path -LiteralPath (Join-Path $d 'Dishonored2.exe')){return $d}}
  throw 'Dishonored 2 was not found in your Steam libraries. Run: Install.cmd "<your Dishonored2 folder>"'
}
if(!$GameDirectory){$GameDirectory=Find-Dishonored2;"Dishonored 2 found: $GameDirectory"}
$game=(Resolve-Path -LiteralPath $GameDirectory).Path
# Refuse while this game folder's Dishonored2.exe runs (a process whose path cannot be read counts as running).
$running=@(Get-Process -Name Dishonored2 -ErrorAction SilentlyContinue|Where-Object {$path=$null;try{$path=$_.Path}catch{};!$path-or([IO.Path]::GetFullPath($path)-ieq[IO.Path]::GetFullPath((Join-Path $game 'Dishonored2.exe')))})
if($running.Count){throw 'Close Dishonored 2 first; nothing changed.'}
if((Get-Sha256Hash (Join-Path $game 'Dishonored2.exe'))-ne$manifest.requiredExecutableSha256){throw 'Unsupported Dishonored2.exe build (the mod needs the current Steam version); nothing changed.'}
if((Get-Sha256Hash (Join-Path $game 'steam_api64.dll'))-ne$manifest.requiredSteamApiSha256){throw 'Unexpected steam_api64.dll; nothing changed.'}
$receiptPath=Join-Path $game 'd2coop-install-receipt.json'
if(Test-Path -LiteralPath $receiptPath){throw 'Co-op is already installed here (receipt present); uninstall first.'}
if((Get-Sha256Hash (Join-Path $game 'base/master.index'))-ne$manifest.originalMasterIndexSha256){throw 'base/master.index is not the original; restore it (Steam: Properties > Installed Files > Verify) first.'}
# Shipped files sit under files\ in the package and install to the same relative path in the game folder.
foreach($f in @($manifest.files|ForEach-Object {[pscustomobject]@{path="files/$($_.path)";sha256=$_.sha256}})+@($manifest.tools)+@($manifest.data)){if((Get-Sha256Hash (Join-Path $package $f.path))-ne$f.sha256){throw "Package file corrupted: $($f.path); download the release again."}}
# Build the game-derived files from this game folder into a staging folder.
$stage=Join-Path $game 'd2coop-install-staging'
if(Test-Path -LiteralPath $stage){Remove-Item -LiteralPath $stage -Recurse -Force}
New-Item -ItemType Directory -Force (Join-Path $stage 'd2coop')|Out-Null
$setup=Join-Path $package 'bin\d2coop_setup.exe'
function Invoke-Setup([string[]]$arguments,[string]$what){
  Write-Host "Building $what from your game files..."
  # Windows PowerShell 5.1 turns native stderr into errors; read the exit code instead.
  $previous=$ErrorActionPreference;$ErrorActionPreference='Continue'
  try{$out=@(& $setup @arguments 2>&1)}finally{$ErrorActionPreference=$previous}
  if($LASTEXITCODE){throw "Building $what failed: $(($out|ForEach-Object {"$_"}) -join ' ')"}
  return ("$($out|Select-Object -Last 1)"|ConvertFrom-Json)
}
try{
  $packages=Invoke-Setup @('packages',(Join-Path $game 'base'),(Join-Path $package 'data\decl-recipes.bin'),(Join-Path $stage 'base')) 'the co-op resource packages'
  foreach($g in @($manifest.generated|Where-Object {$_.sha256})){
    if((Get-Sha256Hash (Join-Path $stage $g.path))-ne$g.sha256){throw "Built file differs from the release: $($g.path); nothing changed."}
  }
  foreach($role in 'corvo','emily'){Invoke-Setup @('banks',$game,(Join-Path $package "data\$role-bank.patch"),(Join-Path $stage 'd2coop')) "the $role movement bank"|Out-Null}
  Invoke-Setup @('tables',$game,(Join-Path $package 'data\native-tables.recipe'),(Join-Path $stage 'd2coop')) 'the prop animation tables'|Out-Null
  foreach($g in $manifest.generated){if(!(Test-Path -LiteralPath (Join-Path $stage $g.path))){throw "Missing built file: $($g.path); nothing changed."}}
}catch{Remove-Item -LiteralPath $stage -Recurse -Force -ErrorAction SilentlyContinue;throw}
# Install: back up what is replaced, copy shipped and built files, verify, write the receipt.
$items=@($manifest.files|ForEach-Object {[pscustomobject]@{path=$_.path;source=(Join-Path $package "files/$($_.path)")}})+@($manifest.generated|ForEach-Object {[pscustomobject]@{path=$_.path;source=(Join-Path $stage $_.path)}})
$stamp=Get-Date -Format 'yyyyMMdd-HHmmss';$backup=Join-Path $game "d2coop-backup/$stamp"
New-Item -ItemType Directory -Force $backup|Out-Null
$backedUp=New-Object System.Collections.Generic.List[object];$added=New-Object System.Collections.Generic.List[string];$installed=New-Object System.Collections.Generic.List[object]
foreach($i in $items){
  $target=Join-Path $game $i.path
  if(Test-Path -LiteralPath $target){$dest=Join-Path $backup $i.path;New-Item -ItemType Directory -Force (Split-Path $dest)|Out-Null;Copy-Item -LiteralPath $target $dest;$backedUp.Add(@{path=$i.path;sha256=(Get-Sha256Hash $target)})}
  else{$added.Add($i.path)}
}
foreach($i in $items){$dest=Join-Path $game $i.path;New-Item -ItemType Directory -Force (Split-Path $dest)|Out-Null;Copy-Item -LiteralPath $i.source $dest -Force;$installed.Add(@{path=$i.path;sha256=(Get-Sha256Hash $i.source)})}
foreach($i in $installed){if((Get-Sha256Hash (Join-Path $game $i.path))-ne$i.sha256){throw "Installed file mismatch: $($i.path) (run Uninstall.cmd to roll back)"}}
Remove-Item -LiteralPath $stage -Recurse -Force
@{installed=(Get-Date).ToString('o');version=$manifest.version;candidate=$manifest.candidate;backup="d2coop-backup/$stamp";backedUp=@($backedUp);added=@($added);files=@($installed)}|ConvertTo-Json -Depth 6|Set-Content -LiteralPath $receiptPath -Encoding UTF8
"Installed Dishonored 2 co-op $($manifest.version): $($installed.Count) files; originals backed up in d2coop-backup\$stamp"
