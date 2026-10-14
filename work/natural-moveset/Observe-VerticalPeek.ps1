param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-vertical-peek-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function Status($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function Q($r,$mode,$id=0){for($i=0;$i-lt8;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native read $r $mode"}
try{
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid;I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'};C $dst 'setviewpos -30.5 -149 68.1 233.13 0'
  foreach($stance in 0,1){
   if((Status $src).localPlayerStance-ne$stance){I $src B 180;Start-Sleep -Milliseconds 1000}
   foreach($axis in 32767,-32767){
    C $src 'setviewpos -33.5 -153 68.1 0 0';Start-Sleep -Milliseconds 750
    $before=Q $src '--player';I $src Y 1000 $axis;$watch=[Diagnostics.Stopwatch]::StartNew()
    while($watch.ElapsedMilliseconds-lt1900){$state=Q $src '--player-states';$view=Q $src '--player';$rows.Add([pscustomobject]@{source=$src;stance=$stance;axis=$axis;ms=$watch.ElapsedMilliseconds;state=$state;view=$view;baseline=$before});Start-Sleep -Milliseconds 25}
    I $src Neutral 650 -22000;Start-Sleep -Milliseconds 1100;I $src
   }
  }
 }
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;if((Status $r).localPlayerStance-eq1){I $r B 180};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 13|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;scope='Read-only exact native Leaning fields and native camera/root positions under controller Y plus vertical stick. No native camera, physics, state or animation writes.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};'Vertical peek observation saved'
