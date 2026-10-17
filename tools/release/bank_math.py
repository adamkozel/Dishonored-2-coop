"""#30: numpy rig math for the bank patch builder, mirroring outputs/d2-coop-lab/src/bank_rebuild.h (double
precision): our retarget (work/natural-moveset/retarget_native.py), its helper joints and additive composition,
the builders' mirror, and copies by joint name. Native samples are the lab's Edge evaluator samples (.edgepose),
skeletons the lab's exported .edgeskl files; both are read only.
Runs under a Python with numpy (Blender's bundled Python in the lab)."""
import json, re, struct, sys
from pathlib import Path
import numpy as np

ROOT = Path(__file__).resolve().parents[2]
NM = ROOT / 'work/natural-moveset'
EXP = ROOT / 'outputs/d2-coop-lab/research/asset-exports'
sys.path.insert(0, str(NM))
from edge_data import skeleton as parse_skeleton  # noqa: E402

RIGS = {'corvo': 'models/characters/tall/str/corvo/t_str_corvo01.md6skl', 'emily': 'models/characters/female/emily/emily.md6skl'}

def q2m(q):
    q = q / np.linalg.norm(q, axis=-1, keepdims=True)
    x, y, z, w = np.moveaxis(q, -1, 0)
    return np.stack([np.stack([1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)], -1),
                     np.stack([2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)], -1),
                     np.stack([2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)], -1)], -2)

def m2q(m):
    """Rotation matrices -> quaternions (x, y, z, w) with w >= 0 (bank_rebuild.h m2q)."""
    m = np.asarray(m, np.float64); flat = m.reshape(-1, 3, 3); out = np.empty((len(flat), 4))
    for i, a in enumerate(flat):
        t = a[0, 0] + a[1, 1] + a[2, 2]
        if t > 0:
            s = np.sqrt(t + 1) * 2; q = [(a[2, 1] - a[1, 2]) / s, (a[0, 2] - a[2, 0]) / s, (a[1, 0] - a[0, 1]) / s, s / 4]
        elif a[0, 0] > a[1, 1] and a[0, 0] > a[2, 2]:
            s = np.sqrt(1 + a[0, 0] - a[1, 1] - a[2, 2]) * 2; q = [s / 4, (a[0, 1] + a[1, 0]) / s, (a[0, 2] + a[2, 0]) / s, (a[2, 1] - a[1, 2]) / s]
        elif a[1, 1] > a[2, 2]:
            s = np.sqrt(1 + a[1, 1] - a[0, 0] - a[2, 2]) * 2; q = [(a[0, 1] + a[1, 0]) / s, s / 4, (a[1, 2] + a[2, 1]) / s, (a[0, 2] - a[2, 0]) / s]
        else:
            s = np.sqrt(1 + a[2, 2] - a[0, 0] - a[1, 1]) * 2; q = [(a[0, 2] + a[2, 0]) / s, (a[1, 2] + a[2, 1]) / s, s / 4, (a[1, 0] - a[0, 1]) / s]
        q = np.array(q); q /= np.linalg.norm(q)
        out[i] = -q if q[3] < 0 else q
    return out.reshape(m.shape[:-2] + (4,))

def qmul(a, b):
    x1, y1, z1, w1 = np.moveaxis(a, -1, 0); x2, y2, z2, w2 = np.moveaxis(b, -1, 0)
    return np.stack([w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2, w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2,
                     w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2, w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2], -1)

class Rig:
    def __init__(self, joints):
        self.names = [j['nativeName'] for j in joints]; self.parent = [j['parent'] for j in joints]
        b = np.array([j['bindLocalQTS'][:7] for j in joints], np.float64)
        self.R = q2m(b[:, :4]); self.T = b[:, 4:7]
        self.GR, self.GT = self.fk(self.R, self.T)
        self.bind = np.concatenate([m2q(self.R), self.T], -1)

    def fk(self, R, T):
        GR = np.empty_like(R); GT = np.empty_like(T)
        for i, p in enumerate(self.parent):
            if p < 0:
                GR[i] = R[i]; GT[i] = T[i]
            else:
                GR[i] = GR[p] @ R[i]; GT[i] = GR[p] @ T[i] + GT[p]
        return GR, GT

_skeletons = {}
def skeleton(name):
    """A skeleton's joints by its .md6skl name, from the lab's exported .edgeskl."""
    if name not in _skeletons:
        stem = 'generated.skeleton.' + name[:-len('.md6skl')].replace('/', '.') + '.edgeskl'
        path = next(EXP.glob('*/game1_patch/' + stem))
        _skeletons[name] = parse_skeleton(path)['joints']
    return _skeletons[name]

