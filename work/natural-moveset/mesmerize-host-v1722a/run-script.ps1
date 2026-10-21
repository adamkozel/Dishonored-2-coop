param([string]$Take='mesmerize-host-v1719a',[string]$Candidate='candidate-npc-combat-v1214gj',[int]$ActiveSeconds=20)
# 1719: the component is anchored to a static level entity, the nearest NPC's spawner (1718a: the world handle
# 0x3fff0010 did not resolve, configure_failed); 1214gj takes the anchor handle on the spawn command.
# 1718: waits for the Mesmerize component after the spawn (1214gi resolves it on the following frames; 1716a found
# none at once) and records the object's component list.
# 1716 (#131): the native Mesmerize object experiment in the host's world (candidate 1214gh, native_mesmerize_host.h).
# Lab host on the Dunwall guard map: probe the original definition (models/powers/mesmerize/mesmerize_object.def), spawn
# it 3 m from the nearest living NPC toward Corvo, start it (component configured with the world anchor, FSM state 1),
# then sample its FSM state, its target list and the nearby NPCs every 500 ms with frames, stop it (state 2) and sample
# again. Passes when the object reaches the active state (3), takes at least one host NPC, the game survives and the
# stop is accepted. Restores the lab pair with the Dunwall fixture (Deploy-SourceClock).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 10|Set-Content -LiteralPath $p}
function C($cmd){& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function J($suffix){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/host/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};$null}
function Q($mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $script:hostPid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $script:hostPid $mode};if($LASTEXITCODE){throw "probe $mode failed"};$raw|ConvertFrom-Json}
function Frame($name){$hwnd=(Get-Process -Id $script:hostPid).MainWindowHandle.ToInt64()
 & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$name-host.png" 2>$null|Out-Null}
function Living(){$rows=@();foreach($n in @((Q '--world-npcs').npcs)){$hp=$null;try{$hp=Q '--health' ([Convert]::ToInt64($n.handle.Substring(2),16))}catch{}
  if($hp-and$hp.current-gt0){$rows+=[pscustomobject]@{handle=$n.handle;spawnerHandle=$n.spawnerHandle;name=$n.spawnerName;position=@($n.position);health=$hp.current}}};$rows}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $script:hostPid=(Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json).pid
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 $hs=J '';if($hs.localGameLogicState-ne1){throw 'Host lab instance not in gameplay'};$notes.map=$hs.localMap
 C 'coop_player_health_hold on'
 C 'coop_mesmerize_native probe';Start-Sleep 1;$probe=J '.mesmerize-host.json';Save "$out/probe.json" $probe;$notes.probe=$probe.result
 if(!$probe.definitionFound){throw "The original Mesmerize object definition is not resident in the host world ($($probe.result))"}
 $me=@((J '').localPosition);$living=@(Living);Save "$out/npcs-before.json" $living
 $near=@($living|Sort-Object {[Math]::Pow($_.position[0]-$me[0],2)+[Math]::Pow($_.position[1]-$me[1],2)})
 if(!$near.Count){throw 'No living NPC in the host world'};$g=$near[0];$notes.nearest=[ordered]@{name=$g.name;handle=$g.handle;position=$g.position}
 $dx=$me[0]-$g.position[0];$dy=$me[1]-$g.position[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001);$k=[Math]::Min(3,$n/2)
 $sp=@(($g.position[0]+$k*$dx/$n),($g.position[1]+$k*$dy/$n),($g.position[2]+0.5));$notes.spawnAt=$sp
 $anchor=[Convert]::ToUInt32($g.spawnerHandle.Substring(2),16);$notes.anchor=$g.spawnerHandle
 C ("coop_mesmerize_native spawn {0:F3} {1:F3} {2:F3} {3}"-f$sp[0],$sp[1],$sp[2],$anchor);$w0=[Diagnostics.Stopwatch]::StartNew()
 # 1718: the Mesmerize component attaches after the spawn frame (1716a); wait for it, with the object's component list.
 do{Start-Sleep -Milliseconds 300;$spawn=J '.mesmerize-host.json'}while(!$spawn.component-and$w0.ElapsedMilliseconds-lt5000)
 Save "$out/spawn.json" $spawn;$notes.spawn=$spawn.result;$notes.components=@($spawn.componentVtables);$notes.componentMs=$w0.ElapsedMilliseconds
 if($spawn.result-ne'spawned'-or!$spawn.component){throw "The native object or its Mesmerize component was not found ($($spawn.result); components $(@($spawn.componentVtables)-join ' '))"}
 Frame 'spawned'
 C 'coop_mesmerize_native start';Start-Sleep -Milliseconds 400;$st=J '.mesmerize-host.json';Save "$out/start.json" $st;$notes.start=$st.result;$notes.anchorResolved=$st.anchorResolved;$notes.selfResolved=$st.selfResolved
 $w=[Diagnostics.Stopwatch]::StartNew();$samples=[Collections.Generic.List[object]]::new();$maxState=-1;$maxTargets=0;$shot=0
 do{Start-Sleep -Milliseconds 500;$m=J '.mesmerize-host.json';$pos=$null;try{$pos=@((Q '--world-npcs').npcs|Where-Object handle -eq $g.handle|ForEach-Object {$_.position})}catch{}
  $samples.Add([ordered]@{ms=$w.ElapsedMilliseconds;state=$m.state;targets=$m.targets;handles=$m.targetHandles;nearest=$pos;result=$m.result})
  if($m.state-gt$maxState){$maxState=$m.state};if($m.targets-gt$maxTargets){$maxTargets=$m.targets}
  if(($w.ElapsedMilliseconds-ge2000-and$shot-eq0)-or($w.ElapsedMilliseconds-ge8000-and$shot-eq1)-or($w.ElapsedMilliseconds-ge15000-and$shot-eq2)){Frame "active-$shot";$shot++}
 }while($w.ElapsedMilliseconds-lt($ActiveSeconds*1000))
 Save "$out/active-samples.json" @($samples);$notes.maxState=$maxState;$notes.maxTargets=$maxTargets
 C 'coop_mesmerize_native stop';Start-Sleep -Milliseconds 300;$stop=J '.mesmerize-host.json';$notes.stop=$stop.result
 $after=[Collections.Generic.List[object]]::new();$w.Restart()
 do{Start-Sleep -Milliseconds 500;$m=J '.mesmerize-host.json';$after.Add([ordered]@{ms=$w.ElapsedMilliseconds;state=$m.state;targets=$m.targets})}while($w.ElapsedMilliseconds-lt6000)
 Save "$out/stop-samples.json" @($after);Frame 'stopped';Save "$out/npcs-after.json" @(Living)
 $notes.partnerBlocked=(J '.mesmerize-host.json').partnerBlocked
 if(!(Get-Process -Id $script:hostPid -ErrorAction SilentlyContinue)){throw 'The host game exited'}
 if($maxState-lt3){throw "The native object never reached the active state (max state $maxState)"}
 if($maxTargets-lt1){throw 'The native object took no host NPC'}
 if($stop.result-ne'stopped'){throw "The stop was refused ($($stop.result))"}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Frame 'failure'}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  try{C 'coop_player_health_hold off'}catch{}
  $mutex.ReleaseMutex();$owned=$false
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope='1716 (#131): the original Mesmerize object spawned and started natively in the lab host world (1214gh), its FSM state and host NPC targets sampled, then stopped; lab restored.'})
 }
 $mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
