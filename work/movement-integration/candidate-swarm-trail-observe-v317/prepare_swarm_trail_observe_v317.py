from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert 'Test-SwarmRatVisualNetwork-v315.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v315.ps1','Test-SwarmRatVisualNetwork-v317.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v315.ps1').read_text()
needle=" $visualBaseline=S client '.swarm-rat-visual.json'";assert needle in s
s=s.replace(needle," $fxSourceBaseline=S host '.swarm-spawn-fx.json';$fxPeerBaseline=S client '.swarm-spawn-fx.json'\n [ordered]@{source=$fxSourceBaseline;peer=$fxPeerBaseline}|ConvertTo-Json -Depth 10|Set-Content \"$out/spawn-fx-baseline.json\"\n"+needle)
needle="if($fxSource.sent-ne1-or$fxPeer.applied-ne1-or$fxPeer.received-ne2-or$fxPeer.rejected-ne1-or$fxSource.reads-gt8)";assert needle in s
s=s.replace(needle,"if(($fxSource.sent-$fxSourceBaseline.sent)-ne1-or($fxPeer.applied-$fxPeerBaseline.applied)-ne1-or($fxPeer.received-$fxPeerBaseline.received)-ne2-or($fxPeer.rejected-$fxPeerBaseline.rejected)-ne1-or($fxSource.reads-$fxSourceBaseline.reads)-gt8)")
(P/'Test-SwarmRatVisualNetwork-v317.ps1').write_text(s)
# Historical generator also must generate its own intended PowerShell filter.
p=W/'prepare_swarm_spawn_fx_v314.py';s=p.read_text();s=s.replace('Where-Object kind -eq20','Where-Object { $_.kind -eq 20 }');p.write_text(s)
s=(W/'archive_swarm_trail_observe_v316.py').read_text().replace("A=W/'candidate-swarm-trail-observe-v316'","A=W/'candidate-swarm-trail-observe-v317'").replace('Test-SwarmRatVisualNetwork-v315.ps1','Test-SwarmRatVisualNetwork-v317.ps1').replace('Candidate316:', 'Candidate317:316 native read-only observations linked128 trail samples to strict rats; the full harness failed because it compared cumulative counters against one. Summon assertions now compare increments against a captured pre-cast baseline. ')
s=s.replace("Path(__file__),W/'Invoke", "Path(__file__),W/'prepare_swarm_trail_observe_v317.py',W/'Invoke")
(W/'archive_swarm_trail_observe_v317.py').write_text(s)
print('Prepared317 per-run summon counter deltas; gameplay DLL and probe unchanged')
