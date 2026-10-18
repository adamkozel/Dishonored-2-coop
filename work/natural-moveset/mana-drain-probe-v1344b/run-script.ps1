param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mana-drain-probe-v1344b',[string[]]$Cases=@('host:windblast','client:farreach','client:darkvision','client:domino','client:doppelganger','client:mesmerize','client:shadowwalk'))
# 1344b: arm and disarm the product controller around each power (1344a: the checkpoint reload refused during the
# armed shared reset), and cast a Corvo windblast first as the control. Cases are role:power.
# 1344 (#11, mana_elixir): which native powers spend mana for this role? In coverage-v1330e/g Emily's live mana
# (itemUse.manaNow) stayed at 100 after Doppelganger and Mesmerize casts that the partner presented, while Corvo's windblast
# spent it to 50. For each power: grant it by name (1214cm), press LT for the power's own hold, trace the live mana every
# 100 ms for 4 s, then reload the lab checkpoint and restore the note baseline. Product controller armed. Read-only otherwise.
$Cases=@($Cases|ForEach-Object {$_-split','}|Where-Object {$_})
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
$names=@{farreach='models/powers/grapple/grapple.inventoryitem';darkvision='models/powers/darkvision/darkvision.inventoryitem';domino='models/powers/phantasmallink/phantasmal_link.inventoryitem';doppelganger='models/powers/doppelganger/doppelganger.inventoryitem';mesmerize='models/powers/mesmerize/mesmerize.inventoryitem';shadowwalk='models/powers/shadow_walk/shadow_walk.inventoryitem';windblast='models/powers/windblast/windblast.inventoryitem';blink='models/powers/blink/blink.inventoryitem'}
$holds=@{farreach=1200;darkvision=250;domino=1400;doppelganger=800;mesmerize=1200;shadowwalk=250;windblast=250;blink=1200}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function Mana($r){try{[double](Get-Content "$root/work/lab-instances/$r/coop-status.json.observer-camera.json" -Raw|ConvertFrom-Json).itemUse.manaNow}catch{$null}}
$rows=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($case in $Cases){$Role,$p=$case-split':';$row=[ordered]@{role=$Role;power=$p}
  foreach($r in 'host','client'){& "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $aw=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(Get-Content "$root/work/lab-instances/host/coop-status.json.product.json" -Raw|ConvertFrom-Json).result-eq'armed'-and(Get-Content "$root/work/lab-instances/client/coop-status.json.product.json" -Raw|ConvertFrom-Json).result-eq'armed'}while(!$ok-and$aw.ElapsedMilliseconds-lt60000);if(!$ok){throw 'Product controller not armed'};Start-Sleep -Seconds 3
  C $Role 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 600
  C $Role "coop_grant_named $($names[$p])";Start-Sleep -Milliseconds 1800
  $row.manaBefore=Mana $Role;& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 20 -Quick|Out-Null
  & "$lab/Send-D2Input.ps1" -Role $Role -LeftTrigger 255 -DurationMs $holds[$p]|Out-Null
  $trace=[Collections.Generic.List[object]]::new();$w=[Diagnostics.Stopwatch]::StartNew()
  while($w.ElapsedMilliseconds-lt4000){$trace.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;mana=(Mana $Role)});Start-Sleep -Milliseconds 100}
  $row.trace=@($trace);$row.manaMin=($trace|Where-Object {$_.mana-ne$null}|Measure-Object mana -Minimum).Minimum;$rows.Add($row)
  # Disarm as the coverage harnesses do before the checkpoint reload.
  foreach($r in 'host','client'){foreach($cmd in 'coop_observer_camera off','coop_product_auto off','coop_movement_resume off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off','coop_checkpoint_sync off'){try{C $r $cmd}catch{}}};$dw=[Diagnostics.Stopwatch]::StartNew()
  do{Start-Sleep -Milliseconds 500;$busy=@('host','client'|Where-Object {try{(Get-Content "$root/work/lab-instances/$_/coop-status.json.reset.json" -Raw|ConvertFrom-Json).enabled}catch{$false}}).Count}while($busy-and$dw.ElapsedMilliseconds-lt30000)
  & "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"
  & "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration-$p" *>> "$out/note-restoration.log"}
}finally{
 foreach($r in 'host','client'){try{C $r 'coop_product_auto off'}catch{}}
 $rows|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
foreach($r in $rows){"$($r.role) $($r.power): mana before $($r.manaBefore), minimum $($r.manaMin)"}
