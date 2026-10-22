param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='coop-hostsave-v1773a',[string]$Candidate='candidate-npc-combat-v1214hv',[int]$VideoSeconds=900,[int]$StopAfter=99)
# 1773 (#156): the host's co-op save on two machines and two Steam accounts. The host (lab host instance on this PC,
# simulated pad only, as a player with a controller) leaves its fixture for the main menu (pause > Quit Game > Back To
# Main Menu), passes the title, opens CO-OP and starts a New Game (the slot, difficulty and tutorial screens, the
# intro, the native character choice: Corvo). In the mission it hosts a friends-only lobby; the VM (second account, at
# its main menu) joins and follows the host's mission as Emily. Both play; the host saves through the pause menu (Save
# Game), then loads that save through the pause menu (Load Game); the partner must be connected again after the load.
# The host's campaign saves (lab host instance savegame) must stay unchanged throughout (slot files exactly;
# profile.bin by its keys) and its co-op folder must hold the new saves. Frames of both screens after every step, the
# menu status of both games and a host window video. -StopAfter N ends after phase N (1 menu, 2 co-op screen, 3 host in
# the mission, 4 both connected, 5 saved, 6 loaded). Restores: sessions off, the VM lab role stopped, both lab instances
# relaunched (Deploy-SourceClock) and checked, protected saves checked. SteamIDs are never recorded.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$notes=[ordered]@{steps=@()};$checks=[ordered]@{};$captures=[Collections.Generic.List[object]]::new()
$hostBase=Join-Path $root 'work/lab-instances/host/base'
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function C($r,$cmd){if($r-eq'client'){Send-D2LabVMCommand -Command $cmd -WaitConsumed|Out-Null}else{& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}}
function S($r,$suffix=''){
 if($r-eq'client'){$side=if($suffix){$suffix.TrimStart('.').Replace('.json','')}else{''}
  for($i=0;$i-lt3;$i++){try{if($side){return(Get-D2LabVMStatus -Sidecar $side)}else{return(Get-D2LabVMStatus)}}catch{Start-Sleep -Milliseconds 300}};throw "VM status unavailable $suffix"}
 for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/host/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable host"}
function I($r,[string]$b='Neutral',[int]$ms=200,[int]$lx=0,[int]$ly=0){if($r-eq'client'){Send-D2LabVMInput -Button $b -DurationMs $ms -LeftX $lx -LeftY $ly|Out-Null;return}
 & "$lab/Send-D2Input.ps1" -Role host -Button $b -DurationMs $ms -LeftX $lx -LeftY $ly|Out-Null}
function Frame($r,$name){
 if($r-eq'client'){try{Save-D2LabVMScreen -Path "$out/$name-vm.png"|Out-Null}catch{};return}
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$name-host.png" 2>$null|Out-Null}
$safe='active','host','phase','result','connectionState','role','lobbyMembers'
function Safe($o){if(!$o){return $null};$h=[ordered]@{};foreach($n in $safe){if($o.PSObject.Properties[$n]){$h[$n]=$o.$n}};[pscustomobject]$h}
function HO{try{Get-Content "$root/work/lab-instances/host/coop-status.json.online.json" -Raw|ConvertFrom-Json}catch{$null}}
function Menu($r){try{S $r '.coop-menu.json'}catch{$null}}
function Step($tag,[string[]]$sides=@('host')){foreach($r in $sides){Frame $r $tag};$h=S host;$g=$null;try{$g=S client}catch{};$m=Menu host
 $script:notes.steps+=[ordered]@{step=$tag;host=[ordered]@{logic=$h.localGameLogicState;pending=$h.localPendingGameLogicState;map=$h.localMap;epoch=$h.localEpoch;fresh=$h.remoteFresh;saves=$m.saves;online=(Safe (HO))}
  vm=[ordered]@{logic=$g.localGameLogicState;pending=$g.localPendingGameLogicState;map=$g.localMap;fresh=$g.remoteFresh}}}
function Folders{$r=[ordered]@{};foreach($d in 'savegame','savegame_coop'){$p=Join-Path $hostBase $d;$r[$d]=@(if(Test-Path $p){Get-ChildItem -LiteralPath $p -Recurse -File|ForEach-Object{
  $o=[ordered]@{path=$_.FullName.Substring($p.Length+1);length=$_.Length;written=$_.LastWriteTimeUtc.ToString('o');sha256=(Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash}
  if($_.Name-eq'profile.bin'){$x=[IO.File]::ReadAllBytes($_.FullName);if($x.Length-gt16){$o.body=[BitConverter]::ToString([Security.Cryptography.SHA256]::Create().ComputeHash($x,16,$x.Length-16)).Replace('-','')}};$o}})};$r}
function WaitHost($sec,[scriptblock]$ok){$d=(Get-Date).AddSeconds($sec);do{Start-Sleep -Milliseconds 700;$h=S host}while((Get-Date)-lt$d-and!(& $ok $h));$h}
function WaitVm($sec,[scriptblock]$ok){$d=(Get-Date).AddSeconds($sec);do{Start-Sleep 2;$g=S client}while((Get-Date)-lt$d-and!(& $ok $g));$g}
# Steer both games through the native new-game, mission card, intro and character choice screens (as the route
# harnesses do): the host takes Corvo (Left), the VM Emily (Right); logic 6 on the menu gets A every 4 s (host only).
function Settle($label,$sec,[string[]]$roles){
 $w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0};$choice=@{host=0;client=0};$menuA=0
 do{Start-Sleep -Milliseconds 700;$ready=$true
  foreach($r in $roles){$x=S $r;if($x.localGameLogicState-ne1-or$x.localPendingGameLogicState-or!$x.localMap-or$x.localMap-match'menu'){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt3500){continue}
   switch($x.localGameLogicState){
    14{$choice[$r]++;Start-Sleep -Milliseconds 1500;$side=if($r-eq'host'){'Left'}else{'Right'};foreach($k in 1,2){I $r $side 250 $(if($side-eq'Left'){-32000}else{32000});Start-Sleep -Milliseconds 600};Start-Sleep -Milliseconds 400;I $r A 400;$last[$r]=$w.ElapsedMilliseconds}
    3{Frame $r "$label-question-$($w.ElapsedMilliseconds)";if($x.localMap-and$x.localMap-notmatch'menu'){I $r Right 250;Start-Sleep -Milliseconds 400};I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
    5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
    {$_-in 8,15,9,11}{I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
    12{$choice[$r]++;I $r $(if($choice[$r]%2){'B'}else{'A'}) 300;$last[$r]=$w.ElapsedMilliseconds}
    {$_-in 2,4}{I $r B 250;$last[$r]=$w.ElapsedMilliseconds}
    6{if($r-eq'host'-and!$x.localPendingGameLogicState){I host A 250;$menuA++;$last[$r]=$w.ElapsedMilliseconds}}
    13{throw "$r died during $label (GameOver)"}}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt($sec*1000))
 $script:notes["settle-$label"]=[ordered]@{ready=$ready;seconds=[math]::Round($w.Elapsed.TotalSeconds);choices=$choice;menuA=$menuA}
 return $ready}
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
 $notes.identity=[ordered]@{candidate=$Candidate;hostDll=$pin.dll;guestDllMatches=($guest-eq$pin.dll)};if(!$notes.identity.guestDllMatches){throw 'Guest DLL differs from the host candidate'}
 $dirty=$true
 # The host's own lab co-op folder starts empty (its earlier test saves), so New Game asks no overwrite question.
 $coopDir=Join-Path $hostBase 'savegame_coop';$notes.clearedCoop=if(Test-Path $coopDir){Remove-Item -LiteralPath $coopDir -Recurse -Force;'removed'}else{'absent'}
 $notes.before=Folders
 Start-D2LabVMRole -Role client|Out-Null
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$gs=Get-D2LabVMStatus}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne6)
 Start-Sleep 4;Send-D2LabVMKey 0x0D;Start-Sleep 3
 if($VideoSeconds){$hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
  $va=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$VideoSeconds,'-an','-c:v','libx264','-preset','veryfast','-crf','23','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+frag_keyframe+empty_moov+default_base_moof',"$out/take-host.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $va -WindowStyle Hidden -RedirectStandardError "$out/take-host-capture.log" -PassThru))}
 Step '00-start' @('host','client')
 # Phase 1: the host leaves its fixture for the main menu (no session yet: the native Back To Main Menu).
 I host Start 180;$h=WaitHost 10 {param($s)$s.localGameLogicState-eq2};Start-Sleep 2;Step '01-pause'
 foreach($k in 1..8){I host Down 150;Start-Sleep -Milliseconds 450};I host A 200;Start-Sleep 2;Step '02-quit-dialog'
 I host Right 200;Start-Sleep -Milliseconds 900;I host A 200
 $h=WaitHost 120 {param($s)$s.localGameLogicState-eq6-and!$s.localPendingGameLogicState};Start-Sleep 6;Step '03-title'
 $i0=[int](Menu host).inserts;I host A 250;$d=(Get-Date).AddSeconds(45);do{Start-Sleep 2;$m=Menu host}while((Get-Date)-lt$d-and!([int]$m.inserts-gt$i0));Start-Sleep 2;Step '04-menu'
 if([int]$m.inserts-le$i0){throw 'Co-op entry not inserted on the host menu'};if($StopAfter-le1){throw 'stop:1'}
 # Phase 2: CO-OP (under Campaign) and its screen with the co-op saves.
 I host Down 200;Start-Sleep -Milliseconds 800;I host A 250;$d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$m=Menu host}while((Get-Date)-lt$d-and!($m.saves.active-and$m.saves.phase-eq0));Start-Sleep 2;Step '05-coop-screen'
 $checks.coopSaves=[bool]$m.saves.active;if(!$checks.coopSaves){throw "Co-op saves not active ($($m.saves.result))"};if($StopAfter-le2){throw 'stop:2'}
 # Phase 3: New Game (CONTINUE has the focus and is locked without co-op saves), its screens, the mission as Corvo.
 I host Down 200;Start-Sleep -Milliseconds 800;Step '06-newgame-focus';I host A 250;Start-Sleep 3;Step '07-newgame'
 if(!(Settle 'newgame' 480 @('host'))){Step '08-newgame-timeout';throw 'The host did not reach the mission'}
 Step '08-host-mission';$h=S host;$notes.hostMission=[ordered]@{map=$h.localMap;epoch=$h.localEpoch}
 try{$notes.hostCharacter=(& "$lab/bin/d2_runtime_probe.exe" $pids.host '--character'|ConvertFrom-Json).definition}catch{}
 $checks.hostCorvo=("$($notes.hostCharacter)"-match'corvo');$notes.afterStart=Folders
 $checks.missionStartSaved=(@($notes.afterStart.savegame_coop|Where-Object{$_.path-match'^SLOT\d\d\\'}).Count-gt0);if($StopAfter-le3){throw 'stop:3'}
 # Phase 4: the session. The host hosts a lobby and publishes its mission; the VM joins from its menu and follows.
 foreach($r in 'host','client'){C $r 'coop_follow_host on';C $r 'coop_product_auto on'}
 C host 'coop_session host_quiet';$d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$ho=HO}while((Get-Date)-lt$d-and!($ho.active-and$ho.lobby-and"$($ho.lobby)"-ne'0'))
 if(!$ho.lobby-or"$($ho.lobby)"-eq'0'){throw 'Host did not create a lobby'}
 C client "coop_session join $($ho.lobby)"
 if(!(Settle 'join' 420 @('host','client'))){Step '09-join-timeout' @('host','client');throw 'The partner did not reach the host mission'}
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$h=S host;$g=S client}while((Get-Date)-lt$d-and!($h.remoteFresh-and$g.remoteFresh))
 Step '09-connected' @('host','client');$checks.connected=[bool]($h.remoteFresh-and$g.remoteFresh-and$g.localMap-eq$h.localMap)
 if(!$checks.connected){throw 'Two-account session did not connect in the mission'}
 C host 'coop_player_health_hold on';C client 'coop_player_health_hold on'
 # Play: both walk forward a little.
 I host Neutral 2500 0 32000;I client Neutral 2500 0 32000;Start-Sleep 2;Step '10-played' @('host','client');if($StopAfter-le4){throw 'stop:4'}
 # Phase 5: the host saves through the pause menu (Save Game, the second entry; the first slot of the save screen).
 $b5=Folders;I host Start 180;$h=WaitHost 10 {param($s)$s.localGameLogicState-eq2};Start-Sleep 2;Step '11-save-pause'
 I host Down 200;Start-Sleep -Milliseconds 600;I host A 250;Start-Sleep 3;Step '12-save-screen'
 I host A 250;Start-Sleep 3;Step '13-save-chosen'
 $h=S host;if($h.localGameLogicState-eq3){I host A 250;Start-Sleep 3;Step '14-save-confirmed'}
 Start-Sleep 5;$notes.afterSave=Folders
 $new=@($notes.afterSave.savegame_coop|Where-Object{$_.path-match'^SLOT(0[4-9]|1\d|20)\\game\.details$'});$checks.manualSaveWritten=($new.Count-gt0);$notes.manualSaves=@($new|ForEach-Object path)
 foreach($k in 1..3){$h=S host;if($h.localGameLogicState-eq1){break};I host B 250;Start-Sleep 1500}
 $h=WaitHost 15 {param($s)$s.localGameLogicState-eq1};Step '15-saved-resumed' @('host','client');if($StopAfter-le5){throw 'stop:5'}
 # Phase 6: the host loads that save through the pause menu (Load Game, the third entry; the newest save first).
 I host Neutral 2000 0 32000;$e0=(S host).localEpoch
 I host Start 180;$h=WaitHost 10 {param($s)$s.localGameLogicState-eq2};Start-Sleep 2;Step '16-load-pause'
 foreach($k in 1,2){I host Down 200;Start-Sleep -Milliseconds 600};I host A 250;Start-Sleep 3;Step '17-load-screen'
 I host A 250;Start-Sleep 3;Step '18-load-chosen'
 $h=S host;if($h.localGameLogicState-eq3){Step '19-load-question';I host A 250;Start-Sleep 3}
 if(!(Settle 'load' 300 @('host'))){Step '20-load-timeout' @('host','client');throw 'The host did not come back from the load'}
 $h=S host;$checks.loaded=($h.localGameLogicState-eq1-and"$($h.localEpoch)"-ne"$e0"-and$h.localMap-eq$notes.hostMission.map)
 $d=(Get-Date).AddSeconds(180);do{Start-Sleep 3;$h=S host;$g=S client;if($g.localGameLogicState-ne1){Settle 'partner-after-load' 20 @('client')|Out-Null}}while((Get-Date)-lt$d-and!($h.remoteFresh-and$g.remoteFresh-and$g.localGameLogicState-eq1))
 Step '20-loaded' @('host','client');$checks.partnerAfterLoad=[bool]($h.remoteFresh-and$g.remoteFresh)
 # The campaign saves: slot files exactly, profile.bin by its keys; the co-op folder holds the new saves.
 $notes.final=Folders
 $sig={param($f)if($f.path-eq'profile.bin'){"$($f.path)|$($f.body)"}else{"$($f.path)|$($f.length)|$($f.written)|$($f.sha256)"}}
 $checks.campaignUnchanged=((@($notes.before.savegame|ForEach-Object{& $sig $_})-join"`n")-eq(@($notes.final.savegame|ForEach-Object{& $sig $_})-join"`n"))
 $pb=@($notes.before.savegame|Where-Object path -eq 'profile.bin')[0];$pa=@($notes.final.savegame|Where-Object path -eq 'profile.bin')[0]
 $notes.profileRewritten=[bool]($pb-and$pa-and($pb.sha256-ne$pa.sha256-or$pb.written-ne$pa.written))
 $bad=@($checks.Keys|Where-Object{!$checks[$_]});if($bad.Count){throw "Checks failed: $($bad-join ', ')"}
}catch{$failure=$_.Exception.Message;if($failure-like'stop:*'){$notes.stoppedAfter=$failure;$failure=$null}else{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Frame host 'failure';Frame client 'failure'}catch{}}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}
  try{$notes.end=Folders}catch{}
  if($dirty){
   foreach($r in 'host','client'){try{C $r 'coop_product_auto off';C $r 'coop_follow_host off';C $r 'coop_player_health_hold off'}catch{}}
   try{C client 'coop_session off'}catch{};try{C host 'coop_session off'}catch{}
   try{Stop-D2LabVMRole -Role client|Out-Null}catch{$errors.Add("vm stop: $($_.Exception.Message)")}
  }
  $mutex.ReleaseMutex();$owned=$false
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;checks=$checks;notes=$notes;scope='The host co-op save (#156) on two machines and two Steam accounts: the host (lab host, pad) opens CO-OP, starts a New Game as Corvo, hosts; the VM joins as Emily; the host saves and loads through the pause menu; its campaign saves stay unchanged and the co-op folder holds the saves.'})
 }
 $mutex.Dispose()
}
foreach($f in Get-ChildItem $out -File -Filter *.json){if((Get-Content $f.FullName -Raw)-match '7656119[0-9]{10}'){throw "SteamID leaked into $($f.Name)"}}
Get-Content "$out/result.json"|Select-Object -First 30;if($failure){throw $failure}
