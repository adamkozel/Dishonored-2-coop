"""#30: parameters for the native tables the installer rebuilds from the player's animations (`d2coop_setup tables`,
outputs/d2-coop-lab/src/native_tables.h) instead of compiling them into the DLL:
- crossbow and grenade mechanism clips: the player's exact Edge samples of the prop animations, with their
  skeletons' parents and bind poses;
- ranged hand tracks: the prop attachment relative to the wrist in the player's first-person samples
  (author_gun_hand_references_v1.py / author_items.py), at fitted times, completed by a residual.
The reference tables (D2NTB001, local lab data: work/release/native-tables-v1/reference-tables.bin) are the values
the DLL compiled in until #30 (crossbow_mechanism_data.h, grenade_mechanism_data.h and ranged_hand_tracks.h, removed
after commit a3dc1a92); `tables-encode` measures the residual against them. Read only on lab data. Runs under a
Python with numpy.

Usage: build_native_tables.py REFERENCE OUT_PARAMS OUT_REPORT

D2NTP001 (little-endian, strings u32 length + UTF-8): magic, u32 skeletons + names (.md6skl), u32 assets + per asset
(name .md6anim, u32 skeleton), u32 entries + per entry: str name, u8 kind, then kind 0 (parents, i16[joints]) or 1
(bind, f32[joints][7]): u32 skeleton; kind 2 (sample, f32[frames][joints][7]): u32 asset; kind 3 (hand tracks,
f32[tracks][frames][7]): u32 rig skeleton, u32 first-person skeleton, u32 tracks, u32 frames, then per track and
frame u32 asset, f32 time (in source frames).
D2NTB001: magic, u32 entries + per entry: str name, u8 type (0 f32, 1 i16), u32 rank, u32 dims[rank], data.
"""
import json, struct, sys
from pathlib import Path
import numpy as np
sys.path.insert(0, str(Path(__file__).parent))
import bank_math as bm

FP_SKELETON = 'models/characters/player/emily/player_leftarm_emily.md6skl'
SLOTS = ['idle', 'ooa', 'fire', 'reload', 'equip', 'unequip']
PARTS = ['idle', 'ooa', 'fire', 'reload_in', 'reload_loop', 'reload_out', 'equip', 'unequip']

def read_tables(path):
    b = Path(path).read_bytes(); assert b[:8] == b'D2NTB001'
    at = 8; n = struct.unpack_from('<I', b, at)[0]; at += 4; out = {}
    for _ in range(n):
        ln = struct.unpack_from('<I', b, at)[0]; name = b[at + 4:at + 4 + ln].decode(); at += 4 + ln
        kind, rank = struct.unpack_from('<BI', b, at); at += 5
        dims = list(struct.unpack_from(f'<{rank}I', b, at)); at += 4 * rank; count = int(np.prod(dims))
        out[name] = (kind, dims, list(struct.unpack_from(f'<{count}' + ('h' if kind else 'f'), b, at))); at += count * (2 if kind else 4)
    assert at == len(b)
    return out

def palm(G, w, i, p, m):
    inv_r = G[w][:3, :3].T; inv_t = -inv_r @ G[w][:3, 3]
    a, b, c = (inv_r @ G[k][:3, 3] + inv_t for k in (i, p, m))
    x = (a - b) / np.linalg.norm(a - b); y = c - x * (c @ x); y /= np.linalg.norm(y)
    return np.stack([x, y, np.cross(x, y)], 1)

def global_bind(rig):
    return [bm.m4(r, t) for r, t in zip(rig.GR, rig.GT)]

def invcal(fp, rig):
    sn = {n: i for i, n in enumerate(fp.names)}; tn = {n: i for i, n in enumerate(rig.names)}
    sp = palm(global_bind(fp), 4, sn['left_index_02_jnt'], sn['left_pinky_02_jnt'], sn['left_medium_02_jnt'])
    tp = palm(global_bind(rig), 9, tn['left_index01_jnt'], tn['left_pinky02_jnt'], tn['left_middle01_jnt'])
    return tp @ sp.T  # (sp @ tp^-1)^-1

