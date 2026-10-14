param([Parameter(Mandatory)][string]$TakeDir)
# Restores the isolated lab after a harness was stopped before its own cleanup
# (e.g. user interruption). Ordinary native input only: A on load prompts
# (logic 8/15), B on a native modal (logic 12), X hold on a mission intro
# (logic 5); then lab features off, Dunwall fixture and note inventory restored.
# Writes recovery.json into the interrupted take; never rewrites its evidence.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=(Resolve-Path $TakeDir).Path
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$pids=@{};$errors=[Collections.Generic.List[string]]::new();$log=[Collections.Generic.List[object]]::new()
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
try{
 try{$owned=$mutex.WaitOne(5000)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid}
 $w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=-5000;client=-5000}
 do{$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt1500){continue}
   if($s.localGameLogicState-in8,15){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds+2500}}
  Start-Sleep -Milliseconds 500
 }while(!$ready-and$w.ElapsedMilliseconds-lt120000)
 if(!$ready){throw 'Instances did not return to gameplay'}
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_observer_camera off','coop_movement_bank off','coop_shadow_stream off'){try{C $r $cmd}catch{$errors.Add("$r ${cmd}: $($_.Exception.Message)")}}}
 & "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/recovery-restored-setup.json"|Out-Null
 & "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/recovery-note-restoration"|Out-Null
 foreach($r in 'host','client'){$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){$errors.Add("${r}: final gameplay/health/transport not restored")}}
}catch{$errors.Add($_.Exception.Message)}
finally{
 foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
 @{at=(Get-Date).ToString('o');passed=(!$errors.Count);errors=@($errors);log=@($log);note='Lab recovery after an interrupted harness (cleanup did not run).'}|ConvertTo-Json -Depth 6|Set-Content "$out/recovery.json"
 if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()
}
Get-Content "$out/recovery.json" -Raw|ConvertFrom-Json|Select-Object passed,errors|ConvertTo-Json -Compress
