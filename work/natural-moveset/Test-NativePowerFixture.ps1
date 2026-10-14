param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-power-fixture-v1',[ValidateSet('host','client')][string[]]$Sources=@('host','client'),[ValidateSet('cancel','expire','pause')][string]$Mode='cancel')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function InputState($r,$button='Neutral') {& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs 180|Out-Null}
function Probe($r,$mode,$id=0){for($i=0;$i-lt12;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native read $r $mode"}
function State($r,$suffix='.power-fixture.json'){return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}
function AwaitResult($r,$result,$ms=6000){$w=[Diagnostics.Stopwatch]::StartNew();do{$s=State $r;$rows.Add([pscustomobject]@{role=$r;ms=$w.ElapsedMilliseconds;state=$s});if($s.result-eq$result){return $s};if($s.lost){throw "Power fixture cleanup lost: $($s.result)"};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt$ms);throw "Power fixture deadline: $($s.result)"}
$protected=Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json
try{
 foreach($f in $protected){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save differs'}}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;InputState $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_power_fixture off'}
 foreach($r in $Sources){
  $peer=if($r-eq'host'){'client'}else{'host'};$before=Probe $r '--player-power';$inventory=Probe $r '--player-inventory';$sword=Probe $r '--player-sword';$health=Probe $r '--health' 1
  $before|ConvertTo-Json -Depth 5|Set-Content "$out/$r-before.json";$inventory|ConvertTo-Json -Depth 6|Set-Content "$out/$r-inventory-before.json"
  if($before.quantity-ne0-or$before.equipped-or$before.started){throw 'Baseline power is not unavailable'}
  $s=State $r;C $r 'coop_power_fixture 20000';Start-Sleep -Milliseconds 400;$refused=State $r
  if($refused.active-or$refused.refused-ne($s.refused+1)){throw 'Unisolated fixture did not refuse'};$checks.Add("${r}: grant refused outside NPC-isolated scope")
  C $r 'coop_movement_fixture 60000';Start-Sleep -Milliseconds 600;C $r 'coop_power_fixture 20000';$ready=AwaitResult $r 'native_power_ready'
  $power=Probe $r '--player-power';$power|ConvertTo-Json -Depth 5|Set-Content "$out/$r-enabled.json"
  if($power.quantity-ne1-or!$power.equipped-or!$power.started-or(Probe $peer '--player-power').quantity-ne0){throw 'Power not exclusively enabled on source'}
  $checks.Add("${r}: existing native starting power enabled and selected, peer unchanged")
  C $r 'coop_power_fixture 20000';Start-Sleep -Milliseconds 400;$duplicate=State $r
  if($duplicate.started-ne$ready.started-or$duplicate.remainingMs-ge$ready.remainingMs-or$duplicate.refused-ne($ready.refused+1)){throw 'Duplicate renewed fixture'}
  $checks.Add("${r}: duplicate refuses without renewing lease")
  if($Mode-eq'cancel'){C $r 'coop_power_fixture off'}
  elseif($Mode-eq'pause'){InputState $r Start;Start-Sleep -Milliseconds 1400;$paused=State $r;if(!$paused.active-or!$paused.restoring){throw 'Pause did not retain cleanup ownership'};InputState $r A;Start-Sleep -Milliseconds 1200}
  $restored=AwaitResult $r 'quantity_and_selection_restored' $(if($Mode-eq'expire'){26000}else{6000})
  $after=Probe $r '--player-power';$after|ConvertTo-Json -Depth 5|Set-Content "$out/$r-after.json"
  if($after.quantity-ne0-or$after.equipped-or$after.started-or$restored.restored-ne($ready.restored+1)){throw 'Quantity or selection not restored'}
  $afterInventory=Probe $r '--player-inventory';$afterInventory|ConvertTo-Json -Depth 6|Set-Content "$out/$r-inventory-after.json"
  $beforeItems=@($inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});$afterItems=@($afterInventory.items|ForEach-Object {"$($_.name):$($_.quantity)"})
  if(!$beforeItems.Count-or(Compare-Object $beforeItems $afterItems)){throw 'Other inventory quantities changed'}
  if((Probe $r '--health' 1).current-ne$health.current-or[bool](Probe $r '--player-sword').equipped-ne[bool]$sword.equipped){throw 'Health or sword changed'}
  $checks.Add("${r}: $Mode restores power quantity/selection, all inventory quantities, health and sword draw state")
  C $r 'coop_movement_fixture off'
 }
 foreach($f in $protected){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}};$checks.Add('Protected original/save hashes unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{InputState $r;C $r 'coop_power_fixture off';Start-Sleep -Milliseconds 1200;C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 7|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;checks=$checks;mode=$Mode;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Native inventory quantity/equip/unequip in isolated Dunwall. Does not certify power casting, remote presentation or native UI discovery metadata restoration. No saves written.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_POWER_FIXTURE $($checks.Count) checks"
