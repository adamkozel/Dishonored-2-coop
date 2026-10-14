param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='turn-corvo-v18-v1',[switch]$Record,[switch]$NativeFeet,[switch]$CloseCamera,[switch]$AuthoredFeet,[switch]$Unarmed,[switch]$Crouched)
if($AuthoredFeet){$NativeFeet=$true}
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$ms=100,$trigger=0,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftX $x -LeftY $y -RightTrigger $trigger|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Probe($r,$mode,$id=0){for($i=0;$i-lt12;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 10};throw "Native read $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_power_fixture off'
  C $r 'coop_probe_follow exact';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
  if((Probe $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
  if((S $r).localPlayerStance-eq1){I $r 0 0 180 0 B;Start-Sleep -Milliseconds 900}
 }
 if($CloseCamera){C $target 'setviewpos -30.5 -150.5 68.1 212 0'}else{C $target 'setviewpos -28.5 -151.2 68.1 195 0'};C $Source 'setviewpos -34.5 -153 68.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch $(if($CloseCamera){16}else{7}) -Evidence "$out/observer-camera.json"
 Copy-Item "$PSScriptRoot/crouch-turn-banks-v1/$character-crouch-turn-bank.bin" "$root/work/lab-instances/$target/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $target ('coop_movement_feet '+$(if($AuthoredFeet){'authored'}elseif($NativeFeet){'on'}else{'off'}));C $target 'coop_movement_bank on'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$m=S $target '.movement.json';if($m.active-and$m.version-eq19-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne19-or$m.weight-lt.9-or[bool]$m.nativeFeet-ne[bool]$NativeFeet){throw 'Movement bank not ready'};$checks.Add('Native Steam stream and v19 full-stance turn bank ready')
 if($Crouched){I $Source 0 0 180 0 B;Start-Sleep -Milliseconds 1400;if((S $target '.movement.json').stanceMix-lt.99){throw 'Native crouch did not reach full-body stance'};$checks.Add('Replicated native crouch stance ready')}
 if($Unarmed){
  & "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 1500|Out-Null;Start-Sleep -Milliseconds 3200
  $m=S $target '.movement.json'
  if($m.armedMix-gt.01-or(Probe $Source '--player-sword').equipped-or(Probe $target '--avatar-sword' $handles[$target]).equipped){throw 'Native swords did not holster'}
  $checks.Add('Native hands-free presentation ready')
 }
 & "$lab/Send-D2Input.ps1" -Role $Source -RightX 28000 -DurationMs 1800|Out-Null
 $w.Restart();while($w.ElapsedMilliseconds-lt2200){$m=S $target '.movement.json';$rows.Add($m);Start-Sleep -Milliseconds 50}
 $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$target] --skeleton-final $handles[$target] (S $target).bodyPublicationAddress
 if(!$LASTEXITCODE){$raw|Set-Content "$out/pose.json"}
 $m|ConvertTo-Json|Set-Content "$out/movement.json"
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;if((S $r).localPlayerStance-eq1){I $r 0 0 180 0 B;Start-Sleep -Milliseconds 1000};C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 5|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Diagnostic only; no movement acceptance'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};$m|Select-Object active,applied,refused,kneeCorrections,kneeReject,kneeOffsetError
