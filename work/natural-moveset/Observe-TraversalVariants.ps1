param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-traversal-variants-v1',[switch]$CrouchOnly,[switch]$Unarmed)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt8;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native read $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 $dst=if($Source-eq'host'){'client'}else{'host'};C $dst 'setviewpos -35.5 -150 68.1 50 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/camera.json"
 $cases=@(@('near',-147.75,90,$false,22000),@('edge',-147.35,90,$false,22000),@('far',-149.2,90,$false,32767),@('stationary-edge',-147.2,90,$false,0),@('crouch-normal',-148.5,90,$true,22000),@('crouch-near',-147.75,90,$true,22000),@('desk-back',-144.5,270,$false,22000))
 if($CrouchOnly){$cases=@($cases|Where-Object {$_[3]})}
 if($Unarmed){I $Source X 1500;Start-Sleep -Milliseconds 2800;if((Q $Source '--player-sword').equipped){throw 'Sword did not holster'}}
 foreach($case in $cases){
  I $Source;if((Q $Source '--player-states').stance-eq1){I $Source B 180;Start-Sleep -Milliseconds 650}
  C $Source "setviewpos -32 $($case[1]) 68.1 $($case[2]) 0";Start-Sleep -Milliseconds 800
  if($case[3]){I $Source B 180;Start-Sleep -Milliseconds 850}
  I $Source A 1300 $case[4];$watch=[Diagnostics.Stopwatch]::StartNew()
  while($watch.ElapsedMilliseconds-lt2600){
   $state=Q $Source '--player-states';$midnight=$null
   if(@($state.active|Where-Object {$_.id-eq8-or$_.id-eq9-or$_.id-eq53}).Count){$midnight=Q $Source '--midnight'}
   $rows.Add([pscustomobject]@{source=$Source;trial=$case[0];ms=$watch.ElapsedMilliseconds;state=$state;midnight=$midnight});Start-Sleep -Milliseconds 15
  }
  if((Q $Source '--health' 1).current-ne56){throw 'Health changed'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;if((Q $r '--player-states').stance-eq1){I $r B 180;Start-Sleep -Milliseconds 650};C $r 'setviewpos -33.5 -153 68.1 0 0';if($Unarmed-and$r-eq$Source-and!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2200};C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 15|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;scope='Native input variations at the existing Dunwall desk, read-only scene/state observation; no animation or world asset modification.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};"OBSERVED_TRAVERSAL_VARIANTS $($rows.Count) samples"
