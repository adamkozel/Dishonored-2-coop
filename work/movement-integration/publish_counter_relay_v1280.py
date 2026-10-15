"""Audit 1280 (#9, #14): the NPC attack relay follows dynamic relevance (1214ah) and Emily's counterattack (takes 1258i-l,
1270i). Writes the audit and catalog annotations, updates the live state and rebuilds the catalog. Candidate archives are
never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1214ah'
at = datetime.datetime.now().astimezone().isoformat()
takes = ['counterattack-v1258i', 'counterattack-v1258j', 'counterattack-v1258k', 'counterattack-v1258l', 'mission-multicombat-v1270i']
rows = []
for t in takes:
    r = rd(N / t / 'result.json'); i = rd(N / t / 'identity.json')
    rows.append(dict(take=t, candidate=i.get('candidate'), passed=r.get('passed'), failure=r.get('failure')))
findings = [
    'Confirmed runtime (1258i/1258j, 1214ag): Emily never counter-killed; in 1258j the NPC attack relay was off on both peers during the case (host no_ready_actors, client negotiation_timeout), so the guard\'s swings at her clone never reached her world and no native parry was possible.',
    'Confirmed static: the relay bound only the NPCs present at its start and stopped on any retirement (actor_retired), any binding that missed negotiation (negotiation_timeout), an empty start (no_ready_actors) or a peer re-arm (peer_replaced); nothing re-armed it outside checkpoint recovery. Emily\'s health stayed 70 in every 1270 fight.',
    'Fix (1214ah): relay bindings follow relevance (added/retired per NPC each tick, 3 s re-add block), a late binding retires alone, contacts from not-ready bindings are dropped, a peer re-arm restarts the sequence, and the product controller re-arms a stopped relay after 3 s.',
    'Confirmed runtime (1270i, 1214ah): in a three-guard fight relayed guard hits damaged Emily (70 -> 55 -> 40, 2 applied); 25 bindings added and 14 retired on each peer; inconclusive overall (Corvo died at 17.8 s).',
    'Confirmed runtime (1258k): the relay was active but the guard never swung at Emily\'s clone while she attacked every 1.15 s (0 hits sent).',
    'Confirmed runtime (1258l, harness 1258e, block held 1.6 s per cycle): 9 relayed guard hits (3 blocked, 1 parry-lose, 5 applied); Emily\'s native sword_parry_win then the counter finisher fatality_leftleg_medium_a; the host presented it (phase 105) and the guard is dead in both worlds, 0.0 m apart.',
    'Visual: the 1258l observer view is far and dark (opposite balcony); not judgeable. Counterattack visual acceptance stays open.']
wr(W / 'counter-relay-audit-v1280.json', dict(at=at, candidate=CAND, takes=rows, findings=findings, complete=False,
    scope='#9 counterattack in both roles (runtime); #14 NPC attack relay under dynamic relevance. Visual acceptance of the counter pending; player knockdown open.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); items = an['items']
it = items['counterattack']
it['runtime_binding'] = 'native_counter_kill_finisher_shared_both_roles'
it['notes'] = ('Audit 1280 (1214ah): Emily\'s timed parry win against a relayed guard hit then attack plays the counter finisher fatality_leftleg_medium_a, presented by the host (phase 105) and dead in both worlds (1258l). '
               'This needed the NPC attack relay to follow dynamic relevance. Visual review pending (1258l view too far). ') + (it.get('notes') or '')
ev = '../movement-integration/counter-relay-audit-v1280.json'
if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(p, an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = m['current'].replace('1214ag live', '1214ah live') + ' The NPC attack relay follows relevance (1214ah) and Emily\'s counterattack is shared (1258l).'
(R / 'work/current-task-state.json').write_bytes(json.dumps(s, indent=2, ensure_ascii=True).replace('\n', '\r\n').encode('utf-8'))
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
