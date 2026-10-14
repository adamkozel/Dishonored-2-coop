[CmdletBinding()]
param(
 [Parameter(Mandatory)][ValidateSet('Deploy','Prepare','GrenadeNetwork','GrenadeResume','GrenadeProp')][string]$Operation,
 [ValidateSet('host','client')][string]$Source='host',
 [ValidatePattern('^[a-z0-9-]+$')][string]$Take,
 [ValidateRange(-15,45)][int]$Pitch=0,
 [switch]$DeployFirst,
 [switch]$Closeup,
 [switch]$ValidateMechanism,
 [switch]$FrontView,
 [switch]$ValidateFx,
 [switch]$InstallGrenadeFx,
 [ValidateSet('none','receiver-stop','source-pause','receiver-pause')][string]$Interrupt='none'
)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65')
$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Another serialized D2 lab operation is still running, including its cleanup. Wait for its final process exit.'}
 if($Operation-in@('GrenadeNetwork','GrenadeResume','GrenadeProp')-and!$Take){throw 'A unique evidence take is required'}
 [ordered]@{pid=$PID;operation=$Operation;source=$Source;take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 if($InstallGrenadeFx){
  $exe=Join-Path $root 'work/asset-game/Dishonored2.exe'
  foreach($role in 'host','client'){
   $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
   $p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
   if(!$p){continue}
   if($p.Path-ne$exe-or[Math]::Abs(($p.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Isolated process identity changed'}
   Stop-Process -Id $p.Id
   if(!$p.WaitForExit(15000)){throw 'Isolated game still running; asset files protected'}
  }
  & "$root/outputs/d2-coop-lab/Build-D2GrenadeFxAssets.ps1" -Install
  if(!$?){throw 'Isolated effect installation failed'}
  $DeployFirst=$true
 }
 if($DeployFirst-and$Operation-ne'Deploy'){& "$PSScriptRoot/Deploy-SourceClock.ps1";if(!$?){throw 'Deployment failed'}}
 switch($Operation){
  'Deploy'{& "$PSScriptRoot/Deploy-SourceClock.ps1"}
  'Prepare'{& "$root/work/PrepareShadowSword.ps1"}
  'GrenadeNetwork'{& "$root/work/natural-moveset/Test-GrenadeNetwork-v56.ps1" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism:$ValidateMechanism -FrontView:$FrontView -ValidateFx:$ValidateFx -Closeup:$Closeup -Interrupt $Interrupt}
  'GrenadeResume'{& "$root/work/natural-moveset/Test-GrenadeResume-v59.ps1" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism -FrontView -ValidateFx -Closeup -Interrupt $Interrupt}
  'GrenadeProp'{& "$root/work/natural-moveset/Test-GrenadeProp-v55.ps1" -Character $(if($Source-eq'host'){'corvo'}else{'emily'}) -Take $Take -Closeup:$Closeup}
 }
 if(!$?){throw 'Native lab operation failed'}
}finally{
 if($owned){[ordered]@{pid=$PID;operation=$Operation;take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()}
 $mutex.Dispose()
}
