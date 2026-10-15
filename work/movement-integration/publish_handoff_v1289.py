"""Audit 1289 (#11 scripted_scene_pose_handoff): visual review of the hand-back from a native use scene to locomotion at
10 fps (use-variants-dunwall-v1228t host lock: the arm withdraws from the lock and the body settles into idle over a few
frames, no pop), with the other accepted use takes in both roles. Annotates the catalog and rebuilds the catalog and the
generated checklist."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
at = datetime.datetime.now().astimezone().isoformat()
findings = [
    'Visual (10 fps, use-variants-dunwall-v1228t host lock, 3.6-5.6 s): Corvo\'s arm withdraws from the lock and the body settles into idle over about 0.3 s with no pop; the use weight fades as audit 1241 measured (traversal 0.34/0.07/0.02 while locomotion returns 0.49/0.89/0.97).',
    'The same hand-back reads in both roles in the accepted lever, tap, lock and door takes (use-variants-v1228i/j, use-variants-dunwall-v1228l/s/t) and when the reading hold releases (coverage-v1286e).',
    'Accepted (functional) for use scenes in both roles. Takedown and finisher scenes hand back through their own corpse or victim paths (chapters 93 and 95).']
wr(W / 'handoff-audit-v1289.json', dict(at=at, takes=['use-variants-dunwall-v1228t', 'use-variants-v1228i', 'use-variants-v1228j', 'use-variants-dunwall-v1228l', 'use-variants-dunwall-v1228s', 'coverage-v1286e'],
    findings=findings, complete=True, scope='#11 scripted_scene_pose_handoff visual review.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); ev = '../movement-integration/handoff-audit-v1289.json'
it = an['items']['scripted_scene_pose_handoff']; it['visual_validation'] = 'accepted_functional_both_roles_use_scene_handback'
it['notes'] = 'Audit 1289: the hand-back from a native use scene to locomotion has no pop at 10 fps (1228t) and reads in both roles across the accepted use takes; accepted functionally. ' + (it.get('notes') or '')
if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(p, an)
for script, cwd in ((N / 'build_review_catalog.py', N), (R / 'work/regen_movement_requirements.py', R)):
    out = subprocess.run([sys.executable, '-X', 'utf8', str(script)], cwd=cwd, capture_output=True, text=True)
    print(out.stdout[-200:], out.stderr[-300:]); assert out.returncode == 0
