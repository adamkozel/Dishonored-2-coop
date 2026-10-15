"""Audit 1260 (#11): visual-acceptance sweep of the runtime-bound checklist items that had no accepted visuals. Five review
passes (locomotion/stance, traversal, combat/paired, ranged/interactions, powers) built contact sheets from the recorded
real-game videos (partner views only) and judged the partner's full body under the functional-first criteria
(work/movement-integration/visual-sweep-v1260/INSTRUCTIONS.md); the lead reviewer spot-checked sheets before publishing.
Copies the review files as evidence, marks accepted videos and items, records rejections and insufficient views in the
item notes, and rebuilds the catalog. Contact sheets stay local (game footage)."""
from pathlib import Path
import json, datetime, shutil, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
SP = Path(sys.argv[1])  # scratchpad sweep folder with the *-review.json files
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
E = W / 'visual-sweep-v1260'; E.mkdir(exist_ok=True)
GROUPS = ('locomotion_stance', 'traversal', 'combat_paired', 'ranged_interactions', 'powers')
for g in GROUPS: shutil.copy2(SP / f'{g}-review.json', E / f'{g}-review.json')
shutil.copy2(SP / 'INSTRUCTIONS.md', E / 'INSTRUCTIONS.md')
reviews = {g: rd(E / f'{g}-review.json') for g in GROUPS}
videos = rd(N / 'runtime-video-manifest.json'); byPath = {v['video']: v for v in videos}
an = rd(N / 'review-evidence-annotations.json'); items = an['items']
# Effects or scope the body review cannot accept on its own (recorded separately so the body acceptance is not overstated).
SCOPE = {
    'corvo_devouring_swarm_cast': 'body accepted; the rat swarm is not visible in the observer world (effect gap)',
    'emily_shadow_walk_enter_move_attack_exit': 'body accepted; the shadow-walk transformed material is not applied and no attack is recorded',
    'corvo_possession_enter_loop_exit': 'body accepted for the empty-target aim and recovery only; possession of a real target is not recorded',
    'lethal_takedown_front_back_sides': 'rear presentation accepted (sweep); drop and slide accepted in audit 1257',
    'nonlethal_takedown_front_back_sides': 'rear presentation accepted (sweep); side/front runtime in audits 1257 and 1254c',
}
counts = {'accept': 0, 'reject': 0, 'insufficient': 0}; accepted = []; rejected = []; insufficient = []
at = datetime.datetime.now().astimezone().isoformat()
for g, rows in reviews.items():
    for r in rows:
        it = items[r['id']]; counts[r['verdict']] += 1
        chars = ', '.join(f'{c} {v}' for c, v in r['characters'].items() if v != 'n/a')
        ev = '../movement-integration/visual-sweep-v1260/' + f'{g}-review.json'
        if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
        for v in r['videos']:
            key = v['path'].replace('work/natural-moveset/', '', 1)
            if key in byPath and v['verdict'] == 'accept':
                m = byPath[key]; m['visual_accepted'] = True
                m['review'] = ((m.get('review') or '') + ' Sweep 1260: ' + v['review']).strip()
                if r['id'] not in (m.get('requirements') or []): m.setdefault('requirements', []).append(r['id'])
        if r['verdict'] == 'accept':
            it['visual_validation'] = 'accepted_sweep_1260_functional_both_roles' if 'n/a' not in r['characters'].values() else 'accepted_sweep_1260_functional_owner'
            note = f'Visual sweep 1260: accepted ({chars}). {r["summary"]}' + (f' Scope: {SCOPE[r["id"]]}.' if r['id'] in SCOPE else '')
            accepted.append(r['id'])
        else:
            note = f'Visual sweep 1260: {r["verdict"]} ({chars}). {r["summary"]}'
            (rejected if r['verdict'] == 'reject' else insufficient).append(dict(id=r['id'], summary=r['summary'], characters=r['characters']))
        it['notes'] = note + ' ' + (it.get('notes') or '')
wr(N / 'runtime-video-manifest.json', videos); wr(N / 'review-evidence-annotations.json', an)
wr(W / 'visual-sweep-audit-v1260.json', dict(at=at, method='Contact sheets from recorded real-game partner views (source first-person views skipped), functional-first criteria, five review passes plus lead spot checks.',
    counts=counts, accepted=accepted, rejected=rejected, insufficient=insufficient, scope_notes=SCOPE,
    findings=['Visual (sweep 1260): ' + f'{counts["accept"]} items accepted, {counts["reject"]} rejected with concrete defects, {counts["insufficient"]} without a judgeable view.',
              'Rejected defects to fix: Corvo crouched heavy attack snaps to standing for ~0.2 s before the windup (v636); the carried body appears on the shoulder in one frame with no lift (v13); the high grenade throw pops the body sideways 0.35-0.75 m at release; elixirs, pickups, notes and food show no partner gesture (the native game plays none in first person).',
              'Insufficient: fall loop (no long fall recorded), Emily slides (not in the listed videos), run 8 directions (no dedicated run take), doors (partner hidden), key use (no Corvo view), map load (camera faces a wall), kick (legs only), death/ragdoll (no Corvo-sourced take).']))
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at
s['MovementExecutionPriority']['visualSweep'] = f'Audit 1260: {counts["accept"]} accepted, {counts["reject"]} rejected, {counts["insufficient"]} insufficient; fixes and missing views tracked in the audit.'
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(counts, out.stdout[-120:], out.stderr[-300:]); assert out.returncode == 0
