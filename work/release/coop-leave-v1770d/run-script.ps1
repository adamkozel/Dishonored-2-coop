param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='coop-leave-v1770a',[string]$Candidate='candidate-npc-combat-v1214ht',[int]$VideoSeconds=300)
# 1770 (#157, 1214ht): Leave co-op across two machines and two Steam accounts. The host (lab host instance on this PC,
# Corvo, simulated controller) hosts a friends-only lobby; the VM lab instance (second account, Emily, keyboard) loads
# its checkpoint and joins. Part A: the host opens the pause menu (Start), where Quit Game must read LEAVE CO-OP, moves
# to it (eight Downs, the menu does not wrap), presses A (the confirmation must ask to leave the co-op session), Right
# to the second button (Leave co-op, the native Back to main menu) and A: the pause must close as Resume does, the host
# must stay in its world (same map and world epoch), its session must end (as F8), and the VM must see the host leave.
# The pause menu opened again after the session must read Quit Game. Part B: the host hosts again, the VM joins again
# and leaves the same way from its own pause menu (Escape, eight Downs, Enter, Right, Enter). Frames of both screens
# after every step, the menu status of both games (coop-menu sidecar, pause section), and a host window video.
# Restores: both sessions off, the VM lab role stopped, both lab instances relaunched (Deploy-SourceClock) and checked;
# protected saves checked. SteamIDs are never recorded.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$notes=[ordered]@{steps=@()};$checks=[ordered]@{};$captures=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function C($r,$cmd){if($r-eq'client'){Send-D2LabVMCommand -Command $cmd -WaitConsumed|Out-Null}else{& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}}
function S($r,$suffix=''){
 if($r-eq'client'){$side=if($suffix){$suffix.TrimStart('.').Replace('.json','')}else{''}
  for($i=0;$i-lt3;$i++){try{if($side){return(Get-D2LabVMStatus -Sidecar $side)}else{return(Get-D2LabVMStatus)}}catch{Start-Sleep -Milliseconds 300}};throw "VM status unavailable $suffix"}
 for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/host/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable host"}
function I($b,[int]$ms=200){& "$lab/Send-D2Input.ps1" -Role host -Button $b -DurationMs $ms|Out-Null}
function Key($vk,$wait=1500){Send-D2LabVMKey $vk;Start-Sleep -Milliseconds $wait}
function Frame($r,$name){
 if($r-eq'client'){try{Save-D2LabVMScreen -Path "$out/$name-vm.png"|Out-Null}catch{};return}
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$name-host.png" 2>$null|Out-Null}
$safe='active','host','phase','result','connectionState','joinRequests','role','lobbyMembers'
function Safe($o){if(!$o){return $null};$h=[ordered]@{};foreach($n in $safe){if($o.PSObject.Properties[$n]){$h[$n]=$o.$n}};[pscustomobject]$h}
function HO{try{Get-Content "$root/work/lab-instances/host/coop-status.json.online.json" -Raw|ConvertFrom-Json}catch{$null}}
function GO{try{Get-D2LabVMStatus -Sidecar online}catch{$null}}
function Pause($r){$m=$null;try{$m=S $r '.coop-menu.json'}catch{};if($m){$m.pause}else{$null}}
function Step($tag,$r){Frame $r $tag;$h=S host;$g=$null;try{$g=S client}catch{}
 $script:notes.steps+=[ordered]@{step=$tag;host=[ordered]@{logic=$h.localGameLogicState;pending=$h.localPendingGameLogicState;map=$h.localMap;epoch=$h.localEpoch;pause=(Pause host);online=(Safe (HO))}
  vm=[ordered]@{logic=$g.localGameLogicState;pending=$g.localPendingGameLogicState;map=$g.localMap;epoch=$g.localEpoch;pause=(Pause client);online=(Safe (GO))}}}
function Connect($label){
 C host 'coop_session host_quiet'
 $d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$ho=HO}while((Get-Date)-lt$d-and!($ho.active-and$ho.lobby-and"$($ho.lobby)"-ne'0'))
 if(!$ho.lobby-or"$($ho.lobby)"-eq'0'){throw "Host did not create a lobby ($label)"}
 C client "coop_session join $($ho.lobby)"
 $d=(Get-Date).AddSeconds(60);do{Start-Sleep 3;$hs=S host;$gs=S client}while((Get-Date)-lt$d-and!($hs.remoteFresh-and$gs.remoteFresh-and$gs.connectionState-eq3))
 $j=[ordered]@{hostOnline=(Safe (HO));vmOnline=(Safe (GO));connected=[bool]($hs.remoteFresh-and$gs.remoteFresh)};$script:notes["join-$label"]=$j
 if(!$j.connected){throw "Two-account join did not connect ($label)"}}
function WaitHost($sec,[scriptblock]$ok){$d=(Get-Date).AddSeconds($sec);do{Start-Sleep -Milliseconds 700;$h=S host}while((Get-Date)-lt$d-and!(& $ok $h));$h}
function WaitVm($sec,[scriptblock]$ok){$d=(Get-Date).AddSeconds($sec);do{Start-Sleep 2;$g=S client}while((Get-Date)-lt$d-and!(& $ok $g));$g}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $l=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json;$pids.host=$l.pid
 if((Get-Process -Id $pids.host).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated host process'}
 if((S host).localGameLogicState-ne1){throw 'Host lab instance not in gameplay'}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 & "$root/tools/vm/Test-D2LabVM.ps1" -Repair|Out-Null;if($LASTEXITCODE){throw 'Lab VM not ready'}
 try{Stop-D2LabVMRole -Role client|Out-Null}catch{}
 & "$root/tools/vm/Sync-D2LabVMWorkspace.ps1" -Role client -SkipGameCopy *> "$out/vm-sync.log"
 $guest=Invoke-D2LabVM {(Get-FileHash C:\D2Lab\ws\outputs\d2-coop-lab\bin\dinput8.dll).Hash}
 $notes.identity=[ordered]@{candidate=$Candidate;hostDll=$pin.dll;guestDllMatches=($guest-eq$pin.dll)}
 if(!$notes.identity.guestDllMatches){throw 'Guest DLL differs from the host candidate'}
 $dirty=$true
 Start-D2LabVMRole -Role client|Out-Null
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$gs=Get-D2LabVMStatus}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne6)
 Send-D2LabVMKey 0x0D;Start-Sleep 3
 C client 'LoadGame 81'
 $d=(Get-Date).AddSeconds(120);do{Start-Sleep 3;$gs=Get-D2LabVMStatus;if($gs.localGameLogicState-in 13,8,15){Send-D2LabVMKey 0x0D}}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne1)
 if($gs.localGameLogicState-ne1){throw 'VM instance did not reach gameplay'}
 Connect 'a';Start-Sleep 3
 if($VideoSeconds){$hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
  $va=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$VideoSeconds,'-an','-c:v','libx264','-preset','veryfast','-crf','23','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+frag_keyframe+empty_moov+default_base_moof',"$out/take-host.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $va -WindowStyle Hidden -RedirectStandardError "$out/take-host-capture.log" -PassThru))}
 Step 'a0-connected' host;Frame client 'a0-connected'
 $p0=Pause host;$checks.hostTranslationHooked=[bool]$p0.translationHooked;$checks.hostInSession=[bool]$p0.inSession
 $h0=S host;$notes.hostWorld=[ordered]@{map=$h0.localMap;epoch=$h0.localEpoch}
 # --- Part A: the host leaves from its pause menu.
 I Start 180;$h=WaitHost 10 {param($s)$s.localGameLogicState-eq2};Start-Sleep 2;Step 'a1-host-pause' host
 $p1=Pause host;$checks.hostLabelled=($h.localGameLogicState-eq2-and$p1.labelled-and[int]$p1.relabels-ge1)
 foreach($k in 1..8){I Down 150;Start-Sleep -Milliseconds 450};Step 'a2-host-quit-focus' host
 $t0=[int]$p1.translations;I A 200;Start-Sleep 2;Step 'a3-host-dialog' host
 $p3=Pause host;$checks.hostDialogTranslated=([int]$p3.translations-gt$t0)
 I Right 200;Start-Sleep -Milliseconds 900;Step 'a4-host-dialog-button' host
 I A 200;$h=WaitHost 20 {param($s)$s.localGameLogicState-eq1-and!$s.localPendingGameLogicState-and!(HO).active};Start-Sleep 2;Step 'a5-host-left' host
 $p5=Pause host;$ho=HO
 $checks.hostLeft=([int]$p5.leaves-ge1-and!$ho.active)
 $checks.hostResumedInWorld=($h.localGameLogicState-eq1-and$h.localMap-eq$h0.localMap-and"$($h.localEpoch)"-eq"$($h0.localEpoch)")
 $g=WaitVm 30 {param($s)!$s.remoteFresh};Step 'a6-vm-after-host-left' client
 $checks.vmSawHostLeave=(!$g.remoteFresh-and$g.localGameLogicState-eq1)
 I Start 180;$h=WaitHost 10 {param($s)$s.localGameLogicState-eq2};Start-Sleep 2;Step 'a7-host-pause-after' host
 $p7=Pause host;$checks.hostLabelRestored=($h.localGameLogicState-eq2-and!$p7.labelled)
 I B 200;$h=WaitHost 10 {param($s)$s.localGameLogicState-eq1};Start-Sleep 1;Step 'a8-host-resumed' host
 # --- Part B: the partner leaves from its own pause menu.
 Connect 'b';Start-Sleep 3;Step 'b0-connected' client
 $q0=Pause client;$checks.vmTranslationHooked=[bool]$q0.translationHooked
 Key 0x1B 2500;Step 'b1-vm-pause' client
 $q1=Pause client;$g=S client;$checks.vmLabelled=($g.localGameLogicState-eq2-and$q1.labelled-and[int]$q1.relabels-ge1)
 foreach($k in 1..8){Key 0x28 400};Step 'b2-vm-quit-focus' client
 $u0=[int]$q1.translations;Key 0x0D 2000;Step 'b3-vm-dialog' client
 $q3=Pause client;$checks.vmDialogTranslated=([int]$q3.translations-gt$u0)
 Key 0x27 900;Step 'b4-vm-dialog-button' client
 Key 0x0D 1000;$g=WaitVm 30 {param($s)$s.localGameLogicState-eq1-and!$s.localPendingGameLogicState-and!(GO).active};Start-Sleep 2;Step 'b5-vm-left' client
 $q5=Pause client;$go=GO
 $checks.vmLeft=([int]$q5.leaves-ge1-and!$go.active)
 $checks.vmResumedInWorld=($g.localGameLogicState-eq1-and$g.localMap-and$g.localMap-notmatch'menu')
 $h=WaitHost 30 {param($s)!$s.remoteFresh};Step 'b6-host-after-vm-left' host
 $checks.hostSawVmLeave=(!$h.remoteFresh-and$h.localGameLogicState-eq1)
 Key 0x1B 2500;Step 'b7-vm-pause-after' client;$q7=Pause client;$g=S client
 $checks.vmLabelRestored=($g.localGameLogicState-eq2-and!$q7.labelled)
 Key 0x1B 2000;Step 'b8-vm-resumed' client
 $bad=@($checks.Keys|Where-Object{!$checks[$_]});if($bad.Count){throw "Checks failed: $($bad-join ', ')"}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Frame host 'failure';Frame client 'failure'}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   try{C client 'coop_session off'}catch{};try{C host 'coop_session off'}catch{}
   try{Stop-D2LabVMRole -Role client|Out-Null}catch{$errors.Add("vm stop: $($_.Exception.Message)")}
  }
  $mutex.ReleaseMutex();$owned=$false
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;checks=$checks;notes=$notes;scope='Leave co-op (#157, 1214ht) on two machines and two Steam accounts: the host and then the partner leave the session from the pause menu (LEAVE CO-OP, the co-op confirmation, Leave co-op), each stays in its world, the other side sees the leave, and the pause menu reads Quit Game again after the session.'})
 }
 $mutex.Dispose()
}
foreach($f in Get-ChildItem $out -File -Filter *.json){if((Get-Content $f.FullName -Raw)-match '7656119[0-9]{10}'){throw "SteamID leaked into $($f.Name)"}}
Get-Content "$out/result.json";if($failure){throw $failure}
