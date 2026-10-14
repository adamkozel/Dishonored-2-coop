param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-lower-floor-landing-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$pids=@{};$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  & "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -DurationMs 100|Out-Null
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Baseline health changed'}
 }
 C client 'setviewpos -30 -150.5 68.1 212 0'
 foreach($spot in @(@{name='guardfloor';x=-45.6;y=-145.852;eye=62.8;floor=61.004},@{name='searchfloor';x=-42.59;y=-131.78;eye=63.4;floor=61.604})){
  C host ('setviewpos '+$spot.x+' '+$spot.y+' '+$spot.eye+' 0 0');Start-Sleep -Milliseconds 1000
  C host ('setviewpos '+$spot.x+' '+$spot.y+' '+($spot.eye+6.8)+' 0 0');$watch=[Diagnostics.Stopwatch]::StartNew()
  while($watch.ElapsedMilliseconds-lt3200){
   $n=Q host '--player-states';$scene=$null;if(@($n.active|Where-Object id -eq 4).Count){$scene=Q host '--midnight'}
   $rows.Add([pscustomobject]@{spot=$spot;ms=$watch.ElapsedMilliseconds;native=$n;midnight=$scene})
   if($n.position[2]-lt($spot.floor-.3)){throw 'Established floor boundary exceeded'};Start-Sleep -Milliseconds 20
  }
  if((Q host '--health' 1).current-ne56){throw 'Unexpected fall damage'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 18|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;scope='Native gravity only, bounded 6.8m nominal lifts over previously surveyed floors. No animation, physics, health or save writes.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};'LOWER_FLOOR_LANDINGS_OBSERVED'
