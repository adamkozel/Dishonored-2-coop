param([string]$Take='state-trigger-v1580a',[string]$Role='host',[string]$Triggers='RightThumb:1500;Back:800',[int]$SampleMs=120,[int]$SettleMs=1500,[string]$Candidate='candidate-npc-combat-v1214es')
# 1580 (#102): which native player states an input enters, read-only. In the current lab fixture each trigger
# 'Button[+Button]:holdMs' (or 'LT:ms' / 'RT:ms' for the triggers) is held on -Role while --player-states is sampled
# every SampleMs until SettleMs after the release; every active state id with its RTTI class is recorded per trigger.
# A modal game logic state (menus, journal) is closed with B afterwards. No commands, no grants; Dunwall fixture kept.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$pids=@{};$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='StateTrigger1580';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 Save "$out/identity.json" @{candidate=$Candidate;dll=$pin.dll;role=$Role;triggers=$Triggers}
 Save "$out/state-table.json" (Q $Role '--player-state-table')
 foreach($spec in @($Triggers-split';'|Where-Object {$_})){$name,$ms=$spec-split':';$ms=[int]$ms
  $before=@((Q $Role '--player-states').active|ForEach-Object {"$($_.id):$($_.type)"});$seen=[ordered]@{};$trace=[Collections.Generic.List[object]]::new()
  $ia=@{Role=$Role;DurationMs=$ms};if($name-eq'LT'){$ia.LeftTrigger=255}elseif($name-eq'RT'){$ia.RightTrigger=255}else{$ia.Button=@($name-split'\+')}
  & "$lab/Send-D2Input.ps1" @ia|Out-Null;$w=[Diagnostics.Stopwatch]::StartNew()
  while($w.ElapsedMilliseconds-lt($ms+$SettleMs)){$ps=try{Q $Role '--player-states'}catch{$null}
   if($ps){$ids=@($ps.active|ForEach-Object {"$($_.id):$($_.type)"});$trace.Add([ordered]@{ms=$w.ElapsedMilliseconds;active=$ids;stance=$ps.stance;logic=(S $Role).localGameLogicState});foreach($x in $ids){if(!$seen.Contains($x)){$seen[$x]=$w.ElapsedMilliseconds}}}
   Start-Sleep -Milliseconds $SampleMs}
  $logic=(S $Role).localGameLogicState;$closed=0;while($logic-ne1-and$closed-lt4){& "$lab/Send-D2Input.ps1" -Role $Role -Button B -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 900;$closed++;$logic=(S $Role).localGameLogicState}
  $rows.Add([ordered]@{trigger=$spec;before=$before;entered=@($seen.Keys|Where-Object {$_-notin$before});firstSeenMs=$seen;closedWithB=$closed;logicAfter=$logic});Save "$out/trace-$($rows.Count).json" @($trace)
  Start-Sleep -Milliseconds 600}
 Save "$out/rows.json" @($rows)
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){$errors=[Collections.Generic.List[string]]::new()
  foreach($r in 'host','client'){try{& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -DurationMs 100|Out-Null;$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;rows=@($rows);scope='Native player states entered by held inputs in the current lab fixture (read-only probes; no commands).'}
  [ordered]@{pid=$PID;operation='StateTrigger1580';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json"|Select-Object -First 3;if($failure){throw $failure}