def rig(role):
    return Rig(skeleton(RIGS[role]))

def m4(R, T):
    m = np.eye(4); m[:3, :3] = R; m[:3, 3] = T
    return m

def helpers(tgt, R, T):
    """Native skin helper joints follow their driver from the bind pose (retarget_native.py helpers())."""
    lookup = {n: i for i, n in enumerate(tgt.names)}; G = []
    BG = [m4(r, t) for r, t in zip(tgt.GR, tgt.GT)]
    for i, n in enumerate(tgt.names):
        side = 'left' if n.startswith('left') else 'right' if n.startswith('right') else ''
        driver = None; p = tgt.parent[i]
        if side and 'helper' in n:
            if 'upper_leg' in n or 'thigh' in n: driver = side + '_femur_jnt'
            elif 'knee' in n: driver = side + '_tibia_jnt'
            elif 'arm01' in n and 'forearm' not in n: driver = side + '_arm_jnt'
            elif 'elbow' in n or 'forearm' in n: driver = side + '_elbow_jnt'
            elif 'wrist' in n: driver = side + '_wrist_jnt'
            elif 'ankle' in n: driver = side + '_ankle_jnt'
        if driver and lookup[driver] < i:
            k = lookup[driver]; L = np.linalg.inv(G[p]) @ (G[k] @ np.linalg.inv(BG[k]) @ BG[i]); R[i] = L[:3, :3]; T[i] = L[:3, 3]
        G.append(m4(R[i], T[i]) if p < 0 else G[p] @ m4(R[i], T[i]))

def retarget(frames, src, tgt):
    """frames: (F, n, 12) native sample on skeleton src; returns (F, J, 7) local QT on the target rig."""
    body = len(src.names) > 21 and src.names[19] == 'left_femur_jnt'
    scale = np.linalg.norm(tgt.GT[19] - tgt.GT[21]) / np.linalg.norm(src.GT[19] - src.GT[21]) if body else 1.0
    sidx = {n: i for i, n in enumerate(src.names)}
    mapping = {i: sidx[n] for i, n in enumerate(tgt.names) if n in sidx}
    out = np.empty((len(frames), len(tgt.names), 7))
    for f, fr in enumerate(frames):
        SR = q2m(fr[:, :4]); ST = fr[:, 4:7].copy()
        SR[0] = SR[1] = np.eye(3); ST[0] = ST[1] = 0
        SGR, _ = src.fk(SR, ST)
        R = tgt.R.copy(); T = tgt.T.copy(); GR = np.empty_like(R)
        for i, p in enumerate(tgt.parent):
            if i in mapping and i >= 2 and 'helper' not in tgt.names[i]:
                k = mapping[i]
                R[i] = GR[p].T @ (SGR[k] @ src.GR[k].T @ tgt.GR[i])
                if i == 2 and body:
                    T[i] = tgt.T[i] + (fr[k, 4:7] - src.T[k]) * scale
            GR[i] = R[i] if p < 0 else GR[p] @ R[i]
        R[0] = R[1] = np.eye(3); T[0] = T[1] = 0
        helpers(tgt, R, T)
        out[f, :, :4] = m2q(R); out[f, :, 4:] = T
    return out

def by_name(frames, src, tgt):
    """Local rotations copied by joint name (from joint 3 on); the bind pose elsewhere."""
    out = np.broadcast_to(tgt.bind, (len(frames), len(tgt.names), 7)).copy()
    sidx = {n: i for i, n in enumerate(src.names)}
    for j, n in enumerate(tgt.names):
        if j >= 3 and n in sidx:
            q = frames[:, sidx[n], :4] / np.linalg.norm(frames[:, sidx[n], :4], axis=-1, keepdims=True)
            out[:, j, :4] = np.where(q[:, 3:4] < 0, -q, q)
    return out

