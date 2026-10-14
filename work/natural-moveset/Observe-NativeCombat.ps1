param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-combat-timing-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$pids=@{};$probe=Join-Path $root 'work/build/Release/d2_runtime_probe.exe'
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$trigger=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -RightTrigger $trigger|Out-Null}
function ReadTestStatus($path){for($attempt=0;$attempt-lt8;$attempt++){try{return(Get-Content -LiteralPath $path -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{if($attempt-eq7){throw};Start-Sleep -Milliseconds 15}}}
function B($r){ReadTestStatus "$root/work/lab-instances/$r/coop-status.json"}
function F($r){ReadTestStatus "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json"}
function Health($r){$raw=& $probe $pids[$r] --health 1;if($LASTEXITCODE){throw 'Health read failed'};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'
  if((B $r).localGameLogicState-ne1-or(Health $r).current-ne56){throw 'Clean live fixture required'}
 }
 foreach($r in 'host','client'){
  foreach($side in 'host','client'){C $side 'coop_movement_fixture off';C $side 'coop_movement_fixture 60000'}
  Start-Sleep -Milliseconds 500
  foreach($side in 'host','client'){if(!(F $side).ready-or(F $side).heldReferences-ne6){throw 'Movement fixture unavailable'}}
  $other=if($r-eq'host'){'client'}else{'host'};C $other 'setviewpos -30.5 -149 68.1 233.13 0'
  foreach($trial in 'tap','held','combo','crouch-tap','block','crouch-block','air-tap'){
   $stance=if($trial.StartsWith('crouch')){1}else{0};if((B $r).localPlayerStance-ne$stance){I $r B 180;Start-Sleep -Milliseconds 1100}
   C $r 'setviewpos -33.5 -153 68.1 0 0';Start-Sleep -Milliseconds 700
   $events=if($trial-match'block'){@(@(0,'RightShoulder',1400,0))}elseif($trial-eq'air-tap'){@(@(0,'A',180,0),@(150,'Neutral',180,255))}elseif($trial-eq'combo'){@(@(0,'Neutral',180,255),@(550,'Neutral',180,255),@(1100,'Neutral',180,255))}elseif($trial-eq'held'){@(@(0,'Neutral',1500,255))}else{@(@(0,'Neutral',180,255))}
   # Force an object array even for a single event.
   if($events.Count-eq4-and$events[0]-is[int]){$events=,@($events)}
   $next=0;$w=[Diagnostics.Stopwatch]::StartNew()
   while($w.ElapsedMilliseconds-lt2900){
    if((B $r).localGameLogicState-ne1-or!(F $r).ready-or!(F $other).ready){throw 'Native observation fixture retired'}
    if($next-lt$events.Count-and$w.ElapsedMilliseconds-ge$events[$next][0]){$e=$events[$next];I $r $e[1] $e[2] $e[3];$next++}
    $raw=& $probe $pids[$r] --player-states
    if(!$LASTEXITCODE){$native=$raw|ConvertFrom-Json;$scenes=& $probe $pids[$r] --midnight;if(!$LASTEXITCODE){$rows.Add([pscustomobject]@{role=$r;trial=$trial;ms=$w.ElapsedMilliseconds;native=$native;midnight=($scenes|ConvertFrom-Json)})}}
    Start-Sleep -Milliseconds 15
   }
   I $r
   foreach($side in 'host','client'){if((Health $side).current-ne56){throw 'Native health changed during observation'}}
  }
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;if((B $r).localPlayerStance-eq1){I $r B 180};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 14|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash $probe).Hash;scope='Read-only native state and Midnight scene timing under real standing, crouched, held/combo and airborne sword and block input. Fields remain hypotheses until correlated. No native FP animation or physics writes.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};'Native combat timing observations saved'
