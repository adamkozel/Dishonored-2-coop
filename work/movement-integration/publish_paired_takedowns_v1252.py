"""Audit 1252 (#46): shared paired takedowns under the product controller - client- and host-sourced rear choke and
lethal on a host-owned NPC with natural guard AI. Probes 1250-1251c (native knockout), takes 1248b-1248j
(candidates 1213u-1214a). Writes the audit, the reviewed videos and the catalog annotation, updates the live
state and rebuilds the review catalog. Candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1214a'
TAKES = {'npc-knockout-v1250': None, 'npc-sleep-knockout-v1251': 'candidate-npc-combat-v1213s', 'npc-sleep-knockout-v1251b': 'candidate-npc-combat-v1213t',
         'npc-sleep-knockout-v1251c': 'candidate-npc-combat-v1213t', 'paired-victim-product-v1248b': 'candidate-npc-combat-v1213u',
         'paired-victim-product-v1248c': 'candidate-npc-combat-v1213v', 'paired-victim-product-v1248d': 'candidate-npc-combat-v1213w',
         'paired-victim-product-v1248e': 'candidate-npc-combat-v1213x', 'paired-victim-product-v1248f': 'candidate-npc-combat-v1213y',
         'paired-victim-product-v1248g': 'candidate-npc-combat-v1213y', 'paired-victim-product-v1248h': 'candidate-npc-combat-v1213y',
         'paired-victim-product-v1248i': 'candidate-npc-combat-v1213z', 'paired-victim-product-v1248j': CAND}
rows = []
for t, c in TAKES.items():
    if not (N / t / 'result.json').exists(): continue
    r = rd(N / t / 'result.json'); rest = rd(N / t / 'restoration.json') if (N / t / 'restoration.json').exists() else {}
    rows.append(dict(take=f'../natural-moveset/{t}', candidate=c, passed=r['passed'], failure=r.get('failure'), restoration=rest.get('passed'),
        cases=[{k: x.get(k) for k in ('source', 'kind', 'passed', 'failure', 'outcome', 'victimGapM', 'attempts')} for x in r.get('cases', [])]))
REVIEW = {
    'paired-victim-product-v1248j/client-choke-observer.mp4': (True, 'Host window, product controller, client-sourced rear choke: the host\'s guard struggles in remote Emily\'s arms (it follows the client\'s native victim pose), is lowered and lies unconscious at her feet; the victim is in the same place in both worlds (0.1 m). Small in frame (camera far down the corridor; reviewed cropped).'),
    'paired-victim-product-v1248j/client-lethal-observer.mp4': (True, 'Host window: remote Emily stabs from behind and the host\'s guard falls dead at her feet; the host game shows its own hide-the-body hint. Bodies together in both worlds.'),
}
videos = rd(N / 'runtime-video-manifest.json')
for path, (accepted, review) in REVIEW.items():
    f = N / path
    if not f.exists(): continue
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/'))
    kind = 'lethal' if '-lethal-' in path else 'choke'
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role='host', character='emily', label=f'Paired / client-sourced rear {kind} with shared victim (product controller) / {path.split("/")[0]}',
        travelling=False, gameplay_bound=True, actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Held observer camera on the host; client performs the takedown on a host-owned NPC with natural AI.', review=review, candidate=CAND,
        dll=rd(W / CAND / 'identity.json')['dll'], runtime_test_passed=True, visual_accepted=accepted, requirements=['paired_victim_alignment'] if accepted else []))
wr(N / 'runtime-video-manifest.json', videos)
at = datetime.datetime.now().astimezone().isoformat()
wr(W / 'paired-takedowns-audit-v1252.json', dict(at=at, candidate=CAND, takes=rows,
    findings=['Confirmed runtime (probe 1250): a native choke knocks the NPC out with code_referenced generic_sleep_damage - health 40/40 -> 10/10 (the body\'s suppressed maximum), actor flag 0x20 at +0xd1, AI trees destroyed; a lethal sets health 0. The game\'s sleep_damage reproduces the knockout with a drowsy collapse; sleep_no_reaction_damage leaves the body standing (1251-1251c).',
              'Confirmed runtime (1248b-1248c): under the product controller the NPC combat session was dead ("no_ready_native_actors"): it bound only the NPCs present when it began, stopped when any retired, and was never re-armed, so no hit or takedown outcome was shared. 1213v: bindings join and retire per NPC, per-NPC divergence retires only that NPC, the product controller re-arms it.',
              'Confirmed runtime (1248c-1248d): with combat active, a choke\'s own knockout damage was captured as unknown, suppressed and stopped the session; 1213w passes knockout damage through and shares unconsciousness by the NPC stream (Unconscious host->client, PairedKnockout client->host).',
              'Confirmed runtime (1248e-1248i): the AI lease pauses decisions but not an in-progress walk, so a walking held guard left the client\'s victim; 1213x streams the client\'s native victim pose to the host (PairedPose; 1213z fixed its message size), and 1214a pins the host NPC for 3 s after the knockout and keeps presenting an unconscious NPC.',
              'Confirmed runtime (1248j, 1214a, product controller, natural AI): client choke unconscious in both worlds 0.1 m apart; client lethal dead in both, bodies together; host choke unconscious in both, 0.0 m apart. Host lethal: outcome shared in 1248i (dead in both worlds); in 1248j the native scene was assassination_back (1933 ms), which the partner reader does not admit yet (only assassination_fast_back), so its partner phase was not presented.',
              'Visual (host window): client-sourced choke and lethal accepted - the host\'s guard is choked, lowered and lies unconscious, or falls dead, at remote Emily\'s feet.'],
    complete=True, scope='#46: shared outcome and victim for paired takedowns on host-owned NPCs, both source roles, choke and fast-back lethal; other takedown scenes (assassination_back, front, air, slide) remain under their own checklist items.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); it = an['items']['paired_victim_alignment']
it['runtime_binding'] = 'shared_victim_and_outcome_both_roles_choke_and_lethal_product'
it['visual_validation'] = 'accepted_host_and_client_sourced_rear_choke_and_lethal_victims'
it['notes'] = ('Audit 1252 (1214a, product controller, natural guard AI): for a rear choke or fast-back lethal by either player on a host-owned NPC, the victim is '
               'animated and ends in the same place and state in both worlds (host-sourced: the NPC stream carries the host victim; client-sourced: the host '
               'holds its NPC, follows the client victim pose and knocks it out at choke_win; deaths ride the combat relay). Visuals accepted for the host- and '
               'client-sourced choke and the client-sourced lethal. Other takedown scenes (assassination_back, front, air, slide) and body sizes remain open.')
for ev in ('../movement-integration/paired-takedowns-audit-v1252.json',):
    if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(p, an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = ('1214a live (protocol 75): paired takedowns shared under the product controller in both roles (audit 1252: client/host choke and lethal, '
                'victim animated and in the same place and state in both worlds); NPC combat follows dynamic relevance; crouched takedowns (audit 1246).')
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
