"""Audit 1281 (#9): client sword finishers presented on the host (1214ai-1214ak) and the Emily counter takes 1258m onward.
Writes the audit and catalog annotations, updates the live state and rebuilds the catalog and the generated checklist.
Candidate archives and takes are never rewritten. Usage: publish_finisher_follow_v1281.py [counter-take-accepted-visually]"""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1214ak'
COUNTER_TAKE = sys.argv[1] if len(sys.argv) > 1 else None  # a true counter-kill take (harness 1258n), if one passed
ACCEPTED_COUNTER = COUNTER_TAKE if len(sys.argv) > 2 and sys.argv[2] == 'visual' else None
at = datetime.datetime.now().astimezone().isoformat()
earlier = {'counterattack-v1258' + c for c in 'abcdefghijkl'}
takes = sorted(p.name for pattern in ('counterattack-v1258*', 'finisher-follow-v1258*') for p in N.glob(pattern)
               if p.is_dir() and p.name not in earlier and (p / 'result.json').exists())
rows = []
for t in takes:
    r = rd(N / t / 'result.json'); i = rd(N / t / 'identity.json')
    rows.append(dict(take=t, candidate=i.get('candidate'), passed=r.get('passed'), failure=r.get('failure')))
findings = [
    'Confirmed runtime (video, 1258l on 1214ah): Emily\'s own view shows the close paired counter stab, but on the host the guard stayed about 2 m away and fell where it stood. The host refused her PairedHold on a corpse (the replayed finisher kill arrives first) and rejected all 63 victim poses.',
    'Fix 1214ai: a lethal paired hold on a corpse that died within 3 s keeps the corpse animated and follows the client victim pose until 1.6 s after the last hold (npcPairedCorpseFollows, unit-tested).',
    'Confirmed runtime (1258m on 1214ai): an alerted guard refuses the AI lease, so the host guard walked 3 m away during fatality_rightarm_fast_a, 47 victim poses were rejected and the lethal replay failed its 3 m source check (native_replay_failed); the guard got up in both worlds and killed Emily.',
    'Fix 1214aj: a lethal scene whose lease is refused follows the client victim pose (host_paired_follow), keeping the replay proximity true; the NPC attack relay drops hits from a guard inside the client paired scene.',
    'Confirmed runtime (finisher-follow-v1258a/f on 1214aj; 1256f/1256g on 1214g; 1258l): the client replica rose 7-26 m during the finisher. Once the host followed the client pose, both bodies ended at z 75 (video 1258f).',
    'Fix 1214ak: the client holds its corpse replica animated only after its paired window ends. Confirmed runtime (finisher-follow-v1258i): max z 67.4, final 66.7 in both worlds; the guard falls at Emily\'s feet on the host. Visual accepted (functional).',
    'Harness (1258f-1258m): walkway camera spots with a 30 degree pitch; block held until a blocked guard hit is detected, 0.3 s more, release 60 ms, then attack (an attack replacing a held block in the same frame never slashed, 1258q). In 1258l/1258m the finishers followed blocked hits (sword_parry_hit), not sword_parry_win.',
    'Correction (confirmed runtime, host npc-combat of 1258l/1258m): the 1258l finisher was a killing blow, Emily\'s 4th committed wound on the 40-health guard after a blocked hit (sword_parry_hit_1), not the native counter-kill, which follows sword_parry_win (Corvo, 1258f). Audit 1280 called it a counter; the 1258y harness run (blocked hit then attack) gave plain slashes.',
    'Hypothesis: whether the guard swings at Emily\'s clone varies by take (no host contacts in 1258o/r/t/u/v/w/x); the counter take therefore needs retries.']
if COUNTER_TAKE:
    findings.append(f'Confirmed runtime ({COUNTER_TAKE}, harness 1258n): Emily\'s sword_parry_win then attack plays the counter-kill finisher on a full-health guard, presented by the host and dead in both worlds.')
if ACCEPTED_COUNTER:
    findings.append(f'Visual: {ACCEPTED_COUNTER} reviewed and accepted (functional): the host presents Emily\'s counter finisher with the guard in the paired pose.')
wr(W / 'finisher-follow-audit-v1281.json', dict(at=at, candidate=CAND, takes=rows, findings=findings, complete=bool(ACCEPTED_COUNTER),
    scope='#9: client sword finishers presented on the host in both AI states, the client ragdoll launch, and Emily counter provocation.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); items = an['items']
it = items['counterattack']
note = ('Audit 1281 (1214ak): client finishers now follow on the host (corpse follow 1214ai, follow without AI lease 1214aj) and no longer launch the body (1214ak); finisher-follow-v1258i accepted functionally. '
        + 'Correction: the 1258l finisher was a killing blow (4th wound after a blocked hit), not the counter-kill. '
        + (f'Emily counter-kill confirmed ({COUNTER_TAKE}). ' if COUNTER_TAKE else 'Emily counter-kill (parry win then attack) not yet produced. ')
        + ('Emily counter visual accepted. ' if ACCEPTED_COUNTER else ''))
it['notes'] = note + (it.get('notes') or '')
it['runtime_binding'] = 'native_counter_kill_finisher_shared_both_roles' if COUNTER_TAKE else 'partial_native_counter_kill_finisher_shared_host_source'
if ACCEPTED_COUNTER: it['visual_validation'] = 'accepted_functional_both_roles_counter_finisher_presented'
ev = '../movement-integration/finisher-follow-audit-v1281.json'
if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(p, an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = m['current'].replace('1214ah live', '1214ak live') + ' Client sword finishers follow on the host and the client ragdoll no longer launches (1214ai-1214ak, audit 1281).'
(R / 'work/current-task-state.json').write_bytes(json.dumps(s, indent=2, ensure_ascii=True).replace('\n', '\r\n').encode('utf-8'))
for script, cwd in ((N / 'build_review_catalog.py', N), (R / 'work/regen_movement_requirements.py', R)):
    out = subprocess.run([sys.executable, '-X', 'utf8', str(script)], cwd=cwd, capture_output=True, text=True)
    print(out.stdout[-200:], out.stderr[-300:]); assert out.returncode == 0
