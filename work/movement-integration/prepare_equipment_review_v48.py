"""Reuse native equipment assertions with the latest bank and actual-game video."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-EquipmentVisibility-v26.ps1').read_text()
s=s.replace("'equipment-visibility-v26'","'equipment-review-v48'").replace('carry-pickup-contact-bank-manifest-v5.json','outofmana-context-bank-manifest-v46.json').replace('version-ne34','version-ne41')
s=s.replace("$failure=$null;$pids=@{};", "$failure=$null;$capture=$null;$captures=@();$chapters=[Collections.Generic.List[object]]::new();$pids=@{};")
a="  Start-Sleep -Milliseconds 600;C $dst 'coop_movement_bank on';Start-Sleep -Milliseconds 1200"
b="""  C $src 'setviewpos -33.5 -153 68.1 90 0';C $dst 'setviewpos -33.5 -150 68.1 270 0'
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $dst -Pitch 12 -Evidence "$out/camera-$src.json"
  Start-Sleep -Milliseconds 600;C $dst 'coop_movement_bank on';Start-Sleep -Milliseconds 1200"""
assert a in s;s=s.replace(a,b)
a="  foreach($stance in 0,1){"
b="""  $window=(Get-Process -Id $pids[$dst]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','48','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-$src.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-$src.log" -RedirectStandardOutput "$out/capture-$src.stdout" -PassThru;$captures+=$capture
  Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Capture failed'};$overall=[Diagnostics.Stopwatch]::StartNew()
  foreach($stance in 0,1){"""
assert a in s;s=s.replace(a,b)
s=s.replace("    I $src X $(if($held){650}else{1500});", "    $chapters.Add([pscustomobject]@{source=$src;stance=$stance;action=$trial;videoTime=(.7+$overall.Elapsed.TotalSeconds)})\n    I $src X $(if($held){650}else{1500});")
s=s.replace("C $src 'setviewpos -33.5 -153 68.1 0 0';Start-Sleep", "C $src 'setviewpos -33.5 -153 68.1 90 0';Start-Sleep")
s=s.replace(" $checks.Add(\"${src}: original visible drawn sword restored, no visibility refusal or lost lease; health unchanged\")", " $checks.Add(\"${src}: original visible drawn sword restored, no visibility refusal or lost lease; health unchanged\")\n  $capture.WaitForExit(20000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){throw 'Capture did not finish'}")
s=s.replace("}catch{$failure=$_.Exception.Message}finally{", """ foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $checks.Add('All fourteen protected settings/save hashes unchanged')
}catch{$failure=$_.Exception.Message}finally{""")
s=s.replace(' $rows|ConvertTo-Json -Depth 13', ' foreach($rec in $captures){if(!$rec.HasExited){$rec.WaitForExit(50000)|Out-Null}}\n $rows|ConvertTo-Json -Depth 13')
s=s.replace("failure=$failure;checks=$checks;dll=", "failure=$failure;checks=$checks;chapters=$chapters;bankManifest='outofmana-context-bank-manifest-v46.json';dll=")
out=P/'Test-EquipmentReview-v48.ps1';assert not out.exists();out.write_text(s);print(out)
