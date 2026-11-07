# 2135 (#271), third pass: the face and arm directions of clip 201 when joints 0-2 and the pelvis subtree come from
# clip 0 (the locomotion idle), as movement_playback does for combatLocomotionJoint when the ranged move mix is 1, and
# the yaw of each root joint (0 origin, 1 anchor, 2 center_root) in both clips.
import json
import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from measure_gun_idle_yaw_v2135 import clips, world, yaw, qrot, P  # noqa: E402
from measure_gun_idle_yaw_v2135b import row  # noqa: E402


def loco(j, parents):
    while j >= 0:
        if j == 3:
            return False
        if j == 2:
            return True
        j = parents[j]
    return False


def qyaw(q):
    v = qrot(q[:4], (1.0, 0.0, 0.0))
    return math.degrees(math.atan2(v[1], v[0]))


for role in ('corvo', 'emily'):
    joints = json.loads((P.parent / 'mixamo-crouch' / f'native-{role}.json').read_text())['joints']
    parents = [j['parent'] for j in joints]
    n, cl = clips(P / 'ranged-banks-v1' / f'{role}-ranged-bank.bin')
    idle, gun = cl[0][1][0], cl[201][1][0]
    print(role, 'root yaw clip0', [round(qyaw(idle[j]), 1) for j in (0, 1, 2, 3, 18)],
          'clip201', [round(qyaw(gun[j]), 1) for j in (0, 1, 2, 3, 18)])
    mixed = [idle[j] if loco(j, parents) or j in (0, 1) else gun[j] for j in range(n)]
    print(role, 'mix loco root+pelvis ', row(world(mixed, parents)))
    mixed = [idle[j] if j in (0, 1) else gun[j] for j in range(n)]
    print(role, 'mix loco 0-1 only    ', row(world(mixed, parents)))
