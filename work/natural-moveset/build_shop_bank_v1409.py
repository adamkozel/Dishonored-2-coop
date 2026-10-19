"""Bank75/382 (#11 shop_dialogue_gestures): the partner at the black market counter. While the source is in the shop
(arkGameLogicStateBlackMarket, logic 10, confirmed runtime 1402a) the player is placed at the counter and the native
shopkeeper_interact scene plays first-person body tracks only (ply_body_shopkeeper_interact_in/loop/out on the 20-joint
player body; export shop-scenes-v1402, confirmed static), so the partner plays native NPC conversation gestures,
retargeted per role from the role's own skeleton (tall -> Corvo, female -> Emily; retarget 1409):
  gesture_think in full (hand to the chin, weighing the goods), then gesture_calm in full (an open hand toward the
  shopkeeper), joined with a 0.20 s blend, and the last 0.30 s blended back into the first frame so the clip loops.
Both gestures start and end in the same neutral stance. Resampled to 120 frames plus the loop guard; header angle = the
role's clip length (Corvo 7.2 s, Emily 8.7 s), clip 381. Root network-owned, no events. Prior 381 clip payloads stay
byte-identical. Writes shop-banks-v1409/<role>-choke-bank.bin and shop-bank-manifest-v1409.json."""
from pathlib import Path
import numpy as np
import json, struct, hashlib
P = Path(__file__).resolve().parent; O = P / 'shop-banks-v1409'; O.mkdir(exist_ok=False)
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
manifest = rd(P / 'native-retarget-manifest-gestures-v1409.json')
SOURCES = {'corvo': {'think': '95da6eb1bbb39a5b', 'calm': '5053a8e9d7825de3'},
           'emily': {'think': 'f235eb477f8698db', 'calm': 'f4380ca901053f24'}}
CLIP = 381; JOIN = .20; LOOP = .30
def load(role, key, n, layout):
    sid = SOURCES[role][key]
    m = next(r for r in manifest if r.get('role') == role and r.get('source', {}).get('id') == sid)
    p = P / 'native-retargeted' / f"{m['id']}.bin"; raw = p.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == m['sha256'] and (m['joints'], m['layout_hash']) == (n, layout), (role, key)
    assert not m['root_writes'] and not m['events'] and not m['first_person_constraints'] and m['fps'] == 30
    return np.frombuffer(raw, dtype='<f4', offset=28).reshape(m['frames'], n, 7).astype(float), m, p
def nlerp(x, y, t):
    y = y.copy(); y[..., :4] *= np.where(np.sum(x[..., :4] * y[..., :4], axis=-1, keepdims=True) < 0, -1, 1)
    q = x + (y - x) * t; q[..., :4] /= np.linalg.norm(q[..., :4], axis=-1, keepdims=True); return q
def at(a, u):
    f = min(max(u, 0.0), 1.0) * (len(a) - 1); lo = int(f); hi = min(lo + 1, len(a) - 1); return nlerp(a[lo], a[hi], f - lo)
smooth = lambda w: w * w * (3 - 2 * w)
reports = []
for role in ('corvo', 'emily'):
    src = P / 'knockdown-banks-v1400' / f'{role}-choke-bank.bin'; old = src.read_bytes()
    magic, v, n, count, fps, layout = struct.unpack_from('<8sIIIIQ', old); assert (magic, v, count) == (b'D2MOVE74', 74, 381)
    bank = bytearray(struct.pack('<8sIIIIQ', b'D2MOVE75', 75, n, 382, fps, layout) + old[32:])
    think, mt, pt = load(role, 'think', n, layout); calm, mc, pc = load(role, 'calm', n, layout)
    a = (len(think) - 1) / 30.0; b = (len(calm) - 1) / 30.0; duration = round(a + b, 3)
    frames = []
    for k in range(120):
        t = k / 119 * duration
        if t < a: pose = at(think, t / a)
        else:
            pose = at(calm, (t - a) / b)
            if t < a + JOIN: pose = nlerp(think[-1], pose, smooth((t - a) / JOIN))
        if t > duration - LOOP: pose = nlerp(pose, think[0], smooth((t - (duration - LOOP)) / LOOP))
        frames.append(pose)
    q = np.stack(frames).astype('<f4')
    assert np.isfinite(q).all() and np.max(np.abs(q)) <= 3 and np.max(np.abs(q[:, :2] - [0, 0, 0, 1, 0, 0, 0])) < 1e-5, role
    seam = float(np.max(np.abs(q[-1] - q[0])))
    bank.extend(struct.pack('<IIIIff', CLIP, 121, 0, 0, 0, float(duration))); bank.extend(q.tobytes()); bank.extend(q[0].tobytes())
    assert bank[32:len(old)] == old[32:]
    out = O / f'{role}-choke-bank.bin'; out.write_bytes(bank); pelvis = q[:, 2, 6]
    reports.append(dict(role=role, bank=str(out.relative_to(P)), sha256=hashlib.sha256(bank).hexdigest().upper(), version=75, clips=382,
                        prior381PayloadsUnchanged=True, source_bank=str(src.relative_to(P)), source_bank_sha256=hashlib.sha256(old).hexdigest().upper(),
                        added=[dict(clip=CLIP, duration=duration, loop=True, segments=dict(think=[0, round(a, 3)], calm=[round(a, 3), duration], join=JOIN, loopBlend=LOOP),
                                    sources={k: dict(asset=m['source']['asset'], retargeted=str(p.relative_to(P)), sha256=m['sha256'])
                                             for k, (m, p) in dict(think=(mt, pt), calm=(mc, pc)).items()},
                                    loopSeam=round(seam, 5), pelvisMin=round(float(pelvis.min()), 3), pelvisMax=round(float(pelvis.max()), 3))],
                        root_writes=False, events=False, runtime_verified=False, visual_accepted=False, complete=False))
(P / 'shop-bank-manifest-v1409.json').write_text(json.dumps(reports, indent=2), encoding='utf-8')
print(json.dumps([(r['role'], r['sha256'][:16], r['added'][0]['duration'], r['added'][0]['loopSeam'], r['added'][0]['pelvisMin'], r['added'][0]['pelvisMax']) for r in reports]))
