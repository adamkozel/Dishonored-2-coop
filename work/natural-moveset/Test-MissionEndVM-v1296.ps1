param([string]$Take='mission-end-vm-v1296a',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[string]$Candidate='candidate-npc-combat-v1214bc',[int]$WatchSeconds=300)
# Stage 6 mission end across two machines and two Steam accounts (chapters 89
# and 97): host lab instance on this PC = main account (Corvo), client lab
# instance in the lab VM = second account (Emily), friends-only Steam lobby,
# both in product mode (coop_product_auto, which arms the mission end mirror,
# both-ready hold and chaos sync since 1214bb). Both load Edge of the World;
# the host is placed in the Addermire Station trigger and uses the carriage.
# Both worlds must show their own result screen and reach The Good Doctor on
# the same map. Host window recorded; VM screens saved at the key states.
# The host lobby is quiet (1214bc): no invite overlay over the game.
# Online sidecars contain SteamIDs: only whitelisted fields are recorded.
# Restores: sessions off, VM instance stopped, host lab pair relaunched with
# the Dunwall fixture.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{};$hostPid=0;$capture=$null
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function HS($suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/host/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 50}};throw 'Host status unavailable'}
function HO{try{Get-Content "$root/work/lab-instances/host/coop-status.json.online.json" -Raw|ConvertFrom-Json}catch{$null}}
$safe='active','host','phase','result','connectionState','joinRequests','role','inviteOpened','lobbyMembers'
function Safe($o){if(!$o){return $null};$h=[ordered]@{};foreach($n in $safe){if($o.PSObject.Properties[$n]){$h[$n]=$o.$n}};[pscustomobject]$h}
function HC($cmd){& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function VC($cmd){Send-D2LabVMCommand -Command $cmd -WaitConsumed|Out-Null}
function HI([string]$b='',[int]$ms=100){$a=@{Role='host';DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function VI([string]$b='Neutral',[int]$ms=300){Send-D2LabVMInput -Button $b -DurationMs $ms|Out-Null}
function HQ($mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $hostPid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $hostPid $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Host query failed $mode"}
function Snap($s,$m){$o=[ordered]@{logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMap;epoch=$s.localEpoch;fresh=$s.remoteFresh;connection=$s.connectionState}
 if($m){$o.mirror=$m.result;$o.sent=$m.sent;$o.applied=$m.applied;if($m.hold){$o.hold=$m.hold.result;$o.holdWaitMs=$m.hold.lastWaitMs};if($m.chaos){$o.chaos=$m.chaos.result;$o.chaosPoints=$m.chaos.localPoints;$o.chaosLevel=$m.chaos.localLevel}};$o}
function Pair(){$h=Snap (HS) $(try{HS '.missionend.json'}catch{$null});$v=Snap (Get-D2LabVMStatus) $(try{Get-D2LabVMStatus -Sidecar missionend}catch{$null});[ordered]@{host=$h;vm=$v}}
function WaitGameplay($label,$sec){
 $d=(Get-Date).AddSeconds($sec);$last=@{host=Get-Date;vm=Get-Date}
 do{Start-Sleep 2;$p=Pair;$ready=$true
  foreach($r in 'host','vm'){$x=$p[$r];if($x.logic-ne1-or$x.pending){$ready=$false}
   if(((Get-Date)-$last[$r]).TotalSeconds-gt4){
    if($x.logic-in 8,15,11,9){if($r-eq'host'){HI A 300}else{VI A 300};$last[$r]=Get-Date}
    elseif($x.logic-eq5){if($r-eq'host'){HI X 3000}else{VI X 3000};$last[$r]=Get-Date}}}
 }while(!$ready-and(Get-Date)-lt$d)
 $notes[$label]=$p;return $ready}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $l=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json;$hostPid=$l.pid
 if((Get-Process -Id $hostPid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated host process'}
 if((HS).localGameLogicState-ne1){throw 'Host lab instance not in gameplay'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 # VM: health, the same DLL, client instance in the Dunwall fixture.
 & "$root/tools/vm/Test-D2LabVM.ps1" -Repair|Out-Null;if($LASTEXITCODE){throw 'Lab VM not ready'}
 try{Stop-D2LabVMRole -Role client|Out-Null}catch{}
 & "$root/tools/vm/Sync-D2LabVMWorkspace.ps1" -Role client -SkipGameCopy *> "$out/vm-sync.log"
 $notes.identity=[ordered]@{candidate=$Candidate;hostDll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;guestDllMatches=((Invoke-D2LabVM {(Get-FileHash C:\D2Lab\ws\outputs\d2-coop-lab\bin\dinput8.dll).Hash})-eq(Get-FileHash "$lab/bin/dinput8.dll").Hash)}
 if(!$notes.identity.guestDllMatches){throw 'Guest DLL differs from the host candidate'}
 Start-D2LabVMRole -Role client|Out-Null
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$gs=Get-D2LabVMStatus}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne6)
 Send-D2LabVMKey 0x0D;Start-Sleep 3
 VC 'LoadGame 81'
 $d=(Get-Date).AddSeconds(120);do{Start-Sleep 3;$gs=Get-D2LabVMStatus;if($gs.localGameLogicState-in 13,8,15){Send-D2LabVMKey 0x0D}}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne1)
 if($gs.localGameLogicState-ne1){throw 'VM instance did not reach gameplay'}
 # Two-account session: host lobby, VM joins; both in product mode.
 HC 'coop_session host_quiet'
 $d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$ho=HO}while((Get-Date)-lt$d-and!($ho.lobby-and"$($ho.lobby)"-ne'0'))
 if(!$ho.lobby-or"$($ho.lobby)"-eq'0'){throw 'Host did not create a lobby'}
 VC "coop_session join $($ho.lobby)"
 $d=(Get-Date).AddSeconds(60);do{Start-Sleep 3;$hs=HS;$gs=Get-D2LabVMStatus}while((Get-Date)-lt$d-and!($hs.remoteFresh-and$gs.remoteFresh-and$gs.connectionState-eq3))
 $notes.join=[ordered]@{hostOnline=Safe (HO);guestOnline=Safe (Get-D2LabVMStatus -Sidecar online);connected=($hs.remoteFresh-and$gs.remoteFresh)}
 if(!$notes.join.connected){throw 'Two-account join did not connect'}
 HC 'coop_product_auto on';VC 'coop_product_auto on'
 # Both load Edge of the World.
 HC "campaign $MapInfo $Chaos corvo";VC "campaign $MapInfo $Chaos emily"
 if(!(WaitGameplay 'load' 300)){throw 'Both instances did not reach Edge of the World gameplay'}
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$p=Pair}while((Get-Date)-lt$d-and!($p.host.fresh-and$p.vm.fresh-and$p.host.mirror-and$p.vm.mirror-and$p.host.mirror-ne'idle'-and$p.vm.mirror-ne'idle'))
 $notes.armed=$p;$notes.productHost=(HS '.product.json'|Select-Object result,step,worlds);$notes.productVm=(Get-D2LabVMStatus -Sidecar product|Select-Object result,step,worlds)
 if(!$p.host.fresh-or!$p.vm.fresh){throw 'Transport not fresh on the mission map'}
 if($p.host.mirror-ne'armed'-or$p.vm.mirror-ne'armed'){throw "Mission end mirror not armed by product mode (host $($p.host.mirror), vm $($p.vm.mirror))"}
 try{Save-D2LabVMScreen -Path "$out/vm-eotw.png"|Out-Null}catch{}
 # Host to the station trigger, then the carriage (east door stand, as in 1291).
 $ents=(HQ '--entities').entities
 $at=@{};foreach($n in 'script_trigger_addermirestation_inside_area','script_kar_cable_car_01_1'){$e=@($ents|Where-Object name -eq $n);if($e.Count-ne1){throw "Missing $n"};$at[$n]=$e[0].position}
 $hwnd=(Get-Process -Id $hostPid).MainWindowHandle.ToInt64()
 $cargs=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$WatchSeconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/end-host.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $cargs -WindowStyle Hidden -RedirectStandardError "$out/end-host-capture.log" -PassThru
 $t=$at['script_trigger_addermirestation_inside_area'];HC ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$t[0],$t[1],($t[2]+1.0));Start-Sleep 4
 $car=$at['script_kar_cable_car_01_1'];$sx=$car[0]+2.0;$sy=$car[1]+0.7;$sz=$car[2]+1.7;$look=@($car[0],($car[1]+0.7),($car[2]+1.2))
 $yaw=[Math]::Atan2($look[1]-$sy,$look[0]-$sx)*180/[Math]::PI
 HC ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$sx,$sy,$sz,$yaw);Start-Sleep -Milliseconds 900
 $v=HQ '--player';$eye=$v.nativeViewPosition;$hd=[Math]::Sqrt(($look[0]-$eye[0])*($look[0]-$eye[0])+($look[1]-$eye[1])*($look[1]-$eye[1]))
 $pitch=[Math]::Max(-45,[Math]::Min(45,[Math]::Atan2($eye[2]-$look[2],[Math]::Max($hd,.2))*180/[Math]::PI))
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $pitch -Quick|Out-Null
 HI X 250;Start-Sleep 2;if((HS).localGameLogicState-ne5){throw 'Carriage use did not start its scene'}
 # Watch both worlds through the prompt, result screens, hold and load.
 $timeline=[Collections.Generic.List[object]]::new();$clock=[Diagnostics.Stopwatch]::StartNew();$since=@{host=@{logic=5;at=0};vm=@{logic=1;at=0}};$shots=@{};$arrived=@{host=$false;vm=$false}
 do{
  Start-Sleep -Milliseconds 700;$p=Pair;$ms=$clock.ElapsedMilliseconds;$timeline.Add([ordered]@{ms=$ms;host=$p.host;vm=$p.vm})
  foreach($r in 'host','vm'){$x=$p[$r]
   if($x.logic-ne$since[$r].logic){$since[$r]=@{logic=$x.logic;at=$ms}
    if($r-eq'vm'-and!$shots["vm-$($x.logic)"]){try{Save-D2LabVMScreen -Path "$out/vm-logic$($x.logic)-$([int]($ms/1000))s.png"|Out-Null;$shots["vm-$($x.logic)"]=$true}catch{}}}
   if($x.logic-in 9,11-and!$x.pending-and$ms-$since[$r].at-gt5000){if($r-eq'host'){HI A 300}else{VI A 300};$since[$r].at=$ms}
   if($x.logic-in 8,15-and$ms-$since[$r].at-gt8000){if($r-eq'host'){HI A 300}else{VI A 300};$since[$r].at=$ms}
   if(!$arrived[$r]-and$x.map-like'*institute*'-and$x.logic-eq1-and!$x.pending){$arrived[$r]=$ms}}
 }while(!($arrived.host-and$arrived.vm)-and$clock.ElapsedMilliseconds-lt(($WatchSeconds-8)*1000))
 Start-Sleep 5;try{Save-D2LabVMScreen -Path "$out/vm-next-mission.png"|Out-Null}catch{}
 Save "$out/timeline.json" @($timeline)
 $final=Pair;$notes.final=$final;$notes.arrivedMs=$arrived
 $states=@{host=[Collections.Generic.List[string]]::new();vm=[Collections.Generic.List[string]]::new()}
 foreach($x in $timeline){foreach($r in 'host','vm'){$v2="$($x[$r].logic)/$($x[$r].pending)@$($x[$r].map)";if($states[$r].Count-eq0-or$states[$r][$states[$r].Count-1]-ne$v2){$states[$r].Add($v2)}}}
 $notes.hostStates=@($states.host);$notes.vmStates=@($states.vm)
 if(!$arrived.host-or!$arrived.vm){throw "Next mission not reached (host $($arrived.host), vm $($arrived.vm))"}
 if($final.host.map-ne$final.vm.map){throw "Next-map variant differs: host $($final.host.map), vm $($final.vm.map)"}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($capture){try{if(!$capture.HasExited-and!$capture.WaitForExit(30000)){Stop-Process -Id $capture.Id}}catch{}}
  try{VC 'coop_product_auto off'}catch{};try{HC 'coop_product_auto off'}catch{}
  try{VC 'coop_session off'}catch{};try{HC 'coop_session off'}catch{}
  try{Stop-D2LabVMRole -Role client|Out-Null}catch{$errors.Add("vm stop: $($_.Exception.Message)")}
  $mutex.ReleaseMutex();$owned=$false
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes
   scope='Edge of the World mission end across two machines and two Steam accounts in product mode (host = Corvo on this PC, VM = Emily): both worlds show their own result screen and reach The Good Doctor on the same map.'})
 }
 $mutex.Dispose()
}
foreach($f in Get-ChildItem $out -File -Filter *.json){if((Get-Content $f.FullName -Raw)-match '7656119[0-9]{10}'){throw "SteamID leaked into $($f.Name)"}}
Get-Content "$out/result.json";if($failure){throw $failure}
