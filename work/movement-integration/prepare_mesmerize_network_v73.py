from pathlib import Path
import json,hashlib,shutil
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';I=R/'work/movement-integration'
s=(P/'Observe-NativeMesmerize-v72.ps1').read_text()
s=s.replace('$capture=$null;', '$recorders=@();$capture=$null;')
s=s.replace("C client 'setviewpos -33.5 -153 68.1 270 0'", "C client 'setviewpos -33.5 -153 68.1 90 0';C host 'setviewpos -30.7 -151.8 68.1 203 0'\n & \"$PSScriptRoot/Set-ObserverPitch.ps1\" -Role host -Pitch 15 -Evidence \"$out/observer-camera.json\" -Quick")
mark=" $checks.Add('Exact native Emily Mesmerize item acquired and selected without directly invoking power or VFX')"
assert mark in s
s=s.replace(mark,mark+'''
 Copy-Item "$PSScriptRoot/mesmerize-banks-v73/emily-mesmerize-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
 $w.Restart();do{$m=S host '.movement.json';$stream=S host '.shadow-stream.json';if($m.active-and$m.version-eq45-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne45-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Mesmerize bank and native Steam stream not ready'}
 $checks.Add('Native Steam transport and exact bank45/276 remote Emily standing armed context ready')
''')
s=s.replace('"$PSScriptRoot/$Take.mp4"','"$PSScriptRoot/$Take-source.mp4"')
mark=" Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Capture failed'}"
s=s.replace(mark,''' $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $args[$args.IndexOf($args|Where-Object {$_ -like 'gfxcapture=*'})]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"
 $args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Capture failed'}''')
before='''  $scene=Q client '--midnight';$entities=Q client '--entities'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');midnight=$scene;power=(Q client '--player-mesmerize');component=(Q client '--object' $power.component);effects=(OptionalFx client);entities=@($entities.entities|Where-Object {$_.definitionName-match'mesmerize'})})'''
after='''  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $fx=if(($rows.Count%4)-eq0){OptionalFx client}else{$null}
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');movement=$m;stream=$stream;effects=$fx})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw "Native transport/presentation retired: $($stream.result)"}'''
assert before in s;s=s.replace(before,after)
mark=" $checks.Add('Strict scene reader validates all three native phases with reciprocal owned Emily left arm; default production path remains disabled')"
assert mark in s
s=s.replace(mark,""" $checks.Add('Strict scene reader validates all three native phases with reciprocal owned Emily left arm')
 foreach($variant in 16,17,18){if(!@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}).Count){throw "Remote phase $variant not observed"}}
 $eventsSeen=@($rows|Where-Object {$_.movement.powerVariant-in16,17,18-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique)
 if($eventsSeen.Count-ne3){throw "Expected three distinct remote phase events, observed $($eventsSeen.Count)"}
 $checks.Add('All three source phase incarnations drive remote full-body Mesmerize through native Steam')
 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}
 $sword=Q host '--avatar-sword' (S host).probeActorHandle;$sword|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if(!$sword.equipped-or!(Q client '--player-sword').equipped){throw 'Native sword selection changed'}
 foreach($r in 'host','client'){if((Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Remote avatar health changed'}}
 if(@((Q host '--mesmerize-effects').effects).Count){throw 'Source-only world effect leaked into observer world'}
 $checks.Add('Remote body recovers, original sword selections and all avatar health retained; no duplicated observer world effect')""")
s=s.replace("I $r;C $r 'coop_mesmerize_fixture off';C $r 'coop_movement_fixture off'","I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_mesmerize_fixture off';C $r 'coop_movement_fixture off'")
s=s.replace(" if($capture){$capture.WaitForExit(18000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}"," foreach($capture in $recorders){$capture.WaitForExit(18000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}")
s=s.replace('No remote body, VFX, AI or gameplay outcome replication.', 'Standing armed Emily body phases replicate through native Steam. Remote world VFX, AI and gameplay outcomes remain pending; source-only world effects intentionally retained.')
s=s.replace("probe=$initialProbe;scope=", "probe=$initialProbe;bankSha256=(Get-FileHash \"$PSScriptRoot/mesmerize-banks-v73/emily-mesmerize-bank.bin\").Hash;scope=")
(P/'Test-MesmerizeNetwork-v73.ps1').write_text(s)
p=I/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace("'MesmerizeObserve','GrenadeProp'","'MesmerizeObserve','MesmerizeNetwork','GrenadeProp'")
s=s.replace("  'MesmerizeObserve'{", "  'MesmerizeNetwork'{& \"$root/work/natural-moveset/Test-MesmerizeNetwork-v73.ps1\" -Take $Take -Pitch $Pitch}\n  'MesmerizeObserve'{")
p.write_text(s)
A=I/'candidate-mesmerize-network-v73';A.mkdir(exist_ok=False);shutil.copytree(R/'outputs/d2-coop-lab/src',A/'src')
for n in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/n,A/n)
for n in ('Test-MesmerizeNetwork-v73.ps1','mesmerize-bank-manifest-v73.json','build_mesmerize_bank_v73.py'):shutil.copy2(P/n,A/n)
shutil.copy2(p,A/p.name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
(A/'identity.json').write_text(json.dumps(dict(dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),protocol=40,bankVersion=45,clips=276,runtimeVerified=False,scope='Standing armed Emily Mesmerize body phase integration candidate; native scene reader v72 runtime verified. No remote world VFX or AI. Unit/protocol/bank/smoke logs v73. Runtime network verification pending.'),indent=2))
for n in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/n,R/'outputs/d2-coop-lab/bin'/n)
print('Mesmerize network candidate preserved; deployment still required')
