"""Bank 1266 (visual sweep 1260, take 1265a): the grenade release and recovery clips carry ~0.75 m of authored horizontal
pelvis travel (standing clips 256/257; crouched 269/270 ~0.45 m) while the native network root stays still, so the partner
jumped sideways at release and drifted back. Scales that horizontal pelvis travel (joint 2 local x/y relative to the
release clip's first frame) to 25% in those four clips; every other clip payload stays byte-identical. Same version 71
and clip count. Writes use-banks-v1266/<role>-choke-bank.bin and use-bank-manifest-v1266.json."""
import struct, hashlib, json, math
from pathlib import Path
P = Path(__file__).resolve().parent
SCALE = 0.25; CLIPS = {256: 256, 257: 256, 269: 269, 270: 269}  # clip -> clip whose first frame anchors the pelvis
out_dir = P / 'use-banks-v1266'; out_dir.mkdir(exist_ok=True); manifest = []
for role in ('corvo', 'emily'):
    src = P / f'use-banks-v1212/{role}-choke-bank.bin'; data = bytearray(src.read_bytes())
    assert data[:8] == b'D2MOVE71'
    clips = 374; entry = (len(data) - 32) // clips; n = (entry - 24) // (121 * 28); assert 32 + clips * entry == len(data)
    at = lambda c, f, j: 32 + c * entry + 24 + (f * n + j) * 28
    report = {}
    for c, anchor in CLIPS.items():
        ax, ay = struct.unpack_from('<2f', data, at(anchor, 0, 2) + 16)
        before = after = 0.0
        for f in range(121):
            o = at(c, f, 2) + 16; x, y = struct.unpack_from('<2f', data, o)
            nx, ny = ax + SCALE * (x - ax), ay + SCALE * (y - ay)
            before = max(before, math.hypot(x - ax, y - ay)); after = max(after, math.hypot(nx - ax, ny - ay))
            struct.pack_into('<2f', data, o, nx, ny)
        report[c] = dict(anchorClip=anchor, maxPelvisTravelBefore=round(before, 3), maxPelvisTravelAfter=round(after, 3))
    original = src.read_bytes()
    for c in range(clips):
        if c not in CLIPS: assert data[32 + c * entry:32 + (c + 1) * entry] == original[32 + c * entry:32 + (c + 1) * entry]
    dst = out_dir / f'{role}-choke-bank.bin'; dst.write_bytes(bytes(data))
    manifest.append(dict(role=role, bank=f'use-banks-v1266\\{role}-choke-bank.bin', sha256=hashlib.sha256(bytes(data)).hexdigest().upper(), version=71, clips=clips,
                         source_bank_sha256=hashlib.sha256(original).hexdigest().upper(), otherPayloadsUnchanged=True, scale=SCALE, patched=report))
(P / 'use-bank-manifest-v1266.json').write_text(json.dumps(manifest, indent=1), encoding='utf-8')
print(json.dumps(manifest, indent=1)[:1600])
