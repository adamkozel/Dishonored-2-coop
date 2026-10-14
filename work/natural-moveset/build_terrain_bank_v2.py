"""Append measured, event-free native stair cycles to the protected v16 bank.

Run in Blender Python for native skeleton forward kinematics. The donor root
is metadata only: network placement remains the sole owner of actor motion.
"""
import sys, struct, json, math, hashlib
from pathlib import Path
from mathutils import Quaternion
P = Path(__file__).resolve().parent
sys.path.insert(0, str(P))
from rig_math import NativeRig, qts, read
OUT = P/'terrain-banks-v2'
OUT.mkdir(exist_ok=True)
rows = read(P/'library-retarget-manifest.json') + read(P/'terrain-retarget-manifest-v1.json')

def blend(a, b, t):
    result = []
    for x, y in zip(a, b):
        q = Quaternion((x[3], *x[:3])).slerp(Quaternion((y[3], *y[:3])), t).normalized()
        result.append([q.x, q.y, q.z, q.w, *[x[k]+(y[k]-x[k])*t for k in range(4, 7)]])
    return result

def sample(frames, frame):
    a = min(int(frame), len(frames)-1)
    return blend(frames[a], frames[min(a+1, len(frames)-1)], frame-a)

reports = []
for role in ('corvo', 'emily'):
    data = (P/'air-combat-banks-v1'/f'{role}-air-combat-bank.bin').read_bytes()
    magic, version, n, count, fps, layout = struct.unpack_from('<8sIIIIQ', data)
    assert magic == b'D2MOVE16' and count == 141
    bank = bytearray(struct.pack('<8sIIIIQ', b'D2MOVE17', 17, n, 149, fps, layout)+data[32:])
    rig = NativeRig(read(P.parent/f'mixamo-crouch/native-{role}.json')['joints'])
    entry = 24+121*n*28

    def waves(frames):
        g = [rig.fk([qts(v) for v in pose]) for pose in frames]
        channels = [[pose[j].translation.z for pose in g] for j in (21, 25)]
        return [[(x-min(c))/max(.001, max(c)-min(c)) for x in c] for c in channels]

    sources = []
    for offset, alias in enumerate(('sword_walk_stairs_up', 'sword_walk_stairs_down',
            'sword_run_stairs_up', 'sword_run_stairs_down', 'empty_walk_stairs_up',
            'empty_walk_stairs_down', 'empty_run_stairs_up', 'empty_run_stairs_down')):
        cid = 141+offset
        row = next(r for r in rows if r['role'] == role and alias in r['source']['aliases'])
        raw = (P/row['binary']).read_bytes()
        assert hashlib.sha256(raw).hexdigest() == row['sha256']
        tag, joints, frames, source_fps, source_layout = struct.unpack_from('<8sIIfQ', raw)
        assert tag == b'D2NATV01' and joints == n and layout == source_layout
        fs = [[list(struct.unpack_from('<7f', raw, 28+(f*n+j)*28)) for j in range(n)] for f in range(frames)]
        root = row['root_track']
        stride = sum(math.hypot(b[4]-a[4], b[5]-a[5]) for a,b in zip(root,root[1:]))
        assert .2 < stride < 6 and row['native_length_error'] < .0001
        # Full native loop, preserving root-derived horizontal cycle length.
        poses = [sample(fs, (frames-1)*f/120) for f in range(120)]
        reference_id = (60 if offset >= 4 else 1)+(8 if offset%4 >= 2 else 0)
        reference = [[list(struct.unpack_from('<7f', data, 32+reference_id*entry+24+(f*n+j)*28)) for j in range(n)] for f in range(120)]
        ref, wave = waves(reference), waves(poses)
        phase_offset = min(range(120), key=lambda k: sum((wave[j][(f+k)%120]-ref[j][f])**2 for j in range(2) for f in range(120)))
        poses = poses[phase_offset:]+poses[:phase_offset]
        for f in range(115,120):
            t = (f-114)/6; t = t*t*(3-2*t)
            poses[f] = blend(poses[f], poses[0], t)
        poses.append(poses[0])
        bank.extend(struct.pack('<IIIIff', cid, 121, 0, 0, stride, row['duration']))
        for pose in poses:
            for v in pose: bank.extend(struct.pack('<7f', *v))
        sources.append(dict(clip=cid, alias=alias, binary=row['binary'], sha256=row['sha256'],
            asset_sha256=row['source']['asset_sha256'], stride_m=stride, source_frames=frames,
            source_fps=source_fps, source_duration=row['duration'], source_cycles=1, phase_offset=phase_offset/120,
            root_vertical_m=root[-1][6]-root[0][6], native_length_error=row['native_length_error']))
    path = OUT/f'{role}-terrain-bank.bin'
    path.write_bytes(bank)
    reports.append(dict(role=role, bank=str(path.relative_to(P)), sha256=hashlib.sha256(bank).hexdigest(),
        format='D2MOVE17', joints=n, clips=149, previous_payload_unchanged=bank[32:32+141*entry]==data[32:],
        sources=sources, events=False, root_writes=False, visual_accepted=False, gameplay_verified=False))
(P/'terrain-bank-manifest-v2.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('TERRAIN_BANKS: 149 clips each; unchanged 141-clip payload; no events or actor root writes')
