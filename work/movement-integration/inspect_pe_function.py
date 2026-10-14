"""Find exact-image x64 exception-table boundaries, without guessing function starts."""
from pathlib import Path
import argparse,struct,hashlib,json
p=argparse.ArgumentParser();p.add_argument('rva',type=lambda s:int(s,16));a=p.parse_args()
R=Path(__file__).resolve().parents[2];b=(R/'work/asset-game/Dishonored2.exe').read_bytes()
assert hashlib.sha256(b).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
u16=lambda p:struct.unpack_from('<H',b,p)[0];u32=lambda p:struct.unpack_from('<I',b,p)[0]
pe=u32(0x3c);opt=pe+24;assert u16(opt)==0x20b
sections=[]
for i in range(u16(pe+6)):
 s=opt+u16(pe+20)+i*40;sections.append((u32(s+12),u32(s+16),u32(s+20)))
def raw(v):
 for r,n,o in sections:
  if r<=v<r+n:return o+v-r
 raise ValueError(hex(v))
rva,size=struct.unpack_from('<II',b,opt+112+3*8);rows=[]
for off in range(raw(rva),raw(rva)+size,12):
 begin,end,unwind=struct.unpack_from('<III',b,off)
 if begin<=a.rva<end:rows.append(dict(start=hex(begin),end=hex(end),unwind=hex(unwind),prologue=b[raw(begin):raw(begin)+32].hex()))
print(json.dumps(dict(query=hex(a.rva),matches=rows),indent=2))
