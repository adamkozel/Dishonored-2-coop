param([string]$Take='campaign-dunwall-vm-v1301a',[string]$MapInfo='campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[ValidateSet('corvo','emily')][string]$HostCharacter='corvo',[ValidateSet('Left','Right')][string]$HostChoice='Left',[string]$Candidate='candidate-npc-combat-v1214cb',[Parameter(Mandatory)][string]$StepsFile,[int]$LoadSeconds=420,[int]$VideoSeconds=0,[int]$JoinAfterStep=0,[int]$LeaveAfterStep=0,[int]$RejoinAfterStep=0)
# 1703 (#135): the mid-mission join and rejoin on two Steam accounts. With -JoinAfterStep N the VM waits in its main
# menu, unconnected, while the host alone loads the mission and runs steps 1..N. The host then hosts a lobby and
# publishes its mission (coop_follow_host on); the VM joins it, its game follows the host's mission from the main menu
# (join-follow) and the join placement must put her beside the host (join_warp.h). Recorded: distance, the follow
# status, gameflow in both worlds and every NPC's life in both worlds (as Route-Mission-v1666's lab join).
# -LeaveAfterStep L: the VM leaves the session as F8 does in the product (coop_session off; the product controller
# reset, coop_product_auto on), and the host plays on alone. -RejoinAfterStep R: the VM joins the same lobby again
# while still in the mission; both must reconnect, rearm, and converge to the host's gameflow. Then both play the
# remaining steps to the mission end together.
# Campaign route take across two machines and two Steam accounts (chapter 97):
# the host lab instance on this PC (main account, Corvo) and the client lab
# instance in the lab VM (second account, Emily) meet in a friends-only Steam
# lobby (coop_session host_quiet / join), both in product mode
# (coop_product_auto: mission end mirror, both-ready hold, chaos, gameflow and
# level transition mirrors). Both load the mission start, pass the native
# character choice (host Corvo, VM Emily; -HostCharacter emily for Memories of
# Training, which the menu starts as Emily), then the host runs the step list of
# Route-Mission-v1300. After each step both worlds' logic, map and gameflow
# counts are recorded, with a host frame and a VM screen. Online sidecars
# contain SteamIDs: only whitelisted fields are recorded. Restores: sessions
# off, VM instance stopped, host lab pair relaunched with the Dunwall fixture.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$notes=[ordered]@{};$captures=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
# Role helpers: 'host' is the local lab host instance, 'client' the VM instance.
function C($r,$cmd){if($r-eq'client'){Send-D2LabVMCommand -Command $cmd -WaitConsumed|Out-Null}else{& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}}
function S($r,$suffix=''){
 if($r-eq'client'){$side=if($suffix){$suffix.TrimStart('.').Replace('.json','')}else{''}
  for($i=0;$i-lt3;$i++){try{if($side){return(Get-D2LabVMStatus -Sidecar $side)}else{return(Get-D2LabVMStatus)}}catch{Start-Sleep -Milliseconds 300}};throw "VM status unavailable $suffix"}
 for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/host/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable host"}
function Q($r,$mode,$arg=0){
 if($r-eq'client'){if($arg){return(Invoke-D2LabVMProbe -Mode $mode -Arg ([string]$arg))};return(Invoke-D2LabVMProbe -Mode $mode)}
 for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids.host $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids.host $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed host $mode"}
function I($r,[string]$b='',[int]$ms=100){
 if($r-eq'client'){$btn=if($b){$b}else{'Neutral'};Send-D2LabVMInput -Button $btn -DurationMs $ms|Out-Null;return}
 $a=@{Role='host';DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Frame($r,$name){
 if($r-eq'client'){try{Save-D2LabVMScreen -Path "$out/$name-vm.png"|Out-Null}catch{};return}
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$name-host.png" 2>$null|Out-Null}
$gfNames=$null;try{$gfNames=Get-Content "$PSScriptRoot/gameflow-names.local.json" -Raw|ConvertFrom-Json}catch{}
function ActiveNames($m){if(!$gfNames-or!$m-or!$m.gameflowNodes){return $null};(@($m.gameflowNodes|Where-Object {$_[1]-eq2}|ForEach-Object {$n=$gfNames."$($_[0])";if($n){$n}else{"#$($_[0])"}})|Sort-Object)-join ','}
function Snap($r){$s=S $r;$m=$null;try{$m=S $r '.missionend.json'}catch{}
 $o=[ordered]@{logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMap;epoch=$s.localEpoch;cine=$s.localCinematicEpoch;pos=$s.localPosition;fresh=$s.remoteFresh;gameflow=$null;gfApplied=$null;lt=$null;hold=$null;ka=$null;active=$null}
 if($m){$o.gameflow=$m.gameflowStates;if($m.gameflowMirror){$o.gfApplied=$m.gameflowMirror.applied};if($m.levelTransition){$o.lt="$($m.levelTransition.result)/$($m.levelTransition.sent)/$($m.levelTransition.applied)"};if($m.hold){$o.hold="$($m.hold.result)/$($m.hold.lastWaitMs)"}}
 try{$k=S $r '.kiscule.json';$o.ka="$($k.local)/$($k.applied)/$($k.suppressed)"}catch{}
 $o.active=ActiveNames $m
 $o}
function HO{try{Get-Content "$root/work/lab-instances/host/coop-status.json.online.json" -Raw|ConvertFrom-Json}catch{$null}}
$safe='active','host','phase','result','connectionState','joinRequests','role','inviteOpened','lobbyMembers'
function Safe($o){if(!$o){return $null};$h=[ordered]@{};foreach($n in $safe){if($o.PSObject.Properties[$n]){$h[$n]=$o.$n}};[pscustomobject]$h}
# Drives both worlds through scenes, prompts, the character choice and loads
# until both are in gameplay or the time runs out.
function Settle($label,$sec){
 $w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0};$choice=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 700;$ready=$true
  foreach($r in $(if($script:solo){@('host')}else{@('host','client')})){$x=Snap $r;if($x.logic-ne1-or$x.pending){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt3500){continue}
   switch($x.logic){
    # 14: the character choice. The host steers with the D-pad and the stick after the screen settles (the remembered
    # choice wins when the press lands too early: take loadout-dunwall-v1311d started the host as Emily); -HostChoice names the side (Left is Corvo, take choice-dunwall-v1311f).
    14{$choice[$r]++;if($r-eq'host'){Start-Sleep -Milliseconds 1500;foreach($k in 1,2){& "$lab/Send-D2Input.ps1" -Role host -Button $HostChoice -LeftX $(if($HostChoice-eq'Left'){-32000}else{32000}) -DurationMs 250|Out-Null;Start-Sleep -Milliseconds 600}}else{I $r Right 250};Start-Sleep -Milliseconds 400;I $r A 400;$last[$r]=$w.ElapsedMilliseconds}
    3{I $r Right 250;Start-Sleep -Milliseconds 400;I $r A 400;$last[$r]=$w.ElapsedMilliseconds}
    5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
    {$_-in 8,15,9,11}{I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
    12{$choice[$r]++;I $r $(if($choice[$r]%2){'B'}else{'A'}) 300;$last[$r]=$w.ElapsedMilliseconds} # a tutorial page closes with B (the Timepiece's); other logic-12 screens take A
    4{I $r B 250;$last[$r]=$w.ElapsedMilliseconds} # a readable (ReadNote) left open
    2{I $r B 250;$last[$r]=$w.ElapsedMilliseconds} # the pause menu (a game window that lost focus pauses itself): resume
    13{throw "$r died during $label (GameOver)"}}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt($sec*1000))
 return $ready}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1";Copy-Item $StepsFile "$out/steps.json"
 $steps=Get-Content $StepsFile -Raw|ConvertFrom-Json
 $l=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json;$pids.host=$l.pid
 if((Get-Process -Id $pids.host).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated host process'}
 if((S host).localGameLogicState-ne1){throw 'Host lab instance not in gameplay'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 & "$root/tools/vm/Test-D2LabVM.ps1" -Repair|Out-Null;if($LASTEXITCODE){throw 'Lab VM not ready'}
 try{Stop-D2LabVMRole -Role client|Out-Null}catch{}
 & "$root/tools/vm/Sync-D2LabVMWorkspace.ps1" -Role client -SkipGameCopy *> "$out/vm-sync.log"
 $guest=Invoke-D2LabVM {(Get-FileHash C:\D2Lab\ws\outputs\d2-coop-lab\bin\dinput8.dll).Hash}
 $notes.identity=[ordered]@{candidate=$Candidate;hostDll=$pin.dll;guestDllMatches=($guest-eq$pin.dll);hostCharacter=$HostCharacter}
 if(!$notes.identity.guestDllMatches){throw 'Guest DLL differs from the host candidate'}
 Save "$out/identity.json" $notes.identity
 $dirty=$true
 Start-D2LabVMRole -Role client|Out-Null
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$gs=Get-D2LabVMStatus}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne6)
 Send-D2LabVMKey 0x0D;Start-Sleep 3
 $script:solo=[bool]$JoinAfterStep
 if(!$solo){
 C client 'LoadGame 81'
 $d=(Get-Date).AddSeconds(120);do{Start-Sleep 3;$gs=Get-D2LabVMStatus;if($gs.localGameLogicState-in 13,8,15){Send-D2LabVMKey 0x0D}}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne1)
 if($gs.localGameLogicState-ne1){throw 'VM instance did not reach gameplay'}
 C host 'coop_session host_quiet'
 $d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$ho=HO}while((Get-Date)-lt$d-and!($ho.lobby-and"$($ho.lobby)"-ne'0'))
 if(!$ho.lobby-or"$($ho.lobby)"-eq'0'){throw 'Host did not create a lobby'}
 C client "coop_session join $($ho.lobby)"
 $d=(Get-Date).AddSeconds(60);do{Start-Sleep 3;$hs=S host;$gs=S client}while((Get-Date)-lt$d-and!($hs.remoteFresh-and$gs.remoteFresh-and$gs.connectionState-eq3))
 $notes.join=[ordered]@{hostOnline=Safe (HO);guestOnline=Safe (Get-D2LabVMStatus -Sidecar online);connected=($hs.remoteFresh-and$gs.remoteFresh)}
 if(!$notes.join.connected){throw 'Two-account join did not connect'}
 }
 if($VideoSeconds){$hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
  $va=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$VideoSeconds,'-an','-c:v','libx264','-preset','veryfast','-crf','23','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+frag_keyframe+empty_moov+default_base_moof',"$out/take-host.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $va -WindowStyle Hidden -RedirectStandardError "$out/take-host-capture.log" -PassThru))}
 foreach($r in 'host','client'){C $r 'coop_kiscule_mirror on';C $r 'coop_product_auto on'}
 C host "campaign $MapInfo $Chaos $HostCharacter";if(!$solo){C client "campaign $MapInfo $Chaos emily"}
 if(!(Settle 'start' $LoadSeconds)){$notes.startState=@{host=(Snap host);client=(Snap client)};foreach($r in 'host','client'){Frame $r 'start-timeout'};throw 'Both worlds did not reach mission gameplay'}
 # The characters the worlds play (the native choice decides on a mission start; -HostCharacter is what the host must be).
 $chars=@{};foreach($r in 'host','client'){try{$chars[$r]=(Q $r '--character').definition}catch{$chars[$r]=$null}};$notes.characters=$chars
 if($chars.host-and$chars.host-notmatch"/$HostCharacter/"){throw "Host plays $($chars.host), expected $HostCharacter"}
 C host 'coop_player_health_hold on';if(!$solo){C client 'coop_player_health_hold on'}
 Start-Sleep 3;$rows=[Collections.Generic.List[object]]::new();$rows.Add([ordered]@{step='start';host=(Snap host);client=(Snap client)});foreach($r in 'host','client'){Frame $r 'step00-start'}
 $i=0
 function Npc($rx,$near){$all=@((Q host '--entities').entities|Where-Object {$_.type-eq'.?AVarkNPC@@'-and$_.name-match$rx})
  if(!$all.Count){$near2=if($near){$near}else{(Snap host).pos};$names=@((Q host '--entities').entities|Where-Object {$_.type-eq'.?AVarkNPC@@'}|Sort-Object {[Math]::Pow($_.position[0]-$near2[0],2)+[Math]::Pow($_.position[1]-$near2[1],2)}|Select-Object -First 8|ForEach-Object {"$($_.name)@$([int]$_.position[0]),$([int]$_.position[1])"});throw "No NPC matches $rx (nearest: $($names -join ' '))"};if($near){$all=@($all|Sort-Object {[Math]::Pow($_.position[0]-$near[0],2)+[Math]::Pow($_.position[1]-$near[1],2)})};$all[0]}
# Behind: stand $dist m behind an NPC (its forward axis is the entity's rotation row 0 at +0x84; confirmed runtime,
# take route-pa-v1308e) and face it, for a takedown from behind instead of a stand-up fight.
 function Behind($e,$dist,$eyeUp){$w=@((Q host '--object' $e.pointer).words);$f=@($w|Where-Object offset -eq '0x80')[0];$g=@($w|Where-Object offset -eq '0x88')[0]
  $fx=[BitConverter]::ToSingle([BitConverter]::GetBytes([Convert]::ToUInt64($f.word.Substring(2),16)),4);$fy=[BitConverter]::ToSingle([BitConverter]::GetBytes([Convert]::ToUInt64($g.word.Substring(2),16)),0)
  $n=[Math]::Max([Math]::Sqrt($fx*$fx+$fy*$fy),.001);$fx/=$n;$fy/=$n;$p=$e.position
  C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f($p[0]-$dist*$fx),($p[1]-$dist*$fy),($p[2]+$eyeUp),([Math]::Atan2($fy,$fx)*180/[Math]::PI))}
 function Face($p,$dist,$eyeUp){$h=(Snap host).pos;$dx=$h[0]-$p[0];$dy=$h[1]-$p[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001)
  $x=$p[0]+$dist*$dx/$n;$y=$p[1]+$dist*$dy/$n;$yaw=[Math]::Atan2($p[1]-$y,$p[0]-$x)*180/[Math]::PI
  C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,($p[2]+$eyeUp),$yaw)}
 # Entity list for a step; a world still loading from the previous step is settled first.
 function Ents(){for($k=0;$k-lt6;$k++){try{return (Q host '--entities').entities}catch{[void](Settle 'entities' 60);Start-Sleep 2}};throw 'Native query failed host --entities'}
# 1703: the client's NPC lives in one guest call (one probe per NPC through PowerShell Direct would take minutes).
function ClientNpcs(){Invoke-D2LabVM -ArgumentList $GuestWs,$GuestLab -ScriptBlock {param($ws,$lab)
  $l=Get-Content -Raw "$ws\work\lab-instances\client\launch.json"|ConvertFrom-Json;$p="$lab\bin\d2_runtime_probe.exe";$o=@{}
  foreach($n in @((& $p $l.pid '--world-npcs'|ConvertFrom-Json).npcs)){$hp=$null;try{$r=& $p $l.pid '--health' ([Convert]::ToInt64($n.handle.Substring(2),16));if(!$LASTEXITCODE){$hp=$r|ConvertFrom-Json}}catch{}
   $o[$n.handle]=@{name=$n.spawnerName;health=$(if($hp){$hp.current}else{$null});suppressed=$(if($hp){$hp.ownerSuppressed}else{$null})}};$o}}
function HostNpcs(){$o=@{};foreach($n in @((Q host '--world-npcs').npcs)){$hp=$null;try{$hp=Q host '--health' ([Convert]::ToInt64($n.handle.Substring(2),16))}catch{}
  $o[$n.handle]=@{name=$n.spawnerName;health=$(if($hp){$hp.current}else{$null});suppressed=$(if($hp){$hp.ownerSuppressed}else{$null})}};$o}
function LifeOf($a){if(!$a){'absent'}elseif($null-eq$a.health){'unread'}elseif($a.health-le0){'dead'}elseif($a.suppressed){'out'}else{'alive'}}
function Dist3($a,$b){if(!$a-or!$b){return $null};[Math]::Round([Math]::Sqrt([Math]::Pow($a[0]-$b[0],2)+[Math]::Pow($a[1]-$b[1],2)+[Math]::Pow($a[2]-$b[2],2)),2)}
function Converge($label){$d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$h=Snap host;$c=Snap client}while((Get-Date)-lt$d-and"$($h.gameflow)"-ne"$($c.gameflow)")
  [ordered]@{host="$($h.gameflow)";client="$($c.gameflow)";equal=("$($h.gameflow)"-eq"$($c.gameflow)")}}
function Armed(){$d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$p=@{};$m=@{};foreach($r in 'host','client'){$p[$r]=$(try{(S $r '.product.json').result}catch{$null});$m[$r]=$(try{(S $r '.movement.json').active}catch{$null})}}while((Get-Date)-lt$d-and!($p.host-eq'armed'-and$p.client-eq'armed'-and$m.host-and$m.client))
  [ordered]@{product=$p;banks=$m;ok=($p.host-eq'armed'-and$p.client-eq'armed'-and$m.host-and$m.client)}}
function JoinNow(){
 $hp=(Snap host).pos;$cp0=(Snap client)
 C host 'coop_follow_host on';C host 'coop_session host_quiet'
 $d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$ho=HO}while((Get-Date)-lt$d-and!($ho.lobby-and"$($ho.lobby)"-ne'0'))
 if(!$ho.lobby-or"$($ho.lobby)"-eq'0'){throw 'Host did not create a lobby'};$script:lobby="$($ho.lobby)"
 C client 'coop_follow_host on';C client 'coop_product_auto on';$jw=[Diagnostics.Stopwatch]::StartNew();C client "coop_session join $lobby"
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$hs=S host;$gs=S client}while((Get-Date)-lt$d-and!($hs.remoteFresh-and$gs.remoteFresh-and$gs.connectionState-eq3))
 $j=[ordered]@{afterStep=$JoinAfterStep;clientBefore=[ordered]@{logic=$cp0.logic;map=$cp0.map};hostOnline=Safe (HO);guestOnline=Safe (Get-D2LabVMStatus -Sidecar online);connected=($hs.remoteFresh-and$gs.remoteFresh)}
 $notes.join=$j;if(!$j.connected){throw 'Two-account join did not connect'}
 $script:solo=$false
 # The VM's game follows the host's mission from its main menu: the mission card, Emily at the character choice.
 do{Start-Sleep -Milliseconds 1500;$cs=Snap client;if($cs.logic-in 15,14,3,8,9,11){[void](Settle 'join-load' 4)}}while((Snap client).map-ne(Snap host).map-and$jw.ElapsedMilliseconds-lt300000)
 $j.loadMs=$jw.ElapsedMilliseconds;try{Save "$out/join-follow-client.json" (S client '.follow.json')}catch{}
 if((Snap client).map-ne(Snap host).map){Frame client 'join-timeout';throw 'The VM did not load the host mission'}
 if(!(Settle 'join' 300)){throw 'Both worlds did not reach gameplay after the join'}
 C client 'coop_player_health_hold on'
 $d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$f=S client '.follow.json'}while((Get-Date)-lt$d-and!($f.warpDistance-gt0))
 Start-Sleep 3;$h=Snap host;$c=Snap client
 $j.follow=[ordered]@{result=$f.result;loads=$f.loads;warps=$f.warps;warpDistance=$f.warpDistance};$j.hostPos=$h.pos;$j.clientPos=$c.pos;$j.distance=Dist3 $h.pos $c.pos
 $j.gameflow=Converge 'join'
 $hn=HostNpcs;$cn=ClientNpcs;$diff=[Collections.Generic.List[object]]::new()
 foreach($k in @($hn.Keys)+@($cn.Keys)|Sort-Object -Unique){$a=LifeOf $hn[$k];$b=LifeOf $cn[$k];if($a-ne$b){$diff.Add([ordered]@{handle=$k;name=$(if($hn[$k]){$hn[$k].name}else{$cn[$k].name});host=$a;client=$b})}}
 $j.npcCounts=@{host=$hn.Count;client=$cn.Count};$j.npcStateDiffs=@($diff);Save "$out/join.json" $j;foreach($r in 'host','client'){Frame $r 'join'}
 $j.armed=Armed
 if(!($f.loads-ge1)){throw 'The VM did not follow the host mission (loads 0)'}
 if($null-eq$j.distance-or$j.distance-gt3){throw "The VM was not placed beside the host ($($j.distance) m)"}
 if(!$j.gameflow.equal){throw "Gameflow did not converge at the join (host $($j.gameflow.host), VM $($j.gameflow.client))"}
 if(!$j.armed.ok){throw 'The co-op systems did not arm after the join'}}
