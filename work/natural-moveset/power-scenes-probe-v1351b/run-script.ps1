param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='power-scenes-probe-v1351a',[string]$Role='client',[string]$Power='grapple',
 [string]$Plan='lt:3000@0;ltx:200@1300;neutral:100@1600;lt:1200@3800',[int]$WatchMs=7000,[double]$Pitch=20,[string]$Place='-35 -153 68.1 0 0')
# 1351 (#11, emily_far_reach_aim_cancel_release_arrive): which native scenes does a power's aim, X cancellation and release
# play? In coverage-v1350a the partner presented Far Reach's cast-in (variant 4) for 2 samples, never the aim loop (5),
# and no release after a cancel, although the long Far Reach (no cancel) presented the release (6). Grant the power by
# name (1214cm), run the input plan ("button:ms@start;..."; lt, ltx, neutral, rt), and record the player midnight scenes
# and native player states every ~70 ms; then reload the lab checkpoint and restore the note baseline. No product controller.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
function Q($mode){$raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode 2>&1;try{($raw|Out-String)|ConvertFrom-Json}catch{[pscustomobject]@{raw=(($raw|Out-String).Trim())}}}
$steps=@($Plan-split';'|Where-Object {$_}|ForEach-Object {$a=$_-split'[:@]';[pscustomobject]@{button=$a[0];ms=[int]$a[1];at=[int]$a[2];sent=$false}})
$trace=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 C $Role "setviewpos $Place";Start-Sleep -Milliseconds 800
 C $Role "coop_grant_named models/powers/$Power/$Power.inventoryitem";Start-Sleep -Milliseconds 1800
 $grant=(Get-Content "$root/work/lab-instances/$Role/coop-status.json.observer-camera.json" -Raw|ConvertFrom-Json).itemGrant.named
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch $Pitch -Quick|Out-Null
 $w=[Diagnostics.Stopwatch]::StartNew()
 while($w.ElapsedMilliseconds-lt$WatchMs){$t=$w.ElapsedMilliseconds
  foreach($s in $steps){if(!$s.sent-and$t-ge$s.at){$s.sent=$true
   switch($s.button){'lt'{& "$lab/Send-D2Input.ps1" -Role $Role -LeftTrigger 255 -DurationMs $s.ms|Out-Null};'ltx'{& "$lab/Send-D2Input.ps1" -Role $Role -LeftTrigger 255 -Button X -DurationMs $s.ms|Out-Null}
    'rt'{& "$lab/Send-D2Input.ps1" -Role $Role -RightTrigger 255 -DurationMs $s.ms|Out-Null};default{& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs $s.ms|Out-Null}}
   $trace.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;sent=$s.button})}}
  $m=Q '--midnight';$st=Q '--player-states'
  $scenes=@($m.instances|Where-Object {$_.asset-like'*player*'}|ForEach-Object {"$($_.asset)|$($_.subscene)|$($_.sceneDurationMs)|ch$($_.playerChannel)|s$($_.state)"})
  $trace.Add([pscustomobject]@{ms=$t;scenes=$scenes;active=@($st.active|ForEach-Object {"$($_.id):$($_.type)"});powerScene=$st.powerScene;powerSceneValid=$st.powerSceneValid;position=$st.position})}
}finally{
 try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Quick|Out-Null}catch{}
 try{& "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"}catch{"restore failed: $_"|Add-Content "$out/checkpoint-restore.log"}
 try{& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration" *>> "$out/note-restoration.log"}catch{"note restore failed: $_"|Add-Content "$out/note-restoration.log"}
 [ordered]@{power=$Power;plan=$Plan;grant=$grant;trace=@($trace)}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
"grant $($grant.result)"
$prev=''
foreach($r in $trace){if($r.sent){"$($r.ms) SEND $($r.sent)";continue};$k="$(@($r.scenes)-join ' + ') || $(@($r.active)-join ',') || pv=$($r.powerSceneValid)";if($k-ne$prev){"$($r.ms) $k";$prev=$k}}
