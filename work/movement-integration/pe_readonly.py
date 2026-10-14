"""Exact-build PE metadata inspection. Reads bytes only; never loads or executes them."""
import argparse,hashlib,json,struct
from pathlib import Path
EXPECTED='b671f8249422862bf557d00aab2f4bb671ed4515cc10ad8a8a9decad84516180'
EXE=Path(__file__).resolve().parent.parent/'asset-game/Dishonored2.exe'
def inspect(type_name=None,address=None):
 b=EXE.read_bytes();assert hashlib.sha256(b).hexdigest()==EXPECTED
 def u32(at):return struct.unpack_from('<I',b,at)[0]
 pe=u32(0x3c);assert b[pe:pe+4]==b'PE\0\0'
 opt=pe+24;assert struct.unpack_from('<H',b,opt)[0]==0x20b
 base=struct.unpack_from('<Q',b,opt+24)[0]
 count=struct.unpack_from('<H',b,pe+6)[0];sec=opt+struct.unpack_from('<H',b,pe+20)[0]
 sections=[struct.unpack_from('<8sIIII',b,sec+i*40)for i in range(count)]
 def rva(off):
  for name,vsize,va,size,ptr in sections:
   if ptr<=off<ptr+size:return va+off-ptr
  raise ValueError('Not mapped file offset')
 def off(va):
  for name,vsize,start,size,ptr in sections:
   if start<=va<start+size:return ptr+va-start
  raise ValueError('Not file-backed RVA')
 def occurrences(needle):
  start=0
  while True:
   at=b.find(needle,start)
   if at<0:return
   yield at;start=at+1
 out=dict(executable_sha256=EXPECTED,image_base=hex(base))
 if type_name:
  hits=[]
  for found in occurrences((' .?AV'+type_name+'@@\0').lstrip().encode('ascii')):
   td=rva(found)-16
   for ref in occurrences(struct.pack('<I',td)):
    col=ref-12
    if col<0:continue
    try:crva=rva(col)
    except ValueError:continue
    if u32(col)!=1 or u32(col+20)!=crva:continue
    tables=[]
    for v in occurrences(struct.pack('<Q',base+crva)):
     try:tables.append(hex(base+rva(v+8)))
     except ValueError:pass
    hits.append(dict(type_descriptor=hex(base+td),complete_object_locator=hex(base+crva),object_offset=u32(col+4),vtable_candidates=tables))
  out['rtti']=hits
 if address:
  target=int(address,16)-base;directory,size=struct.unpack_from('<II',b,opt+112+3*8);entries=[]
  start=off(directory)
  for i in range(size//12):
   begin,end,unwind=struct.unpack_from('<III',b,start+i*12)
   if begin<=target<end:entries.append(dict(begin=hex(base+begin),end=hex(base+end),unwind=hex(base+unwind)))
  out['runtime_functions']=entries
 return out
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--type');p.add_argument('--address');a=p.parse_args();print(json.dumps(inspect(a.type,a.address),indent=2))
