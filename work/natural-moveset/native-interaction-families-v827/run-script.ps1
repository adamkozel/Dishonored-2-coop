param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-interaction-families-v827')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$pids=@{};$rows=@{};$cache=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 18|Set-Content -LiteralPath $p}
function Q($r,$mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if($LASTEXITCODE){throw "Probe failed $r $mode"};return($raw|ConvertFrom-Json)}
function O($r,$ptr){$key="$r-$ptr";if(!$cache.ContainsKey($key)){$cache[$key]=Q $r '--object' $ptr;Save "$out/$key.json" $cache[$key]};return $cache[$key]}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique evidence directory required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='InteractionFamilies827';state='running';take=$Take}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  $before=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;Save "$out/before-$r.json" $before
  if($before.localGameLogicState-ne1-or$before.localPendingGameLogicState){throw 'Gameplay required'}
  $entities=Q $r '--entities';Save "$out/entities-$r.json" $entities
  $selected=@($entities.entities|Where-Object {$_.type-eq'.?AVarkUsable@@'-or$_.type-eq'.?AVarkMovable@@'-or($_.type-eq'.?AVidEntity@@'-and$_.name-match 'container|drawer|locker|safe_door')})
  $selected+=@($entities.entities|Where-Object type -eq '.?AVarkNPC@@'|Select-Object -First 3)
  if(!$selected.Count-or$selected.Count-gt128){throw 'Inspection scope empty or unbounded'}
  $list=[Collections.Generic.List[object]]::new()
  foreach($e in $selected){
   $cs=Q $r '--components' $e.handle;Save "$out/components-$r-$($e.handle).json" $cs
   foreach($c in $cs.components){if($c.owner-ne$e.pointer){throw 'Component ownership changed'}}
   $list.Add([ordered]@{name=$e.name;type=$e.type;handle=$e.handle;actor=$e.pointer;position=$e.position;components=@($cs.components|Select-Object type,pointer,owner)})
   foreach($c in @($cs.components|Where-Object {$_.type-match 'Loot|Openable|Usable|Lock'})){
    $o=O $r $c.pointer;$decl=@($o.words|Where-Object offset -eq '0x8');if($decl.Count-eq1-and$decl[0].word-ne'0x0'){O $r $decl[0].word|Out-Null}
   }
  }
  $after=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;Save "$out/after-$r.json" $after
  if($before.localEpoch-ne$after.localEpoch-or$after.localGameLogicState-ne1-or$after.localPendingGameLogicState){throw 'World changed during inspection'}
  $again=Q $r '--entities';foreach($e in $list){if(@($again.entities|Where-Object {$_.handle-eq$e.handle-and$_.pointer-eq$e.actor-and$_.name-eq$e.name}).Count-ne1){throw 'Inspected entity retired'}}
  $rows[$r]=$list;Save "$out/catalog-$r.json" $list
 }
 $hk=@($rows.host|ForEach-Object {"$($_.name)|$($_.type)|$(($_.components.type|Sort-Object)-join',')"});$ck=@($rows.client|ForEach-Object {"$($_.name)|$($_.type)|$(($_.components.type|Sort-Object)-join',')"})
 Save "$out/paired-differences.json" @(Compare-Object $hk $ck)
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
}catch{$failure=$_.Exception.Message}finally{
 if($owned){if(Test-Path $out){Save "$out/result.json" @{passed=(!$failure);failure=$failure;readOnly=$true;hostCount=@($rows.host).Count;clientCount=@($rows.client).Count;scope='Read-only native component/declaration census for usable entities, movable objects, named static containers/drawers and three NPC samples. Native family identification only; no interaction, grant, script event or shared-world certification.'}}
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='InteractionFamilies827';state='finished';take=$Take};$mutex.ReleaseMutex()};$mutex.Dispose()
}
if($failure){throw $failure};'PASS_READONLY_INTERACTION_CENSUS'
