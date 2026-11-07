# 2142 (#271): measure_pistol_walk_v2138.py takes the body's forward from the shoulder line, which a side-on duel stance
# turns away from the aim, so its gunFromForward (about 77 degrees Emily, 68 Corvo, in pistol-walk-dunwall-v2140a and
# v2141b, d, e alike) cannot tell a side-held arm from a forward one. This one measures against the avatar's own yaw (the
# anim sample's "yaw", the entity's facing): the gun arm 7 to wrist 9 yaw from facing, the head 12 to joint 92 yaw from
# facing, and the gun arm's drop (wrist z minus arm z). Standing (under 0.1 m/s) and moving (over 0.8 m/s) apart.
# Usage: measure_pistol_facing_v2142.py <take folder> [out.json]
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
        if line.startswith('{"ms"'):
            r = json.loads(line)
            if 'upper' in r:
                rows.append(r)
    groups = {'standing': {'gun': [], 'head': [], 'drop': []}, 'moving': {'gun': [], 'head': [], 'drop': []}}
    for prev, r in zip(rows, rows[1:]):
        dt = ((r['ms'] - prev['ms']) % (1 << 64)) / 1000.0
        if dt <= 0 or dt > 1:
            continue
        speed = math.hypot(r['root'][0] - prev['root'][0], r['root'][1] - prev['root'][1]) / dt
        group = 'standing' if speed < 0.1 else 'moving' if speed > 0.8 else None
        if not group:
            continue
        j = {u['joint']: u['at'] for u in r['upper']}
        if not all(k in j for k in (7, 9, 12, 92)):
            continue
        groups[group]['gun'].append(wrap(yaw(j[7], j[9]) - r['yaw']))
        groups[group]['head'].append(wrap(yaw(j[12], j[92]) - r['yaw']))
        groups[group]['drop'].append(j[9][2] - j[7][2])
    report[path.name] = {g: {k: summary(v) for k, v in d.items()} for g, d in groups.items()}
    report[path.name]['samples'] = len(rows)
text = json.dumps(report, indent=1)
if len(sys.argv) > 2:
    Path(sys.argv[2]).write_text(text + '\n', encoding='utf-8')
print(text)
