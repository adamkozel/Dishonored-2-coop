param([ValidateSet('host','client')][string]$Source='host',[Parameter(Mandatory)][ValidatePattern('^[a-z0-9-]+$')][string]$Take)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$out=Join-Path $PSScriptRoot $Take
$failure=$null;$restored=$false
try {
 & "$PSScriptRoot/Test-NativeGrenadeScene-v53.ps1" -Source $Source -Take $Take
} catch {$failure=$_.Exception.Message} finally {
 if(Test-Path -LiteralPath $out){Copy-Item -LiteralPath $PSCommandPath -Destination "$out/reader-regression-wrapper.ps1"}
 try {
  & "$root/work/PrepareShadowSword.ps1" *> "$out/reader-checkpoint-restore.log"
  if(!$?){throw 'Checkpoint preparation failed'}
  foreach($r in 'host','client'){
   $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
   $p=Get-Process -Id $l.pid
   if($p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Unexpected isolated process'}
   $raw=& "$root/work/build/Release/d2_runtime_probe.exe" $l.pid --player-grenade
   if($LASTEXITCODE){throw 'Grenade restoration read failed'}
   $item=$raw|ConvertFrom-Json;if($item.quantity-or$item.equipped-or$item.started){throw 'Grenade acquisition survives checkpoint'}
  }
  foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){
   if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}
  }
  $restored=$true
 } catch {$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;checkpointRestored=$restored;source=$Source;scope='Source equip/unequip and crouch reader regression with mandatory final checkpoint restoration'}|ConvertTo-Json|Set-Content "$out/reader-regression-result.json"
}
if($failure){throw $failure}
