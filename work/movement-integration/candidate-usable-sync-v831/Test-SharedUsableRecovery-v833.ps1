param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='shared-window-recovery-v833',[ValidateSet('host','client')][string]$Source='host',[ValidateSet('esc_env_electric_cabinet_doors_01','esc_env_safe_door_4','ld_rich_small_01_sndctr_4')][string]$TargetName='ld_rich_small_01_sndctr_4')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$inventory=@{};$binding=@{};$captures=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new();$name=$TargetName
function Save($p,$v){$v|ConvertTo-Json -Depth 18|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null;Start-Sleep -Milliseconds ($ms+30)}
function S($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Read failed $r $mode"}
function Field($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing field $offset"};return $w[0]}
function W($o,$offset){[Convert]::ToUInt64((Field $o $offset).word.Substring(2),16)}
function Signed32($v){[BitConverter]::ToInt32([BitConverter]::GetBytes([uint32]($v-band[uint64]4294967295)),0)}
function U($r){
 $b=$binding[$r];$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState-or$s.localEpoch-ne$b.epoch){throw 'Usable world changed'}
 $o=Q $r '--object' $b.component
 if($o.type-ne'.?AVarkCpntUsableWithStateList@@'-or(Field $o '0x10').word-ne$b.actor-or(Field $o '0x8').word-ne$b.declaration-or((W $o '0x18')-band[uint64]4294967295)-ne$b.generation){throw 'Usable incarnation changed'}
 $state=W $o '0x28';$flags=(W $o '0x30')-band255
 return [ordered]@{tick=[Environment]::TickCount64;current=(Signed32 $state);pending=(Signed32 ($state-shr32));flags=$flags;transition=($flags-band2)-ne0;enabled=((W $o '0x20')-band255);instigator=('0x{0:x}'-f((W $o '0xd0')-band[uint64]4294967295));animation=(Field $o '0xd8').word}
}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='SharedUsable833';state='running';take=$Take}
 $pin=Get-Content "$root/work/movement-integration/candidate-usable-sync-v831/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected831 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  $s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){throw 'Gameplay required'}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r];Save "$out/food-before-$r.json" (Q $r '--player-food')
  $e=@((Q $r '--entities').entities|Where-Object name -eq $name);if($e.Count-ne1){throw 'Unique cabinet required'};$e=$e[0];Save "$out/entity-$r.json" $e
  $cs=Q $r '--components' $e.handle;Save "$out/components-$r.json" $cs;$c=@($cs.components|Where-Object type -eq '.?AVarkCpntUsableWithStateList@@');if($c.Count-ne1-or$c[0].owner-ne$e.pointer){throw 'Usable ownership mismatch'}
  $o=Q $r '--object' $c[0].pointer;Save "$out/component-before-$r.json" $o;$d=Q $r '--object' (Field $o '0x8').word;Save "$out/declaration-$r.json" $d
  if($d.type-ne'.?AVarkDeclCpntUsableWithStateList@@'-or((W $d '0x78')-band0xffffff)-ne2){throw 'Two-state cabinet declaration required'}
  Save "$out/state-entries-$r.json" (Q $r '--object' (Field $d '0x70').word)
  $binding[$r]=@{epoch=$s.localEpoch;handle=$e.handle;actor=$e.pointer;component=$c[0].pointer;declaration=(Field $o '0x8').word;generation=((W $o '0x18')-band[uint64]4294967295)}
  Save "$out/state-before-$r.json" (U $r)
 }
 $dirty=$true
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_usable_sync off','coop_coin_sync off','coop_door_sync off','coop_checkpoint_sync off','coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}}
 foreach($r in 'host','client'){
  C $r 'setviewpos -27.85 -149.123 67.0 0 0';Start-Sleep -Milliseconds 1500
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X -26.5714 -Y -149.123 -Z 68.0 -Evidence "$out/aim-$r.json"
  Save "$out/source-before-input-$r.json" (Q $r '--player')
  C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
 }
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ready=$true;foreach($r in 'host','client'){$iso=Get-Content "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json" -Raw|ConvertFrom-Json;if(!$iso.ready-or$iso.remainingMs-lt20000){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt4000)
 if(!$ready){throw 'Finite native API fixture did not become ready'}
 function Y($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.usable-sync.json" -Raw|ConvertFrom-Json}
 function Ready($tag){
  $w=[Diagnostics.Stopwatch]::StartNew()
  do{$ok=$true;foreach($r in 'host','client'){$y=Y $r;if(!$y.enabled-or!$y.session.enabled-or$y.catalogCount-ne4-or@($y.entries|Where-Object {!$_.enabled-or!$_.ready-or!$_.revision-or$_.applied-ne$_.revision-or$_.inFlight-or$_.awaiting}).Count){$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 70}}while(!$ok-and$w.ElapsedMilliseconds-lt7000)
  foreach($r in 'host','client'){Save "$out/sync-$tag-$r.json" (Y $r)}
  if(!$ok){throw "Shared usable family not ready $tag"}
 }
 function Pair($targetName,$target,$tag){
  $w=[Diagnostics.Stopwatch]::StartNew()
  do{$ok=$true;$pair=@{};foreach($r in 'host','client'){
    $y=Y $r;$entry=@($y.entries|Where-Object name -eq $targetName);$handle=$family[$r][$targetName];$u=Q $r '--usable' $handle
    $pair[$r]=@{native=$u;channel=$entry};$samples.Add(@{role=$r;tag=$tag;state=$u})
    if(!$y.enabled-or$entry.Count-ne1-or!$entry[0].enabled-or!$entry[0].ready-or$entry[0].target-ne$target-or$entry[0].applied-ne$entry[0].revision-or$entry[0].inFlight-or$entry[0].awaiting-or$u.current-ne$target-or$u.pending-ne-1-or$u.transitioning){$ok=$false}
  };if(!$ok){Start-Sleep -Milliseconds 70}}while(!$ok-and$w.ElapsedMilliseconds-lt7000)
  Save "$out/pair-$tag.json" $pair
  if(!$ok){throw "Shared window did not converge $targetName target$target $tag"}
 }
 $family=@{}
 foreach($r in 'host','client'){
  $family[$r]=@{};foreach($e in (Q $r '--entities').entities|Where-Object name -like 'ld_rich_small_01_sndctr_*'){$family[$r][$e.name]=$e.handle}
  Save "$out/family-$r.json" $family[$r];C $r 'coop_usable_sync session'
 }
 Ready 'initial'
 function Capture($prefix,$seconds){foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$file="$prefix$r"
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$seconds",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$file.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$file-capture.log" -RedirectStandardOutput "$out/$file-capture.stdout" -PassThru))
 }}
 function Rst($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.reset.json" -Raw|ConvertFrom-Json}
 Capture 'menu-' 20
 Start-Sleep -Milliseconds 650
 # Preserve session/arms while a real native menu is open. The other player
 # may use their local window, but no indexed receiver call is allowed yet.
 I host Start 180;$w=[Diagnostics.Stopwatch]::StartNew()
 do{$br=S host;if($br.localGameLogicState-eq2){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt4000)
 if($br.localGameLogicState-ne2){throw 'Native pause menu did not open'}
 Start-Sleep -Milliseconds 200;$menu=@{host=(Y host);client=(Y client)};Save "$out/menu-entered.json" $menu
 I client X 180;Start-Sleep -Milliseconds 6300
 foreach($r in 'host','client'){
  $y=Y $r;$native=Q $r '--usable' $family[$r][$name];Save "$out/menu-held-$r.json" @{sync=$y;native=$native;bridge=(S $r)}
  if(!$y.session.enabled-or!$y.enabled-or!$y.menu.suspended){throw 'Usable session was not retained during native menu'}
  if($native.current-ne$(if($r-eq'host'){0}else{1})-or$native.transitioning){throw 'Expected local-only window change during peer menu'}
  foreach($e in $y.entries){$old=@($menu[$r].entries|Where-Object name -eq $e.name)[0];if($e.arm-ne$old.arm-or$e.nativeCalls-ne$old.nativeCalls){throw 'Native call or rearm during held menu'}}
 }
 I host A 180;Ready 'menu-resumed';Pair $name 1 'menu-intent-converged'
 foreach($r in 'host','client'){$y=Y $r;if($y.menu.suspensions-ne1-or$y.menu.resumes-ne1-or$y.menu.pausedMs-lt6000){throw 'Menu retention accounting differs'}}
 foreach($p in $captures){$p.WaitForExit(25000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){throw 'Menu capture incomplete'}}
 $before=@{}
 foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_checkpoint_sync on';C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})}
 $w.Restart();do{$h=Rst host;$c=Rst client;if($h.enabled-and$c.enabled-and$h.peerArm-and$c.peerArm-and!$h.phase-and!$c.phase-and$h.digest-eq$c.digest){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$h.peerArm-or!$c.peerArm-or$h.digest-ne$c.digest){throw 'Checkpoint pair not armed'}
 foreach($r in 'host','client'){$before[$r]=@{reset=(Rst $r);sync=(Y $r);bridge=(S $r);native=(Q $r '--usable' $family[$r][$name])};Save "$out/reload-before-$r.json" $before[$r]}
 Capture 'reload-' 30
 C host 'RestartFromCheckpoint';C client 'RestartFromMemoryCheckpoint'
 $w.Restart();$press=@{host=0;client=0};$reloadSamples=[Collections.Generic.List[object]]::new()
 do{$ready=$true;$pair=@{};foreach($r in 'host','client'){
  $rs=Rst $r;$br=S $r;$y=Y $r;$pair[$r]=@{reset=$rs;bridge=$br;sync=$y}
  if($rs.phase-eq7){throw "Checkpoint recovery failed $r $($rs.result)"}
  if($rs.completed-ne$before[$r].reset.completed+1-or$br.localGameLogicState-ne1-or$br.localPendingGameLogicState-or!$rs.avatarReady-or$rs.avatarPending-or!$y.enabled-or!$y.session.enabled-or$y.session.rearms-ne$before[$r].sync.session.rearms+1){$ready=$false}
  if(!$rs.held-and$w.ElapsedMilliseconds-gt6000-and$br.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-$press[$r]-gt700){I $r A 100;$press[$r]=$w.ElapsedMilliseconds}
 };$reloadSamples.Add(@{ms=$w.ElapsedMilliseconds;pair=$pair});if(!$ready){Start-Sleep -Milliseconds 80}}while(!$ready-and$w.ElapsedMilliseconds-lt50000)
 Save "$out/reload-samples.json" $reloadSamples;if(!$ready){throw 'Usable session did not rebind after shared checkpoint'}
 Ready 'reloaded'
 foreach($r in 'host','client'){
  $rs=Rst $r;$y=Y $r;$old=$before[$r];Save "$out/reload-after-$r.json" @{reset=$rs;sync=$y;character=(Q $r '--character')}
  if($rs.loads-ne$old.reset.loads+1-or$rs.aborted-ne$old.reset.aborted-or$y.session.epoch-eq$old.sync.session.epoch-or$y.session.reloads-ne$old.sync.session.reloads+1-or$y.catalogCount-ne4){throw 'Checkpoint did not retire/rebind usable scope exactly once'}
  foreach($e in $y.entries){$oldEntry=@($old.sync.entries|Where-Object name -eq $e.name)[0];if($e.arm-le$oldEntry.arm-or$e.nativeCalls){throw 'Old usable channel or calls survived checkpoint'}}
  $family[$r]=@{};foreach($e in (Q $r '--entities').entities|Where-Object name -like 'ld_rich_small_01_sndctr_*'){$family[$r][$e.name]=$e.handle}
 }
 Pair $name 0 'fresh-checkpoint-closed'
 foreach($p in $captures){$p.WaitForExit(35000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){throw 'Recovery capture incomplete'}}
 foreach($r in 'host','client'){
  C $r 'setviewpos -27.85 -149.123 67.0 0 0';Start-Sleep -Milliseconds 700
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X -26.5714 -Y -149.123 -Z 68.0 -Evidence "$out/fresh-aim-$r.json"
 }
 $reloadInventory=@{};foreach($r in 'host','client'){$reloadInventory[$r]=Q $r '--player-inventory';Save "$out/inventory-postreload-before-use-$r.json" $reloadInventory[$r]}
 Capture 'fresh-' 10
 I client X 180;Pair $name 1 'fresh-client-open';Ready 'fresh-use'
 foreach($r in 'host','client'){
  Save "$out/inventory-after-$r.json" (Q $r '--player-inventory');Save "$out/food-after-$r.json" (Q $r '--player-food')
  $after=Get-Content "$out/inventory-after-$r.json" -Raw|ConvertFrom-Json
  if(Compare-Object @($reloadInventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Usable experiment altered inventory'}
  if((Q $r '--health' 1).current-ne56){throw 'Native use altered local health'}
 }
 'PASS_SHARED_USABLE_FAMILY'


}catch{$failure=$_.Exception.Message;foreach($r in 'host','client'){try{Save "$out/failure-status-$r.json" (S $r);Save "$out/failure-player-$r.json" (Q $r '--player');Save "$out/failure-state-$r.json" (U $r)}catch{}}}finally{
 if($owned){
  foreach($p in $captures){$p.WaitForExit(65000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
  foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq2){I $r A 180;Start-Sleep -Milliseconds 300};I $r;C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';C $r 'coop_usable_sync off';C $r 'coop_movement_fixture off'}catch{}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline $out -Evidence "$out/note-restoration"
   foreach($r in 'host','client'){$restored=Q $r '--player-inventory';Save "$out/inventory-restored-$r.json" $restored;if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory restore differs'};if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health restore differs'}}
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  Save "$out/samples.json" @($samples);Save "$out/result.json" @{passed=(!$failure);failure=$failure;source=$Source;name=$name;scope='Native menu held over6s with ordinary client use: no receiver mutation until resume, retained four channels. One shared checkpoint each retires old bindings and rearms fresh native window catalog; fresh ordinary client use after load. Baseline restoration and protected hashes. No full campaign or generic quest-device approval.'}
  Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='SharedUsable833';state='finished';take=$Take};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_NATIVE_USABLE'




