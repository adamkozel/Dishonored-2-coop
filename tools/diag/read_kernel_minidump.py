"""Reads a Windows 64-bit kernel minidump (PAGEDU64, small/triage dump) without a debugger:
bugcheck code and parameters, the loaded-driver list, the driver containing the faulting
instruction (parameter 3 / context RIP), and a heuristic stack scan that maps return-address-like
values on the crashing thread's stack to drivers.

Usage: python read_kernel_minidump.py <dump.dmp>
Layouts: DUMP_HEADER64 (0x2000 bytes) followed by TRIAGE_DUMP64; offsets in TRIAGE_DUMP64 are
file offsets. DUMP_DRIVER_ENTRY64 = { ULONG DriverNameOffset; KLDR_DATA_TABLE_ENTRY LdrEntry }
whose size varies by build, so the stride is inferred by validating every entry.
"""
import struct, sys, collections

def u32(b, o): return struct.unpack_from('<I', b, o)[0]
def u64(b, o): return struct.unpack_from('<Q', b, o)[0]

def dump_string(b, off):
    n = u32(b, off)
    if n == 0 or n > 512 or off + 4 + 2 * n > len(b): return None
    try: return b[off + 4: off + 4 + 2 * n].decode('utf-16-le')
    except UnicodeDecodeError: return None

def main(path):
    b = open(path, 'rb').read()
    if b[:8] != b'PAGEDU64': sys.exit(f'not a 64-bit kernel dump (signature {b[:8]!r})')
    code = u32(b, 0x38); params = [u64(b, 0x40 + 8 * i) for i in range(4)]
    dump_type = u32(b, 0xF98)
    print(f'bugcheck 0x{code:X} ({", ".join(f"0x{p:016X}" for p in params)}) dumpType={dump_type} build={u32(b, 0x0C)}')
    t = 0x2000
    names = ['ServicePackBuild', 'SizeOfDump', 'ValidOffset', 'ContextOffset', 'ExceptionOffset', 'MmOffset', 'UnloadedDriversOffset',
             'PrcbOffset', 'ProcessOffset', 'ThreadOffset', 'CallStackOffset', 'SizeOfCallStack', 'DriverListOffset', 'DriverCount',
             'StringPoolOffset', 'StringPoolSize', 'BrokenDriverOffset', 'TriageOptions']
    tri = {n: u32(b, t + 4 * i) for i, n in enumerate(names)}
    top_of_stack = u64(b, t + 0x48)
    # Infer the driver entry stride: every entry's name offset must decode inside the string pool,
    # and its DllBase (LdrEntry+0x30) must be a kernel-space address.
    lo, cnt = tri['DriverListOffset'], tri['DriverCount']
    pool_lo, pool_hi = tri['StringPoolOffset'], tri['StringPoolOffset'] + tri['StringPoolSize']
    drivers = None
    for stride in range(0x80, 0x200, 8):
        rows = []
        for i in range(cnt):
            e = lo + i * stride
            if e + 8 + 0x48 > len(b): break
            no = u32(b, e); base = u64(b, e + 8 + 0x30); size = u32(b, e + 8 + 0x40)
            name = dump_string(b, no) if pool_lo <= no < pool_hi else None
            if not name or base >> 47 != 0x1FFFF or not (0 < size < 0x10000000): break
            rows.append((base, size, name))
        if len(rows) == cnt: drivers = rows; break
    if not drivers: sys.exit(f'could not decode the driver list ({cnt} entries at 0x{lo:X})')
    drivers.sort()
    def owner(addr):
        for base, size, name in drivers:
            if base <= addr < base + size: return f'{name}+0x{addr - base:X}'
        return None
    print(f'{len(drivers)} drivers decoded')
    rip = u64(b, tri['ContextOffset'] + 0xF8) if tri['ContextOffset'] else 0
    for label, addr in (('bugcheck parameter 3 (faulting IP)', params[2]), ('context RIP', rip)):
        if addr: print(f'{label}: 0x{addr:016X} -> {owner(addr) or "not in any loaded driver"}')
    # Heuristic stack scan: qwords on the saved stack that land inside a driver image.
    st, sz = tri['CallStackOffset'], tri['SizeOfCallStack']
    hits = []
    for o in range(st, min(st + sz, len(b)) - 7, 8):
        v = u64(b, o); w = owner(v)
        if w: hits.append((o - st, w))
    print(f'stack scan (top 0x{top_of_stack:016X}, {sz} bytes): {len(hits)} driver addresses, first 25:')
    for off, w in hits[:25]: print(f'  +0x{off:04X} {w}')
    print('drivers on stack:', ', '.join(f'{n} x{c}' for n, c in collections.Counter(w.split('+')[0] for _, w in hits).most_common(10)))

if __name__ == '__main__':
    main(sys.argv[1])
