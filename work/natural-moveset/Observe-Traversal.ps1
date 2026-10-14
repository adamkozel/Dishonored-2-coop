param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-traversal-v1',[ValidateSet('host','client')][string]$Source='host')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$pids=@{}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt8;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native read $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 $dst=if($Source-eq'host'){'client'}else{'host'};C $dst 'setviewpos -33.5 -153 68.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/camera.json"
 # The native scene contains a desk top at approximately (-31,-146,67.25),
 # observed in the installed world entity data. No replacement geometry.
 foreach($trial in @(@('desk-center',-31.2,-148.5,90),@('desk-left',-32,-148.5,90),@('desk-right',-30.3,-148.5,90))){
  I $Source;C $Source "setviewpos $($trial[1]) $($trial[2]) 68.1 $($trial[3]) 0";Start-Sleep -Milliseconds 900
  I $Source A 1300 22000;$watch=[Diagnostics.Stopwatch]::StartNew()
  while($watch.ElapsedMilliseconds-lt2800){
   $state=Q $Source '--player-states';$midnight=$null
   if(@($state.active|Where-Object {$_.id-eq8-or$_.id-eq9-or$_.id-eq53}).Count){$midnight=Q $Source '--midnight'}
   $rows.Add([pscustomobject]@{source=$Source;trial=$trial[0];ms=$watch.ElapsedMilliseconds;state=$state;midnight=$midnight});Start-Sleep -Milliseconds 20
  }
  I $Source;if((Q $Source '--health' 1).current-ne56){throw 'Health changed during traversal observation'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 15|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;scope='Native controls and fixture placement in existing Dunwall geometry; read-only state and Midnight observations. No traversal binding, animation writes or world asset modification.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};$rows|Group-Object trial|ForEach-Object {[pscustomobject]@{Trial=$_.Name;States=(@($_.Group.state.active|ForEach-Object {$_.id.ToString()+':'+$_.type}|Sort-Object -Unique)-join', ')}}|Format-Table -AutoSize
