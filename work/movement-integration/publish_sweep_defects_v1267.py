"""Audit 1267 (#11): fixes and missing views from visual sweep 1260 - grenade release pop (bank 1266, candidate 1214l;
takes 1265a/1266a), carry re-review from the newest v38 recordings, coverage takes 1267a (runs in 8 directions, slides,
both roles), Corvo-sourced NPC deaths from the 1254b takedown videos, crouched-heavy re-records (inconclusive). Writes the
audit, reviewed videos and annotations, and rebuilds the catalog. Candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1214l'
REVIEW = {  # path -> (requirement, character shown, candidate, review)
    'grenade-network-host-v1266a.mp4': ('grenade_equip_aim_throw', 'corvo', CAND, 'Client window: remote Corvo cooks and throws; with bank 1266 the release reads as a short step into the throw instead of a 0.75 m sideways jump (1265a, same build, old bank: pop still present).'),
    'grenade-network-client-v1266a.mp4': ('grenade_equip_aim_throw', 'emily', CAND, 'Host window: remote Emily cooks and throws with the short release step of bank 1266.'),
    'native-high-throw-corvo-regression-v38/native.mp4': ('carry_body_lift_idle_move_drop_throw', 'corvo', None, 'Re-review (sweep input had picked the old v13 take): Corvo bends to the body, lifts it through mid-air onto his shoulder, carries it and throws it with a lunge; the body lands and he stands.'),
    'native-high-throw-emily-regression-v38/native.mp4': ('carry_body_lift_idle_move_drop_throw', 'emily', None, 'Re-review: Emily crouches to the body, lifts it onto her shoulder, carries and throws it, then stands.'),
    'coverage-v1267a/client-run8-observer.mp4': ('run_8_directions', 'emily', CAND, 'Host window: remote Emily runs full-stick forward, back, both strafes and the diagonals with full running strides; no frozen or popping frames.'),
    'coverage-v1267a/host-run8-observer.mp4': ('run_8_directions', 'corvo', CAND, 'Client window: remote Corvo runs in the eight directions with full strides and clean direction changes.'),
    'coverage-v1267a/client-slide-observer.mp4': ('slide_enter_loop_exit', 'emily', CAND, 'Host window: remote Emily sprints in, drops into a low slide and ends crouched.'),
    'coverage-v1267a/host-slide-observer.mp4': ('slide_enter_loop_exit', 'corvo', CAND, 'Client window: remote Corvo sprints, slides with a leg-forward pose and ends crouched.'),
    'takedown-variants-v1254b/slide-host-lethal-observer.mp4': ('death_variants', 'corvo', 'candidate-npc-combat-v1214e', 'Client window: the guard killed by remote Corvo collapses and lies dead beside him (Corvo-sourced NPC death and ragdoll in the observer world).'),
    'takedown-variants-v1254b/drop-host-lethal-observer.mp4': ('ragdoll_handoff', 'corvo', 'candidate-npc-combat-v1214e', 'Client window: after remote Corvo\'s drop kill the guard goes down beneath him and lies as a corpse.'),
}
videos = rd(N / 'runtime-video-manifest.json'); byPath = {v['video']: v for v in videos}
for path, (req, character, cand, review) in REVIEW.items():
    f = N / path
    if not f.exists(): continue
    if path in byPath:
        m = byPath[path]; m['visual_accepted'] = True; m['review'] = ((m.get('review') or '') + ' Audit 1267: ' + review).strip()
        if req not in (m.get('requirements') or []): m.setdefault('requirements', []).append(req)
        continue
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/'))
    videos.append(dict(video=path, role='client' if character == 'corvo' else 'host', character=character, label=f'Coverage / {req} / {path.split("/")[0]}', travelling=req in ('run_8_directions', 'slide_enter_loop_exit'),
        gameplay_bound=True, actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Partner view in the other player\'s window.', review=review, candidate=cand, runtime_test_passed=True, visual_accepted=True, requirements=[req]))
wr(N / 'runtime-video-manifest.json', videos)
at = datetime.datetime.now().astimezone().isoformat()
rows = []
for t in ('grenade-network-host-v1265a', 'grenade-network-client-v1265a', 'grenade-network-host-v1266a', 'grenade-network-client-v1266a', 'crouched-heavy-corvo-v1265a', 'crouched-heavy-corvo-v1265b', 'gestures-v1261c', 'coverage-v1267a'):
    f = N / t / 'result.json'
    if f.exists(): r = rd(f); rows.append(dict(take=f'../natural-moveset/{t}', passed=r.get('passed'), failure=r.get('failure')))
findings = [
    'Confirmed static (bank 1212): the grenade release/recovery clips 256/257 (crouched 269/270) carry ~0.75 m (crouched ~0.45 m) of authored horizontal pelvis travel; with the network root still, the partner jumped sideways at release (1265a on 1214j: still present). Bank 1266 scales that travel to 25%, other payloads byte-identical; 1266a shows a short step into the throw. Candidate 1214l deploys bank 1266; product regression 1261c passes in both roles.',
    'Carry (re-review): the newest v38 recordings show a floor-to-shoulder lift, carry and throw for both characters; the sweep had judged the old v13 take because its input sorted videos oldest-first.',
    'Coverage 1267a (1214l, product controller): runs in 8 directions and sprint-to-slide with crouched exit in both roles; presentation stayed live (no weight drops) and the slide was presented; visuals accepted.',
    'Corvo-sourced NPC death and corpse are visible in the client window of the 1254b lethal takedowns; with the sweep-accepted Emily-sourced take (1117) both roles are covered.',
    'Crouched heavy (open): 1265a stopped on the old candidate pin; 1265b ran on 1214l but the source never held the charge, so the sweep\'s ~0.2 s standing snap (v636) is neither reproduced nor cleared; its restore hit an inventory mismatch and the lab was re-restored (checkpoint and note inventory; both inventories and health back to baseline).',
    'Still without a judgeable view: long fall loop, doors (partner hidden), key use as Corvo, map-load recovery as Corvo, kick (legs only).']
wr(W / 'sweep-defects-audit-v1267.json', dict(at=at, candidate=CAND, takes=rows, findings=findings, complete=False))
an = rd(N / 'review-evidence-annotations.json'); items = an['items']
def note(item, visual, text):
    it = items[item]; it['visual_validation'] = visual; it['notes'] = text + ' ' + (it.get('notes') or '')
    ev = '../movement-integration/sweep-defects-audit-v1267.json'
    if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
note('grenade_equip_aim_throw', 'accepted_functional_both_roles_bank_1266', 'Audit 1267: release pop fixed in bank 1266 (pelvis travel scaled to 25%); throw accepted from both windows.')
note('carry_body_lift_idle_move_drop_throw', 'accepted_functional_both_roles_v38', 'Audit 1267: lift, carry and throw accepted for both characters from the v38 recordings (the sweep had judged v13).')
note('run_8_directions', 'accepted_functional_both_roles', 'Audit 1267: runs in 8 directions accepted in both roles (coverage 1267a).')
note('slide_enter_loop_exit', 'accepted_functional_both_roles', 'Audit 1267: sprint-to-slide with crouched exit accepted for both characters (1267a); Corvo stand/jump exits accepted in sweep 1260.')
note('slide_to_crouch_or_stand', 'accepted_functional_both_roles', 'Audit 1267: crouched exits for both characters (1267a); Corvo stand and jump exits accepted in sweep 1260.')
note('death_variants', 'accepted_functional_both_roles', 'Audit 1267: Corvo-sourced NPC deaths seen in the client window (1254b); Emily-sourced in sweep 1260 (1117).')
note('ragdoll_handoff', 'accepted_functional_both_roles', 'Audit 1267: the killed guard\'s corpse in the observer world for both source roles (1254b, 1117).')
wr(N / 'review-evidence-annotations.json', an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at
s['MovementExecutionPriority']['current'] = ('1214l live (protocol 75, bank 1266): grenade release pop fixed; pickups, food and elixirs present partner gestures; counter-kill finishers shared; '
                                             'sword locks and b-swing hits presented; every native takedown and finisher scene is a paired phase (audit 1257); visual sweep 1260 and fixes 1267.')
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
