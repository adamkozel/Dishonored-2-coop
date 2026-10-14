"""Read x64 PE exception-directory boundaries; never modify the executable."""
import struct,sys,json
from pathlib import Path
p=Path(__file__).resolve().parents[1]/'asset-game'/'Dishonored2.exe'
b=p.read_bytes();pe=struct.unpack_from('<I',b,0x3c)[0]
assert b[pe:pe+4]==b'PE\0\0'
sections=struct.unpack_from('<H',b,pe+6)[0];opt=pe+24;size=struct.unpack_from('<H',b,pe+20)[0]
assert struct.unpack_from('<H',b,opt)[0]==0x20b
table=opt+size
def offset(rva):
    for i in range(sections):
        virtual,base,raw_size,raw=struct.unpack_from('<IIII',b,table+i*40+8)
        if base<=rva<base+max(virtual,raw_size):return raw+rva-base
    raise ValueError(hex(rva))
rva,n=struct.unpack_from('<II',b,opt+112+3*8);data=offset(rva)
for address in sys.argv[1:]:
    a=int(address,16);a=a-0x140000000 if a>=0x140000000 else a
    found=[dict(address=hex(a),begin=hex(start),end=hex(end),unwind=hex(unwind)) for start,end,unwind in struct.iter_unpack('<III',b[data:data+n]) if start<=a<end]
    print(json.dumps(found))
