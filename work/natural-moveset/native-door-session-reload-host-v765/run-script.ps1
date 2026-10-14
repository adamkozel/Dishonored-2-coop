param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-door-session-reload-client-v764',[ValidateSet('host','client')][string]$Source='client',[ValidateSet('ld_door_350cm_int_rich_wood_01_3','ld_door_350cm_int_rich_wood_01_2')][string]$DoorName='ld_door_350cm_int_rich_wood_01_3')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$capture=$null;$monitor=$null;$peerCapture=$null
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new()
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function DAll($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.door-sync.json" -Raw|ConvertFrom-Json}
function RS($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.reset.json" -Raw|ConvertFrom-Json}
function DS($r){$d=@((DAll $r).entries|Where-Object name -eq $DoorName);if($d.Count-ne1){throw 'Selected door channel absent or ambiguous'};return $d[0]}
function CatalogReady($d){$d.enabled-and$d.catalogCount-eq6-and$d.ambiguous-eq0-and$d.ineligible-eq4-and@($d.entries|Where-Object {!$_.enabled-or!$_.ready-or$_.revision-ne1-or$_.applied-ne1}).Count-eq0}
$initialOpen=$DoorName-eq'ld_door_350cm_int_rich_wood_01_2'
$expectedCycle=if($initialOpen){'2,1,0,1,2'}else{'0,1,2,1,0'}
function S($r){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
function W($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing object field $offset"};[Convert]::ToUInt64($w[0].word.Substring(2),16)}
function DoorSample($r){
 $d=$doorBindings[$r];$o=Q $r '--object' $d.component
 if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$d.owner-or((W $o '0x18')-band [uint64]4294967295)-ne$d.generation){$o|ConvertTo-Json -Depth 7|Set-Content "$out/failed-door-$r.json";throw 'Door component lifetime changed'}
 $state=W $o '0x48';$flags=W $o '0x1e8'
 [pscustomobject]@{handle=$d.handle;current=[uint32]($state-band [uint64]4294967295);pending=[uint32]($state-shr 32);locked=[int]($flags-band 255);flags1e8=('0x{0:x}'-f$flags);state90=('0x{0:x}'-f(W $o '0x90'));movingTarget=('0x{0:x}'-f(W $o '0xb0'));child=[uint32]((W $o '0xd8')-shr 32)}
}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='NativeDoorSessionReload764';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
 $pin=Get-Content "$root/work/movement-integration/candidate-door-session-v763/identity.json" -Raw|ConvertFrom-Json
 if($dll-ne$pin.dll){throw 'Expected archived763 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true
 foreach($source in @($Source)){
  $peer=if($source-eq'host'){'client'}else{'host'}
  foreach($case in @(@{name='unlocked-door-tap';yaw=270;stance=0;distance=1.0})){
   if($initialOpen){throw 'Reload interruption requires the verified closed closet door'}
   I $source;I $peer
   $state=Q $source '--player-states';if($state.stance-ne$case.stance){I $source B 180;Start-Sleep -Milliseconds 500}
   if($initialOpen){
    C $peer 'setviewpos -48 -158.5 68.1 215 0'
    C $source 'setviewpos -50.3 -160.75 68.1 180 0'
   }else{
    C $peer 'setviewpos -44 -161 68.1 220 0'
    C $source 'setviewpos -47.05 -163.5 68.1 270 0'
   }
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$out/source-camera.json" -Quick
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 10 -Evidence "$out/observer-camera.json" -Quick
   $components=Q $source '--components' 1
   $components|ConvertTo-Json -Depth 8|Set-Content "$out/player-components-before.json"
   $sensors=@($components.components|Where-Object {$_.type-match'(?i)(PlayerSensor.*(Interaction|Use)|PlayerFocus)'})
   foreach($r in 'host','client'){@((Q $r '--entities').entities|Where-Object {$_.type-match'Door'})|ConvertTo-Json -Depth 8|Set-Content "$out/doors-before-$r.json"}
   $doorBindings=@{}
   foreach($r in 'host','client'){
    $door=@((Q $r '--entities').entities|Where-Object name -eq $DoorName)
    if($door.Count-ne1-or$door[0].type-ne'.?AVarkDoor@@'){throw 'Exact ordinary door missing'};$door=$door[0]
    $cp=@((Q $r '--components' $door.handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@')
    if($cp.Count-ne1-or$cp[0].owner-ne$door.pointer){throw 'Door component owner mismatch'};$cp=$cp[0]
    $obj=Q $r '--object' $cp.pointer;$obj|ConvertTo-Json -Depth 7|Set-Content "$out/openable-before-$r.json"
    $doorBindings[$r]=[pscustomobject]@{handle=$door.handle;component=$cp.pointer;owner=[Convert]::ToUInt64($door.pointer.Substring(2),16);generation=((W $obj '0x18')-band [uint64]4294967295)}
    if((DoorSample $r).locked-ne0){throw 'Unlocked door required'}
   }
   $doorBindings|ConvertTo-Json -Depth 5|Set-Content "$out/door-bindings.json"
   $character=if($source-eq'host'){'corvo'}else{'emily'}
   $entry=Get-Content "$PSScriptRoot/heavy-bank-manifest-v633.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
   $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank hash differs'}
   Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force;C $peer 'coop_movement_bank on'
   $bw=[Diagnostics.Stopwatch]::StartNew();do{$m=Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json;if($m.active-and$m.version-eq67){break};Start-Sleep -Milliseconds 40}while($bw.ElapsedMilliseconds-lt5000)
   if(!$m.active-or$m.version-ne67){throw 'Choke bank preload failed'}
   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
   foreach($r in 'host','client'){
    if((RS $r).enabled){C $r 'coop_checkpoint_sync off'}
    C $r 'coop_checkpoint_sync on';C $r $(if($r-eq$peer){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})
   }
   $cw=[Diagnostics.Stopwatch]::StartNew();do{$rh=RS host;$rc=RS client;if($rh.enabled-and$rc.enabled-and$rh.peerArm-and$rc.peerArm-and!$rh.phase-and!$rc.phase){break};Start-Sleep -Milliseconds 50}while($cw.ElapsedMilliseconds-lt5000)
   if(!$rh.enabled-or!$rc.enabled-or!$rh.peerArm-or!$rc.peerArm-or$rh.phase-or$rc.phase-or$rh.digest-ne$rc.digest){throw 'Shared checkpoint pair did not arm'}
   $resetBefore=@{host=$rh;client=$rc};$bridgeBefore=@{host=(S host);client=(S client)}
   $resetBefore|ConvertTo-Json -Depth 8|Set-Content "$out/reset-before.json"
   $nativeSwitchBefore=@{}
   foreach($r in 'host','client'){
    $nativeSwitchBefore[$r]=Get-Content "$root/work/lab-instances/$r/coop-status.json.loading.json" -Raw|ConvertFrom-Json
    $nativeSwitchBefore[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/loading-before-$r.json"
   }
   foreach($r in 'host','client'){C $r 'coop_door_sync session'}
   $dw=[Diagnostics.Stopwatch]::StartNew();do{$dh=DAll host;$dc=DAll client;if((CatalogReady $dh)-and(CatalogReady $dc)){break};Start-Sleep -Milliseconds 40}while($dw.ElapsedMilliseconds-lt6000)
   $catalogBefore=@{host=$dh;client=$dc};$catalogBefore|ConvertTo-Json -Depth 9|Set-Content "$out/catalog-before.json"
   if(!(CatalogReady $dh)-or!(CatalogReady $dc)){throw 'Six-door catalog not armed'}
   foreach($r in 'host','client'){if((DoorSample $r).current-ne$(if($initialOpen){2}else{0})){throw 'Initial door state differs'}}
   $key="$source-$($case.name)";$hwnd=(Get-Process -Id $pids[$source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
   $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','40','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$key.mp4")
   $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$key-capture.log" -RedirectStandardOutput "$out/$key-capture.stdout" -PassThru
   $peerHwnd=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();$peerArgs=$args.Clone();$graphIndex=[Array]::FindIndex($peerArgs,[Predicate[object]]{param($x) "$x" -like "gfxcapture=*"});if($graphIndex-lt0){throw "Capture graph absent"};$peerArgs[$graphIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$peerArgs[$peerArgs.Length-1]="$out/observer.mp4"
   $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $peerArgs -WindowStyle Hidden -RedirectStandardError "$out/observer-capture.log" -RedirectStandardOutput "$out/observer-capture.stdout" -PassThru
   $inputs=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new()
   I $source X 200;$inputs.Add([pscustomobject]@{input='native open';role=$source})
   $moving=[Diagnostics.Stopwatch]::StartNew();do{
    $ns=DoorSample $source;$np=DoorSample $peer
    if($ns.current-eq1-and$np.current-eq1-and(DS $peer).nativeCalls-eq1){break};Start-Sleep -Milliseconds 15
   }while($moving.ElapsedMilliseconds-lt3500)
   [ordered]@{source=$ns;peer=$np;sourceSync=(DS $source);peerSync=(DS $peer)}|ConvertTo-Json -Depth 8|Set-Content "$out/door-at-interruption.json"
   if($ns.current-ne1-or$np.current-ne1){throw 'Both native doors must be Moving at the interruption'}
   C $source 'RestartFromCheckpoint';$inputs.Add([pscustomobject]@{input='RestartFromCheckpoint';role=$source})
   C $peer 'RestartFromMemoryCheckpoint';$inputs.Add([pscustomobject]@{input='duplicate RestartFromMemoryCheckpoint';role=$peer})
   $watch=[Diagnostics.Stopwatch]::StartNew();$lastPress=@{host=0;client=0};$done=$false
   do{
    $pair=@{};$done=$true
    foreach($r in 'host','client'){
     $reset=RS $r;$bridge=S $r;$doors=DAll $r
     $pair[$r]=[pscustomobject]@{reset=$reset;bridge=$bridge;doors=$doors}
     if($reset.phase-eq7){throw "Shared checkpoint failed: $r $($reset.result)"}
     if($reset.completed-ne$resetBefore[$r].completed+1-or$bridge.localGameLogicState-ne1-or$bridge.localPendingGameLogicState){$done=$false}
     if(!$reset.held-and$watch.ElapsedMilliseconds-gt6000-and$bridge.localGameLogicState-in8,15-and$watch.ElapsedMilliseconds-$lastPress[$r]-gt700){I $r A 100;$lastPress[$r]=$watch.ElapsedMilliseconds}
    }
    $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;host=$pair.host;client=$pair.client})
    if(!$done){Start-Sleep -Milliseconds 70}
   }while(!$done-and$watch.ElapsedMilliseconds-lt30000)
   if(!$done){throw 'Shared reload did not finish on both players'}
   $avatarWatch=[Diagnostics.Stopwatch]::StartNew();$avatarReady=$false
   do{
    $pair=@{};$avatarReady=$true
    foreach($r in 'host','client'){
     $reset=RS $r;$bridge=S $r;$pair[$r]=[pscustomobject]@{reset=$reset;bridge=$bridge;doors=(DAll $r)}
     if(!$reset.avatarReady-or$reset.avatarPending-or!$bridge.probeFollowEnabled){$avatarReady=$false}
     if($reset.avatarRefused-ne$resetBefore[$r].avatarRefused){throw 'Counterpart restore refused'}
    }
    $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;host=$pair.host;client=$pair.client})
    if(!$avatarReady){Start-Sleep -Milliseconds 80}
   }while(!$avatarReady-and$avatarWatch.ElapsedMilliseconds-lt6000)
   if(!$avatarReady){throw 'Both counterpart-ready signals did not arrive'}
   $renewWatch=[Diagnostics.Stopwatch]::StartNew()
   do{
    $renewH=DAll host;$renewC=DAll client
    $renewed=(CatalogReady $renewH)-and(CatalogReady $renewC)-and$renewH.session.rearms-eq1-and$renewC.session.rearms-eq1
    if(!$renewed){Start-Sleep -Milliseconds 50}
   }while(!$renewed-and$renewWatch.ElapsedMilliseconds-lt6000)
   if(!$renewed){throw 'Fresh door catalogs did not renew automatically'}
   $final=@{}
   foreach($r in 'host','client'){
    $reset=RS $r;$bridge=S $r;$doors=DAll $r;$character=Q $r '--character';$entities=Q $r '--entities'
    $final[$r]=[pscustomobject]@{reset=$reset;bridge=$bridge;doors=$doors;character=$character}
    $final[$r]|ConvertTo-Json -Depth 14|Set-Content "$out/reset-after-native-$r.json"
    $switches=Get-Content "$root/work/lab-instances/$r/coop-status.json.loading.json" -Raw|ConvertFrom-Json
    $switches|ConvertTo-Json -Depth 8|Set-Content "$out/loading-after-$r.json"
    if($switches.switches-$nativeSwitchBefore[$r].switches-gt32){throw 'Native loading trace coverage lost'}
    $events=@($switches.events|Where-Object sequence -gt $nativeSwitchBefore[$r].switches)
    if(@($events|Where-Object {$_.caller-eq'0x787a55'-and$_.to-eq15}).Count-ne1-or@($events|Where-Object {$_.caller-eq'0x7879cd'-and$_.to-eq1}).Count-ne1){throw 'Expected exactly one actual native loading entry and finish'}
    if($reset.continuationAccepted-$resetBefore[$r].continuationAccepted-gt1){throw 'Multiple native memory continuations accepted'}
    $avatars=@($entities.entities|Where-Object name -match '^d2coop_probe_')
    if($avatars.Count-ne1-or$avatars[0].name-ne$(if($r-eq'host'){'d2coop_probe_emily'}else{'d2coop_probe_corvo'})-or$avatars[0].handle-ne('0x'+([uint32]$bridge.probeActorHandle).ToString('x'))-or$reset.avatarRestored-ne$resetBefore[$r].avatarRestored+1){throw 'Expected exactly one restored following counterpart'}
    if($reset.loads-ne$resetBefore[$r].loads+1-or$reset.completed-ne$resetBefore[$r].completed+1-or$reset.aborted-ne$resetBefore[$r].aborted-or$reset.maxDistance-gt0.001){throw 'Shared checkpoint did not complete exactly one guarded load'}
    if($character.character-ne$(if($r-eq'host'){1}else{2})-or$character.queued-or$bridge.localEpoch-eq$bridgeBefore[$r].localEpoch){throw 'New world epoch or preserved player role missing'}
    if(!(CatalogReady $doors)-or!$doors.session.enabled-or$doors.session.reloadPending-or$doors.session.reloads-ne1-or$doors.session.rearms-ne1-or$doors.session.retiredChannels-ne6){throw 'Expected exactly one automatic fresh catalog renewal'}
    if($doors.session.retiredNativeCalls-ne$(if($r-eq$peer){1}else{0})){throw 'Old-world native call count changed during retirement'}
    foreach($old in $catalogBefore[$r].entries){
     $new=@($doors.entries|Where-Object name -eq $old.name)
     if($new.Count-ne1-or$new[0].arm-le$old.arm-or$new[0].nativeCalls-ne0){throw 'New-world binding reused an arm or replayed a native request'}
    }
    if($doors.session.epoch-ne$bridge.localEpoch-or$doors.session.peerEpoch-ne$bridge.remoteEpoch){throw 'Fresh catalog epochs differ from the ready pair'}
    $isolation=Get-Content "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json" -Raw|ConvertFrom-Json
    $isolation|ConvertTo-Json -Depth 6|Set-Content "$out/postload-isolation-$r.json"
    if($isolation.active-or$isolation.heldReferences){throw 'Door renewal unexpectedly retained AI isolation'}
    $npcs=Q $r '--world-npcs';$ramsey=@($npcs.npcs|Where-Object spawnerName -eq 'ld_spawner_ramsey_escape')
    if($ramsey.Count-ne1){throw 'Native Ramsey identity unavailable'}
    $decision=Q $r '--ai-decision' $ramsey[0].handle;$decision|ConvertTo-Json -Depth 7|Set-Content "$out/postload-native-ai-$r.json"
    if(!$decision.started-or!$decision.schedulerActive-or$decision.pauseReferences){throw 'Native AI did not resume after checkpoint load'}
    $door=@($entities.entities|Where-Object name -eq $DoorName)
    if($door.Count-ne1){throw 'New-world authored door missing'}
    $cp=@((Q $r '--components' $door[0].handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@')
    if($cp.Count-ne1-or$cp[0].owner-ne$door[0].pointer){throw 'New-world door component owner mismatch'}
    $obj=Q $r '--object' $cp[0].pointer;$obj|ConvertTo-Json -Depth 7|Set-Content "$out/new-world-openable-$r.json"
    if((W $obj '0x48')-ne0){throw 'New checkpoint door inherited the old-world open request'}
    $doorBindings[$r]=[pscustomobject]@{handle=$door[0].handle;component=$cp[0].pointer;owner=[Convert]::ToUInt64($door[0].pointer.Substring(2),16);generation=((W $obj '0x18')-band [uint64]4294967295)}
    if(!$reset.avatarReady-or$reset.avatarPending-or!$bridge.probeFollowEnabled){throw 'Counterpart presentation not restored'}
   }
   $h=$final.host.reset;$c=$final.client.reset
   if($h.transaction-ne$c.transaction-or$h.hostReady-ne$c.hostReady-or$h.clientReady-ne$c.clientReady-or$h.releasedAt-lt$c.readyAt-or$c.releasedAt-lt$h.readyAt){throw 'Shared reset readiness/transaction mismatch'}
   if(($h.duplicates-$resetBefore.host.duplicates)+($c.duplicates-$resetBefore.client.duplicates)-lt1){throw 'Duplicate checkpoint request was not observed'}
   $final|ConvertTo-Json -Depth 14|Set-Content "$out/reset-final.json"
   # No second door-enable command: use the automatically rebuilt channels.
   C $peer 'setviewpos -44 -161 68.1 220 0';C $source 'setviewpos -47.05 -163.5 68.1 270 0'
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$out/postload-source-camera.json" -Quick
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 10 -Evidence "$out/postload-peer-camera.json" -Quick
   I $source X 200;$inputs.Add([pscustomobject]@{input='fresh native open after automatic renewal';role=$source})
   $useWatch=[Diagnostics.Stopwatch]::StartNew();$afterUse=$null
   do{
    $ns=DoorSample $source;$np=DoorSample $peer;$ds=DS $source;$dp=DS $peer
    $afterUse=@{nativeSource=$ns;nativePeer=$np;source=$ds;peer=$dp}
    $used=$ns.current-eq2-and$np.current-eq2-and$ds.revision-eq2-and$dp.revision-eq2-and$ds.applied-eq2-and$dp.applied-eq2
    if(!$used){Start-Sleep -Milliseconds 65}
   }while(!$used-and$useWatch.ElapsedMilliseconds-lt5000)
   $afterUse|ConvertTo-Json -Depth 10|Set-Content "$out/postload-door-use.json"
   if(!$used-or!$ds.enabled-or!$dp.enabled-or$ds.nativeCalls-ne0-or$dp.nativeCalls-ne1){throw 'Fresh native use did not synchronize after automatic catalog renewal'}

   $samples|ConvertTo-Json -Depth 15|Set-Content "$out/reload-observations.json"
   $inputs|ConvertTo-Json -Depth 5|Set-Content "$out/inputs.json"
   $checks.Add('Both native doors were Moving when either player requested one shared reload; duplicate peer retry performed no extra load')
   $checks.Add('Both roles and counterparts restored after both-ready hold; six fresh door channels renew without manual setup; native AI resumes; checkpoint-closed state has no replay, then a fresh use synchronizes')
   foreach($r in 'host','client'){C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0'}
   $capture.WaitForExit(40000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){throw 'Native capture failed'};$capture=$null
   C $peer 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
   $rows.Add([pscustomobject]@{source=$source;case=$case;key=$key;count=$samples.Count;sensors=@($sensors.type);partnerHandle=(S $source).probeActorHandle})
   foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed during neutral targeting observation'}}
  }
 }
 $checks.Add('Both native views recorded across interrupted door motion, shared loading and checkpoint return')
}catch{$failure=$_.Exception.Message;if($samples){$samples|ConvertTo-Json -Depth 17|Set-Content "$out/partial-observations.json"};if($inputs){$inputs|ConvertTo-Json|Set-Content "$out/partial-inputs.json"}}finally{
 if($owned){
  foreach($r in 'host','client'){
   foreach($suffix in 'checkpoint.json','reset.json','loading.json','replacement.json'){
    $trace="$root/work/lab-instances/$r/coop-status.json.$suffix"
    if(Test-Path -LiteralPath $trace){Copy-Item -LiteralPath $trace -Destination "$out/final-$r-$suffix"}
   }
   if($failure){foreach($name in 'engine.log','loader.log'){Copy-Item "$root/work/lab-instances/$r/$name" "$out/failed-$r-$name"}}
  }
  foreach($r in 'host','client'){try{I $r}catch{}}
  if($peerCapture){$peerCapture.WaitForExit(45000)|Out-Null;if(!$peerCapture.HasExited-or$peerCapture.ExitCode){$failure=($failure+'; observer capture incomplete').TrimStart('; ')}}
  if($monitor){$monitor.WaitForExit(15000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene monitor incomplete').TrimStart('; ')}}
  if($capture){$capture.WaitForExit(10000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture incomplete').TrimStart('; ')}}
  foreach($r in 'host','client'){try{C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';C $r 'coop_door_sync off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off'}catch{}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
   foreach($r in 'host','client'){
    $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-checkpoint-restored-$r.json"
    if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete checkpoint inventory differs'}
    if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}
   };$checks.Add('Both checkpoint inventories and player/partner health restored')
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path -LiteralPath $out){[ordered]@{passed=(!$failure);failure=$failure;dll=$dll;checks=$checks;cases=$rows;door=$DoorName;initialOpen=$initialOpen;scope='Shared fixed-SLOT82 checkpoint interrupts active native door motion; one load per peer, both-ready role restoration, old door scope retirement and no old target applied to new-world door. Automatic same-map door rearming and a fresh native use are verified, with active native AI after load. Cross-campaign saves and general world authority remain unverified.'}|ConvertTo-Json -Depth 10|Set-Content "$out/result.json"}
  [ordered]@{pid=$PID;operation='NativeDoorSessionReload764';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_NATIVE_DOOR_OBSERVATION $($checks.Count) checks"




