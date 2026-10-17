"""Print the Dust District's Jindosh lock state (read-only; game-derived output stays local).

The lock (entity ld_enigma_v2_kiscule) owns a jindosh_enigma.kiscule decl
instance, which owns one jindosh_enigma_wheel.kiscule instance per seat.
Confirmed static (the wheel decl): a wheel's family stage maps through
TranslatorFamily to a family id, whose position in RandomFamily is
WheelFamilyIndex (the same for heirlooms); a seat is right when both
indices are equal, and the lock opens when the five seats hold the five
indices (CombinationIndexSum 31; take route-dd-v1306a dialled seat i to
index i). Each use advances a wheel one stage. Int variables keep their
value at +0x1c; nested decl tables can have 256 buckets.

Usage: python -I d2_jindosh_lock.py <repo> <pid>
Prints JSON: {combination: {...}, wheels: [{WheelFamily, WheelHeirloom,
WheelFamilyIndex, WheelHeirloomIndex, CombinationFound, CombinationBinaryIndex}]}
"""
import json
import subprocess
import sys

repo, pid = sys.argv[1], int(sys.argv[2])
sys.path.insert(0, repo + '/tools/diag')
import d2_kiscule_vars as kv  # noqa: E402
from d2_gameflow_dump import Proc  # noqa: E402

p = Proc(pid)
game = p.u64(p.base + 0x39a5290)
ents = json.loads(subprocess.run([str(kv.PROBE), str(pid), '--entities'], capture_output=True, text=True).stdout)['entities']
byh = {int(e['handle'], 16): e['name'] for e in ents}


def nested(inst):
    out = []
    table, n = p.u64(inst + 0x88), p.u32(inst + 0x90) & 0xffffff
    for b in range(min(n, 4096)):
        node, h = p.u64(table + b * 8), 0
        while node and h < 64:
            out.append(p.u64(node + 8))
            node, h = p.u64(node + 0x10), h + 1
    return out


def declname(i):
    return kv.text(p, p.u64(p.u64(i + 0x68) + 8)) or ''


def ival(v):
    return int.from_bytes(p.read(v + 0x1c, 4), 'little', signed=True)


lock = kv.instance_for(p, game, next(int(e['handle'], 16) for e in ents if e['name'] == 'ld_enigma_v2_kiscule'))
enig = [i for i in nested(lock) if declname(i).endswith('jindosh_enigma.kiscule')][0]
out = {'combination': {}, 'wheels': []}
for name, raw, v in kv.variables(p, enig):
    if name and (name.startswith('Combination') or name == 'CombinationIndexSum'):
        out['combination'][name] = ival(v)
for w in nested(enig):
    if 'wheel' not in declname(w):
        continue
    row = {}
    for name, raw, v in kv.variables(p, w):
        if name in ('WheelFamily', 'WheelHeirloom'):
            row[name] = [byh[x] for x in (p.u32(v + o) for o in range(0x10, 0x30, 4)) if x in byh and x != 0 and byh[x].startswith('ld_wheel')]
        elif name in ('WheelFamilyIndex', 'WheelHeirloomIndex', 'CombinationBinaryIndex'):
            row[name] = ival(v)
        elif name == 'CombinationFound':
            row[name] = raw[9]
    out['wheels'].append(row)
print(json.dumps(out))
