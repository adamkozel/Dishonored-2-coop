"""Read-only exact PE string and unwind-boundary lookup for native consumables."""
from pathlib import Path
import struct,re,json,hashlib
R=Path(__file__).resolve().parent.parent
raw=(R/'work/asset-game/Dishonored2.exe').read_bytes()
assert hashlib.sha256(raw).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
u16=lambda at:struct.unpack_from('<H',raw,at)[0]
u32=lambda at:struct.unpack_from('<I',raw,at)[0]
pe=u32(0x3c);opt=pe+24;sect=opt+u16(pe+20)
sections=[]
for i in range(u16(pe+6)):
    at=sect+i*40
    sections.append(dict(name=raw[at:at+8].rstrip(b'\0').decode(),vsize=u32(at+8),rva=u32(at+12),size=u32(at+16),offset=u32(at+20)))
def address(offset):
    for s in sections:
        if s['offset']<=offset<s['offset']+s['size']:return hex(0x140000000+s['rva']+offset-s['offset'])
def offset(rva):
    for s in sections:
        if s['rva']<=rva<s['rva']+s['size']:return s['offset']+rva-s['rva']
matches=[]
for m in re.finditer(rb'[\x20-\x7e]{6,}',raw):
    if re.search(rb'potion|elixir|elixer',m.group(),re.I):matches.append(dict(address=address(m.start()),text=m.group().decode()))
tableRva,tableSize=struct.unpack_from('<II',raw,opt+112+3*8)
table=offset(tableRva);functions=[]
for p in range(table,table+tableSize,12):
    begin,end,unwind=struct.unpack_from('<III',raw,p)
    if begin<=0x6745d4<end:functions.append(dict(begin=hex(0x140000000+begin),end=hex(0x140000000+end),unwind=hex(0x140000000+unwind)))
result=dict(strings=matches,quickPotionReferenceUnwind=functions)
(R/'work/potion-pe-inspection-v64.json').write_text(json.dumps(result,indent=2))
print(json.dumps(result,indent=2))
