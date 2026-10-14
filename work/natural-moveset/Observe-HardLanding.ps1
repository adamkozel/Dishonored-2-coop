param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-hard-landing-v1',[ValidateRange(1,7)][double[]]$Heights=@(2,4,5.5))
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 12};throw "Probe $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  & "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -DurationMs 100|Out-Null
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_agility_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Baseline health changed'}
 }
 foreach($r in 'host','client'){
  $observer=if($r-eq'host'){'client'}else{'host'};C $observer 'setviewpos -28 -149.2 68.1 225 0';Start-Sleep -Milliseconds 500
  foreach($height in $Heights){
   C $r 'setviewpos -34 -153 68.1 0 0';Start-Sleep -Milliseconds 650
   C $r ('setviewpos -34 -153 '+(68.1+$height).ToString([Globalization.CultureInfo]::InvariantCulture)+' 0 0')
   $watch=[Diagnostics.Stopwatch]::StartNew();$cat=$false;$fall=$false
   while($watch.ElapsedMilliseconds-lt3500){
    $n=Q $r '--player-states';$scene=$null;$active=@($n.active|Where-Object id -eq 4)
    if($active.Count){$scene=Q $r '--midnight';$cat=$true};$fall=$fall-or@($n.active|Where-Object id -eq 3).Count-gt0
    $rows.Add([pscustomobject]@{role=$r;height=$height;ms=$watch.ElapsedMilliseconds;native=$n;midnight=$scene});Start-Sleep -Milliseconds 12
   }
   if(!$fall){throw 'Native fall not observed'};if((Q $r '--health' 1).current-ne56){throw 'Unexpected fall damage'}
   $checks.Add("${r}: height=$height native falling observed, CatFall=$cat, health unchanged")
  }
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Protected original files and saves unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 18|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;heights=$Heights;scope='Native gravity from bounded isolated nominal-height test placements; state and scene inspection only, no source animation/physics writes. No damage-height test or remote binding.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"NATIVE_LANDING_OBSERVED $($checks.Count) checks"
