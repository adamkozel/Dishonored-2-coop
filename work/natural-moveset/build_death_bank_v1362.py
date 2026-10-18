"""Bank72/378 (#11 death_variants): the partner's death collapse. A player's death plays no native scene (native state 15
Dead only; probe 1361a), so the presentation uses the native generic sleep-dart collapses, retargeted per role from the
role's own native skeleton (tall -> Corvo, female -> Emily; retarget-sleepdart-v1362.log). Each native clip staggers
1-4 s before falling; only the collapse window is kept (from 0.5 s before the pelvis starts to drop until 0.5 s after it
settles), so the partner falls within about 2 s of the native death and holds the lying end pose:
374 front, 375 back, 376 left, 377 right (direction of the native hit relative to the facing). Resampled to 120 frames
plus the loop guard the decoder requires; clip header angle = window duration in seconds. Root network-owned, no events.
Prior 374 clip payloads stay byte-identical. Writes death-banks-v1362/<role>-choke-bank.bin and death-bank-manifest-v1362.json."""
from pathlib import Path
import numpy as np
import json, struct, hashlib
P = Path(__file__).resolve().parent; O = P / 'death-banks-v1362'; O.mkdir(exist_ok=False)
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
WINDOWS = {'front': (374, 49, 116), 'back': (375, 82, 142), 'left': (376, 0, 54), 'right': (377, 63, 120)}
retargets = rd(P / 'native-retarget-manifest-sleepdart-v1362.json'); reports = []
for role in ('corvo', 'emily'):
    src = P / 'use-banks-v1266' / f'{role}-choke-bank.bin'; old = src.read_bytes()
    magic, v, n, count, fps, layout = struct.unpack_from('<8sIIIIQ', old); assert (magic, v, count) == (b'D2MOVE71', 71, 374)
    bank = bytearray(struct.pack('<8sIIIIQ', b'D2MOVE72', 72, n, 378, fps, layout) + old[32:]); clips = []
    part = '/tall/' if role == 'corvo' else '/female/'
    for direction, (clip, first, last) in WINDOWS.items():
        [m] = [r for r in retargets if r['role'] == role and part in r['source']['asset'] and r['source']['aliases'] == [f'generic_sleepdart_{direction}']]
        p = P / 'native-retargeted' / f"{m['id']}.bin"; raw = p.read_bytes()
        assert hashlib.sha256(raw).hexdigest() == m['sha256'] and (m['joints'], m['layout_hash']) == (n, layout)
        assert not m['root_writes'] and not m['events'] and not m['first_person_constraints'] and m['fps'] == 30
        a = np.frombuffer(raw, dtype='<f4', offset=28).reshape(m['frames'], n, 7).astype(float)[first:last + 1]
        at = np.linspace(0, len(a) - 1, 120); lo = at.astype(int); hi = np.minimum(lo + 1, len(a) - 1); t = (at - lo)[:, None, None]
        x = a[lo].copy(); y = a[hi].copy(); y[..., :4] *= np.where(np.sum(x[..., :4] * y[..., :4], axis=-1, keepdims=True) < 0, -1, 1)
        q = x + (y - x) * t; q[..., :4] /= np.linalg.norm(q[..., :4], axis=-1, keepdims=True); q = q.astype('<f4')
        assert np.isfinite(q).all() and np.max(np.abs(q)) <= 3 and np.max(np.abs(q[:, :2] - [0, 0, 0, 1, 0, 0, 0])) < 1e-5, direction
        duration = (last - first) / 30
        bank.extend(struct.pack('<IIIIff', clip, 121, 0, 0, 0, float(duration))); bank.extend(q.tobytes()); bank.extend(q[0].tobytes())
        pelvis = q[:, 2, 6]
        clips.append(dict(clip=clip, direction=direction, asset=m['source']['asset'], retargeted=str(p.relative_to(P)), retargetedSha256=m['sha256'],
                          nativeFrames=[first, last], duration=round(duration, 4), pelvisStart=round(float(pelvis[0]), 3), pelvisEnd=round(float(pelvis[-1]), 3)))
    assert bank[32:len(old)] == old[32:]
    out = O / f'{role}-choke-bank.bin'; out.write_bytes(bank)
    reports.append(dict(role=role, bank=str(out.relative_to(P)), sha256=hashlib.sha256(bank).hexdigest().upper(), version=72, clips=378,
                        prior374PayloadsUnchanged=True, source_bank=str(src.relative_to(P)), source_bank_sha256=hashlib.sha256(old).hexdigest().upper(), added=clips,
                        root_writes=False, events=False, runtime_verified=False, visual_accepted=False, complete=False))
(P / 'death-bank-manifest-v1362.json').write_text(json.dumps(reports, indent=2), encoding='utf-8')
print(json.dumps([(r['role'], r['sha256'], [(c['clip'], c['duration'], c['pelvisStart'], c['pelvisEnd']) for c in r['added']]) for r in reports], indent=1))
