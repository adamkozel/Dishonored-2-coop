# 2138 (#271): measures the other player's held pistol from an engine anim sample (coop_anim_sample, with the upper
# joints since progress-v2136). measure_pistol_walk_v2137.py took the face as head 12 to joint 92, which in
# pistol-walk-dunwall-v2137e points about 70 degrees off the body, so this one takes the body's forward from the shoulder
# line: left arm 7 to right arm 14, turned 90 degrees. Per sample: the root speed and travel direction, the left (gun)
# arm 7 to wrist 9 yaw from forward and its drop (wrist z minus arm z; about -0.5 m hanging, near 0 level), the right
# arm's drop, and the head 12's offset from the shoulders' midpoint along forward. Standing (under 0.1 m/s), walking
# forward and walking back (over 0.8 m/s, travel within 60 degrees of forward or of backward) are summarised apart.
# Usage: measure_pistol_walk_v2138.py <take folder> [out.json]
import json
import math
import sys
from pathlib import Path


def yaw(a, b):
    return math.degrees(math.atan2(b[1] - a[1], b[0] - a[0]))


def wrap(d):
    return (d + 180.0) % 360.0 - 180.0


def summary(values):
    if not values:
        return None
    s = sorted(values)
    return {'n': len(s), 'median': round(s[len(s) // 2], 2), 'min': round(s[0], 2), 'max': round(s[-1], 2)}


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
        rows.append((r['ms'], r['root'], {p['joint']: p['at'] for p in r['upper']}))
    if not rows:
        report[path.name] = 'no upper joints (sampler before progress-v2136)'
        continue
    groups = {'standing': [], 'forward': [], 'back': []}
    for k in range(1, len(rows)):
        (t0, r0, _), (t1, r1, u) = rows[k - 1], rows[k]
        dt = (t1 - t0) / 1000.0
        if dt <= 0:
            continue
        dx, dy = r1[0] - r0[0], r1[1] - r0[1]
        speed = math.hypot(dx, dy) / dt
        forward = wrap(yaw(u[7], u[14]) + 90.0)
        mid = [(u[7][i] + u[14][i]) / 2 for i in range(3)]
        f = (math.cos(math.radians(forward)), math.sin(math.radians(forward)))
        entry = {'gunFromForward': wrap(yaw(u[7], u[9]) - forward),
                 'gunDrop': u[9][2] - u[7][2],
                 'otherDrop': u[16][2] - u[14][2],
                 'headAhead': (u[12][0] - mid[0]) * f[0] + (u[12][1] - mid[1]) * f[1]}
        if speed < 0.1:
            groups['standing'].append(entry)
        elif speed > 0.8:
            travel = wrap(math.degrees(math.atan2(dy, dx)) - forward)
            entry['travelFromForward'] = travel
            if abs(travel) <= 60:
                groups['forward'].append(entry)
            elif abs(travel) >= 120:
                groups['back'].append(entry)
    out = {name: {key: summary([e[key] for e in group if key in e])
                  for key in ('gunFromForward', 'gunDrop', 'otherDrop', 'headAhead', 'travelFromForward')}
           for name, group in groups.items()}
    out['samples'] = len(rows)
    report[path.name] = out
text = json.dumps(report, indent=1)
print(text)
if len(sys.argv) > 2:
    Path(sys.argv[2]).write_text(text + '\n')
