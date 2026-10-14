from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NativeUsable-v828.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$Take='native-cabinet-input-v828'","$Take='native-window-api-v830'").replace("$TargetName='esc_env_electric_cabinet_doors_01'","$TargetName='ld_rich_small_01_sndctr_4'")
s=s.replace('NativeUsable828','NativeUsable830').replace('candidate-coin-sync-v821','candidate-usable-state-v829').replace('Expected821','Expected829')
start=s.index(" $peer=if($Source-eq'host')")
end=s.index(" 'PASS_NATIVE_USABLE_CYCLE'",start)+len(" 'PASS_NATIVE_USABLE_CYCLE'")
s=s[:start]+r''' foreach($r in 'host','client'){
  C $r 'setviewpos -27.85 -149.123 67.0 0 0';Start-Sleep -Milliseconds 1500
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $r -X -26.5714 -Y -149.123 -Z 68.0 -Evidence "$out/aim-$r.json"
  Save "$out/source-before-input-$r.json" (Q $r '--player')
  C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
 }
 $wait=[Diagnostics.Stopwatch]::StartNew();do{$ready=$true;foreach($r in 'host','client'){$iso=Get-Content "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json" -Raw|ConvertFrom-Json;if(!$iso.ready-or$iso.remainingMs-lt20000){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 50}}while(!$ready-and$wait.ElapsedMilliseconds-lt4000)
 if(!$ready){throw 'Finite native API fixture did not become ready'}
 foreach($r in 'host','client'){
  $hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','24','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 900
 function F($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.usable-fixture.json" -Raw|ConvertFrom-Json}
 function Settle($r,$target,$tag){
  $wait=[Diagnostics.Stopwatch]::StartNew()
  do{$u=Q $r '--usable' $binding[$r].handle;$samples.Add(@{role=$r;tag=$tag;state=$u});if($u.component-ne$binding[$r].component-or$u.generation-ne$binding[$r].generation){throw 'Owned usable changed during native experiment'};if($u.current-eq$target-and$u.pending-eq-1-and!$u.transitioning){break};Start-Sleep -Milliseconds 40}while($wait.ElapsedMilliseconds-lt4000)
  Save "$out/state-$tag-$r.json" $u;Save "$out/api-$tag-$r.json" (F $r)
  if($u.current-ne$target-or$u.pending-ne-1-or$u.transitioning){throw "Native indexed state did not settle $r $tag"}
 }
 foreach($r in 'host','client'){
  C $r "coop_usable_fixture $name 1";Settle $r 1 'open';$open=F $r
  if($open.result-ne'native_index_observed'-or$open.calls-ne1){throw 'Native opening call not confirmed'}
  C $r "coop_usable_fixture $name 1";Start-Sleep -Milliseconds 150;$again=F $r;Save "$out/api-noop-$r.json" $again
  if($again.calls-ne$open.calls-or$again.noops-ne1-or$again.result-ne'already_at_target'){throw 'Repeated desired state did not remain a no-op'}
  C $r "coop_usable_fixture $name 0";Settle $r 0 'close'
  if((F $r).calls-ne2){throw 'Native closing call not confirmed'}
  C $r "coop_usable_fixture $name 1";C $r "coop_usable_fixture $name 0";Save "$out/api-reversal-$r.json" (F $r);Settle $r 0 'reversed'
  if((F $r).calls-ne4){throw 'Native interrupted reversal did not record two indexed requests'}
  Save "$out/inventory-after-$r.json" (Q $r '--player-inventory');Save "$out/food-after-$r.json" (Q $r '--player-food')
  $after=Get-Content "$out/inventory-after-$r.json" -Raw|ConvertFrom-Json
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Usable experiment altered inventory'}
  if((Q $r '--health' 1).current-ne56){throw 'Native use altered local health'}
 }
 'PASS_NATIVE_USABLE_INDEX_MATRIX'
''' + s[end:]
s=s.replace('Two ordinary native controller interactions with a two-state cabinet. Exact owned state snapshots and actual native recordings; peer intentionally has no usable replication channel. Baseline/protected restoration. This is local native behavior evidence, not shared interaction certification.', 'Both local roles: native indexed window open/close, repeated-target no-op and opposite request during transition. Exact owned reader, finite API fixture and actual game recordings; full inventory/health/protected restoration. No shared usable network channel yet. Ordinary input reference is828d.')
(N/'Test-NativeUsableApi-v830.ps1').write_text(s,encoding='utf-8')
