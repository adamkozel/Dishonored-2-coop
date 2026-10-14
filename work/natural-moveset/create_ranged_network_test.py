"""One-time derivative of the verified native action observation harness."""
from pathlib import Path
P=Path(__file__).resolve().parent
t=(P/'Observe-RangedActions.ps1').read_text()
t=t.replace("'native-ranged-corvo-gun-actions-v1'","'ranged-corvo-gun-network-v29-v1'")
t=t.replace("$capture=$null;$start=0","$capture=$null;$sourceCapture=$null;$start=0")
t=t.replace("'coop_movement_bank off','coop_shadow_stream off'","'coop_movement_bank off','coop_shadow_stream off','coop_ranged_prop off','coop_review_clip off'")
t=t.replace("C $Source 'setviewpos -33 -153 68.1 180 0';C $peer 'setviewpos -31 -148 68.1 180 0'","C $Source 'setviewpos -33.5 -153 68.1 90 0';C $peer 'setviewpos -30.7 -151.8 68.1 203 0'")
before=" $window=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$window){throw 'No source game window'}"
after=""" & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 12 -Evidence "$out/observer-camera.json"
 $character=if($Source-eq'host'){'corvo'}else{'emily'}
 Copy-Item "$PSScriptRoot/ranged-banks-v1/$character-ranged-bank.bin" "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $peer 'coop_movement_bank on'
 $w.Restart();do{$m=S $peer '.movement.json';$prop=S $peer '.ranged-prop.json';if($m.active-and$m.version-eq29-and$m.rangedWeight-gt.9-and$prop.active){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne29-or$m.rangedWeight-lt.9-or!$prop.active){[ordered]@{movement=$m;prop=$prop;stream=(S $peer '.shadow-stream.json');sourceStream=(S $Source '.shadow-stream.json');player=(Q $Source '--player-states')}|ConvertTo-Json -Depth 10|Set-Content "$out/ready-failure.json";throw 'Ranged network body/prop did not become ready'}
 $checks.Add('Native Steam v25 scene stream, v29 body bank and automatic cosmetic weapon attach ready')
 $prop|ConvertTo-Json|Set-Content "$out/prop-initial.json"
 $window=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();if(!$window){throw 'No peer game window'}"""
assert before in t;t=t.replace(before,after)
before=' Start-Sleep -Milliseconds 700;if($capture.HasExited)'
after=''' $sourceWindow=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64()
 $sourceArgs=@($args);$sourceArgs[8]="gfxcapture=hwnd=${sourceWindow}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"
 # Replace by its observed value, not by a guessed positional argument index.
 $sourceArgs=@($args|ForEach-Object {if($_-like 'gfxcapture=*'){"gfxcapture=hwnd=${sourceWindow}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"}elseif($_-eq"$PSScriptRoot/$Take.mp4"){"$PSScriptRoot/$Take-source.mp4"}else{$_}})
 $sourceCapture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $sourceArgs -WindowStyle Hidden -RedirectStandardError "$out/capture-source.log" -RedirectStandardOutput "$out/capture-source.stdout" -PassThru
 Start-Sleep -Milliseconds 700;if($capture.HasExited)'''
assert before in t;t=t.replace(before,after)
t=t.replace('component=(Q $Source \'--object\' $power.component)',"movement=(S $peer '.movement.json');prop=(S $peer '.ranged-prop.json');stream=(S $peer '.shadow-stream.json');sourceStream=(S $Source '.shadow-stream.json')")
before=" $checks.Add('Owned native scene reader validates fire, reload, empty, equip and unequip identities and clocks');"
after=""" $remoteVariants=@($rows|Where-Object {$_.movement.rangedWeight-gt.1}|ForEach-Object {$_.movement.rangedVariant}|Sort-Object -Unique)
 foreach($variant in $expectedVariants){if($variant-notin$remoteVariants){throw "Remote body missed ranged variant $variant"}}
 if(@($rows|Where-Object {$_.movement.refused-or$_.prop.refused-or!$_.movement.active}).Count){throw 'Remote pose or prop refused during playback'}
 $checks.Add('All five source-owned ranged phases reached the remote body through Steam')
 $checks.Add('Owned native scene reader validates fire, reload, empty, equip and unequip identities and clocks');"""
assert before in t;t=t.replace(before,after)
t=t.replace("I $Source;C $Source 'coop_ranged_fixture off'","I $Source;C $peer 'coop_movement_bank off';C $peer 'coop_ranged_prop off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'};C $Source 'coop_ranged_fixture off'")
t=t.replace("try{I $r;C $r 'coop_ranged_fixture off'","try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_ranged_prop off';C $r 'coop_ranged_fixture off'")
t=t.replace(" $rows|ConvertTo-Json -Depth 18", " if($sourceCapture){$sourceCapture.WaitForExit(16000)|Out-Null;if(!$sourceCapture.HasExited-or$sourceCapture.ExitCode-ne0){$failure='Source capture incomplete'}}\n $rows|ConvertTo-Json -Depth 18")
t=t.replace("capture='Actual native source POV and read-only state/scene/entity observations'","capture='Actual peer body and native source POV; source native input drives Steam presentation'")
t=t.replace("Two-round native ranged weapon/action investigation only; no remote body/effect integration claimed.","Two-round native ranged body/hand attachment integration candidate. Visual acceptance separate; remote projectiles, muzzle effects, sounds, upgrades and nonstanding contexts not yet integrated.")
t=t.replace('OBSERVED_NATIVE_RANGED','RANGED_NETWORK_VERIFIED')
(P/'Test-RangedNetwork.ps1').write_text(t)
print('Created Test-RangedNetwork.ps1')
