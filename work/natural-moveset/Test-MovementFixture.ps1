param([string]$Take='movement-fixture-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function F($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json" -Raw|ConvertFrom-Json}
function N($r,$opt,$h){$raw=& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] $opt $h;if($LASTEXITCODE){throw 'Native fixture read failed'};return($raw|ConvertFrom-Json)}
function Sample($stage){
 $pair=@{}
 foreach($r in 'host','client'){$f=F $r;$actors=@();foreach($h in @($f.handles)+@($f.thirdHandle)){if($h){$actors+=@{handle=$h;decision=(N $r '--ai-decision' $h);reflex=(N $r '--ai-reflex' $h);attack=(N $r '--ai-attack' $h)}}};$pair[$r]=@{status=$f;actors=$actors;health=(N $r '--health' 1)}}
 $rows.Add([pscustomobject]@{stage=$stage;pair=$pair});return $pair
}
function Arm{
 foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000'};Start-Sleep -Milliseconds 500;$pair=Sample 'held'
 foreach($r in 'host','client'){$a=$pair[$r];if(!$a.status.ready-or!$a.status.movementFixture-or$a.status.heldReferences-ne6-or$a.actors.Count-ne3){throw 'Expected six references on three NPCs'}
  foreach($n in $a.actors){foreach($c in $n.decision,$n.reflex){if($c.pauseReferences-ne1-or$c.schedulerActive){throw 'AI reference not held'}};if($n.attack.schedulerActive-or$n.attack.started){throw 'AI was already attacking'}}
 };return $pair
}
function Released($before){foreach($r in 'host','client'){$a=F $r;if($a.active-or$a.heldReferences-or$a.released-ne$before[$r].status.released+6-or$a.lost-ne$before[$r].status.lost){throw 'AI references not balanced'}}}
try{
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid;C $r 'coop_movement_fixture off'}
 $a=Arm;foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000'};Start-Sleep -Milliseconds 350
 foreach($r in 'host','client'){$f=F $r;if($f.started-ne$a[$r].status.started-or$f.remainingMs-ge$a[$r].status.remainingMs){throw 'Duplicate renewed or reacquired fixture'}}
 $checks.Add('Both roles hold exactly six native Decision/Reflex references; duplicate requests do not renew or reacquire')
 foreach($r in 'host','client'){C $r 'coop_movement_fixture off'};Start-Sleep -Milliseconds 400;Released $a;$null=Sample 'cancelled'
 $checks.Add('Explicit cancellation releases each owned reference exactly once')
 $a=Arm;& "$lab/Send-D2Input.ps1" -Role host -Button Start -DurationMs 150|Out-Null;Start-Sleep -Milliseconds 600;Released $a;$null=Sample 'paused'
 & "$lab/Send-D2Input.ps1" -Role host -Button A -DurationMs 150|Out-Null;Start-Sleep -Milliseconds 600
 $checks.Add('Native pause releases both finite fixtures without losing references')
 $a=Arm;$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250}while(((F host).active-or(F client).active)-and$w.ElapsedMilliseconds-lt61000)
 Released $a;$e=Sample 'expired';foreach($r in 'host','client'){if($e[$r].health.current-ne56){throw 'Player health changed'}}
 $checks.Add('Finite lease expires on both roles; references balanced and native player health unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=@($checks);dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Developer-only isolated Dunwall movement fixture; three named native NPCs, 60-second nonrenewable hold. No production AI synchronization claim.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"Movement fixture passed $($checks.Count) checks"
