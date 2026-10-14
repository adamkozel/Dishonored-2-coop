param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-fall-spot-v1',[switch]$SkipWindow)
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
 foreach($spot in @(@{name='window';x=-17.8;y=-148.95;eye=68.1;minimum=60.5},@{name='guardfloor';x=-45.6;y=-145.852;eye=62.9;minimum=59.5},@{name='searchfloor';x=-42.59;y=-131.78;eye=63.3;minimum=60.0})){
  if($SkipWindow-and$spot.name-eq'window'){continue}
  C host ('setviewpos '+$spot.x+' '+$spot.y+' '+$spot.eye+' 0 0');$watch=[Diagnostics.Stopwatch]::StartNew()
  while($watch.ElapsedMilliseconds-lt2100){
   $n=Q host '--player-states';$health=Q host '--health' 1;$rows.Add([pscustomobject]@{spot=$spot;ms=$watch.ElapsedMilliseconds;native=$n;health=$health})
   if($n.position[2]-lt$spot.minimum-or$health.current-ne56){C host 'setviewpos -34 -153 68.1 0 0';throw 'Survey exceeded bounded floor or changed health'}
   Start-Sleep -Milliseconds 20
  }
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 15|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;scope='Bounded native floor survey at map-entity-derived positions; no save or health writes.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};'FLOOR_SURVEY_COMPLETED'
