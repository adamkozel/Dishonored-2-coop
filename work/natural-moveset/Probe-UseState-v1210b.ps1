param([string]$Take='native-use-state-v1210b',[ValidateSet('host','client')][string]$Role='host',[string]$Window='ld_rich_small_01_sndctr_4')
# Stage 3 lever_button_crank_wheel / key_use - native source question: does an
# ordinary native use of a usable (the Dunwall window handle, as 832b/1201)
# enter a native player state with a duration, like LootObject47 for pickups?
# Read-only --watch-player-states during two uses (open, close). Dunwall
# fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$notes=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='Neutral',[int]$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $e=@((Q $Role '--entities').entities|Where-Object name -eq $Window);if($e.Count-ne1){throw "Window $Window not found"};$wp=$e[0].position
 $dirty=$true
 foreach($cmd in 'coop_usable_sync off','coop_door_sync off'){C $Role $cmd}
 C $Role ("setviewpos {0:F3} {1:F3} 67.0 0 0"-f($wp[0]-1.28),$wp[1]);Start-Sleep -Milliseconds 1500
 & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Role -X $wp[0] -Y $wp[1] -Z 68.0 -Evidence "$out/aim.json"
 $watch=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$Role],'--watch-player-states','20000') -WindowStyle Hidden -RedirectStandardOutput "$out/player-states.json" -RedirectStandardError "$out/player-states.stderr" -PassThru
 Start-Sleep -Milliseconds 800
 foreach($want in 'open','close'){
  $before=Q $Role '--usable' $e[0].handle;$t=Get-Date
  $changed=$false;for($k=0;$k-lt3-and!$changed;$k++){I $Role X 130;I $Role;Start-Sleep -Milliseconds 1500;$u=Q $Role '--usable' $e[0].handle;$changed=($u.current-ne$before.current-or$u.pending-ne-1)}
  $notes.Add(@{want=$want;before=$before.current;after=$u.current;changed=$changed;at=$t.ToString('o')})
  if($want-eq'open'){& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Role -X $wp[0] -Y $wp[1] -Z 68.65 -Evidence "$out/aim-close.json"}
 }
 if(!$watch.WaitForExit(30000)){Stop-Process -Id $watch.Id}
 $states=Get-Content "$out/player-states.json" -Raw|ConvertFrom-Json
 $seen=@{};foreach($x in $states.records){foreach($a in $x.active){$k="$($a.id)";if(!$seen[$k]){$seen[$k]=@{firstMs=$x.ms;vtable=$a.vtableRva;count=0}};$seen[$k].count++}}
 Save "$out/state-census.json" @{uses=@($notes);states=$seen;records=$states.records.Count}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;uses=@($notes);scope='Native use-state probe on the Dunwall window handle; fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
