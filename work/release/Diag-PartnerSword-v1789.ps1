param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='diag-partner-sword-v1789a')
# 1789 (#159): diag-pairing-stream-v1788b: the Corvo partner's shadow source failed only its sword quantity term (1214ic
# sourceValidBits 128: the native ply_sword entry quantity is not 1), while the Corvo host loading the same save passes.
# This samples the client's sword entries (--player-inventory: every ply_sword item and its quantity) after LoadGame 80
# and before arming, then each 2 s for 30 s after the product controller arms. Both reload SLOT81 after.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=200){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};$null}
function Swords($r){$p=(Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json).pid
 $inv=& "$lab/bin/d2_runtime_probe.exe" $p '--player-inventory'|ConvertFrom-Json
 [ordered]@{character=$inv.character;swords=@($inv.items|Where-Object{"$($_.name)"-match'sword'}|ForEach-Object{[ordered]@{name=$_.name;quantity=$_.quantity;type=$_.type;handle=$_.handle}})}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $rows=[Collections.Generic.List[object]]::new()
 $rows.Add([ordered]@{at='before';host=(Swords host);client=(Swords client)})
 C client 'LoadGame 80'
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 700;$st=S client;if($st.localGameLogicState-in8,15){I client A 300}}while(($st.localGameLogicState-ne1-or$st.localPendingGameLogicState)-and$w.ElapsedMilliseconds-lt120000)
 Start-Sleep 3;$rows.Add([ordered]@{at='loaded';host=(Swords host);client=(Swords client)})
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 foreach($q in 'host','client'){C $q 'setviewpos -33 -153 68.1 90 0'}
 $sw=[Diagnostics.Stopwatch]::StartNew();while($sw.ElapsedMilliseconds-lt30000){Start-Sleep 2;$x=S client '.shadow-stream.json'
  $rows.Add([ordered]@{at="armed+$($sw.ElapsedMilliseconds)";product=(S client '.product.json').result;sourceValidBits=$x.sourceValidBits;client=(Swords client)})}
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
