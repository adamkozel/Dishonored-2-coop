"""Audit 1257 (#11): takedown census and variants under the product controller - census 1253/1255 (confirmed static),
candidates 1214b-1214g, takes 1248k-1248n, 1254a-b and 1256a-h. Writes the audit, the reviewed videos and the catalog
annotations, updates the live state and rebuilds the review catalog. Candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1214g'
TAKES = {'paired-victim-product-v1248k': 'v1214b', 'paired-victim-product-v1248l': 'v1214c', 'paired-victim-product-v1248m': 'v1214c',
         'paired-victim-product-v1248n': 'v1214c', 'takedown-variants-v1254a': 'v1214c', 'takedown-variants-v1254b': 'v1214e',
         'fatality-v1256a': 'v1214d', 'fatality-v1256b': 'v1214e', 'fatality-v1256c': 'v1214f', 'fatality-v1256d': 'v1214g',
         'fatality-v1256e': 'v1214g', 'fatality-v1256f': 'v1214g', 'fatality-v1256g': 'v1214g', 'fatality-v1256h': 'v1214g'}
rows = []; clientFinisher = None
for t, c in TAKES.items():
    if not (N / t / 'result.json').exists(): continue
    r = rd(N / t / 'result.json'); rest = rd(N / t / 'restoration.json') if (N / t / 'restoration.json').exists() else {}
    cases = [{k: x.get(k) for k in ('approach', 'source', 'kind', 'passed', 'failure', 'scenes', 'sceneIds', 'outcome', 'victimGapM')} for x in r.get('cases', [])]
    rows.append(dict(take=f'../natural-moveset/{t}', candidate=f'candidate-npc-combat-{c}', passed=r['passed'], restoration=rest.get('passed'), cases=cases))
    if t.startswith('fatality') and c == 'v1214g':
        for x in cases:
            if x['source'] == 'client' and x['sceneIds']: clientFinisher = dict(take=t, **x)
REVIEW = {
    'drop-client-lethal': ('aerial_drop_kill', 'host', 'emily', 'Host window: remote Emily comes down on the guard, stabs with the sword extended as he folds under her, and stands over the body (assassination_drop_back, dead in both worlds, 0.0 m).'),
    'drop-host-lethal': ('aerial_drop_kill', 'client', 'corvo', 'Client window: remote Corvo comes down on the guard, arm raised, the guard goes down beneath him and Corvo stands over the body (dead in both worlds, 0.0 m).'),
    'drop-client-choke': ('aerial_nonlethal_takedown', 'host', 'emily', 'Host window: remote Emily lands behind the guard, holds him and lowers him; he ends unconscious at her feet (choke_drop_back, 0.01 m).'),
    'drop-host-choke': ('aerial_nonlethal_takedown', 'client', 'corvo', 'Client window: remote Corvo lands behind the guard and holds and lowers him; the guard ends lying at his feet (0.0 m).'),
    'slide-client-lethal': ('sliding_takedown', 'host', 'emily', 'Host window: remote Emily sprints from the foreground and slides low into the guard, stabs, and stands beside the body (assassination_slide_back, 0.0 m).'),
    'slide-host-lethal': ('sliding_takedown', 'client', 'corvo', 'Client window: remote Corvo slides low into the guard, stabs, and the guard falls beside him (0.0 m).'),
    'slide-client-choke': ('sliding_takedown', 'host', 'emily', 'Host window: remote Emily slides in from the foreground and holds and lowers the guard (choke_slide_back, 0.29 m).'),
    'slide-host-choke': ('sliding_takedown', 'client', 'corvo', 'Client window: remote Corvo slides in, the guard goes down and lies at his feet (0.0 m).'),
}
videos = rd(N / 'runtime-video-manifest.json'); dll = rd(W / 'candidate-npc-combat-v1214e' / 'identity.json')['dll']
for key, (req, role, character, review) in REVIEW.items():
    path = f'takedown-variants-v1254b/{key}-observer.mp4'; f = N / path
    if not f.exists(): continue
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/'))
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role=role, character=character, label=f'Paired / {key.replace("-", " ")} (product controller) / takedown-variants-v1254b',
        travelling=True, gameplay_bound=True, actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Held observer camera behind the guard; the source drops onto or slides into a host-owned guard with natural AI.', review=review + ' Functional: the existing fast-back stab or choke donor normalized to the native scene clock; drop- and slide-specific motion is left for animator polish.',
        candidate='candidate-npc-combat-v1214e', dll=dll, runtime_test_passed=True, visual_accepted=True, requirements=[req]))
wr(N / 'runtime-video-manifest.json', videos)
at = datetime.datetime.now().astimezone().isoformat()
findings = [
    'Confirmed static (census 1253/1255, generate_takedown_table_v1255.py): 156 native player takedown and fatality scenes; every scene has one subscene per player character with its own duration, and subscene names vary (Npc, npc, NPC, Scene_2, small, Hound...). 117 are in the generated table (209 exact subscene/duration pairs); carry-corpse follow-ups, drop-ready poses and the fatality double kill are not takedowns.',
    'Confirmed runtime (1248j/1248k/1248l): assassination_back runs 1933 ms for Corvo in Assassination13 and is presented as id 6 (dead in both worlds, 0.0-0.1 m); 1214b refused Emily\'s 2200 ms version (static census entry; not yet observed at runtime on 1214c+).',
    'Confirmed runtime (1254a/1254b): drop choke (choke_drop_back, id 58) and drop lethal (assassination_drop_back, 19), slide choke (choke_slide_back, 73) and slide lethal (assassination_slide_back, 33) are presented and shared in both source roles - same outcome in both worlds, victims 0.0-0.29 m apart. Side chokes choke_left (68) and choke_right (69) for Emily, front choke choke_front (62) for Corvo.',
    'Confirmed runtime (1254a): slide takedowns keep physical stance 5 inside Assassination13/Choke41 (1214d admits it, with valid root motion and a standing presentation stance); 1214c capped PairedKnockout at id 3, so the client slide choke never asked the host for the knockout (fixed in 1214d).',
    'Confirmed runtime (1256a-1256e): four-binding sword finishers run in exclusive Assassination13 and bind NPC, player, two arms and one empty slot (1214e/1214f); fatality_torso_fast_b (113), fatality_head_medium_a (96) and fatality_rightleg_fast_a (109) are presented for the host source, dead in both worlds, 0.0 m. Generic right-arm finishers (fatality_generic_right, 92) run in Slash39 over Running0; 1214g exempts them from the combat-scene requirement that dropped the source sample.',
    'Visual (contact sheets, both windows): drop and slide chokes and lethals read functionally in both roles; the motion is the existing fast-back stab or choke donor normalized to each scene clock.',
    'Fixture limitation: a standing attacker beside or in front of a guard with natural AI is noticed, so side and front lethals become sword fights; Corvo\'s side approaches found no takedown.']
clientPass = [(r['take'].split('/')[-1], x['sceneIds'][0]) for r in rows if 'fatality-v1256' in r['take'] for x in r['cases'] if x['source'] == 'client' and x['passed']]
findings.append('Confirmed runtime (1214g, client source): ' + ', '.join(f'{t} finisher id {i}' for t, i in clientPass) + ' presented by the host, dead in both worlds, 0.0 m (generic id 92 and four-binding finishers alike); in the other 1214g client takes Emily killed with plain slashes.')
wr(W / 'takedown-variants-audit-v1257.json', dict(at=at, candidate=CAND, takes=rows, findings=findings, complete=False,
    scope='#11 takedown variants: drop and slide takedowns and sword finishers bound in both roles; side/front, window/wall and body sizes remain partial.'))
p = N / 'review-evidence-annotations.json'; an = rd(p)
def note(item, binding, visual, text):
    it = an['items'][item]
    if binding: it['runtime_binding'] = binding
    if visual: it['visual_validation'] = visual
    it['notes'] = text + ' ' + it.get('notes', '') if not it.get('notes', '').startswith('Audit 1257') else text
    ev = '../movement-integration/takedown-variants-audit-v1257.json'
    if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
note('aerial_drop_kill', 'native_drop_lethal_presented_and_shared_both_roles_product', 'accepted_functional_both_roles_fast_back_donor',
     'Audit 1257 (1214e+, product controller, natural guard AI): assassination_drop_back (census id 19) is presented by the partner and the death is shared in both source roles (victims 0.0 m apart). Visual accepted from both windows as functional (fast-back stab donor normalized to the native clock).')
note('aerial_nonlethal_takedown', 'native_drop_choke_presented_and_knockout_shared_both_roles_product', 'accepted_functional_both_roles_choke_donor',
     'Audit 1257: choke_drop_back (census id 58) is presented and the knockout shared in both source roles (the client asks the host for the knockout when the scene begins; 0.0-0.01 m). Visual accepted from both windows as functional (choke donor).')
note('sliding_takedown', 'native_slide_choke_and_lethal_presented_and_shared_both_roles_product', 'accepted_functional_both_roles_choke_and_lethal',
     'Audit 1257: choke_slide_back (73) and assassination_slide_back (33) are presented and shared in both source roles; slide takedowns keep physical stance 5 inside the paired state (0.0-0.29 m). Visual accepted from both windows as functional.')
note('lethal_takedown_front_back_sides', 'partial_census_lethals_back_drop_slide_finishers_both_roles', None,
     'Audit 1257: every census-1255 lethal scene is admitted by exact asset/subscene/duration; runtime: fast-back and assassination_back (Corvo), drop and slide in both roles, host-sourced four-binding sword finishers. Side and front lethals were not reached by the fixture (the guard notices a standing attacker).')
note('nonlethal_takedown_front_back_sides', 'partial_census_chokes_back_both_roles_sides_emily_front_corvo', None,
     'Audit 1257: rear choke in both roles; side chokes choke_left/choke_right presented for Emily (ids 68/69, then loop and win, knockout shared); front choke choke_front for Corvo (62). Corvo\'s side and Emily\'s front chokes not yet reached by the fixture.')
note('death_variants', None, None,
     'Audit 1257: sword finishers (census-1255 fatalities) are presented as paired lethal scenes and the death is shared, 0.0 m, in both source roles: host 96, 109, 113; client generic 92 and four-binding 95.')
wr(p, an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = ('1214g live (protocol 75): every native takedown and sword finisher scene (census 1255, 117 scenes) is a paired partner phase; '
                'drop and slide takedowns bound and shared in both roles (audit 1257); paired takedowns shared under the product controller (audit 1252).')
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
