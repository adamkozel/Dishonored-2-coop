param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='blink-assault-probe-v1360a',[string]$Role='host',[double]$Distance=6,[int]$AttackDelayMs=120)
# 1360 (#11, corvo_blink_pull_and_kick_variants, kick_or_shove_if_present): what does Blink Assault play? Its scene
# midnight/gameplay/player/base/combat/blink_attack_focus.midnightscene has subscene "kick", 800 ms, the native
# ply_body_throw_hit_kick animations and an NPC group (confirmed static), the motion the receiver already presents as the kick
# (combat variant 17). Corvo is granted Blink and the blink_assault upgrade by name (1214cm), stands $Distance m from tutorial
# guard 2 while he pauses on patrol, aims Blink at him, releases, then attacks; the player scenes and native states are polled
# every ~70 ms. Then the lab checkpoint and note baseline are restored. No product controller.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$inv=[Globalization.CultureInfo]::InvariantCulture
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
function Q($mode,$arg=0){for($a=0;$a-lt6;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 30};throw "read failed $mode"}
function F($v){([double]$v).ToString('F4',$inv)}
$trace=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 C $Role 'coop_grant_named models/powers/blink/blink.inventoryitem';Start-Sleep -Milliseconds 1500
 C $Role 'coop_grant_named components/characters/player/base/attributes/blink_assault.inventoryitem';Start-Sleep -Milliseconds 1500
 $grant=(Get-Content "$root/work/lab-instances/$Role/coop-status.json.observer-camera.json" -Raw|ConvertFrom-Json).itemGrant.named
 $w=[Diagnostics.Stopwatch]::StartNew();$g=$null
 do{$a=@((Q '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'});if($a.Count-and[Math]::Sqrt([Math]::Pow([double]$a[0].position[0]+47.95,2)+[Math]::Pow([double]$a[0].position[1]+152.45,2))-lt.4){Start-Sleep -Milliseconds 600;$b=@((Q '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'});if([Math]::Sqrt([Math]::Pow([double]$b[0].position[0]-[double]$a[0].position[0],2)+[Math]::Pow([double]$b[0].position[1]-[double]$a[0].position[1],2))-lt.03){$g=$b[0]}}else{Start-Sleep -Milliseconds 500}}while(!$g-and$w.ElapsedMilliseconds-lt150000)
 if(!$g){throw 'Guard never paused'};$gx=[double]$g.position[0];$gy=[double]$g.position[1];$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$fl=[Math]::Sqrt($fx*$fx+$fy*$fy);$fx/=$fl;$fy/=$fl
 # Stand in front of him along his facing (the walkway he faces), facing him.
 $sx=$gx+$Distance*$fx;$sy=$gy+$Distance*$fy;$yaw=[Math]::Atan2($gy-$sy,$gx-$sx)*180/[Math]::PI
 C $Role ('setviewpos {0} {1} 68.1 {2} 0' -f (F $sx),(F $sy),(F $yaw));Start-Sleep -Milliseconds 600;try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 6 -Quick|Out-Null}catch{}
 $ws=[Diagnostics.Stopwatch]::StartNew();$sent=0
 while($ws.ElapsedMilliseconds-lt6000){$t=$ws.ElapsedMilliseconds
  if($sent-eq0-and$t-ge300){& "$lab/Send-D2Input.ps1" -Role $Role -LeftTrigger 255 -DurationMs 1000|Out-Null;$trace.Add([pscustomobject]@{ms=$t;sent='LT 1000'});$sent=1}
  elseif($sent-eq1-and$t-ge(1300+$AttackDelayMs)){& "$lab/Send-D2Input.ps1" -Role $Role -RightTrigger 255 -DurationMs 250|Out-Null;$trace.Add([pscustomobject]@{ms=$t;sent='RT'});$sent=2}
  $m=Q '--midnight';$st=Q '--player-states'
  $trace.Add([pscustomobject]@{ms=$t;scenes=@($m.instances|Where-Object {$_.asset-like'*player*'}|ForEach-Object {"$(($_.asset-split'/')[-1])|$($_.subscene)|$($_.sceneDurationMs)"});active=@($st.active|ForEach-Object {$_.id});position=$st.position})}
}finally{
 try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Quick|Out-Null}catch{}
 try{& "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"}catch{"restore failed: $_"|Add-Content "$out/checkpoint-restore.log"}
 try{& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration" *>> "$out/note-restoration.log"}catch{"note restore failed: $_"|Add-Content "$out/note-restoration.log"}
 [ordered]@{grant=$grant;distance=$Distance;trace=@($trace)}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
"grant $($grant.result)";$prev=''
foreach($r in $trace){if($r.sent){"$($r.ms) SEND $($r.sent)";continue};$k="$(@($r.scenes)-join ' + ') || $(@($r.active)-join ',')";if($k-ne$prev){"$($r.ms) $k  @$(@($r.position|ForEach-Object {[Math]::Round([double]$_,1)})-join ',')";$prev=$k}}
