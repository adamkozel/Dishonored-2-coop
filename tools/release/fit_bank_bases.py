"""#30: fits, for every clip of a shipped movement bank, a base the installer can rebuild from the player's own
animations (bank_rebuild.h): per frame and body part, the bind pose, one source at a fractional time, or a blend of
two. Sources are the clip's native assets (work/release/payload-classification-v1312/bank-clips.json.gz), each
copied by joint name or retargeted (bank_math.py), optionally mirrored, and the bank clips it was built on.
Writes the parameters (D2BPR001) for `d2coop_setup bank-encode` and a JSON report of how much of each clip's
motion the base explains. Read only on lab data. Runs under a Python with numpy (Blender's bundled Python).

Usage: fit_bank_bases.py ROLE BANK OUT_PARAMS OUT_REPORT [workers] [clips]

D2BPR001 (little-endian; strings are u32 length + UTF-8):
  magic, str role, u32 joints, u32 parts, u8[joints] part of each joint (255: joints 0-1, always identity),
  u32 skeletons, str[] skeleton names (.md6skl), u32 rig skeleton,
  u32 assets, per asset: str name (.md6anim), u32 skeleton, u8 additive (0 none, 1 sword, 2 empty), u32 ref1, u32 ref2,
  u32 clips, per clip: u8 loop (last frame repeats the first), u32 sources, per source: u8 kind (1 asset, 2 bank
  clip), u32 index, u8 variant (0 by name, 1 retargeted), u8 mirrored; then frames x parts x
  (u16 source a, f32 time a, u16 source b, f32 time b, f32 weight); source 0 is the bind pose.
"""
import gzip, json, os, struct, sys
from pathlib import Path
from multiprocessing import Pool
import numpy as np
sys.path.insert(0, str(Path(__file__).parent))
import bank_math as bm

FRAMES = 121
DENSE = 8  # fit grid: 1/8 of a native frame
PARTS = {9: 'left_hand', 8: 'left_forearm', 6: 'left_upper_arm', 16: 'right_hand', 15: 'right_forearm', 13: 'right_upper_arm',
         11: 'head', 21: 'left_foot', 19: 'left_leg', 25: 'right_foot', 23: 'right_leg', 3: 'spine', 18: 'pelvis', 2: 'root'}
WEIGHTS = np.linspace(0, 1, 17)[1:-1]
STEP = 2.0 ** -17

def setup(role, bank_path):
    global ROLE, META, RIG, BANK, J, PART, NP, MASKS, IDENT
    ROLE = role
    # #137: D2_BANK_CLASSIFICATION names another classification (v1667 adds the clips of banks 72-75).
    path = os.environ.get('D2_BANK_CLASSIFICATION', 'work/release/payload-classification-v1312/bank-clips.json.gz')
    META = {x['clip']: x for x in json.load(gzip.open(bm.ROOT / path)) if x['role'] == role}
    RIG = bm.rig(role); J = len(RIG.names)
    BANK = Path(bank_path).read_bytes()
    names = list(PARTS.values()); PART = []
    for i in range(J):
        k, g = i, 255
        while k >= 2:
            if k in PARTS:
                g = names.index(PARTS[k]); break
            k = RIG.parent[k]
        PART.append(g)
    PART = np.array(PART); NP = len(names); MASKS = [PART == k for k in range(NP)]
    IDENT = RIG.bind.copy(); IDENT[:2] = [0, 0, 0, 1, 0, 0, 0]

def clip(c):
    size = 24 + FRAMES * J * 28
    return np.frombuffer(BANK, '<f4', FRAMES * J * 7, 32 + c * size + 24).reshape(FRAMES, J, 7).astype(np.float64)

def variants(asset):
    """[(variant, mirrored, track)] for a native asset, or [] when the lab has no sample of it."""
    s = bm.sample_of(asset)
    if s is None:
        return []
    frames, skel = s
    src = bm.Rig(bm.skeleton(skel))
    plain = [(0, bm.by_name(frames, src, RIG)), (1, bm.retarget(bm.compose_additive(bm.real_asset(asset), frames), src, RIG))]
    return [(v, 0, t) for v, t in plain] + [(v, 1, bm.mirror(t, RIG)) for v, t in plain]

