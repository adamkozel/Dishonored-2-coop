param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='character-choice-v1778a',[string]$Candidate='candidate-npc-combat-v1214hx',[int]$VideoSeconds=1200,[int]$StopAfter=99)
# 1778 (#158, 1214hx): the partner chooses Corvo or Emily on every join, on two machines and two Steam accounts. The host
# (lab host instance on this PC, Corvo) loads Edge of the World (the first mission has the game's own choice screen) and
# hosts a friends-only lobby. The VM (second account) waits in its main menu with coop_character_choice on, joins, and
# must get the game's own message box ("Character": "Play as Corvo Attano" / "Play as Emily Kaldwin") before it loads
# the host's mission. First join: Corvo (the first button). The VM must play Corvo (its native player definition) and
# both games must report it (the pose packet's character: the VM's local 1, the host's remote 1). Then the VM leaves,
# returns to its main menu and joins again: the box must open again (every join); Emily (the second button), and both
# games must report 2. Frames of both screens and the character sidecars after every step, and a host window video.
# The partner's avatar on the host still uses the role's rig in this candidate (the pairings are #159).
# Restores: sessions and toggles off, the VM lab role stopped, both lab instances relaunched (Deploy-SourceClock) and
# checked, protected saves checked. SteamIDs are never recorded. -StopAfter N ends after phase N (1 host mission,
# 2 first box, 3 first join played, 4 second box, 5 second join played).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$notes=[ordered]@{steps=@()};$checks=[ordered]@{};$captures=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function C($r,$cmd){if($r-eq'client'){Send-D2LabVMCommand -Command $cmd -WaitConsumed|Out-Null}else{& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}}
function S($r,$suffix=''){
 if($r-eq'client'){$side=if($suffix){$suffix.TrimStart('.').Replace('.json','')}else{''}
  for($i=0;$i-lt3;$i++){try{if($side){return(Get-D2LabVMStatus -Sidecar $side)}else{return(Get-D2LabVMStatus)}}catch{Start-Sleep -Milliseconds 300}};throw "VM status unavailable $suffix"}
 for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/host/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable host"}
function I($r,[string]$b='Neutral',[int]$ms=200,[int]$lx=0,[int]$ly=0){if($r-eq'client'){Send-D2LabVMInput -Button $b -DurationMs $ms -LeftX $lx -LeftY $ly|Out-Null;return}
 & "$lab/Send-D2Input.ps1" -Role host -Button $b -DurationMs $ms -LeftX $lx -LeftY $ly|Out-Null}
function Key($vk,$wait=1500){Send-D2LabVMKey $vk;Start-Sleep -Milliseconds $wait}
function Frame($r,$name){
 if($r-eq'client'){try{Save-D2LabVMScreen -Path "$out/$name-vm.png"|Out-Null}catch{};return}
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$name-host.png" 2>$null|Out-Null}
$safe='active','host','phase','result','connectionState','role','lobbyMembers'
function Safe($o){if(!$o){return $null};$h=[ordered]@{};foreach($n in $safe){if($o.PSObject.Properties[$n]){$h[$n]=$o.$n}};[pscustomobject]$h}
function HO{try{Get-Content "$root/work/lab-instances/host/coop-status.json.online.json" -Raw|ConvertFrom-Json}catch{$null}}
function Ch($r){try{S $r '.character.json'}catch{$null}}
function Def($r){try{if($r-eq'client'){(Invoke-D2LabVMProbe -Mode '--character').definition}else{(& "$lab/bin/d2_runtime_probe.exe" $pids.host '--character'|ConvertFrom-Json).definition}}catch{$null}}
function Step($tag,[string[]]$sides=@('host','client')){foreach($r in $sides){Frame $r $tag};$h=S host;$g=$null;try{$g=S client}catch{}
 $script:notes.steps+=[ordered]@{step=$tag;host=[ordered]@{logic=$h.localGameLogicState;pending=$h.localPendingGameLogicState;map=$h.localMap;fresh=$h.remoteFresh;character=(Ch host);online=(Safe (HO))}
  vm=[ordered]@{logic=$g.localGameLogicState;pending=$g.localPendingGameLogicState;map=$g.localMap;fresh=$g.remoteFresh;character=(Ch client)}}}
# Steer both games through the native mission card, intro and load screens until both play the mission (route harnesses).
function Settle($label,$sec,[string[]]$roles){
 $w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 700;$ready=$true
  foreach($r in $roles){$x=S $r;if($x.localGameLogicState-ne1-or$x.localPendingGameLogicState-or!$x.localMap-or$x.localMap-match'menu'){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt3500){continue}
   switch($x.localGameLogicState){
    5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
    {$_-in 8,15,9,11}{I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
    12{I $r B 300;$last[$r]=$w.ElapsedMilliseconds}
    {$_-in 2,4}{I $r B 250;$last[$r]=$w.ElapsedMilliseconds}
    13{throw "$r died during $label (GameOver)"}}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt($sec*1000))
 $script:notes["settle-$label"]=[ordered]@{ready=$ready;seconds=[math]::Round($w.Elapsed.TotalSeconds)};return $ready}
# One join: the VM joins, its box opens, the harness presses the button, the VM loads and plays the host's mission.
function JoinAs($label,$lobby,[int]$button,[int]$want){
 $c0=Ch client;$o0=[int]$c0.choice.opens
 C client "coop_session join $lobby"
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 2;$c=Ch client}while((Get-Date)-lt$d-and!([int]$c.choice.state-eq1-and[int]$c.choice.opens-gt$o0))
 Start-Sleep 3;Step "$label-box" @('client');$script:checks["$label-boxOpened"]=([int]$c.choice.state-eq1-and[int]$c.choice.opens-gt$o0)
 if(!$script:checks["$label-boxOpened"]){throw "The character box did not open ($($label): $($c.choice.result))"}
 if($StopAfter-le($script:phase+1)){throw "stop:$($script:phase+1)"};$script:phase++
 foreach($k in 1,2){Key 0x25 500};if($button){Key 0x27 700};Step "$label-box-focus" @('client');Key 0x0D 2500
 $d=(Get-Date).AddSeconds(20);do{Start-Sleep 1;$c=Ch client}while((Get-Date)-lt$d-and[int]$c.choice.state-ne2)
 $script:notes["$label-choice"]=$c.choice
 if(!(Settle $label 420 @('host','client'))){Step "$label-timeout";throw "The partner did not reach the host mission ($label)"}
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$h=S host;$g=S client;$hc=Ch host;$gc=Ch client}while((Get-Date)-lt$d-and!($h.remoteFresh-and$g.remoteFresh-and[int]$hc.remote-eq$want-and[int]$gc.local-eq$want))
 I client Neutral 2500 0 32000;Start-Sleep 2;Step "$label-played"
 $def=Def client;$script:notes["$label-vmDefinition"]=$def
 $script:checks["$label-vmPlays"]=("$def"-match$(if($want-eq1){'/corvo/'}else{'/emily/'}))
 $script:checks["$label-vmReports"]=([int](Ch client).local-eq$want);$script:checks["$label-hostReceives"]=([int](Ch host).remote-eq$want)
 $script:checks["$label-connected"]=[bool]((S host).remoteFresh-and(S client).remoteFresh)
 if($StopAfter-le($script:phase+1)){throw "stop:$($script:phase+1)"};$script:phase++}
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
 $dirty=$true;$script:phase=0
 Start-D2LabVMRole -Role client|Out-Null
 $d=(Get-Date).AddSeconds(90);do{Start-Sleep 3;$gs=Get-D2LabVMStatus}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne6)
 Start-Sleep 4;Send-D2LabVMKey 0x0D;Start-Sleep 3
 if($VideoSeconds){$hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
  $va=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$VideoSeconds,'-an','-c:v','libx264','-preset','veryfast','-crf','23','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+frag_keyframe+empty_moov+default_base_moof',"$out/take-host.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $va -WindowStyle Hidden -RedirectStandardError "$out/take-host-capture.log" -PassThru))}
 Step '00-start'
 # Phase 1: the host plays Edge of the World as Corvo.
 C host 'campaign campaign/edge_of_the_world/eotw.mapinfo low corvo'
 if(!(Settle 'host-mission' 420 @('host'))){Step '01-host-timeout';throw 'The host did not reach Edge of the World'}
 Step '01-host-mission';$notes.hostDefinition=Def host;$notes.hostMap=(S host).localMap
 $checks.hostMission=("$($notes.hostMap)"-match'eotw|edge')
 if($StopAfter-le1){throw 'stop:1'};$script:phase=1
 foreach($r in 'host','client'){C $r 'coop_follow_host on';C $r 'coop_product_auto on'};C client 'coop_character_choice on'
 C host 'coop_session host_quiet';$d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$ho=HO}while((Get-Date)-lt$d-and!($ho.active-and$ho.lobby-and"$($ho.lobby)"-ne'0'))
 if(!$ho.lobby-or"$($ho.lobby)"-eq'0'){throw 'Host did not create a lobby'};$lobby="$($ho.lobby)"
 C host 'coop_player_health_hold on'
 # Phases 2-3: the first join as Corvo (the first button).
 JoinAs 'join1' $lobby 0 1
 # The VM leaves and returns to its main menu (out of session: the native Back To Main Menu), then passes its title.
 C client 'coop_session off';Start-Sleep 3
 Key 0x1B 2500;foreach($k in 1..8){Key 0x28 400};Key 0x0D 2000;Key 0x27 800;Key 0x0D 2000
 $d=(Get-Date).AddSeconds(120);do{Start-Sleep 3;$gs=S client}while((Get-Date)-lt$d-and!($gs.localGameLogicState-eq6-and!$gs.localPendingGameLogicState))
 Start-Sleep 5;Step 'join2-title' @('client');Key 0x0D 4000;Step 'join2-menu' @('client')
 # Phases 4-5: the second join as Emily (the second button).
 JoinAs 'join2' $lobby 1 2
 $checks.everyJoin=([int](Ch client).choice.opens-ge2)
 $bad=@($checks.Keys|Where-Object{!$checks[$_]});if($bad.Count){throw "Checks failed: $($bad-join ', ')"}
}catch{$failure=$_.Exception.Message;if($failure-like'stop:*'){$notes.stoppedAfter=$failure;$failure=$null}else{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Frame host 'failure';Frame client 'failure'}catch{}}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{C $r 'coop_product_auto off';C $r 'coop_follow_host off';C $r 'coop_player_health_hold off';C $r 'coop_character_choice off'}catch{}}
   try{C client 'coop_session off'}catch{};try{C host 'coop_session off'}catch{}
   try{Stop-D2LabVMRole -Role client|Out-Null}catch{$errors.Add("vm stop: $($_.Exception.Message)")}
  }
  $mutex.ReleaseMutex();$owned=$false
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;checks=$checks;notes=$notes;scope='The partner character choice on every join (#158, 1214hx) on two accounts: the host plays Edge of the World as Corvo; the VM joins twice and chooses Corvo, then Emily, in the game own message box; each time it must play and report the chosen character, and the host must receive it.'})
 }
 $mutex.Dispose()
}
foreach($f in Get-ChildItem $out -File -Filter *.json){if((Get-Content $f.FullName -Raw)-match '7656119[0-9]{10}'){throw "SteamID leaked into $($f.Name)"}}
Get-Content "$out/result.json"|Select-Object -First 30;if($failure){throw $failure}
