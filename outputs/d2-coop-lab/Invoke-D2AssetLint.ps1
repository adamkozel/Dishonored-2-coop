[CmdletBinding()]
param([Parameter(Mandatory)][string]$Path,[switch]$Json)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$tool=Join-Path $root 'work/tools/void-slice-cc58fc7'
$pin=Get-Content (Join-Path $tool 'pin.json') -Raw|ConvertFrom-Json
$exe=Join-Path $tool 'voidslice.exe'
if((Get-FileHash -LiteralPath $exe).Hash-ne$pin.binary_sha256){throw 'Pinned Void Slice executable differs'}
$inputFile=(Get-Item -LiteralPath $Path).FullName
if(!(Test-Path -LiteralPath $inputFile -PathType Leaf)){throw 'One existing asset text file is required'}
if($Json){& $exe lint $inputFile --json}else{& $exe lint $inputFile}
# Preserve native exit status. Warnings (including duplicate array indices)
# still return0; inspect diagnostics. Known native MD6 tuples false-positive.
exit $LASTEXITCODE
