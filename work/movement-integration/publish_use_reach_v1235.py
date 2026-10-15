"""Audit 1235 (#6): hand targeting on the vise - takes 1234 (harness bug) and 1234b (pass), candidate 1213k.
Adds the observed videos with reviews (contact-sheet inspection) to the runtime video manifest, notes the
catalog item, writes the audit and rebuilds the review catalog. Candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1213k'
TAKES = ['use-reach-eotw-v1234', 'use-reach-eotw-v1234b']
REVIEW = {
    'use-reach-eotw-v1234/use-host-env_vise_01_1_3-observed.mp4': (True, ['lever_button_crank_wheel'],
        'Client view of remote Corvo (host source, 1213k, hand reach on), Edge of the World bench vise, daylight, side view: steps in, both hands on the vise screw handle cranking 2.5-5 s, recovers. Natural; reach applied at full weight through the contact window (target = vise entity origin behind the handle, arm at full extension toward it).'),
    'use-reach-eotw-v1234/use-client-env_vise_01_1_3-observed.mp4': (False, [],
        'Host view of remote Emily (client source): a wandering worker NPC occludes her for most of the clip. Runtime-bound; not reviewable.'),
    'use-reach-eotw-v1234b/use-client-env_vise_01_1_3-observed.mp4': (True, ['lever_button_crank_wheel'],
        'Host view of remote Emily (client source, 1213k), mirrored side, open sea, daylight: approaches the bench, right arm extended over it to the vise front screw 2.5-4.5 s, recovers at 6 s. Natural crank reach; the hand stops near the screw (target = entity origin behind it, full extension).'),
    'use-reach-eotw-v1234b/use-host-env_vise_01_1_3-observed.mp4': (False, [],
        'Client view of remote Corvo (host source, 1213k): not separately reviewed; the 1234 host video is the accepted Corvo reach.'),
}
cases = []
for take in TAKES:
    r = rd(N / take / 'result.json'); cases.append(dict(take=f'../natural-moveset/{take}', candidate=CAND, passed=r['passed'], failure=r.get('failure'),
        restoration=rd(N / take / 'restoration.json')['passed'], cases=r.get('cases')))
videos = rd(N / 'runtime-video-manifest.json')
for path, (acc, reqs, review) in REVIEW.items():
    f = N / path
    if not f.exists(): continue
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/'))
    src = 'host' if '-host-' in f.name else 'client'; obs = 'client' if src == 'host' else 'host'
    take = path.split('/')[0]; res = rd(N / take / 'result.json'); case = next((c for c in res.get('cases') or [] if c.get('source') == src), {})
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role=obs, character='corvo' if obs == 'client' else 'emily',
        label=f'Interaction / native use of env_vise_01_1_3 -> crank + hand reach (source {src}) / {take}', travelling=False, gameplay_bound=True,
        actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Observer window of the remote partner while the source natively uses the bench vise; the partner pulls its right wrist toward the used entity (hand reach, 1213k).',
        review=review, candidate=CAND, dll=rd(W / CAND / 'identity.json')['dll'], runtime_test_passed=bool(case.get('passed')), visual_accepted=acc, requirements=reqs if acc else []))
wr(N / 'runtime-video-manifest.json', videos)
at = datetime.datetime.now().astimezone().isoformat()
wr(W / 'use-reach-audit-v1235.json', dict(at=at, candidate=CAND, cases=cases,
    findings=['Confirmed runtime (1234b, pass): the source sends the focused entity origin with the use (protocol 75) and the partner applies the right-hand reach in both roles (88/131 frames, no refusal, full weight through the contact window).',
              'The vise entity origin sits behind its screw handle and beyond a fully extended arm at a 1 m stand-off (closest approach 0.52/0.69 m); the arm extends toward it and the hands meet the screw handle in both videos.',
              'Visual: Corvo (1234) and Emily (1234b) crank reaches accepted on real footage.'],
    complete=True, scope='#6 hand targeting to the used mechanism (entity origin); exact per-handle contact points remain approximate.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); it = an['items']['lever_button_crank_wheel']
note = ' Audit 1235 (#6, 1213k): hand reach toward the used entity origin, both roles on the vise; Corvo and Emily crank reaches visually accepted.'
it['notes'] = (it.get('notes') or '').split(' Audit 1235')[0] + note
if '../movement-integration/use-reach-audit-v1235.json' not in it.setdefault('evidence', []): it['evidence'].append('../movement-integration/use-reach-audit-v1235.json')
wr(p, an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at
s['MovementExecutionPriority']['current'] = ('1213k live (protocol 75): use variants on real mechanisms in both roles with a right-hand reach toward the used entity (#4, #6); '
    '#5 horizontal-only end; presentation recovery: pause ~1.6 s, modal 5.1 s (#10, #34, #35).')
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
