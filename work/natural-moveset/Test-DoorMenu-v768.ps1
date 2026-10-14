param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-door-menu-baseline-v768',[ValidateSet('host','client')][string]$PauseRole='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Candidate='candidate-door-session-v763',[switch]$Moving,[switch]$UseDuringMenu)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null
$pids=@{};$inventory=@{};$captures=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$samples=[Collections.Generic.List[object]]::new()
$source=if($PauseRole-eq'host'){'client'}else{'host'};$doorName='ld_door_350cm_int_rich_wood_01_3'
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt12;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 20}};throw "Status unavailable $r$suffix"}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
function CatalogReady($d){$d.enabled-and$d.catalogCount-eq6-and$d.ineligible-eq4-and@($d.entries|Where-Object {!$_.enabled-or!$_.ready-or!$_.revision-or$_.revision-ne$_.applied}).Count-eq0}
function D($r){S $r '.door-sync.json'}
function Selected($d){@($d.entries|Where-Object name -eq $doorName)[0]}
function W($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing object field $offset"};[Convert]::ToUInt64($w[0].word.Substring(2),16)}
function NativeDoor($r){$b=$bindings[$r];$o=Q $r '--object' $b.component;if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$b.owner-or((W $o '0x18')-band [uint64]4294967295)-ne$b.generation){throw 'Native door lifetime changed'};$s=W $o '0x48';[pscustomobject]@{current=[uint32]($s-band [uint64]4294967295);pending=[uint32]($s-shr32);side=[int]((W $o '0xc8')-band255)}}
function Sample($tag){$o=[ordered]@{tag=$tag;at=[DateTimeOffset]::Now.ToString('o')};foreach($r in 'host','client'){$o[$r]=@{bridge=((S $r)|Select-Object localGameLogicState,localPendingGameLogicState,remoteGameLogicState,localEpoch,remoteEpoch,connectionState,remoteFresh,sent,received);doors=(D $r);native=(NativeDoor $r)}};$samples.Add([pscustomobject]$o);return $samples[$samples.Count-1]}
try{
 if($Moving-and$UseDuringMenu){throw 'Choose either in-flight pause or use during menu'}
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='NativeDoorMenu768';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if($dll-ne$pin.dll){throw 'Archived candidate pin mismatch'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated game incarnation'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw 'Gameplay baseline required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-$r.json"
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Living baseline required'}
 }
 $dirty=$true
 foreach($r in 'host','client'){foreach($cmd in 'coop_door_sync off','coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}}
 C $source 'setviewpos -47.05 -163.5 68.1 270 0';C $PauseRole 'setviewpos -45.5 -162 68.1 235 0'
 $bindings=@{}
 foreach($r in 'host','client'){
  $door=@((Q $r '--entities').entities|Where-Object name -eq $doorName);if($door.Count-ne1){throw 'Exact closet missing'}
  $cp=@((Q $r '--components' $door[0].handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@');if($cp.Count-ne1-or$cp[0].owner-ne$door[0].pointer){throw 'Door owner mismatch'}
  $o=Q $r '--object' $cp[0].pointer;$bindings[$r]=@{component=$cp[0].pointer;owner=[Convert]::ToUInt64($door[0].pointer.Substring(2),16);generation=((W $o '0x18')-band [uint64]4294967295)}
  if((NativeDoor $r).current-ne0){throw 'Closed closet required'};C $r 'coop_door_sync session'
 }
 $w=[Diagnostics.Stopwatch]::StartNew();do{$dh=D host;$dc=D client;if((CatalogReady $dh)-and(CatalogReady $dc)){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt6000)
 if(!(CatalogReady $dh)-or!(CatalogReady $dc)){throw 'Six-door session not ready'};$before=@{host=$dh;client=$dc};$before|ConvertTo-Json -Depth 9|Set-Content "$out/catalog-before.json"
 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Native window missing'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','20','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 900
 if($Moving){I $source X 180;$w.Restart();do{$n=NativeDoor $PauseRole;$ds=Selected (D $PauseRole);if($n.current-eq1-and$ds.inFlight){break};Start-Sleep -Milliseconds 20}while($w.ElapsedMilliseconds-lt2500);if($n.current-ne1-or!$ds.inFlight){throw 'Receiver native in-flight opening was not observed'};Sample 'before-menu-moving'|Out-Null}
 I $PauseRole Start 180;$w.Restart();do{$s=S $PauseRole;if($s.localGameLogicState-eq2){break};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt3000);if($s.localGameLogicState-ne2){throw 'Native pause menu not reached'}
 $menu=Sample 'menu-entered';$w.Restart();$menuInput=$false;do{if($UseDuringMenu-and!$menuInput-and$w.ElapsedMilliseconds-ge1000){I $source X 180;$menuInput=$true};Sample 'menu-held'|Out-Null;Start-Sleep -Milliseconds 200}while($w.ElapsedMilliseconds-lt6200)
 $held=Sample 'menu-end';$held|ConvertTo-Json -Depth 12|Set-Content "$out/menu-end.json"
 if($UseDuringMenu-and($held.$source.native.current-ne2-or$held.$PauseRole.native.current-ne0)){throw 'Expected local native use during the other player menu without receiver mutation'}
 I $PauseRole A 180;$w.Restart();do{$s=S $PauseRole;if($s.localGameLogicState-eq1-and!$s.localPendingGameLogicState){break};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt3000)
 if($s.localGameLogicState-ne1){throw 'Native menu resume failed'}
 $w.Restart();do{$after=Sample 'resuming';if((CatalogReady $after.host.doors)-and(CatalogReady $after.client.doors)){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt6000)
 $after|ConvertTo-Json -Depth 12|Set-Content "$out/after-resume.json"
 foreach($r in 'host','client'){
  if(!$held.$r.doors.session.enabled-or!$held.$r.doors.enabled-or!(CatalogReady $after.$r.doors)){throw "Persistent door catalog retired or did not resume for $r"}
  if($after.$r.doors.menu-and($after.$r.doors.menu.resumes-ne1-or$after.$r.doors.menu.suspensions-ne1-or$after.$r.doors.menu.pausedMs-lt6000-or$after.$r.doors.menu.suspended)){throw 'Native menu suspension/resumption accounting differs'}
  if($held.$r.bridge.localEpoch-ne$after.$r.bridge.localEpoch-or$held.$r.bridge.remoteEpoch-ne$after.$r.bridge.remoteEpoch){throw 'Menu changed native world epoch'}
  foreach($old in $before[$r].entries){$new=@($after.$r.doors.entries|Where-Object name -eq $old.name)[0];if($new.arm-ne$old.arm-or!$new.enabled){throw 'Menu replaced or retired native door binding'}}
  $during=Selected $held.$r.doors;$entry=Selected $menu.$r.doors;if($during.nativeCalls-ne$entry.nativeCalls){throw 'Door queue mutated while native menu remained open'}
 }
 $checks.Add('Native pause held over six seconds; original six channels and world epochs retained; no door API calls during held menu; session resumed automatically')
 if(!$Moving-and!$UseDuringMenu){I $source X 180}
 $w.Restart();do{$after=Sample 'post-resume-opening';$h=Selected $after.host.doors;$c=Selected $after.client.doors;if($h.target-eq2-and$h.applied-eq$h.revision-and$c.applied-eq$h.revision-and$after.host.native.current-eq2-and$after.client.native.current-eq2){break};Start-Sleep -Milliseconds 45}while($w.ElapsedMilliseconds-lt6000)
 if($h.target-ne2-or$c.target-ne2-or$h.applied-ne$h.revision-or$c.applied-ne$h.revision-or$after.host.native.current-ne2-or$after.client.native.current-ne2-or$after.host.native.side-ne$h.side-or$after.client.native.side-ne$c.side){throw 'Native opening failed after menu'}
 if(($h.nativeCalls+$c.nativeCalls)-ne1-or$h.revision-ne2){throw 'Expected one replicated native opening without replay'}
 $after|ConvertTo-Json -Depth 12|Set-Content "$out/native-final.json";$checks.Add('One native source interaction converged to both real open states and matching side with one receiver API call, without manual rearm')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed during menu observation'}}
}catch{$failure=$_.Exception.Message}finally{
 if($owned){
  foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq2){I $r A 180;Start-Sleep -Milliseconds 600};I $r}catch{}}
  foreach($cap in $captures){$cap.WaitForExit(25000)|Out-Null;if(!$cap.HasExited-or$cap.ExitCode){$failure=($failure+'; native capture incomplete').TrimStart('; ')}}
  $samples|ConvertTo-Json -Depth 13|Set-Content "$out/observations.json"
  foreach($r in 'host','client'){try{C $r 'coop_door_sync off';C $r 'coop_movement_fixture off'}catch{}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
   foreach($r in 'host','client'){$restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-restored-$r.json";if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete inventory differs'};if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$checks.Add('Complete inventories and native player/counterpart health restored')
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path -LiteralPath $out){[ordered]@{passed=(!$failure);failure=$failure;dll=$dll;candidate=$Candidate;pauseRole=$PauseRole;source=$source;moving=[bool]$Moving;useDuringMenu=[bool]$UseDuringMenu;checks=$checks;scope='Native pause menu held over six seconds in the same world, idle, receiver opening in flight, or other-player use during menu; retain catalog and complete one door use after resume. Bounded AI isolation for fixture safety, not shared enemy authority.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"}
  [ordered]@{pid=$PID;operation='NativeDoorMenu768';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json";$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_NATIVE_DOOR_MENU $($checks.Count) checks"
