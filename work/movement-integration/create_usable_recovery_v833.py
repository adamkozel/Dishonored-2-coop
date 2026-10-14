from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-SharedUsable-v832.ps1').read_text().replace('shared-window-family-v832','shared-window-recovery-v833').replace('SharedUsable832','SharedUsable833')
# Keep the same exact native readers and baseline restoration, replace the family batch.
a=s.index(" foreach($r in 'host','client'){\n  $hwnd=")
b=s.index(' # Ordinary native input',a)
s=s[:a]+''' function Capture($prefix,$seconds){foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();$file="$prefix$r"
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$seconds",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$file.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$file-capture.log" -RedirectStandardOutput "$out/$file-capture.stdout" -PassThru))
 }}
 function Rst($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.reset.json" -Raw|ConvertFrom-Json}
 Capture 'menu-' 20
 Start-Sleep -Milliseconds 650
'''+s[b:]
a=s.index(' # Ordinary native input');b=s.index(" foreach($r in 'host','client'){\n  Save \"$out/inventory-after",a)
s=s[:a]+''' # Preserve session/arms while a real native menu is open. The other player
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
 foreach($r in 'host','client'){C $r 'coop_checkpoint_sync on';C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})}
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
 Capture 'fresh-' 10
 I client X 180;Pair $name 1 'fresh-client-open';Ready 'fresh-use'
'''+s[b:]
s=s.replace("I $r;C $r 'coop_usable_sync off'","if((S $r).localGameLogicState-eq2){I $r A 180;Start-Sleep -Milliseconds 300};I $r;C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';C $r 'coop_usable_sync off'")
s=s.replace('Shared native window family: ordinary host open/client close; all four catalog members exercised with alternating source, exact native state convergence and duplicate envelopes; full inventory/health/protected restoration. Gameplay binding limited to the observed Dunwall family, not arbitrary quest devices.','Native menu held over6s with ordinary client use: no receiver mutation until resume, retained four channels. One shared checkpoint each retires old bindings and rearms fresh native window catalog; fresh ordinary client use after load. Baseline restoration and protected hashes. No full campaign or generic quest-device approval.')
(N/'Test-SharedUsableRecovery-v833.ps1').write_text(s)
print('Prepared native state-list menu/checkpoint lifecycle test833')
