from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
s=(N/'Test-DoppelgangerFx-v453.ps1').read_text(encoding='utf-8-sig')
def replace(old,new):
 global s
 assert old in s, old[:100]
 s=s.replace(old,new)
replace('[int]$Pitch=30)', '[int]$Pitch=30,[switch]$RetainWorldForNextTake,[switch]$ExpectVisualReuse)')
replace("$visualStart=S host '.doppelganger-visual.json';", """$cloneStart=S client '.doppelganger-world.json';$sequenceFloor=[uint64](($cloneStart.history.sequence|Measure-Object -Maximum).Maximum)
 $epochStart=@{host=(S host).localEpoch;client=(S client).localEpoch}
 $epochStart|ConvertTo-Json|Set-Content "$out/world-start.json"
 $visualStart=S host '.doppelganger-visual.json';""")
replace('$cloneCaptured=$false;$cloneSceneCaptured=$false;$sceneNext=2900', '$cloneCaptured=$false;$cloneSceneCaptured=$false;$sceneNext=2900;$followNext=14000')
replace("  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'", """  if($w.ElapsedMilliseconds-ge$followNext){
   $followNext=$w.ElapsedMilliseconds+500;$following=S host '.doppelganger-visual.json'
   if($following.visible-and$following.transform.Count-eq12){
    $xyz=@([double]$following.transform[0],([double]$following.transform[1]-4),([double]$following.transform[2]+1.66))
    $position=($xyz|ForEach-Object {$_.ToString('F4',[Globalization.CultureInfo]::InvariantCulture)})-join' '
    C host "setviewpos $position 90 0"
    $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Observer camera follows source-owned clone for disappearance inspection';position=$xyz})
   }
  }
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'""")
replace('$sentBySeq=@{};foreach($record in $sent)', '$sent=@($sent|Where-Object sequence -gt $sequenceFloor);$accepted=@($accepted|Where-Object sequence -gt $sequenceFloor)\n $sentBySeq=@{};foreach($record in $sent)')
replace('$lastVisual.created-$visualStart.created-ne1', '$lastVisual.created-$visualStart.created-ne$(if($ExpectVisualReuse){0}else{1})')
replace(" $checks.Add('One isolated cosmetic clone follows native219-joint pose and interpolated root, then hides on source retirement')", """ if($ExpectVisualReuse-and(!$visualStart.handle-or$visualStart.handle-ne$lastVisual.handle)){throw 'Second cast did not retain the exact cosmetic actor handle'}
 $checks.Add('One isolated cosmetic clone follows native219-joint pose and interpolated root, then hides on source retirement')
 if($ExpectVisualReuse){$checks.Add('Second cast reuses the same cosmetic actor with no second model creation')}""")
replace(' if($dirty){try{', """ if($dirty-and$RetainWorldForNextTake-and!$failure){
  $end=@{host=(S host).localEpoch;client=(S client).localEpoch};$end|ConvertTo-Json|Set-Content "$out/world-retained.json"
  if($end.host-ne$epochStart.host-or$end.client-ne$epochStart.client){$failure='Unexpected world replacement before reuse'}
 }
 if($dirty-and(!$RetainWorldForNextTake-or$failure)){try{""")
replace("passed=(!$failure);failure=$failure;source='client'", "passed=(!$failure);failure=$failure;retainedWorld=($RetainWorldForNextTake-and!$failure);expectedVisualReuse=$ExpectVisualReuse.IsPresent;source='client'")
replace('no effect, AI or damage;', 'native cosmetic FX but no AI or damage;')
target=N/'Test-DoppelgangerFx-v454.ps1';assert not target.exists();target.write_text(s,encoding='utf-8')
wrapper=r'''$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$retained=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Native operation still active'}
 [ordered]@{pid=$PID;operation='DoppelgangerFxReuse454';started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 foreach($take in 'doppelganger-fx-v455','doppelganger-fx-v456'){if(Test-Path "$root/work/natural-moveset/$take"){throw 'Unique take required'}}
 & "$root/work/natural-moveset/Test-DoppelgangerFx-v454.ps1" -Take doppelganger-fx-v455 -RetainWorldForNextTake
 if(!$?){throw 'First native cast failed'};$retained=$true
 & "$root/work/natural-moveset/Test-DoppelgangerFx-v454.ps1" -Take doppelganger-fx-v456 -ExpectVisualReuse
 if(!$?){throw 'Second native cast failed'};$retained=$false
 $a=Get-Content "$root/work/natural-moveset/doppelganger-fx-v455/world-retained.json" -Raw|ConvertFrom-Json
 $b=Get-Content "$root/work/natural-moveset/doppelganger-fx-v456/world-start.json" -Raw|ConvertFrom-Json
 if($a.host-ne$b.host-or$a.client-ne$b.client){throw 'World changed between casts'}
 'PASS_TWO_CASTS_SAME_COSMETIC_ACTOR'
}finally{
 if($owned){
  if($retained){
   $second="$root/work/natural-moveset/doppelganger-fx-v456/result.json"
   $restored=$false
   if(Test-Path $second){$r=Get-Content $second -Raw|ConvertFrom-Json;$restored=@($r.checks|Where-Object {$_ -like 'Both isolated checkpoints restore*'}).Count-eq1}
   if(!$restored){& "$root/work/PrepareShadowSword.ps1"}
  }
  [ordered]@{pid=$PID;operation='DoppelgangerFxReuse454';ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
'''
(W/'Run-DoppelgangerFx-v455.ps1').write_text(wrapper,encoding='utf-8')
print('Prepared bounded same-world reuse and close disappearance view; DLL unchanged')
