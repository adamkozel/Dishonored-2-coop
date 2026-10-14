param([string]$Take='mission-usable-v1127',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[string]$TargetName='env_interiors_rich_sink_01_1_9',[double[]]$AimHeights=@(1.0,0.6,1.4,0.3),[double]$Stand=1.3)
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
function UseOrdinary($r,$target,$want){
 # Stand off the toggle towards open floor, aim at native heights, press X.
 $p=$target.position;$from=(Q $r '--player').candidatePosition
 $dx=$from[0]-$p[0];$dy=$from[1]-$p[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001)
 $tries=[Collections.Generic.List[object]]::new()
 foreach($bearing in 0,60,-60,120,-120,180){
  $a=[Math]::Atan2($dy/$n,$dx/$n)+$bearing*[Math]::PI/180;$sx=$p[0]+$Stand*[Math]::Cos($a);$sy=$p[1]+$Stand*[Math]::Sin($a);$yaw=[Math]::Atan2(-[Math]::Sin($a),-[Math]::Cos($a))*180/[Math]::PI
  C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$sx,$sy,($p[2]+1.7),$yaw);Start-Sleep -Milliseconds 1300;Modal
  foreach($h in $AimHeights){
   try{& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X $p[0] -Y $p[1] -Z ($p[2]+$h) -Evidence "$out/aim-$r-$want-$bearing-$h.json"}catch{$tries.Add(@{bearing=$bearing;height=$h;aim=$_.Exception.Message});continue}
   # Diff the native census around the press: the prompt may belong to a
   # neighbouring usable (e.g. sink cabinet vs basin); follow what changed.
   $before=@{};foreach($u in (Q $r '--usable-catalog').usables){$before[$u.name]=$u.current}
   I $r X 150;I $r;Start-Sleep -Milliseconds 1600
   $changed=@((Q $r '--usable-catalog').usables|Where-Object {$before.ContainsKey($_.name)-and$before[$_.name]-ne$_.current}|ForEach-Object {@{name=$_.name;from=$before[$_.name];to=$_.current;toggle=$_.toggle}})
   $tries.Add(@{bearing=$bearing;height=$h;stand=@($sx,$sy);changed=$changed})
   if($changed.Count){
    $hit=@($changed|Where-Object {$_.toggle-and$script:sharedNames-contains$_.name})
    if($hit.Count-ne1){Save "$out/use-$r-$want.json" @($tries);throw "Native use changed non-shared or several usables on $r"}
    if($hit[0].name-ne$script:target.name){
     if($r-ne'host'){Save "$out/use-$r-$want.json" @($tries);throw 'Client use changed a different usable'}
     $script:target=$script:census.host.usables|Where-Object name -eq $hit[0].name|Select-Object -First 1
     foreach($q in 'host','client'){$e=@((Q $q '--entities').entities|Where-Object name -eq $script:target.name);if($e.Count-ne1){throw "Unique entity required $q"};$script:handles[$q]=$e[0].handle}}
    if($hit[0].to-ne$want){$script:want=$hit[0].to}
    Save "$out/use-$r-$want.json" @($tries);return @{bearing=$bearing;height=$h;name=$hit[0].name;from=$hit[0].from;to=$hit[0].to}}}}
 Save "$out/use-$r-$want.json" @($tries);throw "Ordinary native use did not reach $want on $r"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1123b/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1123b candidate'}
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
 $pick=@($shared|Where-Object {$_.current-eq0}|Sort-Object @{e={if($_.name-like'*door*'){1}else{0}}},@{e={[Math]::Sqrt([Math]::Pow($_.position[0]-$spawn[0],2)+[Math]::Pow($_.position[1]-$spawn[1],2)+[Math]::Pow($_.position[2]-$spawn[2],2))}})
 if($TargetName){$pick=@($shared|Where-Object name -eq $TargetName)}
 Save "$out/shared-toggles.json" @{hostToggles=$hostToggles.Count;clientToggles=@($census.client.usables|Where-Object toggle).Count;shared=@($shared|ForEach-Object name);order=@($pick|ForEach-Object name)}
 if(!$pick.Count){throw 'No shared closed toggle on this map'}
 $script:target=$pick[0];$target=$script:target;$script:census=$census;$script:sharedNames=@($shared|ForEach-Object name);$script:handles=@{}
 foreach($r in 'host','client'){$e=@((Q $r '--entities').entities|Where-Object name -eq $target.name);if($e.Count-ne1){throw "Unique entity required $r"};$script:handles[$r]=$e[0].handle}
 # 1127 diagnostic: ordinary press variants at the observed cabinet aim (low), sync not armed.
 $p=$target.position;$from=(Q 'host' '--player').candidatePosition;$dx=$from[0]-$p[0];$dy=$from[1]-$p[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001)
 $a=[Math]::Atan2($dy/$n,$dx/$n);$sx=$p[0]+$Stand*[Math]::Cos($a);$sy=$p[1]+$Stand*[Math]::Sin($a);$yaw=[Math]::Atan2(-[Math]::Sin($a),-[Math]::Cos($a))*180/[Math]::PI
 C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$sx,$sy,($p[2]+1.7),$yaw);Start-Sleep -Milliseconds 1300;Modal
 Capture 'diag' 60;Start-Sleep -Milliseconds 800;$diag=[Collections.Generic.List[object]]::new()
 foreach($h in 0.3,0.45,0.15){
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role host -X $p[0] -Y $p[1] -Z ($p[2]+$h) -Evidence "$out/diag-aim-$h.json"
  foreach($ms in 150,400,900){
   $before=@{};foreach($u in (Q 'host' '--usable-catalog').usables){$before[$u.name]=$u.current}
   $t0=[Environment]::TickCount64;I host X $ms;I host;Start-Sleep -Milliseconds 1800
   $changed=@((Q 'host' '--usable-catalog').usables|Where-Object {$before.ContainsKey($_.name)-and$before[$_.name]-ne$_.current}|ForEach-Object {@{name=$_.name;from=$before[$_.name];to=$_.current}})
   $diag.Add(@{height=$h;pressMs=$ms;atMs=$t0;changed=$changed;player=(Q 'host' '--player').candidatePosition});Save "$out/diag.json" @($diag)
   if($changed.Count){break}}
  if($diag[-1].changed.Count){break}}
 WaitCaptures;throw 'diagnostic complete'
 foreach($r in 'host','client'){I $r;C $r 'coop_usable_sync session'}
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{$ok=$true;foreach($r in 'host','client'){$y=Y $r;$e=@($y.entries|Where-Object name -eq $target.name);if(!$y.enabled-or!$y.session.enabled-or$e.Count-ne1-or!$e[0].ready-or$e[0].applied-ne$e[0].revision){$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 100}}while(!$ok-and$w.ElapsedMilliseconds-lt10000)
 foreach($r in 'host','client'){Save "$out/sync-armed-$r.json" (Y $r)}
 if(!$ok){throw 'Mission usable family did not pair'}
 $paired=@{};foreach($r in 'host','client'){$paired[$r]=@((Y $r).entries|Where-Object ready).Count}
 # Partner standing beside the source so both recordings frame the toggle.
 Capture 'usable' 75;Start-Sleep -Milliseconds 800
 $hostUse=UseOrdinary 'host' $target 1;$target=$script:target;$openMs=Pair $target.name $hostUse.to 'host-use'
 Start-Sleep -Seconds 2
 $clientUse=UseOrdinary 'client' $target $hostUse.from;$closeMs=Pair $target.name $hostUse.from 'client-use'
 Start-Sleep -Seconds 2
 foreach($r in 'host','client'){Save "$out/sync-final-$r.json" (Y $r)}
 WaitCaptures
 $rows.Add(@{target=$target.name;declaration=$target.declaration;position=$target.position;sharedToggles=$shared.Count;pairedChannels=$paired;hostUse=$hostUse;clientUse=$clientUse;openConvergeMs=$openMs;closeConvergeMs=$closeMs})
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
