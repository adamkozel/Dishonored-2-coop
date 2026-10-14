from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=(N/'Observe-NativeDoppelganger-v406.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$capture=$null;$monitor=$null;', '$capture=$null;$monitor=$null;$strictMonitor=$null;')
needle=" $checks.Add('Exact Emily Doppelganger item acquired through native inventory API; no direct spawn, AI or damage invocation')"
assert s.count(needle)==1
s=s.replace(needle,needle+'''
 & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null
 Start-Sleep -Milliseconds 500
 $crouch=Q client '--player-states';$crouch|ConvertTo-Json -Depth 10|Set-Content "$out/crouch-before.json"
 if($crouch.stance-ne1-or!$crouch.motionValid-or$crouch.motion-ne1-or!@($crouch.active|Where-Object {$_.id-eq6-and$_.vtableRva-eq'0x1c84258'}).Count){throw 'Native crouch identity not grounded before observation'}
''')
needle=' $w.Restart();$cast=$false'
assert s.count(needle)==1
s=s.replace(needle,'''
 $strictMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-doppelganger-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 $w.Restart();$cast=$false;$entityNext=5000
''')
s=s.replace('Native Emily LT1800 then release at grounded floor','Native crouched Emily LT1800 then release at grounded floor')
s=s.replace("  $entities=Q client '--entities'", "  $entities=$null;if($w.ElapsedMilliseconds-ge$entityNext){$entityNext=$w.ElapsedMilliseconds+2000;$entities=Q client '--entities'}")
needle=" $checks.Add('Native input produces reciprocal Doppelganger entry/loop/out scenes with observed clocks and state sets')"
assert s.count(needle)==1
s=s.replace(needle,needle+'''
 $strictMonitor.WaitForExit(4000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){throw 'Doppelganger state watcher failed'}
 $strict=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $nativeCrouch=@($strict|Where-Object {$_.playerStatesValid-and$_.stance-eq1-and6-in$_.activeStates-and63-in$_.activeStates-and!@($_.activeStates|Where-Object {$_-notin6,63}).Count})
 [ordered]@{crouchedCastingSamples=$nativeCrouch.Count;standingReaderAccepted=@($strict|Where-Object valid).Count;stateSets=@($strict|Where-Object playerStatesValid|ForEach-Object {($_.activeStates|Sort-Object)-join','}|Sort-Object -Unique);rawSceneSamples=$scenes.Count;scope='Source-only native crouched cast observation. Full-body crouched binding and remote contexts remain pending.'}|ConvertTo-Json -Depth 8|Set-Content "$out/crouch-observations.json"
 if($nativeCrouch.Count-lt3){throw 'Too few native crouch6 and power63 state observations'}
 if(@($strict|Where-Object valid).Count){throw 'Standing-only Doppelganger reader unexpectedly accepts unimplemented crouch'}
 $checks.Add('Native crouch6 plus power63 independently observed with reciprocal In/Loop/Out scenes; current standing-only reader refuses unsupported crouch')
''')
s=s.replace('@($capture,$monitor)', '@($capture,$monitor,$strictMonitor)')
s=s.replace('Native source Emily Doppelganger acquisition, controller input,', 'Native source crouched Emily Doppelganger acquisition, controller input,')
new(N/'Observe-NativeDoppelgangerCrouch-v489.ps1',s)
w=(W/'Run-DoppelgangerObserve-v406.ps1').read_text(encoding='utf-8-sig').replace('native-doppelganger-source-v406','native-doppelganger-crouch-v489').replace('Observe-NativeDoppelganger-v406','Observe-NativeDoppelgangerCrouch-v489')
new(W/'Run-DoppelgangerCrouchObserve-v489.ps1',w)
print('Prepared read-only scene/state observation of native crouched cast, with reversible native input and checkpoint cleanup')
