$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
Set-Location $root
$lab='outputs/d2-coop-lab'
$probe="$lab/bin/d2_runtime_probe.exe"
function Cmd($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function Bridge($r){Get-Content "work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function Span($v){$max=0.;foreach($a in $v){foreach($b in $v){$d=0.;for($i=0;$i-lt3;$i++){$d+=[Math]::Pow($a[$i]-$b[$i],2)};$max=[Math]::Max($max,[Math]::Sqrt($d))}};$max}
$rows=[Collections.Generic.List[object]]::new()
$results=[Collections.Generic.List[object]]::new()
try{
 foreach($role in 'host','client'){
  $l=Get-Content "work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Process mismatch'}
  $s=Bridge $role
  Cmd $role 'coop_probe_locomotion on';Cmd $role 'coop_shadow_loco combat'
  foreach($stage in 'GraphIdle','DirectNativeIdle'){
   Cmd $role 'coop_shadow_action off'
   if($stage-eq'GraphIdle'){Start-Sleep -Milliseconds 1600}else{Cmd $role 'coop_shadow_action idle'}
   $start=Get-Date
   for($i=0;$i-lt18;$i++){
    $b=& $probe $l.pid --bones $s.probeActorHandle|ConvertFrom-Json
    if($LASTEXITCODE){continue}
    $a=if($i%4-eq0){& $probe $l.pid --animation $s.probeActorHandle|ConvertFrom-Json}else{$null}
    $rows.Add([pscustomobject]@{Role=$role;Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Bones=$b;Animation=$a})
    Start-Sleep -Milliseconds 60
   }
   $stable=@($rows|Where-Object {$_.Role-eq$role-and$_.Stage-eq$stage-and$_.Bones.stable})
   $feet=@(foreach($j in 21,25){$v=@($stable|ForEach-Object {,@($_.Bones.bodies|Where-Object joint -eq $j)[0].localPosition});[pscustomobject]@{Joint=$j;Span=(Span $v)}})
   $inputs=@($rows|Where-Object {$_.Role-eq$role-and$_.Stage-eq$stage-and$_.Animation}|ForEach-Object {@($_.Animation.declaredInputs|Where-Object name -eq animBasicInputData1)[0]})
   $results.Add([pscustomobject]@{Role=$role;Stage=$stage;StableSamples=$stable.Count;Seconds=((Get-Date)-$start).TotalSeconds;Feet=$feet;AnimBasic=$inputs})
  }
  Cmd $role 'coop_shadow_action off'
 }
}finally{foreach($role in 'host','client'){Cmd $role 'coop_shadow_action off'}}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Dll=(Get-FileHash 'work/asset-game/dinput8.dll').Hash;Results=$results;Samples=$rows;Scope='Reference comparison at stationary network roots. Does not waive the existing idle regression threshold.'}|ConvertTo-Json -Depth 20|Set-Content "$lab/research/doppelganger-idle-reference.json"
$results|Select-Object Role,Stage,StableSamples,Seconds,Feet|ConvertTo-Json -Depth 5
