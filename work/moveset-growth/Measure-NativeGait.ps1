param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-speed-1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path -LiteralPath $out){throw 'Take already exists'};New-Item -ItemType Directory -Path $out|Out-Null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$y=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftY $y -DurationMs $ms|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
$records=[Collections.Generic.List[object]]::new();$failure=$null
try{
 foreach($r in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$launch.assetExperiment-or(Get-Process -Id $launch.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'}
  I $r;C $r 'coop_movement_bank off';C $r 'coop_authoring_proof off';C $r 'coop_shadow_stream off'
 }
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'}
  C $dst 'setviewpos -28 -151.5 68.1 214 11'
  foreach($axis in 11000,16000,22000,28000,32767){
   I $src;C $src 'setviewpos -34.3 -154.8 68.1 0 0';Start-Sleep -Milliseconds 650
   $before=B $src;if($before.localGameLogicState-ne1-or!$before.remoteFresh-or!$before.localPlayerMotionValid-or$before.localPlayerMotion-ne0){throw 'Native walking fixture is not ready'}
   I $src $axis 750;$watch=[Diagnostics.Stopwatch]::StartNew()
   do{
    $records.Add([pscustomobject]@{source=$src;axis=$axis;elapsed=$watch.ElapsedMilliseconds;tick=[Environment]::TickCount64;state=(B $src);observer=(B $dst)})
    Start-Sleep -Milliseconds 35
   }while($watch.ElapsedMilliseconds-lt850)
   I $src;Start-Sleep -Milliseconds 300
  }
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'}catch{}}
 try{C 'host' 'setviewpos -33 -153 68.1 0 0';C 'client' 'setviewpos -29 -151.5 68.1 204 11'}catch{}
 $records|ConvertTo-Json -Depth 7|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;records=$records.Count;scope='Ordinary native analog forward speed calibration, no sprint-button coverage; pose bank remains off.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};Get-Content "$out/result.json"
