"""Audit 1282 (#33): lever and valve use variants filmed and accepted in both roles (takes use-variants-v1228i/j and
use-variants-dunwall-v1228k/l on 1214ak). Writes the audit, annotates lever_button_crank_wheel, updates chapter 92 and the
current state, and rebuilds the catalog and the generated checklist. Takes are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
D = R / 'documentation/handoff-2026-10-11'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
def edit(path, old, new):
    s = path.read_bytes().decode('utf-8')
    if '\r\n' in s: old = old.replace('\n', '\r\n'); new = new.replace('\n', '\r\n')
    assert s.count(old) == 1, (path.name, s.count(old), old[:80]); path.write_bytes(s.replace(old, new).encode('utf-8'))
at = datetime.datetime.now().astimezone().isoformat()
takes = ['use-variants-v1228i', 'use-variants-v1228j', 'use-variants-dunwall-v1228k', 'use-variants-dunwall-v1228l']
rows = []
for t in takes:
    r = rd(N / t / 'result.json'); i = rd(N / t / 'identity.json')
    for c in r.get('cases', []):
        rows.append(dict(take=t, candidate=i.get('candidate'), source=c.get('source'), observer=c.get('observer'), target=c.get('target'),
                         passed=c.get('passed'), shownSamples=c.get('shownSamples'), video=f"{t}/use-{c.get('source')}-{c.get('target')}-observed.mp4"))
visual = {
    'use-variants-v1228i/client': 'accepted: Emily on the turbine platform reaches the lever on the post, full body in view',
    'use-variants-v1228i/host': 'not accepted: Corvo\'s head and shoulders above the frame (2.4 m, pitch 15)',
    'use-variants-v1228j/host': 'accepted: Corvo fully framed reaching the lever (3.0 m, pitch 5); turbine blades pass in front at times',
    'use-variants-dunwall-v1228k/client': 'accepted: Emily reaches and turns the sink tap',
    'use-variants-dunwall-v1228k/host': 'not accepted: Corvo fills the frame and hides the tap (1.3 m behind, 0.5 m aside)',
    'use-variants-dunwall-v1228l/host': 'accepted: Corvo fully framed at the sink with his hand at the tap (1.4 m behind, 0.9 m aside, pitch 20)',
    'use-variants-dunwall-v1228l/client': 'accepted (regression of 1228k)'}
for r in rows: r['visual'] = visual.get(f"{r['take']}/{r['source']}")
findings = [
    'Framing (confirmed runtime): holding the observer camera in the same step as the teleport (pitch first) keeps the view at the intended spot while the body falls; every earlier lever spot fell from the turbine platform (z 20.5) to the street before the hold.',
    'Confirmed runtime: the turbine lever (variant 2) and the Dunwall sink tap (variant 4) pass in both roles on 1214ak, presented at weight 1.0.',
    'Visual: lever and valve accepted (functional) in both roles - Emily 1228i (lever) and 1228k/l (tap), Corvo 1228j (lever) and 1228l (tap).',
    'Harness: 1228i holds the camera at the teleport and adds -LeversOnly; 1228j adds -Back, -ObserverPitch and -LeverSources; 1228k adds -Side and -TapsOnly.']
wr(W / 'lever-valve-audit-v1282.json', dict(at=at, candidate='candidate-npc-combat-v1214ak', cases=rows, findings=findings, complete=True,
    scope='#33: lever and valve use variants filmed and accepted in both roles.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); it = an['items']['lever_button_crank_wheel']
it['notes'] = ('Audit 1282 (#33): the turbine lever and the sink tap are filmed and accepted (functional) in both roles '
               '(use-variants-v1228i/j, use-variants-dunwall-v1228k/l). ') + (it.get('notes') or '')
ev = '../movement-integration/lever-valve-audit-v1282.json'
if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(p, an)
edit(D / '92-native-use-interactions.md',
"Accepted visuals so far: Corvo at the vise and Emily at the ring lock. Lever and valve visual acceptance continues in #33.\n",
"""Accepted visuals so far: Corvo at the vise and Emily at the ring lock. Lever and valve visual acceptance continues in #33.

**#33 lever and valve visuals (1214ak, [audit 1282](../../work/movement-integration/lever-valve-audit-v1282.json)):** accepted (functional) in both roles.
- **Framing fix:** the observer takes its pitch, then teleports and holds the camera in the same step. The held view stays at the spot while the body falls. Every earlier lever spot fell from the turbine platform (z 20.5) to the street before the hold.

| Take | Mechanism | Source | Visual |
|---|---|---|---|
| [use-variants-v1228i](../../work/natural-moveset/use-variants-v1228i) | turbine lever | Emily | Accepted: full body on the platform, reaching the lever on the post |
| use-variants-v1228i | turbine lever | Corvo | Not accepted: head and shoulders above the frame (2.4 m, pitch 15) |
| [use-variants-v1228j](../../work/natural-moveset/use-variants-v1228j) | turbine lever | Corvo | Accepted: fully framed from 3.0 m at pitch 5; turbine blades sometimes pass in front |
| [use-variants-dunwall-v1228k](../../work/natural-moveset/use-variants-dunwall-v1228k) | sink tap | Emily | Accepted: reaches and turns the tap |
| use-variants-dunwall-v1228k | sink tap | Corvo | Not accepted: his body fills the frame and hides the tap (1.3 m behind, 0.5 m aside) |
| [use-variants-dunwall-v1228l](../../work/natural-moveset/use-variants-dunwall-v1228l) | sink tap | Corvo (Emily again) | Accepted: fully framed with his hand at the tap (1.4 m behind, 0.9 m aside, pitch 20) |

Harness options: `-LeversOnly` (1228i); `-Back`, `-ObserverPitch` and `-LeverSources` (1228j); `-Side` and `-TapsOnly` (1228k).
""")
edit(D / '01-current-state.md',
"protocol 75, bank 71/374 (bank 1266). Latest:\n",
"""protocol 75, bank 71/374 (bank 1266). Latest:
- **Lever and valve visuals (#33, [audit 1282](../../work/movement-integration/lever-valve-audit-v1282.json)):** the Edge of the World turbine lever and the Dunwall sink tap are filmed and accepted in both roles. The observer camera is held at the moment of the teleport ([chapter 92](92-native-use-interactions.md)).
""")
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = m['current'] + ' Lever and valve use variants are filmed and accepted in both roles (#33, audit 1282).'
(R / 'work/current-task-state.json').write_bytes(json.dumps(s, indent=2, ensure_ascii=True).replace('\n', '\r\n').encode('utf-8'))
for script, cwd in ((N / 'build_review_catalog.py', N), (R / 'work/regen_movement_requirements.py', R)):
    out = subprocess.run([sys.executable, '-X', 'utf8', str(script)], cwd=cwd, capture_output=True, text=True)
    print(out.stdout[-200:], out.stderr[-300:]); assert out.returncode == 0
