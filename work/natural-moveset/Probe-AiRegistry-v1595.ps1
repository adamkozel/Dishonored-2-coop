param([string]$Take='ai-registry-research-v1595a',[string]$Candidate='candidate-npc-combat-v1214ew')
# 1595 (#113): read-only. In the current lab world, for every living unique NPC whose authority read fails (civilians:
# arkCpntAIDecisionCivilian) and for the first readable one (a guard), --npc-ai-registries lists each AI component and
# the AI-manager registry holding it, in both roles. Saves npc-authority-all-<role>.json and registries-<role>.json.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw "Take exists: $Take"};New-Item -ItemType Directory $out|Out-Null
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$pids=@{};$failure=$null
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function Q($r,$mode,$arg=$null){for($i=0;$i-lt8;$i++){$raw=if($null-ne$arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='AiRegistry1595';take=$Take;state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash-ne$pin.probe){throw "Expected the $Candidate probe"}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$pids[$r]=$l.pid}
 Save "$out/identity.json" @{candidate=$Candidate;probe=$pin.probe;dll=$pin.dll}
 foreach($r in 'host','client'){
  $all=Q $r '--npc-authority-all';Save "$out/npc-authority-all-$r.json" $all
  $pick=@($all.entries|Where-Object {!$_.authority-and[double]$_.health-gt0})+@($all.entries|Where-Object {$_.authority-and[double]$_.health-gt0}|Select-Object -First 1)
  $rows=foreach($e in $pick){[ordered]@{handle=$e.handle;name=$e.name;definition=$e.definition;authority=$e.authority;registries=(Q $r '--npc-ai-registries' ([string]$e.handle))}}
  Save "$out/registries-$r.json" @($rows)}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){Save "$out/result.json" @{passed=(!$failure);failure=$failure;scope='Read-only AI component registries of unreadable (civilian) NPCs and one readable NPC, both roles.'}
  [ordered]@{pid=$PID;operation='AiRegistry1595';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json";$mutex.ReleaseMutex()}
 $mutex.Dispose()}
if($failure){throw $failure}
