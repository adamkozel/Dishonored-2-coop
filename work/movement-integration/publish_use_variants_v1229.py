"""Audit 1229 (#4/#5): use variants on real mechanisms - takes 1227b, 1228, 1228b and the 1228c takes.
Writes the audit, adds the takes' videos (with honest per-video reviews from contact-sheet inspection)
to the runtime video manifest, updates the catalog annotations, the candidate identities and the
current-task state, then rebuilds the review catalog. Run after the takes; reviews live in REVIEW."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')

TAKES = {  # take -> candidate
    'mission-mechanisms-v1227b': 'candidate-npc-combat-v1213f',
    'use-variants-dunwall-v1228': 'candidate-npc-combat-v1213g',
    'use-variants-dunwall-v1228b': 'candidate-npc-combat-v1213g',
    'use-variants-eotw-v1228c': 'candidate-npc-combat-v1213h',
    'use-variants-dunwall-v1228c': 'candidate-npc-combat-v1213h',
}
TAKES = {t: c for t, c in TAKES.items() if (N / t / 'result.json').exists()}
REVIEW = json.loads((W / 'use-variants-review-v1229.json').read_text(encoding='utf-8')) if (W / 'use-variants-review-v1229.json').exists() else {}

cases = []
for take, cand in TAKES.items():
    r = rd(N / take / 'result.json'); rest = rd(N / take / 'restoration.json')
    cases.append(dict(take=f'../natural-moveset/{take}', candidate=cand, passed=r['passed'], failure=r.get('failure'),
                      restoration=rest['passed'], cases=r.get('cases')))

def variant_pass(variant):
    """Roles in which a required case of this variant passed, across all takes."""
    roles = set()
    for t in cases:
        for c in t['cases'] or []:
            if c.get('passed') and (c.get('expected') == variant or c.get('sourceLastVariant') == variant and c.get('used')):
                roles.add(c['source'])
    return sorted(roles)

coverage = {name: variant_pass(v) for name, v in (('generic', 1), ('lever', 2), ('crank', 3), ('wheel_valve', 4), ('key_lock', 5))}

videos = rd(N / 'runtime-video-manifest.json')
for take, cand in TAKES.items():
    T = N / take; res = rd(T / 'result.json')
    for f in sorted(T.glob('*-observed.mp4')):
        meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
        a, b = map(float, meta['avg_frame_rate'].split('/'))
        path = f'{take}/{f.name}'; src = 'host' if '-host-' in f.name else 'client'; obs = 'client' if src == 'host' else 'host'
        rv = REVIEW.get(path, {})
        case = next((c for c in res.get('cases') or [] if c.get('source') == src and c.get('target') and c['target'] in f.name), {})
        reqs = rv.get('requirements', [])
        videos = [x for x in videos if x['video'] != path]
        videos.append(dict(video=path, role=obs, character='corvo' if obs == 'client' else 'emily',
            label=f"Interaction / native use of {case.get('target', '?')} -> authored variant {case.get('expected', '?')} (source {src}) / {take}",
            travelling=False, gameplay_bound=True, actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'],
            timing='Native wall-time recording, no speedup',
            scope='Observer window of the remote partner while the source player natively uses a real mission mechanism (player state 75 + focused usable).',
            review=rv.get('review', 'Raw native recording; not reviewed.'), candidate=cand, dll=rd(W / cand / 'identity.json')['dll'],
            runtime_test_passed=bool(case.get('passed')), visual_accepted=bool(rv.get('accepted')), requirements=reqs if rv.get('accepted') else []))
wr(N / 'runtime-video-manifest.json', videos)

at = datetime.datetime.now().astimezone().isoformat()
findings = [
    'Confirmed static (tools/diag/d2_resource_index.py): usables carry native interaction names - Usable_lever, Usable_vise, Usable_crank (valve_set), Usable_tap, Usable_lock (ring_locker), Usable_latch, Usable_redbutton.',
    'Confirmed runtime (1228/1228b, 1213g): the Dunwall ring locker (variant 5) and sink tap (variant 4) are used natively in both roles and presented on the partner at weight 1.0 to 100%, ending by duration.',
    'Confirmed static: Send-D2Input publishes an expiring packet and returns at once; the harness Neutral right after the press cut presses to ~50-150 ms (first presses missed in 1227b/1228). Held presses (1228b) registered every use on the first press.',
    'Confirmed runtime (1228b, #5): the host window use ended because the opening sash dropped the source 0.46 m off a ledge (0.36 m horizontal, 0.668 m 3D > 0.6 m). 1213h ends a use on horizontal travel only.',
]
wr(W / 'use-variants-audit-v1229.json', dict(at=at, cases=cases, coverage=coverage, findings=findings,
    complete=all(len(v) == 2 for v in coverage.values()),
    outstanding=['Visual acceptance of each variant on real mechanisms (framing).', 'Hand targeting to the actual handle geometry (#6).']))

p = N / 'review-evidence-annotations.json'; an = rd(p)
lev = an['items']['lever_button_crank_wheel']; key = an['items']['key_use']
both = lambda v: len(coverage[v]) == 2
lev['runtime_binding'] = 'native_use_state75_real_mechanisms_' + '_'.join(k for k in ('generic', 'lever', 'crank', 'wheel_valve') if both(k)) + '_both_roles'
key['runtime_binding'] = 'native_use_state75_real_lock_both_roles' if both('key_lock') else key['runtime_binding']
note = f" Audit 1229: real mechanisms, both roles - {', '.join(f'{k}: {'/'.join(v) or 'none'}' for k, v in coverage.items())}."
for it in (lev, key):
    it['notes'] = (it.get('notes') or '') + note
    it.setdefault('evidence', [])
    if '../movement-integration/use-variants-audit-v1229.json' not in it['evidence']:
        it['evidence'].append('../movement-integration/use-variants-audit-v1229.json')
wr(p, an)

for take, cand in TAKES.items():
    C = rd(W / cand / 'identity.json'); r = rd(N / take / 'result.json')
    if r['passed'] and not C.get('runtimeVerified'):
        C.update(runtimeVerified=True, runtimeEvidence='../use-variants-audit-v1229.json', result=f'{take} pass')
        wr(W / cand / 'identity.json', C)

s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at
s['MovementExecutionPriority']['current'] = ('1213h live (protocol 74, bank 71/374): native use on real mechanisms presents the authored variant on the partner body; '
    f"coverage (roles with a passing take): {json.dumps(coverage)}. #5 early end explained (window drop) and fixed in 1213h (horizontal-only end rule).")
s['MovementExecutionPriority']['next'] = 'Stage 3: visual acceptance of the use variants, #6 hand targeting, #7 climbable window, #8 cutscene entry/exit, #9 counter/knockdown, #10 restore latency, #11 checklist.'
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-200:], out.stderr[-300:]); assert out.returncode == 0
print(json.dumps(coverage))
