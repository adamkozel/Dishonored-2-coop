param([string]$Take='mission-usable-v1138',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[string]$TargetName='',[string[]]$Candidates=@('script_rich_small_01_sndctrl_14','env_interiors_door_1_19','env_interiors_rich_display_case_01_2','env_interiors_door_left_1','env_interiors_door_middle_1','env_interiors_right_door','script_rich_small_01_sndctrl_14','script_rich_small_01_sndctrl_13'),[double]$EyeZ=14.55,[double[]]$AimHeights=@(0.9,1.55,1.3,1.8,0.5),[double[]]$Lateral=@(0),[int[]]$Bearings=@(135,90,180,45,225,0,270,315),[double]$Stand=1.2,[double[]]$Normal=@(1,0),[double]$FeetOffset=-0.4)
# Stage 5 on a mission map (candidate 1123): read-only usable census on both
# roles, then the structural two-state toggle family is armed with
# coop_usable_sync session. The host opens one shared toggle with ordinary
# native use (X); the client closes it with ordinary native use. Both worlds
# must converge on the native state. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r $suffix"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Y($r){S $r '.usable-sync.json'}
function Modal(){foreach($r in 'host','client'){if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}}
function Capture($name,$seconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$name-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$name-$r-capture.log" -PassThru))}}
function WaitCaptures(){foreach($c in @($captures)){if(!$c.WaitForExit(90000)-or$c.ExitCode){throw 'Recording failed'}};$captures.Clear()}
function Entry($r,$name){@((Y $r).entries|Where-Object name -eq $name)}
function Pair($name,$target,$tag,[int]$ms=7000){
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{$ok=$true;$pair=@{};foreach($r in 'host','client'){
   $e=Entry $r $name;$u=Q $r '--usable' $script:handles[$r];$pair[$r]=@{native=$u;channel=$e};$samples.Add(@{ms=$w.ElapsedMilliseconds;role=$r;tag=$tag;current=$u.current;pending=$u.pending})
   if($e.Count-ne1-or!$e[0].enabled-or!$e[0].ready-or$e[0].target-ne$target-or$e[0].applied-ne$e[0].revision-or$e[0].inFlight-or$e[0].awaiting-or$u.current-ne$target-or$u.pending-ne-1-or$u.transitioning){$ok=$false}}
  if(!$ok){Start-Sleep -Milliseconds 80}}while(!$ok-and$w.ElapsedMilliseconds-lt$ms)
 Save "$out/pair-$tag.json" $pair;if(!$ok){throw "Shared toggle did not converge $name target $target ($tag)"};$w.ElapsedMilliseconds}
function FocusInit($r){
 # 1133 (confirmed runtime): arkCpntPlayerSensorInteraction+0x80 high dword is
 # the focused entity handle (0x3fff.... when none).
 $pl=Q $r '--player';$ents=(Q $r '--entities').entities;$me=@($ents|Where-Object pointer -eq $pl.player);if($me.Count-ne1){throw "Player entity not unique $r"}
 $s=@((Q $r '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@');if($s.Count-ne1){throw "Interaction sensor not unique $r"}
 $script:sensor[$r]=$s[0].pointer;$script:byHandle[$r]=@{};foreach($e in $ents){$script:byHandle[$r][[Convert]::ToUInt32(($e.handle-replace'^0x',''),16)]=$e}}
function Focus($r){
 $o=Q $r '--object' $script:sensor[$r];$w=@($o.words|Where-Object offset -eq '0x80');if($w.Count-ne1){throw 'Sensor word missing'}
 $h=[uint32]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32);$e=$script:byHandle[$r][$h]
 if(!$e){return @{handle=('0x{0:x}'-f$h);name=$null}};return @{handle=$e.handle;name=$e.name;type=$e.type;position=$e.position}}
function UseOrdinary($r,$target,$want){
 # Stand off the toggle towards open floor, aim at native heights, press X.
 # Wall windows: stand on either side of the wall normal (Dunwall geometry:
 # 1.28 m off, feet 0.4 m below the native origin, aim +0.6 closed / +1.24 open).
 $p=$target.position;$tries=[Collections.Generic.List[object]]::new()
 foreach($bearing in $Bearings){
  $ang=[Math]::Atan2($Normal[1],$Normal[0])+$bearing*[Math]::PI/180;$sx=$p[0]+$Stand*[Math]::Cos($ang);$sy=$p[1]+$Stand*[Math]::Sin($ang);$yaw=[Math]::Atan2(-[Math]::Sin($ang),-[Math]::Cos($ang))*180/[Math]::PI
  C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$sx,$sy,$EyeZ,$yaw);Start-Sleep -Milliseconds 1500;Modal
  # Reject stands inside geometry: the native player must settle near the stand.
  $at=(Q $r '--player').candidatePosition;if([Math]::Sqrt([Math]::Pow($at[0]-$sx,2)+[Math]::Pow($at[1]-$sy,2))-gt.6-or[Math]::Abs($at[2]-($EyeZ-1.7))-gt.8){$tries.Add(@{bearing=$bearing;rejectedStand=$at});continue}
  # Once the native driver is known (host focus), aim at its live entity
  # position (the sash handle moves with the window state).
  $points=[Collections.Generic.List[object]]::new()
  if($script:driver){$d=@((Q $r '--entities').entities|Where-Object name -eq $script:driver);if($d.Count-eq1){foreach($dz in 0,0.1,-0.1,0.2,-0.2){$points.Add(@{lat=0;h=$dz;x=$d[0].position[0];y=$d[0].position[1];z=$d[0].position[2]+$dz})}}}
  else{foreach($lat in $Lateral){foreach($h in $AimHeights){$points.Add(@{lat=$lat;h=$h;x=($p[0]-$lat*$Normal[1]);y=($p[1]+$lat*$Normal[0]);z=($p[2]+$h)})}}}
  foreach($pt in $points){$lat=$pt.lat;$h=$pt.h
   try{& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X $pt.x -Y $pt.y -Z $pt.z -Evidence "$out/aim-$r-$want-$bearing-$lat-$h.json"}catch{$tries.Add(@{bearing=$bearing;lateral=$lat;height=$h;aim=$_.Exception.Message});continue}
   # Diff the native census around the press: the prompt may belong to a
   # neighbouring usable (e.g. sink cabinet vs basin); follow what changed.
   $before=@{};foreach($u in (Q $r '--usable-catalog').usables){$before[$u.name]=$u.current}
   Start-Sleep -Milliseconds 250;$f=Focus $r
   if(!$f.name-or$f.type-ne'.?AVarkUsable@@'-or[Math]::Sqrt([Math]::Pow($f.position[0]-$p[0],2)+[Math]::Pow($f.position[1]-$p[1],2)+[Math]::Pow($f.position[2]-$p[2],2))-gt2.5-or($script:driver-and$f.name-ne$script:driver)){
    $tries.Add(@{bearing=$bearing;lateral=$lat;height=$h;focus=$f});continue}
   I $r X 150;I $r;Start-Sleep -Milliseconds 1600
   $changed=@((Q $r '--usable-catalog').usables|Where-Object {$before.ContainsKey($_.name)-and$before[$_.name]-ne$_.current}|ForEach-Object {@{name=$_.name;from=$before[$_.name];to=$_.current;toggle=$_.toggle}})
   $tries.Add(@{bearing=$bearing;lateral=$lat;height=$h;stand=@($sx,$sy);focus=$f;changed=$changed})
   if($changed.Count){
    $hit=@($changed|Where-Object {$_.toggle-and$script:sharedNames-contains$_.name})
    if($hit.Count-ne1){Save "$out/use-$r-$want-$($script:target.name).json" @($tries);throw "Native use changed non-shared or several usables on $r"}
    if($hit[0].name-ne$script:target.name){
     if($r-ne'host'){Save "$out/use-$r-$want-$($script:target.name).json" @($tries);throw 'Client use changed a different usable'}
     $script:target=$script:census.host.usables|Where-Object name -eq $hit[0].name|Select-Object -First 1
     foreach($q in 'host','client'){$e=@((Q $q '--entities').entities|Where-Object name -eq $script:target.name);if($e.Count-ne1){throw "Unique entity required $q"};$script:handles[$q]=$e[0].handle}}
    if($hit[0].to-ne$want){$script:want=$hit[0].to}
    $script:driver=$f.name;Save "$out/use-$r-$want-$($script:target.name).json" @($tries);return @{bearing=$bearing;lateral=$lat;height=$h;driver=$f.name;name=$hit[0].name;from=$hit[0].from;to=$hit[0].to}}}}
 Save "$out/use-$r-$want-$($script:target.name).json" @($tries);throw "Ordinary native use did not reach $want on $r"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1123c/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1123c candidate'}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;mapInfo=$MapInfo}
 $dirty=$true
 C host "campaign $MapInfo $Chaos corvo";C client "campaign $MapInfo $Chaos emily"
 $w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{
  Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt240000)
 Save "$out/load-log.json" @($log)
 if(!$ready){throw 'Both instances did not reach mission gameplay'}
 Start-Sleep -Seconds 2
 if(!(S host).remoteFresh-or!(S client).remoteFresh){throw 'Transport not fresh on mission map'}
 $census=@{};foreach($r in 'host','client'){Save "$out/player-$r.json" (Q $r '--player');$census[$r]=Q $r '--usable-catalog';Save "$out/usable-census-$r.json" $census[$r]}
 # Shared toggles: identical name and native rules on both characters.
 $sig={param($u)"$($u.declaration)|$($u.states)|$(($u.entries|ForEach-Object {$_ -join ','}) -join ';')"}
 $hostToggles=@{};foreach($u in $census.host.usables|Where-Object toggle){$hostToggles[$u.name]=$u}
 $shared=@($census.client.usables|Where-Object {$_.toggle-and$hostToggles.ContainsKey($_.name)-and(& $sig $_)-eq(& $sig $hostToggles[$_.name])})
 $spawn=(Get-Content "$out/player-host.json" -Raw|ConvertFrom-Json).candidatePosition
 $pick=@($Candidates|ForEach-Object {$n=$_;$shared|Where-Object name -eq $n})
 Save "$out/shared-toggles.json" @{hostToggles=$hostToggles.Count;clientToggles=@($census.client.usables|Where-Object toggle).Count;shared=@($shared|ForEach-Object name);order=@($pick|ForEach-Object name)}
 if(!$pick.Count){throw 'No shared closed toggle on this map'}
 $script:target=$pick[0];$target=$script:target;$script:census=$census;$script:sharedNames=@($shared|ForEach-Object name);$script:handles=@{}
 foreach($r in 'host','client'){$e=@((Q $r '--entities').entities|Where-Object name -eq $target.name);if($e.Count-ne1){throw "Unique entity required $r"};$script:handles[$r]=$e[0].handle}
 $script:sensor=@{};$script:byHandle=@{};$script:driver=$null;foreach($r in 'host','client'){FocusInit $r}
 foreach($r in 'host','client'){I $r;C $r 'coop_usable_sync session'}
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{$ok=$true;foreach($r in 'host','client'){$y=Y $r;$e=@($y.entries|Where-Object name -eq $target.name);if(!$y.enabled-or!$y.session.enabled-or$e.Count-ne1-or!$e[0].ready-or$e[0].applied-ne$e[0].revision){$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 100}}while(!$ok-and$w.ElapsedMilliseconds-lt10000)
 foreach($r in 'host','client'){Save "$out/sync-armed-$r.json" (Y $r)}
 if(!$ok){throw 'Mission usable family did not pair'}
 $paired=@{};foreach($r in 'host','client'){$paired[$r]=@((Y $r).entries|Where-Object ready).Count}
 # Partner standing beside the source so both recordings frame the toggle.
 $hostUse=$null;$misses=[Collections.Generic.List[object]]::new()
 foreach($cand in $pick){
  $script:target=$cand;foreach($q in 'host','client'){$e=@((Q $q '--entities').entities|Where-Object name -eq $cand.name);if($e.Count-ne1){throw "Unique entity required $q"};$script:handles[$q]=$e[0].handle}
  try{$hostUse=UseOrdinary 'host' $cand (1-$cand.current);break}catch{$misses.Add(@{candidate=$cand.name;miss=$_.Exception.Message});Save "$out/misses.json" @($misses)}}
 if(!$hostUse){throw 'No candidate toggle could be driven by ordinary native use'}
 $target=$script:target;$openMs=Pair $target.name $hostUse.to 'host-use'
 # Recorded section: host returns its own native use, then the client drives
 # the toggle from the state it left (closed), then the host returns it again.
 Capture 'usable' 120;Start-Sleep -Milliseconds 800
 $hostBack=UseOrdinary 'host' $target $hostUse.from;$backMs=Pair $target.name $hostUse.from 'host-return'
 Start-Sleep -Seconds 2
 $clientUse=UseOrdinary 'client' $target $hostUse.to;$closeMs=Pair $target.name $hostUse.to 'client-use'
 Start-Sleep -Seconds 2
 $hostUse2=UseOrdinary 'host' $target $hostUse.from;$reopenMs=Pair $target.name $hostUse.from 'host-reuse'
 Start-Sleep -Seconds 2
 foreach($r in 'host','client'){Save "$out/sync-final-$r.json" (Y $r)}
 WaitCaptures
 $rows.Add(@{target=$target.name;declaration=$target.declaration;position=$target.position;sharedToggles=$shared.Count;pairedChannels=$paired;hostUse=$hostUse;clientUse=$clientUse;hostReturn=$hostBack;hostReturnConvergeMs=$backMs;hostReuse=$hostUse2;reopenConvergeMs=$reopenMs;misses=@($misses);openConvergeMs=$openMs;closeConvergeMs=$closeMs})
 Save "$out/verified.json" @{passed=$true;scope='Mission usable toggle shared by ordinary native use from both roles.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};foreach($r in 'host','client'){try{Save "$out/failure-sync-$r.json" (Y $r)}catch{}}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(90000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_usable_sync off'}catch{}}
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/samples.json" @($samples)
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Mission usable census and shared two-state toggle by ordinary native use (host open, client close); Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
