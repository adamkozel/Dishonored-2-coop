param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='combat-bank-v1-test')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$pids=@{};$probe=Join-Path $root 'work/build/Release/d2_runtime_probe.exe'
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$trigger=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -RightTrigger $trigger|Out-Null}
function ReadStatus($path){for($attempt=0;$attempt-lt8;$attempt++){try{return(Get-Content -LiteralPath $path -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{if($attempt-eq7){throw};Start-Sleep -Milliseconds 15}}}
function B($r){ReadStatus "$root/work/lab-instances/$r/coop-status.json"}
function M($r){ReadStatus "$root/work/lab-instances/$r/coop-status.json.movement.json"}
function S($r){ReadStatus "$root/work/lab-instances/$r/coop-status.json.shadow-stream.json"}
function F($r){ReadStatus "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json"}
function NativeHealth($r){$raw=& $probe $pids[$r] --health 1;if($LASTEXITCODE){throw 'Health read failed'};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'
  if((B $r).localGameLogicState-ne1-or(NativeHealth $r).current-ne56){throw 'Clean live fixture required'}
 }
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($side in 'host','client'){C $side 'coop_movement_fixture off';C $side 'coop_movement_fixture 60000'}
  C $dst 'setviewpos -30.5 -149 68.1 233.13 0'
  foreach($side in 'host','client'){C $side 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C $dst 'coop_movement_bank on';Start-Sleep -Milliseconds 1000
  if(!(M $dst).active-or(M $dst).version-ne8){throw 'Combat bank unavailable'}
  foreach($trial in 'tap','held','combo','crouch-tap','block','crouch-block','air-tap'){
   $stance=if($trial.StartsWith('crouch')){1}else{0};if((B $src).localPlayerStance-ne$stance){I $src B 180;Start-Sleep -Milliseconds 1100}
   C $src 'setviewpos -33.5 -153 68.1 0 0';Start-Sleep -Milliseconds 700
   $events=if($trial-match'block'){@(@(0,'RightShoulder',1400,0))}elseif($trial-eq'air-tap'){@(@(0,'A',180,0),@(150,'Neutral',180,255))}elseif($trial-eq'combo'){@(@(0,'Neutral',180,255),@(550,'Neutral',180,255),@(1100,'Neutral',180,255))}elseif($trial-eq'held'){@(@(0,'Neutral',1500,255))}else{@(@(0,'Neutral',180,255))}
   if($events.Count-eq4-and$events[0]-is[int]){$events=,@($events)}
   $trialRows=[Collections.Generic.List[object]]::new();$next=0;$w=[Diagnostics.Stopwatch]::StartNew()
   while($w.ElapsedMilliseconds-lt3200){
    if((B $src).localGameLogicState-ne1-or!(F $src).ready-or!(F $dst).ready){throw 'Native observation fixture retired'}
    if($next-lt$events.Count-and$w.ElapsedMilliseconds-ge$events[$next][0]){$e=$events[$next];I $src $e[1] $e[2] $e[3];$next++}
    $m=M $dst;$stream=S $dst
    if(!$m.active-or!$stream.armed-or!$stream.ready){throw "Combat stream retired: $src $trial $($stream.result)"}
    $raw=& $probe $pids[$src] --player-states
    if(!$LASTEXITCODE){$row=[pscustomobject]@{source=$src;trial=$trial;ms=$w.ElapsedMilliseconds;native=($raw|ConvertFrom-Json);movement=$m;stream=$stream;bridge=(B $dst)};$rows.Add($row);$trialRows.Add($row)}
    Start-Sleep -Milliseconds 15
   }
   I $src
   $live=@($trialRows|Where-Object {$_.movement.combatWeight-gt.7})
   if($trial-eq'air-tap'){
    if(!@($trialRows|Where-Object {$_.movement.airWeight-gt.7-and$_.movement.combatWeight-lt.1}).Count){throw 'Air priority was lost'}
    $checks.Add("$src airborne sword keeps verified air-body priority; air combat remains pending")
   }else{
    if(!$live.Count-or!@($live|Where-Object {$_.movement.weight-gt.8}).Count){throw "No full-body combat applied: $src $trial"}
    if($trial-eq'held'-and(!@($live|Where-Object {$_.movement.combatVariant-eq7-and$_.movement.combatPhase-eq2}).Count-or!@($live|Where-Object {$_.movement.combatVariant-eq8}).Count)){throw 'Heavy hold/release missing'}
    if($trial-eq'combo'-and@($live.movement.combatEvent|Sort-Object -Unique).Count-lt2){throw 'Combo reused one event'}
    if($trial-match'block'-and!@($live|Where-Object {$_.movement.combatVariant-eq11-and$_.movement.combatPhase-eq2}).Count){throw 'Sustained guard missing'}
    if($trial-eq'crouch-tap'-and!@($live|Where-Object {$_.movement.combatClip-in48,49}).Count){throw 'Crouched authored attack missing'}
    $checks.Add("$src $trial exact scene semantics drive the authored full-body bank")
   }
   $m=M $dst;if($m.combatWeight-gt.01-or$m.weight-lt.9){throw 'Combat did not recover to locomotion'}
   $observer=& $probe $pids[$dst] --player-states|ConvertFrom-Json
   if($LASTEXITCODE-or($observer.active.id|Where-Object {$_-in39,44})){throw 'Observer first-person action changed'}
   foreach($side in 'host','client'){if((NativeHealth $side).current-ne56){throw 'Native health changed'}}
  }
  $checks.Add("$src all trials recover; observer first-person controls and both player health values preserved")
  C $dst 'coop_movement_bank off';foreach($side in 'host','client'){C $side 'coop_shadow_stream off'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((B $r).localPlayerStance-eq1){I $r B 180};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 14|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=@($checks);dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Native input, strict combat-scene semantics, Steam transport and full-body playback. No duplicate native animation events; visual acceptance, contact, hit reactions, directional parries and air combat remain pending.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"COMBAT_BANK_VALIDATED $($checks.Count) checks"
