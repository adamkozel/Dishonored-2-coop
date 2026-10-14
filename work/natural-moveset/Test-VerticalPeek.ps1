param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='vertical-peek-v1-test')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$handles=@{};$checks=[Collections.Generic.List[string]]::new();$rows=[Collections.Generic.List[object]]::new();$failure=$null
$liveDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$x=0,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftX $x -LeftY $y|Out-Null}
function Status($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe failed $r $mode"}
function Collect($src,$dst,$label,$ms){
 $samples=[Collections.Generic.List[object]]::new();$watch=[Diagnostics.Stopwatch]::StartNew()
 while($watch.ElapsedMilliseconds-lt$ms){
  $m=Status $dst '.movement.json';$s=Status $dst '.shadow-stream.json'
  if(!$m.active-or$m.version-ne11-or!$s.ready-or!$s.armed){throw "Presentation retired during $label"}
  if(!(Status $src '.observer-isolation.json').ready-or!(Status $dst '.observer-isolation.json').ready){throw 'Fixture retired'}
  $row=[pscustomobject]@{source=$src;label=$label;ms=$watch.ElapsedMilliseconds;movement=$m;stream=$s;sourcePosition=(Status $src).localPosition};$samples.Add($row);$rows.Add($row);Start-Sleep -Milliseconds 35
 };return $samples
}
function AwaitBank($r){$watch=[Diagnostics.Stopwatch]::StartNew();do{$m=Status $r '.movement.json';if($m.active-and$m.weight-ge.9){return};Start-Sleep -Milliseconds 35}while($watch.ElapsedMilliseconds-lt5000);throw 'Bank readiness deadline'}
function ExitLean($r){
 I $r Neutral 650 0 -22000;Start-Sleep -Milliseconds 900
 if(@((Q $r '--player-states').active|Where-Object id -eq 18).Count){C $r 'setviewpos -33.5 -153 68.1 90 0';Start-Sleep -Milliseconds 850}
 if(@((Q $r '--player-states').active|Where-Object id -eq 18).Count){throw 'Native lean did not exit under movement/turn controls'}
 I $r;C $r 'setviewpos -33.5 -153 68.1 0 0';Start-Sleep -Milliseconds 400
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(Status $r).probeActorHandle;I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';ExitLean $r
  if(!(Q $r '--player-sword').equipped-or(Q $r '--health' 1).current-ne56){throw 'Clean armed fixture required'}
  $role=if($r-eq'host'){'emily'}else{'corvo'};Copy-Item "$PSScriptRoot/vertical-peek-banks-v1/$role-vertical-peek-bank.bin" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
 }
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'}
  C $src 'setviewpos -33.5 -153 68.1 0 0';C $dst 'setviewpos -30.5 -149 68.1 233.13 0'
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C $dst 'coop_movement_bank on';AwaitBank $dst
  foreach($armed in $true,$false){
   foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
   if(!$armed){I $src X 1500;Start-Sleep -Milliseconds 3000}
   foreach($stance in 0,1){
    if((Status $src).localPlayerStance-ne$stance){I $src B 180;Start-Sleep -Milliseconds 1000}
    I $src Y 1000 0 32767;$s=Collect $src $dst "up-$armed-$stance" 1300
    if(!@($s|Where-Object {$_.movement.leanUp-gt.9-and$_.movement.leanWeight-gt.9-and$_.movement.weight-gt.9}).Count){throw "Native upward peek missing: $src $armed $stance"}
    $m=$s[-1].movement;if(($armed-and$m.armedMix-lt.99)-or(!$armed-and$m.armedMix-gt.01)){throw 'Wrong equipment blend'}
    $checks.Add("${src}: armed=$armed stance=$stance native vertical peek selects a complete body pose")
    I $src Y 850 32767 0;$s=Collect $src $dst "side-$armed-$stance" 1100
    if(!@($s|Where-Object {[Math]::Abs($_.movement.leanAmount)-gt.8-and$_.movement.leanWeight-gt.9}).Count){throw 'Upward to lateral peek failed'}
    I $src Y 1000 0 -32767;$s=Collect $src $dst "down-$armed-$stance" 1300
    if($s[-1].movement.leanUp-gt.05){throw 'Vertical peek did not lower'}
    ExitLean $src;$s=Collect $src $dst "exit-$armed-$stance" 700
    if($s[-1].movement.leanWeight-gt.05){throw 'Lean did not return to locomotion'}
    $sword=Q $dst '--avatar-sword' $handles[$dst];if([bool]$sword.equipped-ne$armed){throw 'Peek changed remote sword state'}
    $checks.Add("${src}: armed=$armed stance=$stance side transition, lower and native movement/turn exit preserve equipment")
   }
  }
  if((Status $src).localPlayerStance-eq1){I $src B 180;Start-Sleep -Milliseconds 900};I $src X 650;Start-Sleep -Milliseconds 3000
  if(!(Q $src '--player-sword').equipped-or(Status $dst '.movement.json').armedMix-lt.99){throw 'Armed recovery failed'}
  C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off';if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
  $checks.Add("${src}: native armed recovery and both player/avatar health preserved")
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2500};if((Status $r).localPlayerStance-eq1){I $r B 180};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 10|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=$liveDll;scope='Native vertical/lateral peek input, Steam state replication, full-body bank, equipment recovery and health. Visual posture, arbitrary geometry and complete movement acceptance remain separate.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"VERTICAL_PEEK_VALIDATED $($checks.Count) checks"
