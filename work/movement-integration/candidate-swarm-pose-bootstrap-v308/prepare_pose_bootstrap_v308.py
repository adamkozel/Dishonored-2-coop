from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v307.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v307.ps1','Test-SwarmRatVisualNetwork-v308.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v307.ps1').read_text(encoding='utf-8-sig')
(P/'Test-SwarmRatVisualNetwork-v308.ps1').write_text(s)
s=(W/'archive_rat_scale_owner_v307.py').read_text().replace('rat_scale_owner_v307','pose_bootstrap_v308').replace('swarm-rat-scale-owner-v307','swarm-pose-bootstrap-v308').replace('swarm-rat-scale-owner-{kind}-v307','swarm-pose-bootstrap-{kind}-v308').replace('swarm-rat-scale-owner-*-v307','swarm-pose-bootstrap-*-v308').replace('Test-SwarmRatVisualNetwork-v307','Test-SwarmRatVisualNetwork-v308')
start=s.index("scope='")+len("scope='");end=s.index("')\n(A/",start)
s=s[:start]+'Candidate308: before publishing a new root clock scope, collect at least8 distinct samples over250ms real/source time plus100ms near the minimum observed arrival-source offset. Pending calibration publishes no source session, preventing action/world admission. This addresses observed303 startup first-packet bias; steady-state303 mapper/freshness rules remain unchanged, and outages do not restart calibration. Durable16-scope boundary records supplement rejection ring. Cosmetic scale307 passed23 checks; assets/bank/packet schemas unchanged. Native runtime pending. Real Internet clock accuracy, performance, colors/VFX and full shared gameplay remain incomplete.'+s[end:]
s=s.replace("assert read(R/'work/natural-moveset/swarm-rat-clock-v303/result.json')['passed']","assert read(R/'work/natural-moveset/swarm-rat-scale-owner-v307/result.json')['passed']")
(W/'archive_pose_bootstrap_v308.py').write_text(s)
