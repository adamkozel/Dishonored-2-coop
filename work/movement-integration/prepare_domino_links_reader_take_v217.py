from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeDominoLinks-v214.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$targetMonitor=$null;', '$targetMonitor=$null;$linkMonitor=$null;')
key=' $w.Restart();$cast=$false;'
assert key in s
s=s.replace(key,''' $linkMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-links','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-links.ndjson" -RedirectStandardError "$out/domino-links.stderr" -PassThru
'''+key)
key=" $checks.Add('Native player state, equipment, raw component/entity"
assert key in s
s=s.replace(key,''' $linkMonitor.WaitForExit(4000)|Out-Null;if(!$linkMonitor.HasExited-or$linkMonitor.ExitCode){throw 'Strict native link watcher failed'}
 $linkRows=@(Get-Content "$out/domino-links.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $linked=@($linkRows|Where-Object {$_.valid-and$_.present-and$_.connections.Count-eq2-and@($_.connections|Where-Object hasBeam).Count-eq1})
 [ordered]@{samples=$linkRows.Count;valid=@($linkRows|Where-Object valid).Count;linked=$linked.Count;maximumReadMs=($linkRows.readMs|Measure-Object -Maximum).Maximum;failures=@($linkRows|Where-Object {!$_.valid}|Group-Object stage|ForEach-Object {[ordered]@{stage=$_.Name;count=$_.Count}})}|ConvertTo-Json -Depth 8|Set-Content "$out/link-reader-summary.json"
 if($linked.Count-lt30){throw 'Insufficient strictly owned native Domino links with both validated endpoints'}
 $checks.Add('Strict read-only connection reader verifies two selected reciprocal enemies, directed pool membership, one Emily-owned native beam, both endpoint bases and native opening interpolation in at least thirty real samples')
'''+key)
key=' if($targetMonitor){$targetMonitor.WaitForExit(18000)'
assert key in s
s=s.replace(key,''' if($linkMonitor){$linkMonitor.WaitForExit(18000)|Out-Null;if(!$linkMonitor.HasExited-or$linkMonitor.ExitCode){$failure=($failure+'; link watcher failed').TrimStart('; ')}}
'''+key)
s=s.replace('Raw connection records are not yet trusted for replication.','Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim.')
(P/'Observe-NativeDominoLinks-v217.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1'
s=runner.read_text(encoding='utf-8-sig');assert 'Observe-NativeDominoLinks-v214.ps1' in s
runner.write_text(s.replace('Observe-NativeDominoLinks-v214.ps1','Observe-NativeDominoLinks-v217.ps1'),encoding='utf-8')
