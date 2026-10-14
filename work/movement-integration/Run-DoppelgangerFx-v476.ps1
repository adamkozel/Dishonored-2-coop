$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$retained=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Native operation still active'}
 [ordered]@{pid=$PID;operation='DoppelgangerFxReuse476';started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 foreach($take in 'doppelganger-fx-v476','doppelganger-fx-v477'){if(Test-Path "$root/work/natural-moveset/$take"){throw 'Unique take required'}}
 & "$root/work/natural-moveset/Test-DoppelgangerFx-v476.ps1" -Take doppelganger-fx-v476 -RetainWorldForNextTake
 if(!$?){throw 'First native cast failed'};$retained=$true
 & "$root/work/natural-moveset/Test-DoppelgangerFx-v476.ps1" -Take doppelganger-fx-v477 -ExpectVisualReuse
 if(!$?){throw 'Second native cast failed'};$retained=$false
 $a=Get-Content "$root/work/natural-moveset/doppelganger-fx-v476/world-retained.json" -Raw|ConvertFrom-Json
 $b=Get-Content "$root/work/natural-moveset/doppelganger-fx-v477/world-start.json" -Raw|ConvertFrom-Json
 if($a.host-ne$b.host-or$a.client-ne$b.client){throw 'World changed between casts'}
 'PASS_TWO_CASTS_SAME_COSMETIC_ACTOR'
}finally{
 if($owned){
  if($retained){
   $second="$root/work/natural-moveset/doppelganger-fx-v477/result.json"
   $restored=$false
   if(Test-Path $second){$r=Get-Content $second -Raw|ConvertFrom-Json;$restored=@($r.checks|Where-Object {$_ -like 'Both isolated checkpoints restore*'}).Count-eq1}
   if(!$restored){& "$root/work/PrepareShadowSword.ps1"}
  }
  [ordered]@{pid=$PID;operation='DoppelgangerFxReuse476';ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
