param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='physics-props-v1773a',[string]$Candidate='candidate-npc-combat-v1214ir',[ValidateSet('windblast','grenade','sword')][string]$Cause='windblast',[ValidateSet('host','client')][string]$Source='host',[double]$StandX=-33.0,[double]$StandY=-153.0,[double]$TargetX=-31.91,[double]$TargetY=-158.05,[double]$Pitch=8,[switch]$BreakableSync,[switch]$PropRest)
# 1773 (#178): -Source host|client acts (the partner's world can start it); -Cause sword swings at the props.
# 1772 (#178): the native reference frame word (+0x80/+0x84) of every changed prop in both worlds.
# 1771 (#178): -PropRest turns 1214ip's prop rest stream on in both worlds first.
# 1770 (#178): -BreakableSync turns the breakable stream on in both worlds first (1214io: movables in it).
# 1769 (#178): v1768 with the stand, target and pitch as parameters.
# 1768 (#178): v1767 reading every entity type except characters, lights and markers.
# 1767 (#178): v1766 with a cast check (a frame and the player states after each of two presses).
# 1766 (#178): do physics props (arkMovable, and pickups) that no player holds stay in the same place in both worlds when
# a blast moves them? Lab pair on the Dunwall tower fixture: the host knocks the two props south of the start
# (c_env_dunwall_streets_interior_bayle_125/126) with Windblast (or a grenade); every arkMovable and arkPickup is read in
# both worlds before, 5 s and 15 s after. Frames of both windows. The lab is restored at the end (Deploy-SourceClock).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
$pids=@{};foreach($r in 'host','client'){$pids[$r]=(Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json).pid}
function Props($r){$e=(& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --entities|ConvertFrom-Json).entities
 # 1768: every entity type except characters, lights, cues and markers (the chair in front of the stand moved in no
 # arkMovable or arkPickup record in physics-props-v1767a); the type is kept with the position.
 $skip='.?AVarkNPC@@','.?AVarkPlayer@@','.?AVidLight@@','.?AVarkAICue@@','.?AVarkWaypoint@@','.?AVarkSpawner@@','.?AVarkTrigger@@','.?AVarkAreaVolume@@'
 $m=[ordered]@{};foreach($x in $e){if($x.type-notin$skip-and$x.name){$m[$x.name]=@($x.position[0],$x.position[1],$x.position[2])}};$m}
