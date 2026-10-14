from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeMoving-v83.ps1').read_text(encoding='utf-8-sig').replace('[int]$Move=9000','[int]$Move=12000',1)
s=s.replace('$capture=$null;$monitor=$null;','$capture=$null;$monitor=$null;$eyeMonitor=$null;',1)
needle=' $w.Restart();$cast=$false;'
i=s.index(needle)
s=s[:i]+''' $eyeMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-mesmerize-eyes','28000') -WindowStyle Hidden -RedirectStandardOutput "$out/eyes.ndjson" -RedirectStandardError "$out/eyes.stderr" -PassThru
'''+s[i:]
needle=" $checks.Add('Native forward walking overlaps all-body cast presentation, with measured displacement and standing recovery')"
s=s.replace(needle,needle+'''
 $eyeMonitor.WaitForExit(4000)|Out-Null;if(!$eyeMonitor.HasExited-or$eyeMonitor.ExitCode){throw 'Native eye watcher failed'}
 $eyeSamples=@(Get-Content "$out/eyes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $validEyes=@($eyeSamples|Where-Object {$_.valid-and$_.eyes.Count-gt0})
 [ordered]@{total=$eyeSamples.Count;valid=@($eyeSamples|Where-Object valid).Count;withEyes=$validEyes.Count;invalidStages=@($eyeSamples|Where-Object {!$_.valid}|Group-Object stage|ForEach-Object {[ordered]@{stage=$_.Name;count=$_.Count}});ids=@($validEyes.eyes.id|Select-Object -Unique)}|ConvertTo-Json -Depth 8|Set-Content "$out/eyes-summary.json"
 if($validEyes.Count-lt10){throw 'Referenced native eye ownership not established'}
 $checks.Add('Read-only eye references resolve exact committed owner/effect IDs and finite native transforms; no eye playback yet')
''',1)
s=s.replace(' $rows|ConvertTo-Json -Depth 20'," if($eyeMonitor){$eyeMonitor.WaitForExit(18000)|Out-Null;if(!$eyeMonitor.HasExited-or$eyeMonitor.ExitCode){$failure=($failure+'; eye watcher failed').TrimStart('; ')}}\n $rows|ConvertTo-Json -Depth 20",1)
s=s.replace('AI distraction/target beams/gameplay outcome consistency remain pending.','Source eye ownership is inspected read-only. Remote eye/target beams and AI/gameplay outcome consistency remain pending.')
(P/'Test-MesmerizeEyesMoving-v84.ps1').write_text(s,encoding='utf-8')
