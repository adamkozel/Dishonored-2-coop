"""Read-only Edge skeleton metadata and native-sampled poses, exact D2 format."""
from pathlib import Path
import struct, hashlib, re

def skeleton(path):
    b=Path(path).read_bytes()
    def u16(o): return struct.unpack_from('<H',b,o)[0]
    def u32(o): return struct.unpack_from('<I',b,o)[0]
    assert b[:4]==b'60SE' and u32(4)==len(b)
    count=u16(0x10);assert 0<count<=512
    p=0x30+u32(0x30)
    def take(fmt):
        nonlocal p
        values=struct.unpack_from('<'+fmt,b,p);p+=struct.calcsize('<'+fmt)
        return values[0] if len(values)==1 else values
    def string():
        nonlocal p
        n=take('I');assert n<4096 and p+n<=len(b)
        s=b[p:p+n].decode('utf-8');p+=n;return s
    version=take('I');inherit=string()
    if inherit:take('H')
    tags=take('HH');declared,levels=take('HH');assert declared==count and 0<=levels<=4
    if levels:
        take('H'*levels);take('H'*count)
    names=[string() for _ in range(count)]
    assert len(set(names))==count and all(re.fullmatch(r'[A-Za-z0-9_]+',n) for n in names)
    bind_at=0x18+u32(0x18);parent_map=0x1c+u32(0x1c)
    joints=[]
    for i,name in enumerate(names):
        mapped=u16(parent_map+i*2);parent=u16(0x42+mapped*4)&0x7fff
        parent=-1 if parent>=0x4000 else parent
        assert parent<i
        joints.append(dict(nativeName=name,parent=parent,bindLocalQTS=list(struct.unpack_from('<12f',b,bind_at+i*48))))
    return dict(joints=joints,sha256=hashlib.sha256(b).hexdigest(),version=version,inherit=inherit,tags=tags)

def poses(path):
    b=Path(path).read_bytes();assert b[:8]==b'D2EDGE01'
    joints,count,fps,reserved=struct.unpack_from('<IIfI',b,8)
    assert 0<joints<=512 and 0<count<=18000 and 1<=fps<=240 and not reserved
    assert len(b)==24+joints*count*48
    frames=[]
    for f in range(count):
        frames.append([list(struct.unpack_from('<12f',b,24+(f*joints+j)*48)) for j in range(joints)])
    return dict(joints=joints,frames=frames,fps=fps,sha256=hashlib.sha256(b).hexdigest())

if __name__=='__main__':
    import json,sys
    result=skeleton(sys.argv[1]);Path(sys.argv[2]).write_text(json.dumps(result,indent=2))
    print('Skeleton:',len(result['joints']),'joints; sha256',result['sha256'])
