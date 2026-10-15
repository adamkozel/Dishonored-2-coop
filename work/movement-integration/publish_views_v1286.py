"""Audit 1286 (#11): crouched heavy (coverage-v1285a) and map-load / checkpoint-restart recovery (mission-lifecycle
v1199e-v1199h) accepted in both roles; the free observer camera hides the observer's own first-person arms (1214aq); the
lab health hold is confirmed at runtime (counterattack-v1258ap). Writes the audit, annotates the catalog, updates chapters
92/94 and the current state, rebuilds the catalog and checklist. Takes are never rewritten."""
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
for t in ['coverage-v1285a', 'counterattack-v1258ap', 'mission-lifecycle-v1199e', 'mission-lifecycle-v1199f', 'mission-lifecycle-v1199g', 'mission-lifecycle-v1199h']:
    p = N / t
    r = rd(p / 'result.json') if (p / 'result.json').exists() else rd(p / 'verified.json') if (p / 'verified.json').exists() else {}
    i = rd(p / 'identity.json') if (p / 'identity.json').exists() else {}
    rows.append(dict(take=t, candidate=i.get('candidate'), passed=r.get('passed'), failure=r.get('failure')))
findings = [
    'Confirmed runtime (coverage-v1285a, 1214ap): crouched at the open lab spot, two 1.5 s charged attacks per role; the partner presented the crouched heavy (combat variant 18) in 11-12 samples. Visual: Corvo stays crouched through charge, hold and release with no standing pop (the 1260 rejection), Emily likewise. Accepted (functional) in both roles.',
    'Confirmed runtime (mission-lifecycle-v1199e, 1214ap, -FreeCam): after the native map load and after the shared checkpoint restart the partner is present on both peers; the client window now shows Corvo (1199d faced a wall).',
    'Confirmed runtime (1199e/1199f entity lists): the floating shoulders and arms beside Corvo after the restart were the observer\'s own first-person arms (left_arm_1/right_arm_2 at the client body, which the restart left 0.9 m off its spot); there was exactly one partner clone. Not a co-op defect; 1199g (camera from actual positions) showed the arms in every view.',
    'Fix 1214aq (lab only): while an observer camera hold is active, the local first-person arm entities are hidden with the native hide (+0x8784b0) and shown on release after re-validation by handle.',
    'Confirmed runtime (mission-lifecycle-v1199h, 1214aq): Corvo on the client and Emily on the host, upright and whole after the load and after the restart, no arms in view (hides 2 and 4, shows 2). Visual: map-load and checkpoint recovery accepted in both roles.',
    'Confirmed runtime (counterattack-v1258ap, 1214ap): the lab health hold refilled the source 4 times and the source ended at 70 of 70; the guard walked away and never engaged (combat views stay parked).']
wr(W / 'views-audit-v1286.json', dict(at=at, candidates=['candidate-npc-combat-v1214ap', 'candidate-npc-combat-v1214aq'], takes=rows, findings=findings, complete=True,
    scope='#11: crouched heavy and map-load/checkpoint recovery views; free camera hides the observer arms; health hold verified.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); ev = '../movement-integration/views-audit-v1286.json'
for k, vis, note in (('heavy_attack_variants', 'accepted_functional_both_roles_crouched_heavy_retake', 'Audit 1286: Corvo\'s crouched heavy re-taken (coverage-v1285a) without the standing pop; accepted in both roles. '),
                     ('map_load_checkpoint_recovery', 'accepted_functional_both_roles', 'Audit 1286: the partner is filmed upright and whole on both peers after the map load and the shared checkpoint restart (mission-lifecycle-v1199h); accepted in both roles. ')):
    it = an['items'][k]; it['visual_validation'] = vis; it['notes'] = note + (it.get('notes') or '')
    if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(p, an)
edit(D / '94-visual-sweep-counterattack-and-gestures.md', "## Long fall and landing (audit 1283)\n",
"""## Crouched heavy and map-load recovery (audit 1286)

[Audit 1286](../../work/movement-integration/views-audit-v1286.json):
- **Crouched heavy:** re-taken at the open lab spot with the free camera ([coverage-v1285a](../../work/natural-moveset/coverage-v1285a)). Corvo stays crouched through charge, hold and release, with no standing pop. Accepted (functional) in both roles.
- **Map load and checkpoint restart:** filmed from both windows with the free camera ([mission-lifecycle-v1199h](../../work/natural-moveset/mission-lifecycle-v1199h)). Corvo on the client and Emily on the host are upright and whole after both. Accepted (functional) in both roles.
  - **Takes 1199e–1199g:** the floating arms beside Corvo after the restart were the observer's own first-person arms, not a partner defect (confirmed runtime from the entity lists).
  - **Fix (1214aq):** the free camera hides them while it holds.

## Long fall and landing (audit 1283)
""")
edit(D / '92-native-use-interactions.md', "- **Take [use-variants-dunwall-v1228u](../../work/natural-moveset/use-variants-dunwall-v1228u):** lock and door pass in both roles with the camera held at the spot.\n",
"""- **Take [use-variants-dunwall-v1228u](../../work/natural-moveset/use-variants-dunwall-v1228u):** lock and door pass in both roles with the camera held at the spot.
- **Hidden arms (1214aq):** while a hold is active, the observer's own first-person arm entities are hidden with the native hide (+0x8784b0) and shown again on release. Before, they floated in view when the observer's body stood in front of the camera ([audit 1286](../../work/movement-integration/views-audit-v1286.json)).
""")
edit(D / '01-current-state.md', "Live candidate-npc-combat-v1214ap ([identity](../../work/movement-integration/candidate-npc-combat-v1214ap/identity.json))",
     "Live candidate-npc-combat-v1214aq ([identity](../../work/movement-integration/candidate-npc-combat-v1214aq/identity.json))")
edit(D / '01-current-state.md', "protocol 75, bank 71/374 (bank 1266). Latest:\n",
"""protocol 75, bank 71/374 (bank 1266). Latest:
- **Crouched heavy and map-load recovery views (#11, [audit 1286](../../work/movement-integration/views-audit-v1286.json)):** both accepted in both roles. The free camera hides the observer's own arms (1214aq), and the lab health hold is confirmed at runtime ([chapter 94](94-visual-sweep-counterattack-and-gestures.md#crouched-heavy-and-map-load-recovery-audit-1286)).
""")
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = m['current'].replace('1214ap live', '1214aq live') + ' Crouched heavy and map-load recovery accepted; free camera hides the observer arms (audit 1286).'
(R / 'work/current-task-state.json').write_bytes(json.dumps(s, indent=2, ensure_ascii=True).replace('\n', '\r\n').encode('utf-8'))
for script, cwd in ((N / 'build_review_catalog.py', N), (R / 'work/regen_movement_requirements.py', R)):
    out = subprocess.run([sys.executable, '-X', 'utf8', str(script)], cwd=cwd, capture_output=True, text=True)
    print(out.stdout[-200:], out.stderr[-300:]); assert out.returncode == 0
