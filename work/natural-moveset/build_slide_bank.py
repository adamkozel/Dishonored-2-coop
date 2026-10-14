"""Append eventless slide curves; native gameplay owns travel and outcomes."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from mathutils import Quaternion
def blend(a,b,t):
    out=[]
    for x,y in zip(a,b):
        q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t).normalized()
        out.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
    return out
OUT=P/'slide-banks-v1';OUT.mkdir(exist_ok=True)
rows=json.loads((P/'dynamics-retarget-manifest-v3.json').read_text());reports=[]
for role in ('corvo','emily'):
    data=(P/'lean-banks-v1'/f'{role}-lean-bank.bin').read_bytes()
    magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert magic==b'D2MOVE05' and count==33
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE06',6,n,37,fps,layout)+data[32:]);sources=[]
    for cid,alias in enumerate(('slide_enter','slide_loop','slide_exit_to_run','slide_to_crouch'),33):
        row=next(r for r in rows if r['role']==role and r['selection']['alias']==alias)
        raw=(P/row['binary']).read_bytes();assert hashlib.sha256(raw).hexdigest()==row['sha256']
        tag,joints,frames,source_fps,source_layout=struct.unpack_from('<8sIIfQ',raw)
        assert tag==b'D2NATV01' and joints==n and layout==source_layout
        fs=[[list(struct.unpack_from('<7f',raw,28+(f*n+j)*28))for j in range(n)]for f in range(frames)]
        poses=[]
        for f in range(120):
            t=(frames-1)*f/119;a=int(t);poses.append(blend(fs[a],fs[min(a+1,frames-1)],t-a))
        poses.append(poses[0]);duration=(frames-1)/source_fps
        bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration))
        for pose in poses:
            for v in pose:bank.extend(struct.pack('<7f',*v))
        sources.append(dict(id=cid,alias=alias,source=row['binary'],sha256=row['sha256'],duration=duration,sampling='0..119 full authored sequence; endpoint clamp, loop only for slide_loop'))
    path=OUT/f'{role}-slide-bank.bin';path.write_bytes(bank)
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE06',previous_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,root_writes=False,events=False,gameplay_verified=False,visual_accepted=False))
(P/'slide-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2));print('SLIDE_BANKS prepared, runtime validation pending')
