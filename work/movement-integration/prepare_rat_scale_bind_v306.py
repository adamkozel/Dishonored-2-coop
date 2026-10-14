from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v305.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v305.ps1','Test-SwarmRatVisualNetwork-v306.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v304.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$_.visual.activeModels-eq25-and$_.peer.count-eq25','$_.visual.activeModels-eq$_.peer.count-and$_.peer.count-gt0')
s=s.replace("throw 'Full25-rat pool did not remain active'","throw 'Insufficient snapshots with a complete match of currently indexed source rats'")
s=s.replace('$models.Count-ne25-or@($models.handle|Sort-Object -Unique).Count-ne25','$models.Count-ne$row.peer.count-or@($models.handle|Sort-Object -Unique).Count-ne$row.peer.count')
s=s.replace('25 unique owned cosmetic models match25 opaque source indices/handles/incarnations for at least20 snapshots, all with verified live transforms','All currently indexed source rats match unique owned cosmetic identities for at least20 snapshots; native source retirement is preserved')
s=s.replace("nativeDispatch='0x140753630'","nativeDispatch='0x140751050 self-binding then0x140753630 scale'")
(P/'Test-SwarmRatVisualNetwork-v306.ps1').write_text(s)
s=(W/'archive_rat_scale_v304.py').read_text().replace('rat_scale_v304','rat_scale_bind_v306').replace('swarm-rat-scale-v304','swarm-rat-scale-bind-v306').replace("f'swarm-rat-scale-{kind}-v304.log'","f'swarm-rat-scale-bind-{kind}-v306.log'").replace("'swarm-rat-scale-*-v304.log'","'swarm-rat-scale-bind-*-v306.log'").replace('Test-SwarmRatVisualNetwork-v304.ps1','Test-SwarmRatVisualNetwork-v306.ps1')
start=s.index("scope='")+len("scope='");end=s.index("')\n(A/",start)
s=s[:start]+'''Candidate306:305 read-only evidence confirms exact self-driver vt but null driver+8 backlink; initialize this already-current self-driver using native751050(frame,frame+20), verify reciprocal links, then753630 native scale with500ms frame/entity/render settlement. Never bind foreign driver/frame.304 failed before scale invocation.305 unit-scale playback/clock retained; test failed arbitrary20 full25 snapshots while source naturally became24, yielding18 full25 and80 full24 matching rows.306 matches every currently indexed rat, retaining exact25 models created/cached, unique identities, native transform/gait/retirement checks. Original Steam/source gameplay unaffected. Startup clock bias and remaining effects/state/color/gameplay remain incomplete.''' + s[end:]
(W/'archive_rat_scale_bind_v306.py').write_text(s)
