[CmdletBinding()]
param(
 [Parameter(Mandatory)][ValidateSet('Deploy','Prepare','GrenadeNetwork','CrouchedGrenadeNetwork','GrenadeAirNetwork','StunNetwork','GrenadeResume','GrenadeObserve','GrenadeAirObserve','PotionObserve','MesmerizeObserve','MesmerizeContext','SwarmObserve','SwarmNetwork','SwarmContextObserve','SwarmContextNetwork','SwarmWorldNetwork','SwarmWorldObserve','DominoObserve','DominoFinalContext','DominoContextTarget','DominoContext','DominoHolsteredFinalObserve','DominoHolsteredObserve','DominoCrouchFinalNetwork','DominoCrouchFinalObserve','DominoCrouchSelectionObserve','DominoCrouchNetwork','DominoCrouchObserve','DominoNetworkObserve','DominoWorld','DominoPartner','DominoCancel','DominoFinalSelect','DominoDeselect','DominoLinksNetwork','DominoLinksObserve','DominoNetwork','MesmerizeEmptyAirObserve','MesmerizeAirNetwork','MesmerizeNetwork','MesmerizeWorld','MesmerizeWorldNetwork','MesmerizePartner','MesmerizeNpc','MesmerizeLinks','MesmerizeCrouchNetwork','MesmerizeCrouchObserve','MesmerizeLifetime','MesmerizeCancel','MesmerizeResume','MesmerizeVisual','GrenadeProp')][string]$Operation,
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
 [switch]$CrouchedCancel,
 [switch]$HolsteredObserve,
 [switch]$Holstered,
 [switch]$DominoTarget,
 [switch]$AirObserve,
 [switch]$AirReleaseObserve,
 [switch]$AirEntryObserve,
 [ValidateSet('entry','release')][string]$AirCase='entry',
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
 if($Operation-in@('GrenadeNetwork','CrouchedGrenadeNetwork','GrenadeAirNetwork','StunNetwork','GrenadeResume','GrenadeObserve','GrenadeAirObserve','PotionObserve','MesmerizeObserve','MesmerizeContext','SwarmObserve','SwarmNetwork','SwarmContextObserve','SwarmContextNetwork','SwarmWorldNetwork','SwarmWorldObserve','DominoObserve','DominoFinalContext','DominoContextTarget','DominoContext','DominoHolsteredFinalObserve','DominoHolsteredObserve','DominoCrouchFinalNetwork','DominoCrouchFinalObserve','DominoCrouchSelectionObserve','DominoCrouchNetwork','DominoCrouchObserve','DominoNetworkObserve','DominoWorld','DominoPartner','DominoCancel','DominoFinalSelect','DominoDeselect','DominoLinksNetwork','DominoLinksObserve','DominoNetwork','MesmerizeEmptyAirObserve','MesmerizeAirNetwork','MesmerizeNetwork','MesmerizeWorld','MesmerizeWorldNetwork','MesmerizePartner','MesmerizeNpc','MesmerizeLinks','MesmerizeCrouchNetwork','MesmerizeCrouchObserve','MesmerizeLifetime','MesmerizeCancel','MesmerizeResume','MesmerizeVisual','GrenadeProp')-and!$Take){throw 'A unique evidence take is required'}
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
  'MesmerizePartner'{& "$root/work/natural-moveset/Test-MesmerizePartner-v95.ps1" -Take $Take -Pitch $Pitch}
  'MesmerizeNpc'{& "$root/work/natural-moveset/Test-MesmerizeNpc-v104.ps1" -Take $Take -Pitch $Pitch}
  'MesmerizeContext'{& "$root/work/natural-moveset/Test-MesmerizeContextNetwork-v153.ps1" -Take $Take -Pitch $Pitch -Stance $Stance -Move $Move -Holstered:$Holstered}
  'DominoNetwork'{& "$root/work/natural-moveset/Test-DominoNetwork-v196.ps1" -Take $Take -Pitch $Pitch}
  'DominoPartner'{& "$root/work/natural-moveset/Test-DominoPartner-v211.ps1" -Take $Take -Pitch $Pitch}
  'DominoFinalSelect'{& "$root/work/natural-moveset/Test-DominoFinalRecovery-v248.ps1" -Take $Take -Pitch $Pitch -AimGuard}
  'DominoDeselect'{& "$root/work/natural-moveset/Test-DominoNativeDeselect-v235.ps1" -Take $Take -Pitch $Pitch -AimGuard}
  'DominoCancel'{& "$root/work/natural-moveset/Test-DominoNativeCancel-v233.ps1" -Take $Take -Pitch $Pitch -AimGuard}
  'DominoLinksNetwork'{& "$root/work/natural-moveset/Test-DominoLinks-v225.ps1" -Take $Take -Pitch $Pitch -AimGuard}
  'DominoLinksObserve'{& "$root/work/natural-moveset/Observe-NativeDominoLinks-v221.ps1" -Take $Take -Pitch $Pitch -AimGuard}
  'DominoWorld'{& "$root/work/natural-moveset/Test-DominoWorld-v218.ps1" -Take $Take -Pitch $Pitch}
  'DominoNetworkObserve'{& "$root/work/natural-moveset/Observe-NativeDominoTargets-v199.ps1" -Take $Take -Pitch $Pitch -AimGuard}
  'DominoCrouchNetwork'{& "$root/work/natural-moveset/Test-DominoCrouchNetwork-v250.ps1" -Take $Take -Pitch $Pitch}
  'DominoCrouchFinalNetwork'{& "$root/work/natural-moveset/Test-DominoCrouchFinalNetwork-v256.ps1" -Take $Take -Pitch $Pitch}
  'DominoFinalContext'{& "$root/work/natural-moveset/Test-DominoFinalContextNetwork-v264.ps1" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered}
  'DominoContextTarget'{& "$root/work/natural-moveset/Test-DominoTargetContextNetwork-v262.ps1" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered -Move $Move}
  'DominoContext'{& "$root/work/natural-moveset/Test-DominoContextNetwork-v258.ps1" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered -Move $Move}
  'DominoHolsteredFinalObserve'{& "$root/work/natural-moveset/Observe-NativeDominoHolsteredFinal-v263.ps1" -Take $Take -Pitch $Pitch -Stance $Stance}
  'DominoHolsteredObserve'{& "$root/work/natural-moveset/Observe-NativeDominoHolstered-v253.ps1" -Take $Take -Pitch $Pitch -Stance $Stance}
  'DominoCrouchFinalObserve'{& "$root/work/natural-moveset/Observe-NativeDominoCrouchFinal-v251.ps1" -Take $Take -Pitch $Pitch}
  'DominoCrouchSelectionObserve'{& "$root/work/natural-moveset/Observe-NativeDominoCrouchSelection-v249.ps1" -Take $Take -Pitch $Pitch}
  'DominoCrouchObserve'{& "$root/work/natural-moveset/Observe-NativeDominoCrouch-v245.ps1" -Take $Take -Pitch $Pitch}
  'SwarmNetwork'{& "$root/work/natural-moveset/Test-SwarmNetwork-v270.ps1" -Take $Take -Pitch $Pitch}
  'SwarmContextNetwork'{& "$root/work/natural-moveset/Test-SwarmContextNetwork-v276.ps1" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered -Move $Move}
  'SwarmWorldNetwork'{& "$root/work/natural-moveset/Test-SwarmWorldNetwork-v290.ps1" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered -Move $Move}
  'SwarmWorldObserve'{& "$root/work/natural-moveset/Observe-NativeSwarmWorld-v289.ps1" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered}
  'SwarmContextObserve'{& "$root/work/natural-moveset/Observe-NativeSwarmContext-v275.ps1" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered}
  'SwarmObserve'{& "$root/work/natural-moveset/Observe-NativeSwarm-v265.ps1" -Take $Take -Pitch $Pitch}
  'DominoObserve'{& "$root/work/natural-moveset/Observe-NativeDomino-v189.ps1" -Take $Take -Pitch $Pitch -AimGuard:$DominoTarget}
  'MesmerizeEmptyAirObserve'{if($AirCase-eq'entry'){& "$root/work/natural-moveset/Observe-NativeMesmerizeEmptyAirEntry-v172.ps1" -Take $Take -Pitch $Pitch}else{& "$root/work/natural-moveset/Observe-NativeMesmerizeEmptyAirRelease-v172.ps1" -Take $Take -Pitch $Pitch}}
  'MesmerizeAirNetwork'{& "$root/work/natural-moveset/Test-MesmerizeAirNetwork-v178.ps1" -Take $Take -Pitch $Pitch -AirCase $AirCase -Holstered:$Holstered}
  'MesmerizeCrouchNetwork'{if($Move){& "$root/work/natural-moveset/Test-MesmerizeCrouchMoving-v143.ps1" -Take $Take -Pitch $Pitch}else{& "$root/work/natural-moveset/Test-MesmerizeCrouchNetwork-v133.ps1" -Take $Take -Pitch $Pitch}}
  'MesmerizeCrouchObserve'{& "$root/work/natural-moveset/Observe-NativeMesmerizeCrouch-v130.ps1" -Take $Take -Pitch $Pitch}
  'MesmerizeLifetime'{& "$root/work/natural-moveset/Test-MesmerizeLifetime-v128.ps1" -Take $Take -Pitch $Pitch}
  'MesmerizeCancel'{if($CrouchedCancel){& "$root/work/natural-moveset/Test-MesmerizeCrouchCancel-v135.ps1" -Take $Take -Pitch $Pitch}else{& "$root/work/natural-moveset/Test-MesmerizeCancel-v122.ps1" -Take $Take -Pitch $Pitch}}
  'MesmerizeResume'{& "$root/work/natural-moveset/Test-MesmerizeResume-v119.ps1" -Take $Take -Pitch $Pitch -Interrupt $Interrupt}
  'MesmerizeLinks'{if($Interrupt-ne'none'){& "$root/work/natural-moveset/Test-MesmerizeLinksInterrupt-v115.ps1" -Take $Take -Pitch $Pitch -Interrupt $Interrupt}else{& "$root/work/natural-moveset/Test-MesmerizeLinks-v106.ps1" -Take $Take -Pitch $Pitch}}
  'MesmerizeWorldNetwork'{if($Interrupt-ne'none'){& "$root/work/natural-moveset/Test-MesmerizeTargets-v90.ps1" -Take $Take -Pitch $Pitch -Interrupt $Interrupt}else{& "$root/work/natural-moveset/Test-MesmerizeEyesNetwork-v86.ps1" -Take $Take -Pitch $Pitch -Move $Move}}
  'MesmerizeWorld'{& "$root/work/natural-moveset/Observe-NativeMesmerizeWorld-v75.ps1" -Take $Take -Pitch $Pitch}
  'MesmerizeNetwork'{& "$root/work/natural-moveset/Test-MesmerizeNetwork-v74.ps1" -Take $Take -Pitch $Pitch}
  'MesmerizeObserve'{if($AirEntryObserve){& "$root/work/natural-moveset/Observe-NativeMesmerizeAirEntry-v162.ps1" -Take $Take -Pitch $Pitch}elseif($AirReleaseObserve){& "$root/work/natural-moveset/Observe-NativeMesmerizeAirRelease-v157.ps1" -Take $Take -Pitch $Pitch}else{if($AirObserve){& "$root/work/natural-moveset/Observe-NativeMesmerizeAir-v150.ps1" -Take $Take -Pitch $Pitch}else{if($HolsteredObserve){& "$root/work/natural-moveset/Observe-NativeMesmerizeHolstered-v141.ps1" -Take $Take -Pitch $Pitch -Stance $Stance}else{& "$root/work/natural-moveset/Observe-NativeMesmerize-v72.ps1" -Take $Take -Pitch $Pitch}}}}
  'GrenadeProp'{& "$root/work/natural-moveset/Test-GrenadeProp-v55.ps1" -Character $(if($Source-eq'host'){'corvo'}else{'emily'}) -Take $Take -Closeup:$Closeup}
 }
 if(!$?){throw 'Native lab operation failed'}
}finally{
 if($owned){[ordered]@{pid=$PID;operation=$Operation;take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()}
 $mutex.Dispose()
}









