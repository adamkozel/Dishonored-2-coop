"""Dump the live gameflow nodes of a lab game process (read-only).

Walks the gameflow manager (game+0x1f7780: buckets +0x18, mask +0x28, sorted
chains {id, node +8, next +0x10}; node id +8, state +0xc; confirmed static in
chapter 89). A node id is the 32-bit FNV-1a hash of the lowercased task,
objective or mission name (confirmed runtime: all 29 names of The Good
Doctor's gameflow tree match), so --base names every node from the game's own
models/gameflow/*.arktree files (read with d2_resource_index). Only reads
process memory; use it on the isolated lab instances, never on the user's
own game. Names are game-derived: keep the output local.

Usage:
  python -I d2_gameflow_dump.py <pid> --base work/asset-game/base [--grep REGEX] [--json out.json]
States: 0 inactive, 1 pending, 2 active, 3 complete, 4 failed, 5 canceled (hypothesis for 1, 4 and 5).
"""
import argparse
import ctypes
import ctypes.wintypes as wt
import json
import re

PROCESS_VM_READ, PROCESS_QUERY_INFORMATION = 0x10, 0x400
k32 = ctypes.WinDLL('kernel32', use_last_error=True)
psapi = ctypes.WinDLL('psapi', use_last_error=True)


class Proc:
    def __init__(self, pid):
        self.h = k32.OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, False, pid)
        if not self.h:
            raise SystemExit(f'OpenProcess failed ({ctypes.get_last_error()})')
        mods = (wt.HMODULE * 1)()
        need = wt.DWORD()
        if not psapi.EnumProcessModules(self.h, mods, ctypes.sizeof(mods), ctypes.byref(need)):
            raise SystemExit('EnumProcessModules failed')
        self.base = mods[0]

    def read(self, addr, size):
        buf = ctypes.create_string_buffer(size)
        got = ctypes.c_size_t()
        if not addr or not k32.ReadProcessMemory(self.h, ctypes.c_void_p(addr), buf, size, ctypes.byref(got)) or got.value != size:
            return None
        return buf.raw

    def u64(self, addr):
        b = self.read(addr, 8)
        return int.from_bytes(b, 'little') if b else 0

    def u32(self, addr):
        b = self.read(addr, 4)
        return int.from_bytes(b, 'little') if b else 0


NAME = re.compile(rb'(?:Task|Obj|Mission)_[A-Za-z0-9_ ]{3,80}')


def find_name(p, addr, depth, seen):
    """Return the first gameflow-like ASCII name reachable from addr."""
    if depth < 0 or addr in seen or addr < 0x10000:
        return None
    seen.add(addr)
    raw = p.read(addr, 0x80)
    if not raw:
        return None
    m = NAME.match(raw)
    if m:
        return m.group(0).decode()
    for off in range(0, 0x80, 8):
        ptr = int.from_bytes(raw[off:off + 8], 'little')
        if 0x10000 < ptr < 0x7fffffffffff:
            name = find_name(p, ptr, depth - 1, seen)
            if name:
                return name
    return None


def fnv1a_lower(name):
    h = 2166136261
    for c in name.lower().encode():
        h = ((h ^ c) * 16777619) & 0xffffffff
    return h


def gameflow_names(base):
    """All task, objective and mission names of the game's gameflow trees, by node id."""
    import sys as _sys
    from pathlib import Path as _P
    _sys.path.insert(0, str(_P(__file__).resolve().parent))
    import d2_resource_index as r
    names = {}
    for ix in r.containers(base):
        for e in r.entries(ix):
            if e['type'] == 'arkTree' and e['source'].startswith('models/gameflow/'):
                text = r.read(ix, e).decode('utf-8', 'replace')
                for n in re.findall(r'm_(?:taskName|objectiveName|missionName) = "([^"]+)"', text):
                    names[fnv1a_lower(n)] = n
    return names


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('pid', type=int)
    ap.add_argument('--depth', type=int, default=2)
    ap.add_argument('--grep', default='')
    ap.add_argument('--json')
    ap.add_argument('--base', help='game base directory: name nodes from its gameflow trees')
    a = ap.parse_args()
    known = gameflow_names(a.base) if a.base else {}
    p = Proc(a.pid)
    game = p.u64(p.base + 0x39a5290)
    mgr = game + 0x1f7780
    buckets, mask = p.u64(mgr + 0x18), p.u32(mgr + 0x28)
    if not game or not buckets or mask > 0xffff:
        raise SystemExit('gameflow manager not readable')
    rows = []
    for b in range(mask + 1):
        entry = p.u64(buckets + b * 8)
        hops = 0
        while entry and hops < 256:
            node, nxt = p.u64(entry + 8), p.u64(entry + 0x10)
            if node:
                nid = p.u32(node + 8)
                rows.append({'id': nid, 'state': p.u32(node + 0xc), 'name': known.get(nid) or (None if known else find_name(p, node, a.depth, set()))})
            entry, hops = nxt, hops + 1
    rx = re.compile(a.grep, re.I) if a.grep else None
    for r in sorted(rows, key=lambda r: (r['name'] or '~', r['id'])):
        if rx and not rx.search(r['name'] or ''):
            continue
        print(f"{r['state']} {r['id']:>10} {r['name']}")
    if a.json:
        with open(a.json, 'w') as f:
            json.dump(rows, f, indent=1)


if __name__ == '__main__':
    main()
