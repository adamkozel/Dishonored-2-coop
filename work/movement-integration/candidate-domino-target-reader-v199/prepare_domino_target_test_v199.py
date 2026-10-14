from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeDominoNetwork-v195.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$capture=$null;$monitor=$null;','$capture=$null;$monitor=$null;$targetMonitor=$null;',1)
needle=' $w.Restart();$cast=$false;$networkSnapshot=$false'
assert needle in s
s=s.replace(needle,''' $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-targets','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-targets.ndjson" -RedirectStandardError "$out/domino-targets.stderr" -PassThru
'''+needle,1)
needle=' $checks.Add(\'Strict read-only Domino observer verifies observed native phases'
at=s.index('\n',s.index(needle))
s=s[:at]+'''
 $targetMonitor.WaitForExit(4000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){throw 'Strict native target watcher failed'}
 $targetRows=@(Get-Content "$out/domino-targets.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $markedRows=@($targetRows|Where-Object {$_.valid-and$_.present-and@($_.targets|Where-Object hasMark).Count})
 [ordered]@{samples=$targetRows.Count;valid=@($targetRows|Where-Object valid).Count;marked=$markedRows.Count;maximumReadMs=($targetRows.readMs|Measure-Object -Maximum).Maximum;failures=@($targetRows|Where-Object {!$_.valid}|Group-Object stage|ForEach-Object {[ordered]@{stage=$_.Name;count=$_.Count}});handles=@($markedRows.targets.handle|Sort-Object -Unique)}|ConvertTo-Json -Depth 8|Set-Content "$out/target-reader-summary.json"
 if($markedRows.Count-lt10){throw 'Insufficient strictly owned native Domino target/mark observations'}
 if(@($markedRows|Where-Object {$_.targets.Count-ne1}).Count){throw 'Single-target experiment has an unexpected target count'}
 $checks.Add('Strict reader verifies equipment-derived native network, bounded selected-node pool, reciprocal NPC animation generation and the exact temporary mark owner in at least ten real samples')
'''+s[at:]
s=s.replace('foreach($p in @($capture,$monitor,$strictMonitor))','foreach($p in @($capture,$monitor,$strictMonitor,$targetMonitor))')
# This is a read-only target investigation; it never authorizes receiver FX.
(P/'Observe-NativeDominoTargets-v199.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Observe-NativeDominoNetwork-v195.ps1' in s
p.write_text(s.replace('Observe-NativeDominoNetwork-v195.ps1','Observe-NativeDominoTargets-v199.ps1'),encoding='utf-8')
print('Strict native Domino target observation harness ready')
