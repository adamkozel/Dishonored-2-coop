# 2135 (#271), fourth pass: center_root (joint 2) yaw of the first locomotion clips, and the face and gun-arm directions
# when the pistol idle (clip 201) is blended over each of them with the move mix at 1 (center_root and the pelvis
# subtree from the locomotion clip, spine and above from clip 201), at frame 0.
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from measure_gun_idle_yaw_v2135 import clips, world, P  # noqa: E402
from measure_gun_idle_yaw_v2135b import row  # noqa: E402
from measure_gun_idle_mix_v2135c import loco, qyaw  # noqa: E402

for role in ('corvo', 'emily'):
    joints = json.loads((P.parent / 'mixamo-crouch' / f'native-{role}.json').read_text())['joints']
    parents = [j['parent'] for j in joints]
    n, cl = clips(P / 'ranged-banks-v1' / f'{role}-ranged-bank.bin')
    gun = cl[201][1][0]
    for idx in range(0, 9):
        base = cl[idx][1][0]
        mixed = [base[j] if loco(j, parents) or j in (0, 1) else gun[j] for j in range(n)]
        print(role, 'loco clip', idx, 'center_root %6.1f' % qyaw(base[2]), '| own', row(world(base, parents)), '| with gun', row(world(mixed, parents)))