def attachment(frames, fp, cal, u):
    """Prop attachment relative to the wrist (joint 4) in the first-person sample at fractional frame u."""
    g = min(int(np.floor(u)), len(frames) - 1); h = min(g + 1, len(frames) - 1); t = u - g if h > g else 0.0
    a, b = frames[g], frames[h]
    sgn = np.where(np.sum(a[:, :4] * b[:, :4], -1, keepdims=True) < 0, -1.0, 1.0)
    q = a[:, :4] * (1 - t) + b[:, :4] * sgn * t; q /= np.linalg.norm(q, axis=-1, keepdims=True)
    tr = a[:, 4:7] * (1 - t) + b[:, 4:7] * t
    R = bm.q2m(q); att = fp.names.index('left_hand_attachment_jnt')
    G = {}
    def glob(i):
        if i not in G:
            p = fp.parent[i]; m = bm.m4(R[i], tr[i]); G[i] = m if p < 0 else glob(p) @ m
        return G[i]
    rel = np.linalg.inv(glob(4)) @ glob(att)
    r = cal @ rel[:3, :3]; t3 = cal @ rel[:3, 3]
    return np.concatenate([bm.m2q(r), t3])

def same(a, b):
    """Generated headers print floats with 9 decimals: equal within that rounding."""
    a = np.float64(np.float32(a)); b = np.asarray(b, np.float64)
    return a.shape == b.shape and float(np.max(np.abs(a - b), initial=0)) <= 5.1e-10

def s(x):
    b = x.encode('utf-8'); return struct.pack('<I', len(b)) + b

