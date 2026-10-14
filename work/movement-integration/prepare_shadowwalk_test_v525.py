from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Observe-NativeShadowWalk-v522.ps1').read_text(encoding='utf-8-sig')
def once(a,b):
    global s
    assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
once('$capture=$null;$monitor=$null;', '$capture=$null;$peerCapture=$null;$monitor=$null;')
once("C host 'setviewpos -34.3 -154.8 68.1 90 0'", "C host 'setviewpos -44.39 -153.27 68.1 203 0'\n & \"$PSScriptRoot/Set-ObserverPitch.ps1\" -Role host -Pitch 15 -Evidence \"$out/observer-camera.json\" -Quick")
once(" $hwnd=(Get-Process -Id $pids.client)", ''' Copy-Item "$PSScriptRoot/shadowwalk-banks-v523/emily-shadowwalk-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
 foreach($r in 'client','host'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
 $w.Restart();do{$m=S host '.movement.json';$stream=S host '.shadow-stream.json';if($m.active-and$m.version-eq58-and$m.weight-gt.9-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne58-or$m.weight-lt.9-or!$stream.ready-or!$stream.armed){throw 'Exact bank58 and native Steam stream not ready'}
 $checks.Add('Remote Emily complete-body bank58/335 and native Steam stream ready')
 $initialMovement=$m;$m|ConvertTo-Json -Depth 8|Set-Content "$out/movement-before.json"
 $hwnd=(Get-Process -Id $pids.client)''')
once('"$PSScriptRoot/$Take.mp4")','"$PSScriptRoot/$Take-source.mp4")')
once(" Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Source capture failed'}", ''' $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Observer window missing'}
 $idx=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}));if($idx-lt0){throw 'Capture argument absent'}
 $args[$idx]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$args[-1]="$PSScriptRoot/$Take.mp4"
 $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited-or$peerCapture.HasExited){throw 'Paired capture failed'}''')
once('player=$player;midnight=', "player=$player;movement=(S host '.movement.json');peer=(S host '.shadow-stream.json');source=(S client '.shadow-stream.json');peerSword=(O host '--avatar-sword' (S host).probeActorHandle);midnight=")
# Probe's generic NPC sword mode is independently checked in setup below.
once(" C client 'coop_shadowwalk_fixture off';$w.Restart()", ''' $accepted=@($rows|Where-Object {$_.movement.active-and$_.movement.weight-gt.9-and$_.movement.powerWeight-gt.9})
 foreach($v in 32,33,34){if(!@($accepted|Where-Object {$_.movement.powerVariant-eq$v}).Count){throw "Remote complete-body phase $v not observed at full weight"}}
 if(@($accepted|Where-Object {$_.movement.powerVariant-ge32-and$_.movement.presentedDrawn}).Count){throw 'Shadow form retained a drawn sword'}
 $crawl=@($accepted|Where-Object {$_.movement.powerVariant-eq33})
 $travel=@($crawl.movement.shadowWalkPhase|Sort-Object -Unique)
 if($travel.Count-lt3){throw 'Remote crawl distance phase did not advance'}
 $received=@($rows[-1].peer.powerReceivedHistory.variant|Sort-Object -Unique)
 foreach($v in 32,33,34){if($v-notin$received){throw "Native Steam phase $v not accepted"}}
 if($rows[-1].movement.refused-gt$initialMovement.refused){throw 'Remote body clock rejected a step'}
 $recovery=@($rows|Where-Object {$_.ms-gt10000-and$_.movement.active-and$_.movement.weight-gt.9-and$_.movement.powerWeight-lt.02})
 if(!$recovery.Count){throw 'Remote body did not recover after native exit'}
 [ordered]@{variants=@($accepted.movement.powerVariant|Sort-Object -Unique);received=$received;crawlSamples=$crawl.Count;travelPhases=$travel;clockRefused=$rows[-1].movement.refused;recoverySamples=$recovery.Count;scope='Initial body-only native entry/form/exit. Visual review and native shadow material/particles still required.'}|ConvertTo-Json -Depth 8|Set-Content "$out/remote-body-observations.json"
 $checks.Add('Native Steam entry/form/exit drive remote full-body poses, holstered equipment, distance crawl and recovery without clock rejection')
 C client 'coop_shadowwalk_fixture off';$w.Restart()''')
once("foreach($r in 'host','client'){try{I $r;C $r 'coop_shadowwalk_fixture off'}catch{}}", "foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_shadowwalk_fixture off'}catch{}}")
once('@($capture,$monitor,$strictMonitor)', '@($capture,$peerCapture,$monitor,$strictMonitor)')
once("scope='Source-only native Shadow Walk acquisition, normal LT activation, conditional bounded movement, requested exit, scene/state/equipment observations. No remote form/VFX/gameplay binding. Checkpoint and protected saves must restore.'", "scope='Body-only native Shadow Walk entry, idle, bounded forward/backward crawl and requested exit through native Steam. Original FP preserved. Material, VFX, attacks and untested traversal remain unfinished. Checkpoint and protected saves restored; actual remote/source videos require visual review.'")
assert not (N/'Test-ShadowWalkBody-v525.ps1').exists()
(N/'Test-ShadowWalkBody-v525.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-ShadowWalkObserve-v522.ps1').read_text(encoding='utf-8-sig').replace('native-shadowwalk-scenes-v522','native-shadowwalk-body-v525').replace('Observe-NativeShadowWalk-v522','Test-ShadowWalkBody-v525').replace('ShadowWalkObserve','ShadowWalkBody')
(W/'Run-ShadowWalkBody-v525.ps1').write_text(s,encoding='utf-8')
print('Prepared bounded paired body525 with mandatory checkpoint cleanup')

