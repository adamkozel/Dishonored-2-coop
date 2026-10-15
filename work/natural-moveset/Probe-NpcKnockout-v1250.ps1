param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='npc-knockout-v1250',[string]$Source='host',[string[]]$Kinds=@('choke','lethal'))
# 1250 (#46): native observation of an NPC knockout. A choke leaves the victim unconscious without any damage
# declaration (the resource census has only choke_throw_damage and assassination_damage), and the mod reads no
# unconscious state yet, so a client's choke cannot be shared. The source (host) chokes the Dunwall upper-floor
# tutorial guard 2 from 0.9 m behind (AI held by the movement fixture, as in the choke harnesses) and, for
# comparison, kills it with the fast-back lethal. The guard's native readouts are sampled before, every ~300 ms
# during, and after: world identity, health, the NPC-authority frame (AI components, flags, scheduler), the AI
# decision/reflex/idle/attack components, animation and the component list. Pass: the takedown scene is observed
# and every readout set is captured. Fixture (PrepareShadowSword + note restore) restored after each case.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$inv=[Globalization.CultureInfo]::InvariantCulture
function Save($p,$v){$v|ConvertTo-Json -Depth 20|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($a=0;$a-lt6;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){try{return($raw|ConvertFrom-Json)}catch{return @{raw="$raw"}}};Start-Sleep -Milliseconds 20};return @{failed=$mode;exit=$LASTEXITCODE}}
function F($v){([double]$v).ToString('F5',$inv)}
function Guard($r){$g=@((Q $r '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'});if($g.Count-ne1){throw 'Exact upper-floor guard2 absent'};$g[0]}
function Readouts($r,$h,$full){$x=[ordered]@{world=@((Q $r '--world-npcs').npcs|Where-Object {$_.handle-eq$h})|Select-Object -First 1;health=(Q $r '--health' $h);decision=(Q $r '--ai-decision' $h);reflex=(Q $r '--ai-reflex' $h);idle=(Q $r '--ai-idle' $h);attack=(Q $r '--ai-attack' $h);animation=(Q $r '--animation' $h)}
 if($full){$x.components=(Q $r '--components' $h);$x.authority=(Q $r '--npc-authority')};[pscustomobject]$x}
function Restore($tag){foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_fixture off'}catch{}}
 & "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"
 & "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration-$tag" *>> "$out/note-restoration.log"
 foreach($r in 'host','client'){$restored=Q $r '--player-inventory'
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw "Checkpoint inventory differs ($r)"}
  if((Q $r '--health' 1).current-ne56){throw "Restored player health differs ($r)"}}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='NpcKnockout1250';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
 }
 Save "$out/identity.json" ([ordered]@{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;source=$Source;kinds=$Kinds})
 $peer=if($Source-eq'host'){'client'}else{'host'}
 foreach($kind in $Kinds){
  $key="$Source-$kind";$row=[ordered]@{source=$Source;kind=$kind;passed=$false;failure=$null}
  try{
   $dirty=$true
   foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
   C $peer 'setviewpos -33 -153 68.1 90 0'
   $g=Guard $Source;$h=[uint32]$g.handle;$row.handle=$h;$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
   Save "$out/$key-before.json" (Readouts $Source $h $true)
   C $Source ('setviewpos {0} {1} 68.1 {2} 0' -f (F ([double]$g.position[0]-.9*$fx)),(F ([double]$g.position[1]-.9*$fy)),$look.ToString('F4',$inv));Start-Sleep -Milliseconds 700
   $scenes=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$Source],'--watch-midnight','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/$key-scenes.ndjson" -RedirectStandardError "$out/$key-scenes.stderr" -PassThru
   if($kind-eq'choke'){I $Source RightShoulder 6500}else{& "$lab/Send-D2Input.ps1" -Role $Source -RightTrigger 255 -DurationMs 250|Out-Null}
   $samples=[Collections.Generic.List[object]]::new();$w=[Diagnostics.Stopwatch]::StartNew()
   while($w.ElapsedMilliseconds-lt9000){$samples.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;player=(Q $Source '--player-states');guard=(Readouts $Source $h $false)});Start-Sleep -Milliseconds 150}
   if(!$scenes.WaitForExit(15000)){Stop-Process -Id $scenes.Id}
   Save "$out/$key-samples.json" @($samples)
   Start-Sleep -Milliseconds 1500;Save "$out/$key-after.json" (Readouts $Source $h $true)
   $found=@{};foreach($line in Get-Content "$out/$key-scenes.ndjson"){try{$o=$line|ConvertFrom-Json}catch{continue};foreach($x in @($o.instances)){if("$($x.asset)"-match'/(choke|assassination)/'){$found["$($x.asset)|$($x.subscene)|$($x.sceneDurationMs)"]=1}}}
   $row.scenes=@($found.Keys|Sort-Object);$row.passed=[bool]$row.scenes.Count;if(!$row.passed){$row.failure='No takedown scene observed'}
  }catch{$row.failure=$_.Exception.Message}
  try{Restore $key;$row.restored=$true}catch{$row.restored=$false;$row.failure=(("$($row.failure); restore: "+$_.Exception.Message)).TrimStart('; ')}
  $rows.Add([pscustomobject]$row);Save "$out/$key.json" ([pscustomobject]$row)
  if(!$row.restored){throw "Fixture restore failed after $key"}
 }
 $dirty=$false
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}finally{
 if($owned){
  if($dirty){try{Restore 'final'}catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}else{foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_fixture off'}catch{}}}
  $errors=[Collections.Generic.List[string]]::new()
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}}catch{$errors.Add($_.Exception.Message)}
  foreach($r in 'host','client'){try{if((S $r).localGameLogicState-ne1){$errors.Add("$r not back in gameplay")}}catch{$errors.Add("$r status unavailable")}}
  if(Test-Path -LiteralPath $out){
   Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)}
   $bad=@($rows|Where-Object {!$_.passed});if(!$failure-and$bad.Count){$failure="$($bad.Count) case(s) failed"}
   if($errors.Count-and!$failure){$failure=$errors-join '; '}
   Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;cases=@($rows);scope='Native NPC knockout and lethal observation on the Dunwall upper-floor guard 2 (source host); readouts only, no partner presentation claims.'})
  }
  [ordered]@{pid=$PID;operation='NpcKnockout1250';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_NPC_KNOCKOUT_PROBE $($rows.Count) cases"
