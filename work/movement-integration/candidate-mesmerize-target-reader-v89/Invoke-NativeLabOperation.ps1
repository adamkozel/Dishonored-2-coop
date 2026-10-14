[CmdletBinding()]
param(
 [Parameter(Mandatory)][ValidateSet('Deploy','Prepare','GrenadeNetwork','CrouchedGrenadeNetwork','GrenadeAirNetwork','StunNetwork','GrenadeResume','GrenadeObserve','GrenadeAirObserve','PotionObserve','MesmerizeObserve','MesmerizeNetwork','MesmerizeWorld','MesmerizeWorldNetwork','MesmerizeVisual','GrenadeProp')][string]$Operation,
 [ValidateSet('host','client')][string]$Source='host',
 [ValidatePattern('^[a-z0-9-]+$')][string]$Take,
 [ValidateRange(-15,45)][int]$Pitch=0,
 [switch]$DeployFirst,
 [switch]$Closeup,
 [switch]$ValidateMechanism,
 [switch]$FrontView,
 [switch]$ValidateFx,
 [switch]$InstallGrenadeFx,
 [switch]$InstallMesmerizeVisual,
 [ValidateSet('standing','crouched')][string]$Stance='crouched',
 [ValidateRange(0,15000)][int]$Move=0,
 [ValidateSet('health','mana')][string]$PotionKind='health',
 [ValidateSet('None','B','X','Y','NativeHealth','NativeMana')][string]$WheelButton='None',
 [switch]$PotionNetwork,
 [switch]$PotionStatistics,
 [switch]$PotionDrainMana,
 [switch]$FacingWall,
 [ValidateSet('none','receiver-stop','source-pause','receiver-pause')][string]$Interrupt='none'
)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65')
$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Another serialized D2 lab operation is still running, including its cleanup. Wait for its final process exit.'}
 if($Operation-in@('GrenadeNetwork','CrouchedGrenadeNetwork','GrenadeAirNetwork','StunNetwork','GrenadeResume','GrenadeObserve','GrenadeAirObserve','PotionObserve','MesmerizeObserve','MesmerizeNetwork','MesmerizeWorld','MesmerizeWorldNetwork','MesmerizeVisual','GrenadeProp')-and!$Take){throw 'A unique evidence take is required'}
 [ordered]@{pid=$PID;operation=$Operation;source=$Source;take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 if($InstallGrenadeFx-or$InstallMesmerizeVisual){
  $exe=Join-Path $root 'work/asset-game/Dishonored2.exe'
  foreach($role in 'host','client'){
   $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
   $p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
   if(!$p){continue}
   if($p.Path-ne$exe-or[Math]::Abs(($p.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Isolated process identity changed'}
   Stop-Process -Id $p.Id
   if(!$p.WaitForExit(15000)){throw 'Isolated game still running; asset files protected'}
  }
  if($InstallGrenadeFx){& "$root/outputs/d2-coop-lab/Build-D2GrenadeFxAssets.ps1" -Install;if(!$?){throw 'Grenade assets failed'}}
  if($InstallMesmerizeVisual){& "$root/outputs/d2-coop-lab/Build-D2MesmerizeVisualAssets.ps1" -Install}
  if(!$?){throw 'Isolated effect installation failed'}
  $DeployFirst=$true
 }
 if($DeployFirst-and$Operation-ne'Deploy'){& "$PSScriptRoot/Deploy-SourceClock.ps1";if(!$?){throw 'Deployment failed'}}
 switch($Operation){
  'Deploy'{& "$PSScriptRoot/Deploy-SourceClock.ps1"}
  'Prepare'{& "$root/work/PrepareShadowSword.ps1"}
  'GrenadeNetwork'{& "$root/work/natural-moveset/Test-GrenadeNetwork-v56.ps1" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism:$ValidateMechanism -FrontView:$FrontView -ValidateFx:$ValidateFx -Closeup:$Closeup -Interrupt $Interrupt}
  'CrouchedGrenadeNetwork'{& "$root/work/natural-moveset/Test-CrouchedGrenadeNetwork-v62.ps1" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism -ValidateFx -FrontView -Move $Move}
  'GrenadeAirNetwork'{& "$root/work/natural-moveset/Test-GrenadeAirNetwork-v63.ps1" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism -ValidateFx -FrontView -Move $Move}
  'StunNetwork'{& "$root/work/natural-moveset/Test-StunNetwork-v61.ps1" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism -ValidateFx}
  'GrenadeResume'{& "$root/work/natural-moveset/Test-GrenadeResume-v59.ps1" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism -FrontView -ValidateFx -Closeup -Interrupt $Interrupt}
  'GrenadeObserve'{& "$root/work/natural-moveset/Observe-NativeGrenadeContexts-v60.ps1" -Source $Source -Take $Take -Pitch $Pitch -Stance $Stance -Move $Move -FacingWall:$FacingWall}
  'GrenadeAirObserve'{& "$root/work/natural-moveset/Observe-NativeGrenadeAir-v63.ps1" -Source $Source -Take $Take -Pitch $Pitch -Stance standing -Move $Move}
  'PotionObserve'{& "$root/work/natural-moveset/Observe-NativePotion-v69.ps1" -Source $Source -Take $Take -Kind $PotionKind -WheelButton $WheelButton -Network:$PotionNetwork -Move $Move -ValidateStatistics:$PotionStatistics -DrainMana:$PotionDrainMana}
  'MesmerizeVisual'{& "$root/work/natural-moveset/Test-MesmerizeVisual-v76.ps1" -Take $Take}
  'MesmerizeWorldNetwork'{if($Interrupt-ne'none'){& "$root/work/natural-moveset/Test-MesmerizeTargets-v89.ps1" -Take $Take -Pitch $Pitch -Interrupt $Interrupt}else{& "$root/work/natural-moveset/Test-MesmerizeEyesNetwork-v86.ps1" -Take $Take -Pitch $Pitch -Move $Move}}
  'MesmerizeWorld'{& "$root/work/natural-moveset/Observe-NativeMesmerizeWorld-v75.ps1" -Take $Take -Pitch $Pitch}
  'MesmerizeNetwork'{& "$root/work/natural-moveset/Test-MesmerizeNetwork-v74.ps1" -Take $Take -Pitch $Pitch}
  'MesmerizeObserve'{& "$root/work/natural-moveset/Observe-NativeMesmerize-v72.ps1" -Take $Take -Pitch $Pitch}
  'GrenadeProp'{& "$root/work/natural-moveset/Test-GrenadeProp-v55.ps1" -Character $(if($Source-eq'host'){'corvo'}else{'emily'}) -Take $Take -Closeup:$Closeup}
 }
 if(!$?){throw 'Native lab operation failed'}
}finally{
 if($owned){[ordered]@{pid=$PID;operation=$Operation;take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()}
 $mutex.Dispose()
}
