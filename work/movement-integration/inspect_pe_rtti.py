"""Read-only MSVC x64 RTTI extraction from the pinned D2 image; no process access."""
from pathlib import Path
import argparse,struct,json,hashlib
p=argparse.ArgumentParser();p.add_argument('type');p.add_argument('output');a=p.parse_args()
root=Path(__file__).resolve().parents[2];exe=root/'work/asset-game/Dishonored2.exe';blob=exe.read_bytes()
digest=hashlib.sha256(blob).hexdigest().upper()
assert digest=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180','Wrong executable'
u16=lambda n:struct.unpack_from('<H',blob,n)[0];u32=lambda n:struct.unpack_from('<I',blob,n)[0];u64=lambda n:struct.unpack_from('<Q',blob,n)[0]
nt=u32(0x3c);assert blob[nt:nt+4]==b'PE\0\0';opt=nt+24;assert u16(opt)==0x20b
base=u64(opt+24);sections=[]
for i in range(u16(nt+6)):
 h=opt+u16(nt+20)+i*40
 sections.append(dict(name=blob[h:h+8].rstrip(b'\0').decode(),virtualSize=u32(h+8),rva=u32(h+12),size=u32(h+16),offset=u32(h+20),flags=u32(h+36)))
def rva(offset):
 for s in sections:
  if s['offset']<=offset<s['offset']+s['size']:return s['rva']+offset-s['offset']
 raise ValueError('Offset outside sections')
def executable(address):
 v=address-base
 return any(s['flags']&0x20000000 and s['rva']<=v<s['rva']+s['virtualSize'] for s in sections)
def occurrences(needle):
 start=0
 while (start:=blob.find(needle,start))>=0:
  yield start;start+=1
if a.type.startswith('0x'):
 def raw(address):
  for s in sections:
   if s['rva']<=address<s['rva']+s['size']:return s['offset']+address-s['rva']
  raise ValueError('RVA outside file sections')
 vt=int(a.type,16);col=u64(raw(vt-8))-base;type_rva=u32(raw(col+12));name_start=raw(type_rva+16)
 decorated=blob[name_start:blob.index(b'\0',name_start)].decode()
 assert decorated.startswith(('.?AV','.?AU')) and decorated.endswith('@@'),decorated
 a.type=decorated[4:-2]
names=[]
for prefix in ('.?AV','.?AU'):
 for off in occurrences((prefix+a.type+'@@\0').encode()):names.append((off,prefix))
assert len(names)==1,(a.type,names)
nameOff,prefix=names[0];td=rva(nameOff-16);locators=[];tables=[]
for reference in occurrences(struct.pack('<I',td)):
 off=reference-12
 if off<0 or off+24>len(blob) or u32(off)!=1:continue
 try:at=rva(off)
 except ValueError:continue
 if u32(off+20)!=at:continue
 locator=dict(rva=hex(at),offset=u32(off+4),constructionDisplacement=u32(off+8),hierarchy=hex(u32(off+16)))
 locators.append(locator)
 for pointer in occurrences(struct.pack('<Q',base+at)):
  try:vtable=rva(pointer+8)
  except ValueError:continue
  functions=[]
  for slot in range(128):
   pos=pointer+8+slot*8
   if pos+8>len(blob):break
   address=u64(pos)
   if not executable(address):break
   functions.append(dict(slot=hex(slot*8),rva=hex(address-base),address=hex(address)))
  if functions:tables.append(dict(vtable=hex(vtable),locator=hex(at),offset=locator['offset'],functions=functions))
assert locators and tables,'RTTI relationships not found'
out=Path(a.output).resolve();assert out.is_relative_to(root);assert not out.exists(),'Preserve previous findings'
out.write_text(json.dumps(dict(executable=str(exe),sha256=digest,type=a.type,typeRva=hex(td),locators=locators,tables=tables,scope='Confirmed static MSVC RTTI relationships in exact executable. No runtime identity, safety or behavior claim.'),indent=2),encoding='utf-8')
print(json.dumps(dict(type=a.type,typeRva=hex(td),tables=[dict(vtable=t['vtable'],offset=t['offset'],functions=len(t['functions']))for t in tables],output=str(out))))
