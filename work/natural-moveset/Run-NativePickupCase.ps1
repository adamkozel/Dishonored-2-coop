[CmdletBinding()]
param([ValidateSet('host','client')][string]$Role='host',
 [Parameter(Mandatory)][ValidatePattern('^[a-z0-9-]+$')][string]$Take,
 [ValidateSet('Full','Cancel')][string]$Kind='Full',[switch]$TraceContact,
 [ValidatePattern('^[a-z0-9-]+\.json$')][string]$BankManifest='carry-pickup-bank-manifest-v1.json')
$ErrorActionPreference='Stop'
if(Test-Path -LiteralPath (Join-Path $PSScriptRoot $Take)){throw 'Take already exists'}
if($Kind-eq'Cancel'-and$TraceContact){throw 'Cancellation case does not poll contact'}
# Each case starts from the existing protected-save isolated fixture. Never
# reuse the held body or mutated victim state from the preceding capture.
& (Join-Path $PSScriptRoot '../PrepareShadowSword.ps1')
$script=if($Kind-eq'Cancel'){'Record-NativePickupCancel.ps1'}else{'Record-NativeCarrierPickup.ps1'}
& (Join-Path $PSScriptRoot $script) -Role $Role -Take $Take -Paired -TraceContact:$TraceContact -BankManifest $BankManifest
