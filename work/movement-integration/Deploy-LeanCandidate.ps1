$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
& "$root/work/build/Release/d2_movement_bank_tests.exe" "$root/work/natural-moveset/lean-banks-v1/corvo-lean-bank.bin" "$root/work/natural-moveset/lean-banks-v1/emily-lean-bank.bin"
if($LASTEXITCODE){throw 'Lean banks failed decoding'}
foreach($role in 'host','client'){
 $character=if($role-eq'host'){'emily'}else{'corvo'}
 Copy-Item -LiteralPath "$root/work/natural-moveset/lean-banks-v1/$character-lean-bank.bin" -Destination "$root/work/lab-instances/$role/coop-status.json.movement.bin" -Force
}
& "$PSScriptRoot/Deploy-SourceClock.ps1"
