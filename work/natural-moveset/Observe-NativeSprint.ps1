param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-sprint-observation-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -Button $button -DurationMs $ms|Out-Null}
try {
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Unexpected game identity'}
  I $role;C $role 'coop_movement_bank off';C $role 'coop_shadow_stream off'
  $status=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
  if($status.localPlayerStance-eq1){I $role 0 0 B 180;Start-Sleep -Milliseconds 1000}
  foreach($trial in @(@('run',0,32767,'Neutral'),@('sprint-forward',0,32767,'LeftThumb'),@('sprint-left',-16000,28000,'LeftThumb'),@('sprint-right',16000,28000,'LeftThumb'))){
   C $role 'setviewpos -34.3 -154.8 68.1 0 0';Start-Sleep -Milliseconds 600
   I $role $trial[1] $trial[2] $trial[3] 650
   $timer=[Diagnostics.Stopwatch]::StartNew()
   while($timer.ElapsedMilliseconds-lt1100){
    $raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --player-states
    if(!$LASTEXITCODE){$rows.Add([pscustomobject]@{role=$role;trial=$trial[0];ms=$timer.ElapsedMilliseconds;native=($raw|ConvertFrom-Json)})}
    Start-Sleep -Milliseconds 25
   }
   I $role;Start-Sleep -Milliseconds 500
  }
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){try{I $role}catch{}}
 $rows|ConvertTo-Json -Depth 8|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;scope='Read-only native state observations during bounded real controller inputs; presence and activation are separate evidence.';dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash}|ConvertTo-Json|Set-Content "$out/result.json"
}
$rows|Group-Object role,trial|ForEach-Object {[pscustomobject]@{Trial=$_.Name;States=(@($_.Group.native.active|ForEach-Object {$_.id.ToString()+':'+$_.type}|Sort-Object -Unique)-join', ')}}|Format-Table -AutoSize
if($failure){throw $failure}
