"""#30: builds the release's rebuild data for game-derived payloads (outputs/d2-coop-lab/src/bank_rebuild.h and
table_rebuild.h). The release ships no bank and no native table: the installer rebuilds them from the player's own
animations with `d2coop_setup banks` and `d2coop_setup tables`.

1. `d2coop_setup bank-encode` / `tables-encode` rebuild the base from the game install (read only) and the
   parameters (fit_bank_bases.py / build_native_tables.py), measure the residual against our data and write the
   patch or recipe with a stored body.
2. This script recompresses the body (zlib level 9) and checks the file still parses.
3. `d2coop_setup banks` / `tables` rebuild from the install and the final file; every value must be within half a
   residual step of our data, and the result must equal the encoder's rebuild on this machine.

Usage: python -I build_bank_patch.py bank SETUP_EXE GAME_DIR PARAMS LAB_BANK OUT_PATCH WORK_DIR
       python -I build_bank_patch.py tables SETUP_EXE GAME_DIR PARAMS REFERENCE_TABLES OUT_RECIPE WORK_DIR
"""
import array, hashlib, json, struct, subprocess, sys, zlib
from pathlib import Path

def run(args):
    r = subprocess.run([str(a) for a in args], capture_output=True, text=True)
    if r.returncode:
        raise SystemExit(f'{args[1]} failed ({r.returncode}): {r.stderr.strip()}')
    return json.loads(r.stdout.strip().splitlines()[-1])

def recompress(b):
    """D2BNK001 / D2NTR001: hashes, (bank header), sums, step, body size, stream size, zlib body."""
    magic = b[:8]
    if magic not in (b'D2BNK001', b'D2NTR001'):
        raise SystemExit('not a stored patch or recipe')
    at = 8 + 32
    for _ in range(2):
        n = struct.unpack_from('<I', b, at)[0]; at += 4 + 32 * n
    if magic == b'D2BNK001':
        at += 32
    n = struct.unpack_from('<I', b, at)[0]; at += 4 + 16 * n
    at += 4
    body_size, stream_size = struct.unpack_from('<QI', b, at); head = b[:at]; at += 12
    body = zlib.decompress(b[at:at + stream_size])
    if len(body) != body_size or at + stream_size != len(b):
        raise SystemExit('unexpected stored layout')
    stream = zlib.compress(body, 9)
    return head + struct.pack('<QI', body_size, len(stream)) + stream

def bank_error(rebuilt, lab):
    if len(rebuilt) != len(lab) or rebuilt[:32] != lab[:32]:
        raise SystemExit('rebuilt bank layout differs')
    joints, clips = struct.unpack_from('<II', lab, 12)
    size = 24 + 121 * joints * 28; worst = 0.0
    for c in range(clips):
        o = 32 + c * size
        if rebuilt[o:o + 24] != lab[o:o + 24]:
            raise SystemExit(f'clip {c} header differs')
        a = array.array('f', rebuilt[o + 24:o + size]); b = array.array('f', lab[o + 24:o + size])
        worst = max(worst, max(abs(x - y) for x, y in zip(a, b)))
    return worst

def tables(b):
    at = 8; n = struct.unpack_from('<I', b, at)[0]; at += 4; out = {}
    for _ in range(n):
        ln = struct.unpack_from('<I', b, at)[0]; name = b[at + 4:at + 4 + ln].decode(); at += 4 + ln
        kind, rank = struct.unpack_from('<BI', b, at); at += 5
        dims = struct.unpack_from(f'<{rank}I', b, at); at += 4 * rank; count = 1
        for d in dims: count *= d
        size = count * (2 if kind else 4); out[name] = (kind, dims, array.array('h' if kind else 'f', b[at:at + size])); at += size
    return out

def table_error(rebuilt, reference):
    a, b = tables(rebuilt), tables(reference); worst = 0.0
    for name, (kind, dims, values) in a.items():
        k2, d2, v2 = b[name]
        if (kind, dims) != (k2, d2):
            raise SystemExit('table layout differs: ' + name)
        worst = max(worst, max((abs(x - y) for x, y in zip(values, v2)), default=0.0))
    if set(a) != set(b):
        raise SystemExit('table set differs')
    return worst

def main():
    mode = sys.argv[1]; setup, game, params, lab, out, work = (Path(a) for a in sys.argv[2:8])
    work.mkdir(parents=True, exist_ok=True)
    stored = work / (out.stem + '.stored.bin')
    if stored.exists() or out.exists():
        raise SystemExit('outputs exist; use a new work dir and output name')
    encoded = run([setup, 'bank-encode' if mode == 'bank' else 'tables-encode', game, params, lab, stored])
    out.write_bytes(recompress(stored.read_bytes()))
    rebuilt_dir = work / (out.stem + '-rebuilt')
    decoded = run([setup, 'banks' if mode == 'bank' else 'tables', game, out, rebuilt_dir])
    name = decoded['bank'] if mode == 'bank' else 'native-tables.bin'
    rebuilt = (rebuilt_dir / name).read_bytes()
    worst = bank_error(rebuilt, lab.read_bytes()) if mode == 'bank' else table_error(rebuilt, lab.read_bytes())
    step = 2.0 ** -17 if mode == 'bank' else 2.0 ** -20
    report = dict(mode=mode, file=out.name, bytes=out.stat().st_size, sha256=hashlib.sha256(out.read_bytes()).hexdigest(),
                  storedBytes=stored.stat().st_size, encoded=encoded, decoded=decoded, maximumError=worst,
                  sameAsEncoder=decoded['sha256'] == encoded['rebuiltSha256'])
    if not report['sameAsEncoder'] or worst > step / 2 * 1.05:
        raise SystemExit(json.dumps(report))
    print(json.dumps(report))

if __name__ == '__main__':
    main()