def nlerp(a, b, t):
    sgn = np.where(np.sum(a[..., :4] * b[..., :4], -1, keepdims=True) < 0, -1.0, 1.0)
    q = a[..., :4] * (1 - t) + b[..., :4] * sgn * t
    q = q / np.linalg.norm(q, axis=-1, keepdims=True)
    return np.concatenate([q, a[..., 4:] * (1 - t) + b[..., 4:] * t], -1)

def dense(track):
    ts = np.linspace(0, 1, DENSE + 1)[:-1, None, None]
    return np.concatenate([nlerp(track[g][None], track[g + 1][None], ts) for g in range(len(track) - 1)] + [track[-1:]], 0)

def assemble(base):
    base[:, :2] = IDENT[:2]; base[:, 3:, 4:] = RIG.bind[3:, 4:]
    for f in range(FRAMES):
        R = bm.q2m(base[f, :, :4]); T = base[f, :, 4:].copy(); bm.helpers(RIG, R, T)
        base[f, :, :4] = bm.m2q(R); base[f, :, 4:] = T
    return base

def run(c):
    meta = META[c]; final = clip(c); fq = final[..., :4]
    sources = []; rows = [IDENT[None]]; owner = [(0, 1)]  # pool row -> (source id, time step)
    for name in meta['nativeAssets']:
        for variant, mirrored, track in variants(name):
            sources.append(dict(kind=1, asset=bm.real_asset(name), variant=variant, mirrored=mirrored))
            rows.append(dense(track)); owner.append((len(sources), DENSE))
    for b in meta['baseClips']:
        assert b < c, f'clip {c} builds on later clip {b}'
        for mirrored in (0, 1):
            sources.append(dict(kind=2, index=b, variant=0, mirrored=mirrored))
            rows.append(clip(b) if not mirrored else bm.mirror(clip(b), RIG)); owner.append((len(sources), 1))
    starts = np.cumsum([0] + [len(r) for r in rows]); pool = np.concatenate(rows, 0)
    def ref(row):
        k = int(np.searchsorted(starts, row, 'right') - 1); sid, den = owner[k]
        return sid, (row - starts[k]) / den
    base = np.broadcast_to(IDENT, final.shape).copy(); choice = np.zeros((FRAMES, NP, 5))
    for f in range(FRAMES):
        cost = 1 - np.minimum(np.abs(np.einsum('pjk,jk->pj', pool[..., :4], fq[f])), 1)
        for k in range(NP):
            mk = MASKS[k]; ck = cost[:, mk].sum(1); order = np.argsort(ck, kind='stable')[:6]
            best_v, best = ck[order[0]], pool[order[0]][mk]; pick = (order[0], order[0], 0.0)
            A = pool[order[:4]][:, None, None, mk]; B = pool[order[:6]][None, :, None, mk]
            cand = nlerp(A, B, WEIGHTS[None, None, :, None, None])
            v = (1 - np.minimum(np.abs(np.einsum('abwjk,jk->abwj', cand[..., :4], fq[f, mk])), 1)).sum(-1)
            i = np.unravel_index(np.argmin(v), v.shape)
            if v[i] < best_v:
                best_v, best = v[i], cand[i]; pick = (order[i[0]], order[i[1]], WEIGHTS[i[2]])
            base[f, mk] = best
            (sa, ua), (sb, ub) = ref(pick[0]), ref(pick[1]); choice[f, k] = (sa, ua, sb, ub, pick[2])
    base = assemble(base)
    bq = base[..., :4].copy(); bq[np.sum(fq * bq, -1) < 0] *= -1
    b0 = np.broadcast_to(IDENT[None, :, :4], fq.shape).copy(); b0[np.sum(fq * b0, -1) < 0] *= -1
    motion = float(np.sum((fq - b0) ** 2)); left = float(np.sum((fq - bq) ** 2))
    ang = np.degrees(2 * np.arccos(np.clip(np.abs(np.sum(fq * bq, -1)), 0, 1)))
    loop = bool(np.array_equal(np.float32(final[0]), np.float32(final[-1])))
    report = dict(clip=c, category=meta['category'], components=meta['components'], assets=len(meta['nativeAssets']),
                  missing=[a for a in meta['nativeAssets'] if bm.sample_of(a) is None], loop=loop, motion=motion, residual=left,
                  explained=round(1 - left / motion, 5) if motion else None, rms_degrees=round(float(np.sqrt(np.mean(ang ** 2))), 3),
                  part_rms_degrees={n: round(float(np.sqrt(np.mean(ang[:, MASKS[k]] ** 2))), 2) for k, n in enumerate(PARTS.values())})
    return report, sources, choice, loop

