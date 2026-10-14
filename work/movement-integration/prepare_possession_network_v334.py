from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Observe-NativePossessionTarget-v333.ps1').read_text()
s=s.replace('$capture=$null;', '$peerCapture=$null;$capture=$null;')
# Move only the native local player, toward a measured live NPC on the same floor.
anchor=" Q host '--components' $target.handle|ConvertTo-Json -Depth 12|Set-Content \"$out/target-components-before.json\""
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $dx=-48.59-[double]$target.position[0];$dy=-155.07-[double]$target.position[1];$length=[Math]::Sqrt($dx*$dx+$dy*$dy)
 if($length-lt.1){throw 'Target too close for deterministic placement'}
 $sourceX=[double]$target.position[0]+3*$dx/$length;$sourceY=[double]$target.position[1]+3*$dy/$length
 $yaw=[Math]::Atan2(-$dy,-$dx)*180/[Math]::PI
 C host ('setviewpos {0} {1} 68.1 {2} 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
 $observerX=$sourceX+4.2;$observerY=$sourceY-1.8;$observerYaw=[Math]::Atan2(-1.8,4.2)*180/[Math]::PI+180
 C client ('setviewpos {0} {1} 68.1 {2} 0' -f $observerX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
 Start-Sleep -Milliseconds 700
 $placed=Q host '--player-states';$placed|ConvertTo-Json -Depth 8|Set-Content "$out/source-target-relative.json"
 if(!$placed.defaultMode-or$placed.stance-ne0-or[Math]::Abs([double]$placed.position[2]-66.4370575)-gt.06){throw 'Target-relative placement is not on verified floor'}
 $pin=Get-Content "$PSScriptRoot/possession-bank-manifest-v334.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo'
 if((Get-FileHash "$PSScriptRoot/possession-banks-v334/corvo-possession-bank.bin").Hash-ne$pin.sha256){throw 'Release bank hash differs'}
 Copy-Item "$PSScriptRoot/possession-banks-v334/corvo-possession-bank.bin" "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';$stream=S client '.shadow-stream.json';if($m.active-and$m.version-eq52-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne52-or$m.weight-lt.9-or!$stream.ready-or!$stream.armed){throw 'Release bank/native Steam stream not ready'}
 $checks.Add('Pinned bank52/316 and protocol49 native Steam stream ready;315 prior clip payloads retained')
''')
s=s.replace('"$PSScriptRoot/$Take.mp4")', '"$PSScriptRoot/$Take-source.mp4")')
anchor=' Start-Sleep -Milliseconds 600;if($capture.HasExited)'
assert s.count(anchor)==1
s=s.replace(anchor,''' $peerHwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$peerHwnd){throw 'Observer window missing'}
 $argIndex=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}));if($argIndex-lt0){throw 'Capture argument absent'}
 $args[$argIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$args[-1]="$PSScriptRoot/$Take.mp4"
 $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
'''+anchor)
s=s.replace('[double]$fresh[0].position[1]+155.07,[double]$fresh[0].position[0]+48.59','[double]$fresh[0].position[1]-$sourceY,[double]$fresh[0].position[0]-$sourceX')
s=s.replace("C host ('setviewpos -48.59 -155.07 68.1 {0} 0' -f $yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))\n   $fresh", "C host ('setviewpos {0} {1} 68.1 {2} 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))\n   $fresh")
s=s.replace('I host 255 500','I host 255 1000').replace('LT500','LT1000')
start=s.index("  $entities=Q host '--entities'");end=s.index('  Start-Sleep -Milliseconds 30',start)
s=s[:start]+'''  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;fixture=(S host '.possession-fixture.json');player=(O host '--player-states');power=(O host '--player-possession');movement=(S client '.movement.json');stream=(S client '.shadow-stream.json');sourceStream=(S host '.shadow-stream.json')})
'''+s[end:]
anchor=" $checks.Add('Strict probe observes entry/aim and successful release400ms with EnterPossessedHost26/internal0 and exact NPC target; runtime transport remains entry/aim only')"
assert s.count(anchor)==1
s=s.replace(anchor,''' $checks.Add('Strict probe observes entry/aim and successful release400ms with EnterPossessedHost26/internal0 and exact NPC target')
 $sent=@($rows.sourceStream.powerSentHistory|Where-Object variant -eq 28|Sort-Object event -Unique)
 $received=@($rows.stream.powerReceivedHistory|Where-Object variant -eq 28|Sort-Object event -Unique)
 $presented=@($rows|Where-Object {$_.movement.powerVariant-eq28-and$_.movement.powerWeight-gt.5-and!$_.movement.presentedDrawn})
 [ordered]@{sent=$sent;received=$received;presented=$presented.Count;strictHandles=@($accepted|Where-Object variant -eq 28|ForEach-Object handle|Sort-Object -Unique)}|ConvertTo-Json -Depth 8|Set-Content "$out/release-network-evidence.json"
 if(!$sent.Count-or!$received.Count-or!$presented.Count){throw 'Native release not sent, accepted and visibly posed with holstered sword'}
 if(Compare-Object @($sent|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"}) @($received|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"})){throw 'Release source/peer event identities differ'}
 if(Compare-Object @($sent.nativeScene|Sort-Object -Unique) @($accepted|Where-Object variant -eq 28|ForEach-Object handle|Sort-Object -Unique)){throw 'Release native scene incarnation differs from sender'}
 $checks.Add('Exact native400ms release scene reaches peer over Steam and drives bank52 full-body release with holstered sword; occupied-host stream retirement remains explicit')
''')
s=s.replace("try{I $r;C $r 'coop_possession_fixture off'}", "try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_possession_fixture off'}")
s=s.replace('@($capture,$monitor,$ownedMonitor,$targetMonitor)', '@($capture,$peerCapture,$monitor,$ownedMonitor,$targetMonitor)')
s=s.replace('remote movement/VFX/gameplay remain unverified.', 'Remote native successful release over protocol49/bank52 investigated. Existing stream deliberately expires during unsupported host occupation; remote body hiding, target control, VFX and shared gameplay remain unimplemented.')
(P/'Test-PossessionReleaseNetwork-v334.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Test-PossessionNetwork-v323.ps1','Test-PossessionReleaseNetwork-v334.ps1');p.write_text(s,encoding='utf-8')
print('Prepared target-relative release networking test with exact scene, native holster and whole inventory/checkpoint assertions')
