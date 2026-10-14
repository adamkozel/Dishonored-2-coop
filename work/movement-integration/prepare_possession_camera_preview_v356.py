from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionTargetFxView-v355.ps1').read_text()
s=s.replace('$observerFound=$false', '$observerFound=$false;$cameraOptions=[Collections.Generic.List[object]]::new()')
a='$observerFound=$true;$observerX=[double]$observerGround.position[0];$observerY=[double]$observerGround.position[1];break'
assert s.count(a)==1
s=s.replace(a,'''$observerX=[double]$observerGround.position[0];$observerY=[double]$observerGround.position[1]
   $viewYaw=[Math]::Atan2([double]$target.position[1]-$observerY,[double]$target.position[0]-$observerX)*180/[Math]::PI
   C client ('setviewpos {0} {1} 68.1 {2} 0' -f $observerX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$viewYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 0 -Evidence "$out/camera-option-$($cameraOptions.Count)-pitch.json" -Quick
   Start-Sleep -Milliseconds 250
   $cameraState=Q client '--player-states'
   if(!$cameraState.defaultMode-or$cameraState.stance-ne0-or!$cameraState.motionValid-or$cameraState.motion-ne0-or[Math]::Abs([double]$cameraState.position[2]-66.4)-gt.1){continue}
   $viewHwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();$imagePath="$out/camera-option-$($cameraOptions.Count).png"
   & ffmpeg -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${viewHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -update 1 $imagePath
   if($LASTEXITCODE){throw 'Camera preview capture failed'}
   $cameraOptions.Add([pscustomobject]@{index=$cameraOptions.Count;x=[double]$cameraState.position[0];y=[double]$cameraState.position[1];yaw=$viewYaw;image=$imagePath})
   if($cameraOptions.Count-ge3){break}
   continue
''')
a=" $observerAttempts|ConvertTo-Json -Depth 10|Set-Content \"$out/observer-floor-attempts.json\""
assert a in s
s=s.replace(a,a+'''
 if(!$cameraOptions.Count){throw 'No valid floor camera preview'}
 $cameraOptions|ConvertTo-Json -Depth 5|Set-Content "$out/camera-options.json"
 'CAMERA_PREVIEW_READY '+$out
 $pickWait=[Diagnostics.Stopwatch]::StartNew()
 while(!(Test-Path "$out/camera-choice.json")-and$pickWait.ElapsedMilliseconds-lt35000){Start-Sleep -Milliseconds 100}
 if(!(Test-Path "$out/camera-choice.json")){throw 'Camera preview selection timeout'}
 $pick=Get-Content "$out/camera-choice.json" -Raw|ConvertFrom-Json
 if($pick.index-lt0-or$pick.index-ge$cameraOptions.Count){throw 'Invalid camera choice'}
 $choice=$cameraOptions[$pick.index];$observerX=$choice.x;$observerY=$choice.y;$observerFound=$true
''')
(P/'Test-PossessionTargetFxPreview-v356.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Test-PossessionTargetFxView-v355.ps1')==1;s=s.replace('Test-PossessionTargetFxView-v355.ps1','Test-PossessionTargetFxPreview-v356.ps1');p.write_text(s)
print('356 captures three valid camera previews before cast and awaits local agent selection for35 seconds')
