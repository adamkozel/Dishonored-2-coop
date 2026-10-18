param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='pickpocket-probe-v1359a',[string]$Role='client')
# 1359 (#11, pickup_loot_steal): where does the native use focus land on tutorial guard 2's belt key? Coverage 1358b swept
# yaw and pitch at his right side and read no focus at all; 1254h showed a Pickpocket prompt there during a choke attempt.
# Wait until he stands still at his patrol pause, crouch (stance-aware), then for each spot around him (behind, left, right,
# at 0.6/0.9 m) and pitch read the focused entity (arkCpntPlayerSensorInteraction+0x80, 1133). At the first focus on
# anything other than the floor, press X and record the native states. Then reload the lab checkpoint and the note baseline.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$inv=[Globalization.CultureInfo]::InvariantCulture
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
function Q($mode,$arg=0){for($a=0;$a-lt6;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 30};throw "read failed $mode"}
function F($v){([double]$v).ToString('F4',$inv)}
$rows=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pl=Q '--player';$ents=(Q '--entities').entities;$me=@($ents|Where-Object pointer -eq $pl.player)
 $sensor=@((Q '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@')[0].pointer
 $byHandle=@{};foreach($e in $ents){$byHandle[[Convert]::ToUInt32(($e.handle-replace'^0x',''),16)]=$e}
 function Focus{$o=Q '--object' $sensor;$w=@($o.words|Where-Object offset -eq '0x80');$h=[uint32]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32);$e=$byHandle[$h];"{0:x} {1}" -f $h,$(if($e){$e.name}else{''})}
 $w=[Diagnostics.Stopwatch]::StartNew();$g=$null
 do{$a=@((Q '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'});if($a.Count-and[Math]::Sqrt([Math]::Pow([double]$a[0].position[0]+47.95,2)+[Math]::Pow([double]$a[0].position[1]+152.45,2))-lt.4){Start-Sleep -Milliseconds 600;$b=@((Q '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'});if([Math]::Sqrt([Math]::Pow([double]$b[0].position[0]-[double]$a[0].position[0],2)+[Math]::Pow([double]$b[0].position[1]-[double]$a[0].position[1],2))-lt.03){$g=$b[0]}}else{Start-Sleep -Milliseconds 500}}while(!$g-and$w.ElapsedMilliseconds-lt150000)
 if(!$g){throw 'Guard never paused'};$gx=[double]$g.position[0];$gy=[double]$g.position[1];$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$fl=[Math]::Sqrt($fx*$fx+$fy*$fy);$fx/=$fl;$fy/=$fl
 $st=[int](Q '--player-states').stance;if($st-ne1){& "$lab/Send-D2Input.ps1" -Role $Role -Button B -DurationMs 150|Out-Null;Start-Sleep -Milliseconds 600}
 $spots=@(@('behind',-1,0),@('right',-.4,1),@('left',-.4,-1),@('rightside',0,1),@('leftside',0,-1))
 $pressed=$false
 foreach($sp in $spots){foreach($d in .6,.9){if($pressed){break}
   $sx=$gx+$d*($sp[1]*$fx+$sp[2]*$fy);$sy=$gy+$d*($sp[1]*$fy-$sp[2]*$fx);$yaw=[Math]::Atan2($gy-$sy,$gx-$sx)*180/[Math]::PI
   C $Role ('setviewpos {0} {1} 67.4 {2} 0' -f (F $sx),(F $sy),(F $yaw));Start-Sleep -Milliseconds 500
   foreach($pt in 0,15,30,45){try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch $pt -Quick|Out-Null}catch{};Start-Sleep -Milliseconds 250
    $f=Focus;$stance=[int](Q '--player-states').stance;$rows.Add([pscustomobject]@{spot=$sp[0];d=$d;pitch=$pt;focus=$f;stance=$stance})
    if($f-match' \S'){& "$lab/Send-D2Input.ps1" -Role $Role -Button X -DurationMs 300|Out-Null;$trace=@();for($k=0;$k-lt12;$k++){$trace+=,@((Q '--player-states').active|ForEach-Object {$_.id});Start-Sleep -Milliseconds 120}
     $rows.Add([pscustomobject]@{pressedAt=$f;states=$trace});$pressed=$true;break}}}}
}finally{
 try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Quick|Out-Null}catch{}
 try{& "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"}catch{"restore failed: $_"|Add-Content "$out/checkpoint-restore.log"}
 try{& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration" *>> "$out/note-restoration.log"}catch{"note restore failed: $_"|Add-Content "$out/note-restoration.log"}
 @($rows)|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
$rows|ForEach-Object {if($_.pressedAt){"PRESSED at $($_.pressedAt): $(@($_.states|ForEach-Object {$_-join '+'})-join ' | ')"}else{"$($_.spot) $($_.d) p$($_.pitch) st$($_.stance): $($_.focus)"}}
