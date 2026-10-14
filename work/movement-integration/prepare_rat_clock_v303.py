from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v302.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v302.ps1','Test-SwarmRatVisualNetwork-v303.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v302.ps1').read_text(encoding='utf-8-sig')
needle='}catch{$failure=$_.Exception.Message}finally{';assert needle in s
s=s.replace(needle,needle+'''
 foreach($r in 'host','client'){try{S $r '.pose-clock.json'|ConvertTo-Json -Depth 9|Set-Content "$out/clock-before-cleanup-$r.json"}catch{}}
 $worldRows|ConvertTo-Json -Depth 16|Set-Content "$out/world-states.json"
''')
(P/'Test-SwarmRatVisualNetwork-v303.ps1').write_text(s)
s=(W/'archive_rat_gait_v301.py').read_text().replace('rat_gait_v301','rat_clock_v303').replace('swarm-rat-gait-v301','swarm-rat-clock-v303').replace("f'swarm-rat-gait-{kind}-v301.log'","f'swarm-rat-clock-{kind}-v303.log'").replace("'swarm-rat-gait-*-v301.log'","'swarm-rat-clock-*-v303.log'").replace('Test-SwarmRatVisualNetwork-v301.ps1','Test-SwarmRatVisualNetwork-v303.ps1')
start=s.index("scope='")+len("scope='");end=s.index("')\n(A/",start)
s=s[:start]+'''Candidate303 fixes source clock mapping: same-scope receive gaps retain existing offset with bounded drift; rejected samples do not mutate clock;80ms-future/250ms-stale/monotonic gates apply after gaps too. Only new session/world/map establishes new mapping. Regression reproduces prior queued-packet rebase failure. Bounded128-event clock diagnostics record resets,gaps,rejections.302 failed native_state_stale with action183.9ms future; exact causal runtime gap not yet captured. Keeps cosmetic120ms buffer,25-rat native gait inputs proved301, and unchanged native Steam wire/bank. Overall power/gameplay/visual acceptance incomplete.''' + s[end:]
(W/'archive_rat_clock_v303.py').write_text(s)
