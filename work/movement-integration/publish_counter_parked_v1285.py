"""Audit 1285 (#9, #11): kick and counter-kill attempts after the free camera (takes coverage-v1284a, counterattack-v1258z
to 1258ak), the native god mode found stripped (1214ao, removed in 1214ap) and the lab health hold (1214ap, built and
deployed, not yet runtime-verified). The counter-kill is parked on #9. Writes the audit, annotates the catalog notes,
updates chapter 94, the README index and the current state, and rebuilds the catalog and checklist. Takes are never
rewritten."""
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
for t in ['coverage-v1284a'] + [f'counterattack-v1258{x}' for x in ('z', 'aa', 'ab', 'ac', 'ad', 'ae', 'af', 'ag', 'ah', 'ai', 'aj', 'ak')]:
    if not (N / t / 'result.json').exists(): continue
    r = rd(N / t / 'result.json'); i = rd(N / t / 'identity.json')
    rows.append(dict(take=t, candidate=i.get('candidate'), passed=r.get('passed'), failure=r.get('failure')))
findings = [
    'Confirmed runtime (coverage-v1284a, 1214an, free camera): no native kick (throwhit) in 8 close attacks per role; the guard fought back and the source ended down (state 15). The kick view stays open.',
    'Confirmed runtime (counterattack-v1258y-ab, 1214ak, harness 1258m): blocked hits followed by slashes, no finisher; in z/aa/ab the guard never swung.',
    'Confirmed runtime (counterattack-v1258ac-ae, harness 1258n, attack only after sword_parry_win): the guard never swung (0 relayed hits) when the source did not attack.',
    'Confirmed runtime (counterattack-v1258af-ah): the sender allowlist refused ark_god_player; restored cleanly.',
    'Confirmed static: ark_god_player (object +0x398d500, value +0x398d528) is read only by the god console toggle (+0x718f93), never by damage code. Confirmed runtime (counterattack-v1258ai): health 56 -> 0 with the cvar set. 1214ao allowed the cvar; 1214ap removes it.',
    'Confirmed runtime (counterattack-v1258aj/ak): the guard never swung.',
    '1214ap adds the lab-only coop_player_health_hold (refills the local player health at each service tick); built, unit tests and deploy pass, not yet runtime-verified.',
    'Decision: Emily\'s counter-kill is parked on #9. It depends on the guard\'s AI attacking her clone, which most takes never got; a lab fixture that makes the guard attack on command would make it deterministic.']
wr(W / 'counter-parked-audit-v1285.json', dict(at=at, candidates=['candidate-npc-combat-v1214ao', 'candidate-npc-combat-v1214ap'], takes=rows, findings=findings, complete=False,
    scope='#9 counter-kill attempts (parked) and the #11 kick view; native god mode stripped; lab health hold built.'))
p = N / 'review-evidence-annotations.json'; an = rd(p)
ev = '../movement-integration/counter-parked-audit-v1285.json'
for k, note in (('counterattack', 'Audit 1285: Emily\'s counter-kill parked after takes 1258y-1258ak (the guard rarely attacks her clone; native god mode is stripped). '),
                ('kick_or_shove_if_present', 'Audit 1285: free-camera take coverage-v1284a produced no native kick in 8 close attacks per role; view still open. ')):
    it = an['items'][k]; it['notes'] = note + (it.get('notes') or '')
    if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(p, an)
edit(D / '94-visual-sweep-counterattack-and-gestures.md', "## Long fall and landing (audit 1283)\n",
"""## Counter-kill parked, kick view open (audit 1285)

[Audit 1285](../../work/movement-integration/counter-parked-audit-v1285.json) covers takes 1258y–1258ak and coverage-v1284a.
- Emily's counter-kill is parked on #9. Most takes never got a guard swing at her clone. The native god mode is stripped ([chapter 96](96-external-research-references.md)), so 1214ap adds a lab health hold (`coop_player_health_hold`), built but not yet runtime-verified.
- **Kick:** the free-camera kick take produced no native kick (`throwhit`) in 8 close attacks per role, so the kick view stays open.

## Long fall and landing (audit 1283)
""")
edit(D / '01-current-state.md', "Live candidate-npc-combat-v1214an ([identity](../../work/movement-integration/candidate-npc-combat-v1214an/identity.json))",
     "Live candidate-npc-combat-v1214ap ([identity](../../work/movement-integration/candidate-npc-combat-v1214ap/identity.json))")
edit(D / '01-current-state.md', "protocol 75, bank 71/374 (bank 1266). Latest:\n",
"""protocol 75, bank 71/374 (bank 1266). Latest:
- **Counter-kill parked (#9, [audit 1285](../../work/movement-integration/counter-parked-audit-v1285.json)):** Emily's counter-kill and the kick view stay open; the native god mode is stripped in this build. External research references are in [chapter 96](96-external-research-references.md).
""")
readme = D / 'README.md'; s = readme.read_bytes().decode('utf-8'); nl = '\r\n' if '\r\n' in s else '\n'
if '96-external-research-references.md' not in s:
    line = '- [96. External research references](96-external-research-references.md): public Dishonored 2 resources (camera, cvars, autosplitter, saves) and what was verified against the lab build.' + nl
    idx = s.find('95-shared-combat-authority.md'); end = s.find(nl, idx) + len(nl)
    s = s[:end] + line + s[end:]; readme.write_bytes(s.encode('utf-8'))
st = rd(R / 'work/current-task-state.json'); st['UpdatedAt'] = at; m = st['MovementExecutionPriority']
m['current'] = m['current'].replace('1214an live', '1214ap live') + ' Counter-kill parked; native god mode stripped; lab health hold built (audit 1285).'
(R / 'work/current-task-state.json').write_bytes(json.dumps(st, indent=2, ensure_ascii=True).replace('\n', '\r\n').encode('utf-8'))
for script, cwd in ((N / 'build_review_catalog.py', N), (R / 'work/regen_movement_requirements.py', R)):
    out = subprocess.run([sys.executable, '-X', 'utf8', str(script)], cwd=cwd, capture_output=True, text=True)
    print(out.stdout[-200:], out.stderr[-300:]); assert out.returncode == 0
