param([string]$Take='npc-authority-census-v836')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out=Join-Path $PSScriptRoot $Take;$lab="$root/outputs/d2-coop-lab"
$m=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$rows=[Collections.Generic.List[object]]::new();$pids=@{};$epochs=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 20|Set-Content -LiteralPath $p}
function Q($r,$mode,$handle){
 $tag=$mode.TrimStart('-');$path="$out/$r-$handle-$tag.json"
 $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $handle 2> "$out/$r-$handle-$tag.stderr"
 $code=$LASTEXITCODE;if(!$code){$value=$raw|ConvertFrom-Json;Save $path $value;return @{ok=$true;value=$value;path=$path}}
 return @{ok=$false;code=$code}
}
try{
 try{$owned=$m.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(!(Test-Path $out)){New-Item -ItemType Directory $out|Out-Null};if(Test-Path "$out/result.json"){throw 'Completed take already exists'};Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated identity'};$pids[$r]=$l.pid
  $s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){throw 'Gameplay required'};$epochs[$r]=$s.localEpoch;Save "$out/before-$r.json" $s
  $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --world-npcs;if($LASTEXITCODE){throw 'World catalog failed'};$catalog=$raw|ConvertFrom-Json;Save "$out/world-$r.json" $catalog
  foreach($n in $catalog.npcs|Where-Object unique){
   $h=Q $r '--health' $n.handle;$row=@{role=$r;npc=$n;health=$h;ai=@{}}
   if($h.ok-and$h.value.current-gt0){
    foreach($kind in 'decision','reflex','attack','idle'){$row.ai[$kind]=Q $r "--ai-$kind" $n.handle}
    $row.pose=Q $r '--npc-pose' $n.handle;$row.animation=Q $r '--animation' $n.handle
   }
   $rows.Add($row)
  }
  $s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;Save "$out/after-$r.json" $s;if($s.localEpoch-ne$epochs[$r]-or$s.localGameLogicState-ne1-or$s.localPendingGameLogicState){throw 'World changed during read-only census'}
 }
 Save "$out/profiles.json" @($rows)
 $summary=@();foreach($r in 'host','client'){
  $list=@($rows|Where-Object role -eq $r);$living=@($list|Where-Object {$_.health.ok-and$_.health.value.current-gt0});$active=@($living|Where-Object {$_.ai.decision.ok-and$_.ai.reflex.ok-and$_.ai.decision.value.started-and$_.ai.reflex.value.started-and$_.ai.decision.value.schedulerActive-and$_.ai.reflex.value.schedulerActive-and!$_.ai.decision.value.pauseReferences-and!$_.ai.reflex.value.pauseReferences})
  $summary+=@{role=$r;unique=$list.Count;living=$living.Count;activeAi=$active.Count;activeNames=@($active|ForEach-Object {$_.npc.spawnerName})}
 }
 foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 Save "$out/result.json" @{passed=$true;readOnly=$true;summary=$summary;scope='Owned exact-build NPC identity/health/AI census; no game command, input, pause, damage or world mutation. Profiles select requirements for a general authority channel, not proof of authority.'}
 Get-Content "$out/result.json"
}finally{if($owned){$m.ReleaseMutex()};$m.Dispose()}
