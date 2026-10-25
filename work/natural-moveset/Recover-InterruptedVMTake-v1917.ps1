param([Parameter(Mandatory)][ValidatePattern('^[a-z0-9-]+$')][string]$Take)
# 1917: restore after a Route-MissionVM take stopped before its finally block (campaign-bunker-vm-v1917a was stopped in
# its VM sync, because its route would cast with the Timepiece in hand). The same steps as the harness's own
# restoration: product features and sessions off where a game runs, the VM role stopped, the lab pair relaunched
# (Deploy-SourceClock), both instances checked in gameplay and connected, protected saves checked. Writes
# recovery.json into the take; the take stays as an interrupted take.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
if(!(Test-Path $out)){throw 'Take folder missing'};if(Test-Path "$out/recovery.json"){throw 'Already recovered'}
$errors=[Collections.Generic.List[string]]::new();$steps=[Collections.Generic.List[string]]::new()
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{$owned=$mutex.WaitOne(5000)}catch [Threading.AbandonedMutexException]{$owned=$true;$steps.Add('abandoned mutex taken')}
if(!$owned){throw 'Native operation active'}
try{
 $hl=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json
 if(Get-Process -Id $hl.pid -ErrorAction SilentlyContinue){
  foreach($cmd in 'coop_product_auto off','coop_player_health_hold off','coop_session off'){try{& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null;$steps.Add("host $cmd")}catch{$errors.Add("host $cmd`: $($_.Exception.Message)")}}}
 try{Stop-D2LabVMRole -Role client|Out-Null;$steps.Add('vm role stopped')}catch{$steps.Add("vm stop: $($_.Exception.Message)")}
}finally{$mutex.ReleaseMutex();$mutex.Dispose()}
try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null;$steps.Add('lab pair relaunched')}catch{$errors.Add("restore: $($_.Exception.Message)")}
foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
[ordered]@{interrupted=$true;reason='stopped in its VM sync: its route would cast with the Timepiece in hand (campaign-curator-vm-v1917a showed the left hand can hold another item); re-run with routes that select the power first';steps=@($steps);passed=(!$errors.Count);errors=@($errors)}|ConvertTo-Json|Set-Content "$out/recovery.json"
Get-Content "$out/recovery.json";if($errors.Count){throw ($errors-join '; ')}
