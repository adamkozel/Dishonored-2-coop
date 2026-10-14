from pathlib import Path
import hashlib, struct, json
R = Path(__file__).resolve().parents[2]
p = R / 'work/asset-game/Dishonored2.exe'
b = p.read_bytes()
assert hashlib.sha256(b).hexdigest().upper() == 'B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
pe = struct.unpack_from('<I', b, 0x3c)[0]
count = struct.unpack_from('<H', b, pe+6)[0]
opt = struct.unpack_from('<H', b, pe+20)[0]
sections = pe+24+opt
found = []
found += [dict(address=hex(0x140000000+x), reason='native pause caller') for x in (0x49c5cd,0x49c645,0x49d37e,0x4d2243,0x813421)]
def raw_rva(address):
    for sn in range(count):
        at = sections+sn*40
        vs, vr, sz, ro = struct.unpack_from('<IIII', b, at+8)
        if vr <= address < vr+sz: return ro+address-vr
    raise ValueError(hex(address))
for n in range(count):
    off = sections+n*40
    name = b[off:off+8].rstrip(b'\0').decode()
    virtual_size, rva, size, raw = struct.unpack_from('<IIII', b, off+8)
    if name != '.text': continue
    data = b[raw:raw+size]
    needle = bytes.fromhex('7c02000005000000')
    pos = data.find(needle)
    while pos >= 0:
        found.append(dict(address=hex(0x140000000+rva+pos), bytes=data[max(0,pos-8):pos+16].hex()))
        pos = data.find(needle, pos+1)
for n in range(count):
    off = sections+n*40
    name = b[off:off+8].rstrip(b'\0').decode()
    virtual_size, rva, size, raw = struct.unpack_from('<IIII', b, off+8)
    if name != '.pdata': continue
    for off in range(raw, raw+min(size,virtual_size)-11, 12):
        begin, end, unwind = struct.unpack_from('<III', b, off)
        for item in found:
            site = int(item['address'],16)-0x140000000
            if begin <= site < end:
                item['function'] = hex(0x140000000+begin)
                item['end'] = hex(0x140000000+end)
                chain = []
                while b[raw_rva(unwind)] >> 3 & 4:
                    u = raw_rva(unwind)
                    begin, end, unwind = struct.unpack_from('<III', b, u+4+((b[u+2]+1)&~1)*2)
                    chain.append(hex(0x140000000+begin))
                    if len(chain)>8: raise ValueError('unwind chain')
                item['unwindChain'] = chain
                item['parentFunction'] = hex(0x140000000+begin)
(Path(__file__).with_name('midnight-state5-sites-v454.json')).write_text(json.dumps(found, indent=2))
print(json.dumps(found))
