# 2135 (#271): measure how far the remote pistol idle (bank clip 201, from the guard's sword_gun_idle) turns the gun arm
# away from the body's forward. Reads the ranged bank (D2MOVE29: per clip <IIIIff> header, then frames x joints x
# <7f> local xyzw quaternion + translation) and the rig's parents, and compares the horizontal directions in clip 0
# (locomotion idle, facing forward) and clip 201: the shoulder line, the left arm (shoulder joint 7 to hand joint 9),
# and the head's offset from the neck.
import json
import math
import struct
import sys
from pathlib import Path

P = Path(__file__).resolve().parent


def qmul(a, b):
    ax, ay, az, aw = a
    bx, by, bz, bw = b
    return (aw * bx + ax * bw + ay * bz - az * by, aw * by - ax * bz + ay * bw + az * bx,
            aw * bz + ax * by - ay * bx + az * bw, aw * bw - ax * bx - ay * by - az * bz)


def qrot(q, v):
    x, y, z, w = q
    p = qmul(qmul(q, (v[0], v[1], v[2], 0.0)), (-x, -y, -z, w))
    return p[:3]


def clips(path):
    data = Path(path).read_bytes()
    magic, version, n, count, fps, layout = struct.unpack_from('<8sIIIIQ', data)
    at = 32
    out = {}
    for _ in range(count):
        index, frames, _a, _b, _c, duration = struct.unpack_from('<IIIIff', data, at)
        at += 24
        poses = []
        for f in range(frames):
            poses.append([struct.unpack_from('<7f', data, at + (f * n + j) * 28) for j in range(n)])
        at += frames * n * 28
        out[index] = (duration, poses)
    return n, out


def world(pose, parents):
    w = [None] * len(pose)
    for j, (x, y, z, qw, tx, ty, tz) in enumerate(pose):
        q = (x, y, z, qw)
        t = (tx, ty, tz)
        p = parents[j]
        if p < 0:
            w[j] = (q, t)
        else:
            pq, pt = w[p]
            r = qrot(pq, t)
            w[j] = (qmul(pq, q), (pt[0] + r[0], pt[1] + r[1], pt[2] + r[2]))
    return [t for _, t in w]


def yaw(a, b):
    return math.degrees(math.atan2(b[1] - a[1], b[0] - a[0]))


for role in ('corvo', 'emily'):
    joints = json.loads((P.parent / 'mixamo-crouch' / f'native-{role}.json').read_text())['joints']
    parents = [j['parent'] for j in joints]
    names = [j['nativeName'] for j in joints]
    n, cl = clips(P / 'ranged-banks-v1' / f'{role}-ranged-bank.bin')
    print(role, 'joints', n, 'names 3..13:', [(i, names[i]) for i in range(3, 14)])
    for idx in (0, 201):
        dur, poses = cl[idx]
        for f in (0, len(poses) // 2):
            w = world(poses[f], parents)
            print(' clip', idx, 'frame', f,
                  'L7->R11 shoulder yaw %.1f' % yaw(w[7], w[11]) if len(w) > 11 else '',
                  'L arm 7->9 yaw %.1f' % yaw(w[7], w[9]),
                  'R arm 11->13 yaw %.1f' % yaw(w[11], w[13]) if len(w) > 13 else '',
                  'L hand height %.2f shoulder %.2f' % (w[9][2], w[7][2]))