function LeaveNow(){
 # As F8 in the product: the session stops and the product controller restarts from its first step.
 C client 'coop_session off';C client 'coop_product_auto on'
 $d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$hs=S host;$go=Get-D2LabVMStatus -Sidecar online}while((Get-Date)-lt$d-and($hs.remoteFresh-or$go.active))
 $ho=HO;$notes.leave=[ordered]@{afterStep=$LeaveAfterStep;hostSeesPartner=$hs.remoteFresh;guestOnline=Safe $go;hostOnline=Safe $ho;distance=Dist3 (Snap host).pos (Snap client).pos}
 foreach($r in 'host','client'){Frame $r 'leave'}
 if($hs.remoteFresh-or$go.active){throw 'The VM did not leave the session'}
 if(!$ho.active-or"$($ho.lobby)"-ne$lobby){throw 'The host stopped hosting its lobby when the partner left'}}
function RejoinNow(){
 $c0=Snap client;$h0=Snap host;$rw=[Diagnostics.Stopwatch]::StartNew();C client "coop_session join $lobby"
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$hs=S host;$gs=S client}while((Get-Date)-lt$d-and!($hs.remoteFresh-and$gs.remoteFresh-and$gs.connectionState-eq3))
 $r=[ordered]@{afterStep=$RejoinAfterStep;connected=($hs.remoteFresh-and$gs.remoteFresh);connectMs=$rw.ElapsedMilliseconds;gameflowBefore=[ordered]@{host="$($h0.gameflow)";client="$($c0.gameflow)"};guestOnline=Safe (Get-D2LabVMStatus -Sidecar online)}
 $notes.rejoin=$r;if(!$r.connected){throw 'The VM did not reconnect to the lobby'}
 if(!(Settle 'rejoin' 120)){throw 'Both worlds did not reach gameplay after the rejoin'}
 $r.gameflow=Converge 'rejoin';$f=S client '.follow.json';$r.follow=[ordered]@{result=$f.result;loads=$f.loads;warps=$f.warps};$r.distance=Dist3 (Snap host).pos (Snap client).pos
 $r.armed=Armed;foreach($x in 'host','client'){Frame $x 'rejoin'}
 if(!$r.gameflow.equal){throw "Gameflow did not converge after the rejoin (host $($r.gameflow.host), VM $($r.gameflow.client))"}
 if(!$r.armed.ok){throw 'The co-op systems did not rearm after the rejoin'}}
 foreach($st in $steps){$i++;$tag='step{0:D2}-{1}'-f$i,$st.do
  if($JoinAfterStep-and$i-eq$JoinAfterStep+1){JoinNow}
  if($LeaveAfterStep-and$i-eq$LeaveAfterStep+1){LeaveNow}
  if($RejoinAfterStep-and$i-eq$RejoinAfterStep+1){RejoinNow}
  # unlessMap: skip a step once the host is already on a matching map (a talk that may already have ended the mission).
  if($st.unlessMap-and"$((Snap host).map)"-match$st.unlessMap){$notes.skipped+=@($tag);continue};$ents=Ents
  switch($st.do){
   'trigger'{$e=@($ents|Where-Object name -eq $st.name);if($e.Count-ne1){throw "Missing trigger $($st.name)"};$p=$e[0].position;$back=(Snap host).pos
    C host ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$p[0],$p[1],($p[2]+1.0))
    # bounce: the enter event fires on entry; return to the last stand before a volume without a floor drops the player.
    if($st.bounce){Start-Sleep -Milliseconds 600;C host ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$back[0],$back[1],($back[2]+1.7))}}
   'at'{C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$st.pos[0],$st.pos[1],$st.pos[2],$(if($st.yaw){$st.yaw}else{0}))}
   'use'{$e=if($st.match){@($ents|Where-Object {$_.name-match$st.match}|Sort-Object {if($st.near){[Math]::Pow($_.position[0]-$st.near[0],2)+[Math]::Pow($_.position[1]-$st.near[1],2)}else{0}}|Select-Object -First 1)}else{@($ents|Where-Object name -eq $st.name)};if($e.Count-ne1){throw "Missing usable $($st.name)$($st.match)"};$p=$e[0].position;$z=if($st.z){$st.z}else{$p[2]+1.0};$hit=$false
    $offs=if($st.offsets){$st.offsets}else{@(@(0.9,0),@(-0.9,0),@(0,0.9),@(0,-0.9))}
    # A one-stand list ([[x,y]]) comes out of the if as the bare pair; keep it a list of stands.
    if($offs.Count-gt0-and$offs[0]-isnot[Collections.IList]){$offs=@(,@($offs))}
    # Sweep pitch and bearing at each stand (small targets such as signet buttons).
    $pitches=if($st.pitches){$st.pitches}else{@(0)};$yaws=if($st.yaws){$st.yaws}else{@(0)};$k=0
    # Objectives can change a few seconds after the use (a door's task): compare with the step's start too.
    $g0="$((Snap host).gameflow)";$liveLog=[Collections.Generic.List[object]]::new()
    # focus: press only while the native use focus (arkCpntPlayerSensorInteraction+0x80 high dword, take 1133) is on the target,
    # so a press never lands on something else (a guard, a door, a carried body's drop).
    $sensor=$null;$th=$null;$focusLog=[Collections.Generic.List[object]]::new();if($st.focus){$pl=Q host '--player';$me=@((Q host '--entities').entities|Where-Object pointer -eq $pl.player);$th=[Convert]::ToUInt32(($e[0].handle-replace'^0x',''),16)
     $sensor=@((Q host '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@')[0].pointer}
    # heal: the lab health hold for a stand under fire (the Karnaca watchtower's searchlight); recorded in the take.
    if($st.heal){C host 'coop_player_health_hold on';$notes.healthHold+=@($st.name)}
    foreach($o in $offs){$x=$p[0]+$o[0];$y=$p[1]+$o[1];$yaw0=[Math]::Atan2($p[1]-$y,$p[0]-$x)*180/[Math]::PI
     foreach($pt in $pitches){foreach($yo in $yaws){$k++
      # live: a target on something that moves (a Karnaca watchtower) is re-read before each try.
      # orbit: the target turns about a fixed axis (the Karnaca watchtower's top carries its tank); stand radius m out from the axis through the target.
      if($st.live){$cur=@((Q host '--entities').entities|Where-Object {$_.name-eq$e[0].name});if($cur.Count){$p=$cur[0].position;$x=$p[0]+$o[0];$y=$p[1]+$o[1]
        if($st.orbit){$ang=[Math]::Atan2($p[1]-$st.orbit[1],$p[0]-$st.orbit[0]);$x=$st.orbit[0]+$st.radius*[Math]::Cos($ang);$y=$st.orbit[1]+$st.radius*[Math]::Sin($ang)
         $liveLog.Add([ordered]@{try=$k;target=$p;bearing=[Math]::Round($ang*180/[Math]::PI,1);stand=@($x,$y)});Save "$out/$tag-live.json" @($liveLog)}
        $yaw0=[Math]::Atan2($p[1]-$y,$p[0]-$x)*180/[Math]::PI}}
      C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,$z,($yaw0+$yo));Start-Sleep -Milliseconds 700
      # aim: the pitch from the native eye to the target, plus the listed offsets.
      $pitch=$pt;if($st.aim){try{$eye=(Q host '--player').nativeViewPosition;$hd=[Math]::Sqrt([Math]::Pow($p[0]-$eye[0],2)+[Math]::Pow($p[1]-$eye[1],2));$pitch=[Math]::Max(-60,[Math]::Min(75,[Math]::Atan2($eye[2]-$p[2],[Math]::Max($hd,.2))*180/[Math]::PI+$pt))}catch{}}
      if($pitch){try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $pitch -Quick|Out-Null}catch{}}
      if($sensor){$w=@((Q host '--object' $sensor).words|Where-Object offset -eq '0x80');$fh=[uint32]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32)
       $focusLog.Add([ordered]@{try=$k;stand=@($x,$y,$z);yaw=$yaw0+$yo;pitch=$pitch;focus=('0x{0:x}'-f$fh);target=('0x{0:x}'-f$th)});Save "$out/$tag-focus.json" @($focusLog);if($fh-ne$th){continue}}
      $b=Snap host;if($k-le6){Frame host "$tag-try$k"};I host X $(if($st.hold){$st.hold}else{250});Start-Sleep -Milliseconds $(if($st.checkMs){$st.checkMs}else{1500});$a=Snap host
      if($a.logic-eq13){throw "Host died at $($st.name)"}
      # checkMs: wait longer before reading the result (the Grand Palace vault button completes its task after an autosave delay).
      # gone: a pickup counts only when the entity has left the world.
      if($st.gone){if($a.logic-eq4){I host B 250;Start-Sleep -Milliseconds 800};$left=@((Q host '--entities').entities|Where-Object name -eq $st.name).Count-eq0;if($left){$hit=$true;break};continue}
      # expect logic: only a press that leaves gameplay (a talk's scene) counts; a one-liner scene also changes the cinematic epoch.
      if($st.expect-eq'logic'){if($a.logic-ne1){$hit=$true;break};continue}
      if($a.cine-ne$b.cine-or$a.logic-ne1-or"$($a.gameflow)"-ne"$($b.gameflow)"-or"$($a.gameflow)"-ne$g0){$hit=$true;break}}
      if($hit){break}}
     if($hit){break}}
    if($st.heal){C host 'coop_player_health_hold off'}
    if(!$hit){throw "Use of $($st.name) not registered"}}
   'wait'{Start-Sleep -Milliseconds $st.ms}
   # partner: move the partner to a stand, optionally crouched (Memories of Training: an idle partner in view of its own
   # stealth Corvo restarts that lesson in its world in a loop, take route-tut-v1310g).
   'partner'{C client ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$st.pos[0],$st.pos[1],$st.pos[2],$(if($st.yaw){$st.yaw}else{0}));Start-Sleep -Milliseconds 700
    if($st.crouch){for($k=0;$k-lt4;$k++){$ids=@();try{$ids=@((Q client '--player-states').active|ForEach-Object {$_.id})}catch{};if($ids-contains6){break};I client B 150;Start-Sleep -Milliseconds 900}}}
   # action: a timed host action from tools/diag/d2_route_actions.py (chase_hold, pickpocket, shoot, cycle); its args and output are kept with the step.
   'action'{Save "$out/$tag-action-args.json" $st.args;$lines=@(python -I "$root/tools/diag/d2_route_actions.py" $root $st.name "@$out/$tag-action-args.json" 2>&1);$code=$LASTEXITCODE;$lines|Set-Content "$out/$tag-action.log";if($code){throw "Action $($st.name) failed (exit $code)"}}
   # at: stand at a pose (x, y, view z, yaw, optional pitch), e.g. to set a carried body down on a dock before a talk.
   'at'{C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$st.pos[0],$st.pos[1],$st.pos[2],$st.yaw);Start-Sleep -Milliseconds 700;if($st.pitch){try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $st.pitch -Quick|Out-Null}catch{}}}
   # jindosh: dial the Dust District's Jindosh lock. tools/diag/d2_jindosh_lock.py reads each seat's family and heirloom
   # indices; seat i is pressed to index i (each use advances a wheel one stage; take route-dd-v1306a), aiming each wheel
   # from stands 1 to 1.4 m west of it, pressing only while the native use focus is on it. The lock opens at index sum 31.
   'jindosh'{$dial=[Collections.Generic.List[object]]::new()
    $pl=Q host '--player';$me=@((Q host '--entities').entities|Where-Object pointer -eq $pl.player);$sensor=@((Q host '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@')[0].pointer
    foreach($seat in 1..5){foreach($kind in 'family','herloom'){
      $name="ld_wheel_${kind}_$seat";$key=if($kind-eq'family'){'WheelFamilyIndex'}else{'WheelHeirloomIndex'}
      $w=@((Q host '--entities').entities|Where-Object name -eq $name)[0];$th=[Convert]::ToUInt32(($w.handle-replace'^0x',''),16);$p=$w.position
      for($n=0;$n-lt7;$n++){
       $lock=& python -I "$root/tools/diag/d2_jindosh_lock.py" $root $pids.host|ConvertFrom-Json
       $row=@($lock.wheels|Where-Object {(@($_.WheelFamily)+@($_.WheelHeirloom))-contains$name})[0];$cur=$row.$key;$dial.Add([ordered]@{wheel=$name;press=$n;index=$cur});if($cur-eq$seat){break}
       $hit=$false
       foreach($stand in @(@(1.0,180),@(1.4,180),@(1.0,160),@(1.0,200),@(1.4,160),@(1.4,200))){$ang=$stand[1]*[Math]::PI/180;$sx=$p[0]+$stand[0]*[Math]::Cos($ang);$sy=$p[1]+$stand[0]*[Math]::Sin($ang);$yaw0=[Math]::Atan2($p[1]-$sy,$p[0]-$sx)*180/[Math]::PI
       foreach($pt in 10,0,20,-10,30,40,-20){foreach($yo in 0,-10,10){
         C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$sx,$sy,($p[2]+1.35),($yaw0+$yo));Start-Sleep -Milliseconds 400
         try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $pt -Quick|Out-Null}catch{}
         $wd=@((Q host '--object' $sensor).words|Where-Object offset -eq '0x80');$fh=[uint32]([Convert]::ToUInt64($wd[0].word.Substring(2),16)-shr32)
         if($fh-eq$th){I host X 300;Start-Sleep -Milliseconds 1200;$hit=$true;break}}
        if($hit){break}}
       if($hit){break}}
       if(!$hit){Save "$out/$tag-dial.json" @($dial);throw "Jindosh wheel $name never took the focus"}}}}
    Save "$out/$tag-dial.json" @($dial);$sum=(& python -I "$root/tools/diag/d2_jindosh_lock.py" $root $pids.host|ConvertFrom-Json).combination.CombinationIndexSum
    $notes.jindosh=$sum;if($sum-ne31){throw "Jindosh lock index sum $sum, not 31"}}
   # flag: wait until a native Kiscule variable reaches a value (tools/diag/d2_kiscule_vars.py reads the host's script
   # memory; entity "storyflags" or a script entity, name an exact variable). Scenes and prompts on the way settle as usual.
   # A Crack in the Slab: the ritual counts as seen only while the player stands on the hall floor (pa_ld_trigger_1's volume).
   'flag'{$fw=[Diagnostics.Stopwatch]::StartNew();$want=if($null-ne$st.value){[int]$st.value}else{1};$seen=$null
    do{$line=@(& python -I "$root/tools/diag/d2_kiscule_vars.py" $pids.host $st.entity --grep " $($st.name): "|Where-Object {$_-match" $([regex]::Escape($st.name)): default"})[0]
     if($line-match'value (-?\d+)'){$seen=[int]$Matches[1]};if($seen-eq$want){break}
     Start-Sleep -Milliseconds 2000;$x=Snap host;if($x.logic-eq13){throw "Host died waiting for $($st.name)"};if($x.logic-ne1){[void](Settle $tag 60)}}while($fw.ElapsedMilliseconds-lt$st.ms)
    $notes."$tag"=[ordered]@{variable=$st.name;value=$seen;ms=$fw.ElapsedMilliseconds};if($seen-ne$want){throw "$($st.name) stayed $seen, not $want"}}
   # heal: the lab health hold on or off across several steps (a carry under fire); recorded in the take.
   'heal'{if($st.on){C host 'coop_player_health_hold on';$notes.healthHold+=@("from step $tag")}else{C host 'coop_player_health_hold off'}}
   # carry: pick up an unconscious NPC (Sokolov). Each try stands beside the body's live position and holds use (900 ms or more
   # picks a body up; a first press may talk to him instead); it counts when the player carries a body or the story flag
   # the mission's script sets on Carried reads 1 (tools/diag/d2_kiscule_vars.py).
   'carry'{$npc=Npc $st.name $st.near;$offs=if($st.offsets){$st.offsets}else{@(@(1.1,0),@(-1.1,0),@(0,1.1),@(0,-1.1))};if($offs.Count-gt0-and$offs[0]-isnot[Collections.IList]){$offs=@(,@($offs))};$pitches=if($st.pitches){$st.pitches}else{@(50,65,35)};$done=$false;$k=0;$tries=[Collections.Generic.List[object]]::new()
    foreach($o in $offs){foreach($pt in $pitches){$k++
      $cur=@((Q host '--entities').entities|Where-Object handle -eq $npc.handle);$p=if($cur.Count){$cur[0].position}else{$npc.position};$x=$p[0]+$o[0];$y=$p[1]+$o[1];$yaw=[Math]::Atan2($p[1]-$y,$p[0]-$x)*180/[Math]::PI
      C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,$(if($st.z){$st.z}else{$p[2]+$(if($st.dz){$st.dz}else{1.4})}),$yaw);Start-Sleep -Milliseconds 600
      try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $pt -Quick|Out-Null}catch{}
      if($k-le6){Frame host "$tag-try$k"};I host X $(if($st.hold){$st.hold}else{400});Start-Sleep -Milliseconds 1500
      $flag="$(& python -I "$root/tools/diag/d2_kiscule_vars.py" $pids.host storyflags --grep $st.flag 2>$null)";$tries.Add([ordered]@{try=$k;stand=@($x,$y);pitch=$pt;flag=$flag})
      if((Snap host).logic-eq13){Save "$out/$tag-carry.json" @($tries);throw "Host died carrying $($st.name)"}
      # Native player state 45 is carrying a body (Observe-NativeCarry); the flag can follow a moment later.
      $carrying=$false;try{$carrying=@((Q host '--player-states').active|Where-Object id -eq 45).Count-gt0}catch{};$tries[$tries.Count-1].carrying=$carrying
      if($flag-match'value 1'-or$carrying){$done=$true;break}}
     if($done){break}}
    Save "$out/$tag-carry.json" @($tries);if(!$done){throw "Carry of $($st.name) not registered"}}
   # expect: both worlds reach a map (regex) within ms, or the take fails (a mission end must move both worlds on).
   'expect'{$w=[Diagnostics.Stopwatch]::StartNew();do{$h=Snap host;$c=Snap client;if($h.map-match$st.map-and$c.map-match$st.map-and(($h.logic-eq1-and$c.logic-eq1)-or$st.anyLogic)){break}
     foreach($x in @(@('host',$h),@('client',$c))){if($x[1].logic-eq2){I $x[0] B 250}} # a paused world (focus lost) resumes and follows
     Start-Sleep -Seconds 3}while($w.ElapsedMilliseconds-lt$(if($st.ms){$st.ms}else{180000}))
    if($h.map-notmatch$st.map-or$c.map-notmatch$st.map){throw "Expected both worlds on $($st.map): host $($h.map) logic $($h.logic), client $($c.map) logic $($c.logic)"}}
   'clear'{$c=$st.near;$rad=if($st.radius){$st.radius}else{12};$max=if($st.max){$st.max}else{4};$fights=[Collections.Generic.List[object]]::new()
    if($st.heal){C host 'coop_player_health_hold on';$notes.healthHold+=@("clear $($st.name)")}
    function LiveNear(){foreach($e in @((Q host '--entities').entities|Where-Object {$_.type-eq'.?AVarkNPC@@'-and$_.name-match$st.name-and([Math]::Pow($_.position[0]-$c[0],2)+[Math]::Pow($_.position[1]-$c[1],2))-le$rad*$rad}|Sort-Object {[Math]::Pow($_.position[0]-$c[0],2)+[Math]::Pow($_.position[1]-$c[1],2)})){
      $hp=$null;try{$hp=Q host '--health' ([Convert]::ToInt64($e.handle.Substring(2),16))}catch{};if($hp-and$hp.current-gt0){return $e}};return $null}
    for($n=0;$n-lt$max;$n++){$e=LiveNear;if(!$e){break};$hd=[Convert]::ToInt64($e.handle.Substring(2),16);$dead=$false;$trace=[Collections.Generic.List[object]]::new()
     for($k=0;$k-lt40-and!$dead;$k++){if($k%4-eq0){$cur=@((Q host '--entities').entities|Where-Object handle -eq $e.handle);if($cur.Count){Face $cur[0].position 1.5 1.6};Start-Sleep -Milliseconds 400}
      & "$lab/Send-D2Input.ps1" -Role host -RightTrigger 255 -DurationMs $(if($k%3-eq0){1300}else{150})|Out-Null;Start-Sleep -Milliseconds 700
      $hp=$null;try{$hp=Q host '--health' $hd}catch{};$trace.Add(@{press=$k;health=$(if($hp){$hp.current}else{$null})});if(!$hp-or$hp.current-le0){$dead=$true}
      if((Snap host).logic-eq13){throw 'Host died in the fight'}}
     $fights.Add([ordered]@{npc=$e.name;dead=$dead;presses=$trace.Count;trace=@($trace)})}
    if($st.heal){C host 'coop_player_health_hold off'}
    Save "$out/$tag-clear.json" @($fights)}
   'npcs'{$c=$st.near;$list=@((Q host '--entities').entities|Where-Object {$_.type-eq'.?AVarkNPC@@'}|Sort-Object {[Math]::Pow($_.position[0]-$c[0],2)+[Math]::Pow($_.position[1]-$c[1],2)}|Select-Object -First 16|ForEach-Object {[ordered]@{name=$_.name;handle=$_.handle;position=$_.position}});Save "$out/$tag-npcs.json" @($list)}
   'follow'{$fw=[Diagnostics.Stopwatch]::StartNew();$d=if($st.dist){$st.dist}else{2.0};$trace=[Collections.Generic.List[object]]::new()
    while($fw.ElapsedMilliseconds-lt$st.ms){$npc=Npc $st.name $st.near;Face $npc.position $d 1.6;$trace.Add([ordered]@{ms=$fw.ElapsedMilliseconds;npc=$npc.name;position=$npc.position;host=(Snap host).logic});Start-Sleep -Milliseconds 1500;$x=Snap host;if($x.logic-eq13){throw 'Host died while following'};if($x.logic-ne1){[void](Settle $tag 60)}}
    Save "$out/$tag-follow.json" @($trace)}
   'press'{$who=if($st.role){$st.role}else{'host'};if($st.leftY){& "$lab/Send-D2Input.ps1" -Role $who -LeftY $st.leftY -DurationMs $(if($st.ms){$st.ms}else{800})|Out-Null}elseif($st.leftTrigger){& "$lab/Send-D2Input.ps1" -Role $who -LeftTrigger 255 -DurationMs $(if($st.ms){$st.ms}else{300})|Out-Null}else{I $who $st.button $(if($st.ms){$st.ms}else{200})}} # leftTrigger: the Timepiece's timeline change
   'die'{$who=if($st.role){$st.role}else{'host'};$other=if($who-eq'host'){'client'}else{'host'};$e0=@{host=(Snap host).epoch;client=(Snap client).epoch}
    # Same sequence as Test-MissionCheckpoint-v1151: both death screens ready, one
    # A each, then A only on load prompts; the checkpoint session state is traced.
    function DS($tag){$row=[ordered]@{ms=$dw.ElapsedMilliseconds;event=$tag};foreach($r in 'host','client'){$b=Snap $r;$x=$null;try{$x=S $r '.reset.json'}catch{}
      $row[$r]=[ordered]@{logic=$b.logic;pending=$b.pending;map=$b.map;epoch=$b.epoch;phase=$(if($x){$x.phase});result=$(if($x){$x.result});deathReady=$(if($x){$x.deathReady});peerDeathReady=$(if($x){$x.peerDeathReady});mission=$(if($x){$x.mission});slot=$(if($x){$x.missionSlot});loads=$(if($x){$x.loads});completed=$(if($x){$x.completed});aborted=$(if($x){$x.aborted});requests=$(if($x){$x.requests});duplicates=$(if($x){$x.duplicates});rejected=$(if($x){$x.rejected});arm=$(if($x){$x.arm});peerArm=$(if($x){$x.peerArm});remaps=$(if($x){$x.remaps});digest=$(if($x){$x.digest});details=$(if($x){$x.missionDetails});received=$(if($x){$x.received});pair=$(if($x-and$x.autosavePair){$y=$x.autosavePair;"$($y.result) a$([int]$y.active) s$($y.saves)/o$($y.originated)/m$($y.mirrored)/p$([int]$y.pending) peer s$($y.peerSaves)/o$($y.peerOriginated)/m$($y.peerMirrored)/p$([int]$y.peerPending) req$($y.requests) ref$($y.refused) rej$($y.rejected)"})}};$trace.Add($row);$row}
    $dw=[Diagnostics.Stopwatch]::StartNew();$trace=[Collections.Generic.List[object]]::new();[void](DS 'before')
    C $who 'coop_player_health_hold off';Start-Sleep -Milliseconds 400;$pos=(Snap $who).pos
    C $who ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$pos[0],$pos[1],($pos[2]+45));$dw.Restart()
    do{Start-Sleep -Milliseconds 250;$d=DS 'death';$ready=$d.host.logic-eq13-and$d.client.logic-eq13-and$d.host.deathReady-and$d.host.peerDeathReady-and$d.client.deathReady-and$d.client.peerDeathReady
     foreach($r in 'host','client'){if($d[$r].phase-eq7){Save "$out/$tag-death.json" @($trace);throw "Death failed on $r ($($d[$r].result))"}}}while(!$ready-and$dw.ElapsedMilliseconds-lt30000)
    if(!$ready){Save "$out/$tag-death.json" @($trace);throw "Shared death screens not ready ($who fell)"}
    foreach($r in 'host','client'){Frame $r "$tag-gameover"};Start-Sleep -Seconds 3;I $who A 200;Start-Sleep -Milliseconds 300;I $other A 200;$dw.Restart();$lastA=@{host=0;client=0};$confirm=0
    do{Start-Sleep -Milliseconds 300;$d=DS 'restart'
     # Both still on GameOver: a refused attempt fails within 8 s and the session
     # re-arms 3 s later (1214bn); press Restart again, as a player would.
     if($d.host.logic-eq13-and$d.client.logic-eq13-and$dw.ElapsedMilliseconds-gt(12000+$confirm*12000)-and$confirm-lt4){
      $confirm++;foreach($r in 'host','client'){Frame $r "$tag-retry$confirm"};[void](DS "retry$confirm");I $who A 300}
     # A failed attempt re-arms after 3 s (1214bn); the retry below presses again.
     foreach($r in 'host','client'){
      if($d[$r].logic-in 8,15-and$dw.ElapsedMilliseconds-$lastA[$r]-gt2500-and$dw.ElapsedMilliseconds-gt6000){I $r A 300;$lastA[$r]=$dw.ElapsedMilliseconds}
      elseif($d[$r].logic-eq5-and$dw.ElapsedMilliseconds-$lastA[$r]-gt3500){I $r X 3000;$lastA[$r]=$dw.ElapsedMilliseconds}}
     $back=$d.host.logic-eq1-and!$d.host.pending-and$d.client.logic-eq1-and!$d.client.pending-and$d.host.epoch-ne$e0.host-and$d.client.epoch-ne$e0.client}while(!$back-and$dw.ElapsedMilliseconds-lt180000)
    [void](DS 'end');Save "$out/$tag-death.json" @($trace)
    if(!$back){throw "Shared checkpoint restart did not complete ($who fell)"};C $who 'coop_player_health_hold on'}
   'attack'{$npc=Npc $st.name $st.near;$hd=[Convert]::ToInt64($npc.handle.Substring(2),16);$trace=[Collections.Generic.List[object]]::new();$max=if($st.max){$st.max}else{40};$dead=$false
    if($st.heal){C host 'coop_player_health_hold on';$notes.healthHold+=@("attack $($st.name)")}
    # A story NPC's health may not be readable: then an objective completed during the fight also ends it (only then:
    # take loadout-dunwall-v1311a ended Ramsey's fight at 40 health on an unrelated completion).
    $gf0=(Snap host).gameflow
    for($k=0;$k-lt$max-and!$dead;$k++){if($k%4-eq0){$npc=Npc $st.name $st.near;if($st.behind){Behind $npc 1.0 1.6}else{Face $npc.position 1.5 1.6};Start-Sleep -Milliseconds 500}
     & "$lab/Send-D2Input.ps1" -Role host -RightTrigger 255 -DurationMs $(if($k%3-eq0){1300}else{150})|Out-Null;Start-Sleep -Milliseconds 700
     $hp=$null;try{$hp=Q host '--health' $hd}catch{};$trace.Add(@{press=$k;health=$(if($hp){$hp.current}else{$null});host=(Snap host).logic});if($hp-and$hp.current-le0){$dead=$true};$gfn=(Snap host).gameflow;if(!$hp-and$gf0-and$gfn-and$gfn[3]-gt$gf0[3]){$dead=$true}
     if((Snap host).logic-eq13){throw 'Host died in the fight'}}
    if($st.heal){C host 'coop_player_health_hold off'}
    Save "$out/$tag-fight.json" @($trace);if(!$dead){throw "$($st.name) still alive after $max attacks"}}
   'talk'{$npc=$null;try{$npc=Npc $st.name $st.near}catch{if($st.optional){$notes["$tag"]='skipped: npc gone (scene already ran)';break}else{throw}};$hit=$false
    foreach($dp in @(@(1.5,5),@(1.2,10),@(2.0,0),@(1.0,15))){Face $npc.position $dp[0] 1.65;Start-Sleep -Milliseconds 700;try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $dp[1] -Quick|Out-Null}catch{}
     $b=Snap host;if($st.leftTrigger){& "$lab/Send-D2Input.ps1" -Role host -LeftTrigger 255 -DurationMs $(if($st.hold){$st.hold}else{1500})|Out-Null}else{I host X 400};Start-Sleep -Milliseconds 2000;$a=Snap host;if($a.cine-ne$b.cine-or$a.logic-ne1-or"$($a.gameflow)"-ne"$($b.gameflow)"){$hit=$true;break}}
    if(!$hit){throw "Talk to $($st.name) not registered"}}
   'loot'{$npc=Npc $st.name $st.near;$hit=$false
    foreach($dp in @(@(1.0,45),@(0.8,60),@(1.2,35),@(0.7,70),@(1.4,30))){$d=$dp[0];Face $npc.position $d 1.2;Start-Sleep -Milliseconds 700;try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $dp[1] -Quick|Out-Null}catch{}
     # Done only when a gameflow node completes (state 3 count rises), after any scene.
     $b=Snap host;I host X 600;$w2=[Diagnostics.Stopwatch]::StartNew()
     do{Start-Sleep -Milliseconds 600;$a=Snap host;if($a.logic-eq5){I host X 3000}
      if($a.gameflow-and$b.gameflow){if($st.expect){if($a.gameflow[3]-ge$st.expect){$hit=$true}}elseif($a.gameflow[3]-gt$b.gameflow[3]){$hit=$true}}}while(!$hit-and$w2.ElapsedMilliseconds-lt9000)
     if($hit){break}}
    if(!$hit){throw "Loot of $($st.name) not registered"}}}
  Start-Sleep -Milliseconds $(if($st.settle){$st.settle}else{3000})
  [void](Settle $tag 120);Start-Sleep 2
  $rows.Add([ordered]@{step=$tag;spec=$st;host=(Snap host);client=(Snap client)});foreach($r in 'host','client'){Frame $r $tag;try{Save "$out/$tag-triggers-$r.json" (S $r '.triggers.json');Save "$out/$tag-loading-$r.json" (S $r '.loading.json');Save "$out/$tag-missionend-$r.json" (S $r '.missionend.json')}catch{}}
  Save "$out/route.json" @($rows)}
 $notes.final=@{host=(Snap host);client=(Snap client)}
 $notes.gameflowEqual=("$($notes.final.host.gameflow)"-eq"$($notes.final.client.gameflow)")
 # Both worlds must end on the same map (a mission end the partner did not follow fails the take).
 if($notes.final.host.map-ne$notes.final.client.map){throw "Worlds ended on different maps: host $($notes.final.host.map), client $($notes.final.client.map)"}
 $notes.final=@{host=(Snap host);client=(Snap client)}
 $notes.gameflowEqual=("$($notes.final.host.gameflow)"-eq"$($notes.final.client.gameflow)")
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{foreach($r in 'host','client'){Frame $r 'failure'}}catch{};try{$notes.failureState=@{host=(Snap host);client=(Snap client)}}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{C $r 'coop_product_auto off';C $r 'coop_player_health_hold off'}catch{}}
   try{C client 'coop_session off'}catch{};try{C host 'coop_session off'}catch{}
   try{Stop-D2LabVMRole -Role client|Out-Null}catch{$errors.Add("vm stop: $($_.Exception.Message)")}
  }
  $mutex.ReleaseMutex();$owned=$false
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope="Campaign route steps for $MapInfo across two machines and two Steam accounts in product mode (host Corvo on this PC, VM Emily); 1703 (#135): the VM joins after step $JoinAfterStep from its main menu (placement beside the host, gameflow and NPC lives compared), leaves after step $LeaveAfterStep and rejoins after step $RejoinAfterStep (reconnect, rearm, gameflow converges)."})
 }
 $mutex.Dispose()
}
foreach($f in Get-ChildItem $out -File -Filter *.json){if((Get-Content $f.FullName -Raw)-match '7656119[0-9]{10}'){throw "SteamID leaked into $($f.Name)"}}
Get-Content "$out/result.json";if($failure){throw $failure}