def s(x):
    b = x.encode('utf-8'); return struct.pack('<I', len(b)) + b

def main():
    role, bank, out_params, out_report = sys.argv[1:5]
    workers = int(sys.argv[5]) if len(sys.argv) > 5 else 1
    setup(role, bank)
    clips = list(range(struct.unpack_from('<I', BANK, 16)[0])) if len(sys.argv) <= 6 else [int(x) for x in sys.argv[6].split(',')]
    if workers > 1:
        with Pool(workers, setup, (role, bank)) as p:
            results = list(p.imap(run, clips))
    else:
        results = [run(c) for c in clips]
    assets = []; skeletons = [bm.RIGS[role]]
    def asset_id(name):
        names = [a['name'] for a in assets]
        if name in names:
            return names.index(name)
        skel = bm.sample_of(name)[1]
        if skel not in skeletons:
            skeletons.append(skel)
        index = len(assets); entry = dict(name=name, skeleton=skeletons.index(skel), additive=0, refs=[0xffffffff] * 2); assets.append(entry)
        kind, refs = bm.additive_refs(name)  # an additive reference can be the asset itself (empty_walk_n_add)
        entry['additive'] = kind; entry['refs'] = [asset_id(r) for r in refs] + [0xffffffff] * (2 - len(refs))
        return index
    body = b''
    for report, sources, choice, loop in results:
        body += struct.pack('<BI', loop, len(sources))
        for src in sources:
            body += struct.pack('<BIBB', src['kind'], asset_id(src['asset']) if src['kind'] == 1 else src['index'], src['variant'], src['mirrored'])
        for f in range(FRAMES):
            for k in range(NP):
                sa, ua, sb, ub, w = choice[f, k]
                body += struct.pack('<HfHff', int(sa), ua, int(sb), ub, w)
    head = b'D2BPR001' + s(role) + struct.pack('<II', J, NP) + bytes(int(v) for v in PART)
    head += struct.pack('<I', len(skeletons)) + b''.join(s(n) for n in skeletons) + struct.pack('<I', 0)
    head += struct.pack('<I', len(assets)) + b''.join(s(a['name']) + struct.pack('<IBII', a['skeleton'], a['additive'], *a['refs']) for a in assets)
    head += struct.pack('<I', len(results))
    Path(out_params).write_bytes(head + body)
    reports = [r[0] for r in results]
    tm = sum(r['motion'] for r in reports); tl = sum(r['residual'] for r in reports)
    summary = dict(role=role, clips=len(reports), assets=len(assets), skeletons=skeletons, explained=round(1 - tl / tm, 5),
                   pure_native_clips_explained_99=sum(1 for r in reports if r['components'] == ['native-sampled'] and (r['explained'] or 0) >= .99),
                   pure_native_clips=sum(1 for r in reports if r['components'] == ['native-sampled']))
    Path(out_report).write_text(json.dumps(dict(summary=summary, clips=reports), indent=1))
    print(json.dumps(summary))

if __name__ == '__main__':
    main()
