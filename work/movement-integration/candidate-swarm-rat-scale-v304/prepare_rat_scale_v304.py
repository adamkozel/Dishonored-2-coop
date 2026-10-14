from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab'
p=L/'src/swarm_rat_visual_fixture.h';s=p.read_text()
needle='a.gaitSpeed,a.gaitRate,a.gaitMoving,a.gaitUpdates);';assert needle in s
s=s.replace('\\"gaitUpdates\\":%llu}', '\\"gaitUpdates\\":%llu,\\"sourceScale\\":%.6f,\\"scaleReady\\":%s,\\"scaleVerified\\":%llu,\\"frameScale\\":[%.6f,%.6f,%.6f],\\"entityScale\\":[%.6f,%.6f,%.6f],\\"renderScale\\":[%.6f,%.6f,%.6f]}')
s=s.replace(needle,'a.gaitSpeed,a.gaitRate,a.gaitMoving,a.gaitUpdates,a.sourceScale,a.scaleReady?"true":"false",a.scaleVerified,a.frameScale[0],a.frameScale[1],a.frameScale[2],a.entityScale[0],a.entityScale[1],a.entityScale[2],a.renderScale[0],a.renderScale[1],a.renderScale[2]);');p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v303.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v303.ps1','Test-SwarmRatVisualNetwork-v304.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v303.ps1').read_text(encoding='utf-8-sig').replace('$g.gaitSpeed/1.11','$g.gaitSpeed/(1.11*$g.sourceScale)')
needle=" $checks.Add('Named native gait inputs read back correctly and track measured horizontal travel divided by native1.11m/s stride speed; idle/moving samples covered')";assert needle in s
s=s.replace(needle,needle.replace('native1.11m/s stride speed','native1.11m/s stride speed multiplied by each rat scale')+'''
 $scaled=@($gaits|Where-Object {$_.scaleVerified-gt5})
 if($scaled.Count-lt500){throw 'Insufficient native scale settlement samples'}
 foreach($g in $scaled){if(!$g.scaleReady){throw 'Previously settled scale became unready'};foreach($v in @($g.frameScale)+@($g.entityScale)+@($g.renderScale)){if([Math]::Abs($v-$g.sourceScale)-gt.0001){throw 'Native frame/entity/render scale differs from source'}}}
 if(@($scaled.sourceScale|Sort-Object -Unique).Count-lt10){throw 'Source size variation missing'}
 foreach($row in $fullPool){foreach($g in @($row.visual.models|Where-Object active)){$src=@($row.peer.agents|Where-Object index -eq $g.index);if($src.Count-ne1-or[Math]::Abs($src[0].scale-$g.sourceScale)-gt.0001){throw 'Cosmetic scale is not the matching source rat scale'}}}
 [ordered]@{settledSamples=$scaled.Count;distinctScales=@($scaled.sourceScale|Sort-Object -Unique).Count;minimum=($scaled.sourceScale|Measure-Object -Minimum).Minimum;maximum=($scaled.sourceScale|Measure-Object -Maximum).Maximum;nativeDispatch='0x140753630';visualAccepted=$false}|ConvertTo-Json|Set-Content "$out/scale-summary.json"
 $checks.Add('Each owned rat uses its replicated size through native reference-frame dispatch; frame/entity/render readbacks all match and gait stride accounts for size')
''')
s=s.replace('source-specific animation phase/state, scale, subjective','source-specific animation phase/state, color, subjective');(P/'Test-SwarmRatVisualNetwork-v304.ps1').write_text(s)
s=(W/'archive_rat_clock_v303.py').read_text().replace('rat_clock_v303','rat_scale_v304').replace('swarm-rat-clock-v303','swarm-rat-scale-v304').replace("f'swarm-rat-clock-{kind}-v303.log'","f'swarm-rat-scale-{kind}-v304.log'").replace("'swarm-rat-clock-*-v303.log'","'swarm-rat-scale-*-v304.log'").replace('Test-SwarmRatVisualNetwork-v303.ps1','Test-SwarmRatVisualNetwork-v304.ps1')
s=s.replace("read(R/'work/natural-moveset/swarm-rat-cadence-v300/result.json')", "read(R/'work/natural-moveset/swarm-rat-clock-v303/result.json')")
start=s.index("scope='")+len("scope='");end=s.index("')\n(A/",start)
s=s[:start]+'''Candidate304: native rat size replication through exact guarded753630 self-driver scale setter. Validate generic frame vt, self-driver vt1c574c0, reciprocal ownership, no attachment parent and same animation/render. Native queued update must settle within500ms; no direct entity/render scale writes. Read back frame/entity/render three axes. Walking stride uses1.11m/s multiplied by each rat size. Previous303 passed22checks with96.24percent bracketing and movement clock fix.120ms cosmetic buffer retained. Color, native attack/state phase, effects/audio, upgraded100rats and shared gameplay outcomes incomplete. Startup first-packet clock bias remains a separate observed issue.''' + s[end:]
(W/'archive_rat_scale_v304.py').write_text(s)
