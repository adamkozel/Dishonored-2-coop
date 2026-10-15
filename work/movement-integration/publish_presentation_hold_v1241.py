"""Audit 1241 (#11): one product-aware presentation hold (candidate 1213m), measured on the Edge of the World
window mantle - take 1235d (1213l) against take 1235e (1213m), same harness under the product controller.
Binds scripted_scene_pose_handoff partially (measured native-scene-to-locomotion blend, both roles) and notes
window_enter_exit. Writes the audit, the video manifest entries and the catalog annotations, updates the live
state, then rebuilds the review catalog. Candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
sys.path.insert(0, str(W)); from presentation_gap_metrics import metrics
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1213m'
takes = {'window-traversal-v1235d': 'candidate-npc-combat-v1213l', 'window-traversal-v1235e': CAND}
rows = []
for take, cand in takes.items():
    r = rd(N / take / 'result.json')
    rows.append(dict(take=f'../natural-moveset/{take}', candidate=cand, passed=r['passed'], restoration=rd(N / take / 'restoration.json')['passed'],
        cases=[{k: c.get(k) for k in ('source', 'observer', 'passed', 'crossed', 'mantle', 'maxTraversalWeight')} for c in r['cases']],
        gaps={src: metrics(N / take / f'cross-{src}-observer-movement.ndjson') for src in ('host', 'client')}))
def handoff(take, src):
    """Scene-to-locomotion handoff on the observer: traversal weight and locomotion weight after the scene's last frame."""
    out = []
    for line in (N / take / f'cross-{src}-observer-movement.ndjson').read_text(encoding='utf-8').splitlines():
        s = json.loads(line)['status']
        if s.get('traversalVariant') == 2: out.append(dict(ms=json.loads(line)['ms'], traversalWeight=round(s['traversalWeight'], 3), traversalAmount=round(s['traversalAmount'], 3), locomotionWeight=round(s['weight'], 3)))
    return out[:14]
for row in rows:
    take = row['take'].split('/')[-1]; row['handoff'] = {src: handoff(take, src) for src in ('host', 'client')}
videos = rd(N / 'runtime-video-manifest.json')
for src in ('host', 'client'):
    path = f'window-traversal-v1235e/cross-{src}-observed.mp4'; f = N / path
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/')); obs = 'client' if src == 'host' else 'host'
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role=obs, character='corvo' if obs == 'client' else 'emily',
        label=f'Traversal / window mantle with the 1213m presentation hold (source {src}) / window-traversal-v1235e', travelling=True, gameplay_bound=True,
        actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Observer window while the source mantles through the climbable Edge of the World window under the product controller (1213m hold).',
        review='Same observer spot as 1235d: the camera faces the stair alley and only the partner\'s legs show at the top edge. Runtime-bound; not reviewable (#33).',
        candidate=CAND, dll=rd(W / CAND / 'identity.json')['dll'], runtime_test_passed=True, visual_accepted=False, requirements=[]))
wr(N / 'runtime-video-manifest.json', videos)
d, e = rows[0]['gaps'], rows[1]['gaps']
at = datetime.datetime.now().astimezone().isoformat()
wr(W / 'presentation-hold-audit-v1241.json', dict(at=at, candidate=CAND, takes=rows,
    findings=[f"Confirmed runtime (1235d, 1213l, product controller): with the host as source the client's partner was in its native pose for {d['host']['nativePoseFraction']:.0%} of the samples, dropping {d['host']['presentedToNativeDrops']} times in {d['host']['seconds']} s. Each drop was a source state gap of 250-375 ms (eligibility mask 16, then mask 1 while the semantic timeline refilled) or the root snap after one (a 12-18 m/s step rejected by the gait clock).",
              'Confirmed static: every presentation timeline (movement semantics and 12 action timelines) invalidated its last sample after 200 ms, cleared on a 250 ms gap and extrapolated actions by at most 100 ms, and mask 16 used a fixed 250 ms; the product controller had relaxed only the stream lease (1 s, 1203d).',
              f"Confirmed runtime (1235e, 1213m, same harness, passed, restoration passed): native-pose time {e['host']['nativePoseFraction']:.0%} (host source) and {e['client']['nativePoseFraction']:.0%} (client source), {e['host']['presentedToNativeDrops']} and {e['client']['presentedToNativeDrops']} drops; host-source root-step rejections {d['host']['rootStepRejections']} -> {e['host']['rootStepRejections']}.",
              'Scene-to-locomotion handoff (1235e): the mantle scene pose is presented at full weight to amount 0.977, then the traversal weight decays (0.34, 0.07, 0.02 over ~250 ms) while locomotion ramps back (0.49, 0.89, 0.97): a measured blend from the actual scene pose, in both roles.',
              'Remaining: one frame of the protocol-73 hold (state 24, an unclassified native state) at the mantle end still drops to the native pose for one frame and ramps back; the lab keeps its strict rules outside the product controller.',
              'Visual: not reviewable - the observer spot faces the stair alley (#33).'],
    complete=True, scope='Product-controller presentation continuity across source gaps up to 1 s, measured on the window mantle in both roles; other maps and longer outages are not separately measured.'))
p = N / 'review-evidence-annotations.json'; an = rd(p)
it = an['items']['scripted_scene_pose_handoff']
it['runtime_binding'] = 'partial_native_scene_exit_blend_to_locomotion_measured_window_mantle_both_roles'
it['notes'] = ('A context-dependent animation ownership transition. Audit 1241 (1213m): on the native confined mantle the partner holds the scene pose at full weight, '
               'then blends from that actual pose to locomotion over ~250 ms (traversal weight 0.34/0.07/0.02 while locomotion returns 0.49/0.89/0.97), in both roles. '
               'Use scenes recover the same way (vise reach videos, audit 1235). Other scene families, an unclassified-state frame at the scene end and visual acceptance (#33) remain open.')
for ev in ('../movement-integration/presentation-hold-audit-v1241.json', '../movement-integration/window-traversal-audit-v1238.json', '../movement-integration/use-reach-audit-v1235.json'):
    if ev not in it.setdefault('evidence', []): it['evidence'].append(ev)
it = an['items']['window_enter_exit']
note = ' Audit 1241 (1213m): the same mantle passes again in both roles with the partner held through source gaps (native-pose time 29% -> 1% on the host-source side).'
it['notes'] = (it.get('notes') or '').split(' Audit 1241')[0] + note
if '../movement-integration/presentation-hold-audit-v1241.json' not in it.setdefault('evidence', []): it['evidence'].append('../movement-integration/presentation-hold-audit-v1241.json')
wr(p, an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = ('1213m live (protocol 75): product-controller presentation hold across source gaps up to 1 s (audit 1241: native-pose time 29% -> 1%); '
                'use variants with hand reach (#4, #6); window mantle (#7) and cutscene entry/exit (#8) bound; checklist: sit_stand not applicable (audit 1240).')
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
