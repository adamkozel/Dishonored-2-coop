"""Audit 1238 (#7): window_enter_exit through a genuinely climbable window - probe 1231/1231b, takes
1235-1235d, diagnostics 1237-1237c, candidates 1213k/1213l. Writes the audit, the catalog annotation and
the reviewed videos, then rebuilds the review catalog. Candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
TAKES = {'climbable-windows-v1231': None, 'climbable-windows-v1231b': None, 'window-traversal-v1235': 'candidate-npc-combat-v1213k',
         'window-traversal-v1235b': 'candidate-npc-combat-v1213k', 'window-traversal-v1235c': 'candidate-npc-combat-v1213k',
         'window-mantle-states-v1237': 'candidate-npc-combat-v1213k', 'window-mantle-states-v1237b': 'candidate-npc-combat-v1213k',
         'window-mantle-states-v1237c': 'candidate-npc-combat-v1213k', 'window-traversal-v1235d': 'candidate-npc-combat-v1213l'}
rows = []
for t, c in TAKES.items():
    r = rd(N / t / 'result.json'); rest = rd(N / t / 'restoration.json') if (N / t / 'restoration.json').exists() else {}
    rows.append(dict(take=f'../natural-moveset/{t}', candidate=c, passed=r['passed'], failure=r.get('failure'), restoration=rest.get('passed')))
final = rd(N / 'window-traversal-v1235d' / 'result.json')
videos = rd(N / 'runtime-video-manifest.json')
for src in ('host', 'client'):
    path = f'window-traversal-v1235d/cross-{src}-observed.mp4'; f = N / path
    if not f.exists(): continue
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/')); obs = 'client' if src == 'host' else 'host'
    case = next(c for c in final['cases'] if c['source'] == src)
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role=obs, character='corvo' if obs == 'client' else 'emily',
        label=f'Traversal / window mantle through script_rich_small_01_sndctrl (source {src}) / window-traversal-v1235d', travelling=True, gameplay_bound=True,
        actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Observer window while the source mantles through a climbable Edge of the World window (native confined low-crouch mantle).',
        review='Observer camera stood outside in a stair alley with no line of sight to the window: only steps and wall in frame. Runtime-bound (partner traversal variant 2 at weight ~1.0); not reviewable (#33).',
        candidate='candidate-npc-combat-v1213l', dll=rd(W / 'candidate-npc-combat-v1213l' / 'identity.json')['dll'], runtime_test_passed=bool(case.get('passed')), visual_accepted=False, requirements=[]))
wr(N / 'runtime-video-manifest.json', videos)
at = datetime.datetime.now().astimezone().isoformat()
wr(W / 'window-traversal-audit-v1238.json', dict(at=at, takes=rows, final=final['cases'],
    findings=['Confirmed runtime (1231b): of 41 openable Edge of the World windows, script_rich_small_01_sndctrl, _10 and _18 are climbable with the native Mantle state (forward + jump crosses ~2 m); _16 crossed by slipping.',
              'Confirmed runtime (1237c): the window mantle is the native confined low-crouch mantle (kind 1, next 10, mantle_low_crouch.midnightscene, 500 ms, Scene_2) binding the player and the LEFT arm; the source reader accepted only a right arm for low climbs, so the partner showed a jump (1235c).',
              'Confirmed runtime (1235d, 1213l, passed): both roles mantle through the window and the partner presents the traversal (variant 2) at weight ~1.0; restoration passed.',
              'Visual: not accepted - the side-on observer spot had no line of sight to the window (#33).'],
    complete=True, scope='window_enter_exit runtime binding through a climbable window, both roles; visual acceptance tracked in #33.'))
p = N / 'review-evidence-annotations.json'; an = rd(p)
it = an['items'].setdefault('window_enter_exit', {})
it['runtime_binding'] = 'native_confined_low_mantle_through_climbable_eotw_window_both_roles'
note = ' Audit 1238 (#7, 1213l): native mantle through the climbable EotW window script_rich_small_01_sndctrl presented on the partner (traversal variant 2) in both roles; visual pending (#33).'
it['notes'] = (it.get('notes') or '').split(' Audit 1238')[0] + note
if '../movement-integration/window-traversal-audit-v1238.json' not in it.setdefault('evidence', []): it['evidence'].append('../movement-integration/window-traversal-audit-v1238.json')
wr(p, an)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