function Frame($r,$name){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
 & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$name-$r.png" 2>$null|Out-Null}
function Dist($a,$b){[Math]::Sqrt([Math]::Pow($a[0]-$b[0],2)+[Math]::Pow($a[1]-$b[1],2)+[Math]::Pow($a[2]-$b[2],2))}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{cause=$Cause;source=$Source}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$id=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json
 if($dll-ne$id.dll){throw "Live DLL is not $Candidate"};Save "$out/identity.json" ([ordered]@{candidate=$Candidate;dll=$dll})
 foreach($r in 'host','client'){$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){throw "$r not in connected gameplay"}}
 # 1770: the breakable stream on in both worlds (product step 21 in product mode); its status is kept after the blast.
 if($BreakableSync){foreach($r in 'host','client'){C $r 'coop_breakable_sync on'};Start-Sleep -Seconds 3}
 if($PropRest){foreach($r in 'host','client'){C $r 'coop_prop_rest on'};Start-Sleep -Seconds 3} # 1771: 1214ip's prop rest stream
 $snaps=[ordered]@{};$snaps.before=[ordered]@{host=(Props host);client=(Props client)}
 $tx=$TargetX;$ty=$TargetY;$sx=$StandX;$sy=$StandY;$notes.target=@($tx,$ty);$yaw=[Math]::Atan2($ty-$sy,$tx-$sx)*180/[Math]::PI;$notes.stand=@($sx,$sy);$notes.yaw=$yaw
 # 1773: -Source picks the world that acts; -Cause sword swings the drawn sword (RT) at the props.
 if($Cause-eq'windblast'){C $Source 'coop_grant_named models/powers/windblast/windblast.inventoryitem'}elseif($Cause-eq'grenade'){C $Source 'coop_grant_item grenade'}
 Start-Sleep -Milliseconds 1800
 C $Source ("setviewpos {0} {1} 68.1 {2} 0"-f $sx,$sy,([Math]::Round($yaw,2)));Start-Sleep -Milliseconds 800
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch $Pitch -Quick|Out-Null
 foreach($r in 'host','client'){Frame $r 'before'}
 # 1767: physics-props-v1766a moved nothing (no cast seen). A frame 0.3 s after the press, the host's player states, and a
 # second press 2.5 s later.
 $notes.casts=[Collections.Generic.List[object]]::new()
 foreach($try in 1,2){if($Cause-eq'windblast'){& "$lab/Send-D2Input.ps1" -Role $Source -LeftTrigger 255 -DurationMs 400|Out-Null}
  else{& "$lab/Send-D2Input.ps1" -Role $Source -RightTrigger 255 -DurationMs 250|Out-Null}
  Start-Sleep -Milliseconds 300;Frame $Source "cast$try";$st=try{@((& "$lab/bin/d2_runtime_probe.exe" $pids[$Source] --player-states|ConvertFrom-Json).active|ForEach-Object {"$($_.id):$($_.name)"})}catch{@()}
  $notes.casts.Add([ordered]@{try=$try;states=$st});Start-Sleep -Milliseconds 2200}
 Start-Sleep -Seconds 5;$snaps.after5=[ordered]@{host=(Props host);client=(Props client)};foreach($r in 'host','client'){Frame $r 'after5'}
 Start-Sleep -Seconds 10;$snaps.after15=[ordered]@{host=(Props host);client=(Props client)};foreach($r in 'host','client'){Frame $r 'after15'}
 Save "$out/props.json" $snaps
 if($BreakableSync){foreach($r in 'host','client'){Copy-Item "$root/work/lab-instances/$r/coop-status.json.breakable-sync.json" "$out/breakable-sync-$r.json"}}
 if($PropRest){foreach($r in 'host','client'){Copy-Item "$root/work/lab-instances/$r/coop-status.json.prop-rest.json" "$out/prop-rest-$r.json"}}
 $rows=[Collections.Generic.List[object]]::new()
 foreach($n in $snaps.before.host.Keys){$hb=$snaps.before.host[$n];$ha=$snaps.after15.host[$n];$cb=$snaps.before.client[$n];$ca=$snaps.after15.client[$n]
  $hm=if($ha){Dist $hb $ha}else{-1};$cmv=if($cb-and$ca){Dist $cb $ca}else{-1};$apart=if($ha-and$ca){Dist $ha $ca}else{-1}
  if($hm-gt.05-or$cmv-gt.05-or$apart-gt.05-or$hm-lt0-or$cmv-lt0){$rows.Add([ordered]@{name=$n;hostMoved=[Math]::Round($hm,3);clientMoved=[Math]::Round($cmv,3);apartAfter=[Math]::Round($apart,3)})}}
 # 1772: the native reference frame of each changed prop (component +0x80 and +0x84) in both worlds, after the rest.
 $ents=@{};foreach($r in 'host','client'){$ents[$r]=@{};foreach($x in (& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --entities|ConvertFrom-Json).entities){$ents[$r][$x.name]=$x}}
 foreach($row in $rows){foreach($r in 'host','client'){$x=$ents[$r][$row.name];if(!$x){continue}
  $cs=@((& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --components $x.handle|ConvertFrom-Json).components|Where-Object type -eq '.?AVarkCpntReferenceFrame@@')
  if($cs.Count){$w=@((& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --object $cs[0].pointer|ConvertFrom-Json).words|Where-Object offset -eq '0x80');if($w.Count){$row["frame_$r"]=$w[0].word}}else{$row["frame_$r"]='none'}}}
 $notes.changed=@($rows);$notes.hostMoved=@($rows|Where-Object {$_.hostMoved-gt.05}).Count;$notes.clientMoved=@($rows|Where-Object {$_.clientMoved-gt.05}).Count
 $notes.apart=@($rows|Where-Object {$_.apartAfter-gt.05}).Count
 if(!$(if($Source-eq'host'){$notes.hostMoved}else{$notes.clientMoved})){$failure="The $Cause moved no prop in the $Source world"}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){$mutex.ReleaseMutex();$owned=$false
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope='1766 (#178): physics props moved by a host blast, read in both worlds (measurement: passed means the blast moved a prop in the host world; the notes say whether the client world followed)'})
 $mutex.Dispose()}
if($failure){throw $failure};"MEASURED host moved $($notes.hostMoved), client moved $($notes.clientMoved), apart $($notes.apart)"
