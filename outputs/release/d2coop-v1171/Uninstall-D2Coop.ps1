param([Parameter(Mandatory)][string]$GameDirectory,[switch]$KeepBackup)
# Rolls back an installation made by Install-D2Coop.ps1 using its receipt:
# removes added files, restores every backed-up original byte-identically and
# verifies the restored hashes. The executable and steam_api64.dll are never touched.
$ErrorActionPreference='Stop'
$game=(Resolve-Path -LiteralPath $GameDirectory).Path
function H($p){(Get-FileHash -LiteralPath $p -Algorithm SHA256).Hash}
$receiptPath=Join-Path $game 'd2coop-install-receipt.json'
if(!(Test-Path -LiteralPath $receiptPath)){throw 'No co-op install receipt here; nothing to roll back.'}
$receipt=Get-Content -LiteralPath $receiptPath -Raw|ConvertFrom-Json
$backup=Join-Path $game $receipt.backup
foreach($b in $receipt.backedUp){$src=Join-Path $backup $b.path;if(!(Test-Path -LiteralPath $src)-or(H $src)-ne$b.sha256){throw "Backup missing or changed: $($b.path); nothing changed."}}
foreach($a in $receipt.added){$t=Join-Path $game $a;if(Test-Path -LiteralPath $t){Remove-Item -LiteralPath $t -Force}}
foreach($b in $receipt.backedUp){Copy-Item -LiteralPath (Join-Path $backup $b.path) (Join-Path $game $b.path) -Force}
foreach($b in $receipt.backedUp){if((H (Join-Path $game $b.path))-ne$b.sha256){throw "Restore mismatch: $($b.path)"}}
foreach($a in $receipt.added){if(Test-Path -LiteralPath (Join-Path $game $a)){throw "Added file still present: $a"}}
Remove-Item -LiteralPath $receiptPath -Force
if(!$KeepBackup){Remove-Item -LiteralPath $backup -Recurse -Force;$root=Join-Path $game 'd2coop-backup';if(!(Get-ChildItem -LiteralPath $root -Force)){Remove-Item -LiteralPath $root -Force}}
"Rolled back: $($receipt.added.Count) added files removed, $($receipt.backedUp.Count) originals restored."
