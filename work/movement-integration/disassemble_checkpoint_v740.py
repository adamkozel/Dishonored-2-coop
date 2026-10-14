from pathlib import Path
import sys,struct,hashlib,json
R=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(R/'work/tools/capstone-5.0.6-local'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_64
p=R/'work/asset-game/Dishonored2.exe';data=p.read_bytes()
sha=hashlib.sha256(data).hexdigest().upper()
assert sha=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
pe=struct.unpack_from('<I',data,0x3c)[0];sections=struct.unpack_from('<H',data,pe+6)[0];opt=struct.unpack_from('<H',data,pe+20)[0]
def section(rva,length):
    for i in range(sections):
        at=pe+24+opt+i*40;vs,va,rawsize,raw=struct.unpack_from('<IIII',data,at+8)
        if va<=rva and rva+length<=va+rawsize:return data[raw+rva-va:raw+rva-va+length]
    raise ValueError('Unmapped RVA')
rows=[]
for rva,length in [(0x65db00,0x70),(0x659190,0x80),(0x6597ca,0x1d5)]:
    blob=section(rva,length)
    lines=[f'{i.address:x} {i.bytes.hex()} {i.mnemonic} {i.op_str}' for i in Cs(CS_ARCH_X86,CS_MODE_64).disasm(blob,0x140000000+rva)]
    rows.append(dict(rva=hex(rva),hex=blob.hex(),instructions=lines))
out=dict(exe=str(p),sha256=sha,capstone='5.0.6',rows=rows)
(R/'work/movement-integration/checkpoint-continuation-v740/disk-disassembly.json').write_text(json.dumps(out,indent=2))
for row in rows:
    print('\n'.join(row['instructions']))
