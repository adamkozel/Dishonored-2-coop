param([string]$Take='timepiece-research-v1591a',[string]$Candidate='candidate-npc-combat-v1214et')
# 1591 (#102): read-only Timepiece identities for an exact native reader. Run inside a Route-Mission -HoldAtEnd hold in
# which both roles picked up their Timepiece (route-steps-timepiece-v1590.json); Restore-RouteHold-v1303 restores after.
# Per role: --player-timepiece-research idle, then with the peek open (X; UnfoldTimeShiftLoop 70) together with one
# --midnight read (the gadget scene's binding actors), then X again to fold. No commands are sent; inputs are X only.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw "Take exists: $Take"};New-Item -ItemType Directory $out|Out-Null
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$pids=@{};$failure=$null;$rows=[ordered]@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function Q($r,$mode){for($i=0;$i-lt8;$i++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='TimepieceResearch1591';take=$Take;state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe").Hash-ne$pin.probe){throw "Expected the $Candidate probe"}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$pids[$r]=$l.pid}
 Save "$out/identity.json" @{candidate=$Candidate;probe=$pin.probe;dll=$pin.dll}
 foreach($r in 'host','client'){$row=[ordered]@{}
  $row.idle=Q $r '--player-timepiece-research';$row.idleStates=@((Q $r '--player-states').active|ForEach-Object {"$($_.id):$($_.type)"})
  & "$lab/Send-D2Input.ps1" -Role $r -Button X -DurationMs 300|Out-Null;Start-Sleep -Milliseconds 1400
  $row.peek=Q $r '--player-timepiece-research';$row.peekStates=@((Q $r '--player-states').active|ForEach-Object {"$($_.id):$($_.type)"})
  $m=Q $r '--midnight';$row.peekScenes=@($m.instances|Where-Object {[string]$_.asset-match'timeshift'})
  & "$lab/Send-D2Input.ps1" -Role $r -Button X -DurationMs 300|Out-Null;Start-Sleep -Milliseconds 1400
  $row.after=Q $r '--player-timepiece-research';$row.afterStates=@((Q $r '--player-states').active|ForEach-Object {"$($_.id):$($_.type)"})
  $rows[$r]=$row;Save "$out/$r.json" $row}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){Save "$out/result.json" @{passed=(!$failure);failure=$failure;scope='Read-only Timepiece identities (inventory entry, component, equipment declaration, entity, selection) and peek scene bindings in both roles.'}
  [ordered]@{pid=$PID;operation='TimepieceResearch1591';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json";$mutex.ReleaseMutex()}
 $mutex.Dispose()}
if($failure){throw $failure}
