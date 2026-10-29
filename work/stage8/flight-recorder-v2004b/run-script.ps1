param([string]$Take='flight-recorder-v2004a',[int]$RecordSeconds=20)
# Stage 8.2 (#235): the in-DLL flight recorder (candidate flight-v2004) on the Dunwall fixture. Both games record
# their traffic for $RecordSeconds, `coop_flight dump` writes a .cofr per game, and coop-anything's analyzer
# (third_party/coop-anything/flight/flight.py) prints, aligns and checks them for a first divergence. The mode is
# asserted first: two live peers in gameplay. Read-only for the game state.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$pids=@{};$rows=@{}
$py='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe';$flight="$lab/third_party/coop-anything/flight/flight.py"
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$PSScriptRoot/candidate-flight-v2004/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected the flight-v2004 DLL'}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  $s=S $r;if($s.localGameLogicState-ne1){throw "Gameplay required ($r)"};if(!$s.remoteFresh){throw "Transport not fresh ($r)"}}
 Save "$out/identity.json" @{dll=$pin.dll;candidate=$pin.candidate;recordSeconds=$RecordSeconds;pids=$pids}
 $before=@{};foreach($r in 'host','client'){$before[$r]=@(Get-ChildItem "$root/work/lab-instances/$r" -Filter '*.cofr' -ErrorAction SilentlyContinue|ForEach-Object FullName)}
 $sent=@{};foreach($r in 'host','client'){$sent[$r]=(S $r).sent}
 Start-Sleep -Seconds $RecordSeconds
 foreach($r in 'host','client'){C $r 'coop_flight dump'}
 Start-Sleep -Seconds 2
 foreach($r in 'host','client'){
  $new=@(Get-ChildItem "$root/work/lab-instances/$r" -Filter '*.cofr'|Where-Object {$before[$r]-notcontains$_.FullName}|Sort-Object LastWriteTime)
  if($new.Count-ne1){throw "Expected one new dump for $r, found $($new.Count)"}
  Copy-Item $new[0].FullName "$out/$r.cofr";$rows[$r]=@{dump=$new[0].Name;bytes=$new[0].Length;sentDuring=((S $r).sent-$sent[$r])}}
 & $py -I $flight print "$out/host.cofr" --limit 40|Set-Content "$out/print-host.txt"
 & $py -I $flight print "$out/client.cofr" --limit 40|Set-Content "$out/print-client.txt"
 & $py -I $flight align "$out/host.cofr" "$out/client.cofr"|Set-Content "$out/align.txt";$rows.alignExit=$LASTEXITCODE
 & $py -I $flight diverge "$out/host.cofr" "$out/client.cofr" --json|Set-Content "$out/diverge.json";$rows.divergeExit=$LASTEXITCODE
 Save "$out/dumps.json" $rows
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){throw 'Final gameplay/transport not as before'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;dumps=$rows;scope='Flight recorder dumps from both games on the Dunwall fixture, read by the coop-anything analyzer.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
