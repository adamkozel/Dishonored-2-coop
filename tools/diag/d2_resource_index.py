"""Read-only reader for Dishonored 2 resource containers (.index + .resources).

Usage:
  python -I d2_resource_index.py list <base-dir> [--type T] [--match REGEX]
  python -I d2_resource_index.py grep <base-dir> --type entities --match REGEX --text REGEX

Never writes into the game folder. Point it at a game copy (work/asset-game/base).
Index layout (confirmed static against work/game1.index.json): magic 05 'SER', BE size,
24 zero bytes, BE entry count, then per entry BE id, LE-length type/source/destination
strings, BE u64 offset, BE u32 size, BE u32 packed size, BE u64 + BE u16 flags.
Packed entries (packed != size) are zlib streams. flags2 selects the .resources file
(confirmed static: every campaign .entities blob decompresses to its declared size).
Use the newest index of a family (gameNvoidmod.index) for the current data.
"""
import argparse, json, re, struct, sys, zlib
from pathlib import Path


def entries(index_path):
    d = index_path.read_bytes()
    if d[:4] != b'\x05SER':
        raise ValueError(f'{index_path.name}: not a resource index')
    count = struct.unpack_from('>I', d, 0x20)[0]
    at = 0x24
    for _ in range(count):
        rid = struct.unpack_from('>I', d, at)[0]; at += 4
        parts = []
        for _s in range(3):
            n = struct.unpack_from('<I', d, at)[0]; at += 4
            parts.append(d[at:at + n].decode('utf-8', 'replace')); at += n
        offset, size, packed, f1, f2 = struct.unpack_from('>QIIQH', d, at); at += 26
        yield {'id': rid, 'type': parts[0], 'source': parts[1], 'destination': parts[2],
               'offset': offset, 'size': size, 'packed': packed, 'flags1': f1, 'flags2': f2}


SUFFIXES = ['', '_001', '_002', '_003', '_004', '_005', '_patch', 'voidmod']


def resources_for(index_path, e):
    """flags2 names the container holding the data: bits 0-1 game number - 1, bits 2+ the patch level."""
    family = f"game{(e['flags2'] & 3) + 1}"
    return index_path.with_name(family + SUFFIXES[e['flags2'] >> 2] + '.resources')


def read(index_path, e):
    with open(resources_for(index_path, e), 'rb') as f:
        f.seek(e['offset']); raw = f.read(e['packed'])
    return raw if e['packed'] == e['size'] else zlib.decompress(raw)


def containers(base):
    for n in (1, 2, 3):
        ix = Path(base) / f'game{n}voidmod.index'
        if ix.exists():
            yield ix


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('mode', choices=['list', 'grep'])
    ap.add_argument('base')
    ap.add_argument('--type')
    ap.add_argument('--match', default='')
    ap.add_argument('--text')
    a = ap.parse_args()
    m = re.compile(a.match, re.I); t = re.compile(a.text, re.I) if a.text else None
    for ix in containers(a.base):
        for e in entries(ix):
            if a.type and e['type'] != a.type:
                continue
            if not m.search(e['source']):
                continue
            if a.mode == 'list':
                print(json.dumps({'container': ix.name, **e}))
                continue
            try:
                body = read(ix, e).decode('utf-8', 'replace')
            except Exception as ex:
                print(json.dumps({'container': ix.name, 'source': e['source'], 'error': str(ex)}))
                continue
            for line in body.splitlines():
                if t.search(line):
                    print(json.dumps({'container': ix.name, 'source': e['source'], 'line': line.strip()[:240]}))


if __name__ == '__main__':
    sys.exit(main())