def mirror(pose, tgt):
    """Left/right mirror across the rig's Y axis (the builders' mirrored()); joints 0-1 stay identity."""
    names = tgt.names; lookup = {n: i for i, n in enumerate(names)}
    pairs = [lookup.get('right' + n[4:] if n.startswith('left') else 'left' + n[5:] if n.startswith('right') else n, i) for i, n in enumerate(names)]
    F = len(pose); Rf = np.diag([1.0, -1.0, 1.0])
    R = q2m(pose[..., :4]); old = np.empty_like(R)
    for i, p in enumerate(tgt.parent):
        old[:, i] = R[:, i] if p < 0 else old[:, p] @ R[:, i]
    outR = np.broadcast_to(tgt.R, R.shape).copy(); outT = np.broadcast_to(tgt.T, (F, len(names), 3)).copy(); G = np.empty_like(R)
    for i, p in enumerate(tgt.parent):
        if i < 2:
            outR[:, i] = np.eye(3); outT[:, i] = 0
        else:
            k = pairs[i]
            outR[:, i] = np.swapaxes(G[:, p], -1, -2) @ (Rf @ old[:, k] @ tgt.GR[k].T @ Rf @ tgt.GR[i])
            if i == 2:
                outT[:, i] = pose[:, 2, 4:7] @ Rf.T
        G[:, i] = outR[:, i] if p < 0 else G[:, p] @ outR[:, i]
    out = np.empty_like(pose)
    for f in range(F):
        r = outR[f].copy(); t = outT[f].copy(); helpers(tgt, r, t)
        out[f, :, :4] = m2q(r); out[f, :, 4:] = t
    return out

# ---- Lab native samples. ----
_index = None
def _samples():
    global _index
    if _index is None:
        _index = {}
        for p in sorted(NM.glob('**/*.edgepose')):
            j = p.with_suffix('.json')
            if not j.exists():
                continue
            try:
                m = json.loads(j.read_text(encoding='utf-8-sig'))
            except ValueError:
                continue
            if isinstance(m, dict) and m.get('asset') and m.get('skeleton'):
                _index.setdefault(m['asset'], (p, m['skeleton']))
    return _index

_all = None
def _samples_all(asset):
    """Every lab sample of a native asset: [(path, skeleton name)]."""
    global _all
    if _all is None:
        _all = {}
        for p in sorted(NM.glob('**/*.edgepose')):
            j = p.with_suffix('.json')
            try:
                m = json.loads(j.read_text(encoding='utf-8-sig')) if j.exists() else None
            except ValueError:
                m = None
            if isinstance(m, dict) and m.get('asset') and m.get('skeleton'):
                _all.setdefault(m['asset'], []).append((p, m['skeleton']))
    return _all.get(asset, [])

def edgepose_frames(path):
    b = Path(path).read_bytes(); n, f = struct.unpack_from('<II', b, 8)
    return np.frombuffer(b, '<f4', f * n * 12, 24).reshape(f, n, 12).astype(np.float64)

def real_asset(asset):
    """Clip provenance names first-person samples by their lab file; map them to the native asset."""
    m = re.match(r'\(native edgepose sample\) (.*)\.edgepose\.md6anim$', asset)
    return asset if not m else json.loads((NM / (m.group(1) + '.json')).read_text(encoding='utf-8-sig'))['asset']

def sample_of(asset):
    """(frames (F, n, 12) float64, skeleton name) for a native asset, or None."""
    hit = _samples().get(real_asset(asset))
    if hit is None:
        return None
    b = hit[0].read_bytes(); n, f = struct.unpack_from('<II', b, 8)
    return np.frombuffer(b, '<f4', f * n * 12, 24).reshape(f, n, 12).astype(np.float64), hit[1]

ADD = re.compile(r'(.*/)(sword|empty)_(walk|run)_([nesw])_add\.md6anim$')
def additive_refs(asset):
    """Native additive clips and their references: sword_* use sword_neutral frame 0; empty_* use empty_walk_n
    frame 0 composed with the inverse of empty_walk_n_add frame 0 (empty-additive-reference.json, analytic)."""
    m = ADD.match(asset)
    if not m:
        return 0, []
    if m.group(2) == 'sword':
        return 1, [m.group(1) + 'sword_neutral.md6anim']
    return 2, [m.group(1) + 'empty_walk_n.md6anim', m.group(1) + 'empty_walk_n_add.md6anim']

def compose_additive(asset, frames):
    kind, refs = additive_refs(asset)
    if not kind:
        return frames
    if kind == 1:
        ref = sample_of(refs[0])[0][0]
    else:
        o = sample_of(refs[0])[0][0]; d = sample_of(refs[1])[0][0]
        ref = o.copy(); ref[:, :4] = qmul(o[:, :4], d[:, :4] * np.array([-1, -1, -1, 1.0])); ref[:, 4:7] = o[:, 4:7] - d[:, 4:7]
    out = frames.copy()
    out[..., :4] = qmul(ref[None, :, :4], frames[..., :4]); out[..., 4:7] = ref[None, :, 4:7] + frames[..., 4:7]
    return out
