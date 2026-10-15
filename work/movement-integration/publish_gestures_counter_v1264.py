"""Audit 1264 (#9, #11): counterattack (takes 1258a-f), sword locks and b-swing hits (1214h), combat-channel re-binding
(1214i), and the pickup, food and elixir gestures (1214j; takes 1261a/b, 1262a, 1263a). Writes the audit, reviewed
videos and catalog annotations, updates the live state and rebuilds the catalog. Candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1214j'
takes = ['counterattack-v1258a', 'counterattack-v1258b', 'counterattack-v1258c', 'counterattack-v1258d', 'counterattack-v1258e', 'counterattack-v1258f',
         'gestures-v1261a', 'gestures-v1261b', 'elixir-v1262a', 'native-potion-drink-host-v1263a', 'native-potion-drink-client-v1263a']
rows = []
for t in takes:
    f = N / t / 'result.json'
    if not f.exists(): continue
    r = rd(f); rest = (N / t / 'restoration.json')
    rows.append(dict(take=f'../natural-moveset/{t}', passed=r.get('passed'), failure=r.get('failure'), restoration=rd(rest).get('passed') if rest.exists() else None,
                     cases=[{k: x.get(k) for k in ('source', 'kind', 'case', 'passed', 'failure', 'sceneIds', 'outcome', 'peakUseWeight', 'itemTaken')} for x in r.get('cases', [])]))
REVIEW = {
    'gestures-v1261b/host-food-observer.mp4': ('eat_drink', 'client', 'corvo', 'Client window: remote Corvo stands at the table and reaches his hand to the plate as the grapes vanish in both worlds, then returns to idle.'),
    'gestures-v1261b/client-food-observer.mp4': ('eat_drink', 'host', 'emily', 'Host window: remote Emily reaches to the plate with her sword hand as the grapes are taken.'),
    'gestures-v1261b/host-coin-observer.mp4': ('pickup_loot_steal', 'client', 'corvo', 'Client window: remote Corvo reaches toward the coin as it is taken.'),
    'gestures-v1261b/client-coin-observer.mp4': ('pickup_loot_steal', 'host', 'emily', 'Host window: remote Emily reaches toward the coin with her sword hand as it is taken.'),
    'native-potion-drink-host-v1263a-receiver.mp4': ('health_elixir', 'client', 'corvo', 'Client window: remote Corvo raises his hand to his face for about 2 s and lowers it after the native health elixir is consumed (sword still in that hand).'),
    'native-potion-drink-client-v1263a-receiver.mp4': ('health_elixir', 'host', 'emily', 'Host window: remote Emily raises her hand to her face for about 2 s and lowers it after the native health elixir is consumed (sword still in that hand).'),
}
videos = rd(N / 'runtime-video-manifest.json'); dll = rd(W / CAND / 'identity.json')['dll']
for path, (req, role, character, review) in REVIEW.items():
    f = N / path
    if not f.exists(): continue
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/'))
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role=role, character=character, label=f'Gesture / {req} (1214j) / {path.split("/")[0]}', travelling=False, gameplay_bound=True,
        actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Partner view while the source performs the native pickup, food or elixir action.', review=review + ' Functional: the generic use reach (variant 1); a dedicated gesture is left for animator polish.',
        candidate=CAND, dll=dll, runtime_test_passed=True, visual_accepted=True, requirements=[req]))
wr(N / 'runtime-video-manifest.json', videos)
at = datetime.datetime.now().astimezone().isoformat()
findings = [
    'Confirmed runtime (1258b): a sword lock runs the exclusive Versus31 state (vtable RVA 1c405e8 from the native state table, 1258c) with the 4-actor scene combat/versus_low (versus_in 533, versus_loop 600, versus_mediumlose 1000 ms); before 1214h it fell to the unbound hold. 1214h presents versus_low/versus_elite (census 1259, ids 118/119) with the parry-idle clip; not yet observed at runtime on 1214h+.',
    'Confirmed runtime (1256a, 1258b): b-swing hit scenes sword_attack_right_b_hit/left_b_hit (Scene_1, 733 ms, player and right arm) fell to the unbound fallback; 1214h presents them as combat variants 22/23 with the slash-b clips.',
    'Confirmed runtime (1258d): after the client\'s combat channels retired (actor_unbound) a retired channel blocked its NPC forever; when the host re-armed (nonce 3) the client rejected all 48 hellos (receive_binding) and the host\'s counter-kill finisher was dead on the host and conscious on the client. 1214i re-binds a living NPC 2 s after a non-terminal retirement.',
    'Confirmed runtime (1258f, 1214i): Corvo\'s timed parry win then attack plays the counter-kill finisher fatality_head_fast_b (94), presented by the partner; dead in both worlds. Emily produced no counter in 1258a-f (parries and slashes only).',
    'Confirmed runtime (1261b, 1214j): coin and grapes pickups in both roles - the partner presents the use reach (weight 1.0, valid target) and the item is taken; LootObject47 lasts under the 80 ms sample interval (sampled at most once).',
    'Confirmed runtime (1262a/1263a, 1214j): the map health elixir pickup plays the reach (count 0 -> 1); no D-pad slot drinks it in this save; the native consume callback (isolated potion fixture, stream on) drinks it and the partner presents the hand-to-mouth reach ~1 s later (600 ms alternating poll) at weight 1.0 for ~2.3 s, both roles. The old harness then failed only its bank-continuity check after the drink.',
    'Visual: pickup, food and health-elixir gestures accepted from both windows as functional (generic reach, sword still in the reaching hand). Counter-kill video (1258f) too far to judge.']
wr(W / 'gestures-counter-audit-v1264.json', dict(at=at, candidate=CAND, takes=rows, findings=findings, complete=False,
    scope='#9 counterattack (Corvo runtime; Emily and sword-lock presentation unobserved), #11 pickup/food/health-elixir gestures (both roles); mana elixir same path unrecorded; note reading pose open.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); items = an['items']
def note(item, binding, visual, text):
    it = items[item]
    if binding: it['runtime_binding'] = binding
    if visual: it['visual_validation'] = visual
    it['notes'] = text + ' ' + (it.get('notes') or '')
    ev = '../movement-integration/gestures-counter-audit-v1264.json'
    if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
note('pickup_loot_steal', 'native_pickup_reach_presented_both_roles_product', 'accepted_functional_both_roles_generic_reach',
     'Audit 1264 (1214j): a native pickup (LootObject47) presents the generic use reach toward the item in both roles (coin and grapes, product controller, 1261b). Body loot and pickpocketing not yet recorded.')
note('eat_drink', 'native_food_reach_presented_both_roles_product', 'accepted_functional_both_roles_generic_reach',
     'Audit 1264 (1214j): eating native food (grapes, LootObject47) presents the reach to the plate in both roles while the item is taken in both worlds (1261b).')
note('health_elixir', 'native_health_elixir_drink_reach_presented_both_roles', 'accepted_functional_both_roles_hand_to_mouth',
     'Audit 1264 (1214j): a consumed health elixir (native consume, quantity drop) presents a hand-to-mouth reach about 1 s later in both roles (1263a).')
note('mana_elixir', None, None,
     'Audit 1264 (1214j): the mana elixir uses the same quantity-drop gesture path as the health elixir (kind 2); not yet recorded.')
note('counterattack', 'partial_native_counter_kill_finisher_shared_host_source', None,
     'Audit 1264: Corvo\'s timed parry win then attack plays a counter-kill finisher (fatality_head_fast_b) presented by the partner and dead in both worlds (1258f, 1214i). Sword locks (Versus31, versus_low/elite) are presented since 1214h but not yet observed; Emily\'s counter not yet produced.')
wr(p, an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = ('1214j live (protocol 75): pickups, food and elixirs present partner gestures; counter-kill finishers shared (1214i combat-channel re-binding); '
                'sword locks and b-swing hits presented (1214h); every native takedown and finisher scene is a paired phase (audit 1257); visual sweep 1260.')
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
