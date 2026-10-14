param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-equipment-timing-v1',[switch]$NormalDraw)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$pids=@{};$probe=Join-Path $root 'work/build/Release/d2_runtime_probe.exe'
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$trigger=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -RightTrigger $trigger|Out-Null}
function ReadStatus($path){for($a=0;$a-lt8;$a++){try{return(Get-Content -LiteralPath $path -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{if($a-eq7){throw};Start-Sleep -Milliseconds 15}}}
function B($r){ReadStatus "$root/work/lab-instances/$r/coop-status.json"}
function F($r){ReadStatus "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json"}
function Sword($r){$raw=& $probe $pids[$r] --player-sword;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};return $null}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
 }
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'};C $src 'setviewpos -33.5 -153 68.1 0 0';C $dst 'setviewpos -30.5 -149 68.1 233.13 0'
  foreach($stance in 0,1){
   if((B $src).localPlayerStance-ne$stance){I $src B 180;Start-Sleep -Milliseconds 1100}
   foreach($trial in 'holster','draw'){
    $before=Sword $src;if(!$before-or$before.equipped-ne($trial-eq'holster')){throw "Unexpected source equipment: $src $trial"}
    if($trial-eq'holster'){I $src X 1500}elseif($NormalDraw){I $src X 650}else{I $src Neutral 180 255}
    $watch=[Diagnostics.Stopwatch]::StartNew()
    while($watch.ElapsedMilliseconds-lt2600){
     if(!(F $src).ready-or!(F $dst).ready-or(B $src).localGameLogicState-ne1){throw 'Native fixture changed'}
     $raw=& $probe $pids[$src] --player-states
     if(!$LASTEXITCODE){$native=$raw|ConvertFrom-Json;$sword=Sword $src;$scenes=& $probe $pids[$src] --midnight
      if(!$LASTEXITCODE){$rows.Add([pscustomobject]@{role=$src;stance=$stance;trial=$trial;ms=$watch.ElapsedMilliseconds;native=$native;sword=$sword;midnight=($scenes|ConvertFrom-Json)})}}
     Start-Sleep -Milliseconds 15
    }
    if((Sword $src).equipped-ne($trial-eq'draw')){throw 'Native input did not finish equipment transition'}
   }
  }
 }
 foreach($r in 'host','client'){$health=& $probe $pids[$r] --health 1|ConvertFrom-Json;if($LASTEXITCODE-or$health.current-ne56){throw 'Native player health changed'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;if(!(Sword $r).equipped){I $r Neutral 180 255;Start-Sleep -Milliseconds 1000};if((B $r).localPlayerStance-eq1){I $r B 180};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 14|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash $probe).Hash;normalDraw=[bool]$NormalDraw;scope='Read-only observation under actual native holster/draw input, both characters and stances. State+24 outside verified combat remains a hypothesis until scene/ownership correlation.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};'Native equipment observations saved'
