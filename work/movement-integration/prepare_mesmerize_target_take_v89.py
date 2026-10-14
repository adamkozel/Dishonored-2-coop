from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeEyeInterrupt-v88.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$capture=$null;$monitor=$null;','$capture=$null;$monitor=$null;$targetMonitor=$null;',1)
pos=s.index(" $monitor=Start-Process")
s=s[:pos]+''' $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-mesmerize-targets','28000') -WindowStyle Hidden -RedirectStandardOutput "$out/targets.ndjson" -RedirectStandardError "$out/targets.stderr" -PassThru
'''+s[pos:]
needle=" $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')"
assert needle in s
s=s.replace(needle,needle+'''
 $targetMonitor.WaitForExit(5000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){throw 'Target watcher failed'}
 $targetRows=@(Get-Content "$out/targets.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $validTargets=@($targetRows|Where-Object {$_.valid-and$_.worldPresent-and@($_.targets.effects).Count})
 [ordered]@{samples=$targetRows.Count;valid=@($targetRows|Where-Object valid).Count;withEffects=$validTargets.Count;stages=@($targetRows|Group-Object stage|Select-Object Name,Count);targetNames=@($validTargets.targets.definition|Select-Object -Unique);kinds=@($validTargets.targets.effects.kind|Select-Object -Unique);effects=@($validTargets.targets.effects|Select-Object kind,id,owner,name -Unique)}|ConvertTo-Json -Depth 8|Set-Content "$out/target-summary.json"
 if($validTargets.Count-lt10-or!@($validTargets.targets.effects|Where-Object kind -eq 1).Count){throw 'Native target/beam references not proven'}
 $checks.Add('Read-only native target records resolve exact target incarnations, world-owned beam effects and bounded source/destination transforms')
''',1)
s=s.replace(" if($monitor){$monitor.WaitForExit", " if($targetMonitor){$targetMonitor.WaitForExit(40000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){$failure=($failure+'; target watcher failed').TrimStart('; ')}}\n if($monitor){$monitor.WaitForExit",1)
s=s.replace('Source-referenced eye effects are owned by the cosmetic model and retire on interruption.','Source-referenced eye effects are owned by the cosmetic model and retire on interruption. Native target/beam records are inspected read-only; no beam playback is added.')
(P/'Test-MesmerizeTargets-v89.ps1').write_text(s,encoding='utf-8')
print('Target observation and receiver interruption take prepared')
