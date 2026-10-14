from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
s=(P/'Observe-NativeHeart-v563.ps1').read_text()
s=s.replace("Take='native-heart-v563'","Take='heart-network-v565'").replace('HeartObserve563','HeartNetwork565')
s=s.replace('$pids=@{};', "$peer=if($Source-eq'host'){'client'}else{'host'};$pids=@{};$peerCapture=$null;")
s=s.replace("'coop_heart_fixture off','coop_movement_fixture off'", "'coop_heart_fixture off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat','coop_movement_fixture off'")
s=s.replace(" C $Source 'setviewpos -33.5 -153 68.1 0 0'", """ C $Source 'setviewpos -33.5 -153 68.1 0 0';C $peer 'setviewpos -31.5 -151 68.1 225 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 12 -Evidence "$out/observer-camera.json"
 $character=if($Source-eq'host'){'corvo'}else{'emily'};$entry=Get-Content "$PSScriptRoot/heart-bank-manifest-v564.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
 $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank hash differs'}
 Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force;C $peer 'coop_movement_bank on'
 $bw=[Diagnostics.Stopwatch]::StartNew();do{$m=S $peer '.movement.json';if($m.active-and$m.version-eq60){break};Start-Sleep -Milliseconds 50}while($bw.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne60){throw 'Heart bank preload failed'}""")
mark=' Start-Sleep -Milliseconds 500;if($capture.HasExited)'
pos=s.index(mark)
s=s[:pos]+''' $peerHwnd=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();$peerArgs=$args.Clone();$peerArgs[7]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$peerArgs[$peerArgs.Length-1]="$out/observer.mp4"
 $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $peerArgs -WindowStyle Hidden -RedirectStandardError "$out/observer-capture.log" -RedirectStandardOutput "$out/observer-capture.stdout" -PassThru
'''+s[pos:]
# Input string is index7? Resolve the actual graph slot by existing array value.
s=s.replace('$peerArgs[7]=', '$graphIndex=[Array]::FindIndex($peerArgs,[Predicate[object]]{param($x) "$x" -like "gfxcapture=*"});if($graphIndex-lt0){throw "Capture graph absent"};$peerArgs[$graphIndex]=')
s=s.replace("C $Source 'coop_heart_fixture 20000';$w=", "C $Source 'coop_heart_fixture 20000';foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};$w=")
s=s.replace("if($step-eq6-and$w.ElapsedMilliseconds-ge12500)","if($step-eq6-and$w.ElapsedMilliseconds-ge12500)")
s=s.replace("fixture=(S $Source '.heart-fixture.json')", "fixture=(S $Source '.heart-fixture.json');movement=(S $peer '.movement.json');stream=(S $peer '.shadow-stream.json');sourceStream=(S $Source '.shadow-stream.json');prop=(S $peer '.heart-prop.json')")
mark=" $checks.Add('Quantity and selected slot rolled back')"
# Insert integration assertions immediately before catch; source cleanup may already be complete.
needle="}catch{$failure=$_.Exception.Message}finally{"
insert=''' foreach($variant in 0,1,2,3){
  $seen=@($rows|Where-Object {$_.movement.heartHeld-eq1-and$_.movement.heartVariant-eq$variant-and$_.movement.heartWeight-gt.8-and$_.movement.weight-gt.9})
  if(!$seen.Count){throw "Remote Heart phase $variant missing"};$checks.Add("Steam remote Heart phase $variant drives authored body")
 }
 if(!@($rows|Where-Object {$_.prop.active-and$_.prop.joint-eq9-and$_.prop.attached-ge1}).Count){throw 'Native render-only Heart prop absent'};$checks.Add('Native render-only Heart attached to owned remote left wrist')
 if(@($rows|Where-Object {$_.prop.refused-gt0}).Count){throw 'Heart prop identity/attachment refusal'}
 if(!@($rows|Where-Object {$_.player.stance-eq1-and$_.movement.heartWeight-gt.8-and$_.movement.weight-gt.9}).Count){throw 'Crouched Heart body absent'};$checks.Add('Existing crouch locomotion composes with native Heart action')
 C $peer 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'};Start-Sleep -Milliseconds 350
 $end=S $peer '.heart-prop.json';if($end.active-or$end.pendingHide){throw 'Heart prop did not retire'};$checks.Add('Stopping stream hides native Heart prop')
'''
s=s.replace(needle,insert+needle)
s=s.replace("C $r 'coop_heart_fixture off';C $r 'coop_movement_fixture off'", "C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_heart_fixture off';C $r 'coop_movement_fixture off'")
s=s.replace('@($capture,$monitor)','@($capture,$peerCapture,$monitor)')
s=s.replace("scope='Native Heart source observation; no remote body/prop binding yet.'", "scope='Native Heart equip/use/holster and selected idle through Steam with existing authored full-body bank60 and native render-only held prop. Crouch uses existing grounded lower-body blend. Heart mechanism heartbeat, targeting dialogue effects and visual polish remain unaccepted.'")
(P/'Test-HeartNetwork-v565.ps1').write_text(s)
# Archive/stage uses exactly the already-tested candidate and assets retain an installer rollback receipt.
s=(W/'stage_heart_v562.py').read_text().replace('candidate-heart-observation-v562','candidate-heart-network-v564').replace('candidate-unbound-presentation-v558','candidate-heart-observation-v562').replace('v562','v564')
s=s.replace("'candidate-heart-observation-v564'", "'candidate-heart-observation-v562'")
s=s.replace("for p in (W/'prepare_heart_v564.py',W/'Build-Heart-v564.ps1',Path(__file__),L/'Send-D2Command.ps1')", "for p in (W/'implement_heart_v564.py',W/'Build-Heart-v564.ps1',W/'implement_heart_prop_v564.py',W/'test_heart_source_v564.py',Path(__file__),L/'Send-D2Command.ps1')")
s=s.replace("protocol=59,bankVersion=59,clips=336", "protocol=60,bankVersion=60,clips=340")
s=s.replace("Adds read-only native Heart identity and isolated20s equipment lease; no production Heart presentation binding yet.", "Native Heart full-body presentation and render-only prop through Steam; exact clocks/ownership. Isolated20s acquisition fixture. Runtime integration pending.")
s=s.replace("A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src')", "A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src');shutil.copytree(R/'work/natural-moveset/heart-banks-v564',A/'banks');shutil.copy2(L/'assets/heart_prop.def',A/'heart_prop.def')")
(W/'stage_heart_v564.py').write_text(s)
s=(W/'Deploy-Heart-v562.ps1').read_text().replace('v562','v564').replace('HeartDeploy562','HeartDeploy564')
mark=' & "$PSScriptRoot/Deploy-SourceClock.ps1"'
s=s.replace(mark,''' $root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$exe=Join-Path $root 'work/asset-game/Dishonored2.exe'
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid -ErrorAction SilentlyContinue;if($p){if($p.Path-ne$exe){throw 'Wrong isolated process'};Stop-Process -Id $p.Id;if(!$p.WaitForExit(15000)){throw 'Process did not stop'}}}
 & "$lab/Build-D2HeartPropAssets.ps1" -Install *> "$PSScriptRoot/heart-assets-v564.log"
 if(!$?){throw 'Isolated Heart asset install failed'}
'''+mark)
(W/'Deploy-Heart-v564.ps1').write_text(s)
print('Heart runtime integration test and isolated asset/deploy prepared.')
