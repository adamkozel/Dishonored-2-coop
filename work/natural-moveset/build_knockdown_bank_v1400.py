"""Bank74/381 (#11 stagger_knockdown_recovery): the partner's knockdown. The player's native knockdown (PLAYER_HITREACT_KNOCKDOWN:
KnockedDown, state 16, hit_react_knocked_down Scene_1 2599 ms; provoked with the lab shove in coverage-v1397f/g, confirmed runtime)
is first-person only, so the partner plays a composition of native NPC motion, retargeted per role from the role's own
skeleton where one exists (tall -> Corvo, female -> Emily; the rise exists only for tall and is retargeted to both):
  0.00-0.70 s  the generic sleep-dart front collapse, native frames 60-116 (standing to lying face down; retarget 1362)
  0.70-0.90 s  lying still (its last frame)
  0.90-1.10 s  blend into the face-down get-up's first frame (kneeling, head down)
  1.10-1.80 s  generic_recovery_front, frames 0-45 (to all fours; retarget 1374)
  1.80-2.599 s sword_recovery_fast_front, frames 0-55 (all fours to standing, guard up; library retarget), the first 0.12 s
               blended from the get-up's last frame.
Join poses were checked in renders (native-previews/kd1400*-*.png). Resampled to 120 frames plus the loop guard; header angle =
2.599 s, clip 380. Root network-owned, no events. Prior 380 clip payloads stay byte-identical. Writes
knockdown-banks-v1400/<role>-choke-bank.bin and knockdown-bank-manifest-v1400.json."""
from pathlib import Path
import numpy as np
import json, struct, hashlib
P = Path(__file__).resolve().parent; O = P / 'knockdown-banks-v1400'; O.mkdir(exist_ok=False)
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def rows(name):
    m = rd(P / name); return m if isinstance(m, list) else m.get('rows', [])
manifests = [r for name in ('native-retarget-manifest-sleepdart-v1362.json', 'native-retarget-manifest-knockdown-v1374.json', 'library-retarget-manifest.json') for r in rows(name)]
SOURCES = {'corvo': {'fall': '6ded466514f8e2df', 'getup': 'dbbf121c526861e2', 'rise': '1f92a3c98f34c807'},
           'emily': {'fall': '8cab794852b1306c', 'getup': 'e3f281dbcbe760d7', 'rise': '1f92a3c98f34c807'}}
WINDOWS = {'fall': (60, 116), 'getup': (0, 45), 'rise': (0, 55)}
DURATION = 2.599; CLIP = 380
def load(role, key, n, layout):
    sid = SOURCES[role][key]
    [m] = [r for r in manifests if r.get('role') == role and r.get('source', {}).get('id') == sid]
    p = P / 'native-retargeted' / f"{m['id']}.bin"; raw = p.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == m['sha256'] and (m['joints'], m['layout_hash']) == (n, layout), (role, key)
    assert not m['root_writes'] and not m['events'] and not m['first_person_constraints'] and m['fps'] == 30
    a = np.frombuffer(raw, dtype='<f4', offset=28).reshape(m['frames'], n, 7).astype(float)
    first, last = WINDOWS[key]; return a[first:last + 1], m, p
def nlerp(x, y, t):
    y = y.copy(); y[..., :4] *= np.where(np.sum(x[..., :4] * y[..., :4], axis=-1, keepdims=True) < 0, -1, 1)
    q = x + (y - x) * t; q[..., :4] /= np.linalg.norm(q[..., :4], axis=-1, keepdims=True); return q
def at(a, u):
    f = min(max(u, 0.0), 1.0) * (len(a) - 1); lo = int(f); hi = min(lo + 1, len(a) - 1); return nlerp(a[lo], a[hi], f - lo)
smooth = lambda w: w * w * (3 - 2 * w)
reports = []
for role in ('corvo', 'emily'):
    src = P / 'recoil-banks-v1365' / f'{role}-choke-bank.bin'; old = src.read_bytes()
    magic, v, n, count, fps, layout = struct.unpack_from('<8sIIIIQ', old); assert (magic, v, count) == (b'D2MOVE73', 73, 380)
    bank = bytearray(struct.pack('<8sIIIIQ', b'D2MOVE74', 74, n, 381, fps, layout) + old[32:])
    fall, mf, pf = load(role, 'fall', n, layout); getup, mg, pg = load(role, 'getup', n, layout); rise, mr, pr = load(role, 'rise', n, layout)
    frames = []
    for k in range(120):
        t = k / 119 * DURATION
        if t < .70: pose = at(fall, t / .70)
        elif t < .90: pose = fall[-1]
        elif t < 1.10: pose = nlerp(fall[-1], getup[0], smooth((t - .90) / .20))
        elif t < 1.80: pose = at(getup, (t - 1.10) / .70)
        else:
            u = (t - 1.80) / (DURATION - 1.80); pose = at(rise, u)
            if t < 1.92: pose = nlerp(getup[-1], pose, smooth((t - 1.80) / .12))
        frames.append(pose)
    q = np.stack(frames).astype('<f4')
    assert np.isfinite(q).all() and np.max(np.abs(q)) <= 3 and np.max(np.abs(q[:, :2] - [0, 0, 0, 1, 0, 0, 0])) < 1e-5, role
    bank.extend(struct.pack('<IIIIff', CLIP, 121, 0, 0, 0, float(DURATION))); bank.extend(q.tobytes()); bank.extend(q[0].tobytes())
    assert bank[32:len(old)] == old[32:]
    out = O / f'{role}-choke-bank.bin'; out.write_bytes(bank); pelvis = q[:, 2, 6]
    reports.append(dict(role=role, bank=str(out.relative_to(P)), sha256=hashlib.sha256(bank).hexdigest().upper(), version=74, clips=381,
                        prior380PayloadsUnchanged=True, source_bank=str(src.relative_to(P)), source_bank_sha256=hashlib.sha256(old).hexdigest().upper(),
                        added=[dict(clip=CLIP, duration=DURATION, segments=dict(fall=[0, .70], lie=[.70, .90], blend=[.90, 1.10], getup=[1.10, 1.80], rise=[1.80, DURATION]),
                                    sources={k: dict(asset=m['source']['asset'], retargeted=str(p.relative_to(P)), sha256=m['sha256'], nativeFrames=list(WINDOWS[k]))
                                             for k, (m, p) in dict(fall=(mf, pf), getup=(mg, pg), rise=(mr, pr)).items()},
                                    pelvisStart=round(float(pelvis[0]), 3), pelvisMin=round(float(pelvis.min()), 3), pelvisEnd=round(float(pelvis[-1]), 3))],
                        root_writes=False, events=False, runtime_verified=False, visual_accepted=False, complete=False))
(P / 'knockdown-bank-manifest-v1400.json').write_text(json.dumps(reports, indent=2), encoding='utf-8')
print(json.dumps([(r['role'], r['sha256'][:16], r['added'][0]['pelvisStart'], r['added'][0]['pelvisMin'], r['added'][0]['pelvisEnd']) for r in reports]))
