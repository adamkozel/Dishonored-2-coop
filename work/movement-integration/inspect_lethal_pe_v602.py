"""Use the established exact-PE parser for bounded function boundaries."""
from pathlib import Path
import json,struct
R=Path(__file__).resolve().parents[2];O=R/'work/natural-moveset/native-lethal-research-v602';O.mkdir(exist_ok=True)
p=R/'work/inspect_potion_code_v64.py';ns={'__file__':str(p)};exec(p.read_text().split('matches=[]')[0],ns)
raw=ns['raw'];tableRva,tableSize=struct.unpack_from('<II',raw,ns['opt']+112+3*8);table=ns['offset'](tableRva);functions=[]
for at in range(table,table+tableSize,12):
 b,e,u=struct.unpack_from('<III',raw,at)
 if b<=0x863799<e:functions.append(dict(begin=hex(0x140000000+b),end=hex(0x140000000+e),unwind=hex(0x140000000+u)))
vt=ns['offset'](0x1c83468);slots=[]
for i in range(25):
 address=struct.unpack_from('<Q',raw,vt+i*8)[0]
 if not 0x140000000<=address<0x1441ca800:break
 slots.append(dict(slot=hex(i*8),address=hex(address)))
result=dict(executableSha256='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180',nativeState13Vtable='0x141c83468',stringXrefs=['0x140863799','0x1408637b5'],functions=functions,vtableSlots=slots,scope='Exact PE exception directory boundaries and raw native table. Slot semantics need Ghidra/runtime evidence.')
(O/'pe-boundaries.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