def main():
    reference_tables = read_tables(sys.argv[1]); out_params, out_report = Path(sys.argv[2]), Path(sys.argv[3])
    skeletons, assets, entries, reference, report = [], [], [], [], dict(entries=[])
    def table(name):
        return reference_tables[name][2]
    def skel(name):
        if name not in skeletons: skeletons.append(name)
        return skeletons.index(name)
    def asset(name, skeleton):
        key = (name, skeleton)
        if key not in assets: assets.append(key)
        return assets.index(key)
    def ref(name, kind, dims, values):
        reference.append((name, kind, dims, values))
    # Mechanism clips: exact samples; the asset and skeleton the runtime tables were sampled from.
    xb = json.loads((bm.NM / 'crossbow-mechanism-runtime-sources-v1.json').read_text())
    for role in ('corvo', 'emily'):
        clips = [c for c in xb['clips'] if c['role'] == role]
        sample_skel = None
        for c in clips:
            rows = [r for r in bm._samples_all(c['source']) if (r[1] == 'models/weapons/crossbow/crossbo_corvo.md6skl') == (role == 'corvo')]
            path, sample_skel = rows[0]
            frames = bm.edgepose_frames(path)
            name = f'crossbow_{role}_{c["part"]}'
            entries.append((name, 2, struct.pack('<I', asset(c['source'], skel(sample_skel)))))
            ref(name, 0, [frames.shape[0], frames.shape[1], 7], table(name))
            assert same(frames[..., :7].ravel(), reference[-1][3]), name + ' is not the exact sample'
        joints = bm.skeleton(sample_skel)
        for kind, suffix in ((0, 'parents'), (1, 'bind')):
            name = f'crossbow_{role}_{suffix}'; entries.append((name, kind, struct.pack('<I', skel(sample_skel))))
            values = table(name)
            ref(name, 1 if kind == 0 else 0, [len(joints)] if kind == 0 else [len(joints), 7], values)
            mine = [j['parent'] for j in joints] if kind == 0 else [v for j in joints for v in j['bindLocalQTS'][:7]]
            assert same(mine, values), name + ' differs from the skeleton file'
    gr = json.loads((bm.NM / 'grenade-mechanism-runtime-sources-v57.json').read_text())
    gm = json.loads((bm.NM / 'grenade-mechanism-manifest-v55.json').read_text())
    gskel = None
    for c in gr['clips']:
        m = next(r for r in gm if r['asset'] == c['asset']); path, gskel = bm.NM / m['sample_file'], m['skeleton']
        frames = bm.edgepose_frames(path); name = 'grenade_' + c['phase']
        entries.append((name, 2, struct.pack('<I', asset(c['asset'], skel(gskel)))))
        ref(name, 0, [frames.shape[0], frames.shape[1], 7], table(name))
        assert same(frames[..., :7].ravel(), reference[-1][3]), name + ' is not the exact sample'
    joints = bm.skeleton(gskel)
    for kind, suffix in ((0, 'parents'), (1, 'bind')):
        name = 'grenade_' + suffix; entries.append((name, kind, struct.pack('<I', skel(gskel))))
        values = table(name)
        ref(name, 1 if kind == 0 else 0, [len(joints)] if kind == 0 else [len(joints), 7], values)
        mine = [j['parent'] for j in joints] if kind == 0 else [v for j in joints for v in j['bindLocalQTS'][:7]]
        assert same(mine, values), name + ' differs from the skeleton file'
    # Ranged hand tracks: per role [kind gun, crossbow][slot][120][7].
    tracks = np.array([table('ranged_hand_tracks_corvo'), table('ranged_hand_tracks_emily')], np.float64).reshape(2, 2, 6, 120, 7)
    fp = bm.Rig(bm.skeleton(FP_SKELETON))
    for r, role in enumerate(('corvo', 'emily')):
        rig = bm.rig(role); cal = invcal(fp, rig); body = b''
        for k, kind in enumerate(('gun', 'crossbow')):
            cands = []
            for part in PARTS:
                name = f'models/characters/player/_animations_/{role}/leftarm/ply_leftarm_{kind}_{part}.md6anim'
                path, sk = bm._samples_all(name)[0]; assert sk == FP_SKELETON
                frames = bm.edgepose_frames(path); grid = np.arange(0, len(frames) - 1 + 1e-9, 1 / 16)
                cands.append((asset(name, skel(FP_SKELETON)), grid, np.stack([attachment(frames, fp, cal, u) for u in grid])))
            for slot in range(6):
                target = tracks[r, k, slot]; worst = 0.0
                for f in range(120):
                    best = None
                    for a, grid, vals in cands:
                        d = 1 - np.minimum(np.abs(vals[:, :4] @ target[f, :4]), 1) + np.linalg.norm(vals[:, 4:] - target[f, 4:], axis=1)
                        i = int(np.argmin(d))
                        if best is None or d[i] < best[0]:
                            best = (d[i], a, grid[i], vals[i])
                    body += struct.pack('<If', best[1], best[2])
                    worst = max(worst, float(np.degrees(2 * np.arccos(min(1, abs(best[3][:4] @ target[f, :4]))))))
                report['entries'].append(dict(role=role, kind=kind, slot=SLOTS[slot], worst_degrees=round(worst, 3)))
        name = f'ranged_hand_tracks_{role}'
        entries.append((name, 3, struct.pack('<IIII', skel(bm.RIGS[role]), skel(FP_SKELETON), 12, 120) + body))
        ref(name, 0, [12, 120, 7], tracks[r].ravel().tolist())
    params = b'D2NTP001' + struct.pack('<I', len(skeletons)) + b''.join(s(n) for n in skeletons)
    params += struct.pack('<I', len(assets)) + b''.join(s(n) + struct.pack('<I', sk) for n, sk in assets)
    params += struct.pack('<I', len(entries)) + b''.join(s(n) + struct.pack('<B', kind) + data for n, kind, data in entries)
    out_params.write_bytes(params)
    for name, kind, dims, values in reference:
        assert reference_tables[name][:2] == (kind, dims), name + ' layout'
    assert sorted(reference_tables) == sorted(n for n, *_ in reference), 'reference table set'
    report.update(skeletons=skeletons, assets=[n for n, _ in assets], tables=[n for n, *_ in reference])
    out_report.write_text(json.dumps(report, indent=1))
    print(json.dumps(dict(entries=len(entries), assets=len(assets), worst_hand_track_degrees=max(e['worst_degrees'] for e in report['entries']))))

if __name__ == '__main__':
    main()
