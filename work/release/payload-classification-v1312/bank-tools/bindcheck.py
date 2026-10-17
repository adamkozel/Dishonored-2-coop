import json, struct, sys
from array import array
W = r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work"
BS = chr(92)
out = {}
for role in ('corvo', 'emily'):
    rig = json.load(open(W + BS + "mixamo-crouch" + BS + "native-" + role + ".json", encoding='utf-8-sig'))
    bind = [j['bindLocalQTS'][4:7] for j in rig['joints']]
    n = len(bind)
    raw = open(W + BS + "natural-moveset" + BS + "use-banks-v1212" + BS + role + "-choke-bank.bin", 'rb').read()
    magic, v, nn, count, fps, lay = struct.unpack_from('<8sIIIIQ', raw)
    assert nn == n
    entry = 24 + 121 * n * 28
    per_clip = []
    joint_dev = [0.0] * n
    for c in range(count):
        a = array('f')
        a.frombytes(raw[32 + c * entry + 24: 32 + (c + 1) * entry])
        dev = [0.0] * n
        for f in range(121):
            base = f * n * 7
            for j in range(n):
                k = base + j * 7 + 4
                b = bind[j]
                d = max(abs(a[k] - b[0]), abs(a[k + 1] - b[1]), abs(a[k + 2] - b[2]))
                if d > dev[j]:
                    dev[j] = d
        for j in range(n):
            if dev[j] > joint_dev[j]:
                joint_dev[j] = dev[j]
        nonbind = [j for j in range(n) if dev[j] > 1e-4]
        per_clip.append(dict(clip=c, joints_off_bind=nonbind, max_dev_excl_0_2=max(dev[3:])))
    off = [(j, rig['joints'][j]['nativeName'], round(joint_dev[j], 4)) for j in range(n) if joint_dev[j] > 1e-4]
    out[role] = dict(joints=n, skeleton=rig.get('skeleton'), joints_ever_off_bind=off,
                     clips_all_bind_beyond_joint2=sum(1 for p in per_clip if all(j < 3 for j in p['joints_off_bind'])),
                     clips=per_clip)
    print(role, n, 'joints whose translation ever leaves native bind:', off[:40])
    print(role, 'clips whose translations equal native bind for every joint >=3:', out[role]['clips_all_bind_beyond_joint2'], '/', count)
    bad = [(p['clip'], p['joints_off_bind'][:10]) for p in per_clip if any(j >= 3 for j in p['joints_off_bind'])]
    print(role, 'clips with non-bind translations beyond joint 2:', len(bad), bad[:60])
json.dump(out, open(sys.argv[1], 'w'), indent=0)
