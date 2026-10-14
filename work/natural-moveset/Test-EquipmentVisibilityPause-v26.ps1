param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='equipment-visibility-pause-v26')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$paused=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function Status($r,$suffix=''){Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$id=0){for($i=0;$i-lt12;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe $r $mode"}
function SaveState($src,$dst,$label){$rows.Add([pscustomobject]@{source=$src;observer=$dst;label=$label;sourceLogic=(Status $src).localGameLogicState;targetLogic=(Status $dst).localGameLogicState;bank=(Status $dst '.movement.json');equipment=(Status $dst '.shadow-equipment.json');sword=(Q $dst '--avatar-sword' $handles[$dst])})}
try{
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid;$handles[$r]=(Status $r).probeActorHandle}
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C $dst 'coop_movement_bank on';Start-Sleep -Milliseconds 1200
  I $src X 1500;Start-Sleep -Milliseconds 3000;SaveState $src $dst 'holstered'
  if(!(Q $dst '--avatar-sword' $handles[$dst]).explicitHidden-or(Q $dst '--avatar-sword' $handles[$dst]).equipped-or!(Status $dst '.shadow-equipment.json').remoteDriven){throw 'Remote equipment did not holster'}
  I $src Start 180;$paused=$src;Start-Sleep -Milliseconds 700;SaveState $src $dst 'source-paused'
  if((Status $src).localGameLogicState-ne2-or(Status $dst '.movement.json').active-or(Status $dst '.shadow-equipment.json').active){throw 'Pause did not retire equipment and body'}
  $before=Status $dst '.movement.json';C $dst 'coop_movement_bank on';Start-Sleep -Milliseconds 350
  if((Status $dst '.movement.json').active-or(Status $dst '.movement.json').applied-ne$before.applied){throw 'Paused rearm was accepted'}
  $checks.Add("${src}: pause retires body/equipment and refuses paused rearm")
  I $src A 180;Start-Sleep -Milliseconds 850;$paused=$null;SaveState $src $dst 'resumed'
  $eq=Status $dst '.shadow-equipment.json';$sword=Q $dst '--avatar-sword' $handles[$dst]
  if((Status $dst '.movement.json').active-or$eq.active-or$eq.restoring-or$eq.lost-or$eq.visibilityRefused-or$sword.explicitHidden-or!$sword.equipped-or!$sword.started){throw 'Original native sword not restored or retired action replayed'}
  $checks.Add("${src}: resume restores original visible native sword without replaying movement")
  I $src X 650;Start-Sleep -Milliseconds 1300
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
  $checks.Add("${src}: native first-person resumes normally and health stays unchanged")
 }
}catch{$failure=$_.Exception.Message}finally{
 if($paused){try{if((Status $paused).localGameLogicState-eq2){I $paused A 180;Start-Sleep -Milliseconds 700}}catch{}}
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 1200};C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 10|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Pause cleanup while remote clone sword is holstered, both source roles. Full death/loading/session lifecycle remains separate.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"EQUIPMENT_PAUSE_VALIDATED $($checks.Count) checks"
