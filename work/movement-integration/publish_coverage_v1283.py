"""Audit 1283 (#11): coverage views - long fall and landing (coverage-v1283a), door use and Corvo's key use
(use-variants-dunwall-v1228s/t), and crouched uses presented on the partner (1214am, diagnosed with 1214al in takes
1228m-1228r). Writes the audit, annotates the catalog, updates chapters 92/94 and the current state, and rebuilds the
catalog and the generated checklist. Takes are never rewritten."""
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
for t in ['coverage-v1283a', 'use-variants-dunwall-v1228m', 'use-variants-dunwall-v1228n', 'use-variants-dunwall-v1228o',
          'use-variants-dunwall-v1228p', 'use-variants-dunwall-v1228q', 'use-variants-dunwall-v1228r', 'use-variants-dunwall-v1228s',
          'use-variants-dunwall-v1228t']:
    r = rd(N / t / 'result.json'); i = rd(N / t / 'identity.json')
    for c in r.get('cases', []):
        rows.append(dict(take=t, candidate=i.get('candidate'), source=c.get('source'), case=c.get('target') or c.get('case'),
                         passed=c.get('passed'), shownSamples=c.get('shownSamples'), failure=c.get('failure'), fallMetres=c.get('fallMetres')))
findings = [
    'Confirmed runtime (coverage-v1283a, 1214ak): a 5.3-6.1 m fall in both roles (native falling state 3, then grounded); the partner shows the falling pose with arms out, a deep landing crouch and recovery. Visual: fall loop and hard landing accepted (functional), both roles.',
    'Confirmed runtime (1228m, 1228n, 1228q, 1228r on 1214ak/1214al): Emily\'s ring-lock and office-door uses register at the source, and the host receives and accepts every use packet (1214al useDiag: 66 sent, 66 accepted, variant 5), but the host never presented them. Corvo\'s lock and door uses and both roles\' taps were presented (control 1228o).',
    'Cause (confirmed runtime): Emily was natively crouched (stance 1) at the lock and door, and the partner use presentation required a standing pose (stance 0, motion 0). Hypothesis: the harness modal-clearing B press toggles crouch.',
    'Fix 1214am: a native use made while crouched is presented as well; the partner rises into the standing use reach and returns to the crouch (polish: a crouched reach would be more faithful). 1214al keeps the use-path diagnostics (shadow-stream useDiag).',
    'Confirmed runtime (use-variants-dunwall-v1228s, 1214am): ring lock and office door pass in both roles. Visual: door use accepted (functional) in both roles; the lock framing was blocked by a candelabra and raised camera.',
    'Confirmed runtime (use-variants-dunwall-v1228t, 1214am, observer 1.8 m back on the other side): ring lock in both roles. Visual: Corvo reaches the bookcase lock, accepted (key_use for Corvo); Emily\'s crouched use rises into the reach, accepted (functional).',
    'Not available on the Dunwall lab map: a mana elixir pickup (the mana elixir view needs a mission map).']
