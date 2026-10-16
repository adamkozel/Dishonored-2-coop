"""Print the live graph variables of Kiscule script instances (read-only).

An entity's script instance comes from the Kiscule manager (game+0x1f6e48:
buckets +0x18, mask +0x28; chain {entity handle, instance +8, next +0x10}).
The instance's graph (+0x70) lists its variables at +0x40 (count in the low
24 bits of +0x48). A variable keeps its name at +8; for a bool the default is
at +0x18 and the current value at +0x19 (0x1408f7f10 copies the default
into the value on init; confirmed static). The story flag decls
(storyflags/*.kiscule) are instances in the worldspawn instance's table at
+0x88 (16 buckets; chain {key, instance +8, next +0x10}), named by their
decl (+0x68, name at +8). Only reads process memory; use it on the
isolated lab instances. Names are game-derived: keep the output local.

Usage:
  python -I d2_kiscule_vars.py <pid> <entity name | storyflags> [--grep REGEX] [--nested] [--raw]
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from d2_gameflow_dump import Proc  # noqa: E402

PROBE = Path(__file__).resolve().parents[2] / 'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'


def text(p, addr, n=96):
    raw = p.read(addr, n) if addr else None
    if not raw:
        return None
    s = raw.split(b'\0')[0]
    return s.decode('latin1') if re.fullmatch(rb'[\x20-\x7e]{1,95}', s) else None


def instance_for(p, game, handle):
    mgr = game + 0x1f6e48
    buckets, mask = p.u64(mgr + 0x18), p.u32(mgr + 0x28)
    if not buckets or mask > 0xffff:
        return 0
    for b in range(mask + 1):
        node, hops = p.u64(buckets + b * 8), 0
        while node and hops < 256:
            if p.u32(node) == handle:
                return p.u64(node + 8)
            node, hops = p.u64(node + 0x10), hops + 1
    return 0


def variables(p, inst):
    graph = p.u64(inst + 0x70)
    arr, count = p.u64(graph + 0x40), p.u32(graph + 0x48) & 0xffffff
    for i in range(min(count, 512)):
        v = p.u64(arr + i * 8)
        name = text(p, p.u64(v + 8))
        raw = p.read(v + 0x10, 0x10) or b''
        yield name, raw, v


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('pid', type=int)
    ap.add_argument('entity')
    ap.add_argument('--grep', default='')
    ap.add_argument('--nested', action='store_true', help='read the decl instances the entity owns instead of its own graph')
    ap.add_argument('--raw', action='store_true', help='also dump the words of each variable object (int and list layouts)')
    a = ap.parse_args()
    p = Proc(a.pid)
    game = p.u64(p.base + 0x39a5290)
    import json
    ents = json.loads(subprocess.run([str(PROBE), str(a.pid), '--entities'], capture_output=True, text=True).stdout)['entities']
    rx = re.compile(a.grep, re.I) if a.grep else None
    targets = []
    def nested(owner):
        # Decl instances an instance owns (story flags on the worldspawn, a DeclInstance node's decl): table +0x88.
        table, n = p.u64(owner + 0x88), p.u32(owner + 0x90) & 0xffffff
        for b in range(min(n, 4096)):
            node, hops = p.u64(table + b * 8), 0
            while node and hops < 64:
                inst = p.u64(node + 8)
                decl = p.u64(inst + 0x68)
                targets.append((text(p, p.u64(decl + 8)) or hex(inst), inst))
                node, hops = p.u64(node + 0x10), hops + 1
    if a.entity == 'storyflags':
        nested(instance_for(p, game, next(int(e['handle'], 16) for e in ents if e['type'] == '.?AVidWorldspawn@@')))
    else:
        for e in ents:
            if e['name'] == a.entity:
                inst = instance_for(p, game, int(e['handle'], 16))
                if a.nested and inst:
                    nested(inst)
                else:
                    targets.append((a.entity, inst))
    if not targets:
        raise SystemExit('no instance for ' + a.entity)
    for label, inst in targets:
        if not inst:
            print(f'{label}: no script instance')
            continue
        for name, raw, v in variables(p, inst):
            line = f'{label} {name}: default {raw[8]} value {raw[9]} (bytes {raw.hex()})'
            if rx and not rx.search(line):
                continue
            print(line)
            if a.raw:
                # Unknown layouts (int, lists): the object's words and, for heap pointers, the first ints they point at.
                for off in range(0x10, 0x50, 8):
                    w = p.u64(v + off)
                    ints = ''
                    if 0x10000 < w < 0x7fffffffffff:
                        blob = p.read(w, 40)
                        ints = ' -> ' + ' '.join(str(int.from_bytes(blob[i:i + 4], 'little', signed=True)) for i in range(0, 40, 4)) if blob else ''
                    print(f'    +{off:#04x} {w:#018x}{ints}')


if __name__ == '__main__':
    main()
