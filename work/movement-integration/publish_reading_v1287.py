"""Audit 1287 (#11 read_object_or_note): the partner presents a reading hold while the source reads a note (1214ar-1214at,
takes coverage-v1286a-e). Writes the audit, annotates the catalog, updates chapter 94 and the current state, and rebuilds
the catalog and checklist. Takes are never rewritten."""
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
rows = []
for t in ['coverage-v1286a', 'coverage-v1286b', 'coverage-v1286c', 'coverage-v1286d', 'coverage-v1286e']:
    r = rd(N / t / 'result.json'); i = rd(N / t / 'identity.json')
    rows.append(dict(take=t, candidate=i.get('candidate'), passed=r.get('passed'), failure=r.get('failure')))
findings = [
    'Confirmed runtime (1286a): from 0.8 m the table note needed an unsupported steep aim pitch; the source stands 1.3 m away from 1286b on.',
    'Confirmed runtime (1286b on 1214ar): both sources entered the native reading UI (logic 4) but the partner showed no reading hold.',
    'Confirmed runtime (1286c on 1214as, lease diagnostic): the movement resume suspends the bank and the shadow stream for any menu, the partner\'s ReadNote included, so a bank lease cannot cover reading.',
    'Fix 1214at: the reading hold depends only on the readable retention and a loaded bank; the apply draws the held-object reach (use variant 6, held amount) over the frozen body without a lease. Presentation only: no root, action or damage is sampled from the menu.',
    'Confirmed runtime (1286d/1286e on 1214at): the reading hold is presented in both roles (12-13 samples at full weight) and clears when reading ends.',
    'Visual (1286e, camera 1.5 m above the source\'s actual feet): the partner raises the arm into the held reach while reading and returns to idle afterwards, in both roles. Accepted (functional); a two-handed reading pose (the sword may still be in hand) is polish.']
wr(W / 'reading-audit-v1287.json', dict(at=at, candidates=['candidate-npc-combat-v1214ar', 'candidate-npc-combat-v1214as', 'candidate-npc-combat-v1214at'], takes=rows, findings=findings, complete=True,
    scope='#11 read_object_or_note: partner reading hold while the source reads a note.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); ev = '../movement-integration/reading-audit-v1287.json'
it = an['items']['read_object_or_note']; it['visual_validation'] = 'accepted_functional_both_roles_held_reach_reading'
it['notes'] = 'Audit 1287 (1214at): while the source reads a note the partner holds the held-object reach and returns to idle afterwards, in both roles (coverage-v1286e); accepted functionally, a two-handed reading pose is polish. ' + (it.get('notes') or '')
if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(p, an)
edit(D / '94-visual-sweep-counterattack-and-gestures.md', "## Crouched heavy and map-load recovery (audit 1286)\n",
"""## Reading a note (1214ar–1214at, audit 1287)

While the partner reads a note (native ReadNote4), the readable retention keeps its body visible and frozen. Sweep 1260 rejected this item because the partner made no reading gesture.
- **Why a lease can't cover it (confirmed runtime, take 1286c):** the movement resume suspends the bank and the shadow stream for any menu, the partner's note included.
- **Fix (1214at):** in exactly that retained state, the bank's apply draws the existing held-object reach (use variant 6, held amount) over the frozen body. Nothing is sampled from the menu, and root, actions and damage stay untouched.
- **Take [coverage-v1286e](../../work/natural-moveset/coverage-v1286e):** the partner raises the arm into the held reach while reading and returns to idle afterwards, in both roles.
  - **Accepted (functional).** A two-handed reading pose is polish.
  - [Audit 1287](../../work/movement-integration/reading-audit-v1287.json).

## Crouched heavy and map-load recovery (audit 1286)
""")
edit(D / '01-current-state.md', "Live candidate-npc-combat-v1214aq ([identity](../../work/movement-integration/candidate-npc-combat-v1214aq/identity.json))",
     "Live candidate-npc-combat-v1214at ([identity](../../work/movement-integration/candidate-npc-combat-v1214at/identity.json))")
edit(D / '01-current-state.md', "protocol 75, bank 71/374 (bank 1266). Latest:\n",
"""protocol 75, bank 71/374 (bank 1266). Latest:
- **Reading a note (#11, 1214at, [audit 1287](../../work/movement-integration/reading-audit-v1287.json)):** while the partner reads, they hold a reach instead of standing frozen; accepted in both roles ([chapter 94](94-visual-sweep-counterattack-and-gestures.md#reading-a-note-1214ar1214at-audit-1287)).
""")
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = m['current'].replace('1214aq live', '1214at live') + ' The partner holds a reading reach while the source reads a note (audit 1287).'
(R / 'work/current-task-state.json').write_bytes(json.dumps(s, indent=2, ensure_ascii=True).replace('\n', '\r\n').encode('utf-8'))
for script, cwd in ((N / 'build_review_catalog.py', N), (R / 'work/regen_movement_requirements.py', R)):
    out = subprocess.run([sys.executable, '-X', 'utf8', str(script)], cwd=cwd, capture_output=True, text=True)
    print(out.stdout[-200:], out.stderr[-300:]); assert out.returncode == 0
