# 2137 (#271): the held-pistol facing in a take's engine anim samples (coop_anim_sample lines with "upper", progress-v2136
# on). For each sample: the rig's root speed, its travel direction, the face (head 12 to nose 92), the gun arm (left arm 7
# to wrist 9) and the shoulder line (7 to right arm 14), as yaws in degrees. Standing samples (speed under 0.1 m/s) and
# walking samples (over 0.8 m/s) are summarised apart: the gun arm's yaw from the face, the face's yaw from the travel
# direction, and the gun hand's height against the shoulder. Usage: measure_pistol_walk_v2137.py <take> [<out json>]
import json
import math
import statistics
import sys
from pathlib import Path


def yaw(a, b):
    return math.degrees(math.atan2(b[1] - a[1], b[0] - a[0]))


def wrap(d):
    return (d + 180.0) % 360.0 - 180.0


def summary(values):
    if not values:
        return None
    return {'n': len(values), 'median': round(statistics.median(values), 1), 'min': round(min(values), 1), 'max': round(max(values), 1)}


take = Path(sys.argv[1])
report = {}
for path in sorted(take.glob('*-anim.jsonl')):
    rows = []
    for line in path.read_text(encoding='utf-8-sig').splitlines():
        line = line.strip()
        if not line.startswith('{"ms"'):
            continue
        r = json.loads(line)
        if 'upper' not in r:
            continue
        u = {p['joint']: p['at'] for p in r['upper']}
        rows.append((r['ms'], r['root'], u))
    if not rows:
        report[path.name] = 'no upper joints (sampler before progress-v2136)'
        continue
    standing, walking = [], []
    for k in range(1, len(rows)):
        (t0, r0, _), (t1, r1, u) = rows[k - 1], rows[k]
        dt = (t1 - t0) / 1000.0
        if dt <= 0:
            continue
        dx, dy = r1[0] - r0[0], r1[1] - r0[1]
        speed = math.hypot(dx, dy) / dt
        face, gun = yaw(u[12], u[92]), yaw(u[7], u[9])
        entry = {'gunFromFace': wrap(gun - face), 'shouldersFromFace': wrap(yaw(u[7], u[14]) - face),
                 'handAboveShoulder': u[9][2] - u[7][2]}
        if speed < 0.1:
            standing.append(entry)
        elif speed > 0.8:
            entry['faceFromTravel'] = wrap(face - math.degrees(math.atan2(dy, dx)))
            walking.append(entry)
    out = {}
    for name, group in (('standing', standing), ('walking', walking)):
        out[name] = {key: summary([e[key] for e in group if key in e])
                     for key in ('gunFromFace', 'shouldersFromFace', 'handAboveShoulder', 'faceFromTravel')}
    out['samples'] = len(rows)
    report[path.name] = out
text = json.dumps(report, indent=1)
print(text)
if len(sys.argv) > 2:
    Path(sys.argv[2]).write_text(text + '\n')
