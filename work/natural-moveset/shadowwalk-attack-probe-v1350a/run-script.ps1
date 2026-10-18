param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='shadowwalk-attack-probe-v1350a',[string]$Role='client',[int]$AttackHoldMs=250,[string]$AttackButton='rt')
# 1350 (#11, emily_shadow_walk_enter_move_attack_exit): which native scenes does an attack from Shadow Walk play? The game
# has power_shadow_walk_attack_out.midnightscene (confirmed static, resource index); the receiver maps only trans_in (32),
# trans_in_sneak (35) and trans_out (34). Grant Shadow Walk by name (1214cm), enter it with LT, crawl forward, attack
# (RT, or the given button), and record the player-channel midnight scenes and native player states every 100 ms; then
# reload the lab checkpoint and restore the note baseline. Read-only otherwise; no product controller.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
function Q($mode){$raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode 2>&1;try{($raw|Out-String)|ConvertFrom-Json}catch{[pscustomobject]@{raw=(($raw|Out-String).Trim())}}}
$trace=[Collections.Generic.List[object]]::new()
function Watch($label,$ms){$w=[Diagnostics.Stopwatch]::StartNew()
 while($w.ElapsedMilliseconds-lt$ms){$m=Q '--midnight';$st=Q '--player-states'
  $scenes=@($m.instances|Where-Object {$_.asset-like'*player*'-or$_.playerChannel-ge0}|ForEach-Object {"$($_.asset)|$($_.subscene)|$($_.sceneDurationMs)|ch$($_.playerChannel)|s$($_.state)"})
  $trace.Add([pscustomobject]@{phase=$label;ms=$w.ElapsedMilliseconds;scenes=$scenes;states=$st});Start-Sleep -Milliseconds 60}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 C $Role 'setviewpos -34 -153 68.1 90 0';Start-Sleep -Milliseconds 800
 C $Role 'coop_grant_named models/powers/shadow_walk/shadow_walk.inventoryitem';Start-Sleep -Milliseconds 1800
 $grant=(Get-Content "$root/work/lab-instances/$Role/coop-status.json.observer-camera.json" -Raw|ConvertFrom-Json).itemGrant.named
 & "$lab/Send-D2Input.ps1" -Role $Role -LeftTrigger 255 -DurationMs 250|Out-Null;Watch 'enter' 1600
 & "$lab/Send-D2Input.ps1" -Role $Role -LeftY 14000 -DurationMs 1200|Out-Null;Watch 'crawl' 1300
 switch($AttackButton){'rt'{& "$lab/Send-D2Input.ps1" -Role $Role -RightTrigger 255 -DurationMs $AttackHoldMs|Out-Null}
  default{& "$lab/Send-D2Input.ps1" -Role $Role -Button $AttackButton -DurationMs $AttackHoldMs|Out-Null}}
 Watch 'attack' 4000
 & "$lab/Send-D2Input.ps1" -Role $Role -LeftTrigger 255 -DurationMs 250|Out-Null;Watch 'exit' 2500
}finally{
 try{& "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"}catch{"restore failed: $_"|Add-Content "$out/checkpoint-restore.log"}
 try{& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration" *>> "$out/note-restoration.log"}catch{"note restore failed: $_"|Add-Content "$out/note-restoration.log"}
 [ordered]@{grant=$grant;trace=@($trace)}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
$seen=@($trace|ForEach-Object {foreach($s in $_.scenes){"$($_.phase): $s"}}|Select-Object -Unique);"grant $($grant.result)";$seen
