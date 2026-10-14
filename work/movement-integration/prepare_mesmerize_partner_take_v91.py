from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeWorldNetwork-v81.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$capture=$null;$monitor=$null;','$capture=$null;$monitor=$null;$targetMonitor=$null;',1)
s=s.replace(" $visualBefore|ConvertTo-Json|Set-Content", " $filterBefore=S client '.mesmerize-partner-filter.json';$filterBefore|ConvertTo-Json|Set-Content \"$out/filter-before.json\"\n $visualBefore|ConvertTo-Json|Set-Content",1)
pos=s.index(' $monitor=Start-Process')
s=s[:pos]+''' $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-mesmerize-targets','28000') -WindowStyle Hidden -RedirectStandardOutput "$out/targets.ndjson" -RedirectStandardError "$out/targets.stderr" -PassThru
'''+s[pos:]
s=s.replace("visual=(S host '.mesmerize-visual.json');effects=$fx", "visual=(S host '.mesmerize-visual.json');filter=(S client '.mesmerize-partner-filter.json');eyes=(S host '.mesmerize-eyes.json');effects=$fx",1)
needle=" $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')"
assert needle in s
s=s.replace(needle,needle+'''
 $targetMonitor.WaitForExit(5000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){throw 'Target watcher failed'}
 $targetRows=@(Get-Content "$out/targets.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $validTargets=@($targetRows|Where-Object {$_.valid-and$_.worldPresent})
 $filter=S client '.mesmerize-partner-filter.json';$filter|ConvertTo-Json|Set-Content "$out/filter-after.json"
 [ordered]@{samples=$targetRows.Count;validPresent=$validTargets.Count;targetSamples=@($validTargets|Where-Object {@($_.targets).Count}).Count;blocked=$filter.blocked-$filterBefore.blocked;considered=$filter.considered-$filterBefore.considered;readFailed=$filter.readFailed-$filterBefore.readFailed;forwarded=$filter.forwarded-$filterBefore.forwarded;otherAccepted=$filter.otherAccepted-$filterBefore.otherAccepted}|ConvertTo-Json|Set-Content "$out/partner-exclusion-summary.json"
 if(!$filter.installed-or($filter.blocked-$filterBefore.blocked)-lt1-or$filter.readFailed-ne$filterBefore.readFailed){throw 'Native local partner eligibility was not cleanly excluded'}
 if($validTargets.Count-lt20-or@($validTargets|Where-Object {@($_.targets).Count}).Count){throw 'Partner remained a native Mesmerize target'}
 if(@($rows.eyes.eyes).Count){throw 'No-target source invented a receiver eye'}
 $checks.Add('Native local Mesmerize eligibility excludes the exact partner before target insertion; valid world snapshots retain zero targets and no remote target eye is invented')
''',1)
s=s.replace('WaitForExit(18000)','WaitForExit(40000)')
s=s.replace(' if($monitor){$monitor.WaitForExit'," if($targetMonitor){$targetMonitor.WaitForExit(40000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){$failure=($failure+'; target watcher failed').TrimStart('; ')}}\n if($monitor){$monitor.WaitForExit",1)
s=s.replace('AI distraction/eye/target beams/gameplay outcome consistency remain pending.','Exact local partner is excluded before native target insertion. Ordinary NPC target preservation requires a separate positive control. Target beams and authoritative NPC outcomes remain pending.')
(P/'Test-MesmerizePartner-v91.ps1').write_text(s,encoding='utf-8')
print('Partner exclusion runtime take prepared')
