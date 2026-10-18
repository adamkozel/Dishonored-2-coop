"""Bank73/380 (#11 stagger_knockdown_recovery): the partner's recoil when the player's swing is parried or bounces off a
block. The player's native recoil scenes (sword_parry_ll/lr/rl/rr 666 ms, sword_recoil_left/right(b) and
sword_sneak_recoil 666-733 ms, under player state 40 Recoil) bind only the first-person camera and right arm
(confirmed static, native-stagger-scenes-v1365), so the partner plays the native NPC stagger instead:
sword_parry_lose_left/right, retargeted per role from the role's own skeleton (tall high for Corvo, female low for Emily;
retarget-parrylose-v1365.log). Only the stagger is kept (native frames 0-24, 0.8 s; the clip then settles and recovers to
guard, 2.13 s in all): 378 recoil left, 379 recoil right. Resampled to 120 frames plus the loop guard; header angle = 0.8 s.
Root network-owned, no events. Prior 378 clip payloads stay byte-identical. Writes recoil-banks-v1365/<role>-choke-bank.bin
and recoil-bank-manifest-v1365.json."""
from pathlib import Path
import numpy as np
import json, struct, hashlib
P = Path(__file__).resolve().parent; O = P / 'recoil-banks-v1365'; O.mkdir(exist_ok=False)
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
CLIPS = {'left': 378, 'right': 379}; FIRST, LAST = 0, 24
retargets = rd(P / 'native-retarget-manifest-parrylose-v1365.json'); reports = []
for role in ('corvo', 'emily'):
    src = P / 'death-banks-v1362' / f'{role}-choke-bank.bin'; old = src.read_bytes()
    magic, v, n, count, fps, layout = struct.unpack_from('<8sIIIIQ', old); assert (magic, v, count) == (b'D2MOVE72', 72, 378)
    bank = bytearray(struct.pack('<8sIIIIQ', b'D2MOVE73', 73, n, 380, fps, layout) + old[32:]); clips = []
    part = '/tall/' if role == 'corvo' else '/female/'
    for side, clip in CLIPS.items():
        [m] = [r for r in retargets if r['role'] == role and part in r['source']['asset'] and r['source']['aliases'] == [f'sword_parry_lose_{side}']]
        p = P / 'native-retargeted' / f"{m['id']}.bin"; raw = p.read_bytes()
        assert hashlib.sha256(raw).hexdigest() == m['sha256'] and (m['joints'], m['layout_hash']) == (n, layout)
        assert not m['root_writes'] and not m['events'] and not m['first_person_constraints'] and m['fps'] == 30
        a = np.frombuffer(raw, dtype='<f4', offset=28).reshape(m['frames'], n, 7).astype(float)[FIRST:LAST + 1]
        at = np.linspace(0, len(a) - 1, 120); lo = at.astype(int); hi = np.minimum(lo + 1, len(a) - 1); t = (at - lo)[:, None, None]
        x = a[lo].copy(); y = a[hi].copy(); y[..., :4] *= np.where(np.sum(x[..., :4] * y[..., :4], axis=-1, keepdims=True) < 0, -1, 1)
        q = x + (y - x) * t; q[..., :4] /= np.linalg.norm(q[..., :4], axis=-1, keepdims=True); q = q.astype('<f4')
        assert np.isfinite(q).all() and np.max(np.abs(q)) <= 3 and np.max(np.abs(q[:, :2] - [0, 0, 0, 1, 0, 0, 0])) < 1e-5, side
        duration = (LAST - FIRST) / 30
        bank.extend(struct.pack('<IIIIff', clip, 121, 0, 0, 0, float(duration))); bank.extend(q.tobytes()); bank.extend(q[0].tobytes())
        clips.append(dict(clip=clip, side=side, asset=m['source']['asset'], retargeted=str(p.relative_to(P)), retargetedSha256=m['sha256'], nativeFrames=[FIRST, LAST], duration=round(duration, 4)))
    assert bank[32:len(old)] == old[32:]
    out = O / f'{role}-choke-bank.bin'; out.write_bytes(bank)
    reports.append(dict(role=role, bank=str(out.relative_to(P)), sha256=hashlib.sha256(bank).hexdigest().upper(), version=73, clips=380,
                        prior378PayloadsUnchanged=True, source_bank=str(src.relative_to(P)), source_bank_sha256=hashlib.sha256(old).hexdigest().upper(), added=clips,
                        root_writes=False, events=False, runtime_verified=False, visual_accepted=False, complete=False))
(P / 'recoil-bank-manifest-v1365.json').write_text(json.dumps(reports, indent=2), encoding='utf-8')
print(json.dumps([(r['role'], r['sha256']) for r in reports]))
