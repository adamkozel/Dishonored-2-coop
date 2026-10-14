$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$exe="$root/work/asset-game/Dishonored2.exe"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Another native operation remains active'}
 [ordered]@{pid=$PID;operation='DoppelgangerVisualInstall';started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 foreach($f in(Get-Content "$PSScriptRoot/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file differs'}}
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
  if(!$p){continue}
  if($p.Path-ne$exe-or[Math]::Abs(($p.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Isolated process identity differs'}
  Stop-Process -Id $p.Id;if(!$p.WaitForExit(15000)){throw 'Isolated game did not exit'}
 }
 & "$lab/Build-D2DoppelgangerVisualAssets.ps1" -Install *> "$PSScriptRoot/doppelganger-visual-install-v423.log"
 if(!$?){throw 'Dedicated clone asset installation failed'}
 & "$PSScriptRoot/Deploy-SourceClock.ps1"
 if(!$?){throw 'Isolated deployment failed'}
 'Cosmetic clone assets423 installed only in physical lab, protected saves retained'
}finally{
 if($owned){[ordered]@{pid=$PID;operation='DoppelgangerVisualInstall';ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()
}
