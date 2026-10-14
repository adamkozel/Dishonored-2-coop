from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoLinks-v225.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$peerCapture=$null;', '$cancelMonitor=$null;$peerCapture=$null;',1)
s=s.replace("'-t','30'","'-t','40'")
a=s.index(' $stop=[Diagnostics.Stopwatch]::StartNew();C host');b=s.index(' Start-Sleep -Milliseconds 700;',a)
s=s[:a]+''' $cancelBefore=Q client '--domino-links';$cancelBefore|ConvertTo-Json -Depth 10|Set-Content "$out/links-before-cancel.json"
 $cancelMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','8000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-cancel-scenes.ndjson" -RedirectStandardError "$out/native-cancel-scenes.stderr" -PassThru
 I client 255 6000;Start-Sleep -Milliseconds 900
 $cancelState=Q client '--player-states';$cancelState|ConvertTo-Json -Depth 8|Set-Content "$out/native-cancel-state.json"
 if(!@($cancelState.active|Where-Object id -eq72).Count){throw 'Native Domino aim state not active before cancel'}
 & "$lab/Send-D2Input.ps1" -Role client -Button X -LeftTrigger 255 -DurationMs 250|Out-Null
 Start-Sleep -Milliseconds 300;I client
 $stop=[Diagnostics.Stopwatch]::StartNew();$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native X cancel while LT held after two linked enemies; receiver bank remains active'})
'''+s[b:]
s=s.replace("$checks.Add('Receiver beam and mark slots retire within700ms with no resumed update; native effects finish withinfive seconds')", '''$sourceAfter=Q client '--domino-links';$sourceAfter|ConvertTo-Json -Depth 10|Set-Content "$out/links-after-native-cancel.json"
 if(!$sourceAfter.valid-or$sourceAfter.targets-or$sourceAfter.connections.Count){throw 'Native cancellation did not clear selected targets/connections'}
 $cancelMovement=S host '.movement.json';$cancelStream=S host '.shadow-stream.json'
 if(!$cancelMovement.active-or$cancelMovement.weight-lt.9-or!$cancelStream.ready-or!$cancelStream.armed){throw 'Native cancellation retired body stream'}
 $checks.Add('Native X cancellation clears source selected targets/connections while the remote body stream stays active; receiver beam/marks retire without duplicate creation and native particles finish withinfive seconds')''')
key=' if($peerCapture){$peerCapture.WaitForExit(18000)'
assert key in s
s=s.replace(key,''' if($cancelMonitor){$cancelMonitor.WaitForExit(10000)|Out-Null;if(!$cancelMonitor.HasExited-or$cancelMonitor.ExitCode){$failure=($failure+'; cancel watcher failed').TrimStart('; ')}}
'''+key)
s=s.replace('NPC alignment, AI, damage/outcomes and complete-power acceptance remain pending.', 'Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending.')
(P/'Test-DominoNativeCancel-v226.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig')
s=s.replace("'DominoLinksNetwork'","'DominoCancel','DominoLinksNetwork'",2)
key="  'DominoLinksNetwork'{";assert key in s
s=s.replace(key,"  'DominoCancel'{& \"$root/work/natural-moveset/Test-DominoNativeCancel-v226.ps1\" -Take $Take -Pitch $Pitch -AimGuard}\n"+key)
runner.write_text(s,encoding='utf-8')
