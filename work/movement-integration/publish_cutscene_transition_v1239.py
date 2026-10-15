"""Audit 1239 (#8): cutscene_enter_exit - the partner presentation into and out of the native Mayhew scene
(Cinematic5) in the Dunwall fixture, takes 1236 (gate too strict) and 1236b (pass), candidate 1213l. Writes
the audit, the reviewed videos and the catalog annotation, then rebuilds the review catalog."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1213l'
takes = []
for t in ('cutscene-transition-v1236', 'cutscene-transition-v1236b'):
    r = rd(N / t / 'result.json'); takes.append(dict(take=f'../natural-moveset/{t}', candidate=CAND, passed=r['passed'], failure=r.get('failure'),
        restoration=rd(N / t / 'restoration.json')['passed']))
scene = rd(N / 'cutscene-transition-v1236b' / 'scene-result.json')
REVIEW = {
    'cutscene-transition-v1236b/scene-host.mp4': ('host', 'emily', 'Host window through the native Mayhew scene (letterboxed, Corvo comforts Mayhew and takes the blade at 22 s): no partner avatar intrudes on the scene; back in gameplay at 34 s remote Emily stands presented in front (masked, sword drawn).'),
    'cutscene-transition-v1236b/scene-client.mp4': ('client', 'corvo', 'Client window through the same native scene from Emily\'s hands: no partner intrusion; after the scene remote Corvo is presented in front (masked, sword drawn).'),
}
videos = rd(N / 'runtime-video-manifest.json')
for path, (role, character, review) in REVIEW.items():
    f = N / path
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/'))
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role=role, character=character, label=f'Lifecycle / native cutscene entry and exit (Mayhew, Cinematic5) / cutscene-transition-v1236b / {role}',
        travelling=False, gameplay_bound=True, actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Both players use Mayhew in the Dunwall fixture; the native skippable scene plays in full; the partner presentation retires for the scene and returns after it.',
        review=review, candidate=CAND, dll=rd(W / CAND / 'identity.json')['dll'], runtime_test_passed=True, visual_accepted=True, requirements=['cutscene_enter_exit']))
wr(N / 'runtime-video-manifest.json', videos)
at = datetime.datetime.now().astimezone().isoformat()
wr(W / 'cutscene-transition-audit-v1239.json', dict(at=at, candidate=CAND, takes=takes, scene=scene,
    findings=['Confirmed runtime (1236b, pass): both peers enter the native Mayhew scene together (0.52 s after use); during the 30 s scene the partner presentation is retired on both peers (follow and bank off in all 58 samples), so no partner stands in the scene; 1.57 s after the scene ends the full partner presentation is live again.',
              'The partner stream needs the native sword: from the pre-Mayhew save (before the scene hands it over) it never arms (1236, missing 8) - entry is gated on the avatar being present (follow + bank).',
              'Visual accepted on both windows: no partner intrusion in the native scene, partner presented right after it.'],
    complete=True, scope='cutscene_enter_exit on a real native scene (Mayhew, Cinematic5, played in full); other scenes and skipped playback follow the same lifecycle but are not separately recorded.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); it = an['items'].setdefault('cutscene_enter_exit', {})
it['runtime_binding'] = 'native_cinematic5_partner_retired_and_restored_both_roles'
it['visual_validation'] = 'accepted_mayhew_scene_both_windows'
note = ' Audit 1239 (#8, 1213l): both peers enter the native Mayhew scene together, the partner presentation retires for the scene and is live again 1.57 s after it; visual accepted on both windows.'
it['notes'] = (it.get('notes') or '').split(' Audit 1239')[0] + note
if '../movement-integration/cutscene-transition-audit-v1239.json' not in it.setdefault('evidence', []): it['evidence'].append('../movement-integration/cutscene-transition-audit-v1239.json')
wr(p, an)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
