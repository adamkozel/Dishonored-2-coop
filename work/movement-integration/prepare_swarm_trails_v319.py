from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';S=R/'outputs/d2-coop-lab/src'
p=S/'swarm_trail_presentation.h';s=p.read_text();s=s.replace('const char* swarmTrailResult="idle";', 'const char* swarmTrailResult="idle";\nconst char* swarmTrailLastRefusal="none";')
for why in ('start_refused','owned_record_guard_refused','readback_unavailable','readback_mismatch'):
 s=s.replace('swarmTrailResult="'+why+'";', 'swarmTrailLastRefusal=swarmTrailResult="'+why+'";')
s=s.replace('\\"result\\":\\"%s\\",\\"refused', '\\"result\\":\\"%s\\",\\"lastRefusal\\":\\"%s\\",\\"refused')
s=s.replace('swarmTrailFinished,swarmTrailResult,swarmTrailRefused','swarmTrailFinished,swarmTrailResult,swarmTrailLastRefusal,swarmTrailRefused');p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert 'Test-SwarmRatVisualNetwork-v318.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v318.ps1','Test-SwarmRatVisualNetwork-v319.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v318.ps1').read_text();needle="($trailsPeer.refused-$trailsPeerBaseline.refused)-ne0-or";assert needle in s
s=s.replace(needle,"($trailsPeer.stopped+$trailsPeer.finished-$trailsPeerBaseline.stopped-$trailsPeerBaseline.finished)-ne7-or\n    "+needle)
(P/'Test-SwarmRatVisualNetwork-v319.ps1').write_text(s)
s=(W/'archive_swarm_trails_v318.py').read_text().replace('v318','v319').replace('Candidate318:', 'Candidate319:318 started7 trails and verified411 transform updates but classified3 end-of-life record checks as refusals. Native record expiry is now explicit completion with persistent refusal diagnostics; tombstones remain unchanged. ')
(W/'archive_swarm_trails_v319.py').write_text(s)
print('Prepared319 native particle completion accounting and retained refusal reasons')
