"""Keep the native no-mana test, add actual remote body and equipment checks."""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Observe-BendTimeNoMana.ps1').read_text();target=P/'Test-OutOfMana.ps1';assert not target.exists()
def swap(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
swap("[switch]$Crouched,[switch]$Holstered,",'')
swap("$ErrorActionPreference='Stop';","$Crouched=$false;$Holstered=$false;$captures=@();$trace=$null;$ErrorActionPreference='Stop';")
swap("C host 'setviewpos -33.5 -148 68.1 90 0';C client 'setviewpos -28 -151 68.1 160 0'","C host 'setviewpos -33.5 -153 68.1 90 0';C client 'setviewpos -30.7 -151.8 68.1 203 0'\n & \"$PSScriptRoot/Set-ObserverPitch.ps1\" -Role client -Pitch 15 -Evidence \"$out/observer-camera.json\"")
a=" $window=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$window){throw 'No source game window'}"
swap(a,''' $entry=Get-Content "$PSScriptRoot/outofmana-bank-manifest-v45.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo';$bank=Join-Path $PSScriptRoot $entry.bank
 if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank identity mismatch'}
 Copy-Item $bank "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';if($m.active-and$m.version-eq40-and$m.weight-gt.9-and$m.presentedDrawn){break};Start-Sleep -Milliseconds 35}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne40-or$m.weight-lt.9-or!$m.presentedDrawn){throw 'Native v40 armed body not ready'}
 $checks.Add('Exact v40 bank active on the native Steam remote avatar')
 $avatar=(S client).probeActorHandle;$fxBefore=S client '.power-fx.json'
 $trace=Start-Process -FilePath "$PSHOME/pwsh.exe" -ArgumentList @('-NoProfile','-File',('"'+"$PSScriptRoot/Trace-BendTimePresentation.ps1"+'"'),'-Take',$Take,'-DurationMs','12500') -WindowStyle Hidden -RedirectStandardOutput "$out/trace.stdout" -RedirectStandardError "$out/trace.stderr" -PassThru
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
 foreach($r in 'host','client'){
 $window=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$window){throw 'No game window'};$suffix=if($r-eq'host'){'-source'}else{''}''')
swap('"$PSScriptRoot/$Take.mp4")','"$PSScriptRoot/$Take$suffix.mp4")')
swap('$start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process','$capture=Start-Process')
swap('-RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru','-RedirectStandardError "$out/capture$suffix.log" -RedirectStandardOutput "$out/capture$suffix.stdout" -PassThru\n $captures+=$capture\n }')
swap('power=(Q host \'--player-bendtime\')})','power=(Q host \'--player-bendtime\');movement=(S client \'.movement.json\');stream=(S client \'.shadow-stream.json\')})')
swap(" I host;C host 'coop_bendtime_fixture off';",''' $trace.WaitForExit(4000)|Out-Null;if(!$trace.HasExited-or$trace.ExitCode){throw 'Presentation recorder did not finish'}
 $tr=Get-Content "$out/presentation-trace.json" -Raw|ConvertFrom-Json
 $nativeFailures=@($rows|Where-Object {$_.player.powerSceneValid-and$_.player.powerScene.variant-eq15}|ForEach-Object {$_.player.powerScene.handle}|Select-Object -Unique)
 $remoteFailures=@($tr|Where-Object {$_.active-and$_.ready-and$_.variant-eq15-and$_.weight-gt.5}|ForEach-Object {$_.event}|Select-Object -Unique)
 if($nativeFailures.Count-ne2-or$remoteFailures.Count-ne2){throw "Expected two owned native/remote failures, observed $($nativeFailures.Count)/$($remoteFailures.Count)"}
 $checks.Add('Two distinct owned native failure scenes each drive one remote body event over Steam')
 $m=S client '.movement.json';$stream=S client '.shadow-stream.json';if(!$m.active-or$m.weight-lt.9-or$m.powerWeight-gt.02-or!$stream.ready){throw 'Failure did not recover to normal native standing'}
 $sword=Q client '--avatar-sword' $avatar;$sword|ConvertTo-Json -Depth 6|Set-Content "$out/sword-after.json"
 if(!$sword.equipped-or!(Q host '--player-sword').equipped){throw 'Failed cast changed sword state'}
 if((Q client '--health' $avatar).current-ne40){throw 'Failed cast damaged remote body'}
 $fxAfter=S client '.power-fx.json';if($fxAfter.started-ne$fxBefore.started-or$fxAfter.active){throw 'Failure created unsolicited observer effect'}
 $checks.Add('Remote failure recovers to standing with original sword, health and no observer screen effect')
 I host;C host 'coop_bendtime_fixture off';''')
swap("foreach($r in 'host','client'){try{I $r;C $r 'coop_bendtime_fixture off';", "foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_bendtime_fixture off';")
swap(" if($capture){$capture.WaitForExit(16000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Native capture incomplete'}}", " foreach($capture in $captures){$capture.WaitForExit(16000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Native capture incomplete'}}")
swap("capture='Actual native source POV and read-only state/scene/entity observations'", "bankSha256=(Get-FileHash $bank).Hash;capture='Paired actual D2 source and remote body videos with native scene, Steam and presentation evidence'")
swap('Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified.', 'Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged.')
target.write_text(s);print(target)
