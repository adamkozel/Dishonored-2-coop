param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='power-cast-probe-v1334a',[string]$Role='host',[string[]]$Powers=@('blink','windblast','swarm','possession'),[int]$HoldMs=900,[switch]$PeerAway)
# 1334 (#11): why do some granted powers never cast in product mode (take 1332a: windblast, possession, swarm for Corvo;
# far reach, domino, doppelganger, mesmerize for Emily: only state 0 on the source) while blink, dark vision, bend time
# and shadow walk do? -PeerAway moves the other role to the stairs (about 28 m away) first. For each power: grant it by name (1214cm), read its native item (quantity, equipped, started) and
# the player power and mana state, press LT, poll the native player states, then reload the lab checkpoint.
$Powers=@($Powers|ForEach-Object {$_-split','}|Where-Object {$_})
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
$names=@{blink='models/powers/blink/blink.inventoryitem';windblast='models/powers/windblast/windblast.inventoryitem';swarm='models/powers/devouring_swarm/devouring_swarm.inventoryitem';possession='models/powers/possession/possession.inventoryitem';farreach='models/powers/grapple/grapple.inventoryitem';domino='models/powers/phantasmallink/phantasmal_link.inventoryitem';doppelganger='models/powers/doppelganger/doppelganger.inventoryitem';mesmerize='models/powers/mesmerize/mesmerize.inventoryitem'}
$modes=@{blink='--player-power';windblast='--player-windblast';swarm='--player-swarm';possession='--player-possession';farreach='--player-grapple';domino='--player-domino';doppelganger='--player-doppelganger';mesmerize='--player-mesmerize'}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
$l=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
function Q($mode){$raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid $mode 2>&1;[ordered]@{exit=$LASTEXITCODE;out=(($raw|Out-String).Trim())}}
$rows=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($p in $Powers){$row=[ordered]@{power=$p}
  C $Role 'setviewpos -35 -153 68.1 0 0';if($PeerAway){C $(if($Role-eq'host'){'client'}else{'host'}) 'setviewpos -45.3 -179 65.1 0 0'};Start-Sleep -Milliseconds 600
  $row.before=Q $modes[$p];C $Role "coop_grant_named $($names[$p])";Start-Sleep -Milliseconds 2000
  $row.grant=(Get-Content "$root/work/lab-instances/$Role/coop-status.json.observer-camera.json" -Raw|ConvertFrom-Json).itemGrant.named
  $row.afterGrant=Q $modes[$p];$row.powerState=Q '--player-power'
  & "$lab/Send-D2Input.ps1" -Role $Role -LeftTrigger 255 -DurationMs $HoldMs|Out-Null;$w=[Diagnostics.Stopwatch]::StartNew();$polls=[Collections.Generic.List[object]]::new()
  while($w.ElapsedMilliseconds-lt($HoldMs+1500)){$s=Q '--player-states';$polls.Add([ordered]@{ms=$w.ElapsedMilliseconds;states=$s.out});Start-Sleep -Milliseconds 120}
  $row.polls=@($polls);$row.afterCast=Q $modes[$p];$rows.Add($row)
  & "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"}
}finally{
 $rows|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
foreach($r in $rows){"$($r.power): grant $($r.grant.result); item after grant: $($r.afterGrant.out.Substring(0,[Math]::Min(260,$r.afterGrant.out.Length)))"}
