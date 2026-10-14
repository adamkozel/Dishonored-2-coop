"""Read-only exact-PE x64 unwind ranges for targeted Ghidra function lookup."""
from pathlib import Path
import struct,hashlib,json,sys
R=Path(__file__).resolve().parents[2];exe=R/'work/asset-game/Dishonored2.exe';b=exe.read_bytes()
assert hashlib.sha256(b).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
u16=lambda p:struct.unpack_from('<H',b,p)[0]
u32=lambda p:struct.unpack_from('<I',b,p)[0]
nt=u32(0x3c);assert b[nt:nt+4]==b'PE\0\0';op=nt+24;assert u16(op)==0x20b
sections=[]
for i in range(u16(nt+6)):
 p=op+u16(nt+20)+40*i;sections.append((u32(p+12),u32(p+16),u32(p+20)))
def offset(rva):
 for va,sz,raw in sections:
  if va<=rva<va+sz:return raw+rva-va
 raise ValueError(hex(rva))
rva,size=struct.unpack_from('<II',b,op+112+3*8);start=offset(rva);rows=[]
for address in sys.argv[1:]:
 pc=int(address,16);pc=pc-0x140000000 if pc>=0x140000000 else pc
 matches=[]
 for p in range(start,start+size,12):
  begin,end,unwind=struct.unpack_from('<III',b,p)
  if begin<=pc<end:matches.append(dict(begin=hex(0x140000000+begin),end=hex(0x140000000+end),unwindRva=hex(unwind)))
 rows.append(dict(address=hex(0x140000000+pc),ranges=matches))
print(json.dumps(rows,indent=2))
