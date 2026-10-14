param([ValidatePattern('^[a-z0-9-]+$')][string]$TakePrefix='v16-timeline-v1',[switch]$AuthoredFeet)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$summary=[Collections.Generic.List[object]]::new()
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
try{
 foreach($kind in 'Air','Sprint','Lean','Slide'){
  foreach($r in 'host','client'){
   & "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -DurationMs 100|Out-Null
   C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
   C $r ('coop_movement_feet '+$(if($AuthoredFeet){'authored'}else{'off'}))
   $character=if($r-eq'host'){'emily'}else{'corvo'}
   Copy-Item "$PSScriptRoot/air-combat-banks-v1/$character-air-combat-bank.bin" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
  }
  $take=$kind.ToLower()+'-'+$TakePrefix
  if($kind-eq'Slide'){& "$PSScriptRoot/Test-SlideBank.ps1" -Take $take -AuthoredAir}else{& "$PSScriptRoot/Test-${kind}RegressionV16.ps1" -Take $take}
  $result=Get-Content "$PSScriptRoot/$take/result.json" -Raw|ConvertFrom-Json
  if(!$result.passed){throw "Movement timeline $kind failed"}
  $summary.Add([pscustomobject]@{kind=$kind;take=$take;checks=$result.checks.Count;passed=$result.passed;dll=$result.dll})
  "TIMELINE_REGRESSION $kind $($result.checks.Count) checks"
 }
 foreach($f in (Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
}finally{
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $summary|ConvertTo-Json -Depth 5|Set-Content "$root/work/movement-integration/movement-$TakePrefix-regressions.json"
}
