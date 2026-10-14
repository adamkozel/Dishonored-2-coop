"""Find static E8 candidates and their PE unwind ranges; Ghidra must confirm calls."""
import argparse
import bisect
import hashlib
import json
import struct
from pathlib import Path

root=Path(__file__).resolve().parent
parser=argparse.ArgumentParser()
parser.add_argument('rva',type=lambda s:int(s,0))
ns=parser.parse_args()
b=(root/'input/Dishonored2.exe').read_bytes()
assert hashlib.sha256(b).hexdigest()=='b671f8249422862bf557d00aab2f4bb671ed4515cc10ad8a8a9decad84516180'
u16=lambda p:struct.unpack_from('<H',b,p)[0]
u32=lambda p:struct.unpack_from('<I',b,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20)
sections=[]
for i in range(u16(pe+6)):
    p=table+40*i
    sections.append((b[p:p+8].split(b'\0')[0].decode(),u32(p+12),u32(p+16),u32(p+20),u32(p+36)))
def raw(rva):
    for _,start,size,offset,_ in sections:
        if start<=rva<start+size:return offset+rva-start
    raise ValueError(hex(rva))
exceptions=u32(opt+112+3*8);size=u32(opt+112+3*8+4)
ranges=[struct.unpack_from('<III',b,raw(exceptions)+n)[:2] for n in range(0,size,12)]
ranges.sort();starts=[r[0] for r in ranges]
found=[]
for name,start,size,offset,flags in sections:
    if not flags&0x20000000:continue
    data=b[offset:offset+size];at=-1
    while True:
        at=data.find(b'\xe8',at+1)
        if at<0:break
        if at+5>len(data):continue
        source=start+at
        if source+5+struct.unpack_from('<i',data,at+1)[0]!=ns.rva:continue
        index=bisect.bisect_right(starts,source)-1
        bounds=ranges[index] if index>=0 and ranges[index][0]<=source<ranges[index][1] else None
        found.append({'callRva':hex(source),'unwindStart':hex(bounds[0]) if bounds else None,'unwindEnd':hex(bounds[1]) if bounds else None})
report={'targetRva':hex(ns.rva),'status':'static-byte candidates, not yet instruction-verified','callers':found}
path=root/f'call-candidates-{ns.rva:x}.json';path.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
