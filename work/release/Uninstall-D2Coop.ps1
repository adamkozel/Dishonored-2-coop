param([string]$GameDirectory,[switch]$KeepBackup)
# Rolls back an installation made by Install-D2Coop.ps1 using its receipt (Windows PowerShell 5.1 or 7):
# removes added files, restores every backed-up original byte-identically and verifies the restored hashes.
# The executable and steam_api64.dll are never touched.
$ErrorActionPreference='Stop'
function Get-Sha256Hash($p){(Get-FileHash -LiteralPath $p -Algorithm SHA256).Hash}
function Find-Dishonored2{
  $steam=$null;foreach($k in 'HKCU:\Software\Valve\Steam','HKLM:\SOFTWARE\WOW6432Node\Valve\Steam','HKLM:\SOFTWARE\Valve\Steam'){
    $v=Get-ItemProperty -Path $k -ErrorAction SilentlyContinue;if($v){if($v.SteamPath){$steam=$v.SteamPath}elseif($v.InstallPath){$steam=$v.InstallPath}};if($steam){break}}
  if(!$steam){throw 'Steam was not found. Run: Uninstall.cmd "<your Dishonored2 folder>"'}
  $libraries=@($steam.Replace('/','\'));$vdf=Join-Path $steam 'steamapps\libraryfolders.vdf'
  if(Test-Path -LiteralPath $vdf){foreach($m in [regex]::Matches((Get-Content -LiteralPath $vdf -Raw),'"path"\s+"([^"]+)"')){$libraries+=$m.Groups[1].Value.Replace('\\','\')}}
  foreach($l in $libraries|Select-Object -Unique){$d=Join-Path $l 'steamapps\common\Dishonored2';if(Test-Path -LiteralPath (Join-Path $d 'd2coop-install-receipt.json')){return $d}}
  throw 'No co-op installation was found in your Steam libraries. Run: Uninstall.cmd "<your Dishonored2 folder>"'
}
if(!$GameDirectory){$GameDirectory=Find-Dishonored2;"Co-op installation found: $GameDirectory"}
$game=(Resolve-Path -LiteralPath $GameDirectory).Path
if(Get-Process -Name Dishonored2 -ErrorAction SilentlyContinue){throw 'Close Dishonored 2 first; nothing changed.'}
$receiptPath=Join-Path $game 'd2coop-install-receipt.json'
if(!(Test-Path -LiteralPath $receiptPath)){throw 'No co-op install receipt here; nothing to roll back.'}
$receipt=Get-Content -LiteralPath $receiptPath -Raw|ConvertFrom-Json
$backup=Join-Path $game $receipt.backup
foreach($b in $receipt.backedUp){$src=Join-Path $backup $b.path;if(!(Test-Path -LiteralPath $src)-or(Get-Sha256Hash $src)-ne$b.sha256){throw "Backup missing or changed: $($b.path); nothing changed."}}
foreach($a in $receipt.added){$t=Join-Path $game $a;if(Test-Path -LiteralPath $t){Remove-Item -LiteralPath $t -Force}}
foreach($b in $receipt.backedUp){Copy-Item -LiteralPath (Join-Path $backup $b.path) (Join-Path $game $b.path) -Force}
foreach($b in $receipt.backedUp){if((Get-Sha256Hash (Join-Path $game $b.path))-ne$b.sha256){throw "Restore mismatch: $($b.path)"}}
foreach($a in $receipt.added){if(Test-Path -LiteralPath (Join-Path $game $a)){throw "Added file still present: $a"}}
# Remove the co-op data folder if the package created it and it is now empty
# (runtime status files written by the mod in d2coop are removed with it).
$data=Join-Path $game 'd2coop';if((Test-Path -LiteralPath $data)-and($receipt.added|Where-Object {$_ -like 'd2coop/*'})){Get-ChildItem -LiteralPath $data -Force|Where-Object {$_.Name -like 'coop-status.json*'}|Remove-Item -Force;if(!(Get-ChildItem -LiteralPath $data -Force)){Remove-Item -LiteralPath $data -Force}}
Remove-Item -LiteralPath $receiptPath -Force
if(!$KeepBackup){Remove-Item -LiteralPath $backup -Recurse -Force;$root=Join-Path $game 'd2coop-backup';if(!(Get-ChildItem -LiteralPath $root -Force)){Remove-Item -LiteralPath $root -Force}}
"Rolled back: $(@($receipt.added).Count) added files removed, $(@($receipt.backedUp).Count) originals restored."