wr(W / 'coverage-audit-v1283.json', dict(at=at, candidates=['candidate-npc-combat-v1214ak', 'candidate-npc-combat-v1214al', 'candidate-npc-combat-v1214am'],
    takes=rows, findings=findings, complete=True, scope='#11 coverage views: fall, landing, key use (Corvo), door use; crouched use presentation.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); items = an['items']
ev = '../movement-integration/coverage-audit-v1283.json'
updates = {
    'fall_loop': ('accepted_functional_both_roles_long_fall', 'Audit 1283: a 5-6 m fall in both roles shows the falling pose with arms out (coverage-v1283a), accepted functionally.'),
    'hard_landing': ('accepted_functional_both_roles_long_fall_landing', 'Audit 1283: the landing after a 5-6 m fall shows a deep crouch and recovery in both roles (coverage-v1283a), accepted functionally.'),
    'key_use': ('accepted_functional_both_roles', 'Audit 1283: Corvo reaches the ring lock (use-variants-dunwall-v1228t); Emily\'s crouched lock use is presented since 1214am. Accepted functionally in both roles.'),
    'door_open_close_locked': ('accepted_functional_both_roles_crouched_rise', 'Audit 1283: the partner reaches the office door in both roles (use-variants-dunwall-v1228s, 1214am); a crouched use rises into the standing reach (polish).')}
for k, (vis, note) in updates.items():
    it = items[k]; it['visual_validation'] = vis; it['notes'] = note + ' ' + (it.get('notes') or '')
    if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(p, an)
edit(D / '92-native-use-interactions.md',
"Harness options: `-LeversOnly` (1228i); `-Back`, `-ObserverPitch` and `-LeverSources` (1228j); `-Side` and `-TapsOnly` (1228k).\n",
"""Harness options: `-LeversOnly` (1228i); `-Back`, `-ObserverPitch` and `-LeverSources` (1228j); `-Side` and `-TapsOnly` (1228k).

**Crouched uses, doors and Corvo's key use (1214al–1214am, [audit 1283](../../work/movement-integration/coverage-audit-v1283.json)):**
- **Finding (confirmed runtime, takes 1228m–1228r):** the host never presented Emily's ring-lock and office-door uses, while Corvo's lock and door uses and both roles' taps worked. The 1214al counters showed every use packet sent and accepted (66 of 66, variant 5).
- **Cause (confirmed runtime):** Emily was natively crouched at the lock and the door, and the use presentation required a standing pose. Hypothesis: the harness's modal-clearing B press toggles crouch.
- **Fix (1214am):** a use made while crouched is presented too. The partner rises into the standing use reach and returns to the crouch; a crouched reach would be more faithful (polish).

| Take | Case | Result |
|---|---|---|
| [use-variants-dunwall-v1228s](../../work/natural-moveset/use-variants-dunwall-v1228s) | lock and door, both roles | Pass. Door accepted (functional) in both roles; the lock view was blocked |
| [use-variants-dunwall-v1228t](../../work/natural-moveset/use-variants-dunwall-v1228t) | lock, both roles (1.8 m back, other side) | Pass. Corvo reaches the bookcase lock; Emily rises from the crouch into the reach. Accepted (functional) |

`-Only` selects the Dunwall cases (1228l); 1228m samples both peers' shadow-stream status per case.
""")
edit(D / '94-visual-sweep-counterattack-and-gestures.md', "## Pickup, food and elixir gestures (1214j)\n",
"""## Long fall and landing (audit 1283)

Harness [Record-Coverage-v1283](../../work/natural-moveset/make_coverage_v1283.py) drops the source 6 m above its usual spot while the partner films at a level pitch. In take [coverage-v1283a](../../work/natural-moveset/coverage-v1283a) both roles fell 5.3–6.1 m (native falling state 3, then grounded). The partner shows the falling pose with arms out, a deep landing crouch and the recovery. **Fall loop and hard landing are accepted (functional) in both roles.**

## Pickup, food and elixir gestures (1214j)
""")
edit(D / '01-current-state.md', "protocol 75, bank 71/374 (bank 1266). Latest:\n",
"""protocol 75, bank 71/374 (bank 1266). Latest:
- **Coverage views (#11, 1214al–1214am, [audit 1283](../../work/movement-integration/coverage-audit-v1283.json)):** the long fall and landing, Corvo's key use and the door use are accepted in both roles. A use made while crouched is now presented on the partner; before, Emily's crouched lock and door uses showed nothing ([chapter 92](92-native-use-interactions.md), [chapter 94](94-visual-sweep-counterattack-and-gestures.md#long-fall-and-landing-audit-1283)).
""")
edit(D / '01-current-state.md', "Live candidate-npc-combat-v1214ak ([identity](../../work/movement-integration/candidate-npc-combat-v1214ak/identity.json))",
     "Live candidate-npc-combat-v1214am ([identity](../../work/movement-integration/candidate-npc-combat-v1214am/identity.json))")
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = m['current'].replace('1214ak live', '1214am live') + ' Crouched uses are presented (1214am); fall, landing, key use and door use accepted (audit 1283).'
(R / 'work/current-task-state.json').write_bytes(json.dumps(s, indent=2, ensure_ascii=True).replace('\n', '\r\n').encode('utf-8'))
for script, cwd in ((N / 'build_review_catalog.py', N), (R / 'work/regen_movement_requirements.py', R)):
    out = subprocess.run([sys.executable, '-X', 'utf8', str(script)], cwd=cwd, capture_output=True, text=True)
    print(out.stdout[-200:], out.stderr[-300:]); assert out.returncode == 0
