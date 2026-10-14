param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-double-jump-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe $r $mode"}
function Jump($r,$trial,$second){
 I $r;C $r 'setviewpos -34 -153 68.1 0 0';Start-Sleep -Milliseconds 900
 I $r A 100;$watch=[Diagnostics.Stopwatch]::StartNew();$sent=$false;$jumps=[Collections.Generic.List[object]]::new()
 while($watch.ElapsedMilliseconds-lt3500){
  # Hold across the native minimum second-impulse time. A short pulse can
  # expire before that time when wall time advances faster than game time.
  if($second-and!$sent-and$watch.ElapsedMilliseconds-ge180){I $r A 650;$sent=$true}
  $n=Q $r '--player-states';$rows.Add([pscustomobject]@{role=$r;trial=$trial;ms=$watch.ElapsedMilliseconds;native=$n;fixture=(S $r '.agility-fixture.json')})
  foreach($a in @($n.active|Where-Object id -eq 2)){$jumps.Add($a.jumpResearch)}
  Start-Sleep -Milliseconds 15
 }
 if(!$jumps.Count){throw "No native jump samples: $r $trial"}
 if($second){
  $before=@($jumps|Where-Object remaining94 -eq 1);$after=@($jumps|Where-Object remaining94 -eq 0)
  if(!$before.Count-or!$after.Count){throw "Second impulse count not observed: $r $trial"}
  $delta=($after|Measure-Object targetZa4 -Maximum).Maximum-($before|Measure-Object targetZa4 -Minimum).Minimum
  if([Math]::Abs($delta-2.5)-gt.02){throw "Native second apex increment differs: $delta"}
  $checks.Add("${r}: native Agility input consumes remaining jump 1->0 and raises target apex by 2.5 m")
 }else{
  if(@($jumps|Where-Object remaining94 -ne 0).Count){throw 'Ordinary jump retained an extra impulse'}
  $checks.Add("${r}: $trial native ordinary jump has no extra impulse")
 }
 if(@((Q $r '--player-states').active|Where-Object {$_.id-eq2-or$_.id-eq3}).Count){throw 'Native jump did not land'}
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  I $r;foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_agility_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--player-agility').quantity-or(Q $r '--health' 1).current-ne56){throw 'Baseline agility or health differs'}
 }
 foreach($r in 'host','client'){
  C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';Jump $r 'ordinary-before' $false
  C $r 'coop_agility_fixture 30000';Start-Sleep -Milliseconds 700
  if((Q $r '--player-agility').quantity-ne1-or(S $r '.agility-fixture.json').result-ne'native_agility_ready'){throw 'Native Agility grant not ready'}
  Jump $r 'agility-second-input' $true
  C $r 'coop_agility_fixture off';Start-Sleep -Milliseconds 700
  if((Q $r '--player-agility').quantity-or(S $r '.agility-fixture.json').active){throw 'Native Agility quantity not restored'}
  Jump $r 'ordinary-after' $false
  if((Q $r '--health' 1).current-ne56){throw 'Native jump changed health'}
  $checks.Add("${r}: original quantity, ordinary jump behavior and health restored")
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $checks.Add('Original Steam files and protected saves unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_agility_fixture off';Start-Sleep -Milliseconds 650;C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Native Agility quantity and actual two-input jumping on both roles, restoration verified by ordinary jumps before/after. No jump-state or physics writes. Remote double-jump animation and visual acceptance are not established by this observation.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"NATIVE_DOUBLE_JUMP_VALIDATED $($checks.Count) checks"
