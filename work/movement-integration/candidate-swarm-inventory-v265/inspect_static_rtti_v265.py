"""Read-only MSVC x64 RTTI table lookup from the pinned D2 PE file.

Match the type descriptor, self-referential complete object locator and pointer
to that locator. This discovers candidates, never authorizes runtime writes.
"""
import argparse,hashlib,json,struct
from pathlib import Path
R=Path(__file__).resolve().parents[2]
a=argparse.ArgumentParser();a.add_argument('type');a.add_argument('output');args=a.parse_args()
exe=R/'work/asset-game/Dishonored2.exe';data=exe.read_bytes()
assert hashlib.sha256(data).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
u16=lambda o:struct.unpack_from('<H',data,o)[0]
u32=lambda o:struct.unpack_from('<I',data,o)[0]
u64=lambda o:struct.unpack_from('<Q',data,o)[0]
pe=u32(0x3c);assert data[pe:pe+4]==b'PE\0\0';n=u16(pe+6);opt=pe+24;assert u16(opt)==0x20b;base=u64(opt+24)
sections=[]
for i in range(n):
 o=opt+u16(pe+20)+i*40
 sections.append(dict(name=data[o:o+8].rstrip(b'\0').decode(),virtualSize=u32(o+8),rva=u32(o+12),size=u32(o+16),raw=u32(o+20),flags=u32(o+36)))
def rva(offset):
 for s in sections:
  if s['raw']<=offset<s['raw']+s['size']:return s['rva']+offset-s['raw']
 raise ValueError('Outside sections')
def executable(address):
 v=address-base
 return any(s['flags']&0x20000000 and s['rva']<=v<s['rva']+s['virtualSize'] for s in sections)
def hits(pattern):
 o=data.find(pattern)
 while o>=0:
  yield o;o=data.find(pattern,o+1)
name=(' .?AV'+args.type+'@@\0').strip().encode();names=list(hits(name));assert len(names)==1,names
td=rva(names[0]-16);locators=[];tables=[]
for hit in hits(struct.pack('<I',td)):
 o=hit-12
 if o<0 or o+24>len(data):continue
 signature,offset,cdOffset,typeRva,hierarchy,selfRva=struct.unpack_from('<6I',data,o)
 if signature!=1 or selfRva!=rva(o) or typeRva!=td:continue
 locators.append(dict(rva=hex(selfRva),offset=offset,constructionDisplacement=cdOffset,hierarchy=hex(hierarchy)))
 for p in hits(struct.pack('<Q',base+selfRva)):
  start=p+8
  if start+8>len(data)or not executable(u64(start)):continue
  funcs=[]
  for slot in range(0,0x300,8):
   address=u64(start+slot)
   if not executable(address):break
   funcs.append(dict(slot=hex(slot),rva=hex(address-base),address=hex(address)))
  tables.append(dict(vtable=hex(rva(start)),locator=hex(selfRva),offset=offset,functions=funcs))
out=dict(executable=str(exe),sha256=hashlib.sha256(data).hexdigest().upper(),type=args.type,typeRva=hex(td),locators=locators,tables=tables,scope='Confirmed static RTTI/table candidates from exact PE; no runtime component ownership or safe mutation implied.')
dest=Path(args.output);dest.write_text(json.dumps(out,indent=2));print(json.dumps(dict(type=args.type,typeRva=out['typeRva'],tables=[dict(vtable=t['vtable'],offset=t['offset'],slots=len(t['functions']))for t in tables])))
