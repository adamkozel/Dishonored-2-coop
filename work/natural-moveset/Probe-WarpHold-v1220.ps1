param([string]$Take='warp-hold-probe-v1220',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo')
# 1220: replay of the 1218c staging - host to the vise approach, then the
# CLIENT warps ~26 m to its observer-camera spot, holds the camera and re-parks
# 0.8 m on. The HOST's view of the client partner is sampled, and both peers'
# .pose-clock.json (mapTime rejections) are saved. Earlier header (1219b):
# 1219b: the same diagnostic on Edge of the World, warping the host from its
# mission start to the vise approach (the 1218 hold: client partner 30.2 m off
# for 25 s). Earlier header (1219):
# Diagnostic (candidate 1213c): with the product controller armed in the Dunwall
# fixture, the host is warped ~31 m (-33,-153 -> -42.96,-122 arena) and back.
# The client's partner presentation is sampled every 300 ms: probeTargetPosition
# (what the DLL commands), probeRenderPosition (native render), remotePosition
# (raw network root), follow/bank/stream, movement clock rejection and the
# avatar transform counters. Separates a held pipeline from a refused native
# transform (1204f/1218 warp holds). Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='Neutral',[int]$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMapId})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);return @{ready=$ready;log=@($log)}}
function SnapHost($leg,$ms){$s=S host;$m=S host '.movement.json';$x=S host '.shadow-stream.json';$c=S client
 $rows.Add(@{leg=$leg;ms=$ms;target=$s.probeTargetPosition;render=$s.probeRenderPosition;remote=$s.remotePosition;clientLocal=$c.localPosition;follow=$s.probeFollowEnabled;fresh=$s.remoteFresh;bank=$m.active;stream=$x.ready;clockRejection=$m.clockRejection;product=(S host '.product.json').result;remoteLogic=$s.remoteGameLogicState;remotePending=$s.remotePendingGameLogicState})}
function Snap($leg,$ms){$s=S client;$m=S client '.movement.json';$x=S client '.shadow-stream.json';$a=S client '.avatar-perception.json'
 $rows.Add(@{leg=$leg;ms=$ms;target=$s.probeTargetPosition;render=$s.probeRenderPosition;remote=$s.remotePosition;follow=$s.probeFollowEnabled;fresh=$s.remoteFresh;bank=$m.active;stream=$x.ready;clockRejection=$m.clockRejection;continuous=$a.continuousTransforms;teleport=$a.teleportTransforms;product=(S client '.product.json').result})}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 $dirty=$true
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 $g=WaitGameplay 300000;Save "$out/load-log.json" $g.log;if(!$g.ready){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'});C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'-and(S client '.movement.json').active-and(S client '.shadow-stream.json').ready}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 if(!$ok){throw 'Product controller/presentation not armed'}
 Start-Sleep -Seconds 3
 $v=@((Q host '--entities').entities|Where-Object name -eq 'env_vise_01_1_3')[0].position;Save "$out/vise.json" $v
 C host ("setviewpos {0:F3} {1:F3} {2:F3} 180 0"-f($v[0]+1.0),$v[1],($v[2]+0.7));Start-Sleep -Milliseconds 1500
 $w.Restart();while($w.ElapsedMilliseconds-lt3000){SnapHost 'host-at-vise' $w.ElapsedMilliseconds;Start-Sleep -Milliseconds 300}
 $sp=(Q host '--player').candidatePosition;$oa=70*[Math]::PI/180;$ox=$sp[0]+3.0*[Math]::Cos($oa);$oy=$sp[1]+3.0*[Math]::Sin($oa);$oyaw=[Math]::Atan2($sp[1]-$oy,$sp[0]-$ox)*180/[Math]::PI
 C client ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$ox,$oy,($sp[2]+1.75),$oyaw);Start-Sleep -Milliseconds 700
 $w.Restart();while($w.ElapsedMilliseconds-lt3000){SnapHost 'client-warped' $w.ElapsedMilliseconds;Start-Sleep -Milliseconds 300}
 for($h=0;$h-lt3;$h++){C client 'coop_observer_camera hold';Start-Sleep -Milliseconds 500;$hold=S client '.observer-camera.json';if($hold.active){break};C client 'coop_observer_camera off';Start-Sleep -Milliseconds 700}
 C client ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f($ox+0.8*[Math]::Cos($oa)),($oy+0.8*[Math]::Sin($oa)),($sp[2]+1.75),$oyaw)
 $w.Restart();while($w.ElapsedMilliseconds-lt15000){SnapHost 'held-reparked' $w.ElapsedMilliseconds;Start-Sleep -Milliseconds 300}
 C client 'coop_observer_camera off'
 $w.Restart();while($w.ElapsedMilliseconds-lt8000){SnapHost 'released' $w.ElapsedMilliseconds;Start-Sleep -Milliseconds 300}
 foreach($r in 'host','client'){Copy-Item "$root/work/lab-instances/$r/coop-status.json.pose-clock.json" "$out/pose-clock-$r.json";Copy-Item "$root/work/lab-instances/$r/coop-status.json.observer-camera.json" "$out/observer-camera-$r.json"}
 Save "$out/samples.json" @($rows)
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Save "$out/samples.json" @($rows)}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_observer_camera off';C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;samples=$rows.Count;scope='1218c replay: host view of the client partner across the client warp + observer camera hold; pose-clock diagnostics saved; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
