"""Read exact PE unwind boundaries for targeted Ghidra function creation."""
import hashlib,struct,sys,json
from pathlib import Path
p=Path(__file__).resolve().parents[1]/'asset-game/Dishonored2.exe';data=p.read_bytes()
assert hashlib.sha256(data).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
pe=struct.unpack_from('<I',data,0x3c)[0];assert data[pe:pe+4]==b'PE\0\0'
count=struct.unpack_from('<H',data,pe+6)[0];opt=struct.unpack_from('<H',data,pe+20)[0]
section=pe+24+opt;base=struct.unpack_from('<Q',data,pe+24+24)[0]
for i in range(count):
    s=section+40*i
    if data[s:s+8].rstrip(b'\0')!=b'.pdata':continue
    size,raw=struct.unpack_from('<II',data,s+16);entries=[struct.unpack_from('<III',data,j)for j in range(raw,raw+size-11,12)]
    break
else:raise ValueError('No unwind directory section')
for address in sys.argv[1:]:
    v=int(address,16);rva=v-base if v>=base else v
    print(json.dumps(dict(query=hex(v),matches=[dict(start=hex(base+a),end=hex(base+b),unwind=hex(base+c))for a,b,c in entries if a<=rva<b])))
