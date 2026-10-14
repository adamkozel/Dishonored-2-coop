from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab'
p=L/'src/swarm_rat_visual_fixture.h';s=p.read_text();needle='a.renderScale[0],a.renderScale[1],a.renderScale[2]);';assert needle in s
s=s.replace('\\"renderScale\\":[%.6f,%.6f,%.6f]}','\\"renderScale\\":[%.6f,%.6f,%.6f],\\"scaleStage\\":%u,\\"scaleDiagnostic\\":[%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu]}')
s=s.replace(needle,'a.renderScale[0],a.renderScale[1],a.renderScale[2],a.scaleStage,a.scaleDiagnostic[0],a.scaleDiagnostic[1],a.scaleDiagnostic[2],a.scaleDiagnostic[3],a.scaleDiagnostic[4],a.scaleDiagnostic[5],a.scaleDiagnostic[6],a.scaleDiagnostic[7]);');p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v304.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v304.ps1','Test-SwarmRatVisualNetwork-v305.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v303.ps1').read_text(encoding='utf-8-sig')
needle='    Q client \'--object\' $cp.components[0].owner|ConvertTo-Json -Depth 12|Set-Content "$out/visual-early-entity.json"';assert needle in s
s=s.replace(needle,needle+'''
    $frame=@($cp.components|Where-Object vtableRva -eq '0x1c573e8');if($frame.Count-eq1){Q client '--object' $frame[0].pointer|ConvertTo-Json -Depth 12|Set-Content "$out/visual-early-frame.json"}
''');(P/'Test-SwarmRatVisualNetwork-v305.ps1').write_text(s)
s=(W/'archive_rat_scale_v304.py').read_text().replace('rat_scale_v304','rat_scale_observe_v305').replace('swarm-rat-scale-v304','swarm-rat-scale-observe-v305').replace("f'swarm-rat-scale-{kind}-v304.log'","f'swarm-rat-scale-observe-{kind}-v305.log'").replace("'swarm-rat-scale-*-v304.log'","'swarm-rat-scale-observe-*-v305.log'").replace('Test-SwarmRatVisualNetwork-v304.ps1','Test-SwarmRatVisualNetwork-v305.ps1')
start=s.index("scope='")+len("scope='");end=s.index("')\n(A/",start)
s=s[:start]+'''Candidate305 restores303 working unit-scale gait/presentation and observes prospective scale ownership guards read-only. No scale setter can execute.304 refused all25 models before scale readback; exact guard cause pending. Diagnostic stage and frame/owner/vtable/driver/backlink/parent/render/animation-render addresses captured, plus native frame object read. Retains303 clock correction,120ms cosmetic buffer and33ms source cadence.''' + s[end:]
(W/'archive_rat_scale_observe_v305.py').write_text(s)
