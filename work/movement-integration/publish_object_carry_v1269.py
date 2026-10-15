"""Audit 1269 (#11 carry_object_place_throw): holding and throwing a native movable object (takes 1269a-c, candidate 1214m).
Writes the audit, reviewed videos and the annotation, and rebuilds the catalog. Candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1214m'
rows = []
for t in ('object-carry-v1269a', 'object-carry-v1269b', 'object-carry-v1269c'):
    f = N / t / 'result.json'
    if f.exists(): r = rd(f); rows.append(dict(take=f'../natural-moveset/{t}', passed=r.get('passed'), failure=r.get('failure'),
                     cases=[{k: x.get(k) for k in ('source', 'case', 'passed', 'failure', 'activeSets', 'peakUseWeight')} for x in r.get('cases', [])]))
REVIEW = {'object-carry-v1269c/host-bottle-observer.mp4': ('corvo', 'Client window: remote Corvo reaches out and holds his arm forward at chest height while the source holds the bottle, then finishes the reach when it is thrown and returns to the sword idle.'),
          'object-carry-v1269c/client-bottle-observer.mp4': ('emily', 'Host window: remote Emily holds the same reach while carrying the bottle and finishes it on the throw.')}
videos = rd(N / 'runtime-video-manifest.json'); dll = rd(W / CAND / 'identity.json')['dll']
for path, (character, review) in REVIEW.items():
    f = N / path
    if not f.exists(): continue
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/'))
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role='client' if character == 'corvo' else 'host', character=character, label=f'Object hold and throw (1214m) / {path.split("/")[0]}', travelling=False, gameplay_bound=True,
        actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup', scope='Partner view while the source holds and throws a native movable bottle.',
        review=review + ' Functional: the held object itself is not drawn in the partner\'s hand and the thrown bottle is not shared between worlds (world object sync, stage 5).', candidate=CAND, dll=dll,
        runtime_test_passed=True, visual_accepted=True, requirements=['carry_object_place_throw']))
wr(N / 'runtime-video-manifest.json', videos)
at = datetime.datetime.now().astimezone().isoformat()
findings = ['Confirmed runtime (1269b, 1214l): picking up a native movable (esc_env_bottle_03_3) adds HoldObject49 (RVA 1c7a998, arkPlayerStateHoldObject) over Running0, with Undraw81 while the sword is put away; RT throws it and returns to Running0. The source classified none of it, dropped every sample (50 stage-2 refusals) and the partner presentation weight fell to 0 for the whole hold.',
            'Confirmed runtime (1269c, 1214m): HoldObject is a ground action with valid root motion; the partner stays fully presented and shows use variant 6 (the generic reach held at its contact pose; legs keep the locomotion gait when moving) for the whole hold, then variant 7 (the reach finished in 1.3 s) on the throw, in both roles. Brief presentation dips at the pickup/release instants.',
            'Open: the held object is not drawn in the partner\'s hand and thrown or placed movables are not shared between worlds (world object sync, stage 5).']
wr(W / 'object-carry-audit-v1269.json', dict(at=at, candidate=CAND, takes=rows, findings=findings, complete=False))
an = rd(N / 'review-evidence-annotations.json'); it = an['items']['carry_object_place_throw']
it['runtime_binding'] = 'partial_native_hold_object_gesture_both_roles_product'; it['visual_validation'] = 'accepted_functional_both_roles_held_reach'
it['notes'] = ('Audit 1269 (1214m): holding a native movable (HoldObject49) presents a held reach and its throw finishes it, both roles; the object itself is not drawn in the partner\'s hand and movables are not world-synced yet. ' + (it.get('notes') or ''))
ev = '../movement-integration/object-carry-audit-v1269.json'
if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
wr(N / 'review-evidence-annotations.json', an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at
s['MovementExecutionPriority']['current'] = ('1214m live (protocol 75, bank 1266): held native objects present a held reach and throw; grenade release pop fixed; pickups, food and elixirs present gestures; '
                                             'counter-kill finishers shared; every native takedown and finisher scene is a paired phase; visual sweep 1260 and fixes 1267.')
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
