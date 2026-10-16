param([Parameter(Mandatory)][string]$Take)
# Restores the lab after Route-Mission-v1300 -HoldAtEnd: the same restoration
# block and checks as the harness (sessions and mirrors off, GameOver cleared,
# the Dunwall fixture and note inventory restored, gameplay/health/transport and
# the protected files checked), recorded as restoration-after-hold.json.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!(Test-Path $out)){throw "No take $Take"}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$pids=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$pids[$r]=$l.pid}
 $errors=[Collections.Generic.List[string]]::new();$dirty=$true
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';C $r 'coop_mission_end_mirror off';C $r 'coop_gameflow_mirror off';C $r 'coop_kiscule_mirror off'}catch{}}
   # Product mode armed checkpoint sync: a shared death would block the fixture restore.
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq13){I $r A 400;Start-Sleep 5}}catch{}}
   foreach($r in 'host','client'){try{C $r 'coop_checkpoint_sync off';C $r 'coop_usable_sync off';C $r 'coop_player_health_hold off'}catch{}}
   $w=[Diagnostics.Stopwatch]::StartNew()
   do{$ok=$true;foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-notin 1,2,6,13-or$s.localPendingGameLogicState){$ok=$false;if($s.localGameLogicState-eq5){I $r X 3000}elseif($s.localGameLogicState-eq3){I $r Right 250;Start-Sleep -Milliseconds 300;I $r A 600}elseif($s.localGameLogicState-eq14){I $r A 600}else{I $r A 300}}}catch{$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 1500}}while(!$ok-and$w.ElapsedMilliseconds-lt120000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq13){I $r A 400}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}

 Save "$out/restoration-after-hold.json" @{passed=(!$errors.Count);errors=@($errors)}
 Get-Content "$out/restoration-after-hold.json"
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
