"""Audit 1284 (#61): lab free observer camera (coop_observer_camera hold_at, 1214an) and harness 1228n (-FreeCam), verified
by take use-variants-dunwall-v1228u. Writes the audit and updates chapter 92 and the current state. Takes are never
rewritten."""
from pathlib import Path
import json, datetime
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
D = R / 'documentation/handoff-2026-10-11'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def edit(path, old, new):
    s = path.read_bytes().decode('utf-8')
    if '\r\n' in s: old = old.replace('\n', '\r\n'); new = new.replace('\n', '\r\n')
    assert s.count(old) == 1, (path.name, s.count(old), old[:80]); path.write_bytes(s.replace(old, new).encode('utf-8'))
at = datetime.datetime.now().astimezone().isoformat()
t = 'use-variants-dunwall-v1228u'; r = rd(N / t / 'result.json')
cams = {f.name: rd(f) for f in (N / t).glob('observer-camera-*.json')}
audit = dict(at=at, candidate='candidate-npc-combat-v1214an', take=t,
    cases=[dict(source=c.get('source'), target=c.get('target'), passed=c.get('passed'), shownSamples=c.get('shownSamples'), observerBody=c.get('observerBody')) for c in r.get('cases', [])],
    cameras={k: dict(result=v.get('result'), position=v.get('position')) for k, v in cams.items()},
    findings=[
        'Confirmed static: the lab observer camera already hooks the native render-view setter (game +0x890c50), the function the IGCS Dishonored 2 notes describe (view position at +A8/+C0, 3x3 basis at +CC); IGCS (BSD 2-Clause) was read as a reference only.',
        'Feature (1214an): coop_observer_camera hold_at x y z holds the current native view orientation with the eye at a world point; the observer body stays where it stands. Lab command channel only, like hold.',
        'Confirmed runtime (use-variants-dunwall-v1228u, harness 1228n -FreeCam): ring lock and office door pass in both roles; every case held its camera at the computed spot (holding_at) and released it afterwards; the lock views show the partner at the wall lock from behind (a candelabra overlaps the lower body).',
        'IGCS current Dishonored 2 camera uses the native Ansel session, which pauses the game - not usable for motion videos (confirmed static from its notes).'],
    complete=True)
(W / 'free-camera-audit-v1284.json').write_text(json.dumps(audit, indent=2), encoding='utf-8')
edit(D / '92-native-use-interactions.md',
"`-Only` selects the Dunwall cases (1228l); 1228m samples both peers' shadow-stream status per case.\n",
"""`-Only` selects the Dunwall cases (1228l); 1228m samples both peers' shadow-stream status per case.

**Free observer camera (#61, 1214an, [audit 1284](../../work/movement-integration/free-camera-audit-v1284.json)):** `coop_observer_camera hold_at x y z` holds the current native view orientation with the eye at any world point. The observer's body stays where it stands. It is lab-only, on the same command channel as `hold`.
- **How it works (confirmed static):** the camera hooks the native render-view setter (+0x890c50), the function the [IGCS Dishonored 2 notes](https://github.com/FransBouma/InjectableGenericCameraSystem/tree/master/Cameras/Dishonored2) describe. IGCS was read as a reference only. Its current camera uses the native Ansel mode, which pauses the game.
- **Harness 1228n `-FreeCam`:** the observer turns toward the use where it stands, takes the pitch, and the camera is held at the computed spot (`-Back`, `-Side`, `-EyeHeight`). No body teleport means no falls from platforms, no walls and no crouching at cramped spots.
- **Take [use-variants-dunwall-v1228u](../../work/natural-moveset/use-variants-dunwall-v1228u):** lock and door pass in both roles with the camera held at the spot.
""")
edit(D / '01-current-state.md', "protocol 75, bank 71/374 (bank 1266). Latest:\n",
"""protocol 75, bank 71/374 (bank 1266). Latest:
- **Free observer camera (#61, 1214an, [audit 1284](../../work/movement-integration/free-camera-audit-v1284.json)):** the lab camera can now be held at any point (`coop_observer_camera hold_at`) without moving the observer's body. Visual takes use it with `-FreeCam` ([chapter 92](92-native-use-interactions.md)).
""")
edit(D / '01-current-state.md', "Live candidate-npc-combat-v1214am ([identity](../../work/movement-integration/candidate-npc-combat-v1214am/identity.json))",
     "Live candidate-npc-combat-v1214an ([identity](../../work/movement-integration/candidate-npc-combat-v1214an/identity.json))")
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = m['current'].replace('1214am live', '1214an live') + ' Lab free observer camera (hold_at, #61, audit 1284).'
(R / 'work/current-task-state.json').write_bytes(json.dumps(s, indent=2, ensure_ascii=True).replace('\n', '\r\n').encode('utf-8'))
print('ok')
