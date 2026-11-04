# 2135 (#271), second pass: measure_gun_idle_yaw_v2135.py read joints 11 and 13 as shoulders; they are the neck and the
# right shoulder. This pass uses the rig's names: the face direction (head 12 to nose 92), the left arm (7 to wrist 9),
# the right arm (14 to wrist 16) and the shoulder line (left arm 7 to right arm 14), in the rig's horizontal plane, for
# clip 0 (locomotion idle) and clip 201 (pistol idle), every 4th frame of 201.
import json
import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from measure_gun_idle_yaw_v2135 import clips, world, yaw, P  # noqa: E402


def row(w):
    face = yaw(w[12], w[92])
    return ('face %6.1f  Larm %6.1f (drop %.2f)  Rarm %6.1f (drop %.2f)  shoulders %6.1f' %
            (face, yaw(w[7], w[9]), w[7][2] - w[9][2], yaw(w[14], w[16]), w[14][2] - w[16][2], yaw(w[7], w[14])))


for role in ('corvo', 'emily'):
    joints = json.loads((P.parent / 'mixamo-crouch' / f'native-{role}.json').read_text())['joints']
    parents = [j['parent'] for j in joints]
    n, cl = clips(P / 'ranged-banks-v1' / f'{role}-ranged-bank.bin')
    for idx in (0, 201):
        _, poses = cl[idx]
        for f in range(0, len(poses), 4 if idx == 201 else max(1, len(poses))):
            print(role, 'clip', idx, 'frame', f, row(world(poses[f], parents)))
