from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v301.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v301.ps1','Test-SwarmRatVisualNetwork-v302.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v301.ps1').read_text(encoding='utf-8-sig').replace('existing80ms presentation delay','120ms cosmetic presentation delay').replace('Rat interpolation coverage below85percent after cadence increase','Rat interpolation coverage below85percent with120ms cosmetic buffer')
(P/'Test-SwarmRatVisualNetwork-v302.ps1').write_text(s)
s=(W/'archive_rat_gait_v301.py').read_text().replace('rat_gait_v301','rat_buffer_v302').replace('swarm-rat-gait-v301','swarm-rat-buffer-v302').replace("f'swarm-rat-gait-{kind}-v301.log'","f'swarm-rat-buffer-{kind}-v302.log'").replace("'swarm-rat-gait-*-v301.log'","'swarm-rat-buffer-*-v302.log'").replace('Test-SwarmRatVisualNetwork-v301.ps1','Test-SwarmRatVisualNetwork-v302.ps1')
s=s.replace('Candidate301:','Candidate302:120ms cosmetic-only presentation buffer versus80ms in301. No admission/freshness gate changed.301 proved2388 native input samples and21 moving joints but failed bracketing at84.36percent. ')
(W/'archive_rat_buffer_v302.py').write_text(s)
