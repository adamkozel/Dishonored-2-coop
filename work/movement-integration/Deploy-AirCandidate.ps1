$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
& "$root/work/build/Release/d2_movement_bank_tests.exe" "$root/work/natural-moveset/air-banks-v1/corvo-air-bank.bin" "$root/work/natural-moveset/air-banks-v1/emily-air-bank.bin"
if($LASTEXITCODE){throw 'Air banks failed decoding'}
foreach($role in 'host','client'){
 $character=if($role-eq'host'){'emily'}else{'corvo'}
 Copy-Item -LiteralPath "$root/work/natural-moveset/air-banks-v1/$character-air-bank.bin" -Destination "$root/work/lab-instances/$role/coop-status.json.movement.bin" -Force
}
& "$PSScriptRoot/Deploy-SourceClock.ps1"
