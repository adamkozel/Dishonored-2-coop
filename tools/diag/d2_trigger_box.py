"""Print the world bounding box of map triggers (read-only).

A trigger's entity origin is not its volume: the volume is the trigger's Havok
collision shape, maps/campaign/.../<map>/<trigger>.cvd._100_100_100.0 in the
resource containers (a tagfile that starts 5 bytes into the blob, so its floats
sit at offset 1 mod 4). The compound shape's AABB is the first float4 pair
min, max with min.w = 0.5; it is relative to the entity origin (confirmed
static and runtime, take route-bk-v1307b: A Crack in the Slab's pa_ld_trigger_1
is the ritual hall, 6 m north of its origin, and registers only there).

Usage:
  python -I d2_trigger_box.py <base-dir> <map dir regex> <pid> <trigger name>...
Example:
  python -I d2_trigger_box.py work/asset-game/base "bunker_p_lowchaos/" 12345 pa_ld_trigger_1
Origins come from the live lab process (d2_runtime_probe --entities), so the
map must be loaded there. Names and coordinates are game-derived: keep output local.
"""
import json
import re
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import d2_resource_index as resources  # noqa: E402

PROBE = Path(__file__).resolve().parents[2] / 'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'


def aabb(blob):
    for o in range(1, 0x200, 4):
        mn = struct.unpack_from('<4f', blob, o)
        mx = struct.unpack_from('<4f', blob, o + 16)
        if all(abs(v) < 500 for v in mn[:3] + mx[:3]) and all(mn[i] < mx[i] - 0.05 for i in range(3)) and abs(mn[3] - 0.5) < 1e-3:
            return mn[:3], mx[:3]
    return None


def main():
    if len(sys.argv) < 5:
        raise SystemExit(__doc__)
    base, mapre, pid, want = sys.argv[1], re.compile(sys.argv[2]), sys.argv[3], set(sys.argv[4:])
    ents = json.loads(subprocess.run([str(PROBE), pid, '--entities'], capture_output=True, text=True).stdout)['entities']
    origin = {e['name']: e['position'] for e in ents}
    for ix in resources.containers(base):
        for e in resources.entries(ix):
            m = re.search(r'/([^/]+)\.cvd\._100_100_100\.0$', e['source'])
            if not m or m.group(1) not in want or not mapre.search(e['source']):
                continue
            box, p = aabb(resources.read(ix, e)), origin.get(m.group(1))
            row = {'trigger': m.group(1), 'source': e['source'], 'origin': p}
            if box and p:
                row['min'] = [round(p[i] + box[0][i], 2) for i in range(3)]
                row['max'] = [round(p[i] + box[1][i], 2) for i in range(3)]
            print(json.dumps(row))


if __name__ == '__main__':
    main()
