param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='diag-pairing-stream-v1788a',[ValidateSet(80,81)][int]$HostSlot=81,[ValidateSet(80,81)][int]$ClientSlot=80)
# 1788 (#159): why the shadow action stream stays unready in a new pairing (partner-windblast-v1787a: Corvo-Corvo at the
# Dunwall fixture, both products armed and banks active, but the stream never ready). The lab instances load the
# pairing (-HostSlot/-ClientSlot: 81 the role's own character, 80 the other), arm the product controller, and the
# status of both games is snapshotted every 2 s for 40 s (shadow stream, product, movement, character). Then both
# reload SLOT81 (PrepareShadowSword) with the note inventory restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=200){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};$null}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 if($HostSlot-ne81){C host "LoadGame $HostSlot"};if($ClientSlot-ne81){C client "LoadGame $ClientSlot"}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 700;$ok=$true;foreach($r in 'host','client'){$st=S $r;if($st.localGameLogicState-ne1-or$st.localPendingGameLogicState){$ok=$false;if($st.localGameLogicState-in8,15){I $r A 300}}}}while(!$ok-and$w.ElapsedMilliseconds-lt120000)
 if(!$ok){throw 'The pairing fixture did not load'}
 Start-Sleep 3;foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 foreach($q in 'host','client'){C $q 'setviewpos -33 -153 68.1 90 0'}
 $rows=[Collections.Generic.List[object]]::new();$sw=[Diagnostics.Stopwatch]::StartNew()
 while($sw.ElapsedMilliseconds-lt40000){$row=[ordered]@{ms=$sw.ElapsedMilliseconds}
  foreach($r in 'host','client'){$x=S $r '.shadow-stream.json';$row[$r]=[ordered]@{product=(S $r '.product.json').result;character=(S $r '.character.json');bank=(S $r '.movement.json').active;stream=$x}}
  $rows.Add([pscustomobject]$row);Start-Sleep 2}
 $rows|ConvertTo-Json -Depth 8|Set-Content "$out/series.json"
}catch{$failure=$_.Exception.Message;@{message=$failure}|ConvertTo-Json|Set-Content "$out/failure.json"}
finally{
 if($owned){
  foreach($r in 'host','client'){try{C $r 'coop_product_auto off'}catch{}}
  try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";& "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration" *> "$out/note-restoration.log"}catch{$failure="$failure; restore: $($_.Exception.Message)"}
  $errors=@();foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors+="$r not restored"}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors+='Protected file changed'}}
  @{passed=(!$errors.Count);errors=$errors}|ConvertTo-Json|Set-Content "$out/restoration.json"
  $mutex.ReleaseMutex()}
 $mutex.Dispose()}
if($failure){throw $failure}
